require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class SFMLGen : CppGenBind
    override generate_lsp = true
    override func_to_stdout = false
    override generate_constant_decl = false
    xbyak_skip_func : table<string;bool>
    enum2sfmlenum : table<string;string>
    sfml_skip_func : table<string;bool>
    def SFMLGen
        bind_root = "{get_das_root()}/Modules/dasSFML/src"
        bind_module = "dasSFML"
        bind_das_module = "sfml"
        let pfn = "sfml_stub.h"
        let pfp = "{get_das_root()}/Modules/dasSFML/src/"
        let isfml = "{get_das_root()}/Modules/dasSFML/SFML/include/"
        /*
        CONST_FILE_NAMES <- [{string
            "{pfp}{pfn}"
        }]
        */
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-DSFML_NO_EXCEPTION";
            "-I{get_full_file_name(pfp)}";
            "-I{get_full_file_name(isfml)}"
        }]
        func_per_chunk = 100
        init_args(pfn,pfp,args)
        setDefaultFiles()
        init_skip()
        openAllFiles()
    def init_skip
        aot_alias <- {{
            "sf::Vector2u"  => true;
            "sf::Vector2i"  => true;
            "sf::Vector2f"  => true;
            "sf::String"    => true;
            "sf::FloatRect" => true;
            "sf::IntRect"   => true
        }}
        enum2sfmlenum <- {{
            "sf::Sensor::Type"  => "SensorType";
            "sf::Cursor::Type"  => "CursorType"
        }}
        sfml_skip_func <- {{
            "sf::err"   => true;
            "sf::Texture::copyToImage" => true;
            "sf::RenderWindow::capture" => true;
            "sf::Joystick::getIdentification" => true
        }}
    def override isArgByValue ( name:string )
        return true
    def override skip_file ( fname:string ) : bool
        // print("fname = {fname}\n")
        if fname |> find("SFML/") != -1
            return false
        return ! fname |> ends_with(PARSE_FILE_NAME)
    def override dasEnumName(enum_name,cpp_enum_name:string) : string
        return enum2sfmlenum?[cpp_enum_name] ?? CppGenBind`dasEnumName(self,enum_name,cpp_enum_name)
    def override generateModuleHPrefix
        module_h_file |> fwrite("#include \"cb_dasSFML.h\"\n")
    def override skip_anyFunction ( var c : CXCursor; isMethod:bool ) : bool
        if AnyGenBind`skip_anyFunction(self, c, isMethod)
            return true
        let function_name = namespace_name(string(clang_getCursorSpelling(c)))
        if sfml_skip_func |> key_exists(function_name)
            to_log(LOG_INFO, "skipping SFML blocked function {function_name} at {clang_getCursorLocationDescription(c)}")
            return true
        return false
    def override skip_struct ( name : string )
        if name=="HWND__"
            return true
        return false



[export]
def main
    // generate
    var cgb = new SFMLGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
    cgb->genCMakeDecl("DAS_SFML_BIND")
