options gen2
options no_aot
options stack = 1_048_576

require daslib/aot_standalone
require daslib/ast_boost
require daslib/fio
require daslib/rtti
require strings
require dastest/testing_boost public

def private generate_standalone(t : T?; fixture, out_dir : string) : string {
    let input = path_join(get_das_root(), "tests/aot/{fixture}.das")
    using() $(var cop : CodeOfPolicies) {
        cop.threadlock_context = false
        cop.aot = false
        cop.aot_module = true
        cop.aot_lib = false
        cop.fail_on_lack_of_aot_export = true
        cop.ignore_shared_modules = false
        cop.version_2_syntax = true
        cop.tune_frozen = true
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

    rmdir(out_dir)
}
