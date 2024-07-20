module padd_function


require daslib/typemacro_boost
require daslib/templates_boost

[typemacro_function]
def padd_structure ( prog:ProgramPtr; mod:Module?; td:TypeDeclPtr; elemType:TypeDeclPtr; count:int )
    let structName = "PaddStructure`{count}`{td.at.line}`{td.at.column}"
    var st_ptr = compiling_program() |> find_unique_structure(structName)
    if st_ptr == null
        var inscope st <- new [[Structure() at=td.at, name := "PaddStructure_{count}_{td.at.line}_{td.at.column}"]]
        st.flags |= StructureFlags privateStructure | StructureFlags generated
        var inscope st_type <- clone_type(elemType)
        st_type.dim |> push(count)
        st |> add_structure_field ( "data", clone_type(st_type), [[ExpressionPtr]])
        st_ptr = get_ptr(st)
        mod |> add_structure(st)
    return <- new [[TypeDecl() at=td.at, baseType=Type tStructure, structType=st_ptr]]

