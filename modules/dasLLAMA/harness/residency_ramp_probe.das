options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// ATTRIBUTION SWEEP - the post-CPU-window residency-ramp ladder (ARCHITECTURE.md 2.12): its
// arms attribute the first-submission cost across the drv/queue/gpu split (das_metal_boost's
// metal_submit_trace) and across CPU-window lengths; it selects between no implementations.
// A/B rail: DASLLAMA_METAL_HEARTBEAT_S=0 reproduces the ramp, the default heartbeat holds it.

require math
require daslib/jobque_boost
require dasllama/dasllama_vision
require dasllama/dasllama_vision_embedder
require dasllama/dasllama_metal_tower   // registers the GPU tower hooks ([init]) — without it the load picks the CPU q8 lane
require dasllama/dasllama_env
require metal/das_metal_boost
require ../performance/profile_common.das   // tune_gate — the standing pre-measure stack

def private cpu_burn(ms : int) {
    let t0 = ref_time_ticks()   // clock: control - bounds the burn window, never a reported figure
    var sink = 0.0
    while (get_time_usec(t0) < ms * 1000) {
        for (i in range(1000)) {
            sink += sqrt(float(i))
        }
    }
    to_log(LOG_INFO, "burn {ms} ms done (sink {sink})\n")
}

[export]
def main() {
    with_job_que() {
        probe()
    }
}

def private probe() {
    var dir = g_env_harness.models_dir
    if (empty(dir)) {
        dir = "/Users/borisbatkin/Work/llama.cpp/models/"
    }
    var e <- load_vision_embedder("{dir}mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")
    var st = VisionState()
    var img = VisionImage(width = 640, height = 480)
    img.rgb |> reserve(640l * 480l * 3l)
    img.rgb |> resize(640l * 480l * 3l)
    for (i in range64(long_length(img.rgb))) {
        img.rgb[i] = uint8(i % 251l)
    }
    var out : array<float>
    if (!tune_gate()) {   // never measure on fallback winners
        panic("residency_ramp_probe: tune_gate refused - tune first (or point DAS_TUNE_MANIFEST at a complete sidecar)")
    }
    metal_submit_trace = true
    to_log(LOG_INFO, "== warm x3 ==\n")
    for (r in range(3)) {
        let n = encode_image_(e, st, img, "ramp", out)
        to_log(LOG_INFO, "encode {r}: {n} tokens\n")
    }
    for (burn_ms in [50, 200, 1000, 3000, 3000]) {
        to_log(LOG_INFO, "== burn {burn_ms} ms ==\n")
        cpu_burn(burn_ms)
        encode_image_(e, st, img, "ramp", out)
    }
    delete out
    delete img
    delete st
    delete e
}
