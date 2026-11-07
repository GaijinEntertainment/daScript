options gen2
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

// The device log-mel (`tower_mel`, `dasllama_tower_chains_g4a`) on the home the whisper family's seats serve on - the CUDA
// home's under the CUDA ask, else the Vulkan home's - against the CPU mel over a synthetic signal: whisper's 80-mel chunks, then
// qwen3a's 128-mel window on the same residency (the scratch rebuilt for the wider filterbank). Model-free; skips without a
// discrete device the home serves.

require dastest/testing_boost public
require math
require dasllama/dasllama_lint public
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_audio   // log_mel_chunks
require dasllama/dasllama_qwen3a   // log_mel_qwen3a
require _gpu_knobs
require _compares

let private MEL_BAR = Bar(abs_floor = 1e-3)   //! the device log-mel's distance from the CPU's after the global clamp: the qwen3a twin's mel bar

//! a 2.5 s signal of three tones under a slow envelope with a seeded noise floor
def private mel_signal(seed : int) : array<float> {
    var s : array<float>
    let n = 40000
    s |> reserve(n)
    var x = uint(seed)
    for (i in range(n)) {
        x = x * 1664525u + 1013904223u
        let tt = float(i) / 16000.0
        let env = 0.5 + 0.5 * sin(tt * 3.1)
        s |> push(env * (0.3 * sin(tt * 2765.0) + 0.2 * sin(tt * 5530.0 + 1.0) + 0.1 * sin(tt * 1170.0)) + 0.01 * (float(x >> 8u) / 16777216.0 - 0.5))
    }
    return <- s
}

//! the mel rows of `chunks` back to back
def private mel_rows(chunks : array<MelChunk>) : array<float> {
    var rows : array<float>
    for (c in chunks) {
        rows |> push_from(c.data)
    }
    return <- rows
}

//! `mel` over `mel_signal(7)` with the knob off (the CPU, no device mel) and on (the home's device): one device mel, no decline,
//! within `MEL_BAR`; the CPU mel of `mel_signal(11)` outside it the control
def private mel_twin(t : T?; tag : string; mel : block<(s : array<float>) : array<float>>) {
    let sig <- mel_signal(7)
    let other <- mel_signal(11)
    set_serving_gpu_tower(false, TowerFamily.whisper)
    let off0 = gpu_tower_counters(TowerFamily.whisper)
    let cpu <- invoke(mel, sig)
    let cpu_other <- invoke(mel, other)
    t |> equal(gpu_tower_counters(TowerFamily.whisper).mels - off0.mels, 0l, "{tag}: the knob-off mels ran on the CPU (the mels counter's control)")
    set_serving_gpu_tower(true, TowerFamily.whisper)
    let c0 = gpu_tower_counters(TowerFamily.whisper)
    let dev <- invoke(mel, sig)
    let c1 = gpu_tower_counters(TowerFamily.whisper)
    t |> equal(c1.mels - c0.mels, 1l, "{tag}: the device mel served (mels delta)")
    t |> equal(c1.declines, c0.declines, "{tag}: no decline")
    check_rows(t, "{tag}, mel_signal(7): the device log-mel against the CPU's", dev, cpu, MEL_BAR, 0)
    check_exceeds(t, "{tag}: the control - the CPU mel of mel_signal(11) against the device's of mel_signal(7)", dev, cpu_other, MEL_BAR)
}

[test]
def test_device_mel_twin(t : T?) {
    t |> run("the device log-mel on the whisper family's home against the CPU mel: 80-mel chunks, then 128-mel rows on one residency") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            if (!gpu_tower_tiles_up(TowerFamily.whisper)) {
                t |> skip("no discrete GPU device the whisper family's home serves (DASLLAMA_GPU=1, DASLLAMA_CUDA=1 for the CUDA home)")
                return
            }
            with_dasllama_jobque_() {
                with_gpu_knobs() {
                    gpu_tower_shutdown(TowerFamily.whisper)
                    mel_twin(t, "whisper 80-mel chunks") $(s) {
                        var inscope chunks <- log_mel_chunks(s, 80l)
                        return <- mel_rows(chunks)
                    }
                    mel_twin(t, "qwen3a 128-mel window") $(s) {
                        var inscope m <- log_mel_qwen3a(s, 128l)
                        return <- clone_to_move(m.data)
                    }
                    gpu_tower_shutdown(TowerFamily.whisper)
                }
            }
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}
