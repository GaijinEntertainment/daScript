options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_asr
require dasllama/dasllama_audio
require dasllama/dasllama_math
require dasllama/dasllama_common   // forward_profile_reset/report — the decoder-engine buckets
require audio
require math   // the parakeet mel worker resolves log at the instantiation site
require daslib/clargs
require daslib/jobque_boost
require daslib/jobque_profile   // JOBQUE_PROFILING_ENABLED — the probe refuses on a build without the marker rail

// Per-stage ASR profile: one warmup pass, then --reps profiled passes through the engine's
// asr_prof rail (PROF lines: label, total ms, hits, %). Parakeet first; whisper stages ride
// the same rail as they get instrumented.
//
//   bin/daslang -jit modules/dasLLAMA/harness/asr_stage_probe.das -- \
//       -m ~/Work/whisper.cpp/models/ggml-parakeet-tdt-0.6b-v2-f32.bin -w jfk.wav

[CommandLineArgs]
struct ProbeArgs {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "ASR model path"
    model : string

    @clarg_doc = "mmproj GGUF for two-file models"
    mmproj : string

    @clarg_required
    @clarg_short = "w"
    @clarg_doc = "Audio file"
    wav : string

    @clarg_short = "r"
    @clarg_doc = "Profiled repetitions (default 1)"
    reps : int

    @clarg_doc = "Pin fp32 weights (set_asr_fp32) — A/B the q8 default"
    fp32 : bool

    @clarg_doc = "Decoder-session KV dtype, LLM-decoder families only: f16 (default) | f32 | q8"
    kv : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

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

def run_once(m : AsrModel; samples : array<float>) {
    var s = create_session(m, "en")
    transcribe(m, s, samples) $(seg : TranscribeSegment) {
        feint("{seg.text}\n")
    }
}

[export]
def main() : int {
    var r <- parse_args(type<ProbeArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<ProbeArgs>), "asr_stage_probe")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<ProbeArgs>), "asr_stage_probe")
        return 0
    }
    if (!jit_enabled()) {
        print("JIT-only — run with: daslang -jit <file>\n")
        return 1
    }
    set_asr_fp32(cfg.fp32)
    if (cfg.kv == "f16") {
        set_asr_kv(KVDtype.f16)
    } elif (cfg.kv == "f32") {
        set_asr_kv(KVDtype.f32)
    } elif (cfg.kv == "q8") {
        set_asr_kv(KVDtype.q8_0)
    } elif (!empty(cfg.kv)) {
        print("unknown --kv '{cfg.kv}' (f16 | f32 | q8)\n")
        return 1
    }
    if (!JOBQUE_PROFILING_ENABLED) {
        panic("asr_stage_probe needs the marker rail compiled in — rerun with JOBQUE_PROFILING=1 (or --jobque-profiling)")
    }
    var m <- empty(cfg.mmproj) ? load_asr_model(cfg.model) : load_asr_model(cfg.model, cfg.mmproj)
    let samples <- load_audio_16k_mono(cfg.wav)
    let reps = cfg.reps > 0 ? cfg.reps : 1
    with_job_que() {
        setup_dasllama_jobque_()   // the engine dispatch standard — bare with_job_que profiles the
                                   // ~10us/job fifo fork/join instead of the shipped team dispatch
        run_once(m, samples)               // warmup, unprofiled
        set_asr_prof(true)
        forward_profile_reset()   // the decoder engine's own buckets (blk_attn/blk_ffn/mm_*)
        for (_rep in range(reps)) {
            run_once(m, samples)
        }
        asr_prof_report()
        forward_profile_report()
    }
    return 0
}
