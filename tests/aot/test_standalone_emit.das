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

def private generate_standalone_files(t : T?; fixture, out_dir : string; cross_platform : bool = false; stack_policy : uint = 0u) : tuple<header : string; source : string> {
    let input = path_join(get_das_root(), "tests/aot/{fixture}.das")
    using() $(var cop : CodeOfPolicies) {
        standalone_cop(cop)
        if (stack_policy != 0u) {
            cop.stack = stack_policy
        }
        ast_gc_guard() {
            standalone_aot(input, out_dir, cross_platform, false, cop)
        }
    }
    var files : tuple<header : string; source : string>
    let generated = path_join(out_dir, "{fixture}.das.cpp")
    fopen(generated, "rb") $(fr) {
        if (fr != null) {
            files.source = fread(fr)
        }
    }
    fopen(path_join(out_dir, "{fixture}.das.h"), "rb") $(fr) {
        if (fr != null) {
            files.header = fread(fr)
        }
    }
    if (empty(files.source)) {
        t |> failure("standalone_aot produced no {generated}")
    }
    remove(generated)
    remove(path_join(out_dir, "{fixture}.das.h"))
    return files
}

def private generate_standalone_source(t : T?; fixture, out_dir : string) : string {
    return generate_standalone_files(t, fixture, out_dir).source
}

