// The signed idot family on the wasm64 cross target lowers to the ISA's exact int8 dot halves -
// the i16x8 extmul products summed by i32x4.extadd_pairwise_i16x8_s and folded even/odd - and never
// to the generic widen-multiply chain that runs a fifth as fast there; the relaxed 7-bit dot is
// refused by design (it wraps at -128, and +relaxed-simd re-means min/max/mad), so the dump carries
// no relaxed intrinsic either. A host compile of the same fixture emits no wasm intrinsic.
// Compile-only keeps both compiles on this host and never touches the artifact cache, so --jit-dump
// prints every time.
options gen2
options no_aot

require dastest/testing_boost
require daslib/fio
require daslib/strings_boost
require strings

let FIXTURE = "{get_das_root()}/tests/jit_tests/_wasm_idot_fixture.das"
let PAIRWISE_ADD = "llvm.wasm.extadd.pairwise.signed"
let RELAXED_DOT = "llvm.wasm.relaxed.dot"

def private spawn_child(cmd : string; var lines : array<string>) : int {
    var rc : int
    unsafe {
        rc = popen_timeout("{cmd} 2>&1", 300.0) $(f) {
            if (f == null) {
                return
            }
            while (!feof(f)) {
                let ln = strip(fgets(f))
                if (!empty(ln)) {
                    lines |> push("{ln}")
                }
            }
        }
    }
    return rc
}

def private count_with(lines : array<string>; needle : string) : int {
    var n = 0
    for (ln in lines) {
        if (find(ln, needle) >= 0) {
            n++
        }
    }
    return n
}

[test]
def test_wasm_idot_lowering(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    t |> run("the wasm64 cross compile lowers idot4 to the exact pairwise int8 dot") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{FIXTURE}\" -- --jit-target=wasm64-unknown-emscripten --jit-compile-only --jit-dump", ln)
        let pairs = count_with(ln, PAIRWISE_ADD)
        t |> success(pairs >= 3, "the declaration and the two half calls of {PAIRWISE_ADD} are in the dump ({pairs} lines, {length(ln)} lines total)")
        t |> success(count_with(ln, RELAXED_DOT) == 0, "the relaxed 7-bit dot is never emitted")
        t |> success(count_with(ln, "idot_prod") == 0, "no generic widen-multiply chain remains")
    }
    t |> run("a host compile emits no wasm intrinsic") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{FIXTURE}\" -- --jit-compile-only --jit-dump", ln)
        t |> success(length(ln) > 20, "the host dump printed ({length(ln)} lines)")
        t |> success(count_with(ln, "llvm.wasm.") == 0, "no wasm intrinsic on the host")
    }
}
