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

def private hash_of(program : ProgramPtr; mod : Module?; name : string) : uint64 =>
    function_hash(program, find_unique_function(mod, name))

def private check_hash(t : T?; program : ProgramPtr; mod : Module?) {
    let returned = hash_of(program, mod, "returned_a") != hash_of(program, mod, "returned_b")
    t |> success(returned, "returned blobs hash alike")
    let compared = hash_of(program, mod, "compared_a") != hash_of(program, mod, "compared_b")
    t |> success(compared, "compared blobs hash alike")
    t |> equal(hash_of(program, mod, "plain_a"), hash_of(program, mod, "plain_b"))
}

[test(description = "string constants keep embedded null bytes through escaping and hashing")]
def test_string_null_byte(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.keep_symbol_tables = true
            compile_file(null_byte_fixture_path(), access, unsafe(addr(mg)),
                         cop) $(ok; program; issues) {
                t |> success(ok, "the fixture did not compile: {issues}")
                return if (!ok || program == null)
                let mod = get_this_module(program)
                check_escape(t, mod)
                simulate(program) $(sok; _context; serrors) {
                    t |> success(sok, "the fixture did not simulate: {serrors}")
                    return if (!sok)
                    check_hash(t, program, mod)
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

[test(description = "a tuple and a variant over an uninstantiated struct template with a typedecl field compile")]
def test_holder_over_uninstantiated_template_compiles(t : T?) {
    for (holder in ["tuple", "variant"]) {
        let path = "{get_das_root()}/tests/ast/_virtual_template_holder.das"
        var inscope access <- make_file_access("")
        let source = ("options gen2\n{holder} H \{\n    a : S\n\}\n"
            + "struct template S \{\n    f : typedecl(1)\n\}\n")
        set_file_source(access, path, source)
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                compile_file(path, access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                    t |> success(ok, "{holder}: {issues}")
                }
            }
        }
    }
}

//! need_rtti of a program compiled from `body` with the symbol
//! tables kept
def private need_rtti_of(t : T?; body : string; force : bool = false) : bool {
    let path = "{get_das_root()}/tests/ast/_need_rtti_probe.das"
    var inscope access <- make_file_access("")
    set_file_source(access, path, "options gen2\nrequire daslib/rtti\n{body}")
    var answer = false
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.keep_symbol_tables = true
            cop.rtti = force
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the probe compiles: {issues}")
                return if (!ok)
                answer = need_rtti(get_ptr(program))
            }
        }
    }
    return answer
}

[test(description = "a program needs runtime RTTI exactly when a used function or global initializer reaches a need_rtti builtin, or a policy forces it")]
def test_need_rtti(t : T?) {
    let plain = "[export]\ndef main \{\n    print(\"hi\\n\")\n\}\n"
    t |> equal(need_rtti_of(t, plain), false, "a plain program")
    t |> equal(need_rtti_of(t, plain, true), true, "CodeOfPolicies.rtti")
    t |> equal(need_rtti_of(t, "options rtti\n" + plain), true, "options rtti")
    t |> equal(need_rtti_of(t, "[export]\ndef main \{\n    unsafe(heap_collect())\n\}\n"),
        false, "heap_collect: the heap kind decides its global infos")
    t |> equal(need_rtti_of(t, "[export]\ndef main \{\n    stackwalk()\n\}\n"), true, "stackwalk")
    t |> equal(need_rtti_of(t, "def helper \{\n    print(\"\{get_stackwalk()\}\")\n\}\n"
            + "[export]\ndef main \{\n    helper()\n\}\n"),
        true, "get_stackwalk in a callee")
    t |> equal(need_rtti_of(t, "def unused \{\n    stackwalk()\n\}\n" + plain),
        false, "stackwalk in an unused function")
    t |> equal(need_rtti_of(t, "[export]\ndef main \{\n"
            + "    let info & = get_function_info(this_context(), 0)\n"
            + "    print(\"\{info.name\}\\n\")\n\}\n"),
        true, "get_function_info")
    t |> equal(need_rtti_of(t, "let g = get_stackwalk()\n"
            + "[export]\ndef main \{\n    print(g)\n\}\n"),
        true, "a global initializer")
    t |> equal(need_rtti_of(t, "[finalize]\ndef bye \{\n    print(\"bye\\n\")\n\}\n" + plain),
        false, "a [finalize] function runs from the context's shutdown list")
    t |> equal(need_rtti_of(t, "let shared g_s <- [1, 2, 3]\n"
            + "[export]\ndef main \{\n    print(\"\{g_s\}\\n\")\n\}\n"),
        true, "a shared global")
}

