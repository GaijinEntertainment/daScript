options gen2
options indenting = 4
options no_aot     // invokes the compiler at runtime (compile_file) — not AOT-linkable

// Lint compiles with folding off, so a const global keeps the initializer as written. The emitter
// still needs its value -- for a constant `let` it lowers to an OpConstant, and for a named layout
// index it decorates with. Both must ask the compiler what such an expression is worth instead of
// taking only the shapes that are already constant, or every shader-bearing module is unlintable.

require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost
require spirv/spirv_grammar
require spirv/spirv_dis

def fixture_path() : string {
    return "{get_das_root()}/tests/spirv/_lint_fold_fixture.das"
}

// The [compute_shader] macro stores the emitted blob as `name : array<uint>` initialized with
// to_array_move([uint literals]) -- unwrap the call and read the words off the array literal.
def shader_words(program : ProgramPtr; name : string) : array<uint> {
    var words : array<uint>
    for_each_global(get_this_module(program)) $(gv) {
        if (gv.name == name && gv.init != null) {
            let call = gv.init ?as ExprCall
            if (call != null && length(call.arguments) == 1) {
                let mk = call.arguments[0] ?as ExprMakeArray
                if (mk != null) {
                    for (v in mk.values) {
                        let cu = v ?as ExprConstUInt
                        if (cu != null) {
                            words |> push(cu.value)
                        }
                    }
                }
            }
        }
    }
    return <- words
}

[test]
def test_shader_compiles_with_folding_off(t : T?) {
    t |> run("a shader module compiles under the lint policies") <| @(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.export_all = true
                cop.lint_check = true
                cop.no_optimizations = true
                cop.no_infer_time_folding = true
                compile_file(fixture_path(), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                    t |> success(ok, "the fixture did not compile: {issues}")
                    t |> success(program != null, "no program")
                    if (!ok || program == null) return
                    // pin the folded VALUES, not just compiles-clean: a get_const_expr answering
                    // the wrong number would still compile.
                    var words <- shader_words(program, "fold_spv")
                    t |> success(length(words) > 5, "fold_spv has no instructions past the 5-word header")
                    t |> success(op_has_operand_pair(words, SpvOp.ExecutionMode, uint(SpvExecutionMode.LocalSize), 48u),
                        "LocalSize x is not 48 (FOLD_LOCAL = 3 * 16)")
                    t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 7u),
                        "Binding is not 7 (FOLD_SLOT = 3 + 4)")
                    t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet), 1u),
                        "DescriptorSet is not 1 (FOLD_SET = 3 - 2)")
                    t |> success(op_has_operand_at(words, SpvOp.Constant, 2, 6u),
                        "no OpConstant 6 (FOLD_ALIAS = FOLD_SCALE = 2u * 3u)")
                    delete words
                }
            }
        }
    }
}
