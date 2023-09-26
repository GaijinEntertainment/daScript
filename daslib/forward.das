options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module forward shared public

require daslib/ast_boost
require daslib/templates_boost
require strings

[structure_macro(name="forward")]
class ForwardTypeMacro : AstStructureAnnotation
    def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        var anyErrors = false
        for arg in args
            var tname = ""
            if arg.basicType==Type tBool
                pass
            elif arg.basicType==Type tString && arg.name=="type"
                pass
            else
                errors := "{errors}\nexpecting type name or type=\"name\""
                anyErrors = true
        return !anyErrors
    def override aotNeedReinterpret ( var st:StructurePtr; args:AnnotationArgumentList ) : bool
        return true
    def override canSubstitute ( var st:StructurePtr; args:AnnotationArgumentList; passType:TypeDeclPtr ) : bool
        if passType.baseType != Type tStructure
            return false
        var modname = passType.structType._module!=null ? string(passType.structType._module.name) : ""
        var passname = "{modname}::{passType.structType.name}"
        for arg in args
            var tname = ""
            if arg.basicType==Type tBool
                tname = string(arg.name)
            elif arg.basicType==Type tString && arg.name=="type"
                tname = string(arg.sValue)
            if tname==""
                return false
            if tname |> find("::") < 0
                var thismodname = st._module!=null ? string(st._module.name) : ""
                tname := "{modname}::{tname}"
            if tname==passname
                return true
        return false

    def override canBeSubstituted ( var st:StructurePtr; args:AnnotationArgumentList; passType:TypeDeclPtr ) : bool
        print("canBeSubstituted {st.name} vs {describe(passType)}\n")
        return false