def private expect_emit_failure(t : T?; fixture, out_dir : string) {
    let input = path_join(get_das_root(), "tests/aot/{fixture}.das")
    var ok = true
    using() $(var cop : CodeOfPolicies) {
        standalone_cop(cop)
        ast_gc_guard() {
            ok = standalone_aot(input, out_dir, false, false, cop)
        }
    }
    t |> success(!ok, "standalone_aot reports failure")
    let generated = path_join(out_dir, "{fixture}.das.cpp")
    t |> success(!stat(generated).is_valid, "no output is written on failure")
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
def test_standalone_emit(t : T?) {   // nolint:STYLE038 - flat list of emit subcases
    var tmp_err : string
    let out_dir = create_temp_directory("standalone_emit", tmp_err)
    if (!empty(tmp_err)) {
        t |> failure("cannot create temp directory: {tmp_err}")
        return
    }

    t |> run("a context with no init functions still closes its constructor") @(t : T?) {
        let source = generate_standalone_source(t, "_standalone_novars_fixture", out_dir)
        t |> success(find(source, "Standalone::Standalone() : Context(16384/*stack*/) \{") >= 0, "constructor was emitted with the default stack size")
        t |> equal(brace_balance(source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("the context stack and heap hints honor the script's options") @(t : T?) {
        let source = generate_standalone_source(t, "_standalone_options_fixture", out_dir)
        t |> success(find(source, ": Context(262144/*stack*/)") >= 0, "the ctor sizes the stack from options stack")
        t |> success(find(source, "policies.heap_size_hint = 123456;") >= 0, "heap_size_hint comes from the script's options")
        t |> success(find(source, "policies.string_heap_size_hint = 234567;") >= 0, "string_heap_size_hint comes from the script's options")
        t |> success(find(source, "unused_interpreted_helper") < 0, "an unused [no_aot] function neither blocks emission nor emits")
        t |> equal(brace_balance(source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("an explicit stack option below the floor is honored exactly") @(t : T?) {
        let source = generate_standalone_source(t, "_standalone_small_stack_fixture", out_dir)
        t |> success(find(source, ": Context(4096/*stack*/)") >= 0, "options stack = 4096 emits 4096 - embedded targets budget the stack")
    }

    t |> run("an explicit stack of zero keeps the floor - Context(0) has no stack at all") @(t : T?) {
        let source = generate_standalone_source(t, "_standalone_zero_stack_fixture", out_dir)
        t |> success(find(source, ": Context(16384/*stack*/)") >= 0, "options stack = 0 falls back to the floored default")
    }

    t |> run("a sub-floor policies stack without an explicit option is raised to the floor") @(t : T?) {
        let files = generate_standalone_files(t, "_standalone_novars_fixture", out_dir, stack_policy = 8192u)
        t |> success(find(files.source, ": Context(16384/*stack*/)") >= 0, "policies.stack = 8192 still gets the 16K floor")
    }

    t |> run("with no stack option the policies' stack size decides") @(t : T?) {
        let files = generate_standalone_files(t, "_standalone_novars_fixture", out_dir, stack_policy = 32768u)
        t |> success(find(files.source, ": Context(32768/*stack*/)") >= 0, "the ctor sizes the stack from CodeOfPolicies")
    }

    t |> run("forward declarations spell types the way the definitions do") @(t : T?) {
        let source = generate_standalone_source(t, "_standalone_tuple_fixture", out_dir)
        t |> success(find(source, "TTuple<") >= 0, "the tuple definition was emitted")
        t |> success(find(source, "AutoTuple<") < 0, "cross-platform spelling leaked into a native emit")
        t |> equal(brace_balance(source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("the constructor runs the init script and then the [init] functions") @(t : T?) {
        let source = generate_standalone_source(t, "_standalone_init_fixture", out_dir)
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
        let files = generate_standalone_files(t, "_standalone_cross_module_fixture", out_dir)
        let a_pos = find(files.source, "/*g_dep_a*/")
        let b_pos = find(files.source, "/*g_dep_b*/")
        let main_pos = find(files.source, "/*g_main*/")
        t |> success(a_pos >= 0 && b_pos >= 0 && main_pos >= 0, "all three globals were emitted")
        t |> success(a_pos < b_pos && b_pos < main_pos, "dependency-first order: dep_a, dep_b, entry module")
        t |> success(find(files.header, "static_assert(sizeof(DepSpan)") >= 0, "a required module's struct definition is in the header")
        t |> success(find(files.header, "struct _standalone_dep_a::DepSpan") >= 0, "the method spells the required module's struct by its public name")
        t |> success(find(files.source, "static_assert(sizeof(DepSpan)") < 0, "the source does not redefine the required module's struct")
    }

    t |> run("struct and enum signatures put the type definitions in the header") @(t : T?) {
        let files = generate_standalone_files(t, "_standalone_struct_sig_fixture", out_dir)
        t |> success(find(files.header, "static_assert(sizeof(Pair)") >= 0, "the struct definition is in the header")
        t |> success(find(files.header, "enum class Mode") >= 0, "the enum definition is in the header")
        t |> success(find(files.header, "-> struct _standalone_struct_sig_fixture::Pair") >= 0, "the method spells the struct by its public name")
        t |> success(find(files.header, "DAS_COMMENT(enum) _standalone_struct_sig_fixture::Mode") >= 0, "the method spells the enum by its public name")
        t |> success(find(files.header, "pair_sum") >= 0, "the struct-taking method is declared")
        t |> success(find(files.header, "namespace  \{") < 0, "no 'namespace  \{' (empty module name) in the header")
        t |> success(find(files.header, "#pragma once") >= 0, "the header guards against double inclusion")
        t |> success(find(files.header, "bin_serializer.h") >= 0, "required-module includes are in the header")
        t |> success(find(files.source, "static_assert(sizeof(Pair)") < 0, "the source does not redefine the struct")
        t |> success(find(files.source, "enum class Mode") < 0, "the source does not redefine the enum")
        t |> equal(brace_balance(files.header), 0, "unbalanced braces in the generated header")
        t |> equal(brace_balance(files.source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("cross-platform debug info spells entry-module types by their public name") @(t : T?) {
        let files = generate_standalone_files(t, "_standalone_struct_sig_fixture", out_dir, true)
        t |> success(find(files.source, "offsetof(_standalone_struct_sig_fixture::Pair,") >= 0, "offsetof names the qualified struct")
        t |> success(find(files.source, "TypeSize<_standalone_struct_sig_fixture::Pair>") >= 0, "TypeSize names the qualified struct")
        t |> success(find(files.source, "DAS_COMMENT(enum) _standalone_struct_sig_fixture::Mode>") >= 0, "TypeSize names the qualified enum")
        t |> success(find(files.source, "offsetof(Pair,") < 0, "no unqualified offsetof survives")
        t |> success(find(files.source, "TypeSize<Pair>") < 0, "no unqualified struct TypeSize survives")
        t |> equal(brace_balance(files.source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("a compile-time macro stays out of the standalone context") @(t : T?) {
        // AOT mode used to force-export every generated struct ctor, macro classes
        // included; that rooted the whole macro into the simulated context and the
        // [no_aot] check below then reported its visit() as an unemittable function.
        let source = generate_standalone_source(t, "_standalone_macro_fixture", out_dir)
        t |> success(find(source, "5050") >= 0, "the macro's compile-time sum is baked into the source")
        t |> success(find(source, "BakeSum") < 0, "the macro class does not reach the generated source")
        t |> equal(brace_balance(source), 0, "unbalanced braces in the generated C++")
    }

    t |> run("a used [no_aot] function is a collected error, not a runtime crash") @(t : T?) {
        expect_emit_failure(t, "_standalone_no_aot_fixture", out_dir)
    }

    t |> run("a required module's [init] is a collected error, not a crash") @(t : T?) {
        expect_emit_failure(t, "_standalone_foreign_init_fixture", out_dir)
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
