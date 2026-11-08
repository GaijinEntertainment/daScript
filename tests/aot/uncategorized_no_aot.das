options gen2
options no_aot

require daslib/aot_cpp
require daslib/ast
require strings
require dastest/testing_boost public

var g_anchor = 17

def private emit_const_ptr(pvalue : void?) : string {
    var emitted = ""
    ast_gc_guard() {
        emitted = build_string() $(writer) {
            var gen = new CppAot(ss = unsafe(addr(writer)))
            var cptr = new ExprConstPtr(value = pvalue)
            gen->visitExprConstPtr(cptr)
            unsafe {
                delete gen.collector
                delete gen
            }
        }
    }
    return emitted
}

def private paren_balance(text : string) : int {
    var balance = 0
    peek_data(text) $(bytes) {
        for (byte in bytes) {
            let ch = int(byte)
            if (ch == '(') {
                balance ++
            } elif (ch == ')') {
                balance --
            }
        }
    }
    return balance
}

[test(description = "The emitter spells a null pointer constant as nullptr and a non-null one as a balanced ((void *) 0x...) cast")]
def test_const_ptr_emit(t : T?) {
    t |> run("null pointer constant emits nullptr") @(t : T?) {
        t |> equal(emit_const_ptr(null), "nullptr")
    }
    t |> run("non-null pointer constant is a balanced cast expression") @(t : T?) {
        let emitted = emit_const_ptr(unsafe(addr<void?>(g_anchor)))
        t |> success(starts_with(emitted, "((void *) 0x"), "got {emitted}")
        t |> equal(paren_balance(emitted), 0, "unbalanced parentheses in {emitted}")
    }
}

[test(description = "Emitting a module with a template member produces C++ with no unresolved auto or alias type")]
def test_template_member_emit(t : T?) {
    var cpp = ""
    using() $(var cop : CodeOfPolicies) {
        cop.aot_macros = true
        cop.aot_module = true
        cpp = aot("{get_das_root()}/tests/aot/_template_member_fixture.das", false, false, cop)
    }
    t |> success(!empty(cpp), "emitter produced output")
    t |> success(find(cpp, "DAS_COMMENT(auto)") < 0, "no unresolved auto type in output")
    t |> success(find(cpp, "DAS_COMMENT(alias)") < 0, "no unresolved alias type in output")
}
