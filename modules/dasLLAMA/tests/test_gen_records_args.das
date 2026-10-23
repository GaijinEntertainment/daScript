options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require ../performance/profile_common.das
require ../performance/gen_bench_records.das   // the record rig's own pure seams - file-relative require, explicit .das (pays the rig's compile, like test_tok_seed)
require daslib/fio
require strings
require daslib/strings_boost
require daslib/env_registry

// Model-free instruments for the measurement orchestrator's pure seams: the pybench args
// builder's per-tool arms, the GPU-pair roster, the ZERO-cells refusal, the receipt form, the
// rig's --catalog scope and the two guards on the public box store, the das child's argv
// spelling, and the heat keeper's per-axis cold/warm verdict - the branches that decide what
// lands on the public board, none of which any model suite reaches.

def private has(args : array<string>; v : string) : bool {
    for (a in args) {
        if (a == v) {
            return true
        }
    }
    return false
}

def private at_of(args : array<string>; v : string) : int {
    var i = 0
    for (a in args) {
        if (a == v) {
            return i
        }
        i++
    }
    return -1
}

// the value following a flag, or "<absent>" - a missing flag asserts cleanly, never OOB
def private arg_value(args : array<string>; flag : string) : string {
    let at = at_of(args, flag)
    return at >= 0 && at + 1 < length(args) ? args[at + 1] : "<absent>"
}

[test]
def test_pybench_args(t : T?) {
    t |> run("the onnx arm carries the recovery --out and never a device") @(t : T?) {
        let args <- asr_pybench_args("onnx", "parakeet-v3-int8", ["jfk.wav", "gb1.wav"], 6, 2, "mps")
        t |> success(find(args[1], "onnx_bench.py") >= 0, "the onnx script is the target")
        t |> equal(arg_value(args, "--out"), asr_pybench_out_path(), "--out names the recovery path (the Windows stdout-wreck recovery file)")
        t |> success(!has(args, "--device"), "onnx stays CPU - device never reaches it")
        t |> equal(arg_value(args, "--threads"), "6", "threads literal")
        t |> equal(arg_value(args, "--reps"), "2", "reps literal")
        var wavs = 0
        for (a in args) {
            if (a == "--wav") {
                wavs++
            }
        }
        t |> equal(wavs, 2, "one --wav per clip")
    }
    t |> run("the nemo arm forwards the device and skips the recovery file") @(t : T?) {
        let args <- asr_pybench_args("nemo", "nvidia/canary-qwen-2.5b", ["jfk.wav"], 8, 3, "mps")
        t |> success(find(args[1], "canary_qwen_bench.py") >= 0, "the nemo script is the target")
        t |> equal(arg_value(args, "--device"), "mps", "--device carries the value (the torch-MPS arm)")
        t |> success(!has(args, "--out"), "no recovery file - nemo rows parse from stdout")
        let bare <- asr_pybench_args("nemo", "nvidia/canary-qwen-2.5b", ["jfk.wav"], 8, 3)
        t |> success(!has(bare, "--device"), "no device argument when none is asked")
    }
    t |> run("the mlx arm runs the MLX venv's script over the variant, with no device and no recovery file") @(t : T?) {
        let args <- asr_pybench_args("mlx", "parakeet-v3", ["jfk.wav", "gb1.wav"], 8, 3, "mps")
        t |> equal(args[0], mlx_py(), "the MLX venv's interpreter")
        t |> success(find(args[1], "mlx_asr_bench.py") >= 0, "the mlx script is the target")
        t |> equal(arg_value(args, "--variant"), "parakeet-v3", "the variant names the MLX checkpoint")
        t |> success(!has(args, "--device"), "MLX runs on the GPU - device never reaches it")
        t |> success(!has(args, "--out"), "no recovery file - mlx rows parse from stdout")
        t |> equal(arg_value(args, "--reps"), "3", "reps literal")
    }
}

