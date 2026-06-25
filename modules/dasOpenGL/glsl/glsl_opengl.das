options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

module glsl_opengl shared public

require daslib/ast
require daslib/ast_boost
require daslib/strings_boost
require daslib/shader_block_layout

require glsl/glsl_internal
require glsl/glsl_common public

class GlslOpenGLShader : GlslShader {
    // Pack struct @uniform / @push_constant blocks as std140 UBOs by default (the GL/WebGL2 portable
    // floor, byte-for-byte with the Vulkan rail). Loose by-name binding stays the fallback for the
    // scalar / vector / matrix / sampler uniforms that aren't interface blocks.
    override pack_std140_uniforms = true
    def override generate_bind_uniform_dummy(var func : FunctionPtr) {
        glsl_opengl::generate_bind_uniform_dummy(func)
    }
    def override generate_bind_uniform(fnMain : FunctionPtr; var fn : FunctionPtr) {
        glsl_opengl::generate_bind_uniform(fnMain, fn, pack_std140_uniforms)
    }
}

[function_macro(name="vertex_program")]
class GlslVertexProgram : GlslOpenGLShader {
    override shaderType = ShaderType.vertex
    override output_shader_lines = false
}

[function_macro(name="fragment_program")]
class GlslPixelProgram : GlslOpenGLShader {
    override shaderType = ShaderType.fragment
    override output_shader_lines = false
}

[function_macro(name="compute_program")]
class GlslComputeProgram : GlslOpenGLShader {
    override shaderType = ShaderType.compute
    override output_shader_lines = false
}

[macro_function]
def private generate_bind_uniform_dummy(fnMain : FunctionPtr) {
    var fn = new Function(at = fnMain.at, atDecl = fnMain.at, name := bind_uniform_function_name(fnMain))
    fn.flags.generated = true
    fn.flags.exports = true   // note: this is temporary, until we are done with dependency collecting etc
    fn.result = new TypeDecl(baseType = Type.tVoid, at = fnMain.at)
    fn.arguments |> emplace_new() <| new Variable(at = fnMain.at,
        name := "_opengl_program",
        _type <- new TypeDecl(at = fnMain.at, baseType = Type.tUInt)
    )
    fn.body = new ExprBlock(at = fnMain.at)
    if (!(compiling_module() |> add_function(fn))) {
        panic("can't setup")
    }
    return true
}

// Emit the host-side bind for a combined image sampler: bind <var>.<fieldName> (the GL texture
// handle) to texture unit `binding` via bindFn, then point the sampler uniform at that unit. Shared
// by the samplerCube / sampler2DArray / sampler2DShadow paths (sampler2D / sampler3D predate it and
// stay inline). bindFn must resolve in the consumer's require chain (opengl_boost), like glUniformAny.
[macro_function]
def private emit_combined_sampler_bind(var blk : ExprBlock?; vv : VariablePtr; fieldName, bindFn : string) {
    let binding = vv.annotation |> find_arg("stage") ?as tInt ?? 0
    var vtex = new ExprField(at = vv.at,
        value <- new ExprVar(at = vv.at, name := vv.name),
        name := fieldName
    )
    var cll_bind = new ExprCall(at = vv.at, name := bindFn)
    cll_bind.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = binding)
    cll_bind.arguments |> emplace(vtex)
    blk.list |> emplace(cll_bind)
    var cll_gul = new ExprCall(at = vv.at, name := "glGetUniformLocation")
    cll_gul.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "_opengl_program")
    cll_gul.arguments |> emplace_new() <| new ExprConstString(at = vv.at, value := vv.name)
    var cll_uni = new ExprCall(at = vv.at, name := "glUniformAny")
    cll_uni.arguments |> emplace(cll_gul)
    cll_uni.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = binding)
    blk.list |> emplace(cll_uni)
}

