options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — matches how llama.cpp is benched
options _cyclomatic_complexity = 0   // bench cell main = the measurement protocol ladder
options _function_length = 0   // same
options _ascii_strings = false   // the bench report is human-facing: the markdown/console rows print the mean +- sd with a real plus-minus sign
options log_compile_time = true   // front-end compile wall + the jit backend's per-phase breakdown (llvm_jit_run reads the same option)

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ + apply_box_profile_runtime (engine-level, not re-exported)
require dasllama/dasllama_image   // load_model_cached — the .dlim rail, without the full facade's compile weight
require dasllama/dasllama_tokenizer   // the tg-real row needs REAL tokens (see Lever.real)
require dasllama/dasllama_asr   // --asr: load_asr_model/create_session/transcribe (narrower than the full facade; it requires the transformer umbrella itself)
require audio   // --asr: the miniaudio decoder behind load_audio_16k_mono
require dasllama/dasllama_audio   // --asr: AUDIO_SAMPLE_RATE, the 16 kHz mono contract
require dasllama/dasllama_chat   // --image: the chat renderer + the two-span image turn
require dasllama/dasllama_gemma4uv   // --image: the vision embedder + encode_image_
require dasllama/dasllama_vision   // --image: VisionImage — the decoded-pixel carrier
require dasllama/dasllama_vision_io   // --image: decode the picture to RGB8
require dasllama/dasllama_sampling   // --image: generate_embd_ over the spliced prompt
require daslib/module_path            // get_this_module_dir — the default prompt corpus beside this bench
require daslib/jobque_profile   // JOBQUE_PROFILING_ENABLED — --prof refuses on a build without the marker rail
require ?das_accelerate dasllama/dasllama_math_accelerate   // the --accel leg; Apple-only C++ module
require ../performance/profile_common.das   // profile_threads + affinity_on + bench records — the standing per-box methodology
require ../performance/exchange_client.das   // --submit: POST the stamped records to the exchange
require llvm/daslib/llvm_tune   // tune_app_script — the exe-vs-script discriminator behind --for-debug-purposes
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require daslib/md_boost
require math
require dasllama/dasllama_env

// llama-bench mirror — EXACTLY llama.cpp's test shapes, rep counts, and timing boundaries, and
// NOTHING else. Two independent rows, each = one untimed warmup + -r timed reps, mean ± stdev:
//   pp: one batched forward_prefill of -p tokens from an empty cache per rep
//   tg: -n single-token forwards at pos 0.. per rep, feeding synthetic ids with NO logit read
//       (llama-bench never samples — sampling cost is excluded by design)
//   ours:   bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <model.gguf>
//   theirs: --ref <llama-bench> runs it on the same GGUF with the same -t/-p/-n/-r/--ngl
// `-o json` emits public bench records (schema: METHODOLOGY.md "The record") on stdout — progress goes to
// stderr then, like llama-bench itself. `--ngl` != 0 is the whole graph on Metal
// (MetalMode.required — a CPU fallback fails the run, never silently mismeasures).

enum private OutFmt {
    txt
    md
    json
}

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_short = "p"
    @clarg_doc = "Prompt tokens per pp rep (default: 512, llama-bench -p)"
    plen : int = 512

    @clarg_short = "n"
    @clarg_doc = "Generated tokens per tg rep (default: 128, llama-bench -n)"
    ngen : int = 128

    @clarg_short = "r"
    @clarg_doc = "Timed repetitions per row (default: 5, llama-bench -r)"
    reps : int = 5

    @clarg_short = "t"
    @clarg_doc = "Worker threads (default: the box profile; DAS_JOBQUE_THREADS still wins)"
    threads : int = 0

    @clarg_doc = "GPU layers, llama-bench spelling: 0 = CPU, anything else = Metal, required (default: 0)"
    ngl : int = 0

    @clarg_doc = "Arm the accelerate (+AMX/SME) float-batch tier for the das run — the 'cpu + AMX' board row (records flavor 'accel'; CPU only, fails where das_accelerate is not built)"
    accel : bool

    @clarg_short = "o"
    @clarg_doc = "Output format: txt, md (llama-bench-style table), json (public bench records) (default: txt)"
    output : OutFmt = OutFmt.txt

    @clarg_doc = "Path to a llama-bench binary — also bench llama.cpp on the same GGUF, same shapes (with --tok: a llama-tokenize binary instead)"
    ref : string

    @clarg_name = "ref-flavor"
    @clarg_doc = "The --ref binary's build flavor tag for the record: stock | clean-cpu (default: stock)"
    ref_flavor : string = "stock"

    @clarg_name = "ref-no-affinity"
    @clarg_doc = "Drop --cpu-mask/--cpu-strict from the --ref invocation (see gen_bench_records --ref-no-affinity)"
    ref_no_affinity : bool

    @clarg_name = "json-path"
    @clarg_doc = "With -o json: also write the records to this file — the machine rail (stdout carries engine logs too, and an untuned box's tune-relaunch merges stderr into it)"
    json_path : string

    @clarg_doc = "With -o json: submit the records to the sidecar exchange (dasllama.io; DASLLAMA_EXCHANGE_URL overrides). Non-zero exit when the submit is refused"
    submit : bool

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_doc = "A/B the dense GPU rail in-process: every row runs twice, dense routing OFF then ON. Needs DASLLAMA_GPU_DENSE=1 (uploads + marks); both arms share one process/residency, so cross-run pp drift cancels"
    dense_ab : bool

    @clarg_doc = "A/B the device-side moe combine in-process: every row runs twice, combine OFF then ON (same drift-cancel rationale as --dense-ab; mutually exclusive with it)"
    comb_ab : bool

    @clarg_doc = "A/B the deltanet GPU chain in-process: every row runs twice, DN routing OFF then ON. Needs DASLLAMA_GPU_DN=1 (uploads + marks); mutually exclusive with the other A/B levers"
    dn_ab : bool

    @clarg_doc = "A/B the attention GPU chain in-process: every row runs twice, attention routing OFF then ON. Needs DASLLAMA_GPU_ATTN=1 (uploads + marks); mutually exclusive with the other A/B levers"
    attn_ab : bool

    @clarg_doc = "A/B the GPU MoE prefill arm in-process: every row runs twice, device-side prefill OFF then ON. Needs resident expert layers (DASLLAMA_GPU_MOE_LAYERS); decode has no npos gate so tg reads as a wash. Mutually exclusive with the other A/B levers"
    prefill_ab : bool

    @clarg_name = "mtp-ab"
    @clarg_doc = "A/B MTP/NextN self-speculation in-process, OFF then ON (needs a NextN head in the GGUF). Runs the tg-real row, NOT the llama-bench tg row — acceptance is meaningless on synthetic ids. Reports the draft accept rate; mutually exclusive with the other A/B levers"
    mtp_ab : bool

    @clarg_doc = "A/B the resident ladder's per-token re-encode in-process: every row runs twice, recorded-once then re-encoded every token. Prices the carrier class twins' push-borne pos/cnt (vulkan resident decode only); mutually exclusive with the other A/B levers"
    rerecord_ab : bool

    @clarg_doc = "Refuse to mint artifacts: load only an existing prepared .dlim image, panic otherwise (the gen_bench_records --oracle contract)"
    frozen : bool

    @clarg_name = "for-debug-purposes"
    @clarg_doc = "Allow running as a -jit SCRIPT — a debug instrument only: output is stamped debug-jit and must never enter records. Measurement runs the released exe"
    for_debug_purposes : bool

    @clarg_doc = "ASR cell mode: -m becomes a CATALOG substring (display name or das_model), not a path. Times transcribe best-of--r over the model's corpus buckets and emits the same BenchModel cell the LLM rows use"
    asr : bool

    @clarg_doc = "Image cell mode: the image TURN on -m (a vision decoder path) — embedder encode, the spliced prefill, and decode, as one image-chat cell. Needs --image-mmproj"
    image : string

    @clarg_name = "image-mmproj"
    @clarg_doc = "Image cell: the vision mmproj (gemma4uv) paired with -m"
    image_mmproj : string

    @clarg_doc = "Image cell: the question asked about the picture (default: a short describe prompt)"
    image_prompt : string = ""

    @clarg_doc = "Tokenizer cell mode: encode/decode throughput of -m's tokenizer over corpus x size ladders (no weights loaded; txt output only; --ref here is a llama-tokenize binary)"
    tok : bool

    @clarg_name = "tok-corpus"
    @clarg_doc = "Tokenizer cell: comma list of corpus-name substrings to run (default: all of prose,code,cjk,digits,longword)"
    tok_corpus : string = ""

    @clarg_name = "tok-max-kb"
    @clarg_doc = "Tokenizer cell: size-ladder cap in KB (default: 1024)"
    tok_max_kb : int = 1024

    @clarg_doc = "Prompt corpus for the tg-real row (segments split on lines of two percent signs; default: data/wikitext2_prompts.txt beside this bench)"
    prompts : string = ""

    @clarg_doc = "After the pp row and each tg-real arm's timed reps, run one UNTIMED window with forward_profile buckets — the per-section time split of that row — and print the dispatch census (backend caps, which MoE prefill arm each layer takes) after load. With --mtp-ab this is what attributes draft vs verify vs classifier"
    prof : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// ===== ASR cell mode (--asr) =====
