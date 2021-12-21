require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class ClangGen : CppGenBind
    override func_to_stdout = false
    def ClangGen
        bind_root = "{get_das_root()}/Modules/dasClangBind/src"
        bind_module = "dasClangBind"
        bind_das_module = "cbind"
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
            "CXChildVisitResult" => "CXChildVisit";
            "CX_CXXAccessSpecifier" => "CX_CXX";
            "CXErrorCode" => "CXError";
            "CXGlobalOptFlags" => "CXGlobalOpt";
            "CXTranslationUnit_Flags" => "CXTranslationUnit";
            "CXSaveTranslationUnit_Flags" => "CXSaveTranslationUnit";
            "CXReparse_Flags" => "CXReparse";
            "CXTUResourceUsageKind" => "CXTUResourceUsage";
            "CXLinkageKind" => "CXLinkage";
            "CXVisibilityKind" => "CXVisibility";
            "CXLanguageKind" => "CXLanguage";
            "CXTLSKind" => "CXTLS";
            "CXTypeNullabilityKind" => "CXTypeNullability";
            "CXRefQualifierKind" => "CXRefQualifier";
            "CX_StorageClass" => "CX_SC_";
            "CXPrintingPolicyProperty" => "CXPrintingPolicy";
            "CXObjCPropertyAttrKind" => "CXObjCPropertyAttr";
            "CXObjCDeclQualifierKind" => "CXObjCDeclQualifier";
            "CXNameRefFlags" => "CXNameRange";
            "CXTokenKind" => "CXToken";
            "CXCompletionChunkKind" => "CXCompletionChunk";
            "CXCodeComplete_Flags" => "CXCodeComplete";
            "CXEvalResultKind" => "CXEval";
            "CXVisitorResult" => "CXVisit";
            "CXIdxEntityKind" => "CXIdxEntity";
            "CXIdxEntityLanguage" => "CXIdxEntityLang";
            "CXIdxEntityCXXTemplateKind" => "CXIdxEntity";
            "CXIdxAttrKind" => "CXIdxAttr";
            "CXIdxDeclInfoFlags" => "CXIdxDeclFlag";
            "CXIdxObjCContainerKind" => "CXIdxObjCContainer";
            "CXIdxEntityRefKind" => "CXIdxEntityRef";
            "CXIndexOptFlags" => "CXIndexOpt"
        }}
    def override isArgByValue ( name:string )
        return true
    def override skip_file ( fname:string ) : bool
        if fname |> ends_with("CXErrorCode.h")
            return false
        elif fname |> ends_with("CXString.h")
            return false
        return ! fname |> ends_with(PARSE_FILE_NAME)


[export]
def main
    // generate
    var cgb = new ClangGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
    cgb->genCMakeDecl("DAS_CLANG_BIND")