[test]
def test_gpu_pair_roster(t : T?) {
    t |> run("the gpu-pair roster names the tools with a GPU twin leg") @(t : T?) {
        t |> success(asr_gpu_pair_tool("whisper-cli"), "whisper-cli pairs (Metal build)")
        t |> success(asr_gpu_pair_tool("mtmd"), "mtmd pairs (Metal build)")
        t |> success(asr_gpu_pair_tool("nemo"), "nemo pairs (torch-MPS)")
        t |> success(!asr_gpu_pair_tool("onnx"), "onnx is CPU-only")
        t |> success(!asr_gpu_pair_tool("parakeet-cli"), "parakeet-cli runs -ng on the rig")
    }
    t |> run("the gpu-only roster names the references with no CPU leg") @(t : T?) {
        t |> success(asr_gpu_only_tool("mlx"), "mlx runs on the Apple GPU alone")
        for (tool in ["whisper-cli", "mtmd", "nemo", "onnx", "parakeet-cli"]) {
            t |> success(!asr_gpu_only_tool(tool), "{tool} keeps its CPU leg")
        }
        t |> equal(asr_engine_name("mlx"), "mlx-audio", "the MLX reference's engine is its package")
        t |> equal(asr_engine_name("mtmd"), "llama-mtmd-cli", "mtmd's engine is the binary")
        t |> equal(asr_engine_name("onnx"), "onnx", "every other tool is its own engine")
    }
}

[test]
def test_canary_bench_seams(t : T?) {
    t |> run("the canary reference bench's pure seams pass their self-test") @(t : T?) {
        var pv = ""
        if (run_and_capture(["python3", "--version"], pv, 30.0) != 0) {
            t |> skip("no python3 on this box")
            return
        }
        var out = ""
        let rc = run_and_capture(["python3", "modules/dasLLAMA/benchmarks/asr/canary_qwen_bench.py",
                                  "--self-test"], out, 60.0)
        t |> equal(0, rc, "the self-test passes: {out}")
        t |> success(find(out, "SELFTEST") >= 0, "the self-test announces itself: {out}")
        // the rejection branch: no --wav (and no --self-test) refuses before any model import
        var rout = ""
        let rrc = run_and_capture(["python3", "modules/dasLLAMA/benchmarks/asr/canary_qwen_bench.py"], rout, 60.0)
        t |> equal(2, rrc, "an argless run refuses: {rout}")
        t |> success(find(rout, "required") >= 0, "the refusal names --wav: {rout}")
    }
}

[test]
def test_zero_cells_refusal(t : T?) {
    t |> run("a run that measures nothing carries the red verdict") @(t : T?) {
        let empty_run = records_run_verdict(0, 0)
        t |> equal(1, empty_run.rc, "zero cells red the run")
        t |> success(find(empty_run.msg, "ZERO cells measured") >= 0, "the verdict names the guard")
        let failed_run = records_run_verdict(2, 40)
        t |> equal(1, failed_run.rc, "failed cells red the run regardless of measured count")
        let good = records_run_verdict(0, 1)
        t |> equal(0, good.rc, "one measured cell with no failures is green")
    }
}

[test]
def test_argv_receipt_spells_home_as_tilde(t : T?) {
    let line = argv_receipt(["/Users/ci/Work/bin/daslang", "-jit", "lcpp_bench.das", "--", "-m", "/Users/ci/models/q.gguf", "a b", "/Users/ci/my dir/x.gguf"])
    t |> equal(line, "~/Work/bin/daslang -jit lcpp_bench.das -- -m ~/models/q.gguf \"a b\" ~/\"my dir/x.gguf\"", "every home spelled ~ outside the quotes a shell would otherwise keep literal")
    t |> equal(argv_line(["/Users/ci/bin/tool", "-m", "/Users/ci/my dir/x"]), "~/bin/tool -m ~/\"my dir/x\"", "the display form keeps ~ outside its space-quotes too")
    t |> equal(argv_line(["C:\\Users\\ci\\my dir\\x.exe"]), "~\\\"my dir\\x.exe\"", "the drive form keeps ~ outside the quotes on its own separator")
}

