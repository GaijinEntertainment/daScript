options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers

module type_traits shared private

require daslib/ast_boost

[typeinfo_macro(name="fields_count")]
class TypeInfoGetFieldsNum : AstTypeInfoMacro
    //! this macro implements "fields_count" type trait, which returns total number of fields in the structure
    def override getAstChange(expr : smart_ptr<ExprTypeInfo>; var errors : das_string) : ExpressionPtr
        if expr.typeexpr != null
            if !expr.typeexpr.isStructure
                errors := "expecting structure"
                return <- [[ExpressionPtr]]
            let fieldsNum = length(expr.typeexpr.structType.fields)
            return <- new [[ExprConstInt() at=expr.at, value=fieldsNum]]
        errors := "type is missing or not inferred"
        return <- [[ExpressionPtr]]
