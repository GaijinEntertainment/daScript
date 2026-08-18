options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama
require daslib/clargs
require daslib/jobque_boost
require daslib/fio
require strings
require math

// ASR speed bench: load once, transcribe each file --reps times, one BENCH line per rep.
// Pairs with side_by_side.sh, which interleaves this against whisper-cli / parakeet-cli.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/asr/asr_bench.das -- \
//       --model ~/Work/whisper.cpp/models/ggml-tiny.bin --wav corpus/jfk.wav --reps 3

[CommandLineArgs]
struct BenchArgs {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "ASR model path (ggml-*.bin or GGUF decoder)"
    model : string

    @clarg_doc = "mmproj GGUF for two-file models (qwen3-asr)"
    mmproj : string

    @clarg_required
    @clarg_short = "w"
    @clarg_doc = "Audio file(s) to transcribe (repeatable)"
    wav : array<string>

    @clarg_short = "r"
    @clarg_doc = "Timed repetitions per file (default 3)"
    reps : int

    @clarg_short = "l"
    @clarg_doc = "Language (default en)"
    lang : string

    @clarg_doc = "Print the transcription text of the last rep"
    text : bool

    @clarg_name = "fp32-tower"
    @clarg_doc = "A/B lever: pin every tower to the fp32 reference rail — the q8-vs-f32 profiling comparison's off leg"
    fp32_tower : bool

    @clarg_doc = "Decoder-session KV dtype, LLM-decoder families only (whisper/parakeet have no LLM KV): f16 (default) | f32 | q8"
    kv : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export]
def main() : int {
    var r <- parse_args(type<BenchArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<BenchArgs>), "asr_bench")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<BenchArgs>), "asr_bench")
        return 0
    }
    if (!jit_enabled()) {
        print("This benchmark is JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    let reps = cfg.reps > 0 ? cfg.reps : 3
    let lang = empty(cfg.lang) ? "en" : cfg.lang
    let model_name = base_name(cfg.model)
    if (cfg.fp32_tower) {
        set_asr_fp32(true)
        set_gemma4a_q8(false)
        set_qwen3a_q8(false)
        print("FP32-TOWER\ton (the A/B off leg - every tower on the reference rail)\n")
    }
    if (!empty(cfg.kv)) {
        if (cfg.kv == "f16") {
            set_asr_kv(KVDtype.f16)
        } elif (cfg.kv == "f32") {
            set_asr_kv(KVDtype.f32)
        } elif (cfg.kv == "q8") {
            set_asr_kv(KVDtype.q8_0)
        } else {
            print("unknown --kv '{cfg.kv}' (f16 | f32 | q8)\n")
            return 1
        }
        print("KV\t{cfg.kv}\n")
    }

    let t_load = ref_time_ticks()
    var m <- empty(cfg.mmproj) ? load_asr_model(cfg.model) : load_asr_model(cfg.model, cfg.mmproj)
    let load_ms = double(get_time_usec(t_load)) / 1000.0lf
    print("LOAD\t{model_name}\t{load_ms}\n")

    with_job_que() {
        // pooled forks + team dispatch + spin window — without it every kernel dispatch pays
        // the ~10us/job fifo fork/join (2-7x on the jfk encoder stages)
        setup_dasllama_jobque()
        for (path in cfg.wav) {
            var samples <- load_audio_16k_mono(path)
            let audio_s = double(length(samples)) / double(AUDIO_SAMPLE_RATE)
            let wav_name = base_name(path)
            var last_text = ""
            for (rep in range(reps)) {
                var s = create_session(m, lang)
                let t0 = ref_time_ticks()
                last_text = transcribe(m, s, samples)
                let ms = double(get_time_usec(t0)) / 1000.0lf
                let speed = audio_s / (ms / 1000.0lf)   // x realtime, higher is faster
                print("BENCH\t{model_name}\t{wav_name}\t{audio_s}\t{rep}\t{ms}\t{speed}\n")
            }
            if (cfg.text) {
                print("TEXT\t{wav_name}\t{last_text}\n")
            }
            delete samples
        }
    }
    return 0
}
