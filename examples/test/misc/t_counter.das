module t_counter public

require daslib/ast_boost
require daslib/macro_boost
require daslib/templates_boost
require daslib/strings_boost

template struct TemplateCounter {
    counter : $t(counter_type)
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
        if (length(td.dimExpr) != 2) {
            macro_error(compiling_program(), td.at, "expecting 1 argument")
            return <- default<TypeDeclPtr>
        }
        if (!(td.dimExpr[1] is ExprTypeDecl)) {
            macro_error(compiling_program(), td.at, "expecting counter type")
            return <- default<TypeDeclPtr>
        }
//      if ( td.dimExpr[1]._type == null ) {
//          // type is not inferred, which means we are inferring generic type
//          return null // TODO: fix me
//      }
        var inscope counter_type := (td.dimExpr[1] as ExprTypeDecl).typeexpr
        var inscope template_type <- qmacro_type(type<TemplateCounter>)
        var struct_name = "Counter`{describe(counter_type)}"
        var existing_struct = prog.find_unique_structure(struct_name)
        if (existing_struct != null) {
            return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = td.at)
        }
        return <- qmacro_template_class(struct_name, type<TemplateCounter>)
    }
}
