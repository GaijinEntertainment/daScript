options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot

// OLD das restrictions
options unsafe_table_lookup = false
options strict_smart_pointers = false
options relaxed_pointer_const = true

module opengl_boost_internal shared private

require daslib/ast_boost
require daslib/templates
require daslib/templates_boost

require glsl/glsl_internal

require opengl

[tag_function(compile_shader_tag), unused_argument(vs_main), unused_argument(ps_main)]
def public create_shader_program(vs_main, ps_main : function<void>) : uint {
    panic("somehow we are in this is temporary stub for the resolved create_shader_program")
    return 0u
}

[tag_function(compile_compute_shader_tag), unused_argument(cs_main)]
def public create_compute_shader_program(cs_main : function<void>) : uint {
    panic("somehow we are in this is temporary stub for the resolved create_compute_shader_program")
    return 0u
}

[macro_function]
def get_shader_const_name(arg : ExpressionPtr; var errors : das_string) {
    var aAddr = arg ?as ExprAddr
    if (aAddr == null) {
        errors := "expecting @@shader_function"
        return ""
    }
    if (aAddr.func == null) {
        errors := "function not found yet"
        return ""
    }
    for (ann in aAddr.func.annotations) {
        if (ann.annotation.name == "vertex_program" || ann.annotation.name == "fragment_program" || ann.annotation.name == "compute_program") {
            let cname = ann.arguments |> find_arg("name") ?as tString ?? ""
            if (cname == "") {
                return "{aAddr.func.name}`shader_text"
            } else {
                return cname
            }
        }
    }
    errors := "function needs to be annotated as vertex_program, fragment_program, or compute_program"
    return ""
}

[tag_function_macro(tag="compile_shader_tag")]
class BgfxCompileShaderFunctionAnnotation : AstFunctionAnnotation {
    def override transform(var call : ExprCallFunc?; var errors : das_string) : ExpressionPtr {
        // see if there
        if (call.arguments[0]._type == null || call.arguments[1]._type == null) return default<ExpressionPtr>
        let vs_name = get_shader_const_name(call.arguments[0], errors)
        if (vs_name == "") return default<ExpressionPtr>
        let fs_name = get_shader_const_name(call.arguments[1], errors)
        if (fs_name == "") return default<ExpressionPtr>
        var cll <- new ExprCall(at = call.at, name := "create_shader_program")
        cll.arguments |> emplace_new <| new ExprVar(at = call.at, name := vs_name)
        cll.arguments |> emplace_new <| new ExprVar(at = call.at, name := fs_name)
        return <- cll
    }
}

[tag_function_macro(tag="compile_compute_shader_tag")]
class BgfxCompileComputeShaderFunctionAnnotation : AstFunctionAnnotation {
    def override transform(var call : ExprCallFunc?; var errors : das_string) : ExpressionPtr {
        // see if there
        if (call.arguments[0]._type == null) return default<ExpressionPtr>
        let cs_name = get_shader_const_name(call.arguments[0], errors)
        if (cs_name == "") return default<ExpressionPtr>
        let func <- (call.arguments[0] as ExprAddr).func
        let _target = func._module.name != "" ? "{func._module.name}::{func.name}" : "{func.name}"
        var cll <- new ExprCall(at = call.at, name := "create_compute_shader_program")
        cll.arguments |> emplace_new <| new ExprVar(at = call.at, name := cs_name)
        return <- cll
    }
}

struct private VertexAttrib {
    fieldName : string
    count : int
    glType : uint
    normalized : bool
}

//! One row per field in declaration order; on an unsupported field `errors` is set.
def private vertex_attribs(st : StructurePtr; var errors : das_string) : array<VertexAttrib> {
    var attribs : array<VertexAttrib>
    for (fld in st.fields) {
        let dinfo = get_decl_info(fld._type, fld.annotation)
        if (!dinfo.ok) {
            errors := "field {fld.name} is missing type information"
            return <- attribs
        }
        var glType = 0u
        if (dinfo.info.typet == Type.tFloat) {
            glType = GL_FLOAT
        } elif (dinfo.info.typet == Type.tUInt) {
            glType = GL_UNSIGNED_BYTE
        } elif (dinfo.info.typet == Type.tInt16) {
            glType = GL_SHORT
        } else {
            errors := "field {fld.name} type {dinfo.info.typet} is not supported"
            return <- attribs
        }
        attribs |> push(VertexAttrib(fieldName = string(fld.name), count = dinfo.info.sizei,
            glType = glType, normalized = dinfo.info.normalized))
    }
    return <- attribs
}

def private offset_of(stype : TypeDeclPtr; fieldName : string) : ExpressionPtr {
    var offsetOf <- new ExprTypeInfo(at = stype.at, trait := "offsetof", subtrait := fieldName,
        typeexpr <- clone_type(stype))
    return <- offsetOf
}

def private add_binder(var fn : FunctionPtr; var errors : das_string) : bool {
    fn.flags.generated = true
    append_annotation(fn, "templates", "template", [(
        (argname = "self", argvalue = RttiValue(tBool=true))
    )])
    if (!(compiling_module() |> add_function(fn))) {
        errors := "can't add bind_vertex_buffer"
        return false
    }
    return true
}

//! Emits two `[template(self)] bind_vertex_buffer` overloads per struct: `(ptrd, self, vindex)` edits
//! the bound VAO and array buffer; `(vao, vbo, self, vindex, binding)` is the GL 4.5 named-object form.
[structure_macro(name="vertex_buffer")]
class GlslVertexBuffer : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        let attribs <- vertex_attribs(st, errors)
        if (!empty(errors)) return false
        var stype <- new TypeDecl(baseType = Type.tStructure, structType = st, at = st.at)
        var pointerBody, namedBody : array<ExpressionPtr>
        for (attrib, ai in attribs, count()) {
            let attribIndex = uint(ai)
            pointerBody |> push(qmacro(glEnableVertexAttribArray($v(attribIndex) + vindex)))
            pointerBody |> push(qmacro(glVertexAttribPointer($v(attribIndex) + vindex, $v(attrib.count), $v(attrib.glType), $v(attrib.normalized),
                typeinfo sizeof(type<$t(stype)>), ptrd, $e(offset_of(stype, attrib.fieldName)))))
            namedBody |> push(qmacro(glEnableVertexArrayAttrib(vao, $v(attribIndex) + vindex)))
            namedBody |> push(qmacro(glVertexArrayAttribFormat(vao, $v(attribIndex) + vindex, $v(attrib.count), $v(attrib.glType), $v(attrib.normalized),
                uint($e(offset_of(stype, attrib.fieldName))))))
            namedBody |> push(qmacro(glVertexArrayAttribBinding(vao, $v(attribIndex) + vindex, binding)))
        }
        var fnPointer <- qmacro_function("bind_vertex_buffer") $(ptrd : void? const implicit; self : $t(stype); vindex : uint = 0u) : void {
            $b(pointerBody)
        }
        var fnNamed <- qmacro_function("bind_vertex_buffer") $(vao : uint; vbo : uint; self : $t(stype); vindex : uint = 0u; binding : uint = 0u) : void {
            glVertexArrayVertexBuffer(vao, binding, vbo, 0l, typeinfo sizeof(type<$t(stype)>))
            $b(namedBody)
        }
        return add_binder(fnPointer, errors) && add_binder(fnNamed, errors)
    }
}


