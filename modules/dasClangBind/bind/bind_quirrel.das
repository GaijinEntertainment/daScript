require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost
require daslib/functional

class QuirrelGen : CppGenBind
    override func_to_stdout = false
    override include_local_parse_file = true

    quirrel_c_headers <- [{string
        "squirrel.h";
        "sqstdaux.h";
        "sqstdblob.h";
        "sqstdio.h";
        "sqstdmath.h";
        "sqstdstring.h";
        "sqstdsystem.h"
    }]

    def QuirrelGen
        bind_root = "{get_das_root()}/Modules/dasQuirrel/src"
        bind_module = "dasQUIRREL"
        bind_das_module = "quirrel"
        let pfn = "cb_dasQuirrel.h"
        let pfp = "{get_das_root()}/Modules/dasQuirrel/src/"
        let quirrel_i = "{get_das_root()}/Modules/dasQuirrel/libquirrel/include/"
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-I{get_full_file_name(pfp)}";
            "-I{get_full_file_name(quirrel_i)}";
            "-DWIN32";
            "-DCINDEX_EXPORTS"
        }]
        func_per_chunk = 20
        init_args(pfn,pfp,args)
        setDefaultFiles()
//        init_enum_prefix()
        openAllFiles()
        for h in quirrel_c_headers
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
        if ! any <| [[for h in quirrel_c_headers; ufname |> ends_with(h)]]
            return true
        return false
    def override skip_alias ( var c : CXCursor )
        let aliasn = string(clang_getCursorSpelling(c))
        if aliasn=="SQPRINTFUNCTION"
            return true
        let aliasf = to_upper(clang_getCursorLocationFileName(c))
        if ! any <| [[for h in quirrel_c_headers; aliasf |> ends_with(h)]]
            return true
        return false
    def override skip_enum ( ns_en, en : string )
        return false
    def override skip_struct ( sn:string )
        if sn |> starts_with("__crt")
            return true
        return false
    def override skip_function ( var c : CXCursor ) : bool
    /*
        let function_name = string(clang_getCursorSpelling(c))
        if (function_name == "QUIRRELConstGEP2" || function_name == "QUIRRELConstInBoundsGEP2"
            || function_name == "QUIRRELOrcObjectLayerAddObjectFileWithRT" || function_name == "QUIRRELInitializeCore")
            return true
    */
        let funf = to_upper(clang_getCursorLocationFileName(c))
        if !any <| [[for h in quirrel_c_headers; funf |> ends_with(h)]]
            return true
        if skip_anyFunction(c, false)
            return true
        return false

[export]
def main
    // generate
    var cgb = new QuirrelGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
    cgb->genCMakeDecl("DAS_QUIRREL")
