options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module enum_trait shared private

require ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/templates
require daslib/contracts

[expect_any_enum(arg)]
def public string(arg) {
    //! converts enum value to string
    //!  usage: let s = string(EnumValue)
    return "{arg}"
}

[template(ent), unused_argument(ent)]
def public to_enum(ent : auto(EnumT), name : string) : EnumT {
    //! converts string to enum value, panics if not found
    //!  usage: let e = to_enum(type<EnumType>,"EnumValueName")
    for (e in each_enum(type<EnumT>)) {
        if ("{e}" == name) {
            return e
        }
    }
    panic("to_enum: enum value '{name}' not found in enum '{typeinfo typename(type<EnumT>)}'")
    return default<EnumT>
}

[template(ent), unused_argument(ent)]
def public to_enum(ent : auto(EnumT), name : string; defaultValue : EnumT) : EnumT {
    //! converts string to enum value, returns defaultValue if not found
    //!  usage: let e = to_enum(type<EnumType>,"EnumValueName", EnumType.DefaultValue)
    for (e in each_enum(type<EnumT>)) {
        if ("{e}" == name) {
            return e
        }
    }
    return defaultValue
}

[enumeration_macro(name="string_to_enum")]
class EnumFromStringConstruction : AstEnumerationAnnotation {
    def override apply(var enu : EnumerationPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        var inscope enumT <- new TypeDecl(baseType = Type.tEnumeration, enumType = enu.get_ptr())
        // panicing one
        var inscope enumFn <- qmacro_function("{enu.name}") <| $(src : string) {
            return to_enum(type<$t(enumT)>, src)
        }
        enumFn.flags &= ~FunctionFlags.privateFunction
        compiling_module() |> add_function(enumFn)
        // with default
        var inscope enumFnDefault <- qmacro_function("{enu.name}") <| $(src : string; defaultValue : $t(enumT)) {
            return to_enum(type<$t(enumT)>, src, defaultValue)
        }
        enumFnDefault.flags &= ~FunctionFlags.privateFunction
        compiling_module() |> add_function(enumFnDefault)
        return true

    }
}

[typeinfo_macro(name="enum_length")]
class TypeInfoGetEnumLength : AstTypeInfoMacro {
    //! Implements typeinfo(enum_length EnumOrEnumType) which returns total number of elements in enumeration.
    def override getAstChange(expr : smart_ptr<ExprTypeInfo>; var errors : das_string) : ExpressionPtr {
        if (expr.typeexpr != null) {
            if (!expr.typeexpr.isEnum) {
                errors := "expecting enumeration"
                return <- default<ExpressionPtr>
            }
            let sz = length(expr.typeexpr.enumType.list)
            return <- new ExprConstInt(at = expr.at, value = sz)
        }
        errors := "type is missing or not inferred"
        return <- default<ExpressionPtr>
    }
}

[typeinfo_macro(name="enum_names")]
class TypeInfoGetEnumNames : AstTypeInfoMacro {
    //! Implements typeinfo(enum_names EnumOrEnumType) which returns array of strings with enumValue names.
    def override getAstChange(expr : smart_ptr<ExprTypeInfo>; var errors : das_string) : ExpressionPtr {
        if (expr.typeexpr == null) {
            errors := "type is missing or not inferred"
            return <- default<ExpressionPtr>
        }
        if (!expr.typeexpr.isEnum) {
            errors := "expecting enumeration"
            return <- default<ExpressionPtr>
        }
        var inscope arr <- new ExprMakeArray(at = expr.at, makeType <- typeinfo ast_typedecl(type<string>))
        for (i in iter_range(expr.typeexpr.enumType.list)) {
            if (true) {
                assume name = expr.typeexpr.enumType.list[i].name
                var inscope nameExpr <- new ExprConstString(at = expr.at, value := name)
                arr.values |> emplace <| nameExpr
            }
        }
        return <- arr
    }
}
