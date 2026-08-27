options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - isolated per-shape rate of the serving mm_a batch GEMM kernel; the
// rows attribute prefill GEMM cost across the dense role shapes (gate/up, down, q/wo, k/v).
// Direction-grade instrument, never record-grade: the kernel is the shipped, oracle-gated
// one (test_vulkan_kernels.das), dispatched record-once and timed over repeated submits.
// One arg (gate|down|q|kv) pins a single shape - the long-window ngfx GPU Trace vehicle.
// Shapes default to Llama-3.2-3B Q8 geometry (dim 3072, hidden 8192, kv_dim 1024).

require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes

let private TILE = 128
let private DISPATCHES = 16
let private SUBMITS = 25

def private hash_word(i : uint) : uint {
    var x = i * 2654435761u
    x ^= x >> 16u
    x *= 2246822519u
    x ^= x >> 13u
    return x
}

def private ws_word(i : uint) : uint {
    // two packed f16 scales per word, small positive values (the kernel-suite fixture shape)
    let h = 0x2c00u + (i % 64u)
    return h | (h << 16u)
}

struct private ShapeBufs {
    wq : array<uint>
    ws : array<uint>
    xq : array<uint>
    xs : array<float>
}

def private fill_fixture(var b : ShapeBufs; totblk, rows, nbb : int) {
    b.wq |> resize(totblk * 8)
    b.ws |> resize(totblk / 2 + 1)
    b.xq |> resize(rows * nbb * 8)
    b.xs |> resize(rows * nbb)
    for (i in range(totblk * 8)) {
        b.wq[i] = hash_word(uint(i) + 3u)
    }
    for (i in range(totblk / 2 + 1)) {
        b.ws[i] = ws_word(uint(i) + 7u)
    }
    for (i in range(rows * nbb * 8)) {
        b.xq[i] = hash_word(uint(i) * 13u + 1u)
    }
    for (i in range(rows * nbb)) {
        // cool inputs: the mm family accumulates in f16, hot fixtures overflow 65504
        b.xs[i] = (0.375 + float(i % 9) * 0.0625) * 0.0625
    }
}

def private run_shape(name : string; d, n, cnt : int) {
    let nbb = n / 32
    let rows = cnt
    let wtiles = (d + TILE - 1) / TILE
    let wgs = ((cnt + TILE - 1) / TILE) * wtiles
    let totblk = d * nbb
    var b : ShapeBufs
    fill_fixture(b, totblk, rows, nbb)
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    let xq_bytes = int64(rows * nbb) * 32l
    let xs_bytes = int64(rows * nbb) * 4l
    let y_bytes = int64(rows * d) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let xqd = make_device_buf(xq_bytes)
    let xsd = make_device_buf(xs_bytes)
    let yd = make_device_buf(y_bytes)
    var sched : array<uint>
    sched |> resize(4 + wgs)
    sched[2] = uint(cnt)   // one region: [wblk0 = 0, row0 = 0, cnt, wg0 = 0], map all zeros
    var per_us : double
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(b.wq[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(b.ws[0]), ws_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(b.xq[0]), xq_bytes)
        upload_region_at(xsd, 0l, addr<uint8 const?>(b.xs[0]), xs_bytes)
        let sc_bytes = int64(4 + wgs) * 4l
        let scd = make_device_buf(sc_bytes)
        upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
        let bufs = fixed_array(wqd, wsd, scd, xqd, xsd, yd)
        let sizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes)
        let gbits = fixed_array(1u, 2u, 0u, 4u, 8u, 16u)
        var sc = set_q8_batch_mm_a_cls(bufs, sizes, gbits)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var hz : VkHaz
        var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
        for (_k in range(DISPATCHES)) {
            enc_q8_batch_mm_a_cls(raw, hz, sc, pc, int64(wgs))
        }
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)   // warmup: pipeline compile + clocks settle
        submit_wait(raw)
        let t0 = ref_time_ticks()
        for (_s in range(SUBMITS)) {
            submit_wait(raw)
        }
        let us = get_time_usec(t0)
        per_us = double(us) / double(SUBMITS * DISPATCHES)
    }
    let flop = 2.0lf * double(rows) * double(d) * double(n)
    let tflops = flop / (per_us * 1000000.0lf)
    let ms = per_us / 1000.0lf
    print("{name}: d={d} K={n} cnt={cnt} wgs={wgs}  {ms} ms/dispatch  {tflops} TFLOP/s\n")
    delete sched
    delete b
}

def private run_probe {
    if (!ensure_q8_batch_cls()) {
        print("no Vulkan device\n")
        return
    }
    if (g_gpu.coopmat_mode < 3) {
        print("coopmat mode {g_gpu.coopmat_mode} < 3 - mm tiles not routed on this device\n")
        return
    }
    if (!ensure_q8_batch_mm_a_cls()) {
        print("mm_a pipeline failed\n")
        return
    }
    var only = ""
    for (a in get_command_line_arguments()) {
        if (a == "gate" || a == "down" || a == "q" || a == "kv" || a == "kvm" || a == "qkv") {
            only = a
        }
    }
    if (only == "gate" || only == "") {
        run_shape("gate/up", 8192, 3072, 512)
    }
    if (only == "down" || only == "") {
        run_shape("down   ", 3072, 8192, 512)
    }
    if (only == "q" || only == "") {
        run_shape("q/wo   ", 3072, 3072, 512)
    }
    if (only == "kv" || only == "") {
        run_shape("k/v    ", 1024, 3072, 512)
    }
    if (only == "kvm" || only == "") {
        run_shape("k+v mrg", 2048, 3072, 512)
    }
    if (only == "qkv" || only == "") {
        run_shape("qkv mrg", 5120, 3072, 512)
    }
}

[export]
def main {
    run_probe()
}
