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
            cop.no_optimizations = true   // cursor tests assert node positions; folding would erase the nodes
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

def has_variable_hit(hits : array<CursorHit>; name : string) : bool {
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
            t2 |> success(has_variable_hit(hits, "i"), "for variable i among {seen}")
            t2 |> success(!empty(hits) && hits[0].variable != null && hits[0].name == "i", "the variable hit is innermost: {seen}")
            t2 |> success(!empty(hits) && find(describe(hits[0]), "int") >= 0, "describe carries the variable's type: {seen}")
        }
    }
    t |> run("a function argument and a let variable are variable hits") @(t2 : T?) {
        compile_fixture() $(program) {
            // def add(a, b : int) at 9:8 / 9:11; let p at 20:8
            t2 |> success(has_variable_hit(find_at_cursor(program, "ast_cursor_fixture", 9, 8, true), "a"), "argument a")
            t2 |> success(has_variable_hit(find_at_cursor(program, "ast_cursor_fixture", 9, 11, true), "b"), "argument b")
            t2 |> success(has_variable_hit(find_at_cursor(program, "ast_cursor_fixture", 20, 8, true), "p"), "let variable p")
        }
    }
    t |> run("multi-line spans order by lines: the call, then the loop body block, then main's block") @(t2 : T?) {
        compile_fixture() $(program) {
            // to_log at 25:8 inside the for body block (24:24-26:5) inside main's block (18:11-27:1)
            let hits = find_at_cursor(program, "ast_cursor_fixture", 25, 8)
            var call_idx = -1
            var body_idx = -1
            var main_idx = -1
            for (idx in range(length(hits))) {
                if (hits[idx].rtti == "ExprCall" && hits[idx].name == "to_log") {
                    call_idx = idx
                } elif (hits[idx].rtti == "ExprBlock" && hits[idx].expr != null && int(hits[idx].expr.at.line) == 24) {
                    body_idx = idx
                } elif (hits[idx].rtti == "ExprBlock" && hits[idx].expr != null && int(hits[idx].expr.at.line) == 18) {
                    main_idx = idx
                }
            }
            t2 |> success(call_idx >= 0 && body_idx >= 0 && main_idx >= 0, "the call, the body block and main's block all contain the cursor: {[for (h in hits); describe(h)]}")
            t2 |> success(call_idx < body_idx && body_idx < main_idx, "fewer lines first: {[for (h in hits); describe(h)]}")
        }
    }
    t |> run("without the flag a declaration yields no variable hit") @(t2 : T?) {
        compile_fixture() $(program) {
            t2 |> success(!has_variable_hit(find_at_cursor(program, "ast_cursor_fixture", 24, 9), "i"), "no variable hit by default")
            t2 |> success(!has_variable_hit(find_at_cursor(program, "ast_cursor_fixture", 9, 8), "a"), "no argument hit by default")
        }
    }
}

[test]
def test_synthesized_members(t : T?) {
    t |> run("a cursor on a class name yields no hit from the members the compiler puts on that line") @(t2 : T?) {
        compile_fixture() $(program) {
            // class Cat : Animal at 35:6
            let hits = find_at_cursor(program, "ast_cursor_fixture", 35, 8, true)
            t2 |> success(empty(hits), "no expression hit on a class line: {[for (h in hits); describe(h)]}")
        }
    }
    t |> run("is_synthesized tells the compiler's members from the user's") @(t2 : T?) {
        compile_fixture() $(program) {
            var user = 0
            var synthesized = 0
            ast::for_each_function(get_this_module(program), "") $(var func : FunctionPtr) {
                let f = func
                if (is_synthesized(f)) {
                    synthesized++
                } else {
                    user++
                }
            }
            t2 |> success(synthesized > 0, "a class carries synthesized members: {synthesized}")
            t2 |> equal(user, 6, "add, greet, main, Animal`speak, Cat`speak, no_log - got {user}")
        }
    }
}

