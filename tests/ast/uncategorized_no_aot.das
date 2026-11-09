options gen2
// tests here invoke the compiler at runtime (compile_file) - not
// AOT-linkable
options no_aot

require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/rtti
require daslib/ast_print
require strings

class CountInts : AstConstVisitor {
    ints : int = 0
    sum : int = 0

    def override preVisitExprConstInt(expr : ExprConstInt const?) : void {
        ints ++
        sum += expr.value
    }
}

def count_ints(node) : int2 {
    var visitor = new CountInts()
    make_const_visitor(*visitor) $(adapter) {
        visit(node, adapter)
    }
    let res = int2(visitor.ints, visitor.sum)
    unsafe {
        delete visitor
    }
    return res
}

[test(description = "a const AST visitor walks every node of an expression and the body of a function")]
def test_const_visitor(t : T?) {
    t |> run("a const visitor walks every node of an expression") @(t : T?) {
        ast_gc_guard() {
            var expr <- quote(1 + 2 * 3)
            t |> equal(int2(3, 6), count_ints(expr))
        }
    }
    t |> run("a const visitor walks the body of a function") @(t : T?) {
        ast_gc_guard() {
            var func <- qmacro_function("const_visitor_probe") $() : int {
                return 4 + 5
            }
            t |> equal(int2(2, 9), count_ints(func))
        }
    }
}

def private keep_fixture_path() : string =>
    "{get_das_root()}/tests/ast/_keep_symbol_tables_fixture.das"

//! after simulate, `main` is in the symbol tables iff keep is set
def private check_tables(t : T?; keep : bool) {
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
                    let fn = find_unique_function(get_this_module(program), "main")
                    t |> equal(is_used(program, fn), keep, "is_used(main) with keep={keep}")
                    let indexed = function_index(program, fn) >= 0
                    t |> equal(indexed, keep, "function_index(main) with keep={keep}")
                }
            }
        }
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

//! ExprMakeArray.constValues: inference marks a literal whose every
//! element is a typed constant of the element type, and leaves a
//! literal with a non-constant element unmarked. The flag is what
//! lets InferTypes::canVisitMakeArray skip re-walking the literal
//! each pass.


def make_array_fixture_path() : string =>
    "{get_das_root()}/tests/ast/_make_array_const_values_fixture.das"

// `array<T>(...)` globals init as to_array_move(ExprMakeArray);
// `fixed_array<T>(...)` as the ExprMakeArray itself
def global_make_array(program : ProgramPtr; name : string) : ExprMakeArray? {
    let gv = find_variable(get_this_module(program), name)
    if (gv == null || gv.init == null) {
        return null
    }
    var found = gv.init ?as ExprMakeArray
    if (found == null) {
        let call = gv.init ?as ExprCall
        if (call != null && length(call.arguments) == 1) {
            found = call.arguments[0] ?as ExprMakeArray
        }
    }
    return found
}

[test(description = "an array literal is flagged all-constant only when every element is constant")]
def test_make_array_const_values(t : T?) {
    t |> run("all-constant literals are flagged, a literal with a variable element is not") <| @(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.export_all = true
                cop.no_optimizations = true
                cop.no_infer_time_folding = true
                compile_file(make_array_fixture_path(), access, unsafe(addr(mg)),
                             cop) $(ok; program; issues) {
                    t |> success(ok, "the fixture did not compile: {issues}")
                    if (!ok || program == null) return
                    let all_const = global_make_array(program, "g_all_const")
                    let mixed = global_make_array(program, "g_mixed")
                    let fixed = global_make_array(program, "g_fixed")
                    t |> success(all_const != null && mixed != null && fixed != null,
                                 "the fixture globals are not array literals")
                    if (all_const == null || mixed == null || fixed == null) return
                    t |> success(all_const.constValues,
                                 "array<int>(1, 2, 3) is not flagged constValues")
                    t |> success(fixed.constValues,
                                 "fixed_array<uint>(1u, 2u, 3u) is not flagged constValues")
                    t |> success(!mixed.constValues, "array<int>(1, g_x, 3) is flagged constValues over a variable element")
                }
            }
        }
    }
}

let FAILED_RETURN_SOURCE = ("options gen2\n"
    + "def make_one : int[1] \{\n    var a : int[1]\n    return a\n\}\n"
    + "[export]\ndef f => fixed_array(make_one()).[4]\n")

[test(description = "simulating a program whose return holds a failed subexpression does not crash")]
def test_return_of_a_failed_subexpression(t : T?) {
    using() $(var cop : CodeOfPolicies) {
        cop.threadlock_context = true
        compile("failed_return", FAILED_RETURN_SOURCE, cop) $(ok; program; issues) {
            t |> success(ok, "the program compiles: {issues}")
            return if (!ok)
            simulate(program) $(sok; _ctx; _serrors) {
                t |> success(!sok)
                let codes <- [for (err in program.errors); err.cerr]
                t |> equal(codes, [CompilationError.exceeds_array])
            }
        }
    }
}

def private null_byte_fixture_path() : string =>
    "{get_das_root()}/tests/ast/_string_null_byte_fixture.das"

def private returned_constant(fn : FunctionPtr) : ExprConstString? {
    let blk = fn.body ?as ExprBlock
    return null if (blk == null || empty(blk.list))
    let ret = blk.list[0] ?as ExprReturn
    return ret != null ? (ret.subexpr ?as ExprConstString) : null
}

def private check_escape(t : T?; mod : Module?) {
    var blob = returned_constant(find_unique_function(mod, "returned_a"))
    t |> success(blob != null, "returned_a does not return a string constant")
    return if (blob == null)
    t |> equal(escape(blob.value), "x\\u0000a")
    t |> equal(describe(blob), "\"x\\x00a\"")
    var expr : ExpressionPtr = blob
    let printed = build_string() $(var writer) {
        printExpr(expr, unsafe(addr(writer)))
    }
    t |> equal(printed, "\"x\\u0000a\"")
}

def private check_hash(t : T?; mod : Module?) {
    let hash_of = @(name : string) : uint64 => find_unique_function(mod, name).hash
    t |> success(hash_of("returned_a") != hash_of("returned_b"), "returned blobs hash alike")
    t |> success(hash_of("compared_a") != hash_of("compared_b"), "compared blobs hash alike")
    t |> equal(hash_of("plain_a"), hash_of("plain_b"))
}

[test(description = "string constants keep embedded null bytes through escaping and hashing")]
def test_string_null_byte(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file(null_byte_fixture_path(), access, unsafe(addr(mg)),
                         cop) $(ok; program; issues) {
                t |> success(ok, "the fixture did not compile: {issues}")
                return if (!ok || program == null)
                let mod = get_this_module(program)
                check_escape(t, mod)
                simulate(program) $(sok; _context; serrors) {
                    t |> success(sok, "the fixture did not simulate: {serrors}")
                    return if (!sok)
                    check_hash(t, mod)
                }
            }
        }
    }
}

[test(description = "an lpipe() stranded after a return names the return and the assign-first spelling in its error text")]
def test_lpipe_after_return_message(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            let fixture = "{get_das_root()}/tests/ast/_fixture_lpipe_after_return.das"
            compile_file(fixture, access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                let text = "{issues}"
                t |> success(!ok, "the program is refused")
                t |> success(find(text, "can't pipe into a return") >= 0,
                    "the return is named: {text}")
                t |> success(find(text, "assign the call's result first") >= 0,
                    "the assign-first spelling is named: {text}")
            }
        }
    }
}
