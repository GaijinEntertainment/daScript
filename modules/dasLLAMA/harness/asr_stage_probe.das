options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_asr
require dasllama/dasllama_audio
require dasllama/dasllama_audio_io   // load_audio_16k_mono
require dasllama/dasllama_tower
require dasllama/dasllama_math
require dasllama/dasllama_vad
require dasllama/dasllama_common   // forward_profile_reset/report — the decoder-engine buckets
require ?das_metal dasllama/dasllama_metal_asr_dec   // set_metal_wdec_step — the all-Metal probe arm
require ?das_metal dasllama/dasllama_metal_tower     // set_metal_tower: the CPU lanes pin the CPU tower
require ?das_metal dasllama/dasllama_metal_common    // metal_dispatch_calls: a Metal encode's dispatch count
require ../performance/profile_common.das   // tune_gate + apply_box_profile_runtime, the standing pre-measure stack
require _row_stats   // spread_of: the row statistic over reps
require math   // the parakeet mel worker resolves log at the instantiation site
require daslib/clargs
require daslib/jobque_boost
require daslib/jobque_profile   // JOBQUE_PROFILING_ENABLED — the decoder buckets and --trace need the marker rail
require daslib/fio
require strings

// The ASR stage instrument, any `load_asr_model` family: one clip, the model loaded off the clock, `--warm` untimed passes
// (`--settle` seconds idle first), then `--reps` profiled passes through the engine's asr_prof rail - each pass's
// encode-only bucket and dispatch count, the family's encode row, then the stage table (PROF lines: label, total ms, hits,
// %) and the decoder engine's buckets. `--e2e` puts `--reps` end-to-end walls of `create_session` + `transcribe` with the
// rail off ahead of the profiled passes; `--vad` puts `--reps` passes of the silero VAD over the whole clip behind them
// (`--vad-only`: those rows alone, no model). Each timed row prints its mean, min-max and cv over the reps, VOID past 3%;
// a run with a timed row passes the tune gate first. `--lane` pins cpu-q8 (the CPU tower's q8 lane), cpu-f16 (its exact
// planes) or metal (the Metal tower on the exact planes); unpinned, the build's defaults serve.
//
//   bin/daslang -jit modules/dasLLAMA/harness/asr_stage_probe.das -- \
//       -m ~/Work/whisper.cpp/models/ggml-parakeet-tdt-0.6b-v2-f32.bin -w jfk.wav -r 5
//   DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m5.tune.json bin/daslang -jit modules/dasLLAMA/harness/asr_stage_probe.das -- \
//       -m ~/Work/llama.cpp/models/gigaam-v3-e2e-rnnt-f16.gguf -w modules/dasLLAMA/benchmarks/asr/corpus/ru_long.wav \
//       --lane metal --lang ru -r 5 --e2e --vad

[CommandLineArgs]
struct ProbeArgs {
    @clarg_short = "m"
    @clarg_doc = "ASR model path (every run but --vad-only)"
    model : string

    @clarg_doc = "mmproj GGUF for two-file models"
    mmproj : string

    @clarg_required
    @clarg_short = "w"
    @clarg_doc = "Audio file"
    wav : string

    @clarg_short = "r"
    @clarg_doc = "Passes a row (default 1)"
    reps : int

    @clarg_doc = "Untimed passes before the first row (default 1)"
    warm : int

    @clarg_doc = "Seconds idle before the warm-ups - the settled box state (default 0)"
    settle : int

    @clarg_doc = "Pin a lane: cpu-q8 | cpu-f16 | metal (default: the build's own)"
    lane : string

    @clarg_doc = "Time --reps end-to-end create_session + transcribe walls with the asr_prof rail off"
    e2e : bool

    @clarg_doc = "Time the silero VAD over the whole clip alone"
    vad : bool

    @clarg_name = "vad-only"
    @clarg_doc = "The VAD rows alone: no model load, no transcription"
    vad_only : bool

    @clarg_doc = "Metal: replay the rotary chain's head restrides and slab trio after each profiled encode for their device time"
    replay : bool

    @clarg_doc = "Pin fp32 weights (set_asr_fp32) — A/B the q8 default"
    fp32 : bool

    @clarg_name = "fp32-tower"
    @clarg_doc = "Pin fp32 on the tower half only (set_asr_tower_fp32) — the GPU serving mode"
    fp32_tower : bool

    @clarg_name = "wdec-step"
    @clarg_doc = "Arm the GPU whisper decode step (set_metal_wdec_step) — the all-Metal probe arm"
    wdec_step : bool

    @clarg_doc = "Decoder-session KV dtype, LLM-decoder families only: f16 (default) | f32 | q8"
    kv : string

    @clarg_doc = "Save a jobque lane trace of the profiled reps to this path (perfetto JSON, encoder stages colored)"
    trace : string