[test]
def test_catalog_scope(t : T?) {
    t |> run("--catalog admits official and all, and refuses every other spelling") @(t : T?) {
        t |> success(catalog_known("official"), "official - the public board")
        t |> success(catalog_known("all"), "all - the sweep form")
        t |> success(!catalog_known(""), "an empty --catalog names no rows")
        t |> success(!catalog_known("Official"), "the match is case-sensitive")
        t |> success(!catalog_known("sweep"), "a plausible-but-wrong spelling refuses instead of falling back to a default")
    }
    t |> run("the cpu legs' batched shape is 1 or more on both axes, or there is no batched row") @(t : T?) {
        t |> success(npl_cpu_shape_ok(4, 128, 3), "the shipped shape")
        t |> success(npl_cpu_shape_ok(0, 0, 0), "no batched row - the shape is unread")
        t |> success(!npl_cpu_shape_ok(4, 0, 3), "a zero prompt would run the das child on the flat -p and the reference on -npp 0")
        t |> success(!npl_cpu_shape_ok(4, 128, 0), "a zero rep count would fold no reference run into the row")
        t |> success(!npl_cpu_shape_ok(4, -1, 3), "a negative prompt refuses like a zero one")
    }
    t |> run("the batched row's shape follows the leg: the cpu legs' own, the metal leg the flat rows'") @(t : T?) {
        var cfg = Args()
        cfg.plen = 256
        cfg.reps = 2
        cfg.npl_cpu_plen = 64
        let cpu = npl_leg_shape(cfg, true)
        t |> equal(cpu.plen, 64, "a cpu leg's streams start from --npl-cpu-plen")
        t |> equal(cpu.reps, 3, "a cpu leg's row takes --npl-cpu-reps runs")
        let metal = npl_leg_shape(cfg, false)
        t |> equal(metal.plen, 256, "the metal leg's streams start from the flat rows' -p")
        t |> equal(metal.reps, 2, "the metal leg's row takes the flat rows' -r runs")
        let shipped = npl_leg_shape(Args(), true)
        t |> success(shipped.plen == 128 && shipped.reps == 3, "the shipped cpu shape is 128 x 3 (METHODOLOGY.md)")
    }
}

[test]
def test_board_store_guard(t : T?) {
    t |> run("a --jit run needs a store of its own") @(t : T?) {
        let refusal = board_store_refusal("official", true, "")
        t |> success(find(refusal, "--jit") >= 0, "the refusal names the flag that drew it: {refusal}")
        t |> success(find(refusal, "--store") >= 0, "the refusal names the fix: {refusal}")
        t |> equal(board_store_refusal("official", true, "/tmp/sweep.json"), "", "an explicit store admits the debug-jit run")
    }
    t |> run("a --catalog all sweep needs a store of its own") @(t : T?) {
        let refusal = board_store_refusal("all", false, "")
        t |> success(find(refusal, "--catalog all") >= 0, "the refusal names the flag that drew it: {refusal}")
        t |> success(find(refusal, "--store") >= 0, "the refusal names the fix: {refusal}")
        t |> equal(board_store_refusal("all", false, "/tmp/sweep.json"), "", "an explicit store admits the sweep")
    }
    t |> run("the mlx-lm rows never enter the board") @(t : T?) {
        let refusal = board_store_refusal("official", false, "", "modules/dasLLAMA/performance/records/m5.json")
        t |> success(find(refusal, "--mlx-lm-store") >= 0, "an mlx-lm store on the board is refused: {refusal}")
        t |> equal(board_store_refusal("official", false, "", "/tmp/mlx_lm_m5.json"), "", "a scratch mlx-lm store beside the board mint is admitted")
    }
    t |> run("an explicit store inside records/ is still the board") @(t : T?) {
        let board = "modules/dasLLAMA/performance/records/m5.json"
        t |> success(store_is_board(board), "the relative board path is the board")
        t |> success(store_is_board("/Users/x/daScript/modules/dasLLAMA/performance/records/m5.json"), "so is its absolute spelling")
        t |> success(!store_is_board("/tmp/sweep.json"), "a scratch store is not")
        t |> success(!store_is_board("modules/dasLLAMA/performance/records/mtp/mtp_m5_x.json"), "nor is a ruler record under records/mtp")
        t |> success(find(board_store_refusal("all", false, board), "--catalog all") >= 0, "a sweep naming the board store is refused")
        t |> success(find(board_store_refusal("official", true, board), "--jit") >= 0, "so is a debug-jit run naming it")
        t |> equal(board_store_refusal("official", false, board), "", "the board mint may name its own store")
    }
    t |> run("the board mint itself passes the guard") @(t : T?) {
        t |> equal(board_store_refusal("official", false, ""), "", "the official catalog off --jit is exactly the run the box store exists for")
    }
}

