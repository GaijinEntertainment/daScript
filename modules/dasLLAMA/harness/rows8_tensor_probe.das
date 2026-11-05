options gen2
options persistent_heap
options _dasllama_internal = true

// ROWS8 TENSOR PROBE - the lane-to-element layout of a Metal 4 matmul2d cooperative LEFT input tensor and of its
// destination, on this box: for a 16 x 8 product at K = 32 and at K = 64, every lane's capacity elements as
// (row, k) and (row, col). The layout is the device's, not the API's, so a register-filled operand needs it probed.
//
//   bin/daslang -jit modules/dasLLAMA/harness/rows8_tensor_probe.das   (DASLLAMA_ROWS8_PROBE=<entry> narrows it)

require daslib/fio
require daslib/strings_boost
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_env

def private read_msl(path : string) : string {
    var src = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            src = fread(f)
        }
    }
    return src
}

def private dump(dev : MetalDevice?; queue : MetalCommandQueue?; entry : string) {
    var err = ""
    var pso = pso_from_source(dev, read_msl("modules/dasLLAMA/harness/rows8_tensor_probe.metal"), entry, false, err)
    if (pso == null) {
        print("{entry}: pso - {err}\n")
        return
    }
    let words = 64 + 8192 * 2
    var bout = metal_new_buffer(dev, uint64(words * 4))
    var bscr = metal_new_buffer(dev, 128ul * 8ul * 2ul)
    if (!run_compute_1d(queue, pso, [bout, bscr], 32u, 32u, err)) {
        print("{entry}: run - {err}\n")
        return
    }
    unsafe {
        let p = reinterpret<int?>(metal_buffer_contents(bout))
        let lc = p[0]
        let dc = p[1]
        print("{entry}: left capacity {lc}, destination capacity {dc}\n")
        for (lane in range(32)) {
            let s = build_string() $(var w) {
                w |> write("  lane {lane} L:")
                for (i in range(lc)) {
                    w |> write(" ({p[64 + (lane * 128 + i) * 2]},{p[64 + (lane * 128 + i) * 2 + 1]})")
                }
                w |> write("  D:")
                for (i in range(dc)) {
                    w |> write(" ({p[64 + 8192 + (lane * 128 + i) * 2]},{p[64 + 8192 + (lane * 128 + i) * 2 + 1]})")
                }
            }
            print("{s}\n")
        }
    }
    metal_release(bout)
    metal_release(bscr)
    metal_release(pso)
}

[export]
def main() : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        let only = g_env_harness.rows8_probe
        for (entry in ["probe128", "probe_h_16x128", "probe_u8_16x16", "probe_i8_16x16", "probe_h_16x16", "probe32x32", "probe_u8_16x32", "probe_u8_16x64",
                       "probe_h_16x32", "probe_h_16x64", "probe_i8_16x32", "probe_i8_16x64", "probe_u8_16x64n16"]) {
            continue if (!empty(only) && find(only, entry) < 0)
            dump(dev, queue, entry)
        }
        return 0
    } else {
        print("no das_metal\n")
        return 1
    }
}