// Loose-bind every leaf field of a struct uniform / push-constant. GL exposes a `uniform Block b;`
// as one by-name location per leaf (b.field, b.nested.field, ...), so we walk the struct and emit a
// glUniformAny(glGetUniformLocation(prog, "<flattened.name>"), <field access>) per terminal field,
// recursing into nested structs and flattening fixed arrays of structs per element (name[i].field).
// Arrays of scalar/vector fields fall through to glUniformAny's whole-array overload (one upload from
// the base location); a field glUniformAny has no overload for (e.g. a dynamic array<Struct>) stays
// fail-closed at compile time. The accessExpr is the parent reference (b, then b.nested, ...).
[macro_function]
def private emit_loose_struct_bind(var blk : ExprBlock?; st : Structure?; namePrefix : string; accessExpr : ExpressionPtr) {
    for (f in st.fields) {
        let fieldName = "{namePrefix}.{f.name}"
        var fieldAccess = new ExprField(at = accessExpr.at,
            value <- clone_expression(accessExpr),
            name := f.name
        )
        if (f._type.baseType == Type.tStructure && f._type.structType != null) {
            emit_loose_struct_bind(blk, f._type.structType, fieldName, fieldAccess)
        } elif (f._type.baseType == Type.tFixedArray && f._type.firstType != null &&
                f._type.firstType.baseType == Type.tStructure && f._type.firstType.structType != null) {
            // array of struct: GL exposes name[0].field, name[1].field, ... as separate locations, so
            // flatten per element index. (Arrays of scalar/vector fall through to glUniformAny's array
            // overload below, which uploads the whole array from the base location in one call.)
            for (i in range(f._type.fixedDim)) {
                var elemAccess = new ExprAt(at = accessExpr.at,
                    subexpr <- clone_expression(fieldAccess),
                    index <- new ExprConstInt(at = accessExpr.at, value = i)
                )
                emit_loose_struct_bind(blk, f._type.firstType.structType, "{fieldName}[{i}]", elemAccess)
            }
        } else {
            var cll_gul = new ExprCall(at = accessExpr.at, name := "glGetUniformLocation")
            cll_gul.arguments |> emplace_new() <| new ExprVar(at = accessExpr.at, name := "_opengl_program")
            cll_gul.arguments |> emplace_new() <| new ExprConstString(at = accessExpr.at, value := fieldName)
            var cll_uni = new ExprCall(at = accessExpr.at, name := "glUniformAny")
            cll_uni.arguments |> emplace(cll_gul)
            cll_uni.arguments |> emplace(fieldAccess)
            blk.list |> emplace(cll_uni)
        }
    }
}

// Emit the std140 UBO bind for one struct @uniform / @push_constant block. The caller has already
// confirmed std140_block_is_simple, so every field packs flat. Computes the shared std140 layout
// (compute_block_layout — the same offsets the SPIR-V/Vulkan rails use), then emits a flat call
// sequence into the bind function: std140_reset(prog, block, paddedSize); one std140_set(prog, block,
// offset, vv.field) per field; std140_flush(prog, block, bindingPoint).
[macro_function]
def private emit_std140_ubo_bind(var blk : ExprBlock?; vv : VariablePtr; bindingPoint : int) {
    let st = vv._type.structType
    // Block name = <instance>_ubo — must match glsl_internal's outputStd140UboBlock (distinct from the
    // struct type name, which is still emitted as a standalone GLSL struct).
    let blockName = "{vv.name}_ubo"
    var fields : array<BlockField>
    var errs : array<string>
    let layout = compute_block_layout(blockName, st, fields, errs)
    if (!layout.ok) {
        // Defensive: std140_block_is_simple already rejected non-layoutable blocks to the loose path.
        let msg = empty(errs) ? "(no diagnostic)" : errs[0]
        macro_error(compiling_program(), vv.at, "[opengl std140]: @uniform '{vv.name}' layout failed: {msg}")
        delete fields
        delete errs
        return
    }
    let padded = round_up(layout.size, 16)
    var cll_reset = new ExprCall(at = vv.at, name := "std140_reset")
    cll_reset.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "_opengl_program")
    cll_reset.arguments |> emplace_new() <| new ExprConstString(at = vv.at, value := blockName)
    cll_reset.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = padded)
    blk.list |> emplace(cll_reset)
    for (f in fields) {
        var cll_set = new ExprCall(at = vv.at, name := "std140_set")
        cll_set.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "_opengl_program")
        cll_set.arguments |> emplace_new() <| new ExprConstString(at = vv.at, value := blockName)
        cll_set.arguments |> emplace_new() <| new ExprConstUInt(at = vv.at, value = uint(f.offset))
        var fld_access = new ExprField(at = vv.at,
            value <- new ExprVar(at = vv.at, name := vv.name),
            name := f.name
        )
        cll_set.arguments |> emplace(fld_access)
        blk.list |> emplace(cll_set)
    }
    var cll_flush = new ExprCall(at = vv.at, name := "std140_flush")
    cll_flush.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "_opengl_program")
    cll_flush.arguments |> emplace_new() <| new ExprConstString(at = vv.at, value := blockName)
    cll_flush.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = bindingPoint)
    blk.list |> emplace(cll_flush)
    delete fields
    delete errs
}

