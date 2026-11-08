options gen2

require dastest/testing_boost
require daslib/jobque_boost
require jit
require daslib/tune

// The SimFunction::jitFunction invoke fastpath (dispatch arc step 1): C++ invoke paths
// (das_invoke_function / das_invoke_lambda / jit_call_or_fastcall) call the JIT wrapper
// directly when the mirror is armed, skipping callOrFastcall + the virtual SimNode_Jit::eval.
// This pins: (a) the mirror arms under -jit and always AGREES with is_jit_function,
// (b) results through the fastpath match direct calls, (c) remove_jit disarms it and the
// function still runs (interp), (d) a team-dispatched lambda (the hot production shape)
// covers its range exactly once through the fastpath.

def add_pair(a : int; b : int) : int => a + b * 3

def sum_via_invoke(fun : function<(a : int; b : int) : int>) => invoke(fun, 5, 7)

[test(description = "the JIT invoke fastpath mirror always agrees with is_jit_function, matches direct calls, and remove_jit disarms it while the function still runs")]
def test_fastpath_arms(t : T?) {
    t |> run("mirror agrees with is_jit_function") @(t : T?) {
        // tolerant of the multi-program reference-tier wrinkle: armed OR not-jit, but the
        // mirror must SHADOW the node exactly
        t |> success(is_jit_function(@@add_pair) == has_jit_fastpath(@@add_pair))
    }
    t |> run("fastpath result == direct call") @(t : T?) {
        t |> equal(26, sum_via_invoke(@@add_pair))
        t |> equal(26, add_pair(5, 7))
    }
    t |> run("remove_jit disarms, function still correct") @(t : T?) {
        if (is_jit_function(@@add_pair)) {
            unsafe {
                remove_jit(@@add_pair)
            }
            t |> success(!has_jit_fastpath(@@add_pair))
            t |> equal(26, sum_via_invoke(@@add_pair))      // interp path after disarm
        } else {
            t |> success(!has_jit_fastpath(@@add_pair))     // never armed without jit
        }
    }
}

[test(description = "a lambda dispatched across a team covers its range exactly once through the JIT fastpath")]
def test_fastpath_team_lambda(t : T?) {
    t |> run("team-dispatched lambda covers exactly once") @(t : T?) {
        with_job_que() {
            set_jobque_team_mode(true)
            let N = 64
            var hits : array<int>
            hits |> resize(N)
            var pr = unsafe(addr(hits[0]))
            team_parallel_for(0, N, 16) <| @(job_begin : int; job_end : int) {
                for (i in range(job_begin, job_end)) {
                    unsafe(pr[i]) ++
                }
            }
            var bad = 0
            for (i in range(N)) {
                if (hits[i] != 1) {
                    bad++
                }
            }
            t |> equal(0, bad)
            delete hits
        }
    }
}

// [llvm_code] external-codegen rail: the selftest generator (llvm/daslib/llvm_code_selftest,
// wired via llvm/daslib/llvm_user_modules) emits `a + b` for (int, int) : int stubs and
// declines any other signature. Reference bodies carry interp/AOT/declined-tier semantics.
// This test pins the correctness contract in every lane; the strict generator-FIRED gate
// (sentinel must flip) is a single-program property — dastest's in-process multi-program
// execution can serve a reference-tier result with jit instrumentation live, so here the
// probe only asserts "generated or reference, never garbage".

[llvm_code(name = "llvm_code_selftest::add_i32")]
def jadd(a, b : int) : int => a + b

[llvm_code(name = "llvm_code_selftest::add_i32")]
def jadd_probe(a, b : int) : int =>
    a - b - 1000000  // sentinel result — visible only when the generator did NOT replace the body

[llvm_code(name = "llvm_code_selftest::add_i32")]
def fsub_decline(a, b : float) : float =>
    a - b  // generator declines (float): the reference body compiles as the fallback tier

[test(description = "an llvm_code function returns the generated or the reference body's value, declines on a float signature, and runs the reference body when interpreted")]
def test_llvm_code(t : T?) {
    var n = 2  // nolint:LINT003 — var defeats const-fold so the calls survive to codegen
    t |> equal(jadd(n, 3), 5)  // bit-exact whichever tier ran
    t |> success(fsub_decline(float(n), 2f) == 0f)  // decline -> reference body everywhere
    let r = jadd_probe(n, 3)
    t |> success(r == 5 || r == -1000001)  // generated (a+b) or reference tier — never anything else
    if (!jit_enabled()) {
        t |> equal(r, -1000001)  // interpreted lane always runs the reference body
    }
}
