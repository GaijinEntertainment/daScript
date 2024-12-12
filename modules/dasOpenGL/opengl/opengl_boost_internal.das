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

[tag_function(compile_shader_tag)]
def public create_shader_program ( vs_main, ps_main : function<void> ) : uint
    panic("somehow we are in this is temporary stub for the resolved create_shader_program")
    return 0u

[tag_function(compile_compute_shader_tag)]
def public create_compute_shader_program ( cs_main : function<void> ) : uint
    panic("somehow we are in this is temporary stub for the resolved create_compute_shader_program")
    return 0u

[macro_function]
def get_shader_const_name ( arg:ExpressionPtr; var errors:das_string )
    var aAddr = arg ?as ExprAddr
    if aAddr==null
        errors := "expecting @@shader_function"
        return ""
    if aAddr.func==null
        errors := "function not found yet"
        return ""
    for ann in aAddr.func.annotations
        if ann.annotation.name=="vertex_program" || ann.annotation.name=="fragment_program" || ann.annotation.name=="compute_program"
            let cname = ann.arguments |> find_arg("name") ?as tString ?? ""
            if cname==""
                return "{aAddr.func.name}`shader_text"
            else
                return cname
    errors := "function needs to be annotated as vertex_program, fragment_program, or compute_program"
    return ""

[tag_function_macro(tag="compile_shader_tag")]
class BgfxCompileShaderFunctionAnnotation : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        // see if there
        if call.arguments[0]._type==null || call.arguments[1]._type==null
            return [[ExpressionPtr]]
        let vs_name = get_shader_const_name(call.arguments[0],errors)
        if vs_name==""
            return [[ExpressionPtr]]
        let fs_name = get_shader_const_name(call.arguments[1],errors)
        if fs_name==""
            return [[ExpressionPtr]]
        var cll <- new [[ExprCall() at=call.at, name:="create_shader_program"]]
        cll.arguments |> emplace_new <| new [[ExprVar() at=call.at, name:=vs_name]]
        cll.arguments |> emplace_new <| new [[ExprVar() at=call.at, name:=fs_name]]
        return <- cll

[tag_function_macro(tag="compile_compute_shader_tag")]
class BgfxCompileComputeShaderFunctionAnnotation : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        // see if there
        if call.arguments[0]._type==null
            return [[ExpressionPtr]]
        let cs_name = get_shader_const_name(call.arguments[0],errors)
        if cs_name==""
            return [[ExpressionPtr]]
        let func <- (call.arguments[0] as ExprAddr).func
        let target = func._module.name!="" ? "{func._module.name}::{func.name}" : "{func.name}"
        var cll <- new [[ExprCall() at=call.at, name:="create_compute_shader_program"]]
        cll.arguments |> emplace_new <| new [[ExprVar() at=call.at, name:=cs_name]]
        return <- cll

[structure_macro(name="vertex_buffer")]
class GlslVertexBuffer : AstStructureAnnotation
    def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        // [template(self)] bind_vertex_buffer(vbo:uint; self:VertexType; vindex:uint=0u)
        var fn <- new [[Function() at = st.at, atDecl = st.at, name := "bind_vertex_buffer"]]
        fn.flags |= FunctionFlags generated
        fn.result <- new [[TypeDecl() baseType=Type tVoid, at=st.at]]
        fn.arguments |> emplace_new() <| new [[Variable() at=st.at,
            name := "ptrd",
            _type <- new [[TypeDecl() at=st.at, baseType=Type tPointer,
                flags=TypeDeclFlags _implicit | TypeDeclFlags constant]]
        ]]
        fn.arguments |> emplace_new() <| new [[Variable() at=st.at,
            name := "self",
            _type <- new [[TypeDecl() at=st.at,
                baseType=Type tStructure,
                structType=get_ptr(st)
            ]]
        ]]
        fn.arguments |> emplace_new() <| new [[Variable() at=st.at,
            name := "vindex",
            _type <- new [[TypeDecl() at=st.at, baseType=Type tUInt]],
            init <- new [[ExprConstUInt() at=st.at, value=0u]]
        ]]
        var blk <- new [[ExprBlock() at=st.at]]
        // for each field
        var stype <- new [[TypeDecl() baseType=Type tStructure, structType=get_ptr(st), at=st.at]]
        for fld,attri in st.fields,count()
            // attri+vindex
            var vattr_i <- new [[ExprOp2() at=fld.at,
                op := "+",
                left <- new [[ExprConstUInt() at=fld.at, value=uint(attri)]],
                right <-  new [[ExprVar() at=fld.at, name:="vindex"]]
            ]]
            // glEnableVertexAttribArray(attri)
            var cll_evaa <- new [[ExprCall() at=fld.at, name:="glEnableVertexAttribArray" ]]
            cll_evaa.arguments |> emplace_new() <| clone_expression(vattr_i)
            blk.list |> emplace(cll_evaa)
            var dinfo = get_decl_info(fld._type, fld.annotation)
            if !dinfo.ok
                errors := "field {fld.name} is missing type information"
                return false
            var ttype = 0u
            if dinfo.info.typet==Type tFloat
                ttype = GL_FLOAT
            elif dinfo.info.typet==Type tUInt
                ttype = GL_UNSIGNED_BYTE
            elif dinfo.info.typet==Type tInt16
                ttype = GL_SHORT
            else
                errors := "field {fld.name} type {dinfo.info.typet} is not supported"
                return false
            // glVertexAttribPointer(attri, num_attr, attr_type, is_norm, sizeof(VertexType), offsetof(VertexType::fld))
            var cll_vap <- new [[ExprCall() at=fld.at, name:="glVertexAttribPointer"]]
            cll_vap.arguments |> emplace_new() <| clone_expression(vattr_i)
            cll_vap.arguments |> emplace_new() <| new [[ExprConstInt() at=fld.at, value=dinfo.info.sizei]]
            cll_vap.arguments |> emplace_new() <| new [[ExprConstUInt() at=fld.at, value=ttype]]
            cll_vap.arguments |> emplace_new() <| new [[ExprConstBool() at=fld.at, value=dinfo.info.normalized]]
            cll_vap.arguments |> emplace_new() <| new [[ExprTypeInfo() at=fld.at, trait := "sizeof", typeexpr <- clone_type(stype)]]
            cll_vap.arguments |> emplace_new() <| new [[ExprVar() at=fld.at, name:="ptrd"]]
            cll_vap.arguments |> emplace_new() <| new [[ExprTypeInfo() at=fld.at, trait := "offsetof",
                subtrait := fld.name, typeexpr <- clone_type(stype)]]
            blk.list |> emplace(cll_vap)
            vattr_i := null
        stype := null
        fn.body <- blk
        append_annotation(fn,"templates","template",[{auto[]
            [[auto "self", [[RttiValue tBool=true]] ]]
        }])
        if !(compiling_module() |> add_function(fn))
            panic("can't setup")
        return true


