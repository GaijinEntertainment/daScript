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

// The [compute_shader] macro stores the emitted blob as `name : array<uint>` whose initializer is
// to_array_move([uint literals]) -- read the words back off the compiled program's AST.
def shader_words(program : ProgramPtr; name : string) : array<uint> {
    var words : array<uint>
    for_each_global(get_this_module(program)) $(gv) {
        if (gv.name == name && gv.init != null) {
            var mk = gv.init ?as ExprMakeArray
            if (mk == null) {
                let call = gv.init ?as ExprCall
                if (call != null && length(call.arguments) == 1) {
                    mk = call.arguments[0] ?as ExprMakeArray
                }
            }
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
                    if (!ok || program == null) {
                        return
                    }
                    // pin the folded VALUES, not just compiles-clean: a get_const_expr answering
                    // the wrong number would still compile. The words are the emitted artifact.
                    var words <- shader_words(program, "fold_spv")
                    t |> success(length(words) > 5, "no emitted words on fold_spv")
                    // local_size_x = FOLD_LOCAL = 3 * 16: ExecutionMode LocalSize(17) 48 1 1
                    t |> success(op_has_operand_pair(words, SpvOp.ExecutionMode, 17u, 48u),
                        "LocalSize x is not 48")
                    // @binding = FOLD_SLOT = 3 + 4: Decorate ... Binding(33) 7
                    t |> success(op_has_operand_pair(words, SpvOp.Decorate, 33u, 7u),
                        "Binding is not 7")
                    // @set = FOLD_SET = 3 - 2: Decorate ... DescriptorSet(34) 1
                    t |> success(op_has_operand_pair(words, SpvOp.Decorate, 34u, 1u),
                        "DescriptorSet is not 1")
                    // FOLD_ALIAS = FOLD_SCALE = 2u * 3u reaches the body as OpConstant 6
                    t |> success(op_has_operand_at(words, SpvOp.Constant, 2, 6u),
                        "no OpConstant 6 from FOLD_ALIAS")
                    delete words
                }
            }
        }
    }
}
