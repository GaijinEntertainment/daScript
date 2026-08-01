options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _function_length = 0   // cell main; absorbed into lcpp_bench by the single-rig consolidation

// One ASR das cell for the record store — the audio sibling of lcpp_bench's --json-path protocol.
// Loads ONE catalog model, times transcribe (best-of-reps) over its corpus buckets under the tuned
// per-box runtime, and emits a BenchModel cell JSON the orchestrator (gen_bench_records) absorbs.
// The reference tools are NOT run here — the orchestrator measures them adjacent to this cell,
// exactly as it does for llama.cpp on the LLM legs.
//
// NOTE: unlike lcpp_bench (LLM), this does NOT set _jit_fast_math. LLM emission is a fixed token
// count (content-independent), so fast-math only shifts speed; ASR transcription length is
// content-dependent, so a fast-math token flip would change the decode loop length and the timing.
//   bin/daslang -jit modules/dasLLAMA/benchmarks/asr/asr_bench_cell.das -- -m "Whisper tiny" -o json

require dasllama/dasllama        // nolint:STYLE029 — umbrella pulls the ASR API + every arch [init]
require dasllama/dasllama_math   // setup_dasllama_jobque_ + apply_box_profile_runtime (engine-level, not re-exported)
require dasllama/dasllama_tune   // box_profile_path (the applied per-box manifest)
require llvm/daslib/llvm_tune    // [tune_policy] — the box tunes itself before measuring
require audio
require daslib/jobque_boost
require ../../performance/profile_common.das
require daslib/clargs
require daslib/fio
require strings
require math

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Catalog model to measure — substring of the display name or das_model filename (required)"
    model : string = ""

    @clarg_short = "r"
    @clarg_doc = "Timed repetitions per clip; the cell keeps the best (default 2)"
    reps : int = 2

    @clarg_short = "t"
    @clarg_doc = "das lane count (0 = the box policy, min(16, physical performance cores))"
    threads : int = 0

    @clarg_doc = "Write the cell records JSON here (also printed to stdout)"
    json_path : string = ""

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// Decode any file miniaudio understands to 16 kHz mono f32 (transcribe.das / asr_bench rail).
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
    delete buf
    return <- out
}

// Best-of-`reps` transcribe ms for one loaded model over `samples`, from a warmed JIT + live jobque.
def measure_das(var m : AsrModel; samples : array<float>; reps : int; lang : string) : double {
    var best = 1.0e30lf
    for (_rep in range(reps)) {
        var s <- create_session(m, lang)
        let t0 = ref_time_ticks()
        var ntok = 0
        transcribe(m, s, samples) $(seg) {
            ntok += length(seg.tokens)
        }
        let ms = double(get_time_usec(t0)) / 1000.0lf
        best = min(best, ms)
        delete s
    }
    return best
}

