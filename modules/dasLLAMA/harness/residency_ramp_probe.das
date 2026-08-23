options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// The post-CPU-window residency-ramp ladder (ARCHITECTURE.md 2.12): run the qwen3v Metal
// tower encode after controlled CPU-only burns and read the submission split from
// das_metal_boost's metal_submit_trace — the ramp lives ONLY in drv (kernelStart→kernelEnd).
// A/B rail: DASLLAMA_METAL_HEARTBEAT_S=0 reproduces the ramp, the default heartbeat holds it.

require math
require daslib/jobque_boost
require dasllama/dasllama_qwen3v
require dasllama/dasllama_metal_tower   // registers the GPU tower hooks ([init]) — without it the load picks the CPU q8 lane
require dasllama/dasllama_env
require metal/das_metal_boost

def private cpu_burn(ms : int) {
    let t0 = ref_time_ticks()
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
    let mm = "{dir}mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf"
    var tw <- load_qwen3v_tower(mm)
    var st = Qwen3vState()
    let w = 640l
    let h = 480l
    var planes : array<float>
    planes |> reserve(w * h * 3l)
    planes |> resize(w * h * 3l)
    for (i in range64(w * h * 3l)) {
        planes[i] = float(i % 251l) / 251.0
    }
    var out : array<float>
    metal_submit_trace = true
    to_log(LOG_INFO, "== warm x3 ==\n")
    for (r in range(3)) {
        let n = qwen3v_encode(tw, st, planes, w, h, out)
        to_log(LOG_INFO, "encode {r}: {n} tokens\n")
    }
    for (burn_ms in [50, 200, 1000, 3000, 3000]) {
        to_log(LOG_INFO, "== burn {burn_ms} ms ==\n")
        cpu_burn(burn_ms)
        qwen3v_encode(tw, st, planes, w, h, out)
    }
    delete out
    delete planes
    delete tw
}
