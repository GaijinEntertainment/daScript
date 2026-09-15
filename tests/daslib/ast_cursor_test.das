options gen2
options no_aot
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/ast_boost
require daslib/ast_cursor
require daslib/rtti
require strings

def fixture_path() : string {
    return "{get_das_root()}/tests/daslib/ast_cursor_fixture.das"
}

def compile_fixture(blk : block<(program : smart_ptr<Program>) : void>) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.export_all = true
            compile_file(fixture_path(), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    to_log(LOG_ERROR, "compile error: {issues}\n")
                    return
                }
                simulate(program) $(sok; _ctx; serrors) {
                    if (!sok) {
                        to_log(LOG_ERROR, "simulate error: {serrors}\n")
                        return
                    }
                    invoke(blk, program)
                }
            }
        }
    }
}

[test]
def test_find_var_at_cursor(t : T?) {
    t |> run("finds ExprVar at variable reference") @(t2 : T?) {
        compile_fixture() $(program) {
            // ExprVar(a) at 10:11-10:12
            let hits = find_at_cursor(program, "ast_cursor_fixture", 10, 11)
            t2 |> success(!empty(hits), "should find at least one hit")
            t2 |> success(hits[0].rtti == "ExprVar", "first hit should be ExprVar, got {hits[0].rtti}")
            t2 |> success(hits[0].name == "a", "should be named 'a', got '{hits[0].name}'")
        }
    }
}

[test]
def test_find_call_at_cursor(t : T?) {
    t |> run("finds ExprCall at function call") @(t2 : T?) {
        compile_fixture() $(program) {
            // ExprCall(to_log) at 19:4-19:10 (line 19 after macro expansion of string builder)
            let hits = find_at_cursor(program, "ast_cursor_fixture", 19, 5)
            var found_call = false
            for (h in hits) {
                if (h.rtti == "ExprCall" && h.name == "to_log") {
                    found_call = true
                    break
                }
            }
            t2 |> success(found_call, "should find ExprCall(to_log)")
        }
    }
}

[test]
def test_innermost_first(t : T?) {
    t |> run("innermost expression comes first") @(t2 : T?) {
        compile_fixture() $(program) {
            // ExprOp2(+) at 10:13-10:14 — contains just the '+', inside ExprBlock(9:26-11:1)
            // Hitting col 13 should find ExprOp2 inside ExprBlock
            let hits = find_at_cursor(program, "ast_cursor_fixture", 10, 13)
            t2 |> success(length(hits) >= 2, "should find multiple hits, got {length(hits)}")
            // first should be the innermost ExprOp2
            t2 |> success(hits[0].rtti == "ExprOp2", "first should be ExprOp2, got {hits[0].rtti}")
            // ExprBlock should be further out
            var found_block = false
            for (i in range(1, length(hits))) {
                if (hits[i].rtti == "ExprBlock") {
                    found_block = true
                    break
                }
            }
            t2 |> success(found_block, "should find ExprBlock after ExprOp2")
        }
    }
}

[test]
def test_enclosing_function(t : T?) {
    t |> run("reports enclosing function") @(t2 : T?) {
        compile_fixture() $(program) {
            // ExprVar(a) at 10:11 is inside function 'add'
            let hits = find_at_cursor(program, "ast_cursor_fixture", 10, 11)
            t2 |> success(!empty(hits), "should find hits")
            t2 |> success(hits[0].func != null, "should have enclosing function")
            t2 |> success(hits[0].func.name == "add", "should be in function 'add', got '{hits[0].func.name}'")
        }
    }
}

[test]
def test_field_access(t : T?) {
    t |> run("finds ExprField for field access") @(t2 : T?) {
        compile_fixture() $(program) {
            // ExprField(x) at 21:24-21:26
            let hits = find_at_cursor(program, "ast_cursor_fixture", 21, 24)
            var found_field = false
            for (h in hits) {
                if (h.rtti == "ExprField" && h.name == "x") {
                    found_field = true
                    break
                }
            }
            t2 |> success(found_field, "should find ExprField(x)")
        }
    }
}

[test]
def test_no_hits_outside(t : T?) {
    t |> run("returns empty for position outside code") @(t2 : T?) {
        compile_fixture() $(program) {
            let hits = find_at_cursor(program, "ast_cursor_fixture", 999, 1)
            t2 |> success(empty(hits), "should find no hits")
        }
    }
}

[test]
def test_describe_hit(t : T?) {
    t |> run("describe produces readable output") @(t2 : T?) {
        compile_fixture() $(program) {
            // ExprVar(a) at 10:11
            let hits = find_at_cursor(program, "ast_cursor_fixture", 10, 11)
            t2 |> success(!empty(hits), "should find hits")
            let desc = describe(hits[0])
            t2 |> success(find(desc, "ExprVar") >= 0, "describe should contain ExprVar")
            t2 |> success(find(desc, "a") >= 0, "describe should contain name")
        }
    }
}

def variable_hit(hits : array<CursorHit>; name : string) : bool {
    for (h in hits) {
        return true if (h.variable != null && h.name == name && h.expr == null)
    }
    return false
}

[test]
def test_find_declaration_at_cursor(t : T?) {
    t |> run("a for loop variable in its header is a variable hit when declarations are asked for") @(t2 : T?) {
        compile_fixture() $(program) {
            // for (i in range(2)) at 24:9
            let hits = find_at_cursor(program, "ast_cursor_fixture", 24, 9, true)
            let seen <- [for (h in hits); describe(h)]
            t2 |> success(variable_hit(hits, "i"), "for variable i among {seen}")
            t2 |> success(!empty(hits) && hits[0].variable != null && hits[0].name == "i", "the variable hit is innermost: {seen}")
            t2 |> success(!empty(hits) && find(describe(hits[0]), "int") >= 0, "describe carries the variable's type: {seen}")
        }
    }
    t |> run("a function argument and a let variable are variable hits") @(t2 : T?) {
        compile_fixture() $(program) {
            // def add(a, b : int) at 9:8 / 9:11; let p at 20:8
            t2 |> success(variable_hit(find_at_cursor(program, "ast_cursor_fixture", 9, 8, true), "a"), "argument a")
            t2 |> success(variable_hit(find_at_cursor(program, "ast_cursor_fixture", 9, 11, true), "b"), "argument b")
            t2 |> success(variable_hit(find_at_cursor(program, "ast_cursor_fixture", 20, 8, true), "p"), "let variable p")
        }
    }
    t |> run("without the flag a declaration yields no variable hit") @(t2 : T?) {
        compile_fixture() $(program) {
            t2 |> success(!variable_hit(find_at_cursor(program, "ast_cursor_fixture", 24, 9), "i"), "no variable hit by default")
            t2 |> success(!variable_hit(find_at_cursor(program, "ast_cursor_fixture", 9, 8), "a"), "no argument hit by default")
        }
    }
}
