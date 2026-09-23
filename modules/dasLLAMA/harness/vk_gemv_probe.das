options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - isolated per-format rates of the decode GEMV classes at one served shape:
// synthetic planes per kq format at [n x d], the shipped kq_gemv class dispatched over enough
// regions of d rows that one dispatch streams STREAM_MB or more (the decode reads a layer's
// planes once per token, so the rate wanted is the DRAM one, not an L2 replay of a small plane),
// timed over repeated submits with a dependency between dispatches. Prints the bytes a dispatch
// reads, its wall per dispatch and the GB/s - the number llama.cpp's per-type mat-vec rates
// (test-backend-ops perf -o MUL_MAT, the m=4096 n=1 k=14336 rows) are read against.
// Args: <n> <d> (default 4096 12288, the Qwen3.5-9B FFN gate; 5120 17408 is the 27B's); `single` dispatches ONE
// plane copy - the served size, L2-warm across the chain - so the row reads the dispatch's own floor past its
// bytes at the streaming rate (the decode's small planes pay it once per GEMV).
// ATTRIBUTION SWEEP - the `cols` mode: the N-column ruler, every format the batched decode step serves (q8 and the
// thirteen kq leaves) at their served lanes, the one-column class beside the N-column class at 1, 2, 4 and 8 activation rows,
// each dispatch's wall attributed to its class and width; the last column is the N-column dispatch's wall over N
// one-column dispatches - under 1 the weight pass is shared, at 1 it is not. The chain alternates two output planes,
// so consecutive dispatches overlap as the batched step's do. The q8 rows engage the row-pair stamps
// (`g_q8_n2_min_n` at 0 for the run); `n2off` holds them to the one-row-a-subgroup form.
// ATTRIBUTION SWEEP - the `cold` mode: ONE plane copy a dispatch, the copies in turn - the served launch size in
// the DRAM regime, the token command's own - each row attributing a format's wall at that launch size; `single`
// replays one copy L2-warm and `cold` yields to it.

require strings
require math
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require _vk_probe_fixture   // hash_word, cool_f16_pair: the synthetic planes both probes build

let private DISPATCHES = 20
let private SUBMITS = 5
let private STREAM_MB = 128   //! the least a dispatch streams: four times the 32 MB L2 of the reference card

let private YBIT = 32u    //! the y buffer's hazard bit in the binding order below
let private YBIT2 = 64u   //! the second output plane's, in the `cols` chain

var private g_single_copy = false
var private g_cold = false
var private g_cols = false

//! one probe's synthetic planes and activation rows, uploaded; the dispatch geometry rides beside them
struct private ProbePlanes {
    wq_bytes : int64
    ws_bytes : int64
    xq_bytes : int64
    xs_bytes : int64
    y_bytes : int64
    rg_bytes : int64
    copies : int
    blocks : int     //! weight blocks a plane copy (the region table's stride)
    wgs_all_copies : int64   //! the dispatch grid over every copy's rows (the ring's dispatch)
    wgs_one_copy : int64     //! ... over one copy's rows (a `cold` dispatch's)
    pc : GemvArgs
}

//! the host-side fill of one probe: `wq_words` / `ws_words` of hashed weights, `xq_words` / `xs_words` of activation words
//! and scales over `xrows` rows, `blocks` weight blocks a plane copy (the region table's stride), the copies' regions;
//! `yd2` is the second output plane the `cols` chain alternates with `yd`
def private probe_fill(seed : uint; wq_words, ws_words, xq_words, xs_words, blocks : int; var p : ProbePlanes) : tuple<wqd : uint64; wsd : uint64; xqd : uint64; xsd : uint64; yd : uint64; yd2 : uint64; rgd : uint64> {
    var wqh : array<uint>
    var wsh : array<uint>
    var xqh : array<uint>
    var xsh : array<float>
    wqh |> reserve(wq_words)
    wqh |> resize(wq_words)
    wsh |> reserve(ws_words)
    wsh |> resize(ws_words)
    xqh |> resize(xq_words)
    xsh |> resize(xs_words)
    for (i in range(wq_words)) {
        wqh[i] = hash_word(uint(i) + seed * 77777u)
    }
    for (i in range(ws_words)) {
        wsh[i] = cool_f16_pair(uint(i) + seed * 13u)
    }
    for (i in range(xq_words)) {
        xqh[i] = hash_word(uint(i) * 31u + 5u)
    }
    for (i in range(xs_words)) {
        xsh[i] = 0.5 + float(i % 7) * 0.25
    }
    p.wq_bytes = int64(wq_words) * 4l
    p.ws_bytes = int64(ws_words) * 4l
    p.xq_bytes = int64(xq_words) * 4l
    p.xs_bytes = int64(xs_words) * 4l
    p.rg_bytes = 8l * int64(p.copies)
    let wqd = make_device_buf(p.wq_bytes)
    let wsd = make_device_buf(p.ws_bytes)
    let xqd = make_device_buf(p.xq_bytes)
    let xsd = make_device_buf(p.xs_bytes)
    let yd = make_device_buf(p.y_bytes)
    let yd2 = g_cols ? make_device_buf(p.y_bytes) : 0ul
    let rgd = make_device_buf(p.rg_bytes)
    var regs <- [for (i in range(p.copies * 2)); (i % 2 == 0) ? uint(i / 2 * blocks) : 0u]   //! region r: its own plane copy, the one x
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), p.wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), p.ws_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), p.xq_bytes)
        upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), p.xs_bytes)
        upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), p.rg_bytes)
    }
    delete wqh
    delete wsh
    delete xqh
    delete xsh
    return (wqd = wqd, wsd = wsd, xqd = xqd, xsd = xsd, yd = yd, yd2 = yd2, rgd = rgd)
}