[test]
def test_das_child_args(t : T?) {
    t |> run("the released exe takes its rest after a bare separator") @(t : T?) {
        let args <- das_child_args("/opt/rig/dasllama-bench", false, ["-m", "x.gguf", "-r", "5"])
        t |> equal(args[0], "/opt/rig/dasllama-bench", "the released exe leads")
        t |> equal(args[1], "--", "one separator, then the bench's own flags")
        t |> success(!has(args, "--for-debug-purposes"), "a released child stamps no debug row")
        t |> equal(arg_value(args, "-m"), "x.gguf", "the rest arrives verbatim")
        t |> equal(arg_value(args, "-r"), "5", "and in order")
    }
    t |> run("the --jit child runs the bench SOURCE and stamps itself debug") @(t : T?) {
        let args <- das_child_args("/usr/local/bin/daslang", true, ["-m", "x.gguf"])
        t |> equal(args[0], "/usr/local/bin/daslang", "this daslang leads")
        t |> equal(args[1], "-jit", "the dev loop jits the script")
        t |> success(find(args[2], "lcpp_bench.das") >= 0, "the bench source is the script: {args[2]}")
        t |> equal(args[3], "--", "the separator follows the script, not the exe")
        t |> success(has(args, "--for-debug-purposes"), "the flag the child turns into its debug-jit row stamp")
        t |> equal(arg_value(args, "-m"), "x.gguf", "the rest arrives verbatim")
    }
}

// one text cell as the rig reads it back from a child's --json-path records
def private text_cell(engine : string; pp, pp_sd, tg, tg_sd : double) : array<BenchModel> {
    var run = BenchRun(engine = engine, backend = "cpu", flavor = "tuned")
    run.tests["pp512"] = BenchTest(tok_s = pp, stddev = pp_sd)
    run.tests["tg128"] = BenchTest(tok_s = tg, stddev = tg_sd)
    var m = BenchModel(gguf = "fixture.gguf")
    m.runs |> emplace(run)
    var models : array<BenchModel>
    models |> emplace(m)
    return <- models
}

[test]
def test_cell_rates_split_the_axes(t : T?) {
    t |> run("a das cell's rows fold onto the pp and tg axes separately") @(t : T?) {
        let cell <- text_cell("das", 512.0lf, 20.0lf, 40.0lf, 0.4lf)
        let rates = cell_rates(cell)
        t |> equal(rates.pp, 512.0lf, "the prefill row lands on pp alone")
        t |> equal(rates.tg, 40.0lf, "the emission row lands on tg alone")
        t |> equal(rates.worst_cv, 20.0lf / 512.0lf, "the worst cv is the widest row's, whichever axis it sits on")
    }
    t |> run("a reference row reaches neither axis") @(t : T?) {
        let cell <- text_cell("llama.cpp", 900.0lf, 1.0lf, 70.0lf, 0.1lf)
        let rates = cell_rates(cell)
        t |> equal(rates.pp, 0.0lf, "only das rows are the cell's own measurement")
        t |> equal(rates.tg, 0.0lf, "on both axes")
        t |> equal(rates.worst_cv, 0.0lf, "and the retry verdict never reads a reference row's variance")
    }
}