    @clarg_doc = "Session language (default en; Qwen3-ASR needs auto)"
    lang : string = "en"

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! one pass: a session, the clip transcribed, the transcript
def private transcribe_pass(m : AsrModel; samples : array<float>; lang : string) : string {
    var s <- create_session(m, lang)
    var inscope parts : array<string>
    transcribe(m, s, samples) $(seg) {
        parts |> push(seg.text)
    }
    delete s
    return join(parts, "")
}

def private row(what : string; var ms : array<float>; audio_s : float) {
    let sp = spread_of(ms)
    let void_tag = sp.cv > 3.0 ? " VOID" : ""
    print("ROW {what}: {sp.mean:.2f} ms mean ({sp.best:.2f}-{sp.worst:.2f}), cv {sp.cv:.2f}% over {length(ms)} reps, {audio_s * 1000.0 / sp.mean:.1f}x real time{void_tag}\n")
}

//! the lane's pins: false on a lane this build cannot serve; "" pins nothing
def private pin_lane(lane : string; replay : bool) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_fc_prof_replay(replay)
    }
    return true if (empty(lane))
    let cpu = lane == "cpu-q8" || lane == "cpu-f16"
    return false if (!cpu && lane != "metal")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(!cpu)
    } else {
        return false if (!cpu)
    }
    set_asr_tower_fp32(lane != "cpu-q8")
    return true
}

//! the KV dtype pin; false on a name it does not know
def private pin_kv(kv : string) : bool {
    if (kv == "f16") {
        set_asr_kv(KVDtype.f16)
    } elif (kv == "f32") {
        set_asr_kv(KVDtype.f32)
    } elif (kv == "q8") {
        set_asr_kv(KVDtype.q8_0)
    } elif (!empty(kv)) {
        return false
    }
    return true
}

def private dispatch_count() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_dispatch_calls()
    } else {
        return 0l
    }
}

//! the end-to-end row with the rail off; answers the last pass's transcript
def private e2e_row(cfg : ProbeArgs; m : AsrModel; samples : array<float>; audio_s : float) : string {
    var text = ""
    var inscope e2e : array<float>
    e2e |> reserve(cfg.reps)
    for (_rep in range(cfg.reps)) {
        let t0 = ref_time_ticks()
        text = transcribe_pass(m, samples, cfg.lang)
        e2e |> push(float(get_time_usec(t0)) / 1000.0)
    }
    row("e2e", e2e, audio_s)
    return text
}

//! the profiled passes: each pass's encode bucket and dispatches, the encode row, then the stage and decoder-engine tables
def private prof_rows(cfg : ProbeArgs; m : AsrModel; samples : array<float>; audio_s : float) {
    let bucket = asr_encode_bucket(m)
    if (!empty(cfg.trace)) {
        jobque_trace_start(1 << 21)   // encoder chunks are coarse — a couple of reps fit easily
        set_trace_tags(true)
    }
    set_asr_prof(true)
    asr_prof_reset()
    static_if (JOBQUE_PROFILING_ENABLED) {
        forward_profile_reset()   // the decoder engine's own buckets (blk_attn/blk_ffn/mm_*)
    } else {
        print("note: decoder-engine buckets need JOBQUE_PROFILING=1 - printing the ASR stage table only\n")
    }
    var inscope enc : array<float>
    enc |> reserve(cfg.reps)
    var prev = 0.0lf
    for (rep in range(cfg.reps)) {
        let dc0 = dispatch_count()
        transcribe_pass(m, samples, cfg.lang)
        let total = asr_prof_ms(bucket)
        enc |> push(float(total - prev))
        print("PASS {rep} encode {total - prev:.3f} ms, dispatches {dispatch_count() - dc0}\n")
        prev = total
    }
    if (!empty(cfg.trace)) {
        set_trace_tags(false)
        jobque_trace_stop()
        print(jobque_trace_save(cfg.trace) ? "jobque trace saved: {cfg.trace}\n" : "jobque trace: nothing recorded\n")
    }
    if (!empty(bucket)) {
        row("encode ({bucket})", enc, audio_s)
    }
    print("PROFREPS {cfg.reps}\n")
    asr_prof_report()
    forward_profile_report()
    set_asr_prof(false)
}

//! the silero VAD over the whole clip alone, its first pass off the clock: the served route (the GPU seat where the lane's
//! driver serves it), then the CPU chunk step, then the streaming feed a 32 ms chunk a call on both routes
def private vad_row(reps : int; samples : array<float>; audio_s : float) {
    var inscope v <- load_vad_model(vad_model_path())
    var inscope vad_ms : array<float>
    vad_ms |> reserve(reps)
    var n_seg = 0
    for (rep in range(reps + 1)) {
        let dc0 = dispatch_count()
        let t0 = ref_time_ticks()
        var inscope segs <- find_speech_timestamps(v, samples, default_vad_opts())
        if (rep > 0) {
            vad_ms |> push(float(get_time_usec(t0)) / 1000.0)
        } else {
            print("VAD served pass: {dispatch_count() - dc0} dispatches\n")
        }
        n_seg = length(segs)
    }
    print("VAD {n_seg} speech segments\n")
    row("vad (find_speech_timestamps)", vad_ms, audio_s)
    var inscope cpu_ms : array<float>
    for (rep in range(reps + 1)) {
        let t0 = ref_time_ticks()
        var inscope probs <- vad_probs_cpu(v, samples)
        var inscope segs <- speech_timestamps_of(probs, long_length(samples), default_vad_opts())
        if (rep > 0) {
            cpu_ms |> push(float(get_time_usec(t0)) / 1000.0)
        }
    }
    row("vad cpu (the chunk step)", cpu_ms, audio_s)
    let was = set_vad_stream_gpu(false)
    vad_stream_row(v, samples, "cpu")
    set_vad_stream_gpu(true)
    vad_stream_row(v, samples, "device")
    set_vad_stream_gpu(was)
}

