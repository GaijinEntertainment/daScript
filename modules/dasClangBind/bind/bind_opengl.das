options persistent_heap
options gc

require cbind/cbind_boost
require daslib/safe_addr
require daslib/strings
require fio

let OPENGL_FUNCTION_RESULT_RULES = (
    TypeRules char_ptr_is_string |
    TypeRules function_proto_is_void_ptr |
    TypeRules structure_ptr_is_void_ptr
)

let OPENGL_FUNCTION_ARGUMENT_RULES = (
    TypeRules char_ptr_is_string |
    TypeRules function_proto_is_void_ptr |
    TypeRules structure_ptr_is_void_ptr |
    TypeRules top_level_pointers_are_implicit
)

let known_gl_type <- {{
    "GLboolean"         => "bool";
    "GLboolean *"       => "bool? implicit";
    "const GLbyte *"    => "int8 const? implicit"
}}

def clang_glType ( var nctype:CXType; rules:TypeRules )
    let ncname = string(clang_getTypeSpelling(nctype))
    let tname = known_gl_type?[ncname] ?? ""
    if tname != ""
        return tname
    var ctype = clang_getCanonicalType(nctype)
    return clang_typeToDasType(ctype,rules)

[export]
def main
    let PARSE_FILE_NAME = "/modules/dasOpenGL/opengl/OpenGL/glcorearb.h"
    let ARGV <- [[string[1] "-DGL_GLEXT_PROTOTYPES"]]
    var index = clang_createIndex(0,0)
    var unit = clang_parseTranslationUnit(
        index,
        "{get_das_root()}{PARSE_FILE_NAME}",
        safe_addr(ARGV[0]), length(ARGV),
        null, 0u,
        CXTranslationUnit_None)
    if unit==null
        panic("unable to parse translation unit")
    var prevFileName = ""
    var cursor = clang_getTranslationUnitCursor(unit)
    let output_name = "{get_das_root()}/modules/dasOpenGL/opengl/opengl_func.das"
    fopen(output_name,"wb") <| $( f )
        if f == null
            to_log(LOG_ERROR, "cannot open '{output_name}' for writing. opengl functions will not be written.")
        else
            fprint(f, "require dasbind public\n\n")
            clang_visitChildren(cursor) <| $ ( var c,parent )
                var file : CXFile
                var line, column, offset : uint
                clang_getSpellingLocation(clang_getCursorLocation(c), safe_addr(file), safe_addr(line), safe_addr(column), safe_addr(offset))
                var fname = clang_getFileName(file)
                peek(fname) <| $ ( fn )
                    if prevFileName != fn
                        prevFileName := fn
                        if !prevFileName |> ends_with(PARSE_FILE_NAME)
                            to_log(LOG_WARNING, "skipping {prevFileName}")
                if prevFileName |> ends_with(PARSE_FILE_NAME)
                    let kind = clang_getCursorKind(c)
                    if kind == CXCursorKind CXCursor_TypedefDecl
                        return CXChildVisitResult CXChildVisit_Continue
                    elif kind == CXCursorKind CXCursor_FunctionDecl
                        let function_name = string(clang_getCursorSpelling(c))
                        var fun_type = clang_getCursorType(c)
                        fprint(f, "[extern(opengl,late,name=\"{function_name}\",library=\"Opengl32\")]\n")
                        fprint(f, "def {function_name} ( ")
                        let narg = clang_Cursor_getNumArguments(c)
                        for ai in urange(narg)
                            if ai != 0u
                                fprint(f, "; ")
                            var carg = clang_Cursor_getArgument(c, ai)
                            let arg_name = string(clang_getCursorSpelling(carg))
                            var arg_type = clang_getCursorType(carg)
                            let darg_type = clang_glType(arg_type,OPENGL_FUNCTION_ARGUMENT_RULES)
                            let darg_name = clang_argNameToDasArgName(arg_name,ai)
                            fprint(f, "{darg_name}:{darg_type}")
                        fprint(f, " ) : ")
                        var res_type = clang_getResultType(fun_type)
                        let dres_type = clang_glType(res_type,OPENGL_FUNCTION_RESULT_RULES)
                        fprint(f, "{dres_type}\n")
                        if res_type.kind==CXTypeKind CXType_Void
                            fprint(f, "\tpass\n\n")
                        else
                            fprint(f, "\treturn [[{dres_type}]]\n\n")
                        return CXChildVisitResult CXChildVisit_Continue
                    elif kind == CXCursorKind CXCursor_StructDecl
                        return CXChildVisitResult CXChildVisit_Continue
                    else
                        to_log(LOG_INFO, "{describe(c)}")
                    return CXChildVisitResult CXChildVisit_Recurse
                else
                    return CXChildVisitResult CXChildVisit_Recurse
    clang_disposeTranslationUnit(unit)
    clang_disposeIndex(index)

    // generate defines from #define in the opengl file
    blocked_defines <- [{string []
        "true";
        "false"
    }]
    let output_const_name = "{get_das_root()}/modules/dasOpenGL/opengl/opengl_const.das"
    genDefineConstants([{string "{get_das_root()}{PARSE_FILE_NAME}"}],output_const_name)