// tune_policy: an untuned box silently publishes fallback-tier numbers — see gen_profile.das
[export, tune_policy(missing = "auto")]
def main() : int {
    allow_cpu_prefill()   // CPU rows are this tool's product — disarm the Metal-build tripwire
    if (!jit_enabled()) {
        print("asr_bench_cell is JIT-only — run with: daslang -jit <file>\n")
        return 1
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "asr_bench_cell")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "asr_bench_cell")
        return 0
    }
    if (empty(cfg.model)) {
        print("asr_bench_cell: -m <model> is required (a display-name or das_model substring)\n")
        return 1
    }

    // the standing per-box methodology (lcpp_bench's pattern): -t is the explicit ask, but
    // DAS_JOBQUE_THREADS / DAS_JOBQUE_AFFINITY envs still win the que's actual lane count
    let t_env = has_env_variable("DAS_JOBQUE_THREADS")
    var threads = cfg.threads > 0 ? cfg.threads : (t_env ? to_int(get_env_variable("DAS_JOBQUE_THREADS")) : profile_threads())
    if (t_env && to_int(get_env_variable("DAS_JOBQUE_THREADS")) != threads) {
        threads = to_int(get_env_variable("DAS_JOBQUE_THREADS"))
        print("warning: DAS_JOBQUE_THREADS overrides -t — running {threads} lanes\n")
    } elif (!t_env) {
        set_jobque_threads_cap(max(threads - 1, 1))
    }
    if (affinity_on() && !has_env_variable("DAS_JOBQUE_AFFINITY")) {
        set_jobque_affinity(2)   // 2 = hard mask, the llama-bench --cpu-strict mirror
    }
    apply_box_profile_runtime()   // the tuned per-box stack — what load_asr_model users get
    if (!tune_gate()) {   // never measure on fallback winners — the whole board would sandbag
        return 2
    }

    var found = false
    var jsonModels : array<BenchModel>
    with_job_que() {
        setup_dasllama_jobque_()
        for (spec in asr_catalog()) {
            if (find(spec.display, cfg.model) < 0 && find(spec.das_model, cfg.model) < 0) {
                continue
            }
            let model_path = "{spec.root}{spec.das_model}"
            if (!stat(model_path).is_valid) {
                print("asr_bench_cell: {spec.display} — model absent ({model_path}), skipped\n")
                continue
            }
            found = true
            var m <- (empty(spec.das_mmproj)
                ? load_asr_model(model_path)
                : load_asr_model(model_path, "{spec.root}{spec.das_mmproj}"))
            // warmup (JIT compile + first-touch) on the shortest bucket
            var warm <- load_audio_16k_mono("{asr_corpus_dir()}{spec.buckets[0]}")
            measure_das(m, warm, 1, spec.lang)
            delete warm

            var run = BenchRun(
                engine = "das",
                backend = "cpu",
                flavor = "tuned",
                workload = asr_workload(spec),
                box = box_name(),
                threads = threads,
                date = format_time(get_clock(), "%Y-%m-%d"),
                cmd = bench_cmd_line(),
                env = bench_env_line(),
                sha = run_capture("git rev-parse --short=9 HEAD", true),
                version = das_version(),
                tune = tune_summary(),
                hardware = gather_hardware())
            run.files |> emplace(file_identity(model_path, "weights"))
            if (!empty(spec.das_mmproj)) {
                run.files |> emplace(file_identity("{spec.root}{spec.das_mmproj}", "mmproj"))
            }
            for (wav in spec.buckets) {
                var samples <- load_audio_16k_mono("{asr_corpus_dir()}{wav}")
                let audio_s = double(length(samples)) / double(AUDIO_SAMPLE_RATE)
                let das_ms = measure_das(m, samples, cfg.reps, spec.lang)
                run.tests |> insert("asr:{wav}", BenchTest(ms = das_ms, audio_s = audio_s))
                run.files |> emplace(file_identity("{asr_corpus_dir()}{wav}", "clip"))
                print("asr_bench_cell: {spec.display} {wav} {das_ms} ms ({audio_s} s audio)\n")
                delete samples
            }
            if (!empty(spec.libri_short)) {
                // the dictation aggregate: 25 short clips, ONE whole-set total on one test key
                var lib <- libri_wavs()
                if (empty(lib)) {
                    print("asr_bench_cell: {spec.display} — librispeech set absent/partial (setup_asr_rig --libri), leg skipped\n")
                } else {
                    var lib_ms = 0.0lf
                    var lib_audio = 0.0lf
                    for (wav in lib) {
                        var samples <- load_audio_16k_mono("{asr_corpus_dir()}{wav}")
                        let audio_s = double(length(samples)) / double(AUDIO_SAMPLE_RATE)
                        let das_ms = measure_das(m, samples, cfg.reps, spec.lang)
                        lib_ms += das_ms
                        lib_audio += audio_s
                        run.files |> emplace(file_identity("{asr_corpus_dir()}{wav}", "clip"))
                        print("asr_bench_cell: {spec.display} {wav} {das_ms} ms ({audio_s} s audio)\n")
                        delete samples
                    }
                    run.tests |> insert(libri_test_key(), BenchTest(ms = lib_ms, audio_s = lib_audio))
                    print("asr_bench_cell: {spec.display} librispeech25 total {lib_ms} ms ({lib_audio} s audio, {length(lib)} clips)\n")
                }
                delete lib
            }
            var cell = BenchModel(
                gguf = spec.das_model,
                arch = spec.display,
                size_bytes = int64(stat(model_path).size))
            cell.runs |> emplace(run)
            jsonModels |> emplace(cell)
            delete m   // free the loaded model's weights before a possible next match
        }
    }
    if (!found) {
        print("asr_bench_cell: no catalog model matches '{cfg.model}'\n")
        return 1
    }
    let text = sprint_json(jsonModels, true)
    if (!empty(cfg.json_path)) {
        var wrote = false
        fopen(cfg.json_path, "wb") $(f) {
            if (f != null) {
                fwrite(f, text)
                wrote = true
            }
        }
        if (!wrote) {
            print("warning: could not open {cfg.json_path} for writing — no records file\n")
        }
    }
    print("{text}\n")
    return 0
}
