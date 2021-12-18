require cbind/cbind_boost
require daslib/fio

class ClangGen : CppGenBind
    def ClangGen
        let pfn = "clang-c/Index.h"
        let pfp = "{get_das_root()}/../libclang/include/"
        init_args(pfn,pfp,DEFAULT_CLANG_ARGUMENTS)
        set_default_files()
        init_enum_prefix()
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

[export]
def main
    let clangbind_root = "{get_das_root()}/Modules/dasClangBind/src"
    fopen("{clangbind_root}/dasClangBind.enum.decl.inc","wb") <| $ ( edi )
        fopen("{clangbind_root}/dasClangBind.enum.class.inc","wb") <| $ ( eci )
            fopen("{clangbind_root}/dasClangBind.enum.add.inc","wb") <| $ ( eai )
                var cgb = new ClangGen()
                cgb->setEnumFiles ( edi,eci,eai )
                cgb->generate()
                unsafe
                    delete cgb
