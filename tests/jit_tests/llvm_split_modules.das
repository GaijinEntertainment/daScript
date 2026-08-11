options gen2

require dastest/testing_boost
require daslib/module_path
require daslib/strings_boost
require strings

// --jit-split-modules: per-das-module partition emission - one .o per module, linked into the
// ONE cached DLL. The child is a 3-partition program (main -> mod_a -> mod_b) exercising
// cross-partition direct calls, a cmres cross-call, the shared interp-call glob (a [no_jit]
// callee used from two partitions), and the shared table-method glob. Two child runs: split
// codegen, then the same command again (the split-built DLL cache-hits and installs with
// zero codegen).

def private run_split_child(cmd : string; hit_marker : string) : tuple<rc : int; sawResult : bool; sawMarker : bool> {
    var sawResult = false
    var sawMarker = false
    var rc : int
    unsafe {
        rc = popen_timeout(cmd, 300.0) $(f) {
            if (f == null) {
                return
            }
            while (!feof(f)) {
                let ln = fgets(f)
                sawResult ||= find(ln, "SPLIT_RESULT 340 19 36 33 -1") >= 0
                sawMarker ||= find(ln, hit_marker) >= 0
            }
        }
    }
    return (rc = rc, sawResult = sawResult, sawMarker = sawMarker)
}

[test]
def test_jit_split_modules(t : T?) {
    if (!jit_enabled() || !das_is_dll_build()) {
        // the flag only means something under the LLVM JIT DLL path; the child needs -jit too
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    let client = "{get_this_module_dir()}/_llvm_split_modules_client.das"
    // cmd.exe /c strips the first and last quote of a line that starts with a quote —
    // wrap in a sacrificial outer pair on windows (same trap as the daspkg popen incident)
    // =4 exercises the threaded pool (thread count never changes the artifact or its cache key)
    var cmd = "\"{bin}\" -jit \"{client}\" -- --jit-split-modules=4"
    if (get_platform_name() == "windows") {
        cmd = "\"{cmd}\""
    }
    // round 1: split emission (a warm local rerun may cache-hit; the summary announces
    // ", split" either way, so both assertions hold on both paths; CI runners are always cold)
    let r1 = run_split_child(cmd, ", split")
    t |> equal(r1.rc, 0)
    t |> success(r1.sawResult)   // cross-partition calls compute the interp-identical answer
    t |> success(r1.sawMarker)   // the run really took the split path
    // round 2: the split-built DLL cache-hits and installs with zero codegen
    let r2 = run_split_child(cmd, "DLL cache hit")
    t |> equal(r2.rc, 0)
    t |> success(r2.sawResult)
    t |> success(r2.sawMarker)
}