// The audio sibling of the LLM protocol below, folded in by the single-rig consolidation: ONE
// process, one tune-key demand, one image-identity stamp. It runs under this file's
// `options _jit_fast_math` — the same ggml FP laxity whisper-cli/parakeet-cli are built with, so
// the das side is no longer held to stricter FP than the reference it is measured against.
// The reference tools are NOT run here; gen_bench_records measures them adjacent, as for the LLM legs.

// Decode any file miniaudio understands to 16 kHz mono f32.
def private load_audio_16k_mono(path : string) : array<float> {
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

// Best-of-`reps` transcribe ms for one loaded model, from a warmed JIT + live jobque. `text`
// receives the LAST rep's transcript: under fast-math a token flip is the thing that moves the
// timing, so a run that reports only ms cannot tell a real speedup from a shorter decode.
// `encode_ms` receives the BEST rep's encode split via the asr_prof rail (0 when the family
// carries no encode-total bucket).
def private measure_das(var m : AsrModel; samples : array<float>; reps : int; lang : string;
                        var text : string&; var encode_ms : double&) : double {
    var best = 1.0e30lf
    encode_ms = 0.0lf
    for (_rep in range(reps)) {
        var s <- create_session(m, lang)
        var parts : array<string>
        let t0 = ref_time_ticks()
        transcribe(m, s, samples) $(seg) {
            parts |> push(seg.text)
        }
        let ms = double(get_time_usec(t0)) / 1000.0lf
        if (ms < best) {
            best = ms
        }
        text = join(parts, "")
        delete parts
        delete s
    }
    // the encode split samples in its OWN untimed rep: every asr_prof_add site is a
    // string-keyed table insert, so an enabled profiler inside the timed region would tax
    // exactly the families that carry a bucket and no others
    let ebucket = asr_encode_bucket(m)
    if (!empty(ebucket)) {
        set_asr_prof(true)
        asr_prof_reset()
        var s <- create_session(m, lang)
        transcribe(m, s, samples) $(_seg) {}
        encode_ms = asr_prof_ms(ebucket)
        set_asr_prof(false)
        delete s
    }
    return best
}

// One catalog model's timed buckets -> a BenchModel cell, appended to `jsonModels`.
def private asr_measure_spec(spec : AsrModelSpec; reps, threads : int; var jsonModels : array<BenchModel>) {
    let model_path = asr_model_path(spec)
    var m <- (empty(spec.das_mmproj)
        ? load_asr_model(model_path)
        : load_asr_model(model_path, "{spec.root}{spec.das_mmproj}"))
    var warm <- load_audio_16k_mono("{asr_corpus_dir()}{spec.buckets[0]}")
    var warm_text = ""
    var warm_enc = 0.0lf
    measure_das(m, warm, 1, spec.lang, warm_text, warm_enc)   // JIT compile + first-touch
    delete warm
    var run = BenchRun(engine = "das", backend = "cpu", flavor = flavor_stamp("tuned"),
        exec_fmt = asr_exec_fmt(m),
        workload = asr_workload(spec), box = box_name(), threads = threads,
        date = format_time(get_clock(), "%Y-%m-%d"), cmd = bench_cmd_line(), env = bench_env_line(),
        sha = run_capture("git rev-parse --short=9 HEAD", true), version = das_version(),
        dasllama_version = DASLLAMA_VERSION,
        tune = tune_summary(), tune_sha = tune_manifest_sha(), noise = tune_manifest_noise(),
        hardware = gather_hardware())
    run.files |> emplace(file_identity(model_path, "weights"))
    if (!empty(spec.das_mmproj)) {
        run.files |> emplace(file_identity("{spec.root}{spec.das_mmproj}", "mmproj"))
    }
    run.files |> reserve(length(run.files) + length(spec.buckets))
    for (wav in spec.buckets) {
        var samples <- load_audio_16k_mono("{asr_corpus_dir()}{wav}")
        let audio_s = double(length(samples)) / double(AUDIO_SAMPLE_RATE)
        var text = ""
        var enc_ms = 0.0lf
        let das_ms = measure_das(m, samples, reps, spec.lang, text, enc_ms)
        run.tests |> insert("asr:{wav}", BenchTest(ms = das_ms, audio_s = audio_s, text = text, encode_ms = enc_ms))
        run.files |> emplace(file_identity("{asr_corpus_dir()}{wav}", "clip"))
        print("lcpp_bench --asr: {spec.display} {wav} {das_ms} ms ({audio_s} s audio)\n")
        print("  text: {text}\n")
        delete samples
    }
    if (!empty(spec.libri_short)) {
        // the dictation aggregate: 25 short clips, ONE whole-set total on one test key
        var lib <- libri_wavs()
        if (empty(lib)) {
            print("lcpp_bench --asr: {spec.display} - librispeech set absent/partial (setup_asr_rig --libri), leg skipped\n")
        } else {
            var lib_ms = 0.0lf
            var lib_audio = 0.0lf
            run.files |> reserve(length(run.files) + length(lib))
            for (wav in lib) {
                var samples <- load_audio_16k_mono("{asr_corpus_dir()}{wav}")
                lib_audio += double(length(samples)) / double(AUDIO_SAMPLE_RATE)
                var ltext = ""
                var lenc = 0.0lf
                lib_ms += measure_das(m, samples, reps, spec.lang, ltext, lenc)
                run.files |> emplace(file_identity("{asr_corpus_dir()}{wav}", "clip"))
                delete samples
            }
            run.tests |> insert(libri_test_key(), BenchTest(ms = lib_ms, audio_s = lib_audio))
            print("lcpp_bench --asr: {spec.display} librispeech25 total {lib_ms} ms ({lib_audio} s audio, {length(lib)} clips)\n")
        }
        delete lib
    }
    var cell = BenchModel(gguf = spec.das_model, arch = spec.display,
                          size_bytes = int64(stat(model_path).size))
    cell.runs |> emplace(run)
    jsonModels |> emplace(cell)
    delete m
}

// The --asr entry: catalog match -> measure -> the same cell JSON the LLM rows emit.
def private asr_cell_main(model, json_path : string; reps, threads : int) : bool {
    allow_cpu_prefill()   // the ASR cell is CPU by protocol — disarm the Metal-build tripwire
    apply_box_profile_runtime()
    if (!tune_gate()) {   // never measure on fallback winners — the whole board would sandbag
        return false
    }
    var found = false
    var jsonModels : array<BenchModel>
    with_job_que() {
        setup_dasllama_jobque_()
        for (spec in asr_catalog()) {
            if (find(spec.display, model) < 0 && find(spec.das_model, model) < 0) {
                continue
            }
            if (!stat(asr_model_path(spec)).is_valid) {
                print("lcpp_bench --asr: {spec.display} - model absent ({asr_model_path(spec)}), skipped\n")
                continue
            }
            found = true
            asr_measure_spec(spec, reps, threads, jsonModels)
        }
    }
    if (!found) {
        print("lcpp_bench --asr: no catalog model matches '{model}'\n")
        return false
    }
    let text = sprint_json(jsonModels, true)
    if (!empty(json_path)) {
        var wrote = false
        fopen(json_path, "wb") $(f) {
            if (f != null) {
                fwrite(f, text)
                wrote = true
            }
        }
        if (!wrote) {
            print("warning: could not open {json_path} for writing - no records file\n")
        }
    }
    print("{text}\n")
    return true
}

// ===== Image cell mode (--image) =====
// The vision sibling of the ASR cell: ONE process, one tune-key demand, one image-identity stamp.
// An image turn is a decoder turn with a soft-token span spliced in, so the numbers a user waits
// on are the ordinary ones (prefill, decode) plus the embedder's own encode split — which is why
// this is a cell here and not a bench beside the rig.

// Best-of-`reps` for one image turn: `enc_ms` is the embedder alone, `pp_tok_s` the spliced
// prefill (head tokens + soft-token rows + tail tokens, all positions), `tg_tok_s` the decode.
// `text` carries the LAST rep's reply — a duration alone cannot tell "faster" from "said less".
def private measure_image_turn(var m : Model; emb : Gemma4uvEmbedder; img : VisionImage;
                               prompt : string; reps : int; var enc_ms : double&;
                               var pp_tok_s : double&; var tg_tok_s : double&;
                               var n_rows : int64&; var text : string&) {
    enc_ms = 1.0e30lf
    pp_tok_s = 0.0lf
    tg_tok_s = 0.0lf
    var st = Gemma4uvState()
    var rows : array<float>
    for (_rep in range(reps)) {
        let t_enc = ref_time_ticks()
        n_rows = encode_image_(emb, st, img, "bench", rows)
        enc_ms = min(enc_ms, double(get_time_usec(t_enc)) / 1000.0lf)
        var chat <- create_chat_(m, "", 32l)
        set_thinking_(chat, false)   // the timed shape is the answer, not a scratchpad
        add_user_(chat, prompt)
        var head : array<int64>
        var tail : array<int64>
        render_turn_image_(m, chat, head, tail)
        let dim = m.config.dim
        let n_head = long_length(head)
        let npos = n_head + n_rows + long_length(tail)
        var inscope embd : array<float>
        embd |> ensure_capacity(npos * dim)
        embd |> resize(npos * dim)
        embed_text_rows(m, head, embd, 0l)
        for (i in range64(n_rows * dim)) {
            embd[n_head * dim + i] = rows[i]
        }
        embed_text_rows(m, tail, embd, n_head + n_rows)
        var parts : array<string>
        // the same stop protocol respond_ runs: without it a stray channel marker rides into the
        // timed text, and the row would price a reply the product would never have shown
        var stops <- effective_stop_ids_(chat)
        generate_embd_(m, chat.session, embd, npos, SamplingParams(), 32l, n_head, n_head + n_rows) $(id, piece) {
            for (sid in stops) {
                return false if (id == sid)
            }
            parts |> push(piece)
            return true
        }
        delete stops
        let sres = stats_(chat.session)
        pp_tok_s = max(pp_tok_s, sres.prefill_tps)
        tg_tok_s = max(tg_tok_s, sres.gen_tps)
        text = join(parts, "")
        delete parts
        delete embd
        delete head
        delete tail
        delete chat
    }
    delete rows
    delete st
}

// The --image entry: load the pair, time the turn, emit the same cell JSON the LLM rows emit.
def private image_cell_main(model, mmproj, image, prompt, json_path : string; reps, threads : int) : bool {
    allow_cpu_prefill()   // the image cell is CPU by protocol — disarm the Metal-build tripwire
    apply_box_profile_runtime()
    if (!tune_gate()) {   // never measure on fallback winners — the whole board would sandbag
        return false
    }
    for (p in [model, mmproj, image]) {
        if (!stat(p).is_valid) {
            print("lcpp_bench --image: missing input '{p}'\n")
            return false
        }
    }
    let ask = empty(prompt) ? "What is in this picture? Answer in one sentence." : prompt
    var jsonModels : array<BenchModel>
    with_job_que() {
        setup_dasllama_jobque_()
        var m <- load_model_cached(model, QuantMode.q8)
        var emb <- load_gemma4uv_embedder(mmproj)
        var pic <- load_image_rgb(image)
        if (pic.width == 0) {
            print("lcpp_bench --image: could not decode '{image}'\n")
        } else {
            var enc = 0.0lf
            var pp = 0.0lf
            var tg = 0.0lf
            var rows = 0l
            var text = ""
            measure_image_turn(m, emb, pic, ask, 1, enc, pp, tg, rows, text)   // warm: JIT + first touch
            measure_image_turn(m, emb, pic, ask, reps, enc, pp, tg, rows, text)
            var run = BenchRun(engine = "das", backend = "cpu", flavor = flavor_stamp("tuned"),
                exec_fmt = model_exec_fmt(m),
                workload = "image-chat", box = box_name(), threads = threads,
                date = format_time(get_clock(), "%Y-%m-%d"), cmd = bench_cmd_line(), env = bench_env_line(),
                sha = run_capture("git rev-parse --short=9 HEAD", true), version = das_version(),
                dasllama_version = DASLLAMA_VERSION,
                tune = tune_summary(), tune_sha = tune_manifest_sha(), noise = tune_manifest_noise(),
                hardware = gather_hardware(),
                files <- [file_identity(model, "weights"), file_identity(mmproj, "mmproj"),
                          file_identity(image, "image")])
            run.tests |> insert("img:enc", BenchTest(ms = enc))
            run.tests |> insert("img:pp", BenchTest(tok_s = pp))
            run.tests |> insert("img:tg", BenchTest(tok_s = tg, text = text))
            print("lcpp_bench --image: {pic.width}x{pic.height} -> {rows} soft tokens | enc {enc} ms | pp {pp} tok/s | tg {tg} tok/s\n")
            print("  text: {text}\n")
            var cell = BenchModel(gguf = base_name(model), arch = "gemma4uv",
                                  size_bytes = int64(stat(model).size))
            cell.runs |> emplace(run)
            jsonModels |> emplace(cell)
        }
        delete pic
        delete emb
        delete m
    }
    if (empty(jsonModels)) {
        return false
    }
    let text = sprint_json(jsonModels, true)
    if (!empty(json_path)) {
        var wrote = false
        fopen(json_path, "wb") $(f) {
            if (f != null) {
                fwrite(f, text)
                wrote = true
            }
        }
        if (!wrote) {
            print("warning: could not open {json_path} for writing - no records file\n")
        }
    }
    print("{text}\n")
    return true
}

// ===== Tokenizer cell mode (--tok) =====
// The tokenizer sibling of the LLM protocol, in the one sanctioned rig: encode/decode throughput
// of -m's tokenizer over corpus x size ladders. The headline is the SCALING SHAPE across the 4x
// ladder, not any single number — a superlinear direction hides at one size and is unmistakable
// across steps. Single-threaded pure das by nature: no jobque, no tune_gate (no tuned kernel runs).

let private TOK_SIZE_STEP = 4              // 4x ladder: linear cost steps 4x per size, quadratic 16x
let private TOK_WINDOW_US = 300000l        // calibrate inner iterations to ~this per timed rep
let private TOK_SINGLE_ONLY_US = 2000000l  // past this single-run wall, report that run alone (1 rep)
let private TOK_LADDER_BUDGET_US = 20000000l   // skip the next size when its quadratic-worst estimate passes
                                               // this — kept tight: a quadratic backend's allocation cost also
                                               // DEGRADES with string-heap growth, so the estimate is a floor

struct private TokCorpus {
    name : string
    seed : string
}

struct private TokRow {
    mean_mbs : double
    single_us : int64
    ntok : int64
}

// UTF-8-safe byte-length cut: back the cut off any continuation byte so a sized input is valid text.
def private tok_utf8_prefix(s : string; nbytes : int) : string {
    if (length(s) <= nbytes) {
        return s
    }
    var cut = nbytes
    peek_data(s) $(d : array<uint8> const#) {
        while (cut > 0 && (int(d[cut]) & int(0xC0)) == int(0x80)) {
            cut--
        }
    }
    return slice(s, 0, cut)
}

// A sized input is the seed tiled to >= target, cut back at a codepoint boundary. Tiling is sound
// for cost: BPE cost is pretok-chunk-local, and the SPM merge loop scales on LENGTH, not novelty.
def private tok_sized(seed : string; target : int) : string {
    let sl = length(seed)
    if (sl >= target) {
        return tok_utf8_prefix(seed, target)
    }
    let reps = (target + sl - 1) / sl
    let full = build_string() $(var wr) {
        for (_i in range(reps)) {
            write(wr, seed)
        }
    }
    return tok_utf8_prefix(full, target)
}

def private tok_synth_digits() : string {
    return build_string() $(var wr) {
        for (i in range(160)) {
            let a = i * 7919 % 100000
            let b = i * 104729 % 1000000
            write(wr, "{a} {b} {a}.{b % 997} 2026-0{(i % 9) + 1}-{10 + (i % 18)} {a * b} -{b} {a}e{i % 30}\n")
        }
    }
}

def private tok_synth_longword() : string {
    // letters only: digits or punctuation would pretok-split the run, and the point IS one long word
    let chunk = "abcdefghijklmnopqrstuvwxyzqwertyuiopasdfghjklzxcvbnmlkjhgfdsapo"
    return build_string() $(var wr) {
        for (i in range(64)) {
            for (_j in range(4)) {
                write(wr, chunk)
            }
            write(wr, i % 8 == 7 ? "\n" : " ")
        }
    }
}

// Raw corpus file read; leading '#' lines are provenance headers, not corpus bytes.
// Non-private: tests/test_tok_seed.das gates the header walk.
def tok_read_seed(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    var body = 0   // first byte past the header block; one view walk, one slice
    peek_data(text) $(d) {
        while (starts_with(d, body, "#")) {
            let nl = find(d, '\n', body)
            break if (nl < 0)
            body = nl + 1
        }
    }
    if (body > 0) {
        text = slice(text, body)
    }
    if (empty(text)) {
        panic("lcpp_bench --tok: corpus file missing or empty: {path}")
    }
    return text
}

// prose = the wikitext corpus beside this bench (--prompts overrides); code = the frozen das source
// slice in data/tok_code.txt; cjk = data/tok_cjk.txt (spaceless letter runs — the unbounded-word
// path); digits and longword are synthesized deterministic.
def private tok_corpora(cfg : Args) : array<TokCorpus> {
    let dir = get_this_module_dir()
    var all : array<TokCorpus>
    var prose <- load_corpus(empty(cfg.prompts) ? path_join(dir, "data/wikitext2_prompts.txt") : cfg.prompts)
    all |> emplace(TokCorpus(name = "prose", seed = join(prose, "\n")))
    delete prose
    all |> emplace(TokCorpus(name = "code", seed = tok_read_seed(path_join(dir, "data/tok_code.txt"))))
    all |> emplace(TokCorpus(name = "cjk", seed = tok_read_seed(path_join(dir, "data/tok_cjk.txt"))))
    all |> emplace(TokCorpus(name = "digits", seed = tok_synth_digits()))
    all |> emplace(TokCorpus(name = "longword", seed = tok_synth_longword()))
    if (empty(cfg.tok_corpus)) {
        return <- all
    }
    var wanted <- [for (w in split(cfg.tok_corpus, ",")); strip(w)]
    var out : array<TokCorpus>
    out |> reserve(length(all))
    for (c in all) {
        for (w in wanted) {
            if (!empty(w) && find(c.name, w) >= 0) {
                out |> push(c)
                break
            }
        }
    }
    delete wanted
    delete all
    return <- out
}

// decode(encode(x)) must reproduce x byte-for-byte before any throughput row is trusted — a
// number over a broken pair measures garbage. Mirrors the test_tokenizer corpus gate.
def private tok_roundtrip_ok(tk : Tokenizer; corpus, text : string; fmt : OutFmt) : bool {
    var ids <- encode(tk, text, false)
    let dec = decode(tk, ids)
    delete ids
    if (dec == text) {
        return true
    }
    var at = 0
    let n = min(length(dec), length(text))
    peek_data(text) $(a : array<uint8> const#) {
        peek_data(dec) $(b : array<uint8> const#) {
            while (at < n && a[at] == b[at]) {
                at++
            }
        }
    }
    say(fmt, "tok RT-FAIL {corpus}: decode(encode(x)) != x - in {length(text)} B, out {length(dec)} B, first divergence at byte {at}\n")
    say(fmt, "  in:  ...{tok_utf8_prefix(slice(text, max(at - 20, 0)), 60)}...\n")
    say(fmt, "  out: ...{tok_utf8_prefix(slice(dec, max(at - 20, 0)), 60)}...\n")
    return false
}

// One encode timing cell: a calibration run sizes the inner loop to ~TOK_WINDOW_US, then `reps`
// timed windows. A run past TOK_SINGLE_ONLY_US reports its calibration run alone — repeating a
// multi-second quadratic encode buys noise reduction at minutes of wall.
def private tok_enc_row(tk : Tokenizer; corpus, text : string; reps : int; fmt : OutFmt) : TokRow {
    let bytes = double(length(text))
    let t0 = ref_time_ticks()
    var ids0 <- encode(tk, text, false)
    let single_us = int64(get_time_usec(t0))
    let ntok = long_length(ids0)
    delete ids0
    var mbs : array<double>
    mbs |> reserve(reps)
    var iters = 1
    if (single_us > TOK_SINGLE_ONLY_US) {
        mbs |> push(bytes / double(max(single_us, 1l)))
    } else {
        iters = max(1, int(TOK_WINDOW_US / max(single_us, 1l)))
        for (_rep in range(reps)) {
            let tr = ref_time_ticks()
            for (_it in range(iters)) {
                var ids <- encode(tk, text, false)
                delete ids   // freeing the result is part of the caller-visible cost
            }
            let us = max(int64(get_time_usec(tr)), 1l)
            mbs |> push(bytes * double(iters) / double(us))
        }
    }
    let st = row_stat("enc", mbs)
    let tok_s = st.mean * 1.0e6lf * double(ntok) / max(bytes, 1.0lf)
    say(fmt, "tok enc {corpus} {length(text)} B: {st.mean:.3f} ± {st.sd:.3f} MB/s ({tok_s / 1000.0lf:.0f} Ktok/s, {ntok} tok, {length(mbs)} reps x {iters})\n")
    delete mbs
    return TokRow(mean_mbs = st.mean, single_us = single_us, ntok = ntok)
}

// Decode cell: full-sequence decode MB/s (of produced text), then the streaming shape — one
// piece() per token, the per-generated-token path.
def private tok_dec_row(tk : Tokenizer; corpus, text : string; reps : int; fmt : OutFmt) {
    var ids <- encode(tk, text, false)
    let np = long_length(ids)
    let t0 = ref_time_ticks()
    let dec0 = decode(tk, ids)
    let single_us = int64(get_time_usec(t0))
    let out_bytes = length(dec0)
    var mbs : array<double>
    mbs |> reserve(reps)
    var iters = 1
    if (single_us > TOK_SINGLE_ONLY_US) {
        mbs |> push(double(out_bytes) / double(max(single_us, 1l)))
    } else {
        iters = max(1, int(TOK_WINDOW_US / max(single_us, 1l)))
        for (_rep in range(reps)) {
            var sink = 0
            let tr = ref_time_ticks()
            for (_it in range(iters)) {
                sink += length(decode(tk, ids))
            }
            let us = max(int64(get_time_usec(tr)), 1l)
            // consumes the sink (no dead-call elision) AND pins decode determinism per window
            if (sink != iters * out_bytes) {
                panic("lcpp_bench --tok: decode length varied across identical inputs")
            }
            mbs |> push(double(out_bytes) * double(iters) / double(us))
        }
    }
    let st = row_stat("dec", mbs)
    say(fmt, "tok dec {corpus} {out_bytes} B: {st.mean:.3f} ± {st.sd:.3f} MB/s ({np} tok, {length(mbs)} reps x {iters})\n")
    delete mbs
    var psink = 0
    let tp = ref_time_ticks()
    for (id in ids) {
        psink += length(piece(tk, id))
    }
    let pus = max(int64(get_time_usec(tp)), 1l)
    say(fmt, "tok piece {corpus}: {double(np) * 1000.0lf / double(pus):.0f} Ktok/s ({np} tok, {psink} B out)\n")
    delete ids
}

// llama.cpp reference (encode only): llama-tokenize wall-clock minus a 1-byte-input baseline,
// interleaved, best of `reps` each. A process-subtraction ESTIMATE — both raw walls print beside
// it, and no estimate is claimed when tokenize does not dominate the process wall.
def private tok_ref_row(ref_bin, gguf, corpus, text : string; reps : int; fmt : OutFmt) {
    // unique OS-temp inputs (a fixed name beside the bench assumes a writable tree and collides
    // across concurrent runs), and the run is gated on BOTH writes — a silently missing baseline
    // file would skew the subtraction while printing as a valid estimate
    var terr = ""
    let big_path = create_temp_file("tok_ref_input", "txt", terr)
    let base_path = create_temp_file("tok_ref_base", "txt", terr)
    var wrote_big = false
    var wrote_base = false
    if (!empty(big_path) && !empty(base_path)) {
        fopen(big_path, "wb") $(f) {
            if (f != null) {
                fwrite(f, text)
                wrote_big = true
            }
        }
        fopen(base_path, "wb") $(f) {
            if (f != null) {
                fwrite(f, "a")
                wrote_base = true
            }
        }
    }
    if (!wrote_big || !wrote_base) {
        say(fmt, "tok ref {corpus}: cannot write temp inputs ({terr}) - reference leg skipped\n")
        remove(big_path) if (!empty(big_path))
        remove(base_path) if (!empty(base_path))
        return
    }
    let devnull = get_platform_name() == "windows" ? "NUL" : "/dev/null"
    var big_us = -1l
    var base_us = -1l
    for (_r in range(reps)) {
        let t0 = ref_time_ticks()
        run_capture("\"{ref_bin}\" -m \"{gguf}\" -f \"{big_path}\" --ids > {devnull} 2>&1", false)
        let us = int64(get_time_usec(t0))
        big_us = big_us < 0l ? us : min(big_us, us)
        let t1 = ref_time_ticks()
        run_capture("\"{ref_bin}\" -m \"{gguf}\" -f \"{base_path}\" --ids > {devnull} 2>&1", false)
        let bus = int64(get_time_usec(t1))
        base_us = base_us < 0l ? bus : min(base_us, bus)
    }
    remove(big_path)
    remove(base_path)
    let net = big_us - base_us
    if (net <= base_us / 5l) {
        say(fmt, "tok ref-enc {corpus} {length(text)} B: no estimate - tokenize wall {double(big_us) / 1000.0lf:.1f} ms vs baseline {double(base_us) / 1000.0lf:.1f} ms (subtraction would be noise)\n")
        return
    }
    let mbs = double(length(text)) / double(net)
    say(fmt, "tok ref-enc {corpus} {length(text)} B: ~{mbs:.2f} MB/s (llama-tokenize best {double(big_us) / 1000.0lf:.1f} ms - baseline {double(base_us) / 1000.0lf:.1f} ms, best of {reps})\n")
}

// The --tok entry: per model, per corpus — round-trip gate, encode ladder with the fail-fast
// budget, decode + piece at the largest measured size, optional llama-tokenize reference.
def private tok_cell_main(cfg : Args; reps : int; fmt : OutFmt) : bool {
    var corpora <- tok_corpora(cfg)
    if (empty(corpora)) {
        print("lcpp_bench --tok: no corpus matches '{cfg.tok_corpus}' (have: prose,code,cjk,digits,longword)\n")
        return false
    }
    var any = false
    var ok = true
    var model_list <- [for (m in split(cfg.model, ",")); strip(m)]
    for (mpath in model_list) {
        continue if (empty(mpath))
        if (!stat(mpath).is_valid) {
            print("lcpp_bench --tok: model absent: {mpath}\n")
            ok = false
            continue
        }
        var tk <- load_tokenizer_auto(mpath)
        let vocab_n = tk.kind == TokKind.bpe ? long_length(tk.bpe.vocab) : long_length(tk.spm.vocab)
        let pre = tk.kind == TokKind.bpe ? (empty(tk.bpe.pre) ? "default" : tk.bpe.pre) : "spm"
        say(fmt, "tok: {base_name(mpath)} kind {tk.kind} pre {pre} vocab {vocab_n}\n")
        any = true
        let max_bytes = max(cfg.tok_max_kb, 1) * 1024
        for (c in corpora) {
            // gate small: 4 KB catches round-trip breakage at 1/16th the 16 KB cost on a
            // quadratic backend, and the announce line makes a slow gate attributable
            let gate_bytes = min(4096, max_bytes)
            say(fmt, "tok {c.name}: round-trip gate ({gate_bytes} B)\n")
            if (!tok_roundtrip_ok(tk, c.name, tok_sized(c.seed, gate_bytes), fmt)) {
                ok = false
                continue
            }
            var kb = 1
            var last_text = ""
            var prev_bytes = 0.0lf
            var prev_mbs = 0.0lf
            while (kb * 1024 <= max_bytes) {
                let text = tok_sized(c.seed, kb * 1024)
                let row = tok_enc_row(tk, c.name, text, reps, fmt)
                last_text = text
                if (prev_mbs > 0.0lf && row.mean_mbs > 0.0lf) {
                    let ratio = (double(length(text)) / row.mean_mbs) / (prev_bytes / prev_mbs)
                    let bgrow = double(length(text)) / prev_bytes
                    say(fmt, "tok enc {c.name} scaling: {ratio:.1f}x time for {bgrow:.1f}x bytes (linear {bgrow:.0f}x, quadratic {bgrow * bgrow:.0f}x)\n")
                }
                prev_bytes = double(length(text))
                prev_mbs = row.mean_mbs
                let next_est = row.single_us * int64(TOK_SIZE_STEP * TOK_SIZE_STEP)
                if (kb * TOK_SIZE_STEP * 1024 <= max_bytes && next_est > TOK_LADDER_BUDGET_US) {
                    say(fmt, "tok enc {c.name}: ladder stopped - {kb * TOK_SIZE_STEP} KB estimated up to {next_est / 1000000l} s at quadratic-worst\n")
                    break
                }
                kb *= TOK_SIZE_STEP
            }
            tok_dec_row(tk, c.name, last_text, reps, fmt)
            if (!empty(cfg.ref)) {
                tok_ref_row(cfg.ref, mpath, c.name, tok_sized(c.seed, max_bytes), reps, fmt)
            }
        }
        delete tk
    }
    delete corpora
    delete model_list
    return ok && any
}

// deterministic synthetic ids (vocab-safe low range, the decode_prof convention); llama-bench
// feeds std::rand() — token content doesn't affect speed, determinism keeps runs comparable
def private synth_id(i, seed : int64) : int64 => 1000l + ((i * 17l + seed * 131l) % 5000l)

def private build_prompt(n : int64; seed : int64) : array<int64> {
    return <- [for (i in range64(n)); synth_id(i, seed)]
}

// One in-process A/B lever: both arms run under a SINGLE model load, so cross-run drift cancels and
// the pair is directly comparable. Arms ride the test name (" dense=off" / " dense=on"), which keeps
// both inside one BenchRun and leaves the (box, engine, backend, flavor) upsert identity intact.
// Adding a lever — including one pulled over from an archived bench — is one row in levers_of().
struct private Lever {
    flag : string    // the CLI spelling, for diagnostics: --<flag>-ab
    tag : string     // the row suffix key
    route : function<(on : bool) : void>
    env : string     // env that must arm the uploads first; "" when the lever needs none
    real : bool      // this lever is only meaningful under REAL greedy continuation, so it runs the
                     // tg-real row instead of the llama-bench pp/tg rows (synthetic ids would
                     // measure something the lever cannot affect and read as a valid result)
}

def private levers_of(cfg : Args) : array<Lever> {
    var out : array<Lever>
    if (cfg.dense_ab) {
        out |> emplace(Lever(flag = "dense", tag = "dense", route = @@set_moe_gpu_dense_route,
                             env = "DASLLAMA_GPU_DENSE"))
    }
    if (cfg.comb_ab) {
        out |> emplace(Lever(flag = "comb", tag = "comb", route = @@set_gpu_combine_route, env = ""))
    }
    if (cfg.dn_ab) {
        out |> emplace(Lever(flag = "dn", tag = "dn", route = @@set_moe_gpu_dn_route,
                             env = "DASLLAMA_GPU_DN"))
    }
    if (cfg.attn_ab) {
        out |> emplace(Lever(flag = "attn", tag = "attn", route = @@set_moe_gpu_attn_route,
                             env = "DASLLAMA_GPU_ATTN"))
    }
    if (cfg.prefill_ab) {
        out |> emplace(Lever(flag = "prefill", tag = "gpuprefill", route = @@set_moe_gpu_prefill_route,
                             env = "DASLLAMA_GPU_MOE_LAYERS"))
    }
    if (cfg.mtp_ab) {
        // set_mtp_spec must be live BEFORE the prefill — it warms the draft head's KV
        out |> emplace(Lever(flag = "mtp", tag = "mtp", route = @@set_mtp_spec, env = "", real = true))
    }
    if (cfg.rerecord_ab) {
        out |> emplace(Lever(flag = "rerecord", tag = "rerecord", route = @@set_vk_rdec_rerecord, env = ""))
    }
    return <- out
}

// Output sanity, untimed and always on: each row's warmup logits must be finite with a
// non-degenerate argmax, or the run REFUSES — the timed windows never read output (llama-bench
// parity), so a broken pipeline would otherwise post clean-looking numbers.
def private bench_output_gate(t : Model; s : Session; fmt : OutFmt; tag : string) : bool {
    let vocab = t.config.vocab_size
    var bi = 0l
    var bv = -3.0e38
    var mn = 3.0e38
    var bad = 0l
    for (v in range64(vocab)) {
        let x = s.logits[v]
        if (is_nan(x) || abs(x) > 3.0e38) {
            bad++
        } else {
            if (x > bv) {
                bv = x
                bi = v
            }
            if (x < mn) {
                mn = x
            }
        }
    }
    if (bad > 0l || bv <= mn) {
        say(fmt, "REFUSED ({tag}): warmup logits unusable - {bad} non-finite of {vocab}, max {bv} min {mn}; the serving pipeline is broken and its timings would be meaningless\n")
        return false
    }
    say(fmt, "sanity ({tag}): warmup argmax {bi} \"{piece_(t, bi)}\" (logit {bv})\n")
    return true
}

// Greedy pick over the last logits — the tg-real row's sampler. Runs INSIDE the timed window, but
// both A/B arms pay it identically so it cancels for judging.
def private argmax_logit(s : Session; vocab : int64) : int64 {
    var best = 0l
    var bv = s.logits[0]
    for (v in range64(vocab)) {
        if (s.logits[v] > bv) {
            bv = s.logits[v]
            best = v
        }
    }
    return best
}

// Prompt corpus: segments separated by lines of two percent signs, '#' segments skipped.
def private load_corpus(path : string) : array<string> {
    var texts : array<string>
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("lcpp_bench: can't open prompt corpus {path}")
        }
        for (p in split(fread(f), "%%")) {
            let tp = strip(p)
            if (!empty(tp) && !starts_with(tp, "#")) {
                texts |> push(clone_string(tp))
            }
        }
    }
    return <- texts
}

