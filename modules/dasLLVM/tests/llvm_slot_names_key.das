options gen2
options no_aot

require dastest/testing_boost public
require llvm/daslib/llvm_jit_plan
require daslib/ast_boost
require daslib/rtti
require daslib/fio
require daslib/strings_boost

let private FIXTURE = "modules/dasLLVM/tests/_slot_names_fixture.das"
let private FOLD_CHILD = "modules/dasLLVM/tests/_slot_names_fold_child.das"

struct private FixtureKeys {
    compiled : bool
    var_aot : uint64
    let_aot : uint64
    var_key : string
    let_key : string
    var_fold : uint64
    var_fold_selected : uint64
    other_fold : uint64
}

def private named(mod : Module?; name : string) : array<FunctionPtr> {
    var found : array<FunctionPtr>
    for_each_function(mod, name) $(fn) {
        found |> push(fn)
    }
    return <- found
}

def private dll_key(prog : Program?; funcs : array<FunctionPtr>) : string =>
    jit_dll_basename(prog, funcs, 3, 0, false, false, "", false, false, JitSanitize())

//! the fixture's keys and folds, read from one compiled and simulated program
def private fixture_keys() : FixtureKeys {
    var res : FixtureKeys
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file("{get_das_root()}/{FIXTURE}", access, unsafe(addr(mg)), cop) $(compiled; var program; _issues) {
                return if (!compiled)
                simulate(program) $(simulated; _context; _errors) {
                    return if (!simulated)
                    let prog = get_ptr(program)
                    let mod = get_this_module(program)
                    let by_var <- named(mod, "by_var")
                    let by_let <- named(mod, "by_let")
                    let other <- named(mod, "other_slots")
                    return if (length(by_var) != 1 || length(by_let) != 1 || length(other) != 1)
                    res.compiled = true
                    res.var_aot = get_function_aot_hash(by_var[0])
                    res.let_aot = get_function_aot_hash(by_let[0])
                    res.var_key = dll_key(prog, by_var)
                    res.let_key = dll_key(prog, by_let)
                    res.other_fold = fold_extern_slot_names(prog, other, 0ul)
                    res.var_fold = fold_extern_slot_names(prog, by_var, 0ul)
                    var everything : array<FunctionPtr>
                    program_for_each_module(program) $(m) {
                        everything |> push_from(named(m, ""))
                    }
                    mark_jit_selection(prog, everything)
                    res.var_fold_selected = fold_extern_slot_names(prog, by_var, 0ul)
                    clear_jit_selection(prog)
                }
            }
        }
    }
    return res
}

[test]
def test_slot_names_key(t : T?) {
    let keys = fixture_keys()
    if (!keys.compiled) {
        t |> failure("the fixture {FIXTURE} did not compile and simulate")
        return
    }
    t |> run("a local that turns let keeps the AOT hash and moves the DLL key") @(t : T?) {
        t |> equal(keys.var_aot, keys.let_aot, "the AOT hash does not see the const on the at slot")
        t |> success(keys.var_key != keys.let_key, "by_var and by_let key the same DLL")
    }
    t |> run("the fold does not read the jit selection, so every plan of one program keys the same") @(t : T?) {
        t |> success(keys.var_fold != 0ul, "the walk folded by_var's slots")
        t |> equal(keys.var_fold, keys.var_fold_selected)
    }
    t |> run("another process folds the same names") @(t : T?) {
        let args <- get_command_line_arguments()
        var output : string
        let rc = run_and_capture([args[0], "-dasroot", get_das_root(), "{get_das_root()}/{FOLD_CHILD}"], output, 120.0)
        t |> equal(rc, 0, output)
        t |> success(output |> contains("fold={keys.other_fold}"), "this process folded {keys.other_fold}, the child printed: {output}")
    }
}
