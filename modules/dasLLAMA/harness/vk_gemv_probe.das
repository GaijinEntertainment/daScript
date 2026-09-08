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
// Args: <n> <d> (default 4096 12288, the Qwen3.5-9B FFN gate; 5120 17408 is the 27B's).

require strings
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

let private YBIT = 32u   //! the y buffer's hazard bit in the binding order below

//! one format at [n x d]: the shipped class over as many d-row region copies as one dispatch needs to stream STREAM_MB
def private probe_fmt(fmt : KqFmt; n, d : int) : tuple<mb : double; us : double> {
    let nsb = n / 256
    let nwsb = d * nsb
    let qsb_words = int(kq_qsb(fmt) / 4l)
    let ssb_words = int(kq_dev_ssb(fmt) / 4l)
    let plane_bytes = nwsb * (qsb_words + ssb_words) * 4   //! under 2^31 for every served shape (the 27B's largest plane is 89 MB)
    let copies = (STREAM_MB * 1048576 + plane_bytes - 1) / plane_bytes
    let wq_words = nwsb * qsb_words * copies
    let ws_words = nwsb * ssb_words * copies
    var wqh : array<uint>
    var wsh : array<uint>
    var xqh : array<uint>
    var xsh : array<float>
    wqh |> reserve(wq_words)
    wqh |> resize(wq_words)
    wsh |> reserve(ws_words)
    wsh |> resize(ws_words)
    xqh |> resize(nsb * 64)
    xsh |> resize(nsb)
    for (i in range(wq_words)) {
        wqh[i] = hash_word(uint(i) + uint(int(fmt)) * 77777u)
    }
    for (i in range(ws_words)) {
        wsh[i] = cool_f16_pair(uint(i) + uint(int(fmt)) * 13u)
    }
    for (i in range(nsb * 64)) {
        xqh[i] = hash_word(uint(i) * 31u + 5u)
    }
    for (i in range(nsb)) {
        xsh[i] = 0.5 + float(i % 7) * 0.25
    }
    let wq_bytes = int64(wq_words) * 4l
    let ws_bytes = int64(ws_words) * 4l
    let xq_bytes = int64(nsb) * 256l
    let xs_bytes = int64(nsb) * 4l
    let y_bytes = int64(d) * 4l * int64(copies)
    let rg_bytes = 8l * int64(copies)
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let xqd = make_device_buf(xq_bytes)
    let xsd = make_device_buf(xs_bytes)
    let yd = make_device_buf(y_bytes)
    let rgd = make_device_buf(rg_bytes)
    var regs <- [for (i in range(copies * 2)); (i % 2 == 0) ? uint(i / 2 * nwsb) : 0u]   //! region r: its own plane copy, the one x
    let wgs = (int64(d) * int64(copies) + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg
    var raw = alloc_cmd()
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
        upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
        upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), rg_bytes)
        let bufs = fixed_array(wqd, wsd, rgd, xqd, xsd, yd)
        let sizes = fixed_array(wq_bytes, ws_bytes, rg_bytes, xq_bytes, xs_bytes, y_bytes)
        let gbits = fixed_array(1u, 2u, 4u, 8u, 16u, YBIT)
        var s_cls = gemv_cls_set(int(fmt), bufs, sizes, gbits)
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(copies), ybase = 0u)
        for (_i in range(DISPATCHES)) {
            gemv_cls_enc(int(fmt), raw, h, s_cls, pc, wgs)
            vhz_dep(raw, h, YBIT, 0u, true)   //! the next dispatch waits on this one's y
        }
        vk_check(vkEndCommandBuffer(raw), null)
    }
    submit_wait(raw)   //! the warm-up: pipeline compile, first-touch
    let t0 = ref_time_ticks()
    for (_i in range(SUBMITS)) {
        submit_wait(raw)
    }
    let us = double(get_time_usec(t0)) / double(SUBMITS * DISPATCHES)
    delete wqh
    delete wsh
    delete xqh
    delete xsh
    return (mb = double(wq_bytes + ws_bytes) / 1048576.0lf, us = us)
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
    if (!ensure_q8_batch_cls()) {
        print("vk_gemv_probe: no Vulkan device\n")
        return 1
    }
    let fmts = fixed_array(KqFmt.k4, KqFmt.q40, KqFmt.k5, KqFmt.k6, KqFmt.iq4xs, KqFmt.k3, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq4nl, KqFmt.k2, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs)
    for (f in fmts) {
        verify(gemv_cls_ensure(int(f)), "vk_gemv_probe: every kq gemv class rail must engage on a live device")
    }
    print("vk_gemv_probe: decode GEMV classes at [n {n} x d {d}], {g_gpu.rows_per_wg} rows/wg, plane copies to {STREAM_MB} MB per dispatch, {DISPATCHES} dispatches x {SUBMITS} submits after a warm-up\n")
    print("| fmt    |   MB/dispatch |  us/dispatch |    GB/s |             |\n")
    print("| :----- | ------------: | -----------: | ------: | :---------- |\n")
    for (f in fmts) {
        let r = probe_fmt(f, n, d)
        let gbs = r.mb * 1048576.0lf / (r.us * 1000.0lf)
        print("| {f} | {r.mb:13.1f} | {r.us:12.1f} | {gbs:7.1f} | timing-only |\n")
    }
    return 0
}