// progress line — stdout in txt mode, stderr when stdout carries the md table / json records
def private say(fmt : OutFmt; text : string) {
    if (fmt == OutFmt.txt) {
        print(text)
    } else {
        fwrite(fstderr(), text)
    }
}

// --prof census: the runtime state that picks each MoE prefill arm — a fallback path names
// itself here instead of hiding inside a slow row
def private dispatch_census(t : Model; fmt : OutFmt) {
    let c = t.config
    say(fmt, "census: quant={t.quant} kquant_native={t.kquant_native} kq_repacked={t.kq_repacked} mx4_repacked={t.mx4_repacked} experts_mx4={t.experts_mx4}\n")
    say(fmt, "census: caps kq={kernel_backend_has_kq()} kq_batch={kernel_backend_has_kq_batch()} kq_batch_groupn={kernel_backend_has_kq_batch_groupn()} rows={kernel_backend_has_rows()} mx4_rows={kernel_backend_has_mx4_rows()} grouped_prefill={get_moe_grouped_prefill()}\n")
    // the layout-stamp provenance: 1 = disk order (reference layout — no interleave perm applied)
    say(fmt, "census: layout mr: q8={active_q8_layout_mr()} kq4={kq_active_mr(t, KqFmt.k4)} q51={active_q51_layout_mr()}\n")
    if (c.n_expert <= 0l) {
        say(fmt, "census: dense model, no MoE dispatch\n")
        return
    }
    // mirrors the ffn_gemma4_prefill gate; ffn_moe_prefill also ANDs the mx4 per-call
    // economics (the mx4 note below)
    var grouped = 0l
    var perpos = 0l
    var inscope fmts : array<string>
    var inscope fcnt : array<int64>
    for (l in range64(c.n_layers)) {
        continue if (l < c.n_layer_dense_lead)
        let sb = moe_layer_sb_kq(t, l)
        let kq_ok = !sb || (t.kq_repacked && kernel_backend_has_kq_batch())
        if (t.quant == QuantMode.q8 && get_moe_grouped_prefill() && kq_ok) {
            grouped++
        } else {
            perpos++
        }
        let key = "{fmt_at(t.we1_fmt, l)}/{fmt_at(t.we3_fmt, l)}/{fmt_at(t.we2_fmt, l)}{sb ? " sb_kq" : ""}"
        let fi = fmts |> find_index(key)
        if (fi < 0) {
            fmts |> push(key)
            fcnt |> push(1l)
        } else {
            fcnt[fi]++
        }
    }
    var inscope fdesc <- [for (fk, fc in fmts, fcnt); "{fk} x{fc}"]
    say(fmt, "census: moe E={c.n_expert} k={c.n_expert_used} nfe={c.n_ff_exp} expert fmts gate/up/down: {fdesc}\n")
    say(fmt, "census: moe prefill arms: {grouped} grouped, {perpos} per-position{perpos > 0l ? " <-- UNBATCHED reference path" : ""}\n")
    if (t.experts_mx4) {
        say(fmt, "census: mx4 experts - a grouped layer still drops to per-position when npos*k < 8*E (per-call economics)\n")
    }
}