//! DISPATCHES dependent dispatches of `fmt`'s class (`ncols` 0 = the one-column class) over the planes, timed
//! after a warm-up submit; MB = the weight bytes a dispatch reads. Under `cold` every dispatch is one plane copy
//! at the served launch size, the copies taken in turn so none is L2-warm from the dispatch before. Under `cols`
//! the chain alternates two output planes under their own hazard bits: a dispatch's barrier is the write-after-write
//! against the dispatch two back on its plane (the encoder's own WAW gate), so consecutive dispatches overlap as
//! the batched step's GEMVs do; the one-plane modes chain every dispatch on the one y
def private probe_time(fmt : KqFmt; ncols : int; p : ProbePlanes; b : tuple<wqd : uint64; wsd : uint64; xqd : uint64; xsd : uint64; yd : uint64; yd2 : uint64; rgd : uint64>) : tuple<mb : double; us : double> {
    var raw = alloc_cmd()
    var sets : array<VkDescriptorSet>
    unsafe {
        let sizes = fixed_array(p.wq_bytes, p.ws_bytes, p.rg_bytes, p.xq_bytes, p.xs_bytes, p.y_bytes)
        let gbits = fixed_array(1u, 2u, 4u, 8u, 16u, YBIT)
        if (g_cold) {   //! a region table a copy, each its own set, so a dispatch names one copy
            sets |> reserve(p.copies)
            for (i in range(p.copies)) {
                var pair <- [uint(i * p.blocks), 0u]
                let rgd = make_device_buf(8l)
                upload_region_at(rgd, 0l, addr<uint8 const?>(pair[0]), 8l)
                let bufs = fixed_array(b.wqd, b.wsd, rgd, b.xqd, b.xsd, b.yd)
                let sz = fixed_array(p.wq_bytes, p.ws_bytes, 8l, p.xq_bytes, p.xs_bytes, p.y_bytes)
                sets |> push(ncols > 0 ? gemv_cls_set_n(int(fmt), bufs, sz, gbits) : gemv_cls_set(int(fmt), bufs, sz, gbits))
                delete pair
            }
        } else {
            let bufs = fixed_array(b.wqd, b.wsd, b.rgd, b.xqd, b.xsd, b.yd)
            sets |> push(ncols > 0 ? gemv_cls_set_n(int(fmt), bufs, sizes, gbits) : gemv_cls_set(int(fmt), bufs, sizes, gbits))
            if (g_cols) {
                let bufs2 = fixed_array(b.wqd, b.wsd, b.rgd, b.xqd, b.xsd, b.yd2)
                let gbits2 = fixed_array(1u, 2u, 4u, 8u, 16u, YBIT2)
                sets |> push(ncols > 0 ? gemv_cls_set_n(int(fmt), bufs2, sizes, gbits2) : gemv_cls_set(int(fmt), bufs2, sizes, gbits2))
            }
        }
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = p.pc
        if (g_cold) {
            pc.nreg = 1u
            pc.ystride = pc.d
        }
        let wgs = g_cold ? p.wgs_one_copy : p.wgs_all_copies
        for (i in range(DISPATCHES)) {
            var s_cls = sets[i % length(sets)]
            if (ncols > 0) {
                gemv_cls_enc_n(int(fmt), raw, h, s_cls, pc, wgs)
            } else {
                gemv_cls_enc(int(fmt), raw, h, s_cls, pc, wgs)
            }
            if (!g_cols) {
                vhz_dep(raw, h, YBIT, 0u, true)   //! the next dispatch waits on this one's y
            }
        }
        vk_check(vkEndCommandBuffer(raw), null)
    }
    submit_wait(raw)   //! the warm-up: pipeline compile, first-touch
    let t0 = ref_time_ticks()
    for (_i in range(SUBMITS)) {
        submit_wait(raw)
    }
    let us = double(get_time_usec(t0)) / double(SUBMITS * DISPATCHES)
    let mb = double(p.wq_bytes + p.ws_bytes) / 1048576.0lf
    return (mb = g_cold ? mb / double(p.copies) : mb, us = us)
}

