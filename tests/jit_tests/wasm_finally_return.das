// A return, break or continue that leaves through a `finally` dispatches back to its continuation
// through a switch on an index; the wasm64 cross compile of that shape emits its object (the
// indirectbr form left IR the wasm backend crashed on), and a native run reaches every continuation.
options gen2
options no_aot

require dastest/testing_boost
require daslib/fio
require daslib/strings_boost
require strings

let FIXTURE = "{get_das_root()}/tests/jit_tests/_wasm_finally_fixture.das"

def private spawn_child(cmd : string; var lines : array<string>) : int {
    var full = "{cmd} 2>&1"
    if (get_platform_name() == "windows") {
        full = "\"{full}\""   // cmd.exe strips the first and last quote of a line that opens with one
    }
    return unsafe(popen_timeout(full, 300.0) $(f) {
        if (f != null) {
            while (!feof(f)) {
                let ln = strip(fgets(f))
                if (!empty(ln)) {
                    lines |> push(ln)
                }
            }
        }
    })
}

[test]
def test_wasm_finally_return(tt : T?) {
    if (!jit_enabled()) {
        tt |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view - do not delete
    let bin = args[0]
    tt |> run("a native run takes every exit through its finally once") @(t : T?) {
        var lines : array<string>
        let rc = spawn_child("\"{bin}\" -jit \"{FIXTURE}\"", lines)
        let output = join(lines, "\n")
        t |> equal(rc, 0, output)
        // pick: 2 finally runs; loop_exits(10): i = 0..4 enter the block, 4 breaks out -> 5 runs
        t |> success(find(output, "values 7 9 3 5 finally 7") >= 0, output)
    }
    tt |> run("the wasm64 cross compile emits its object") @(t : T?) {
        let obj = path_join(test_temp_dir(), "wasm_finally.o")
        var lines : array<string>
        let rc = spawn_child("\"{bin}\" -exe -output \"{obj}\" \"{FIXTURE}\" -- --jit-target=wasm64-unknown-emscripten --jit-emit-object", lines)
        let output = join(lines, "\n")
        t |> equal(rc, 0, output)
        let st = stat(obj)
        t |> success(st.is_reg && st.size > 0ul, "{obj} written:\n{output}")
    }
}
