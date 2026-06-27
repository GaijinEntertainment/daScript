options gen2
options indenting = 4
options no_aot

module testing_boost shared

// wasm/AOT variant of dastest/testing_boost. Same [test]/[benchmark] annotation
// surface so test files compile unchanged, but instead of being discovered and
// run by the dastest meta-compiler (compile_file + rtti), the [test] functions
// are accumulated at compile time and a `main` is generated that runs them all
// in-process and returns a non-zero exit code on failure. Swapped in via a
// project-file module_get() override (see tests_wasm.das_project).

require daslib/ast_boost
require daslib/templates_boost
require ../dastest/testing.das public
require strings

// ---- compile-time accumulation of [test] functions ----
struct private TestEntry {
    name : string
    hasArg : bool
}
var private g_tests : array<TestEntry>

[macro_function]
def private validate_arguments(type_name : string, args) : bool {
    let len = length(args)
    if (len == 0) {
        return true
    }
    if (len == 1) {
        return (args[0]._type.isPointer && args[0]._type.firstType.isStructure && args[0]._type.firstType.structType.name == type_name
                && args[0]._type.firstType.structType._module != null && args[0]._type.firstType.structType._module.name == "testing")
    }
    return false
}

[function_macro(name="test")]
class TestFunctionAnnotation : AstFunctionAnnotation {
    [unused_argument(group, args, errors)] def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        if (!validate_arguments("T", func.arguments)) {
            errors := "Invalid arguments for test macro\nexpected function header: def {func.name}() or def {func.name}(t: testing::T?)"
            return false
        }
        func.flags.exports = true
        let nm = string(func.name)
        var seen = false
        for (te in g_tests) {
            if (te.name == nm) { seen = true; break }
        }
        if (!seen) {
            g_tests |> push(TestEntry(name = nm, hasArg = !empty(func.arguments)))
        }
        return true
    }
}

[function_macro(name="benchmark")]
class BenchmarkFunctionAnnotation : AstFunctionAnnotation {
    [unused_argument(group, args, errors)] def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        if (!validate_arguments("B", func.arguments)) {
            errors := "Invalid arguments for benchmark macro\nexpected function header: def {func.name}() or def {func.name}(b: testing::B?)"
            return false
        }
        func.flags.exports = true
        return true   // benchmarks are not executed by the wasm test runner
    }
}

// ---- runtime: run one test with isolation (single context, no compiler) ----
var private g_failures = 0
var private g_total = 0

def run_one_test(name : string; body : lambda<(t : T?) : void>) {
    g_total ++
    var failed = false
    var skipped = false
    var deliberateRecover = false
    var t = new T(name, ref_time_ticks(), false)
    unsafe {
        t.onFail <- @ capture(& t, & failed, & deliberateRecover) (now : bool) {
            t.failed = true
            failed = true
            deliberateRecover = now
        }
        t.onSkipNow <- @ capture(& t, & skipped, & deliberateRecover) () {
            t.skipped = true
            skipped = true
            deliberateRecover = true
        }
        t.onRun <- @ capture(& t) (test_name : string; f : RunT) {
            if (f is func1) {
                invoke(f as func1, t)
            } elif (f is lmd1) {
                invoke(f as lmd1, t)
            }
        }
        t.onLog <- @(msg : string; at : LineInfo) {
            print("    {msg}\n")
        }
    }
    try {
        invoke(body, t)
    } recover {
        if (!deliberateRecover) {
            failed = true
        }
    }
    if (skipped) {
        print("SKIP {name}\n")
    } elif (failed) {
        g_failures ++
        print("FAIL {name}\n")
    } else {
        print("PASS {name}\n")
    }
    unsafe { delete t }
}

def get_test_exit_code() : int {
    print("=== {g_total - g_failures}/{g_total} passed, {g_failures} failed ===\n")
    return g_failures != 0 ? 1 : 0
}

// ---- generate `main` that runs every accumulated [test] in its module ----
[infer_macro]
class GenerateTestMainPass : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || mod.name == "testing_boost" || mod.name == "testing" || empty(g_tests) || mod |> find_unique_function("main", true) != null) return false
        var blkList : array<ExpressionPtr>
        for (te in g_tests) {
            if (mod |> find_unique_function(te.name, true) == null) continue
            let nm = te.name
            if (te.hasArg) {
                blkList |> push(qmacro(_::run_one_test($v(nm), @(t : testing::T?) { $c(nm)(t); })))
            } else {
                blkList |> push(qmacro(_::run_one_test($v(nm), @(t : testing::T?) { $c(nm)(); })))
            }
        }
        if (empty(blkList)) return false
        var mainFn = qmacro_function("main") $() : int {
            $b(blkList)
            return _::get_test_exit_code()
        }
        mainFn.flags.exports = true
        compiling_module() |> add_function(mainFn)
        return true
    }
}
