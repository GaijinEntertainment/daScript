module padd

require daslib/ast_boost
require daslib/macro_boost
require daslib/templates_boost

[type_macro(name="padd_structure")]
class PaddStructureMacro : AstTypeMacro
    def override visit ( prog:ProgramPtr; mod:Module?; td:TypeDeclPtr ) : TypeDeclPtr
        if length(td.dimExpr) != 2
            macro_error(compiling_program(), td.at, "expecting 1 argument")
            return <- [[TypeDeclPtr()]]
        if !(td.dimExpr[1] is ExprConstInt)
            macro_error(compiling_program(), td.at, "expecting constant integer (size)")
            return <- [[TypeDeclPtr()]]
        let bytes = (td.dimExpr[1] as ExprConstInt).value
        var inscope st <- new [[Structure() at=td.at, name := "PaddStructure_{bytes}_at_{td.at.line}"]] // TODO: unique name
        st.flags |= StructureFlags privateStructure | StructureFlags generated
        var inscope st_type <- new [[TypeDecl() at=td.at, baseType=Type tInt8]]
        st_type.dim |> push(bytes)
        st |> add_structure_field ( "bytes", clone_type(st_type), [[ExpressionPtr]])
        var inscope retType <- new [[TypeDecl() at=td.at, baseType=Type tStructure, structType=get_ptr(st)]]
        mod |> add_structure(st)
        return <- retType

