module t_counter public

require daslib/ast_boost
require daslib/macro_boost
require daslib/templates_boost
require daslib/strings_boost

template struct TemplateCounter {
    counter : CounterType
    def next_count {
        counter ++
    }
    def value {
        return counter
    }
}

[type_macro(name="counter")]
class CounterMacro : AstTypeMacro {
    def override visit(prog : ProgramPtr; mod : Module?; td, passT : TypeDeclPtr) : TypeDeclPtr {
        if ( length(td.dimExpr) != 2 ) {
            macro_error(compiling_program(), td.at, "expecting 1 argument")
            return <- default<TypeDeclPtr>
        }
        if ( !(td.dimExpr[1] is ExprTypeDecl) ) {
            macro_error(compiling_program(), td.at, "expecting counter type")
            return <- default<TypeDeclPtr>
        }
        /*
        if td.dimExpr[1]._type == null
            // type is not inferred, which means we are inferring generic type
            return null // TODO: fix me
        */
        var inscope counter_type := (td.dimExpr[1] as ExprTypeDecl).typeexpr
        var inscope template_type <- qmacro_type(type<TemplateCounter>)
        var struct_name = "Counter`{describe(counter_type)}"

        var existing_struct = prog.find_unique_structure(struct_name)
        if ( existing_struct != null ) {
            return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = td.at)
        }

        var inscope structure <- clone_structure(template_type.structType)
        structure.name := struct_name
        structure.flags &= ~(StructureFlags.isTemplate)

        var inscope rules : Template
        rules |> replaceTypeWithTypeDecl("CounterType") <| clone_type(counter_type)
        rules |> replaceStructWithTypeDecl(template_type.structType) <| new TypeDecl(baseType = Type.tStructure, structType = get_ptr(structure), at = td.at)

        // now, lets clone all methods in the module
        var template_module = template_type.structType._module
        for_each_function(template_module,"", $(func){
            if ( func.flags.isClassMethod && func.classParent == template_type.structType ) {
                var inscope func_clone <- clone_function(func)
                // we rename methods
                var method_name_parts = split(string(func.name), "`")
                method_name_parts[0] = struct_name
                let func_clone_name = join(method_name_parts, "`")
                func_clone.name := func_clone_name
                func_clone.moreFlags &= ~(MoreFunctionFlags.isTemplate)
                func_clone.classParent = get_ptr(structure)
                func_clone.result |> move_new <| apply_template(rules, func.at, clone_type(func.result))
                for ( arg in func_clone.arguments ) {
                    arg._type |> move_new <| apply_template(rules, arg.at, clone_type(arg._type))
                }
                rules |> renameVariable(string(func.name), func_clone_name)
                rules |> renameVariable("_::{func.name}", "_::{func_clone_name}")
                mod |> add_function(func_clone)
            }
        })

        // now we fix up fields
        for ( fld in structure.fields ) {
            fld._type |> move_new <| apply_template(rules, fld.at, clone_type(fld._type))
            if ( fld.init != null ) {
                fld.init |> move_new <| apply_template(rules, fld.at, clone_expression(fld.init))
            }
        }

        delete rules

        var inscope return_type <- new TypeDecl(baseType = Type.tStructure, structType = get_ptr(structure), at = td.at)
        mod |> add_structure(structure)
        return <- return_type
    }
}

