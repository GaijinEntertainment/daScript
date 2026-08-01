options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_vad
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_gguf   // rd_u32 / rd_u16 for the WAV reader
require daslib/jobque_boost
require daslib/fio
require math

// Silero-VAD tests. The forward gate is self-contained (the ~1.2 MB weights are checked in at
// modules/dasLLAMA/models/silero_vad.bin) and drives a deterministic integer-LCG signal — the
// expected per-frame probs are frozen from onnxruntime on the official silero_vad_16k_op15.onnx
// (harness/silero_vad_oracle.py). The segment case additionally needs jfk.wav (DASLLAMA_MODELS_DIR)
// and matches silero get_speech_timestamps sample-for-sample. Spec: modules/dasLLAMA/silero_vad_plan.md.

def private vad_bin_path() : string {
    return "{get_das_root()}/modules/dasLLAMA/models/silero_vad.bin"
}

def private samples_dir() : string {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}

// deterministic integer LCG (Numerical Recipes), fp32-mapped to [-0.15, 0.15) — identical to
// the python freeze in the oracle run, so the inputs are bit-for-bit equal on both sides.
def private lcg_signal(n : int) : array<float> {
    var out : array<float>
    out |> resize(n)
    var state = 12345u
    for (i in range(n)) {
        state = state * 1664525u + 1013904223u
        out[i] = (float(state >> 16u) / 32768.0 - 1.0) * 0.15
    }
    return <- out
}

def private read_wav_pcm16_mono(path : string) : array<float> {
    var out : array<float>
    let f = fopen(path, "rb")
    if (f == null) {
        return <- out
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let n = long_length(bytes)
        var o = 12l
        while (o + 8l <= n) {
            let id = rd_u32(bytes, o)
            let sz = int64(rd_u32(bytes, o + 4l))
            if (id == 0x61746164u) {   // "data"
                let cnt = min(sz, n - o - 8l) / 2l
                out |> resize(int(cnt))
                for (i in range64(cnt)) {
                    var v = int(rd_u16(bytes, o + 8l + i * 2l))
                    if (v >= 32768) {
                        v -= 65536
                    }
                    out[i] = float(v) / 32768.0
                }
                break
            }
            o += 8l + sz + (sz & 1l)
        }
    }
    fclose(f)
    return <- out
}

[test]
def test_vad_forward_lcg(t : T?) {
    t |> run("per-frame prob matches onnxruntime op15 on a deterministic LCG signal") @(t : T?) {
        // frozen from harness/silero_vad_oracle.py on the same LCG (10 chunks)
        let expected = [0.01383990, 0.02051842, 0.01915139, 0.01694492, 0.01415426,
                        0.01128137, 0.01160875, 0.01086920, 0.00850144, 0.02220330]
        let sig <- lcg_signal(10 * 512)
        with_job_que() {
            setup_dasllama_jobque_()
            var m <- load_vad_model(vad_bin_path())
            let probs <- vad_probs(m, sig)
            t |> success(length(probs) == 10, "10 chunks -> 10 probs")
            for (i in range(min(length(probs), length(expected)))) {
                t |> success(abs(probs[i] - expected[i]) < 1e-3, "frame {i} prob within 1e-3 of oracle")
            }
        }
    }
}

[test]
def test_vad_segments_jfk(t : T?) {
    t |> run("find_speech_timestamps matches silero get_speech_timestamps on jfk.wav") @(t : T?) {
        let wav = "{samples_dir()}/jfk.wav"
        let samples <- read_wav_pcm16_mono(wav)
        if (empty(samples)) {
            t |> skip("jfk.wav not present in DASLLAMA_MODELS_DIR")
            return
        }
        // silero get_speech_timestamps defaults, sample indices (harness/silero_segment_oracle.py)
        let exp_start = [5152l, 52256l, 86048l, 130592l]
        let exp_end   = [36320l, 71136l, 122848l, 169952l]
        with_job_que() {
            setup_dasllama_jobque_()
            var m <- load_vad_model(vad_bin_path())
            let probs <- vad_probs(m, samples)
            // decoder-robust smoke checks (exact per-frame values are gated by the LCG test);
            // the WAV reader here decodes marginally differently from the oracle's soundfile.
            t |> success(probs[0] < 0.05, "jfk frame 0 (silence) low")
            t |> success(probs[11] > 0.5, "jfk frame 11 (speech onset) high")
            let segs <- find_speech_timestamps(m, samples, default_vad_opts())
            t |> success(length(segs) == 4, "4 speech segments")
            for (i in range(min(length(segs), 4))) {
                t |> success(segs[i].start == exp_start[i], "segment {i} start")
                t |> success(segs[i].end == exp_end[i], "segment {i} end")
            }
        }
    }
}
