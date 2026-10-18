options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - the layer-major prompt pass's copy rates on this box: one routed layer's expert
// stack (Qwen3.8-Flash-Next's iq3s4 gate/up and iq4nl32 down over 512 experts, 1150 MB) from host
// memory into a device buffer by the forms the engine has - a ReBAR direct write (`upload_region_at`,
// the CPU lanes' memcpy into the mapped VRAM), a pinned staging buffer DMA'd on the transfer queue
// (the stream mirrors' form), and the host memcpy of a pageable source into the pinned staging that
// the DMA form needs first (the mapped image is pageable; the driver imports no file-backed pages).
// Each form runs ROUNDS times; the row is its best and its mean. Args: [mb] (default 1150).

require strings
require math
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require daslib/strings_boost
require jobque
require daslib/jobque_boost
require dasllama/dasllama_math

let private ROUNDS = 5
let private DEFAULT_MB = 1150

var private g_exit = 1

//! one form's row: its best and mean wall over the rounds, as GB/s and ms
def private say_row(name : string; bytes : int64; us : array<double>) {
    var best = us[0]
    var sum = 0.0lf
    for (u in us) {
        best = min(best, u)
        sum += u
    }
    let mean = sum / double(length(us))
    let gb = double(bytes) / 1e9lf
    let rounds = build_string() $(var w : StringBuilderWriter) {
        for (u in us) {
            w |> write(" ") |> write(int(u / 1000.0lf))
        }
    }
    print("{name}: {bytes / 1000000l} MB  best {best / 1000.0lf} ms = {gb / (best / 1e6lf)} GB/s  mean {mean / 1000.0lf} ms = {gb / (mean / 1e6lf)} GB/s  (rounds ms:{rounds})\n")
}

//! a device-to-device copy of `bytes` on the compute queue: VRAM reads at the card's rate, a buffer the driver placed in system memory at the link's
def private dev_copy_row(name : string; src, dst : uint64; bytes : int64) {
    var us : array<double>
    us |> reserve(ROUNDS)
    for (_r in range(ROUNDS)) {
        let t0 = ref_time_ticks()
        batch_cmd_one_shot() $(raw : VkCommandBuffer) {
            cmd_copy_whole(raw, src, dst, bytes)
        }
        us |> push(double(get_time_usec(t0)))
    }
    say_row(name, bytes, us)
}

def private run_probe(bytes : int64) {   // nolint:STYLE038,LINT019 — one linear measurement sweep; the length fires where the vulkan arm compiles in
    if (!ensure_q8_batch_cls()) {
        print("no Vulkan device\n")
        return
    }
    var src : array<uint8>
    src |> reserve(bytes)   //! a gigabyte's source passes the unreserved-growth cap
    src |> resize(bytes)
    for (p in range64(bytes / 4096l)) {   //! touch every page: a pageable source the OS has faulted in, as the mapped image's pages are
        src[p * 4096l] = uint8(p)
    }
    let dev = make_device_buf(bytes, true)
    let dev2 = make_device_buf(bytes, true)
    let rebar = key_exists(g_gpu.dev_mapped, dev)
    print("device {rebar ? "ReBAR-mapped" : "not mapped (staged uploads)"}, transfer queue {g_gpu.xfer_fam >= 0 ? "on" : "off"}, {get_total_hw_jobs() + 1} lanes\n")
    unsafe {
        var us_direct : array<double>
        us_direct |> reserve(ROUNDS)
        for (_r in range(ROUNDS)) {
            let t0 = ref_time_ticks()
            upload_region_at(dev, 0l, addr<uint8 const?>(src[0]), bytes)
            us_direct |> push(double(get_time_usec(t0)))
        }
        say_row(rebar ? "direct ReBAR write" : "staged upload", bytes, us_direct)
        dev_copy_row("device -> device copy after the direct write (where the buffer sits)", dev, dev2, bytes)
        var pinned = make_host_buf(bytes, false, [xfer_shared = true])
        par_memcpy(pinned.mapped, reinterpret<void?>(addr<uint8 const?>(src[0])), bytes)
        var us_dma : array<double>
        us_dma |> reserve(ROUNDS)
        if (g_gpu.xfer_fam >= 0) {
            var raw = alloc_xfer_cmd()
            for (r in range(ROUNDS)) {
                let t0 = ref_time_ticks()
                cmd_open(raw, true)
                cmd_copy_whole(raw, pinned.buf, r % 2 == 0 ? dev : dev2, bytes)
                cmd_close(raw)
                let sig = xfer_submit_after(raw, XFER_NO_WAIT)
                xfer_wait(sig, false)
                us_dma |> push(double(get_time_usec(t0)))
            }
            say_row("pinned -> VRAM, transfer queue", bytes, us_dma)
        } else {
            for (r in range(ROUNDS)) {
                let t0 = ref_time_ticks()
                batch_cmd_one_shot() $(raw : VkCommandBuffer) {
                    cmd_copy_whole(raw, pinned.buf, r % 2 == 0 ? dev : dev2, bytes)
                }
                us_dma |> push(double(get_time_usec(t0)))
            }
            say_row("pinned -> VRAM, compute queue", bytes, us_dma)
        }
        var us_host : array<double>
        us_host |> reserve(ROUNDS)
        for (_r in range(ROUNDS)) {
            let t0 = ref_time_ticks()
            par_memcpy(pinned.mapped, reinterpret<void?>(addr<uint8 const?>(src[0])), bytes)
            us_host |> push(double(get_time_usec(t0)))
        }
        say_row("pageable -> pinned, the lanes' memcpy", bytes, us_host)
        var us_one : array<double>
        us_one |> reserve(ROUNDS)
        for (_r in range(ROUNDS)) {
            let t0 = ref_time_ticks()
            memcpy(pinned.mapped, reinterpret<void?>(addr<uint8 const?>(src[0])), bytes)
            us_one |> push(double(get_time_usec(t0)))
        }
        say_row("pageable -> pinned, one thread", bytes, us_one)
    }
    g_exit = 0
}

[export]
def main : int {
    var mb = DEFAULT_MB
    for (a in get_command_line_arguments()) {
        let v = to_int(a)
        if (v > 0) {
            mb = v
        }
    }
    with_job_que() {
        setup_dasllama_jobque_()
        run_probe(int64(mb) * 1000000l)
    }
    return g_exit
}
