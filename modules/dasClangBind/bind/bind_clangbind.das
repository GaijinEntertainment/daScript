require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class ClangGen : CppGenBind
    def ClangGen
        bind_root = "{get_das_root()}/Modules/dasClangBind/src"
        bind_module = "dasClangBind"
        let pfn = "clang-c/Index.h"
        let pfp = "{get_das_root()}/../libclang/include/"
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-I{get_full_file_name(pfp)}";
            "-DWIN32";
            "-DCINDEX_EXPORTS"
        }]
        func_per_chunk = 20
        init_args(pfn,pfp,args)
        setDefaultFiles()
        init_enum_prefix()
        openAllFiles()
    def init_enum_prefix
        enum_prefix <- {{
            "CXAvailabilityKind" => "CXAvailability";
            "CXDiagnosticSeverity" => "CXDiagnostic";
            "CXLoadDiag_Error" => "CXLoadDiag";
            "CXDiagnosticDisplayOptions" => "CXDiagnostic_Display";
            "CXTypeKind" => "CXType";
            "CXCursorKind" => "CXCursor";
            "CXChildVisitResult" => "CXChildVisit"
        }}
    def override isArgByValue ( name:string )
        return true
    def override skip_file ( fname:string ) : bool
        if fname |> ends_with("CXErrorCode.h")
            return false
        elif fname |> ends_with("CXString.h")
            return false
        return ! fname |> ends_with(PARSE_FILE_NAME)
    def override writeFuncPrefix
        fwrite(func_file, "#include \"daScript/misc/platform.h\"\n")
        fwrite(func_file, "#include \"daScript/ast/ast.h\"\n")
        fwrite(func_file, "#include \"daScript/ast/ast_interop.h\"\n")
        fwrite(func_file, "#include \"daScript/ast/ast_handle.h\"\n")
        fwrite(func_file, "#include \"daScript/ast/ast_typefactory_bind.h\"\n")
        fwrite(func_file, "#include \"daScript/simulate/bind_enum.h\"\n")
        fwrite(func_file, "#include \"dasClangBind.h\"\n")
        fwrite(func_file, "#include \"needClangBind.h\"\n")
        fwrite(func_file, "namespace das \{\n")
        fwrite(func_file, "void Module_ClangBind::initFunctions_{func_chunk_index}( ModuleLibrary & lib ) \{\n")
    def override writeFuncPostfix
        fwrite(func_file, "\}\n\}\n\n")

[export]
def main
    // generate
    var cgb = new ClangGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
