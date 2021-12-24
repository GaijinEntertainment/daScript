require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class BgfxGen : CppGenBind
    override func_to_stdout = false
    override generate_constant_decl = true
    override const_uint8_cpp_type = "uint32_t"
    override const_uint16_cpp_type = "uint32_t"
    bgfx_skip_func : table<string;bool>
    bgfx_skip_struct : table<string;bool>
    def BgfxGen
        bind_root = "{get_das_root()}/Modules/dasBGFX/src"
        bind_module = "dasBGFX"
        bind_das_module = "bgfx"
        let pfn = "bgfx/c99/bgfx.h"
        let pfp = "{get_das_root()}/Modules/dasBGFX/bgfx/bgfx/include/"
        let inc_bx = "{get_das_root()}/Modules/dasBGFX/bgfx/bx/include/"
        CONST_FILE_NAMES <- [{string
            "{pfp}{pfn}";
            "{pfp}bgfx/defines.h"
        }]
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-I{get_full_file_name(pfp)}";
            "-I{get_full_file_name(inc_bx)}"
        }]
        func_per_chunk = 20
        init_args(pfn,pfp,args)
        setDefaultFiles()
        init_skip()
        openAllFiles()
    def init_skip
        bgfx_skip_func <- {{
            "bgfx_get_interface" => true;
            "bgfx_dbg_text_vprintf" => true;
            "bgfx_dbg_text_printf" => true
        }}
        bgfx_skip_struct <- {{
            "bgfx_callback_vtbl_s" => true;
            "bgfx_interface_vtbl" => true;
            "bgfx_callback_interface_s" => true;
            "bgfx_allocator_interface_s" => true;
            "bgfx_allocator_vtbl_s" => true
        }}
    def override skip_const ( name:string ) : bool
        if name |> starts_with("GL_")
            return true
        return false
    def override skip_struct ( name : string )
        return bgfx_skip_struct |> key_exists(name)
    def override isArgByValue ( name:string )
        return true
    def override skip_function ( var c : CXCursor )
        if AnyGenBind`skip_function(self, c)
            return true
        let function_name = string(clang_getCursorSpelling(c))
        if bgfx_skip_func |> key_exists(function_name)
            to_log(LOG_INFO, "skipping BGFX blocked function {function_name} at {clang_getCursorLocationDescription(c)}")
            return true
        if function_name |> starts_with("bgfx")
            return false
        else
            to_log(LOG_INFO, "skipping non BGFX function {function_name} at {clang_getCursorLocationDescription(c)}")
            return true
    def override skip_file ( fname:string ) : bool
        if fname|>ends_with("bgfx.h") || fname|>ends_with("defines.h")
            return false
        return ! fname |> ends_with(PARSE_FILE_NAME)
    def override skip_alias ( var c : CXCursor )
        return false

[export]
def main
    // generate
    var cgb = new BgfxGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
    cgb->genCMakeDecl("DAS_BGFX_BIND")
