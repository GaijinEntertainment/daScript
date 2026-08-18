options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// Silero-VAD parity gate: dump one `PROB<TAB>frame<TAB>prob` line per 512-sample chunk,
// diffable against harness/silero_vad_oracle.py (onnxruntime op15). Target max|Δ| ≤ 1e-4.
//   bin/daslang -jit modules/dasLLAMA/harness/silero_parity_probe.das -- <silero_vad.bin> <audio.wav>

require dasllama/dasllama_vad
require dasllama/dasllama_audio
require dasllama/dasllama_math
require audio
require daslib/jobque_boost
require math

def load_audio_16k_mono(path : string) : array<float> {
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, 1u, uint(AUDIO_SAMPLE_RATE))
    if (ma_decoder_init_file(path, unsafe(addr(config)), decoder) != ma_result.MA_SUCCESS) {
        panic("cannot open audio file '{path}'")
    }
    var out : array<float>
    let chunk = 65536
    var buf : array<float>
    buf |> resize(chunk)
    while (true) {
        let got = int(ma_decoder_read_pcm_frames(decoder, unsafe(addr(buf[0])), uint64(chunk)))
        break if (got <= 0)
        out |> reserve(length(out) + got)
        for (i in range(got)) {
            out |> push(buf[i])
        }
        break if (got < chunk)
    }
    ma_decoder_uninit(decoder)
    unsafe {
        delete decoder
    }
    return <- out
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let model_path = args[length(args) - 2]
    let wav_path = args[length(args) - 1]
    let samples <- load_audio_16k_mono(wav_path)
    print("SAMPLES\t{length(samples)}\n")
    with_job_que() {
        setup_dasllama_jobque_()
        var m <- load_vad_model(model_path)
        let probs <- vad_probs(m, samples)
        for (i, p in range(length(probs)), probs) {
            print("PROB\t{i}\t{p}\n")
        }
    }
}
