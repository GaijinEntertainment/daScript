require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost
require daslib/functional

class LlvmGen : CppGenBind
    override func_to_stdout = false
    override include_local_parse_file = true

    llvm_c_headers <- [{string
    // header version
        "Analysis.h";
        "BitReader.h";
        "BitWriter.h";
        "Comdat.h";
        "Core.h";
        "DataTypes.h";
        "DebugInfo.h";
        "Disassembler.h";
        "DisassemblerTypes.h";
        "Error.h";
        "ErrorHandling.h";
        "ExecutionEngine.h";
        "ExternC.h";
        "Initialization.h";
        "IRReader.h";
        "Linker.h";
        "LLJIT.h";
        "lto.h";
        "Object.h";
        "Orc.h";
        "OrcEE.h";
        "Remarks.h";
        "Support.h";
        "Target.h";
        "TargetMachine.h";
        "Types.h";
    // transforms
        "AggressiveInstCombine.h";
        "Coroutines.h";
        "InstCombine.h";
        "IPO.h";
        "PassBuilder.h";
        "PassManagerBuilder.h";
        "Scalar.h";
        "Utils.h";
        "Vectorize.h";
    // def version
        "abi-breaking.h";
        "AsmParsers.def";
        "AsmPrinters.def";
        "Disassemblers.def";
        "llvm-config.h";
        "Targets.def"
    }]

    def LlvmGen
        bind_root = "{get_das_root()}/Modules/dasLLVM/src"
        bind_module = "dasLLVM"
        bind_das_module = "llvm"
        let pfn = "cb_dasLLVM.h"
        let pfp = "{get_das_root()}/Modules/dasLLVM/src/"
        let clang_i = "{get_das_root()}/../libclang/include/"
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-I{get_full_file_name(pfp)}";
            "-I{get_full_file_name(clang_i)}";
            "-DWIN32";
            "-DCINDEX_EXPORTS"
        }]
        func_per_chunk = 20
        init_args(pfn,pfp,args)
        setDefaultFiles()
//        init_enum_prefix()
        openAllFiles()
        for h in llvm_c_headers
            h = to_upper(h)
/*
    def init_enum_prefix
        enum_prefix <- {{
            "CXIdxEntityRefKind" => "CXIdxEntityRef";
            "CXIndexOptFlags" => "CXIndexOpt"
        }}
*/
    def override open_file_name : string
        return "{PARSE_FILE_PREFIX}{PARSE_FILE_NAME}"
    def override isArgByValue ( name:string )
        return true
    def override skip_file ( fname:string ) : bool
        let ufname = to_upper(fname)
        if ! any <| [[for h in llvm_c_headers; ufname |> ends_with(h)]]
            return true
        return false
    def override skip_alias ( var c : CXCursor )
        let aliasf = to_upper(clang_getCursorLocationFileName(c))
        if ! any <| [[for h in llvm_c_headers; aliasf |> ends_with(h)]]
            return true
        return false
    def override skip_enum ( ns_en, en : string )
        if ns_en == "std::byte"
            return true
        return false
    def override skip_struct ( sn:string )
        if sn |> starts_with("__crt")
            return true
        return false
    def override skip_function ( var c : CXCursor ) : bool
        let function_name = string(clang_getCursorSpelling(c))
        if (function_name == "LLVMConstGEP2" || function_name == "LLVMConstInBoundsGEP2"
            || function_name == "LLVMOrcObjectLayerAddObjectFileWithRT" || function_name == "LLVMInitializeCore")
            return true
        let funf = to_upper(clang_getCursorLocationFileName(c))
        if !any <| [[for h in llvm_c_headers; funf |> ends_with(h)]]
            return true
        if skip_anyFunction(c, false)
            return true
        return false

[export]
def main
    // generate
    var cgb = new LlvmGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
    cgb->genCMakeDecl("DAS_LLVM")
