options gen2
options indenting = 4

// Shader function-macro annotations (mirrors dasGlsl's GlslShader / [compute_program]). The emitted
// SPIR-V words are captured into a module global `{name} : array<uint>`. The global is DECLARED in
// `apply` (so consumer code referencing it resolves during the first inference pass — declaring it
// later, in patch/fixup, deadlocks: the consumer fails inference, which gates patchAnnotations off).
// Its init is FILLED in `patch` with astChanged=true: an array<uint> literal lowers to a
// `to_array_move(...)` CALL, and a call set into a global init in `fixup` (no re-infer) is never
// resolved and crashes the alias pass — patch's astChanged triggers the re-infer that resolves it.
// Builtins come in via spirv_builtins. Host: feed `{name}` straight to vkCreateShaderModule.

module spirv_shader shared public

require spirv_emit
require spirv_builtins public
require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require strings

// `array<uint>(w0, w1, ...)` — the gen2 array literal is ExprMakeArray(makeType=uint, gen2=true)
// wrapped in to_array_move (ds2_parser.ypp).
[macro_function]
def private build_uint_array_literal(words : array<uint>; at : LineInfo) : ExpressionPtr {
    var mkArr = new ExprMakeArray(at = at, makeType = new TypeDecl(at = at, baseType = Type.tUInt))
    mkArr.gen2 = true
    for (w in words) {
        mkArr.values |> emplace_new() <| new ExprConstUInt(at = at, value = w)
    }
    var toMove = new ExprCall(at = at, name := "to_array_move")
    toMove.arguments |> emplace(mkArr)
    return toMove
}

class SpirvShader : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        func.flags.exports = true   // temporary: keeps the function alive through dependency collection
        let argName = args |> find_arg("name") ?as tString ?? "{func.name}`spirv"
        var atype = new TypeDecl(at = func.at, baseType = Type.tArray)
        atype.firstType = new TypeDecl(at = func.at, baseType = Type.tUInt)
        if (!add_global_var(compiling_module(), argName, atype, func.at, false)) {
            errors := "can't add global variable {argName}"
            return false
        }
        return true
    }
    def override patch(var func : FunctionPtr; var group : ModuleGroup; args, progArgs : AnnotationArgumentList; var errors : das_string; var astChanged : bool&) : bool {
        let argName = args |> find_arg("name") ?as tString ?? "{func.name}`spirv"
        var local_size : int3
        local_size.x = args |> find_arg("local_size_x") ?as tInt ?? 1
        local_size.y = args |> find_arg("local_size_y") ?as tInt ?? 1
        local_size.z = args |> find_arg("local_size_z") ?as tInt ?? 1
        var err : array<string>
        var glob <- find_variable(compiling_module(), argName)
        if (glob != null && glob.init == null && glob.name == argName) {
            var words <- generate_spirv(func, err, local_size)
            if (err |> empty()) {
                glob.init = build_uint_array_literal(words, func.at)
                astChanged = true
            }
            delete words
        }
        glob := null
        if (!(err |> empty())) {
            errors := join(err, "\n")
            return false
        }
        return true
    }
}

[function_macro(name="compute_shader")]
class SpirvComputeShader : SpirvShader {
}
