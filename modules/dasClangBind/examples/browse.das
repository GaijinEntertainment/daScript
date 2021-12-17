require cbind/cbind_boost
require daslib/safe_addr
require daslib/strings

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
    clang_visitChildren(cursor) <| $ ( var c,parent )
        var file : CXFile
        var line, column, offset : uint
        clang_getSpellingLocation(clang_getCursorLocation(c), safe_addr(file), safe_addr(line), safe_addr(column), safe_addr(offset))
        var fname = clang_getFileName(file)
        peek(fname) <| $ ( fn )
            if prevFileName != fn
                prevFileName := fn
                if !prevFileName |> ends_with(PARSE_FILE_NAME)
                    print("\tskipping {prevFileName}\n")
        if prevFileName |> ends_with(PARSE_FILE_NAME)
            let kind = clang_getCursorKind(c)
            if kind == CXCursorKind CXCursor_TypedefDecl
                // do nothing with typedef
                return CXChildVisitResult CXChildVisit_Continue
            elif kind == CXCursorKind CXCursor_FunctionDecl
                let function_name = string(clang_getCursorSpelling(c))
                var fun_type = clang_getCanonicalType(clang_getCursorType(c))
                var res_type = clang_getResultType(fun_type)
                let function_result_type = string(clang_getTypeSpelling(res_type))
                print("{function_result_type} {function_name} ( ")
                let narg = clang_Cursor_getNumArguments(c)
                for ai in urange(narg)
                    if ai != 0u
                        print(", ")
                    var carg = clang_Cursor_getArgument(c, ai)
                    let arg_name = string(clang_getCursorSpelling(carg))
                    var arg_type = clang_getCanonicalType(clang_getCursorType(carg))
                    let argument_type = string(clang_getTypeSpelling(arg_type))
                    print("{argument_type} {arg_name}")
                print(" );\n")
                return CXChildVisitResult CXChildVisit_Continue
            elif kind == CXCursorKind CXCursor_StructDecl
                // TODO: parse structure declaration
                return CXChildVisitResult CXChildVisit_Continue
            else
                print("{describe(c)}\n")
            return CXChildVisitResult CXChildVisit_Recurse
        else
            return CXChildVisitResult CXChildVisit_Recurse
    clang_disposeTranslationUnit(unit)
    clang_disposeIndex(index)