[test]
def test_heat_keeper(t : T?) {
    t |> run("a warm re-run slower on EITHER axis keeps the cold cell") @(t : T?) {
        let cold = CellRates(pp = 500.0lf, tg = 40.0lf, worst_cv = 0.09lf)
        t |> success(keep_cold(cold, CellRates(pp = 480.0lf, tg = 40.0lf)), "a slower prefill is heat")
        t |> success(keep_cold(cold, CellRates(pp = 500.0lf, tg = 39.0lf)), "a slower emission is heat")
        t |> success(keep_cold(cold, CellRates(pp = 520.0lf, tg = 39.0lf)), "a 20 tok/s prefill gain never pays for a 1 tok/s emission loss - the summed form kept this cell")
    }
    t |> run("a re-run that lost nothing keeps the warm cell") @(t : T?) {
        let cold = CellRates(pp = 500.0lf, tg = 40.0lf, worst_cv = 0.09lf)
        t |> success(!keep_cold(cold, CellRates(pp = 500.0lf, tg = 40.0lf)), "an identical re-run is not slower")
        t |> success(!keep_cold(cold, CellRates(pp = 520.0lf, tg = 41.0lf)), "faster on both axes is the map warming the retry exists for")
        t |> success(!keep_cold(cold, CellRates(pp = 501.0lf, tg = 40.0lf)), "a gain on one axis with the other held is still not a loss")
    }
}

[test]
def test_bench_env_line_spells_home_as_tilde(t : T?) {
    let had = env_is_set("DASLLAMA_MODELS_DIR")
    let before = had ? env_value_of("DASLLAMA_MODELS_DIR") : ""
    set_env_variable("DASLLAMA_MODELS_DIR", "/Users/ci/my models")
    let line = bench_env_line()
    if (had) {
        set_env_variable("DASLLAMA_MODELS_DIR", before)
    } else {
        set_env_variable("DASLLAMA_MODELS_DIR", "")
    }
    t |> success(find(line, "DASLLAMA_MODELS_DIR=~/\"my models\"") >= 0, "the env receipt spells the home ~ outside the quotes a value with a space needs: {line}")
    t |> success(find(line, "/Users/") < 0, "no home directory survives in the env receipt")
}

[test]
def test_tts_ref_args(t : T?) {
    let kokoro = tts_spec_of("/models/kokoro-82m.gguf")
    t |> run("the torch arm runs the tts venv's instrument over one model on the asked device") @(t : T?) {
        let args <- tts_ref_args(kokoro, false, "mps", 20, 3, 8)
        t |> equal(args[0], tts_py(), "the tts venv's interpreter")
        t |> success(find(args[1], "tts_ref_bench.py") >= 0, "the reference instrument is the target")
        t |> equal(arg_value(args, "--models"), "kokoro-82m:af_heart", "one model a process, at the das voice")
        t |> equal(arg_value(args, "--device"), "mps", "the torch device")
        t |> equal(arg_value(args, "--limit"), "20", "the corpus head both engines speak")
        t |> equal(arg_value(args, "--passes"), "3", "best of the rig's reps")
        t |> equal(arg_value(args, "--threads"), "8", "threads literal")
        t |> success(!has(args, "--engine"), "torch is the instrument's default engine")
    }
    t |> run("the mlx arm runs the mlx venv's interpreter and never a device") @(t : T?) {
        let args <- tts_ref_args(kokoro, true, "mps", 20, 3, 8)
        t |> equal(args[0], mlx_py(), "the mlx venv's interpreter")
        t |> equal(arg_value(args, "--engine"), "mlx", "the MLX ports")
        t |> success(!has(args, "--device"), "MLX runs on the GPU - device never reaches it")
    }
    t |> run("the reference row's engine names the package, or mlx-audio") @(t : T?) {
        t |> equal(tts_ref_engine(kokoro, false), "kokoro")
        t |> equal(tts_ref_engine(tts_spec_of("kitten-nano.gguf"), false), "kittentts")
        t |> equal(tts_ref_engine(tts_spec_of("pocket-tts-en-q8.gguf"), false), "pocket-tts")
        t |> equal(tts_ref_engine(kokoro, true), "mlx-audio")
    }
}

