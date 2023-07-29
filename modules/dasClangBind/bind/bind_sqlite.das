require cbind/cbind_boost
require fio
require daslib/defer
require daslib/strings_boost
require daslib/functional

class SqliteGen : CppGenBind
    override func_to_stdout = false
    override include_local_parse_file = true
    override generate_constant_decl = true

    sqlite_c_headers <- [{string
        "sqlite3.h"
    }]

    def SqliteGen
        bind_root = "{get_das_root()}/Modules/dasSQLITE/src"
        bind_module = "dasSQLITE"
        bind_das_module = "sqlite"
        let pfn = "cb_dasSQLITE.h"
        let pfp = "{get_das_root()}/Modules/dasSQLITE/src/"
        let sqlite_i = "{get_das_root()}/Modules/dasSQLITE/sqlite/"
        CONST_FILE_NAMES <- [{string
            "{sqlite_i}sqlite3.h"
        }]
        let args <- [{string
            "-xc-header";
            "-I{get_full_file_name(pfp)}";
            "-I{get_full_file_name(sqlite_i)}";
            "-DWIN32";
            "-DCINDEX_EXPORTS"
        }]
        func_per_chunk = 20
        init_args(pfn,pfp,args)
        setDefaultFiles()
//        init_enum_prefix()
        openAllFiles()
        for h in sqlite_c_headers
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
        if ! any <| [[for h in sqlite_c_headers; ufname |> ends_with(h)]]
            return true
        return false
    def override skip_alias ( var c : CXCursor )
        let tdn = string(clang_getCursorDisplayName(c))
        var tdt = clang_getCanonicalType(clang_getCursorType(c))
        let tdts = string(clang_getTypeSpelling(tdt))
        if "struct {tdn}"==tdts
            return true
        let aliasf = to_upper(clang_getCursorLocationFileName(c))
        if ! any <| [[for h in sqlite_c_headers; aliasf |> ends_with(h)]]
            return true
        return false
    def override skip_enum ( ns_en, en : string )
        return false
    def override skip_struct ( sn:string )
        if sn |> starts_with("__crt")
            return true
        return false
    def override skip_function ( var c : CXCursor ) : bool
        let fname = string(clang_getCursorSpelling(c))
        if fname=="sqlite3_mutex_held" || fname=="sqlite3_mutex_notheld"
            return true
        if fname |> starts_with("sqlite3_win32_set_directory")
            return true
        let funf = to_upper(clang_getCursorLocationFileName(c))
        if !any <| [[for h in sqlite_c_headers; funf |> ends_with(h)]]
            return true
        if skip_anyFunction(c, false)
            return true
        return false

[export]
def main
    // generate
    var cgb = new SqliteGen()
    defer <|
        unsafe
            delete cgb
    cgb->generate()
    cgb->genCMakeDecl("DAS_SQLITE")
