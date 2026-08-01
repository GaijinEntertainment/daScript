options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// Qwen3-Omni-30B transcribe A/B (das side): warmup + best-of-N transcribe ms, encode/decode
// split via asr_prof, xRT vs clip duration. Pair with llama-mtmd-cli for the section A/B.
//   bin/daslang -jit modules/dasLLAMA/harness/q3omni_bench.das -- <thinker.gguf> <mmproj.gguf> <audio.wav> [reps]

require dasllama/dasllama_asr
require dasllama/dasllama_audio
require dasllama/dasllama_math
require audio
require daslib/jobque_boost
require strings
require daslib/strings_boost
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
    let args = get_command_line_arguments()
    var reps = 3
    var base = length(args) - 3
    if (length(args) >= 4 && to_int(args[length(args) - 1]) > 0) {
        reps = to_int(args[length(args) - 1])
        base = length(args) - 4
    }
    let dec = args[base]
    let mmproj = args[base + 1]
    let wav = args[base + 2]
    var m <- load_asr_model(dec, mmproj)
    let samples <- load_audio_16k_mono(wav)
    let audio_s = double(length(samples)) / double(AUDIO_SAMPLE_RATE)
    with_job_que() {
        setup_dasllama_jobque_()
        // warmup (JIT compile + first-touch)
        var s0 <- create_session(m, "auto")
        var ntok = 0
        transcribe(m, s0, samples) $(seg) {
            ntok = length(seg.tokens)
        }
        var best_ms = 1.0e30lf
        set_asr_prof(true)
        for (r in range(reps)) {
            var s <- create_session(m, "auto")
            let t0 = ref_time_ticks()
            transcribe(m, s, samples) $(seg) {
                ntok = length(seg.tokens)
            }
            let ms = double(get_time_usec(t0)) / 1000.0lf
            best_ms = min(best_ms, ms)
            print("REP {r} {ms} ms\n")
        }
        let xrt = audio_s / (best_ms / 1000.0lf)
        print("BENCH audio_s {audio_s} tokens {ntok} best_ms {best_ms} xRT {xrt}\n")
        asr_prof_report()
    }
    delete m.dec.wblob
}
