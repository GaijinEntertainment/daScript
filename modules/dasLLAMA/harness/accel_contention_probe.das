options gen2
options stack = 65536

require das_accelerate
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — the live-Metal-device ingredient (env METAL=1)
require dasllama/dasllama_math
require dasllama/dasllama_par
require daslib/jobque_boost
require daslib/fio
require math

// Contention rig: reproduce the MoE-router call pattern (small inline sgemm between jobque NEON
// bursts) without a model load. Prints per-call sgemm ms + BLASGetThreading per context.
// Router shape (26B): ntok=512, d=128 experts, n=2816.

let NTOK = 512
let D = 128
let N = 2816
let LAYERS = 48

[export]
def main {
    var w : array<float>
    w |> resize(D * N)
    var x : array<float>
    x |> resize(NTOK * N)
    var y : array<float>
    y |> resize(NTOK * D)
    for (i in range(length(w))) {
        w[i] = float(i % 97) * 0.01
    }
    for (i in range(length(x))) {
        x[i] = float(i % 89) * 0.01
    }
    var burst : array<float>
    burst |> resize(4 * 1024 * 1024)
    var lane_mode : array<int>
    lane_mode |> resize(8)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (get_env_variable("METAL") == "1") {
            let dev = metal_create_system_default_device()
            print("metal device: {metal_device_name(dev)}\n")
        }
    }
    with_job_que() {
        setup_dasllama_jobque_()
        print("main threading_mode={accel_threading_mode()}\n")
        unsafe {
            var bp = addr(burst[0])
            var lp = addr(lane_mode[0])
            let wp = addr<float const?>(w[0])
            let xp = addr<float const?>(x[0])
            var yp = addr(y[0])
            maybe_parallel_for(0, 8, 8) $(rb, _re) {
                unsafe {
                    lp[rb] = accel_threading_mode()
                }
            }
            print("lane threading_modes={lane_mode}\n")
            var acc = 0.0
            var tot_ms = 0.0lf
            var max_ms = 0.0lf
            for (_l in range(LAYERS)) {
                // the jobque burst before the router (keeps workers hot/spinning)
                maybe_parallel_for(0, 8, 8) $(rb, _re) {
                    unsafe {
                        var s = 0.0
                        let base = rb * 524288
                        for (i in range(base, base + 524288)) {
                            s += bp[i]
                        }
                        bp[rb * 524288] = s
                    }
                }
                let t0 = ref_time_ticks()
                accel_sgemm_nt(NTOK, D, N, xp, N, wp, N, yp, D)
                let ms = double(get_time_usec(t0)) / 1000.0lf
                tot_ms += ms
                max_ms = max(max_ms, ms)
                acc += yp[0]
            }
            print("sgemm avg {tot_ms / double(LAYERS)} ms  max {max_ms} ms  (checksum {acc})\n")
        }
    }
}
