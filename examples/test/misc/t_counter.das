options gen2

module t_counter public

require daslib/ast_boost
require daslib/templates_boost
require daslib/typemacro_boost

struct template TemplateCounter {
    counter : CounterType   // we add this one in a typemacro (or we can use $t(counter_type))
    def TemplateCounter {
        counter = CounterType($v(initialValue))
    }
    def next {
        counter ++
    }
    def value {
        return counter
    }
}

def template print_counter(c : $t(resType)) {
    print("Counter value: {c.value()}\n")
}

[typemacro_function]
def counter(macroArgument, passArgument : TypeDeclPtr; counter_type : TypeDeclPtr; initialValue : int = 123) : TypeDeclPtr {
        var inscope template_type <- typeinfo ast_typedecl(type<TemplateCounter>)
        var inscope template_arguments <- [
            TypeMacroTemplateArgument(name="CounterType",  argument_type <- clone_type(counter_type))
        ]
        var extra_template_arguments <- [
            ("initialValue", "{initialValue}")
        ]
        // code bellow can be auto-generated
        if (passArgument != null) {
            return <- TypeDeclPtr() if (!is_typemacro_template_instance(passArgument, template_type, extra_template_arguments))
            return <- TypeDeclPtr() if (!infer_struct_aliases(passArgument.structType, template_arguments))
            return <- infer_template_types(passArgument, template_arguments)
        } else {
            return <- TypeDeclPtr() if (!verify_arguments(template_arguments))
            var struct_name = template_structure_name(template_type.structType, template_arguments, extra_template_arguments)
            var existing_struct = compiling_program().find_unique_structure(struct_name)
            return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = template_type.at) if (existing_struct != null)
            var inscope resType <- qmacro_template_class(struct_name, type<TemplateCounter>)
            make_typemacro_template_instance(resType.structType, template_type.structType, extra_template_arguments)
            add_structure_aliases(resType.structType, template_arguments)
            // for demo purposes, we can add a print_counter function here
            var inscope print_fn <- qmacro_template_function(@@print_counter)
            add_function(compiling_module(), print_fn)
            return <- resType
        }
}

