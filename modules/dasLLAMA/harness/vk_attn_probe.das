options gen2
options stack = 524288
options _dasllama_internal = true

//! The decode attention ruler: the token command's attention pass (`DaAttnT`, the f16 mirror or a block
//! codec's) over a synthetic llama-shaped layer, timed per layer across the key-split ladder, so
//! a kernel change reads as a number before a model runs it.
//!
//!   bin/daslang -jit modules/dasLLAMA/harness/vk_attn_probe.das -- [rows] [cnt] [hs] [nheads] [kv_mul] [slab] [f16 | q8_0 | tq4]
//!
//! Defaults: 4 rows, 640 positions, head 64, 32 heads, 4 heads a kv head (Llama-3.2-1B after
//! pp512 and tg128), the slab the token command picks at that group width (`da_slab_heads`: two
//! on a group of one or two heads, else four); `slab` 2 or 4 pins a stamp, so the two race in one
//! process. Each row attends its own mirror region. Every split of the ladder records
//! DISPATCHES layers chained through their outputs, and
//! the device clock over SUBMITS submits after a warm-up is the layer's cost.

require strings
require math
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_kv_dtype   // KVDtype, the block codecs' bytes a block
require _vk_probe_fixture   // hash_word, cool_f16_pair: the synthetic planes the probes build; pfq_arm, pfq_avg_us: the device clock

let private DISPATCHES = 16   //! layers a command
let private SUBMITS = 10

def private hash_unit(i : uint) : float => float(hash_word(i) & 0xFFFFu) / 32768.0 - 1.0

//! a block codec's synthetic mirror: every block a small finite scale then hashed quant bytes
def private cool_block_bytes(nblocks, blk : int) : array<uint8> {
    var out : array<uint8>
    out |> resize(nblocks * blk)
    for (b in range(nblocks)) {
        out[b * blk] = 0x66u8        // the half 0x2E66 ~ 0.1
        out[b * blk + 1] = 0x2Eu8
        for (k in range(2, blk)) {
            out[b * blk + k] = uint8(hash_word(uint(b * blk + k)) & 255u)
        }
    }
    return <- out
}

//! one split's layer cost in microseconds on the `slab`-head stamp over a mirror in codec `dt`
def private time_split(rows, cnt, hs, nheads, kv_mul, nsplit, slab : int; dt : KVDtype) : double {
    let kvd = nheads / kv_mul * hs
    let qd = nheads * hs
    let q_bytes = int64(qd) * int64(rows) * 4l
    let blk = dt == KVDtype.q8_0 ? int(KV_Q8_BLOCK_BYTES) : (dt == KVDtype.tq4 ? int(KV_TQ4_BLOCK_BYTES) : 0)
    let mir_elems = cnt * kvd   //! a row's region in elements
    let mir_units = blk > 0 ? cnt * (kvd / 32) : mir_elems   //! ... and in the mirror's units: blocks under a block codec
    let m_bytes = blk > 0 ? int64(mir_units * rows) * int64(blk) : int64(mir_elems * rows) * 2l
    let cnt_off = nheads * nsplit * (hs + 2) * rows   //! the arrival counters past the pieces, a word a (row, kv head, slab) group
    let p_bytes = (int64(cnt_off) + da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l) * int64(rows)) * 4l
    let s_bytes = int64(qd / 32 * rows) * 4l
    var qh : array<float>
    qh |> resize(qd * rows)
    for (i in range(qd * rows)) {
        qh[i] = hash_unit(uint(i))
    }
    var kw, vw : array<uint>
    var inscope kb <- blk > 0 ? cool_block_bytes(mir_units * rows, blk) : array<uint8>()
    var inscope vb <- blk > 0 ? cool_block_bytes(mir_units * rows, blk) : array<uint8>()
    if (blk == 0) {
        kw |> resize(mir_elems * rows / 2)
        vw |> resize(mir_elems * rows / 2)
        for (i in range(mir_elems * rows / 2)) {
            kw[i] = cool_f16_pair(uint(i))
            vw[i] = cool_f16_pair(uint(i) + 7919u)
        }
    }
    let qdev = make_device_buf(q_bytes)
    let kd = make_device_buf(m_bytes)
    let vd = make_device_buf(m_bytes)
    let yd = make_device_buf(q_bytes)
    let partd = make_device_buf(p_bytes)
    let aqd = make_device_buf(int64(qd * rows))
    let asd = make_device_buf(s_bytes)
    var tokb = make_host_buf(TOK_META_BYTES * int64(rows), true)
    var raw = alloc_cmd()
    unsafe {
        upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
        if (blk > 0) {
            upload_region_at(kd, 0l, addr<uint8 const?>(kb[0]), m_bytes)
            upload_region_at(vd, 0l, addr<uint8 const?>(vb[0]), m_bytes)
        } else {
            upload_region_at(kd, 0l, addr<uint8 const?>(kw[0]), m_bytes)
            upload_region_at(vd, 0l, addr<uint8 const?>(vw[0]), m_bytes)
        }
        upload_zeros(partd, int64(cnt_off) * 4l, da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l) * int64(rows) * 4l)
        var tm = reinterpret<TokMeta?>(tokb.mapped)
        for (r in range(rows)) {
            tm[r].pos = uint(cnt - 1)
            tm[r].cnt = uint(cnt)
            tm[r].parity = 0u
            tm[r].mirbase = uint(r * mir_units)
        }
        let dslab = slab == 2 ? DaSlab.two : DaSlab.four
        var sc = da_attn_stamp_set(dt, dslab, fixed_array(qdev, kd, vd, yd, tokb.buf, partd, aqd, asd, asd),
            fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, TOK_META_BYTES * int64(rows), p_bytes, int64(qd * rows), s_bytes, s_bytes),
            fixed_array(1u, 2u, 4u, 16u, 0u, 32u, 64u, 64u, 64u))
        var pc = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u,
            scale = 1.0 / sqrt(float(hs)), qstride = uint(hs), nsplit = uint(nsplit), rqk = DA_RQ_Q8,
            rowwg = uint(da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit))), qrow = uint(qd), cntoff = uint(cnt_off))
        let wgs = da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit)) * int64(rows)
        record_cmd(raw, false) $(r; var h) {
            pfq_arm(r)   //! device timestamps around every layer: the host's submit and fence latency stay out of the number
            pfq_ts(r)
            for (_i in range(DISPATCHES)) {
                da_attn_stamp_enc(dt, dslab, r, h, sc, pc, wgs)
                vhz_dep(r, h, 16u | 32u | 64u, 0u, true)   //! the next layer waits on this one's row
                pfq_ts(r)
            }
            pfq_end(r)
        }
    }
    return pfq_avg_us(raw, SUBMITS, DISPATCHES)
}

