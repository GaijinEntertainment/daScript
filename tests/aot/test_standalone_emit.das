options gen2
options no_aot
options stack = 1_048_576

require daslib/aot_standalone
require daslib/ast_boost
require daslib/fio
require daslib/rtti
require strings
require dastest/testing_boost public

def private standalone_cop(var cop : CodeOfPolicies) {
    cop.threadlock_context = false
    cop.aot = false
    cop.aot_module = true
    cop.aot_lib = false
    cop.fail_on_lack_of_aot_export = true
    cop.ignore_shared_modules = false
    cop.version_2_syntax = true
    cop.tune_frozen = true
}

def private generate_standalone(t : T?; fixture, out_dir : string) : string {
    let input = path_join(get_das_root(), "tests/aot/{fixture}.das")
    using() $(var cop : CodeOfPolicies) {
        standalone_cop(cop)
        ast_gc_guard() {
            standalone_aot(input, out_dir, false, false, cop)
        }
    }
    let generated = path_join(out_dir, "{fixture}.das.cpp")
    var source = ""
    fopen(generated, "rb") $(fr) {
        if (fr != null) {
            source = fread(fr)
        }
    }
    if (empty(source)) {
        t |> failure("standalone_aot produced no {generated}")
    }
    remove(generated)
    remove(path_join(out_dir, "{fixture}.das.h"))
    return source
}

def private brace_balance(text : string) : int {
    var balance = 0
    peek_data(text) $(bytes) {
        for (byte in bytes) {
            let ch = int(byte)
            if (ch == '{') {
                balance ++
            } elif (ch == '}') {
                balance --
            }
        }
    }
    return balance
}

[test]
def test_standalone_emit(t : T?) {
    var tmp_err : string
    let out_dir = create_temp_directory("standalone_emit", tmp_err)
    if (!empty(tmp_err)) {
        t |> failure("cannot create temp directory: {tmp_err}")
        return
    }

    t |> run("a context with no init functions still closes its constructor") @(t : T?) {
        let source = generate_standalone(t, "_standalone_novars_fixture", out_dir)
        t |> success(find(source, "Standalone::Standalone() \{") >= 0, "constructor was emitted")
        t |> equal(brace_balance(source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("forward declarations spell types the way the definitions do") @(t : T?) {
        let source = generate_standalone(t, "_standalone_tuple_fixture", out_dir)
        t |> success(find(source, "TTuple<") >= 0, "the tuple definition was emitted")
        t |> success(find(source, "AutoTuple<") < 0, "cross-platform spelling leaked into a native emit")
        t |> equal(brace_balance(source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("the constructor runs the init script and then the [init] functions") @(t : T?) {
        let source = generate_standalone(t, "_standalone_init_fixture", out_dir)
        let ctor_pos = find(source, "Standalone::Standalone()")
        t |> success(ctor_pos >= 0, "constructor was emitted")
        let init_script_pos = find(source, "__init_script(&context, true);")
        t |> success(init_script_pos > ctor_pos, "the constructor calls __init_script")
        let zero_pos = find(source, "memset(context.globals, 0, context.getGlobalSize());")
        t |> success(zero_pos > ctor_pos && zero_pos < init_script_pos, "globals are zeroed before the init script runs")
        let tail = slice(source, init_script_pos)
        t |> success(find(tail, "record_init") >= 0, "the [init] function is called after __init_script")
        t |> success(find(source, "runInitScript") < 0, "the constructor does not call runInitScript")
        t |> equal(brace_balance(source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("required modules' globals emit in dependency order before the entry module's") @(t : T?) {
        let source = generate_standalone(t, "_standalone_cross_module_fixture", out_dir)
        let a_pos = find(source, "/*g_dep_a*/")
        let b_pos = find(source, "/*g_dep_b*/")
        let main_pos = find(source, "/*g_main*/")
        t |> success(a_pos >= 0 && b_pos >= 0 && main_pos >= 0, "all three globals were emitted")
        t |> success(a_pos < b_pos && b_pos < main_pos, "dependency-first order: dep_a, dep_b, entry module")
    }

    t |> run("a required module's [init] is a collected error, not a crash") @(t : T?) {
        let input = path_join(get_das_root(), "tests/aot/_standalone_foreign_init_fixture.das")
        var ok = true
        using() $(var cop : CodeOfPolicies) {
            standalone_cop(cop)
            ast_gc_guard() {
                ok = standalone_aot(input, out_dir, false, false, cop)
            }
        }
        t |> success(!ok, "standalone_aot reports failure")
        let generated = path_join(out_dir, "_standalone_foreign_init_fixture.das.cpp")
        t |> success(!stat(generated).is_valid, "no output is written on failure")
    }

    t |> run("before_emit receives the compiled program") @(t : T?) {
        let input = path_join(get_das_root(), "tests/aot/_standalone_novars_fixture.das")
        var saw_program = false
        using() $(var cop : CodeOfPolicies) {
            standalone_cop(cop)
            ast_gc_guard() {
                standalone_aot(input, out_dir, false, false, cop) $(var program : ProgramPtr) {
                    saw_program = program != null
                }
            }
        }
        t |> success(saw_program, "the before_emit block ran with a non-null program")
        remove(path_join(out_dir, "_standalone_novars_fixture.das.cpp"))
        remove(path_join(out_dir, "_standalone_novars_fixture.das.h"))
    }

    rmdir(out_dir)
}
