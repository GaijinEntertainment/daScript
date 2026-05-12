options gen2
require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost
require daslib/functional
require daslib/clargs

[CommandLineArgs]
struct Config {
    @clarg_short = "i"
    @clarg_doc = "Path to the llvm-c/ C include tree (default: /usr/lib/llvm-22/include)"
    input : string = "/usr/lib/llvm-22/include"

    @clarg_short = "o"
    @clarg_doc = "Output folder for generated bindings (default: <das_root>/modules/dasLLVM/bindings)"
    output : string = "{get_das_root()}/modules/dasLLVM/bindings"

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

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
    "IRReader.h",
    "Linker.h",
    "LLJIT.h",
    "Object.h",
    "Orc.h",
    "OrcEE.h",
    "Support.h",
    "Target.h",
    "TargetMachine.h",
    "Types.h",
    "PassBuilder.h",
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
            function_name |> starts_with("LLVMInitializeNative")) // It's static macro function, implemented on das manually
        {
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
        fprint(func_decl_file, "[extern(cdecl, name=\"{function_name}\", library=\"LLVM.dll\")]\n")
    }
}

[export]
def main {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "bind_llvm")
        return
    }
    var cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "bind_llvm")
        return
    }
    let is_dynamic = true
    let clang_i = cfg.input
    let pfn = "cb_dasLLVM.h"
    let pfp = "{get_das_root()}/modules/dasLLVM/src/"
    var args <- array<string>(
        "-xc++-header",
        "-std=c++1z",
        "-I{get_full_file_name(pfp)}",
        "-I{clang_i}"
    )
    // -DWIN32 only needed when parsing on Windows where some llvm-c headers gate
    // declarations on it. Skip on POSIX to avoid steering header pickups wrong.
    if (has_env_variable("OS") || has_env_variable("WINDIR")) {
        args |> push("-DWIN32")
    }
    args |> push("-DCINDEX_EXPORTS")
    if (is_dynamic) {
        // generate
        let output_name = "{cfg.output}/llvm_func.das"
        fopen(output_name, "wb") <| $(fdf) {
            fprint(fdf, "require dasbind public\n")
            fprint(fdf, "options gen2\n")
            fprint(fdf, "module llvm_func shared\n")
            fprint(fdf, "require llvm/bindings/llvm_struct\n")
            fprint(fdf, "require llvm/bindings/llvm_enum\n")
            let output_const_name = "{cfg.output}/llvm_const.das"
            fopen(output_const_name, "wb") <| $(dcf) {
                var glbind = new LlvmGenDinamic(pfn, pfp, args)
                var scf = glbind.openFile("{cfg.output}/llvm_struct.das")
                var ecf = glbind.openFile("{cfg.output}/llvm_enum.das")
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
        defer() {
            unsafe {
                delete cgb
            }
        }
        cgb->generate()
        cgb->genCMakeDecl("DAS_LLVM")
    }
}
