options gen2
require cbind/cbind_boost
require fio
require daslib/defer
require daslib/strings_boost
require daslib/functional

let private llvm_c_headers <- array<string>(
// header version
    "Analysis.h",
    "BitReader.h",
    "BitWriter.h",
    "Comdat.h",
    "Core.h",
    "DataTypes.h",
    "DebugInfo.h",
    "Disassembler.h",
    "DisassemblerTypes.h",
    "Error.h",
    "ErrorHandling.h",
    "ExecutionEngine.h",
    "ExternC.h",
    "Initialization.h",
    "IRReader.h",
    "Linker.h",
    "LLJIT.h",
    "lto.h",
    "Object.h",
    "Orc.h",
    "OrcEE.h",
    "Remarks.h",
    "Support.h",
    "Target.h",
    "TargetMachine.h",
    "Types.h",
// transforms
    "AggressiveInstCombine.h",
    "Coroutines.h",
    "InstCombine.h",
    "IPO.h",
    "PassBuilder.h",
    "PassManagerBuilder.h",
    "Scalar.h",
    "Utils.h",
    "Vectorize.h",
// def version
    "abi-breaking.h",
    "AsmParsers.def",
    "AsmPrinters.def",
    "Disassemblers.def",
    "llvm-config.h",
    "Targets.def"
)

class LlvmGen : CppGenBind {
    override func_to_stdout = false
    override include_local_parse_file = true

    def LlvmGen(args : array<string>) {
        let pfn = "cb_dasLLVM.h"
        let pfp = "{get_das_root()}/modules/dasLLVM/src/"
        func_per_chunk = 20
        init_args(pfn, pfp, args)
        setDefaultFiles()
//        init_enum_prefix()
        openAllFiles()
    }
/*
    def init_enum_prefix
        enum_prefix <- {{
            "CXIdxEntityRefKind" => "CXIdxEntityRef"
            "CXIndexOptFlags" => "CXIndexOpt"
        }}
*/
    def override open_file_name : string {
        return "{PARSE_FILE_PREFIX}{PARSE_FILE_NAME}"
    }
    def override isArgByValue(name : string) {
        return true
    }
    def override skip_file(fname : string) : bool {
        if (!any <| [iterator for(h in llvm_c_headers); fname |> ends_with(h)]) {
            return true
        }
        return false
    }
    def override skip_alias(var c : CXCursor) {
        let aliasf = clang_getCursorLocationFileName(c)
        if (!any <| [iterator for(h in llvm_c_headers); aliasf |> ends_with(h)]) {
            return true
        }
        return false
    }
    def override skip_enum(ns_en, en : string) {
        if (ns_en == "std::byte" || ns_en |> starts_with("(unnamed enum")) {
            return true
        }
        return false
    }
    def override skip_struct(sn : string) {
        if (sn |> starts_with("__crt")) {
            return true
        }
        return false
    }
    def override skip_function(var c : CXCursor) : bool {
        let function_name = string(clang_getCursorSpelling(c))
        if (function_name == "LLVMConstGEP2" || function_name == "LLVMConstInBoundsGEP2"
            || function_name == "LLVMOrcObjectLayerAddObjectFileWithRT" || function_name == "LLVMInitializeCore") {
            return true
        }
        let funf = clang_getCursorLocationFileName(c)
        if (!any <| [iterator for(h in llvm_c_headers); funf |> ends_with(h)]) {
            return true
        }
        if (skip_anyFunction(c, false)) {
            return true
        }
        return false
    }
}

class LlvmGenDinamic : DasGenBind {
    def LlvmGenDinamic(pfn, pfp : string; args : array<string>) {
        init_args(pfn, pfp, args)
    }
    def override skip_const(name : string) : bool {
        if (name == "true" || name == "false") {
            return true
        }
        return false
    }
    def override skip_alias(var c : CXCursor) {
        let aliasf = clang_getCursorLocationFileName(c)
        let typedef_name = string(clang_getCursorSpelling(c))
        // Let's skip ssize_t, we never use it.
        if (typedef_name == "ssize_t") {
            return true
        }
        if (!any <| [iterator for(h in llvm_c_headers); aliasf |> ends_with(h)]) {
            return true
        }
        return false
    }
    def override skip_function(var c : CXCursor) : bool {
        let function_name = string(clang_getCursorSpelling(c))
        if (function_name == "LLVMInitializeCore" || // Defined twice in LLVM C API
            function_name |> starts_with("LLVMInitializeAll") || // It's static macro function, implemented on das manually
            function_name |> starts_with("LLVMInitializeNative") // It's static macro function, implemented on das manually
            ) {
            return true
        }
        let funf = clang_getCursorLocationFileName(c)
        if (!any <| [iterator for(h in llvm_c_headers); funf |> ends_with(h)]) {
            return true
        }
        if (skip_anyFunction(c, false)) {
            return true
        }
        return false
    }

    def override skip_file(fname : string) : bool {
        if (!any <| [iterator for(h in llvm_c_headers); fname |> ends_with(h)]) {
            return true
        }
        return false
    }
    def override output_dasbind(function_name : string) {
        fprint(func_decl_file, "[extern(cdecl, name=\"{function_name}\", linux_library=\"libLLVM.so\", windows_library=\"LLVM-C\")]\n")
    }
}

[export]
def main {
    let is_dynamic = true
    let clang_i = "C:/Work/win64/include"
    let pfn = "cb_dasLLVM.h"
    let pfp = "{get_das_root()}/modules/dasLLVM/src/"
    let args <- array<string>(
        "-xc++-header",
        "-std=c++1z",
        "-I{get_full_file_name(pfp)}",
        "-I{clang_i}",
        "-DWIN32",
        "-DCINDEX_EXPORTS"
    )
    if (is_dynamic) {
        // generate
        let output_name = "{get_das_root()}/modules/dasLLVM/das_llvm/llvm_func.das"
        fopen(output_name, "wb") <| $(fdf) {
            fprint(fdf, "require dasbind public\n")
            fprint(fdf, "options gen2\n")
            fprint(fdf, "module llvm_func shared\n")
            fprint(fdf, "require llvm/llvm_struct\n")
            fprint(fdf, "require llvm/llvm_enum\n")
            let output_const_name = "{get_das_root()}/modules/dasLLVM/das_llvm/llvm_const.das"
            fopen(output_const_name, "wb") <| $(dcf) {
                var glbind = new LlvmGenDinamic(pfn, pfp, args)
                var scf = glbind.openFile("{get_das_root()}/modules/dasLLVM/das_llvm/llvm_struct.das")
                var ecf = glbind.openFile("{get_das_root()}/modules/dasLLVM/das_llvm/llvm_enum.das")
                fprint(scf, "options gen2\nmodule llvm_struct shared\n")
                fprint(ecf, "options gen2\nmodule llvm_enum shared\n")
                glbind->setFiles(dcf, fdf, scf, ecf)
                glbind->generate()
                unsafe {
                    delete glbind
                }
            }
        }
    } else {
        // generate
        var cgb = new LlvmGen(args)
        defer <| $() {
            unsafe {
                delete cgb
            }
        }
        cgb->generate()
        cgb->genCMakeDecl("DAS_LLVM")
    }
}
