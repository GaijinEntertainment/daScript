options gen2
// tests here invoke the compiler at runtime (compile_file) - not
// AOT-linkable
options no_aot

require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost
require daslib/rtti

def private keep_fixture_path() : string =>
    "{get_das_root()}/tests/ast/_keep_symbol_tables_fixture.das"

//! runs `blk` only if the fixture compiles and simulates under
//! `keep_symbol_tables = keep`
def private with_simulated_fixture(t : T?; keep : bool;
                                   blk : block<(program : smart_ptr<Program>) : void>) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.keep_symbol_tables = keep
            compile_file(keep_fixture_path(), access, unsafe(addr(mg)),
                         cop) $(ok; program; issues) {
                t |> success(ok, "the fixture did not compile: {issues}")
                return if (!ok || program == null)
                simulate(program) $(sok; _context; serrors) {
                    t |> success(sok, "the fixture did not simulate: {serrors}")
                    return if (!sok)
                    invoke(blk, program)
                }
            }
        }
    }
}

//! after simulate, `main` is in the symbol tables iff keep is set
def private check_tables(t : T?; keep : bool) {
    with_simulated_fixture(t, keep) $(program) {
        let fn = find_unique_function(get_this_module(program), "main")
        t |> equal(is_used(program, fn), keep, "is_used(main) with keep={keep}")
        let indexed = function_index(program, fn) >= 0
        t |> equal(indexed, keep, "function_index(main) with keep={keep}")
    }
}

[test(description = "the program's symbol tables are freed after compilation by default")]
def test_symbol_tables_freed_by_default(t : T?) {
    check_tables(t, false)
}

[test(description = "keep_symbol_tables retains the program's symbol tables after compilation")]
def test_keep_symbol_tables(t : T?) {
    check_tables(t, true)
}

[test(description = "a simulated program keeps no index record for a builtin function, so each reads -1 like a function the program never saw, while main holds a slot")]
def test_builtin_functions_hold_no_index_record(t : T?) {
    with_simulated_fixture(t, true) $(program) {
        var builtins = 0
        program_for_each_module(program) $(mod) {
            for_each_function(mod, "") $(fn) {
                return if (!fn.flags.builtIn)
                builtins++
                let index = function_index(program, fn)
                if (index != -1) {
                    t |> failure("builtin {mod.name}::{fn.name} holds index {index}")
                }
            }
        }
        t |> success(builtins > 0, "the fixture's library holds builtin functions")
        let main_fn = find_unique_function(get_this_module(program), "main")
        t |> success(function_index(program, main_fn) >= 0, "main holds a slot")
    }
}