[test]
def test_tts_catalog_seams(t : T?) {
    t |> run("a GGUF path finds its catalog row by file name; an unlisted one finds none") @(t : T?) {
        let s = tts_spec_of("/some/dir/kitten-mini.gguf")
        t |> equal(s.file, "kitten-mini.gguf")
        t |> equal(s.voice, "Bella")
        t |> success(!s.torch_gpu, "kitten's package runs the onnxruntime CPU provider")
        t |> success(tts_spec_of("kokoro-82m.gguf").torch_gpu, "kokoro's torch model takes mps and cuda")
        t |> success(empty(tts_spec_of("stories15M.gguf").file), "no row for a non-TTS file")
    }
    t |> run("every catalog row is a TTS file with a voice and a reference name, its file unique") @(t : T?) {
        var inscope cat <- tts_catalog()
        var inscope seen : table<string>
        for (s in cat) {
            t |> success(!empty(s.voice) && !empty(s.ref) && !empty(s.package) && !empty(s.display), "{s.file} carries its fields")
            t |> success(!key_exists(seen, s.file), "{s.file} listed once")
            seen |> insert(s.file)
        }
    }
    t |> run("both engines record under one key: the sentence count and the voice") @(t : T?) {
        t |> equal(tts_test_key(20, "af_heart"), "tts:g2p20@af_heart")
        t |> equal(tts_bench_label(20), "g2p20")
    }
    t |> run("the das child carries the row's file and voice, the rig's sentences and reps, the leg") @(t : T?) {
        let cfg = Args(tts_sentences = 7, tts_reps = 2)
        let args <- tts_das_args(tts_spec_of("kokoro-82m.gguf"), cfg, 6, "cell.json", "/opt/rig/dasllama-bench", "metal")
        t |> success(has(args, "--tts"), "the TTS cell mode")
        t |> success(ends_with(arg_value(args, "-m"), "kokoro-82m.gguf"), "the model under the models dir")
        t |> equal(arg_value(args, "--tts-voice"), "af_heart")
        t |> equal(arg_value(args, "--tts-sentences"), "7")
        t |> equal(arg_value(args, "-r"), "2")
        t |> equal(arg_value(args, "--json-path"), "cell.json")
        t |> equal(arg_value(args, "--ngl"), "1", "the metal leg's flag")
        let cpu <- tts_das_args(tts_spec_of("kokoro-82m.gguf"), cfg, 6, "cell.json", "/opt/rig/dasllama-bench", "")
        t |> success(!has(cpu, "--ngl") && !has(cpu, "--accel"), "the neon leg carries no leg flag")
        let accel <- media_leg_args("accel")
        t |> success(length(accel) == 1 && accel[0] == "--accel", "the amx leg's flag")
        t |> equal(media_leg_args("metal"), [ "--ngl", "1" ], "a media cell's metal leg")
        t |> equal(media_leg_args("metal", 99), [ "--ngl", "99" ], "the image cell's metal leg")
        t |> equal(length(media_leg_args("", 99)), 0, "the neon leg carries no flag")
    }
}

[test]
def test_tts_reference_output(t : T?) {
    let out = "ref bench: x\nVERSIONS\ttorch 2.14.0 / kokoro 0.9.4 (tts_ref_bench.py --device cpu, requirements-tts.txt)\n  lj000 9.2 s\nkokoro-82m af_heart: ...\nBENCH\tkokoro-82m\tg2p20\t187.5000\t0\t14444.90\t0.07704\n"
    t |> run("the BENCH row reads back as the summed generation ms and audio seconds") @(t : T?) {
        var audio_s = -1.0lf
        let ms = parse_bench_best(out, tts_bench_label(20), audio_s)
        t |> equal(ms, 14444.9lf)
        t |> equal(audio_s, 187.5lf)
        var none_s = -1.0lf
        t |> success(parse_bench_best(out, tts_bench_label(5), none_s) < 0.0lf, "another sentence count reads no row")
    }
    t |> run("the VERSIONS line names the packages, and reads empty when the instrument printed none") @(t : T?) {
        t |> equal(ref_versions_line(out), "torch 2.14.0 / kokoro 0.9.4 (tts_ref_bench.py --device cpu, requirements-tts.txt)")
        t |> equal(ref_versions_line("BENCH\tx\tg2p20\t1\t0\t1\t1\n"), "")
    }
    t |> run("a probe's warning line ahead of VERSIONS stays out of the field") @(t : T?) {
        let probe = "[0;93m2026-10-20 W:onnxruntime: device discovery failed[m\nVERSIONS\tonnx-asr 0.7.0 / onnxruntime 1.22.0 (requirements-onnx.txt)\n"
        t |> equal(ref_versions_line(probe), "onnx-asr 0.7.0 / onnxruntime 1.22.0 (requirements-onnx.txt)")
    }
}

