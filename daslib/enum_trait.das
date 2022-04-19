options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module enum_trait shared private

require ast
require daslib/ast_boost

[typeinfo_macro(name="enum_length")]
class TypeInfoGetEnumLength : AstTypeInfoMacro
    //! Implements typeinfo(enum_length EnumOrEnumType) which returns total number of elements in enumeration.
    def override getAstChange ( expr:smart_ptr<ExprTypeInfo>; var errors:das_string ) : ExpressionPtr
        if expr.typeexpr != null
            if !expr.typeexpr.isEnum
                errors := "expecting enumeration"
                return [[ExpressionPtr]]
            let sz = length(expr.typeexpr.enumType.list)
            return new [[ExprConstInt() at=expr.at, value=sz]]
        errors := "type is missing or not inferred"
        return [[ExpressionPtr]]