[macro_function]
def private generate_bind_uniform(fnMain : FunctionPtr; var fn : FunctionPtr; pack_std140 : bool) {
    var blk = new ExprBlock(at = fn.at)
    var ubo_globals : array<VariablePtr>   // struct @uniform/@push_constant blocks to pack as std140 UBOs
    var ubo_seen : table<string>           // dedup: a block reachable via >1 function is collected once
    collect_dependencies(fnMain) $(_vfun, vvar) {
        for (vv in vvar) {
            // @push_constant has no GL equivalent -> bind it like a @uniform (loose). Phase 2 / D4.
            let isUniform = ((vv.annotation |> find_arg("uniform") is tBool) ||
                (vv.annotation |> find_arg("push_constant") is tBool))
            if (isUniform) {
                if (is_glsl_structure(vv, "sampler2D")) {
                    // bind_sampler_2d(binding, varName.texture2D)
                    let binding = vv.annotation |> find_arg("stage") ?as tInt ?? 0
                    var vtex = new ExprField(at = vv.at,
                        value <- new ExprVar(at = vv.at, name := vv.name),
                        name := "texture2D"
                    )
                    var cll_bind = new ExprCall(at = vv.at, name := "bind_sampler_2d")
                    cll_bind.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = binding)
                    cll_bind.arguments |> emplace(vtex)
                    blk.list |> emplace(cll_bind)
                    // glUniformAny(gl_GetUniformLocation("varName"),binding)
                    var cll_gul = new ExprCall(at = vv.at, name := "glGetUniformLocation")
                    cll_gul.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "_opengl_program")
                    cll_gul.arguments |> emplace_new() <| new ExprConstString(at = vv.at, value := vv.name)
                    var cll_uni = new ExprCall(at = vv.at, name := "glUniformAny")
                    cll_uni.arguments |> emplace(cll_gul)
                    cll_uni.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = binding)
                    blk.list |> emplace(cll_uni)
                } elif (is_glsl_structure(vv, "sampler3D")) {
                    // bind_sampler_2d(binding, varName.texture2D)
                    let binding = vv.annotation |> find_arg("stage") ?as tInt ?? 0
                    var vtex = new ExprField(at = vv.at,
                        value <- new ExprVar(at = vv.at, name := vv.name),
                        name := "texture3D"
                    )
                    var cll_bind = new ExprCall(at = vv.at, name := "bind_sampler_3d")
                    cll_bind.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = binding)
                    cll_bind.arguments |> emplace(vtex)
                    blk.list |> emplace(cll_bind)
                    // glUniformAny(gl_GetUniformLocation("varName"),binding)
                    var cll_gul = new ExprCall(at = vv.at, name := "glGetUniformLocation")
                    cll_gul.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "_opengl_program")
                    cll_gul.arguments |> emplace_new() <| new ExprConstString(at = vv.at, value := vv.name)
                    var cll_uni = new ExprCall(at = vv.at, name := "glUniformAny")
                    cll_uni.arguments |> emplace(cll_gul)
                    cll_uni.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = binding)
                    blk.list |> emplace(cll_uni)
                } elif (is_glsl_structure(vv, "image2D") || is_glsl_structure(vv, "image3D")) {
                    // bind_sampler_any(binding, varName.texture?D, format, access)
                    let is2D = is_glsl_structure(vv, "image2D")
                    let binding = vv.annotation |> find_arg("stage") ?as tInt ?? 0
                    var vtex = new ExprField(at = vv.at,
                        value <- new ExprVar(at = vv.at, name := vv.name),
                        name := is2D ? "texture2D" : "texture3D"
                    )
                    var format = vv.annotation |> find_arg("format") ?as tString ?? "rgba8"
                    format = "GL_{to_upper(format)}"
                    var access = "GL_READ_WRITE"
                    if (vv.annotation |> find_arg("writeonly") is tBool) {
                        access = "GL_WRITE_ONLY"
                    }
                    if (vv.annotation |> find_arg("readonly") is tBool) {
                        access = "GL_READ_ONLY"
                    }
                    var cll_uni = new ExprCall(at = vv.at, name := "bind_image_any")
                    cll_uni.arguments |> emplace_new() <| new ExprConstInt(at = vv.at, value = binding)
                    cll_uni.arguments |> emplace(vtex)
                    cll_uni.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := format)
                    cll_uni.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := access)
                    blk.list |> emplace(cll_uni)
                } elif (is_glsl_structure(vv, "samplerCube")) {
                    emit_combined_sampler_bind(blk, vv, "textureCube", "bind_sampler_cube")
                } elif (is_glsl_structure(vv, "sampler2DArray")) {
                    emit_combined_sampler_bind(blk, vv, "texture2DArray", "bind_sampler_2d_array")
                } elif (is_glsl_structure(vv, "sampler2DShadow")) {
                    emit_combined_sampler_bind(blk, vv, "texture2D", "bind_sampler_2d_shadow")
                } elif (vv._type.baseType == Type.tStructure && vv._type.structType != null) {
                    if (pack_std140 && std140_block_is_simple(vv._type.structType)) {
                        // std140 UBO pack (default for blocks whose fields all pack flat): defer until
                        // every block is collected, so binding points can be assigned in a stable order
                        // that agrees across the vertex/fragment bind functions (both walk the same
                        // module globals). The emitter consults the same predicate. Phase 2 / D5.
                        let nm = string(vv.name)
                        if (!key_exists(ubo_seen, nm)) {
                            ubo_seen |> insert(nm)
                            ubo_globals |> push(vv)
                        }
                    } else {
                        // loose fallback (packing off, or a float3x3 / composite block the UBO binder
                        // can't pack yet): a struct uniform lowers to `uniform StructType name;` and each
                        // leaf field binds by flattened name (name.field, name.field.subfield, ...).
                        emit_loose_struct_bind(blk, vv._type.structType, string(vv.name),
                            new ExprVar(at = vv.at, name := vv.name))
                    }
                } else {
                    // scalar / vector / matrix uniform: glUniformAny(glGetUniformLocation("name"), name)
                    var cll_gul = new ExprCall(at = vv.at, name := "glGetUniformLocation")
                    cll_gul.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := "_opengl_program")
                    cll_gul.arguments |> emplace_new() <| new ExprConstString(at = vv.at, value := vv.name)
                    var cll_uni = new ExprCall(at = vv.at, name := "glUniformAny")
                    cll_uni.arguments |> emplace(cll_gul)
                    cll_uni.arguments |> emplace_new() <| new ExprVar(at = vv.at, name := vv.name)
                    blk.list |> emplace(cll_uni)
                }
            }
        }
    }
    // std140 UBO binding points: assign by sorted block name so the same block gets the same point in
    // every stage's bind function (the glBindBufferRange index must agree across vertex/fragment for a
    // linked program). An explicit @binding wins; otherwise the name-sorted index is used.
    ubo_globals |> sort() $(a, b) => string(a.name) < string(b.name)
    for (i in range(length(ubo_globals))) {
        let vv = ubo_globals[i]
        let bp = vv.annotation |> find_arg("binding") ?as tInt ?? i
        emit_std140_ubo_bind(blk, vv, bp)
    }
    unsafe { delete ubo_globals; }
    delete ubo_seen
    fn.body := blk
    if (DEBUG_SHADER_BINDINGS) {
        print("{describe(fn)}\n")
    }
}