[test]
def test_tts_corpus_head(t : T?) {
    let corpus = "[\{\"id\": \"a\", \"text\": \"One.\"\}, \{\"id\": \"b\", \"text\": \"\"\}, \{\"id\": \"c\", \"text\": \"Three.\"\}, \{\"id\": \"d\", \"text\": \"Four.\"\}]"
    t |> run("the head skips a sentence with no text and stops at n") @(t : T?) {
        var inscope head <- tts_corpus_head(corpus, 2)
        t |> equal(length(head), 2)
        t |> equal(head[0].id, "a")
        t |> equal(head[1].id, "c", "the textless row is not a sentence")
        var inscope all <- tts_corpus_head(corpus, 10)
        t |> equal(length(all), 3, "a short corpus answers fewer")
        var inscope bad <- tts_corpus_head("not json", 5)
        t |> success(empty(bad), "an unparsed corpus answers none")
    }
    t |> run("the committed corpus holds the default head") @(t : T?) {
        var inscope head <- tts_corpus_head(fread(tts_corpus_path()), TTS_BENCH_SENTENCES)
        t |> equal(length(head), TTS_BENCH_SENTENCES)
    }
}

[test]
def test_tts_seat_verdict(t : T?) {
    t |> run("no serve is a CPU cell, every call served a GPU one, anything between neither") @(t : T?) {
        t |> equal(tts_seat_verdict([20, 20, 0], [0, 0, 0]), "none", "the knob-off cell")
        t |> equal(tts_seat_verdict([20, 20, 0], [20, 20, 0]), "all", "a seat no call reached is no fallback")
        t |> equal(tts_seat_verdict([20, 20, 1], [20, 19, 1]), "partial", "one chunk's decode fell to the CPU chain")
        t |> equal(tts_seat_verdict([20, 20], [20, 0]), "partial", "a seat that never served beside one that did")
        t |> equal(tts_seat_verdict(array<int>(), array<int>()), "none", "a family with no seats")
    }
}

[test]
def test_mlx_lm_reference(t : T?) {
    t |> run("the mlx-lm argv is the flat reference's shape, the batched row only past one stream") @(t : T?) {
        let flat <- mlx_lm_bench_args("mlx-community/Qwen3.8-27B-4bit", 512, 128, 5, 1)
        t |> equal(flat[0], mlx_py(), "the MLX venv's interpreter")
        t |> success(find(flat[1], "mlx_lm_bench.py") >= 0, "the mlx-lm script is the target")
        t |> equal(arg_value(flat, "--model"), "mlx-community/Qwen3.8-27B-4bit", "the checkpoint")
        t |> equal(arg_value(flat, "-p"), "512", "prompt length")
        t |> equal(arg_value(flat, "-n"), "128", "generated tokens")
        t |> equal(arg_value(flat, "-r"), "5", "trials")
        t |> success(!has(flat, "--npl"), "no batched row at one stream")
        let batched <- mlx_lm_bench_args("m", 512, 128, 5, 4)
        t |> equal(arg_value(batched, "--npl"), "4", "the batched row's streams")
    }
    t |> run("the row parser keeps whole ROW lines with a positive rate") @(t : T?) {
        let out = "VERSIONS\tmlx-lm=0.32.2\nROW\tpp512\t1500.5\t12.0\nnoise\nROW\ttg128\t80.25\t0.5\nROW\ttg128@4\t0\t0\nROW\tbad\tx\t1\nROW\tshort\t3\n"
        let rows <- mlx_lm_rows(out)
        t |> equal(length(rows), 2, "the two good rows, a zero rate, a non-number and a short line dropped")
        t |> equal(rows[0].test, "pp512", "first test")
        t |> equal(rows[0].tok_s, 1500.5lf, "first rate")
        t |> equal(rows[1].test, "tg128", "second test")
        t |> equal(rows[1].stddev, 0.5lf, "second stddev")
    }
}