//! the streaming feed one 512-sample chunk a call over the clip, every call's latency: median, p99 and cv in microseconds
def private vad_stream_row(v : VadModel; samples : array<float>; route : string) {
    var it : VadIter
    vad_iter_reset(it, default_vad_opts())
    var inscope us, part : array<float>
    part |> resize(512)
    let nch = length(samples) / 512
    var events = 0
    for (c in range(nch)) {
        for (i in range(512)) {
            part[i] = samples[c * 512 + i]
        }
        let t0 = ref_time_ticks()
        vad_iter_feed(v, it, part) $(_ev) {
            events++
        }
        if (c > 0) {
            us |> push(float(get_time_usec(t0)))
        }
    }
    let sp = spread_of(us)
    print("ROW vad stream {route}: median {sp.median:.1f} us, p99 {us[length(us) * 99 / 100]:.1f} us, mean {sp.mean:.1f} us, cv {sp.cv:.1f}% over {length(us)} chunks, {events} events\n")
}

//! the pins the flags ask for; "" when they all took, else why not
def private apply_pins(cfg : ProbeArgs) : string {
    set_asr_fp32(cfg.fp32)
    set_asr_tower_fp32(cfg.fp32_tower)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (cfg.wdec_step) {
            set_metal_wdec_step(true)
        }
    }
    return "unknown --kv '{cfg.kv}' (f16 | f32 | q8)" if (!pin_kv(cfg.kv))
    return "lane '{cfg.lane}' is not one this build serves (cpu-q8 | cpu-f16 | metal)" if (!pin_lane(cfg.lane, cfg.replay))
    return "--trace needs the marker rail compiled in - rerun with JOBQUE_PROFILING=1 (or --jobque-profiling)" if (!empty(cfg.trace) && !JOBQUE_PROFILING_ENABLED)
    return "-m names the model on every run but --vad-only" if (empty(cfg.model) && !cfg.vad_only)
    apply_box_profile_runtime()
    return "tune_gate refused - point DAS_TUNE_MANIFEST at a complete sidecar" if ((cfg.e2e || cfg.vad || cfg.vad_only) && !tune_gate())
    return ""
}

[export, cold_path]
def main() : int {
    allow_cpu_prefill()   // a CPU-stage instrument by design: the LLM-decoder families (qwen3a, canary) prefill encoder rows on the CPU
    var r <- parse_args(type<ProbeArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<ProbeArgs>), "asr_stage_probe")
        return 1
    }
    var cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<ProbeArgs>), "asr_stage_probe")
        return 0
    }
    if (!jit_enabled()) {
        print("JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    cfg.reps = max(cfg.reps, 1)
    cfg.warm = max(cfg.warm, 1)
    let why = apply_pins(cfg)
    if (!empty(why)) {
        print("asr_stage_probe: {why}\n")
        return 1
    }
    let samples <- load_audio_16k_mono(cfg.wav)
    let audio_s = float(length(samples)) / float(AUDIO_SAMPLE_RATE)
    if (cfg.vad_only) {
        print("BENCH VAD alone, clip {base_name(cfg.wav)}, lane {cfg.lane}\n")
        with_dasllama_jobque_() {
            vad_row(cfg.reps, samples, audio_s)
        }
        return 0
    }
    let t_load = ref_time_ticks()
    var m <- empty(cfg.mmproj) ? load_asr_model(cfg.model) : load_asr_model(cfg.model, cfg.mmproj)
    let load_ms = float(get_time_usec(t_load)) / 1000.0
    print("BENCH model {base_name(cfg.model)}, clip {base_name(cfg.wav)} ({audio_s:.2f} s), lane {cfg.lane}, exec {asr_exec_fmt(m)}, load {load_ms:.1f} ms\n")
    with_dasllama_jobque_() {
        print("BENCH lanes {get_dispatch_lanes()}, workers {get_total_hw_jobs()}, settle {cfg.settle} s, warm {cfg.warm}\n")
        sleep(uint(max(cfg.settle, 0)) * 1000u)
        var text = ""
        for (_w in range(cfg.warm)) {
            text = transcribe_pass(m, samples, cfg.lang)
        }
        if (cfg.e2e) {
            text = e2e_row(cfg, m, samples, audio_s)
        }
        print("TEXT {text}\n")
        prof_rows(cfg, m, samples, audio_s)
        if (cfg.vad) {
            vad_row(cfg.reps, samples, audio_s)
        }
    }
    return 0
}
