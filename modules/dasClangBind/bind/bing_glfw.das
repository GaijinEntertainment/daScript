require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class GlfwGen : CppGenBind
    def GlfwGen
        bind_root = "{get_das_root()}/Modules/dasGlfw/src"
        bind_module = "dasGLFW"
        let pfn = "GLFW/glfw3.h"
        let pfp = "{get_das_root()}/Modules/dasGlfw/glfw/include/"
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-I{get_full_file_name(pfp)}"
        }]
        func_per_chunk = 100500 // one file only?
        init_args(pfn,pfp,args)
        setDefaultFiles()
        init_enum_prefix()
        // openAllFiles()
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
        if fname |> ends_with("glfw3.h")
            return false
        return ! fname |> ends_with(PARSE_FILE_NAME)

[export]
def main
    // generate
    var cgb = new GlfwGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
