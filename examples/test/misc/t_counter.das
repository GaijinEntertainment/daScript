options gen2

module t_counter public

require daslib/ast_boost
require daslib/templates_boost
require daslib/typemacro_boost

struct public TemplateCounterBase {}

template struct TemplateCounter : TemplateCounterBase {
    counter : CounterType   // we add this one in a typemacro (or we can use $t(counter_type))
    def next {
        counter ++
    }
    def value {
        return counter
    }
}

[typemacro_function]
def counter(macroArgument, passArgument : TypeDeclPtr; counter_type : TypeDeclPtr) : TypeDeclPtr {
    if (passArgument != null) {
        // this is a generic argument match, like $counter(type<auto(TT)>)
        var inscope template_type <- qmacro_type(type<TemplateCounter>)
        return <- TypeDeclPtr() if (!is_qmacro_template_instance(passArgument, template_type))
        var inscope inferred_counter_type <- get_structure_alias(passArgument.structType, "CounterType")
        return <- TypeDeclPtr() if (inferred_counter_type == null)
        return <- infer_template_type(passArgument, counter_type, inferred_counter_type)
    } else {
        // we are instantiating concrete type here
        var struct_name = "Counter<{describe(counter_type,false,false,true)}>"
        var existing_struct = compiling_program().find_unique_structure(struct_name)
        return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = counter_type.at) if (existing_struct != null)
        var inscope resType <- qmacro_template_class(struct_name, type<TemplateCounter>)
        var inscope CounterType <- clone_type(counter_type)
        add_structure_alias(resType.structType, "CounterType", CounterType)
        return <- resType
    }
}


