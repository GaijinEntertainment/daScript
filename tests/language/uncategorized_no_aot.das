options gen2
// the shapes under test compile in-process or run in a spawned
// interpreter child; the UnitTest ABI probes are not declared in AOT
// headers
options no_aot

require UnitTest
require dastest/testing_boost public
require daslib/ast_boost
require daslib/fio
require daslib/strings_boost
require daslib/command_line

def fixture_path() : string => path_join(get_das_root(), "tests/language/_virtual_dead_arm.das")

def fixture_source() : string {
    return [
        "options gen2",
        "",
        "let NC = 4",
        "let RSI = 1",
        "",
        "def body : float \{",
        "    var s0, s1 : float[NC]",
        "    s0[0] = 1.0",
        "    static_if (RSI == 2) \{",
        "        s1[0] = 2.0",
        "    \}",
        "    return s0[0]",
        "\}",
        ""
    ] |> join("\n")
}

class LetMarks : AstVisitor {
    marked : table<string; bool>
    def override preVisitExprLetVariable(_expr : ExprLet?; arg : VariablePtr; _last : bool) : void {
        marked[string(arg.name)] = arg.flags.marked_used
    }
}

[test(description = "with infer-time folding off, a local of the late-resolved type float[NC] used only inside a dead static_if arm is still marked used")]
def test_dead_arm_marks_let_typed_later(t : T?) {
    var inscope access <- make_file_access("")
    set_file_source(access, fixture_path(), fixture_source())
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.export_all = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(fixture_path(), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the fixture compiles: {issues}")
                return if (!ok)
                var marks = new LetMarks()
                for_each_function(get_this_module(program), "body") $(fn) {
                    make_visitor(*marks) $(adapter) {
                        visit(fn, adapter)
                    }
                }
                t |> equal(marks.marked?["s1"] ?? false, true)
                unsafe {
                    delete marks
                }
            }
        }
    }
}

//! a return that leaves only the inner loop hangs the child, and only
//! a parent process can time a hang out
[test(description = "a private function ending in a nested while-true loop returns from it, so the child exits 0 and prints its accumulated value")]
def test_a_private_function_ending_in_a_nested_while_true_returns(t : T?) {
    if (get_running_platform_name() == "emscripten") {
        t |> skip("no child process on the web")
        return
    }
    let fixture = path_join(get_das_root(), "tests/language/_fixture_nested_while_private.das")
    var output : string
    let argv <- [get_das_exe(), "-dasroot", get_das_root(), "-no-module-cache", fixture]
    let rc = run_and_capture(argv, output, child_deadline(20.0))
    t |> equal(0, rc, "the child returned from run and exited, got:\n{output}")
    t |> success(output |> contains("2129000"),
                 "the child printed the accumulated value, got:\n{output}")
}

def jit_test_abi_mad(a, b, c) => test_abi_mad(a, b, c)

def test_vec_abi(t : T?) {
    {
        t |> success(!jit_enabled()
                     || is_jit_function(@@ < (a, b, c : float2) : float2 > jit_test_abi_mad))
        let a = float2(1, 2)
        let b = float2(3, 4)
        let c = float2(5, 6)
        let tt = test_abi_mad(a, b, c)
        let jt = jit_test_abi_mad(a, b, c)
        t |> equal(tt, jt)
    }

    {
        t |> success(!jit_enabled()
                     || is_jit_function(@@ < (a, b, c : float3) : float3 > jit_test_abi_mad))
        let a = float3(1, 2, 3)
        let b = float3(4, 5, 6)
        let c = float3(7, 8, 9)
        let tt = test_abi_mad(a, b, c)
        let jt = jit_test_abi_mad(a, b, c)
        t |> equal(tt, jt)
    }

    {
        t |> success(!jit_enabled()
                     || is_jit_function(@@ < (a, b, c : float4) : float4 > jit_test_abi_mad))
        let a = float4(1, 2, 3, 4)
        let b = float4(5, 6, 7, 8)
        let c = float4(9, 10, 11, 12)
        let tt = test_abi_mad(a, b, c)
        let jt = jit_test_abi_mad(a, b, c)
        t |> equal(tt, jt)
    }
}

[sideeffects]
def dummy {
    assert(false)
}

def test_func_abi(t : T?) {
    let fnA = @@dummy
    let fnB = test_abi_func(fnA)
    t |> equal(fnA, fnB)
}

[test(description = "a daslang wrapper of the C++ float2, float3 and float4 ABI probe returns the same value as the direct call, JIT-compiled when JIT is on, and a function pointer round-trips through C++ unchanged")]
def test_jit_abi(t : T?) {
    test_vec_abi(t)
    test_func_abi(t)
}
