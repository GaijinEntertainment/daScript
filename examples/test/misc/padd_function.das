module padd_function


require daslib/typemacro_boost
require daslib/templates_boost

enum public PadFoo
    A
    B
    C

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

[typemacro_function]
def tpad_enum ( macroArgument:TypeDeclPtr; foo:PadFoo )
    return <- new [[TypeDecl() at=macroArgument.at, baseType=Type tInt, alias:="{foo}"]]

[typemacro_function]
def tpad_bool_const ( macroArgument:TypeDeclPtr; yes:bool )
    return <- new [[TypeDecl() at=macroArgument.at, baseType=yes ? Type tBool : Type tVoid]]

[typemacro_function]
def tpad_bitfield ( macroArgument:TypeDeclPtr; field:bitfield )
    return <- new [[TypeDecl() at=macroArgument.at, baseType=Type tBitfield, alias:="passed {field}"]]

