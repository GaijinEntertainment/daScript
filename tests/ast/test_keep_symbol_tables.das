options gen2
options no_aot

require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost

def private fixture_path() : string => "{get_das_root()}/tests/ast/_keep_symbol_tables_fixture.das"

//! after simulate, `main` is in the symbol tables iff keep is set
def private check_tables(t : T?; keep : bool) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.keep_symbol_tables = keep
            compile_file(fixture_path(), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the fixture did not compile: {issues}")
                return if (!ok || program == null)
                simulate(program) $(sok; _context; serrors) {
                    t |> success(sok, "the fixture did not simulate: {serrors}")
                    return if (!sok)
                    let fn = find_unique_function(get_this_module(program), "main")
                    t |> equal(is_used(program, fn), keep, "is_used(main) with keep={keep}")
                    let indexed = function_index(program, fn) >= 0
                    t |> equal(indexed, keep, "function_index(main) with keep={keep}")
                }
            }
        }
    }
}

[test]
def test_symbol_tables_freed_by_default(t : T?) {
    check_tables(t, false)
}

[test]
def test_keep_symbol_tables(t : T?) {
    check_tables(t, true)
}
