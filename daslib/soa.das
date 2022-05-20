options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module soa public

require daslib/ast_boost
require daslib/templates_boost

// replacing a[index].b with a.b[index]

struct public SOA_INDEX {}

[tag_function(soa_call)]
def public operator . ( var src : SOA_INDEX; field:string )
    assert(false,"this code should never been instanced. soa_call macro replaces the soa[index].field with soa.field[index]")

// this is what annotation also generates
def operator [] ( var src : auto; index:int )
    return [[SOA_INDEX]]    // note SOA_INDEX

[tag_function_macro(tag="soa_call")]
class SoaCallMacro : AstFunctionAnnotation
    //! This macro is used to promote a[index].b to a.b[index] for SOA operations
    def override transform ( var expr : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        if expr.arguments.length != 2
            errors := "expecting soa[index].field"
            return [[ExpressionPtr]]
        if !(expr.arguments[0] is ExprCall)
            errors := "expecting soa[index] as first argument, got {expr.__rtti}"
            return [[ExpressionPtr]]
        let idx_call = expr.arguments[0] as ExprCall
        if idx_call.name!="[]" || idx_call.arguments.length!=2
            errors := "expecting soa[index] as first argument, got {describe(expr.arguments[1])}"
            return [[ExpressionPtr]]
        if !(expr.arguments[1] is ExprConstString)
            errors := "expecting field"
            return [[ExpressionPtr]]
        let fld = expr.arguments[1] as ExprConstString
        var e_field <- new [[ExprField() at=expr.at,
            value <- clone_expression(idx_call.arguments[0]),
            name := fld.value,
            fieldFlags = ExprFieldFieldFlags no_promotion
        ]]
        var e_at <- new [[ExprAt() at=expr.at,
            subexpr <- e_field,
            index <- clone_expression(idx_call.arguments[1]),
            atFlags = ExprAtFlags no_promotion
        ]]
        return <- e_at

[structure_macro(name=soa)]
class SoaStructMacro : AstStructureAnnotation
    //! This macro generates the following::
    //! * structure, where every field is replaced by an array
    //! * operator [] to access the structure, which returns [[SOA_INDEX]]
    def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        // soa
        var stype <- clone_structure(get_ptr(st))
        stype.name := "{stype.name}`SOA"
        for fld in stype.fields
            fld._type := new [[TypeDecl() at=fld.at,
                baseType = Type tArray,
                firstType <- clone_type(fld._type)
            ]]
        // [] operator
        var idx_op <- new [[Function() at = st.at, atDecl = st.at, name := "[]"]]
        idx_op.flags |= FunctionFlags generated
        idx_op.result <- new [[TypeDecl() baseType=Type autoinfer, at=st.at]]
        var idx_body <- quote() <|
            return [[SOA_INDEX]]
        idx_op.body := move_unquote_block(idx_body)
        (idx_op.body as ExprBlock).blockFlags &= ~ ExprBlockFlags isClosure
        idx_op.arguments |> emplace_new <| new [[Variable()
            name:="st",
            at=st.at,
            _type <- new [[TypeDecl() baseType=Type tStructure, at=st.at, structType=get_ptr(stype), flags=TypeDeclFlags constant]]
        ]]
        idx_op.arguments |> emplace_new <| new [[Variable()
            name:="index",
            at=st.at,
            _type <- new [[TypeDecl() baseType=Type tInt, at=st.at, flags=TypeDeclFlags constant]]
        ]]
        // now types
        compiling_module() |> add_structure(stype)
        compiling_module() |> add_function(idx_op)


/*
// this is what annotation also generates
def operator [] ( var src : Particle`SOA; index:int )
    return [[SOA_INDEX]]    // note SOA_INDEX
*/

        return true