struct private RowStat {
    test : string
    mean : double
    sd : double
}

def private row_stat(tag : string; tps : array<double>) : RowStat {
    var sum = 0.0lf
    for (v in tps) {
        sum += v
    }
    let n = length(tps)
    let mean = sum / double(n)
    var vsum = 0.0lf
    for (v in tps) {
        vsum += (v - mean) * (v - mean)
    }
    let sd = n > 1 ? double(sqrt(float(vsum / double(n - 1)))) : 0.0lf
    return RowStat(test = tag, mean = mean, sd = sd)
}

def private tests_of(rows : array<RowStat>) : table<string; BenchTest> {
    return <- { for (r in rows); r.test => BenchTest(tok_s = r.mean, stddev = r.sd) }
}

// true when this process is a -jit script run (--for-debug-purposes): every emitted record's
// flavor is stamped debug-jit so no board or oracle can mistake it for measurement
var private g_debug_run = false

def private flavor_stamp(base : string) : string {
    return g_debug_run ? "debug-jit:{base}" : base
}

[export]
def main() : int {
    if (!jit_enabled()) {
        print("This benchmark is JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "lcpp_bench")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "lcpp_bench")
        return 0
    }
    if (cfg.prof && !JOBQUE_PROFILING_ENABLED) {
        print("--prof needs the marker rail compiled in - rerun with JOBQUE_PROFILING=1 (or --jobque-profiling)\n")
        return 1
    }
    // measurement exists only from the released exe: a -jit SCRIPT run reads whatever sidecar
    // sits beside the script and re-keys nothing — its numbers must never enter records
    g_debug_run = !empty(tune_app_script())
    if (g_debug_run && !cfg.for_debug_purposes) {
        print("lcpp_bench: this is a -jit SCRIPT run - measurement runs the released exe (daspkg release).\n")
        print("  pass --for-debug-purposes to proceed as a debug instrument; output is stamped debug-jit and is not record-grade\n")
        return 1
    }
    if (g_debug_run) {
        print("lcpp_bench: DEBUG RUN (-jit script) - numbers are debug-grade, flavor is stamped debug-jit\n")
    }
    let gpu = cfg.ngl != 0
    if (!gpu) {
        allow_cpu_prefill()   // CPU-protocol bench by intent — disarm the Metal-build tripwire
    }
    let plen = max(int64(cfg.plen), 0l)
    let ngen = max(int64(cfg.ngen), 0l)
    let reps = max(cfg.reps, 1)
    var levers <- levers_of(cfg)
    if (length(levers) > 1) {
        var names <- [for (l in levers); "--{l.flag}-ab"]
        print("warning: A/B levers are mutually exclusive ({join(names, " ")}) - running --{levers[0].flag}-ab only\n")
        delete names
        levers |> resize(1)
    }
    let ab = !empty(levers)
    let real_row = ab && levers[0].real
    var fmt = cfg.output
    if (cfg.tok) {
        if (cfg.asr || ab) {
            print("--tok is its own cell - drop --asr and the A/B levers\n")
            return 1
        }
        if (fmt != OutFmt.txt || cfg.submit) {
            print("--tok supports -o txt only (the tokenizer cell has no records rail)\n")
            return 1
        }
        // single-threaded pure-das cell: no jobque, and no tune_gate - it dispatches no tuned
        // kernel (the REVIEW tune-gate carve-out)
        return tok_cell_main(cfg, reps, fmt) ? 0 : 1
    }
    if (ab && fmt != OutFmt.txt) {
        print("warning: -o {cfg.output} with an A/B lever - records cover the plain protocol only, falling back to txt\n")
        fmt = OutFmt.txt
    }
    if (cfg.submit && fmt != OutFmt.json) {
        print("error: --submit needs -o json (the records rail; A/B levers cannot submit)\n")
        return 1
    }
    if (cfg.submit && g_debug_run) {
        // debug-jit numbers are stamped so no board mistakes them for measurement — they must
        // not reach the public exchange either; a submission is the released exe's job
        print("error: --submit refused on a debug -jit run - the numbers are debug-grade; submit from the released exe\n")
        return 1
    }
    // the standing per-box methodology, enforced here rather than left to the environment
    // (gen_profile's pattern): cap the future que's workers (computing main is the last lane) and
    // hard-pin lanes to distinct physical cores on the boxes that pin. -t is the explicit ask, but
    // DAS_JOBQUE_THREADS / DAS_JOBQUE_AFFINITY envs still win the que's actual lane count.
    let t_env = g_env_core.jobque_threads
    var threads = cfg.threads > 0 ? cfg.threads : (t_env ?? profile_threads())
    if (is_some(t_env) && unwrap(t_env) != threads) {
        threads = unwrap(t_env)
        say(fmt, "warning: DAS_JOBQUE_THREADS overrides -t - running {threads} lanes\n")
    } elif (is_none(t_env)) {
        // the PC measurement regime caps at 16 lanes (an explicit env or -t always wins) —
        // a big-SMT box at full width measures the scheduler, not the kernels
        if (get_platform_name() == "windows" && cfg.threads <= 0 && threads > 16) {
            say(fmt, "lcpp_bench: capping {threads} lanes to the 16-lane PC regime (-t or DAS_JOBQUE_THREADS overrides)\n")
            threads = 16
        }
        set_jobque_threads_cap(max(threads - 1, 1))
    }
    let pinned = affinity_on() && is_none(g_env_core.jobque_affinity)
    if (pinned) {
        set_jobque_affinity(2)   // 2 = hard mask (QoS classification on darwin), the llama-bench --cpu-strict mirror (mode 1 re-rolls SMT placement)
    }
    // the regime is set BY THE BENCH, printed so nobody has to guess: N lanes = N-1 pool
    // workers + the computing main thread (DAS_JOBQUE_THREADS counts LANES, not workers)
    let pin_desc = "{lcpp_mask_on() ? "hard mask" : "QoS classified"} (bench-forced)"
    say(fmt, "regime: {threads} lanes ({max(threads - 1, 1)} pool workers + main), affinity {pinned ? pin_desc : (is_some(g_env_core.jobque_affinity) ? "env override" : "off (platform default)")}\n")
    if (cfg.asr) {   // the audio cell shares every lane decision above, then runs its own protocol
        return asr_cell_main(cfg.model, cfg.json_path, reps, threads) ? 0 : 1
    }
    if (!empty(cfg.image)) {   // the vision cell, same deal
        if (empty(cfg.image_mmproj)) {
            print("--image needs --image-mmproj (the gemma4uv embedder paired with -m)\n")
            return 1
        }
        return image_cell_main(cfg.model, cfg.image_mmproj, cfg.image, cfg.image_prompt,
                               cfg.json_path, reps, threads) ? 0 : 1
    }
    apply_box_profile_runtime()   // the tuned per-box stack — what load_model users get
    if (!tune_gate()) {   // never measure on fallback winners — the whole board would sandbag
        return 1
    }
    if (cfg.accel && gpu) {
        print("--accel is a CPU-tier lever - drop it or run with -ngl 0\n")
        return 1
    }
    if (cfg.accel) {
        static_if (typeinfo builtin_module_exists(das_accelerate)) {
            dasllama_accelerate_enable(32l)   // the shipped +AMX defaults: min ntok 32, work floor 512 MMAC
        } else {
            print("--accel requested but das_accelerate is not built on this platform - no accel row to measure\n")
            return 1
        }
    }
    if (gpu) {
        set_metal_mode(MetalMode.required)   // before load_model — pins the portable backend, activates the metal overrides
    }
    // vulkan rails requested via env but the tier did not install at [init]: refuse BEFORE any
    // model loads — every input to this check is settled by now. Metal (--ngl) rides the
    // override registries, not this SPI — its own required-mode tripwire covers that arm
    if (get_metal_mode() == MetalMode.off && gpu_want_arms_tier() && !moe_gpu_tier_installed()) {
        let why = (moe_gpu_backend_registered()
            ? "backend registered but the tier did not arm (device probe/VRAM - see the log above)"
            : "no GPU backend module is registered (vulkan shared_module absent or failed to load - see the log above)")
        panic("lcpp_bench: DASLLAMA_GPU* rails requested but the GPU tier is NOT installed - {why}")
    }
    if (cfg.frozen) {
        set_image_artifacts_frozen(true)   // fail on a missing .dlim instead of minting one
    }
    // an unarmed lever silently measures the SAME thing twice — the one A/B failure that looks
    // like a real result, so it warns rather than being left to the reader
    if (ab && !empty(levers[0].env) && !env_is_set(levers[0].env)
            && !(cfg.prefill_ab && is_some(g_env_gpu.gpu_moe_stream))) {
        print("warning: --{levers[0].flag}-ab without {levers[0].env}=1 - nothing armed, both arms identical\n")
    }
    // -m accepts a comma-separated list: one process benches every model in turn, so a sweep
    // pays the compile/startup cost once; a single -m path behaves exactly as before
    // trim: `-m "a.gguf, b.gguf"` is the natural spelling, and an untrimmed path fails to open
    var model_list <- [for (m in split(cfg.model, ",")); strip(m)]
    var mdRows : array<array<string>>
    var jsonModels : array<BenchModel>
    with_job_que() {
        setup_dasllama_jobque_()
        for (mpath in model_list) {
        continue if (empty(mpath))
        say(fmt, "loading {mpath}\n")
        var results : array<RowStat>
        var modelArch = ""
        var activeB = 0.0lf
        var execFmt = ""
        // load INSIDE the que, on the image rail: the .dlim maps in ms, and the GPU tier's
        // gather/upload parallelizes over the lanes (que-less it runs single-threaded — at
        // STREAM=35 that gather was the dominant per-run cost)
        var t <- load_model_cached(mpath, cfg.quant)
        // the tg-real row continues REAL prompts, so its KV must cover the longest one + the window
        var inscope toks : array<array<int64>>
        if (real_row) {
            var tk <- load_tokenizer_auto(mpath)
            var texts <- load_corpus(empty(cfg.prompts)
                ? path_join(get_this_module_dir(), "data/wikitext2_prompts.txt") : cfg.prompts)
            var maxlen = 0l
            for (tx in texts) {
                var ids <- encode(tk, tx, false)
                maxlen = max(maxlen, long_length(ids))
                toks |> emplace(ids)
            }
            delete texts
            delete tk
            if (empty(toks)) {
                panic("lcpp_bench: prompt corpus tokenized to nothing - --{levers[0].flag}-ab needs real prompts")
            }
            t.config.seq_len = min(t.config.seq_len, maxlen + ngen + 8l)
        } else {
            t.config.seq_len = min(t.config.seq_len, max(plen, ngen) + 8l)   // cap the KV allocation
        }
        let c = t.config
        // a model with no NextN head silently no-ops set_mtp_spec — both arms would be identical
        // and the row would read as a valid 1.00x result. Fail closed instead.
        if (cfg.mtp_ab && c.n_layer_nextn <= 0l) {
            panic("lcpp_bench: --mtp-ab needs a NextN draft head, and {mpath} has none")
        }
        // the exec REQUEST (exe-native / -jit / interpreted) + the armed GPU tier are stamped;
        // a jit run that produced nothing logs its own LLVM JIT error beside this
        let exec_state = is_standalone_exe() ? "exe-native" : (jit_enabled() ? "jit" : "INTERPRETED")
        let gpu_seg = moe_gpu_tier_installed() ? " | gpu {gpu_tier_status().backend}" : ""
        say(fmt, "config: dim={c.dim} layers={c.n_layers} vocab={c.vocab_size} | backend {active_kernel_backend()} (batch {active_batch_backend()}) | {exec_state}{gpu_seg} | t{threads} affinity {pinned ? "hard" : "env/off"} | pp{plen} tg{ngen} x {reps} reps\n")
        modelArch = "{t.arch}"
        activeB = active_params_b(t)
        execFmt = model_exec_fmt(t)
        if (cfg.prof) {
            dispatch_census(t, fmt)
        }
        var s <- make_run_state(c, KVDtype.f16, KVDtype.f16)
        for (arm in range(ab ? 2 : 1)) {
            var dtag = ""
            if (ab) {
                dtag = " {levers[0].tag}={arm == 0 ? "off" : "on"}"
                invoke(levers[0].route, arm != 0)
            }
            if (real_row) {
                // REAL greedy continuation: fresh session per prompt (no KV/recurrent bleed),
                // prefill untimed as llama-bench excludes the prompt, argmax inside the timed loop
                // (both arms pay it, so it cancels). NOT comparable to the synthetic tg row.
                let mtp_on = cfg.mtp_ab && arm != 0
                var drafted = 0l
                var accepted = 0l
                var tps : array<double>
                for (_rep in range(reps)) {
                    var total_us = 0l
                    var rep_gen = 0l
                    for (pi in range(length(toks))) {
                        var rs <- make_run_state(c, KVDtype.f16, KVDtype.f16)
                        let pl = long_length(toks[pi])
                        forward_prefill(t, rs, toks[pi], pl, 0l)
                        let t0 = ref_time_ticks()
                        if (mtp_on) {
                            rs.n_past = pl   // forward_prefill leaves n_past alone; mtp_spec_eval drives it
                            var tok = argmax_logit(rs, c.vocab_size)
                            var gen = 0l
                            while (gen < ngen) {
                                var acc = 0l   // the id is unused — only the 1-vs-2 advance is timed
                                gen += mtp_spec_eval(t, rs, tok, acc) ? 2l : 1l
                                tok = argmax_logit(rs, c.vocab_size)
                            }
                            rep_gen += gen   // a trailing accept may overshoot by 1 — count what ran
                        } else {
                            var pos = pl
                            for (_g in range64(ngen)) {
                                forward(t, rs, argmax_logit(rs, c.vocab_size), pos)
                                pos++
                            }
                            rep_gen += ngen
                        }
                        total_us += int64(get_time_usec(t0))
                        drafted += rs.mtp_drafted   // session-lifetime counters — harvest before the free
                        accepted += rs.mtp_accepted
                        delete rs
                    }
                    tps |> push(double(rep_gen) * 1.0e6lf / double(total_us))
                }
                let st = row_stat("tg-real{ngen}{dtag}", tps)
                say(fmt, "{st.test}: {st.mean:.2f} ± {st.sd:.2f} tok/s ({reps} reps, {length(toks)} prompts)\n")
                if (mtp_on) {
                    let rate = drafted > 0l ? double(accepted) * 100.0lf / double(drafted) : 0.0lf
                    say(fmt, "  mtp: {accepted}/{drafted} drafts accepted ({rate:.1f}%)\n")
                }
                results |> push(st)
                delete tps
                if (cfg.prof) {
                    // untimed bucket window for THIS arm — the prefill lands BEFORE the reset, so
                    // the report is pure decode (or pure draft+verify on the on arm). Running it
                    // per arm is the point: the off/on bucket delta is what attributes the draft.
                    var ps <- make_run_state(c, KVDtype.f16, KVDtype.f16)
                    let pl0 = long_length(toks[0])
                    forward_prefill(t, ps, toks[0], pl0, 0l)
                    forward_profile_reset()
                    if (mtp_on) {
                        ps.n_past = pl0
                        var ptok = argmax_logit(ps, c.vocab_size)
                        var pgen = 0l
                        while (pgen < ngen) {
                            var pacc = 0l
                            pgen += mtp_spec_eval(t, ps, ptok, pacc) ? 2l : 1l
                            ptok = argmax_logit(ps, c.vocab_size)
                        }
                    } else {
                        var ppos = pl0
                        for (_g in range64(ngen)) {
                            forward(t, ps, argmax_logit(ps, c.vocab_size), ppos)
                            ppos++
                        }
                    }
                    say(fmt, "--- forward_profile: {st.test} ---\n")
                    forward_profile_report()
                    delete ps
                }
                continue
            }
            if (plen > 0l) {
                let warm <- build_prompt(plen, 999l)
                forward_prefill(t, s, warm, plen, 0l)   // warmup — untimed full-size run, as llama-bench does
                if (!bench_output_gate(t, s, fmt, "pp{plen}{dtag}")) {
                    panic("lcpp_bench: output sanity gate refused pp{plen}{dtag} - see the REFUSED line")
                }
                var tps : array<double>
                for (rep in range(reps)) {
                    let prompt <- build_prompt(plen, int64(rep))
                    let t0 = ref_time_ticks()
                    forward_prefill(t, s, prompt, plen, 0l)
                    let us = get_time_usec(t0)
                    tps |> push(double(plen) * 1.0e6lf / double(us))
                }
                let st = row_stat("pp{plen}{dtag}", tps)
                say(fmt, "{st.test}: {st.mean:.2f} ± {st.sd:.2f} tok/s ({reps} reps)\n")
                if (g_env_bench.bench_verbose) {
                    say(fmt, "  reps: {tps}\n")
                }
                results |> push(st)
                delete tps
                if (cfg.prof) {
                    // untimed bucket window for the pp row — reset FIRST, so the report is pure prefill
                    forward_profile_reset()
                    var inscope pprof <- build_prompt(plen, 998l)
                    let ts_wall = ref_time_ticks()
                    forward_prefill(t, s, pprof, plen, 0l)
                    let wall_us = get_time_usec(ts_wall)
                    say(fmt, "--- forward_profile: {st.test} (window wall {wall_us} us) ---\n")
                    forward_profile_report()
                }
            }
            if (ngen > 0l) {
                forward(t, s, synth_id(0l, 999l), 0l)   // warmup — one untimed token, llama-bench's test_gen(ctx, 1)
                if (!bench_output_gate(t, s, fmt, "tg{ngen}{dtag}")) {
                    panic("lcpp_bench: output sanity gate refused tg{ngen}{dtag} - see the REFUSED line")
                }
                var tps : array<double>
                for (rep in range(reps)) {
                    let t0 = ref_time_ticks()
                    for (i in range64(ngen)) {
                        forward(t, s, synth_id(i, int64(rep)), i)
                    }
                    let us = get_time_usec(t0)
                    tps |> push(double(ngen) * 1.0e6lf / double(us))
                }
                let st = row_stat("tg{ngen}{dtag}", tps)
                say(fmt, "{st.test}: {st.mean:.2f} ± {st.sd:.2f} tok/s ({reps} reps)\n")
                if (g_env_bench.bench_verbose) {
                    say(fmt, "  reps: {tps}\n")
                }
                results |> push(st)
                delete tps
            }
        }
        delete s
        delete t
        // the reference runs AFTER our model is freed — one model in memory at a time
        var refRows : array<LlamaBenchRow>
        var refCmd = ""
        if (!empty(cfg.ref)) {
            var refArgs <- [cfg.ref, "-m", mpath, "-ngl", "{cfg.ngl}", "-t", "{threads}"]
            if (!gpu) {
                if (!cfg.ref_no_affinity) {
                    refArgs |> push_from(lcpp_affinity_argv(threads))
                }
            }
            refArgs |> push_from(["-p", "{cfg.plen}", "-n", "{cfg.ngen}", "-r", "{reps}", "-o", "json"])
            refCmd = argv_line(refArgs)
            say(fmt, "ref: {refCmd}\n")
            if (!run_llama_bench(refArgs, refRows)) {
                say(fmt, "warning: llama-bench run/parse failed - no ref rows\n")
            }
            for (rr in refRows) {
                let tag = rr.n_gen == 0 ? "pp{rr.n_prompt}" : "tg{rr.n_gen}"
                say(fmt, "ref {tag}: {rr.avg_ts:.2f} tok/s\n")
            }
        }
        if (fmt == OutFmt.txt) {
            delete results
            delete refRows
            continue
        }
        // the das row's backend is what OUR engine ran on — metal (--ngl protocol), the armed
        // vulkan tier, or cpu; the reference row keeps the --ngl-derived value (llama-bench
        // knows nothing of the vulkan tier)
        let backend = gpu ? "metal" : "cpu"
        let das_backend = gpu ? "metal" : (moe_gpu_tier_installed() ? "vulkan" : "cpu")
        let gguf = base_name(mpath)
        if (fmt == OutFmt.md) {
            for (st in results) {
                var row <- ["das", gguf, das_backend, "{cfg.ngl}", "{threads}", st.test, "{st.mean:.2f} ± {st.sd:.2f}"]
                mdRows |> emplace(row)
            }
            for (rr in refRows) {
                let tag = rr.n_gen == 0 ? "pp{rr.n_prompt}" : "tg{rr.n_gen}"
                let sd = rr.stddev_ts
                var row <- ["llama.cpp", gguf, backend, "{cfg.ngl}", "{threads}", tag, "{rr.avg_ts:.2f} ± {sd:.2f}"]
                mdRows |> emplace(row)
            }
            delete results
            delete refRows
            continue
        }
    // -o json: the public bench records — one BenchModel per gguf, das run + optional llama.cpp sibling
    let hw = gather_hardware()
    let date = format_time(get_clock(), "%Y-%m-%d")
    let box = box_name()
    var m = BenchModel(
        gguf = gguf,
        arch = modelArch,
        quant = quant_str(cfg.quant),
        active_b = activeB,
        size_bytes = int64(stat(mpath).size))
    if (!empty(refRows)) {
        m.params_b = double(refRows[0].model_n_params) / 1.0e9lf
    }
    var dr = BenchRun(
        engine = "das",
        backend = das_backend,
        flavor = flavor_stamp(cfg.accel ? "accel" : "tuned"),
        box = box,
        threads = threads,
        date = date,
        cmd = bench_cmd_line(),
        env = bench_env_line(),
        sha = run_capture("git rev-parse --short=9 HEAD", true),
        version = das_version(),
        dasllama_version = DASLLAMA_VERSION,
        tune = tune_summary(),
        tune_sha = tune_manifest_sha(),
        noise = tune_manifest_noise(),
        exec_fmt = execFmt,
        hardware = hw,
        tests <- tests_of(results))
    dr.files |> emplace(file_identity(mpath, "weights"))
    m.runs |> emplace(dr)
    if (!empty(refRows)) {
        var rr = BenchRun(
            engine = "llama.cpp",
            backend = backend,
            flavor = cfg.ref_flavor,
            box = box,
            threads = threads,
            date = date,
            cmd = refCmd,
            sha = refRows[0].build_commit,
            exec_fmt = lcpp_exec_fmt(backend),
            hardware = hw)
        rr.files |> emplace(file_identity(mpath, "weights"))
        for (row in refRows) {
            let tag = row.n_gen == 0 ? "pp{row.n_prompt}" : "tg{row.n_gen}"
            rr.tests |> insert(tag, BenchTest(tok_s = row.avg_ts, stddev = row.stddev_ts))
        }
        m.runs |> emplace(rr)
    }
    jsonModels |> emplace(m)
    delete results
    delete refRows
    }   // model loop
    }   // with_job_que
    if (fmt == OutFmt.md) {
        let headers <- ["engine", "model", "backend", "ngl", "threads", "test", "t/s"]
        let aligns <- [ColAlign.left, ColAlign.left, ColAlign.left, ColAlign.right, ColAlign.right, ColAlign.left, ColAlign.right]
        print(render_md_table(headers, aligns, mdRows))
        return 0
    }
    if (fmt != OutFmt.json) {
        return 0
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
        if (wrote) {
            say(fmt, "records written to {cfg.json_path}\n")
        } else {
            say(fmt, "warning: could not open {cfg.json_path} for writing - no records file\n")
        }
    }
    print("{text}\n")
    if (cfg.submit) {
        let url = empty(g_env_exchange.exchange_url) ? EXCHANGE_URL_DEFAULT : g_env_exchange.exchange_url
        let serr = exchange_submit_records_doc(url, text)
        if (!empty(serr)) {
            say(fmt, "records NOT submitted to {url}: {serr}\n")
            return 1
        }
        say(fmt, "records submitted to {url}\n")
    }
    return 0
}