[test]
def test_board_refs_guard(t : T?) {
    t |> run("a board mint refuses a leg whose llama-bench is unset") @(t : T?) {
        t |> success(find(board_refs_refusal(true, true, true, true, "", "/b/stock"), "--ref-clean") >= 0, "the cpu leg without its clean ref")
        t |> success(find(board_refs_refusal(true, true, false, true, "", ""), "--ref-stock") >= 0, "the metal leg without its stock ref")
        t |> equal(board_refs_refusal(true, true, true, true, "/b/clean", "/b/stock"), "", "both refs set - the board mint starts")
        t |> equal(board_refs_refusal(true, true, false, true, "", "/b/stock"), "", "a metal-only run needs no clean ref")
        t |> equal(board_refs_refusal(false, true, true, true, "", ""), "", "a scratch store may run das-only")
        t |> equal(board_refs_refusal(true, false, true, true, "", ""), "", "an asr/tts-only run reads no llama-bench")
    }
}

[test]
def test_vulkan_leg_guards(t : T?) {
    t |> run("the vulkan leg runs alone, off Apple, under the GPU ask, on a dasVulkan build") @(t : T?) {
        t |> equal(vulkan_leg_refusal(false, true, true, false), "", "no vulkan leg - nothing to refuse")
        t |> success(find(vulkan_leg_refusal(true, false, true, true), "Apple") >= 0, "refused on Apple")
        var built = false
        static_if (typeinfo builtin_module_exists(vulkan)) {
            built = true
        }
        if (built) {
            t |> success(find(vulkan_leg_refusal(true, false, false, false), "DASLLAMA_GPU") >= 0, "refused without the GPU ask")
            t |> success(find(vulkan_leg_refusal(true, true, false, true), "alone") >= 0, "refused beside another leg")
            t |> equal(vulkan_leg_refusal(true, false, false, true), "", "admitted alone under the ask")
        } else {
            t |> success(find(vulkan_leg_refusal(true, false, false, true), "dasVulkan") >= 0, "refused on a build without dasVulkan")
        }
        t |> success(find(board_refs_refusal(true, true, false, false, "", "", true, ""), "--ref-vulkan") >= 0, "a board mint pairs every vulkan cell")
        t |> equal(board_refs_refusal(true, true, false, false, "", "", true, "/b/vk"), "", "with the vulkan ref the mint starts")
    }
    t |> run("a vulkan cell holds only when every das row served on the device") @(t : T?) {
        var served <- [BenchModel(gguf = "m.gguf", runs <- [BenchRun(engine = "das", backend = "vulkan")])]
        var fell <- [BenchModel(gguf = "m.gguf", runs <- [BenchRun(engine = "das", backend = "cpu")])]
        let empty_cell : array<BenchModel>
        t |> success(cell_served_on(served, "vulkan"), "a vulkan-stamped das row holds")
        t |> success(!cell_served_on(fell, "vulkan"), "a cpu-stamped das row on the vulkan leg fails")
        t |> success(!cell_served_on(empty_cell, "vulkan"), "a cell with no das row fails")
        t |> success(cell_served_on(fell, "cpu"), "the cpu legs stamp themselves")
        delete served
        delete fell
    }
}

[test]
def test_ref_py(t : T?) {
    t |> run("a reference venv's interpreter: the env override when set, else the venv beside the asr benches") @(t : T?) {
        t |> equal(ref_py("/opt/py/bin/python3", "onnx"), normalize("/opt/py/bin/python3"), "the override wins")
        let dflt = ref_py("", "tts")
        t |> success(find(dflt, ".venv-tts") >= 0, "the default names the venv: {dflt}")
        t |> success(find(ref_py("", "mlx"), ".venv-mlx") >= 0, "each name its own venv")
    }
}
