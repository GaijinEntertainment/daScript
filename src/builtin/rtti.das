require rtti

def rttiCompile(mod_name,text:string;subexpr:block<(ok:bool;prog:RttiProgram;issues:das_string):void>)
    rtti_builtin_compile(mod_name,text,subexpr)

def rttiProgramForEachStructure(prog:RttiProgram;subexpr:block<(info:StructInfo):void>)
    rtti_builtin_program_for_each_structure(prog,subexpr)

