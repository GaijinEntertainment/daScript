options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - the submit grain of a streamed expert layer: one layer's expert bytes (default
// 1150 MB over 512 experts) cross the bus in N pieces, each piece a transfer-queue submit that signals
// the transfer timeline and a compute-queue read of that piece. Two wait forms per grain: `own` - the
// piece's compute submit waits its own timeline value (the bytes it reads, nothing more); `last` -
// every compute submit waits the LAST transfer's value (one barrier for the layer). Every transfer
// is queued before the first compute submit, as a rail that streams ahead would queue them. The
// row is a form's wall per grain; the wall growing with N is the per-submit cost, the gap between
// the forms is what the per-piece wait buys. Args: [mb] [experts] (default 1150 512).

require strings
require math
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require daslib/strings_boost

let private ROUNDS = 3
let private DEFAULT_MB = 1150
let private DEFAULT_EXPERTS = 512
let private PAGE = 4096l

var private g_exit = 1

//! one form's row: its best and mean wall over the rounds
def private say_row(name : string; bytes : int64; us : array<double>) {
    var best = us[0]
    var sum = 0.0lf
    for (u in us) {
        best = min(best, u)
        sum += u
    }
    let mean = sum / double(length(us))
    let gb = double(bytes) / 1e9lf
    print("{name}: best {best / 1000.0lf} ms = {gb / (best / 1e6lf)} GB/s  mean {mean / 1000.0lf} ms\n")
}

//! the compute queue's in-order drain: an empty fenced submit after everything queued before it
def private drain_compute(var raw : VkCommandBuffer) {
    cmd_open(raw, true)
    cmd_close(raw)
    submit_wait(raw)
}

//! one round of a form: N transfer submits queued ahead, then N compute reads, each waiting `own` value or the last
def private round_us(var xc : array<VkCommandBuffer>; var cc : array<VkCommandBuffer>; var drain : VkCommandBuffer; pinned, dev, dev2 : uint64; grain : int; piece : int64; own : bool) : double {
    let t0 = ref_time_ticks()
    var sig : array<uint64>
    sig |> reserve(grain)
    for (p in range(grain)) {
        cmd_open(xc[p], true)
        cmd_copy_range(xc[p], pinned, int64(p) * piece, dev, int64(p) * piece, piece)
        cmd_close(xc[p])
        sig |> push(xfer_submit_after(xc[p], XFER_NO_WAIT))
    }
    for (p in range(grain)) {
        cmd_open(cc[p], true)
        cmd_copy_range(cc[p], dev, int64(p) * piece, dev2, int64(p) * piece, piece)
        cmd_close(cc[p])
        g_gpu.pend_wait = own ? sig[p] : sig[grain - 1]
        submit_nofence(cc[p])
    }
    drain_compute(drain)
    return double(get_time_usec(t0))
}

def private run_probe(bytes, experts : int64) {
    if (!ensure_q8_batch_cls()) {
        print("no Vulkan device\n")
        return
    }
    if (g_gpu.xfer_fam < 0) {
        print("no transfer queue: the grain sweep needs the two-queue form\n")
        return
    }
    let dev = make_device_buf(bytes, true)
    let dev2 = make_device_buf(bytes, true)
    let pinned = make_host_buf(bytes, false, [xfer_shared = true])   //! contents are irrelevant to a copy's wall
    let ne = int(experts)
    var xc, cc : array<VkCommandBuffer>
    xc |> reserve(ne)
    cc |> reserve(ne)
    for (_p in range(ne)) {
        xc |> push(alloc_xfer_cmd())
        cc |> push(alloc_cmd())
    }
    var drain = alloc_cmd()
    print("{bytes / 1000000l} MB over {ne} experts, transfer queue on, {ROUNDS} rounds\n")
    var grain = 1
    while (grain <= ne) {
        let piece = (bytes / int64(grain)) / PAGE * PAGE
        for (own in [true, false]) {
            var inscope us <- [for (_r in range(ROUNDS)); round_us(xc, cc, drain, pinned.buf, dev, dev2, grain, piece, own)]
            say_row("grain {grain} ({piece / 1000l} KB/piece) {own ? "own " : "last"} wait", piece * int64(grain), us)
        }
        grain = grain == ne ? ne + 1 : min(grain * 4, ne)
    }
    g_exit = 0
}

[export]
def main : int {
    var mb = DEFAULT_MB
    var experts = DEFAULT_EXPERTS
    var i = 0
    for (a in get_command_line_arguments()) {
        let v = to_int(a)
        if (v > 0) {
            if (i == 0) {
                mb = v
            } else {
                experts = v
            }
            i++
        }
    }
    run_probe(int64(mb) * 1000000l, int64(experts))
    return g_exit
}