[test]
def test_find_declaration(t : T?) {
    t |> run("a class name, its parent on the derived line, a function name") @(t2 : T?) {
        compile_fixture() $(program) {
            let cat = find_declaration_at_cursor(program, "ast_cursor_fixture", 35, 7, "class Cat : Animal \{")
            t2 |> success(cat.kind == DeclarationKind.structure && cat.name == "Cat", "class name: {cat.name}")
            t2 |> equal(cat.word_column, 6, "the name starts at column 6")
            let parent = find_declaration_at_cursor(program, "ast_cursor_fixture", 35, 14, "class Cat : Animal \{")
            t2 |> success(parent.kind == DeclarationKind.structure && parent.name == "Animal", "parent name resolves the parent: {parent.name}")
            t2 |> equal(int(parent.at.line), 31, "the parent's own position")
            let fn = find_declaration_at_cursor(program, "ast_cursor_fixture", 9, 6, "def add(a, b : int) : int \{")
            t2 |> success(fn.kind == DeclarationKind.func && fn.name == "add", "function name: {fn.name}")
            let method = find_declaration_at_cursor(program, "ast_cursor_fixture", 36, 20, "    def override speak() : string => \"meow\"")
            t2 |> success(method.kind == DeclarationKind.func && method.name == "speak", "method name is bare: {method.name}")
        }
    }
    t |> run("the cursor must be on the name token; a line-only lookup takes any declaration on the line") @(t2 : T?) {
        compile_fixture() $(program) {
            let off = find_declaration_at_cursor(program, "ast_cursor_fixture", 9, 1, "def add(a, b : int) : int \{")
            t2 |> success(off.kind == DeclarationKind.none, "cursor on `def` is not the declaration")
            let any = find_declaration_at_cursor(program, "ast_cursor_fixture", 9, 1, "")
            t2 |> success(any.kind == DeclarationKind.func && any.name == "add" && any.word_column < 0, "line-only lookup finds add with no column")
            let none = find_declaration_at_cursor(program, "ast_cursor_fixture", 19, 4, "")
            t2 |> success(none.kind == DeclarationKind.none, "a statement line declares nothing")
        }
    }
}

[test]
def test_text_helpers(t : T?) {
    t |> run("word_at, name_column, spells_name, source_name, bare_name") @(t2 : T?) {
        let w = word_at("    let dx = a.x - b.x", 15)
        t2 |> success(w.word == "x" && w.start == 15, "word under the cursor: {w.word} at {w.start}")
        let mid = word_at("class Animal \{", 9)
        t2 |> success(mid.word == "Animal" && mid.start == 6, "cursor mid-word: {mid.word} at {mid.start}")
        let tail = word_at("class Animal \{", 5)
        t2 |> success(tail.word == "class" && tail.start == 0, "the cursor right after a word is on it: {tail.word}")
        let gap = word_at("class  Animal", 6)
        t2 |> success(empty(gap.word), "cursor between spaces is no word: '{gap.word}'")
        t2 |> equal(name_column("class Cat : Animal \{", "Animal", -1), 12)
        t2 |> equal(name_column("def catalog(cat : Cat)", "cat", -1), 12, "a whole word, not the prefix of catalog")
        t2 |> equal(name_column("nothing here", "Cat", 7), 7, "the hint when the name is not spelled")
        t2 |> success(spells_name("class Cat : Animal \{", 12, "Animal"), "spelled at 12")
        t2 |> success(!spells_name("class Cat : Animal \{", 13, "Animal"), "not starting at 13")
        t2 |> equal(source_name("__i_rename_at_6_13"), "i")
        t2 |> equal(source_name("plain"), "plain")
        t2 |> equal(bare_name("Animal`speak"), "speak")
        t2 |> equal(bare_name("speak"), "speak")
    }
}

