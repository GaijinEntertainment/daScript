options gen2
// the shapes under test compile in-process or run in a spawned
// interpreter child; the UnitTest ABI probes are not declared in AOT
// headers; the copy-order test pins the order the interpreter and JIT
// evaluate a copy in, and AOT takes the C++ compiler's
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

def return_path_errors(fixture : string; lint_profile : bool) : tuple<ok : bool; count : int> {
    var res = (ok = false, count = 0)
    let path = path_join(get_das_root(), "tests/language/{fixture}")
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.export_all = true
            cop.no_optimizations = lint_profile
            cop.no_infer_time_folding = lint_profile
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                res.ok = ok
                res.count = length(split(string(issues), "error[30310]")) - 1
            }
        }
    }
    return res
}

[test(description = "the return-path check reports every loop a function can fall out of, in the folded profile and the lint profile alike")]
def test_loops_that_fall_off_the_end_are_reported(t : T?) {
    if (get_running_platform_name() == "emscripten") {
        t |> skip("the fixtures sit outside the web build das root")
        return
    }
    for (lint_profile in [false, true]) {
        let r = return_path_errors("_fixture_loops_fall_off.das", lint_profile)
        t |> success(!r.ok, "the fixture does not compile, lint profile {lint_profile}")
        t |> equal(r.count, 13, "one error per function, lint profile {lint_profile}")
    }
}

[test(description = "a for over non-empty constant ranges and a while (true) with only a dead break need no trailing return, folded or not")]
def test_loops_that_always_return_compile(t : T?) {
    if (get_running_platform_name() == "emscripten") {
        t |> skip("the fixtures sit outside the web build das root")
        return
    }
    for (lint_profile in [false, true]) {
        let r = return_path_errors("_fixture_loops_return.das", lint_profile)
        t |> success(r.ok, "the fixture compiles, lint profile {lint_profile}")
        t |> equal(r.count, 0)
    }
}

struct CopyOrderSmall {
    a, b, c : int
}

struct CopyOrderBig {
    a, b, c, d, e, f, g, h, i, j : int
}

def copy_order_mark(var order : array<int>; mark, index : int) : int {
    order |> push(mark)
    return index
}

[test(description = "interpreter and JIT: a copy between two elements evaluates the right side, then the left")]
def test_copy_evaluates_right_then_left(t : T?) {
    var order : array<int>
    var ints <- [10, 20]
    ints[copy_order_mark(order, 1, 0)] = ints[copy_order_mark(order, 2, 1)]
    t |> equal(ints[0], 20)
    var small : array<CopyOrderSmall>
    small |> resize(2)
    small[1].c = 7
    small[copy_order_mark(order, 3, 0)] = small[copy_order_mark(order, 4, 1)]
    t |> equal(small[0].c, 7)
    var big : array<CopyOrderBig>
    big |> resize(2)
    big[1].j = 9
    big[copy_order_mark(order, 5, 0)] = big[copy_order_mark(order, 6, 1)]
    t |> equal(big[0].j, 9)
    t |> equal(length(order), 6)
    t |> equal(order[0], 2)
    t |> equal(order[1], 1)
    t |> equal(order[2], 4)
    t |> equal(order[3], 3)
    t |> equal(order[4], 6)
    t |> equal(order[5], 5)
}

struct CopyOrderBox {
    v : CopyOrderBig
}

[sideeffects]
def copy_order_retarget(var q : CopyOrderBox?&; var other : CopyOrderBox?; index : int) : int {
    q = other
    return index
}

[unsafe_deref]
def copy_order_into_retargeted(var first, second : CopyOrderBox?; src : array<CopyOrderBig>) {
    var q = first
    q.v = src[copy_order_retarget(q, second, 1)]
}

[test(description = "interpreter and JIT: a copy through a pointer that the right side re-aims writes where the pointer aims after the right side ran")]
def test_copy_destination_follows_the_right_side(t : T?) {
    var first, second : CopyOrderBox
    var src : array<CopyOrderBig>
    src |> resize(2)
    src[1].j = 9
    copy_order_into_retargeted(unsafe(addr(first)), unsafe(addr(second)), src)
    t |> equal(second.v.j, 9)
    t |> equal(first.v.j, 0)
}
