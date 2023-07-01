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


[typeinfo_macro(name="safe_has_property")]
class TypeInfoHasProperty : AstTypeInfoMacro
    //! this macro implements "has_property" type trait, which returns true when structure has a property
    def override getAstChange(expr : smart_ptr<ExprTypeInfo>; var errors : das_string) : ExpressionPtr
        if expr.typeexpr != null
            if !expr.typeexpr.isHandle && !expr.typeexpr.isStructure
                errors := "expecting handle type or structure. typeinfo(safe_has_property<propName> v)"
                return <- [[ExpressionPtr]]
            if empty(expr.subtrait)
                errors := "expecting trait string. typeinfo(safe_has_property<propName> v)"
                return <- [[ExpressionPtr]]

            let propName = ".`{expr.subtrait}"
            var res = false
            get_ptr(compiling_program()) |> for_each_module() <| $(mod)
                if res
                    return
                mod |> for_each_function(propName) <| $(fn)
                    if fn.moreFlags.propertyFunction && expr.typeexpr |> is_same_type(fn.arguments[0]._type, RefMatters yes, ConstMatters no, TemporaryMatters no)
                        res = true

            return <- new [[ExprConstBool() at=expr.at, value=res]]
        errors := "type is missing or not inferred"
        return <- [[ExpressionPtr]]


[typeinfo_macro(name="has_method")]
class TypeInfoHasMethod: AstTypeInfoMacro
    //! this macro implements "has_method" type trait
    //! a method is defined as a function with its first argument being the type in question
    def override getAstChange(expr : smart_ptr<ExprTypeInfo>; var errors : das_string) : ExpressionPtr
        if expr.typeexpr == null
            errors := "type is missing or not inferred"
            return <- [[ExpressionPtr]]

        if !expr.typeexpr.isHandle && !expr.typeexpr.isStructure
            errors := "expecting handle type or structure. typeinfo(has_method<method> v)"
            return <- [[ExpressionPtr]]

        if expr.subtrait |> empty
            errors := "expecting trait string. typeinfo(has_method<method> v)"
            return <- [[ExpressionPtr]]

        var name = expr.subtrait |> string()
        var done = false

        get_ptr(compiling_program()) |> for_each_module() <| $(mod)
            return if done

            mod |> for_each_function(name) <| $(fn)
                return if fn.arguments |> length == 0
                assume first = fn.arguments[0]
                
                if expr.typeexpr |> is_same_type(first._type, RefMatters yes, ConstMatters no, TemporaryMatters no)
                    done = true

        return <- new [[ExprConstBool() at=expr.at, value=done]]
