options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require das_accelerate
require ?das_metal metal/das_metal_boost  // the live-Metal-device ingredient (env METAL=1)
require dasllama/dasllama_math
require dasllama/dasllama_par
require llvm/daslib/f16_cvt
require daslib/jobque_boost
require math
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 — the read sites sit in the apple static_if half; the linux lint flavor sees the require as unused

// Contention rig: reproduce the MoE-router call pattern (small inline sgemm between jobque NEON
// bursts) without a model load. Prints per-call sgemm ms + BLASGetThreading per context.
// Router shape (26B): ntok=512, d=128 experts, n=2816. Phase 2 repeats the pattern with the
// BNNS f16 arm at the E4B proj shape, strip-dispatched over the jobque exactly like the driver
// (healthy ~15-25 ms/call avg on M1 Max — 21.5 measured 2026-07-24; a foreign-pool starve
// reads ~50 ms+ flat).

let NTOK = 512
let D = 128
let N = 2816
let LAYERS = 48

// E4B per_layer_model_proj shape for the BNNS phase
let PM = 512
let PD = 10752
let PN = 2560
let PLAYERS = 24

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
        if (g_env_harness.metal_contention) {
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
        bnns_phase()
    }
}

// phase 2: BNNS f16 strips between the same jobque bursts — the shipped f16-lane call shape
def bnns_phase {
    var hw : array<uint16>
    hw |> resize(PD * PN)
    var hx : array<uint16>
    hx |> resize(PM * PN)
    var y : array<float>
    y |> resize(PM * PD)
    for (i in range(length(hw))) {
        hw[i] = uint16(f32_to_f16(float(i % 97) * 0.01))
    }
    for (i in range(length(hx))) {
        hx[i] = uint16(f32_to_f16(float(i % 89) * 0.01))
    }
    var burst : array<float>
    burst |> resize(4 * 1024 * 1024)
    unsafe {
        var bp = addr(burst[0])
        let hwp = addr<uint16 const?>(hw[0])
        let hxp = addr<uint16 const?>(hx[0])
        var yp = addr(y[0])
        if (accel_bnns_hgemm_nt(1, 1, 1, hxp, 1, hwp, 1, yp, 1) != 0) {
            print("bnns unavailable on this OS — phase skipped\n")
            return
        }
        var acc = 0.0
        var tot_ms = 0.0lf
        var max_ms = 0.0lf
        for (_l in range(PLAYERS)) {
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
            maybe_parallel_for(0, PD, 8) $(rb, re) {
                unsafe {
                    if (accel_bnns_hgemm_nt(PM, re - rb, PN, hxp, PN, hwp + int64(rb) * int64(PN), PN, yp + rb, PD) != 0) {
                        bp[0] = -1.0
                    }
                }
            }
            let ms = double(get_time_usec(t0)) / 1000.0lf
            tot_ms += ms
            max_ms = max(max_ms, ms)
            acc += yp[0]
        }
        print("bnns f16 strips avg {tot_ms / double(PLAYERS)} ms  max {max_ms} ms  (checksum {acc})\n")
    }
}