[test]
def test_field_name_hit(t : T?) {
    t |> run("a cursor on the field name of a field access hits the access") @(t2 : T?) {
        compile_fixture() $(program) {
            // to_log(LOG_INFO, "{p.x + p.y}\n") at line 21: p.x with x at column 25
            let hits = find_at_cursor(program, "ast_cursor_fixture", 21, 25)
            var found = false
            for (h in hits) {
                found = found || (h.rtti == "ExprField" && h.name == "x")
            }
            t2 |> success(found, "ExprField(x) among {[for (h in hits); describe(h)]}")
        }
    }
}

def has_type_hit(hits : array<CursorHit>; name : string = "") : bool {
    for (h in hits) {
        return true if (h.type_decl != null && (empty(name) || h.name == name))
    }
    return false
}

[test]
def test_type_name_hit(t : T?) {
    t |> run("a cursor on a type name yields the type; on a forward-referenced class's new site too") @(t2 : T?) {
        compile_fixture() $(program) {
            // var pet = new Cat() at line 27, Cat at column 18
            let hits = find_at_cursor(program, "ast_cursor_fixture", 27, 19, true, "    var pet = new Cat()")
            t2 |> success(!empty(hits) && hits[0].type_decl != null && hits[0].name == "Cat", "type hit first: {[for (h in hits); describe(h)]}")
            t2 |> success(!empty(hits) && hits[0].type_decl != null && int(hits[0].type_decl.at.column) == 18, "at the name, not the new keyword: {[for (h in hits); describe(h)]}")
            let prim = find_at_cursor(program, "ast_cursor_fixture", 9, 16, true, "def add(a, b : int) : int \{")
            t2 |> success(!has_type_hit(prim), "no type hit on a builtin type: {[for (h in prim); describe(h)]}")
            let meth = find_at_cursor(program, "ast_cursor_fixture", 36, 20, true, "    def override speak() : string => \"meow\"")
            t2 |> success(!has_type_hit(meth, "Cat"), "the implicit self is not a type hit: {[for (h in meth); describe(h)]}")
        }
    }
    t |> run("without the line text there is no type hit") @(t2 : T?) {
        compile_fixture() $(program) {
            let hits = find_at_cursor(program, "ast_cursor_fixture", 27, 19, true)
            t2 |> success(!has_type_hit(hits), "no line text, no type hit")
        }
    }
    t |> run("a type's declaration is its class, with the name column spelled on the class line") @(t2 : T?) {
        compile_fixture() $(program) {
            let hits = find_at_cursor(program, "ast_cursor_fixture", 27, 19, true, "    var pet = new Cat()")
            let cat = declaration_of(program, hits[0].type_decl)
            t2 |> success(cat.kind == DeclarationKind.structure && cat.name == "Cat" && int(cat.at.line) == 35, "the class: {cat.name} at {int(cat.at.line)}")
            t2 |> equal(declared_name_column(cat.at, cat.name), 6, "the name, not the class keyword")
            t2 |> equal(declaration_of(program, null).kind, DeclarationKind.none)
        }
    }
    t |> run("a typedef another module declares resolves to that module's declaration") @(t2 : T?) {
        compile_fixture() $(program) {
            // def no_log() : file { at line 39, file at column 15; fio declares `typedef file = FILE const?` on its line 12
            let hits = find_at_cursor(program, "ast_cursor_fixture", 39, 16, true, "def no_log() : file \{")
            t2 |> success(has_type_hit(hits, "file"), "the alias is a type hit: {[for (h in hits); describe(h)]}")
            let alias = declaration_of(program, hits[0].type_decl)
            t2 |> success(alias.kind == DeclarationKind.alias && alias.alias_type != null, "resolved: {alias.name} kind {alias.kind}")
            t2 |> success(int(alias.at.line) == 12 && alias.at.fileInfo != null && find(string(alias.at.fileInfo.name), "fio") >= 0, "declared in fio at {int(alias.at.line)}")
            t2 |> success(typedef_named(program, "no_such_alias") == null, "an unknown alias is null")
        }
    }
}
