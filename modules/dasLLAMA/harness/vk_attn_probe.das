options gen2
options stack = 524288
options _dasllama_internal = true

//! The decode attention ruler: the token command's attention pass (`DaAttnT`, the f16 mirror) over
//! a synthetic llama-shaped layer, timed per layer across the key-split ladder, so
//! a kernel change reads as a number before a model runs it.
//!
//!   bin/daslang -jit modules/dasLLAMA/harness/vk_attn_probe.das -- [rows] [cnt] [hs] [nheads] [kv_mul]
//!
//! Defaults: 4 rows, 640 positions, head 64, 32 heads, 4 heads a kv head (Llama-3.2-1B after
//! pp512 and tg128). Each row attends its own mirror region. Every split of the ladder records
//! DISPATCHES layers chained through their outputs, and
//! the device clock over SUBMITS submits after a warm-up is the layer's cost.

require strings
require math
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require _vk_probe_fixture   // hash_word, cool_f16_pair: the synthetic planes the probes build

let private DISPATCHES = 16   //! layers a command
let private SUBMITS = 10

def private hash_unit(i : uint) : float => float(hash_word(i) & 0xFFFFu) / 32768.0 - 1.0

//! one split's layer cost in microseconds
def private time_split(rows, cnt, hs, nheads, kv_mul, nsplit : int) : double {
    let kvd = nheads / kv_mul * hs
    let qd = nheads * hs
    let q_bytes = int64(qd) * int64(rows) * 4l
    let mir_elems = cnt * kvd   //! a row's region
    let m_bytes = int64(mir_elems * rows) * 2l
    let cnt_off = nheads * nsplit * (hs + 2) * rows   //! the arrival counters past the pieces, a word a (row, kv head, slab) group
    let p_bytes = (int64(cnt_off) + da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l) * int64(rows)) * 4l
    let s_bytes = int64(qd / 32 * rows) * 4l
    var qh : array<float>
    qh |> resize(qd * rows)
    for (i in range(qd * rows)) {
        qh[i] = hash_unit(uint(i))
    }
    var kw : array<uint>
    var vw : array<uint>
    kw |> resize(mir_elems * rows / 2)
    vw |> resize(mir_elems * rows / 2)
    for (i in range(mir_elems * rows / 2)) {
        kw[i] = cool_f16_pair(uint(i))
        vw[i] = cool_f16_pair(uint(i) + 7919u)
    }
    let qd_ = make_device_buf(q_bytes)
    let kd = make_device_buf(m_bytes)
    let vd = make_device_buf(m_bytes)
    let yd = make_device_buf(q_bytes)
    let partd = make_device_buf(p_bytes)
    let aqd = make_device_buf(int64(qd * rows))
    let asd = make_device_buf(s_bytes)
    var tokb = make_host_buf(16l * int64(rows), true)
    var raw = alloc_cmd()
    unsafe {
        upload_region_at(qd_, 0l, addr<uint8 const?>(qh[0]), q_bytes)
        upload_region_at(kd, 0l, addr<uint8 const?>(kw[0]), m_bytes)
        upload_region_at(vd, 0l, addr<uint8 const?>(vw[0]), m_bytes)
        var zeros : array<uint>
        zeros |> resize(int(da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l)) * rows)
        upload_region_at(partd, int64(cnt_off) * 4l, addr<uint8 const?>(zeros[0]), long_length(zeros) * 4l)
        var tm = reinterpret<TokMeta?>(tokb.mapped)
        for (r in range(rows)) {
            tm[r].pos = uint(cnt - 1)
            tm[r].cnt = uint(cnt)
            tm[r].parity = 0u
            tm[r].mirbase = uint(r * mir_elems)
        }
        var sc = set_da_attn_f16_cls(fixed_array(qd_, kd, vd, yd, tokb.buf, partd, aqd, asd),
            fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l * int64(rows), p_bytes, int64(qd * rows), s_bytes),
            fixed_array(1u, 2u, 4u, 16u, 0u, 32u, 64u, 64u))
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        ensure_pfq()   //! device timestamps around every layer: the host's submit and fence latency stay out of the number
        g_pfq_on = true
        g_pfq_n = 0u
        cmd_reset_query_pool(vk_value_to_boost(raw), g_pfq_pool, 0u, PFQ_CAP)
        var h : VkHaz
        var pc = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u,
            scale = 1.0 / sqrt(float(hs)), qstride = uint(hs), nsplit = uint(nsplit), rqk = DA_RQ_Q8,
            rowwg = uint(da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit))), qrow = uint(qd), cntoff = uint(cnt_off))
        let wgs = da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit)) * int64(rows)
        pfq_ts(raw)
        for (_i in range(DISPATCHES)) {
            enc_da_attn_f16_cls(raw, h, sc, pc, wgs)
            vhz_dep(raw, h, 16u | 32u | 64u, 0u, true)   //! the next layer waits on this one's row
            pfq_ts(raw)
        }
        pfq_end(raw)
        vk_check(vkEndCommandBuffer(raw), null)
    }
    submit_wait(raw)   //! the warm-up: pipeline compile, first-touch
    var us = 0.0lf
    for (_i in range(SUBMITS)) {
        submit_wait(raw)
        for (i in range(DISPATCHES)) {
            us += pfq_us(uint(i + 1))
        }
    }
    return us / double(SUBMITS * DISPATCHES)
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
    if (!vk_moe_init() || !ensure_da_attn_f16_cls()) {
        print("vk_attn_probe: no Vulkan device, or the decode attention classes declined\n")
        return 1
    }
    print("vk_attn_probe: decode attention (f16 mirror) at {rows} rows x {cnt} positions, head {hs}, {nheads} heads, {kv_mul} a kv head, {g_gpu.sm_count} SMs; {DISPATCHES} layers x {SUBMITS} submits after a warm-up\n")
    print("| split | wgs/row |  us/layer |\n")
    print("| ----: | ------: | --------: |\n")
    for (nsplit in [1, 2, 3, 4, 6, 8, 12, 16]) {
        let wgs = da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit))
        let us = time_split(rows, cnt, hs, nheads, kv_mul, nsplit)
        print("| {nsplit:5d} | {wgs:7d} | {us:9.1f} |\n")
    }
    return 0
}
