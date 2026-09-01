options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require ../performance/profile_common.das
require daslib/fio
require strings
require daslib/strings_boost

// Model-free instruments for the measurement orchestrator's pure seams: the pybench args
// builder's per-tool arms, the GPU-pair roster, and the ZERO-cells refusal - the branches
// that decide what lands on the public board, none of which any model suite reaches.

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
    let line = argv_receipt(["/Users/ci/Work/bin/daslang", "-jit", "lcpp_bench.das", "--", "-m", "/Users/ci/models/q.gguf", "a b"])
    t |> equal(line, "~/Work/bin/daslang -jit lcpp_bench.das -- -m ~/models/q.gguf \"a b\"", "every argv element shell-quoted, every home spelled ~")
}
