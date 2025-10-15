module t_counter public

require daslib/ast_boost
require daslib/macro_boost
require daslib/templates_boost
require daslib/strings_boost
require daslib/typemacro_boost

struct public TemplateCounterBase {
}

template struct TemplateCounter : TemplateCounterBase {
    counter : $t(counter_type)
    def next_count {
        counter ++
    }
    def value {
        return counter
    }
}


[typemacro_function]
def counter(macroArgument, passArgument : TypeDeclPtr; counter_type : TypeDeclPtr) : TypeDeclPtr {
    if (passArgument != null) {
        var pbase = compiling_program() |> find_unique_structure("t_counter::TemplateCounterBase")
        if (!(passArgument.baseType == Type.tStructure && passArgument.structType.parent == pbase)) {
            return <- TypeDeclPtr()
        }
        let field = passArgument.structType |> find_structure_field("counter")
        if (field == null || field._type == null) {
            return <- TypeDeclPtr()
       }
        var inscope argT <- clone_type((macroArgument.dimExpr[1] as ExprTypeDecl).typeexpr)
        if (argT == null) {
            return <- TypeDeclPtr()
        }
        if (argT.isAutoOrAlias) {
            var inscope fieldT <- clone_type(field._type)
            var inscope resT <- infer_generic_type(argT, fieldT, true, true)
            if (resT == null) {
                macro_error(compiling_program(), macroArgument.at, "type mismatch, expecting {describe(argT)} got {describe(fieldT)}")
                return <- TypeDeclPtr()
            }
            if (!fieldT |> is_same_type(resT, RefMatters.no, ConstMatters.no, TemporaryMatters.yes)) {
                macro_error(compiling_program(), macroArgument.at, "type mismatch, expecting {describe(argT)} got {describe(fieldT)}")
                return <- TypeDeclPtr()
            }
            compiling_program() |> update_alias_map(argT, resT)
        } else {
            // do we need to check if passT is auto and do 2-sided infer?
            var inscope fieldT <- clone_type(field._type)
            fieldT.dim |> pop
            if (!fieldT |> is_same_type(argT, RefMatters.no, ConstMatters.no, TemporaryMatters.yes)) {
                macro_error(compiling_program(), macroArgument.at, "type mismatch, expecting {describe(argT)} got {describe(fieldT)}")
                return <- TypeDeclPtr()
            }
        }

        // we are inferring generic here
        var inscope resT <- clone_type(passArgument)
        resT.flags &= ~TypeDeclFlags.ref
        return <- resT
    }
    // we are instantiating concrete type here
    var inscope template_type <- qmacro_type(type<TemplateCounter>)
    var struct_name = "Counter`{describe(counter_type)}"
    var existing_struct = compiling_program().find_unique_structure(struct_name)
    if (existing_struct != null) {
        return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = counter_type.at)
    }
    return <- qmacro_template_class(struct_name, type<TemplateCounter>)
}

