options gen2

module t_counter public

require daslib/ast_boost
require daslib/templates_boost
require daslib/typemacro_boost

struct public TemplateCounterBase {}

template struct TemplateCounter : TemplateCounterBase {
    counter : $t(counter_type)
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
        return <- TypeDeclPtr() if (!is_qmacro_template_instance(passArgument, "t_counter::TemplateCounter"))
        var inscope inferred_counter_type <- determine_template_type_by_field(passArgument, "counter")
        return <- TypeDeclPtr() if (inferred_counter_type == null)
        return <- infer_template_type(passArgument, counter_type, inferred_counter_type)
    } else {
        // we are instantiating concrete type here
        var inscope template_type <- qmacro_type(type<TemplateCounter>)
        var struct_name = "Counter`{describe(counter_type,false,false,true)}"
        var existing_struct = compiling_program().find_unique_structure(struct_name)
        return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = counter_type.at) if (existing_struct != null)
        return <- qmacro_template_class(struct_name, type<TemplateCounter>)
    }
}

