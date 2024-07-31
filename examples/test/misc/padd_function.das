module padd_function


require daslib/typemacro_boost
require daslib/templates_boost

[typemacro_function]
def padd_structure ( macroArgument:TypeDeclPtr; elemType:TypeDeclPtr; count:int )
    let structName = "PaddStructure`{count}`{macroArgument.at.line}`{macroArgument.at.column}"
    var st_ptr = compiling_program() |> find_unique_structure(structName)
    if st_ptr == null
        var inscope st <- new [[Structure() at=macroArgument.at, name := "PaddStructure_{count}_{macroArgument.at.line}_{macroArgument.at.column}"]]
        st.flags |= StructureFlags privateStructure | StructureFlags generated
        var inscope st_type <- clone_type(elemType)
        st_type.dim |> push(count)
        st |> add_structure_field ( "data", clone_type(st_type), [[ExpressionPtr]])
        st_ptr = get_ptr(st)
        compiling_module() |> add_structure(st)
    return <- new [[TypeDecl() at=macroArgument.at, baseType=Type tStructure, structType=st_ptr]]

options log
