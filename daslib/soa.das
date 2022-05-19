options indenting = 4

module soa public

require daslib/ast_boost

// replacing a[index].b with a.b[index]

struct public SOA_INDEX {}

[tag_function(soa_call)]
def public operator . ( var src : SOA_INDEX; field:string )
    assert(false,"this code should never been instanced. soa_call macro replaces the soa[index].field with soa.field[index]")

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
