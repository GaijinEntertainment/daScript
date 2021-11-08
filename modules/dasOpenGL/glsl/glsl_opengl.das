options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module glsl_opengl shared public

require daslib/ast_boost
require daslib/strings_boost

require glsl/glsl_internal
require glsl/glsl_common public

class GlslOpenGLShader : GlslShader
    def override generate_bind_uniform_dummy ( var func:FunctionPtr )
        generate_bind_uniform_dummy(func)
    def override generate_bind_uniform(fnMain:FunctionPtr; var fn:FunctionPtr)
        generate_bind_uniform(fnMain, fn)

[function_macro(name="vertex_program")]
class GlslVertexProgram : GlslOpenGLShader
    override shaderType = ShaderType vertex

[function_macro(name="fragment_program")]
class GlslPixelProgram : GlslOpenGLShader
    override shaderType = ShaderType fragment

[function_macro(name="compute_program")]
class GlslComputeProgram : GlslOpenGLShader
    override shaderType = ShaderType compute

[macro_function]
def private generate_bind_uniform_dummy(fnMain:FunctionPtr)
    var fn <- new [[Function() at = fnMain.at, atDecl = fnMain.at, name := bind_uniform_function_name(fnMain)]]
    fn.flags |= FunctionFlags generated
    fn.flags |= FunctionFlags exports   // note: this is temporary, until we are done with dependency collecting etc
    fn.result <- new [[TypeDecl() baseType=Type tVoid, at=fnMain.at]]
    fn.arguments |> emplace_new() <| new [[Variable() at=fnMain.at,
        name := "__program",
        _type <- new [[TypeDecl() at=fnMain.at, baseType=Type tUInt]]
    ]]
    fn.body <- new [[ExprBlock() at=fnMain.at]]
    if !(compiling_module() |> add_function(fn))
        panic("can't setup")
    return true

[macro_function]
def private generate_bind_uniform(fnMain:FunctionPtr; var fn:FunctionPtr)
    var blk <- new [[ExprBlock() at=fn.at]]
    collect_dependencies(fnMain) <| $ ( vfun, vvar )
        for vv in vvar
            if find_arg("uniform", vv.annotation) is tBool
                if is_glsl_structure(vv,"sampler2D")
                    // bind_sampler_2d(binding, varName.texture2D)
                    var binding = find_arg("stage", vv.annotation) ?as tInt ?? 0
                    var vtex <- new [[ExprField() at=vv.at,
                        value <- new [[ExprVar() at=vv.at, name:=vv.name]],
                        name := "texture2D"
                    ]]
                    var cll_bind <- new [[ExprCall() at=vv.at, name:="bind_sampler_2d"]]
                    cll_bind.arguments |> emplace_new() <| new [[ExprConstInt() at=vv.at, value=binding]]
                    cll_bind.arguments |> emplace(vtex)
                    blk.list |>emplace(cll_bind)
                    // glUniformAny(gl_GetUniformLocation("varName"),binding)
                    var cll_gul <- new [[ExprCall() at=vv.at, name:="glGetUniformLocation"]]
                    cll_gul.arguments |> emplace_new() <| new [[ExprVar() at=vv.at, name:="__program"]]
                    cll_gul.arguments |> emplace_new() <| new [[ExprConstString() at=vv.at, value:=vv.name]]
                    var cll_uni <- new [[ExprCall() at=vv.at, name:="glUniformAny"]]
                    cll_uni.arguments |> emplace(cll_gul)
                    cll_uni.arguments |> emplace_new() <| new [[ExprConstInt() at=vv.at, value=binding]]
                    blk.list |>emplace(cll_uni)
                elif is_glsl_structure(vv,"image2D")
                    // bind_sampler_2d(binding, varName.texture2D, format, access)
                    var binding = find_arg("stage", vv.annotation) ?as tInt ?? 0
                    var vtex <- new [[ExprField() at=vv.at,
                        value <- new [[ExprVar() at=vv.at, name:=vv.name]],
                        name := "texture2D"
                    ]]
                    var format = find_arg("format", vv.annotation) ?as tString ?? "rgba8"
                    format = "GL_{to_upper(format)}"
                    var access = "GL_READ_WRITE"
                    if find_arg("writeonly", vv.annotation) is tBool
                        access = "GL_WRITE_ONLY"
                    if find_arg("readonly", vv.annotation) is tBool
                        access = "GL_READ_ONLY"
                    var cll_uni <- new [[ExprCall() at=vv.at, name:="bind_image_2d"]]
                    cll_uni.arguments |> emplace_new() <| new [[ExprConstInt() at=vv.at, value=binding]]
                    cll_uni.arguments |> emplace(vtex)
                    cll_uni.arguments |> emplace_new() <| new [[ExprVar() at=vv.at, name:=format]]
                    cll_uni.arguments |> emplace_new() <| new [[ExprVar() at=vv.at, name:=access]]
                    blk.list |>emplace(cll_uni)
                else
                    // glUniformAny(gl_GetUniformLocation("varName"),varName)
                    var cll_gul <- new [[ExprCall() at=vv.at, name:="glGetUniformLocation"]]
                    cll_gul.arguments |> emplace_new() <| new [[ExprVar() at=vv.at, name:="__program"]]
                    cll_gul.arguments |> emplace_new() <| new [[ExprConstString() at=vv.at, value:=vv.name]]
                    var cll_uni <- new [[ExprCall() at=vv.at, name:="glUniformAny"]]
                    cll_uni.arguments |> emplace(cll_gul)
                    cll_uni.arguments |> emplace_new() <| new [[ExprVar() at=vv.at, name:=vv.name]]
                    blk.list |>emplace(cll_uni)
    fn.body := blk
    if DEBUG_SHADER_BINDINGS
        print("{describe(fn)}\n")