//! the plane copies a dispatch streams: STREAM_MB or more, or one copy under `single`
def private probe_copies(plane_bytes : int) : int => g_single_copy ? 1 : (STREAM_MB * 1048576 + plane_bytes - 1) / plane_bytes

//! lanes_per_row 0 = the whole subgroup; `ncols` 0 dispatches the one-column class, else the N-column class over
//! that many activation rows
def private probe_fmt(fmt : KqFmt; n, d : int; lanes_per_row : uint; ncols : int = 0) : tuple<mb : double; us : double> {
    let nsb = n / 256
    let nwsb = d * nsb
    let qsb_words = int(kq_qsb(fmt) / 4l)
    let ssb_words = int(kq_dev_ssb(fmt) / 4l)
    let plane_bytes = nwsb * (qsb_words + ssb_words) * 4   //! under 2^31 for every served shape (the 27B's largest plane is 89 MB)
    let xrows = max(ncols, 1)
    var p = ProbePlanes(copies = probe_copies(plane_bytes), blocks = nwsb)
    p.y_bytes = int64(d) * 4l * int64(p.copies) * int64(xrows)
    p.wgs_all_copies = gemv_wgs(int64(d) * int64(p.copies), lanes_per_row)
    p.wgs_one_copy = gemv_wgs(int64(d), lanes_per_row)
    p.pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(p.copies), ybase = 0u, lanes_per_row = lanes_per_row,
        ncols = uint(ncols), ystride = uint(d * p.copies))
    let b = probe_fill(uint(int(fmt)), nwsb * qsb_words * p.copies, nwsb * ssb_words * p.copies, nsb * 64 * xrows, nsb * xrows, nwsb, p)
    return probe_time(fmt, ncols, p, b)
}

//! the q8 decode GEMV (`q8_gemv_cls`, a subgroup a row) at the same shape and ring: 32 int8 quants and an f16 scale a block;
//! `ncols` as in probe_fmt
def private probe_q8(n, d : int; ncols : int = 0) : tuple<mb : double; us : double> {
    let nb = n / 32
    let nblk = d * nb
    let xrows = max(ncols, 1)
    var p = ProbePlanes(copies = probe_copies(nblk * 34), blocks = nblk)
    p.y_bytes = int64(d) * 4l * int64(p.copies) * int64(xrows)
    p.wgs_all_copies = gemv_wgs(int64(d) * int64(p.copies), 0u)
    p.wgs_one_copy = gemv_wgs(int64(d), 0u)
    p.pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(p.copies), ybase = 0u, lanes_per_row = 0u,
        ncols = uint(ncols), ystride = uint(d * p.copies))
    let b = probe_fill(99991u, nblk * 8 * p.copies, (nblk * p.copies + 1) / 2, nb * 8 * xrows, nb * xrows, nblk, p)
    return probe_time(KqFmt.q8, ncols, p, b)
}

//! one probe row of either class at `ncols` (0 = the one-column class)
def private probe_any(fmt : KqFmt; n, d : int; ncols : int) : tuple<mb : double; us : double> {
    if (fmt == KqFmt.q8) {
        return probe_q8(n, d, ncols)
    }
    return probe_fmt(fmt, n, d, gemv_lanes_per_row(int(fmt), int64(n)), ncols)
}

