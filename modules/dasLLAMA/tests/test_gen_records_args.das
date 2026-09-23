options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

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
}

[test]
def test_gpu_pair_roster(t : T?) {
    t |> run("the gpu-pair roster names the tools with a GPU twin leg") @(t : T?) {
        t |> success(asr_gpu_pair_tool("whisper-cli"), "whisper-cli pairs (Metal build)")
        t |> success(asr_gpu_pair_tool("mtmd"), "mtmd pairs (Metal build)")
        t |> success(asr_gpu_pair_tool("nemo"), "nemo pairs (torch-MPS)")
        t |> success(!asr_gpu_pair_tool("onnx"), "onnx is CPU-only")
        t |> success(!asr_gpu_pair_tool("parakeet-cli"), "parakeet-cli is CPU-only")
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