[export]
def main : int {
    let args <- get_command_line_arguments()
    let nums <- [for (a in args); to_int(a); where to_int(a) > 0]
    let rows = !empty(nums) ? nums[0] : 4
    let cnt = length(nums) > 1 ? nums[1] : 640
    let hs = length(nums) > 2 ? nums[2] : 64
    let nheads = length(nums) > 3 ? nums[3] : 32
    let kv_mul = length(nums) > 4 ? nums[4] : 4
    let slab = length(nums) > 5 ? nums[5] : int(da_slab_heads(int64(kv_mul)))
    var dt = KVDtype.f16
    for (a in args) {
        if (a == "q8_0") {
            dt = KVDtype.q8_0
        } elif (a == "tq4") {
            dt = KVDtype.tq4
        }
    }
    if (slab != 2 && slab != 4) {
        print("vk_attn_probe: the slab is 2 or 4 heads\n")
        return 1
    }
    if (slab == 2 && kv_mul > 2) {
        print("vk_attn_probe: the two-head slab serves a group of one or two heads\n")
        return 1
    }
    if (!vk_moe_init() || !da_attn_stamp_ensure(dt, DaSlab.four) || (slab == 2 && !da_attn_stamp_ensure(dt, DaSlab.two))) {   //! the two-head stamp binds through the four-head set
        print("vk_attn_probe: no Vulkan device, or the decode attention classes declined\n")
        return 1
    }
    print("vk_attn_probe: decode attention ({dt} mirror, the {slab}-head slab) at {rows} rows x {cnt} positions, head {hs}, {nheads} heads, {kv_mul} a kv head, {g_gpu.sm_count} SMs; {DISPATCHES} layers x {SUBMITS} submits after a warm-up\n")
    print("| split | wgs/row |  us/layer |\n")
    print("| ----: | ------: | --------: |\n")
    for (nsplit in [1, 2, 3, 4, 6, 8, 12, 16]) {
        let wgs = da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit))
        let us = time_split(rows, cnt, hs, nheads, kv_mul, nsplit, slab, dt)
        print("| {nsplit:5d} | {wgs:7d} | {us:9.1f} |\n")
    }
    return 0
}