//! the N-column ruler: the one-column class, then the N-column class at 1, 2, 4 and 8 rows, per format (the q8 rows
//! take the two-rows-a-subgroup stamps past `g_q8_n2_min_n`, which `main` holds at 0 for the run unless `n2off`),
//! every dispatch's output unread - timing only
def private cols_ruler(n, d : int) {
    let fmts = fixed_array(KqFmt.q8, KqFmt.k4, KqFmt.k6, KqFmt.k5, KqFmt.q40, KqFmt.iq4xs,
        KqFmt.k3, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq4nl, KqFmt.k2, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs)
    for (f in fmts) {
        verify(gemv_cls_ensure(int(f)) && gemv_cls_ensure_n(int(f)), "vk_gemv_probe: the ruler's gemv class rails must engage on a live device")
    }
    let pair_note = g_q8_n2_min_n == 0u ? "the q8 row-pair stamps engaged" : "the q8 rows a row a subgroup (n2off)"
    print("vk_gemv_probe: the N-column ruler at [n {n} x d {d}], {g_gpu.rows_per_wg} rows/wg, {DISPATCHES} dispatches x {SUBMITS} submits after a warm-up over two alternating output planes, {pair_note}; GB/s over the weight bytes; the last column = this dispatch's wall over `cols` one-column dispatches\n")
    print("| fmt    | class | cols |  us/dispatch |    GB/s | vs N single |             |\n")
    print("| :----- | :---- | ---: | -----------: | ------: | ----------: | :---------- |\n")
    let widths = fixed_array(1, 2, 4, 8)
    for (f in fmts) {
        let one = probe_any(f, n, d, 0)
        print("| {f} | one | 1 | {one.us:12.1f} | {one.mb * 1048576.0lf / (one.us * 1000.0lf):7.1f} | {1.0lf:11.2f} | timing-only |\n")
        for (nc in widths) {
            let r = probe_any(f, n, d, nc)
            print("| {f} | ncol | {nc} | {r.us:12.1f} | {r.mb * 1048576.0lf / (r.us * 1000.0lf):7.1f} | {r.us / (one.us * double(nc)):11.2f} | timing-only |\n")
        }
    }
}

[export]
def main : int {
    let args <- get_command_line_arguments()
    var n = 4096
    var d = 12288
    let shape_args <- [for (a in args); a; where to_int(a) > 0]
    if (length(shape_args) >= 2) {
        n = to_int(shape_args[length(shape_args) - 2])
        d = to_int(shape_args[length(shape_args) - 1])
    }
    g_single_copy = find_index(args, "single") >= 0
    g_cold = find_index(args, "cold") >= 0 && !g_single_copy
    g_cols = find_index(args, "cols") >= 0
    if (g_single_copy && find_index(args, "cold") >= 0) {
        print("vk_gemv_probe: WARNING `single` takes precedence over `cold` - the cold arm is dropped; run them apart\n")
    }
    if (!ensure_q8_batch_cls()) {
        print("vk_gemv_probe: no Vulkan device\n")
        return 1
    }
    if (g_cols) {
        g_q8_n2_min_n = find_index(args, "n2off") >= 0 ? 0xFFFFFFFFu : 0u
        cols_ruler(n, d)
        return 0
    }
    let fmts = fixed_array(KqFmt.k4, KqFmt.q40, KqFmt.k5, KqFmt.k6, KqFmt.iq4xs, KqFmt.k3, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq4nl, KqFmt.k2, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs)
    for (f in fmts) {
        verify(gemv_cls_ensure(int(f)), "vk_gemv_probe: every kq gemv class rail must engage on a live device")
    }
    let ring = g_single_copy ? "one plane copy per dispatch (single)" : "plane copies to {STREAM_MB} MB per dispatch"
    print("vk_gemv_probe: decode GEMV classes at [n {n} x d {d}], {g_gpu.rows_per_wg} rows/wg, {ring}, {DISPATCHES} dispatches x {SUBMITS} submits after a warm-up; lanes = lanes per output row (32 = one row per subgroup)\n")
    print("| fmt    | lanes |   MB/dispatch |  us/dispatch |    GB/s |             |\n")
    print("| :----- | ----: | ------------: | -----------: | ------: | :---------- |\n")
    verify(ensure_q8_gemv_cls(), "vk_gemv_probe: the q8 gemv class rail must engage on a live device")
    let q8 = probe_q8(n, d)
    print("| q8 | {g_gpu.subgroup_size:5d} | {q8.mb:13.1f} | {q8.us:12.1f} | {q8.mb * 1048576.0lf / (q8.us * 1000.0lf):7.1f} | timing-only |\n")
    let splits = fixed_array(0u, 16u, 8u)
    for (f in fmts) {
        for (lanes_per_row in splits) {
            let r = probe_fmt(f, n, d, lanes_per_row)
            let gbs = r.mb * 1048576.0lf / (r.us * 1000.0lf)
            print("| {f} | {lanes_per_row == 0u ? g_gpu.subgroup_size : int64(lanes_per_row):5d} | {r.mb:13.1f} | {r.us:12.1f} | {gbs:7.1f} | timing-only |\n")
        }
    }
    return 0
}
