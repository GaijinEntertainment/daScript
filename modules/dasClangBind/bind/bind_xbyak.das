require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class XbyakGen : CppGenBind
    override generate_lsp = true
    override func_to_stdout = false
    override generate_constant_decl = false
    xbyak_skip_func : table<string;bool>
    def XbyakGen
        bind_root = "{get_das_root()}/Modules/dasXbyak/src"
        bind_module = "dasXBYAK"
        bind_das_module = "xbyak"
        let pfn = "xbyak/xbyak.h"
        let pfp = "{get_das_root()}/Modules/dasXbyak/xbyak/"
        CONST_FILE_NAMES <- [{string
            "{pfp}{pfn}"
        }]
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-DXBYAK_NO_EXCEPTION";
            "-I{get_full_file_name(pfp)}"
        }]
        func_per_chunk = 100
        init_args(pfn,pfp,args)
        setDefaultFiles()
        init_skip()
        openAllFiles()
    def override generateModuleNeedHPrefix
        module_need_h_file |> fwrite("#define XBYAK_NO_EXCEPTION\n")
    def init_skip
        xbyak_skip_func <- {{
            "Xbyak::CodeGenerator::L" => true;
            "Xbyak::Label::toStr" => true
        }}
    def override isArgByValue ( name:string )
        return true
    def override skip_anyFunction ( var c : CXCursor; isMethod:bool )
        if AnyGenBind`skip_anyFunction(self, c, isMethod)
            return true
        let function_name = string(clang_getCursorSpelling(c))
        let function_cpp_name = namespace_name(function_name)
        if xbyak_skip_func |> key_exists(function_cpp_name)
            to_log(LOG_INFO, "skipping XBYAK blocked function {function_cpp_name} at {clang_getCursorLocationDescription(c)}")
            return true
        return false
    def override skip_file ( fname:string ) : bool
        if fname|>ends_with("xbyak_mnemonic.h") || fname|>ends_with("xbyak.h")
            return false
        return ! fname |> ends_with(PARSE_FILE_NAME)

[export]
def main
    // generate
    var cgb = new XbyakGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
    cgb->genCMakeDecl("DAS_XBYAK_BIND")