[test(description = "set_policy sets a CodeOfPolicies field by its C++ name from name=value text, a bare name turning a bool on, and answers with the reason for a name that is no policy, a value its field does not take or that is out of its range and a field text cannot set, leaving the field as it was; set_policies stops at the first item refused")]
def test_set_policy(t : T?) {
    using() $(var cop : CodeOfPolicies) {
        t |> equal(set_policy(cop, "force_inscope_pod"), "")
        t |> success(cop.force_inscope_pod, "a bare name turns the bool on")
        t |> equal(set_policy(cop, " force_inscope_pod = false "), "")
        t |> success(!cop.force_inscope_pod, "spaces around the name and the value are dropped")
        t |> equal(set_policy(cop, "stack=0x10000"), "")
        t |> equal(cop.stack, 65536u)
        t |> equal(set_policy(cop, "max_infer_passes=7"), "")
        t |> equal(cop.max_infer_passes, 7)
        t |> equal(set_policy(cop, "max_heap_allocated=5000000000"), "")
        t |> equal(cop.max_heap_allocated, 5000000000ul)
        t |> equal(set_policy(cop, "jit_output_path=out/lib"), "")
        t |> equal(string(cop.jit_output_path), "out/lib")
        t |> equal(set_policy(cop, "no_such_policy"), "unknown policy 'no_such_policy'")
        t |> equal(set_policy(cop, "abi_stamp=1"), "unknown policy 'abi_stamp'")
        t |> equal(set_policy(cop, "stack=-1"),
            "policy 'stack' expects a 32-bit unsigned integer, got '-1'")
        t |> equal(set_policy(cop, "max_infer_passes=many"),
            "policy 'max_infer_passes' expects a 32-bit integer, got 'many'")
        t |> equal(set_policy(cop, "max_heap_allocated=1.5"),
            "policy 'max_heap_allocated' expects an unsigned integer, got '1.5'")
        t |> equal(set_policy(cop, "force_inscope_pod=maybe"),
            "policy 'force_inscope_pod' expects true or false, got 'maybe'")
        t |> equal(set_policy(cop, "dll_search_paths=x"),
            "policy 'dll_search_paths' can't be set from text")
        t |> equal(cop.stack, 65536u, "a refused value leaves the field as it was")
        t |> equal(set_policy(cop, "max_heap_allocated=12x"),
            "policy 'max_heap_allocated' expects an unsigned integer, got '12x'")
        t |> equal(cop.max_heap_allocated, 5000000000ul, "... a 64-bit one too")
        t |> equal(set_policy(cop, "max_heap_allocated=-1"),
            "policy 'max_heap_allocated' expects an unsigned integer, got '-1'")
        t |> equal(set_policy(cop, "max_infer_passes=4294967296"),
            "policy 'max_infer_passes' expects a 32-bit integer, got '4294967296'")
        t |> equal(set_policy(cop, "stack=4294967296"),
            "policy 'stack' expects a 32-bit unsigned integer, got '4294967296'")
        t |> equal(set_policy(cop, "stack="),
            "policy 'stack' expects a 32-bit unsigned integer, got ''")
        t |> equal(set_policy(cop, "force_inscope_pod=1"), "")
        t |> success(cop.force_inscope_pod, "1 is true")
        t |> equal(set_policy(cop, "force_inscope_pod=0"), "")
        t |> success(!cop.force_inscope_pod, "0 is false")
        var inscope items <- ["stack=4096", "no_such_policy", "max_infer_passes=9"]
        t |> equal(set_policies(cop, items), "unknown policy 'no_such_policy'")
        t |> equal(cop.stack, 4096u, "set_policies sets the items before the refused one")
        t |> equal(cop.max_infer_passes, 7, "... and none after it")
    }
}

[test(description = "log_inscope_pod names each array literal the pass built on the stack, each local and move it collected, and each site it left alone with every reason - the parameter, the elements, the callee's result, a generator, unsafe, a type that is not POD - each line once", timeout = 1200)]
def test_log_inscope_pod(t : T?) {
    let site = "in function '::main'"
    let refused = "warning: stack array literal refused for"
    let failed = "warning: POD optimization failed for"
    let lines <- [
        "Stack array literal applied to argument 'xs' of 'total' {site}",
        "{refused} argument 'xs' of 'grow' {site}\n\tthe parameter is not constant",
        "{refused} argument 'rows' of 'total_of_all' {site}\n\tthe elements own heap",
        "\tthe result iterator<int&> may hold a reference to the array",
        "\tfunction is generator",
        "{failed} 'kept' in function '::beside_unsafe'\n\tfunction has unsafe",
        "{failed} 'moved <- kept' in function '::beside_unsafe'\n\tfunction has unsafe",
        "{failed} 'steps' in function '::holds_lambdas'\n\tlambda<int> is not POD",
        "skipping POD optimization for single_return_via_move variable 'made'"
    ]
    using() $(var cop : CodeOfPolicies) {
        t |> equal(set_policy(cop, "force_inscope_pod"), "")
        t |> equal(set_policy(cop, "log_inscope_pod"), "")
        var inscope access <- make_file_access("")
        let fixture = "{get_das_root()}/tests/ast/_fixture_pod_log.das"
        using() $(var mg : ModuleGroup) {
            compile_file(fixture, access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                t |> success(ok, "the fixture did not compile: {issues}")
                let log = string(issues)
                for (line in lines) {
                    let first = find(log, line)
                    t |> success(first >= 0, "no line '{line}' in:\n{log}")
                    t |> equal(find(log, line, first + 1), -1, "'{line}' is logged once:\n{log}")
                }
            }
        }
    }
}
