options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module cbind_boost shared

require cbind public
require daslib/strings_boost
require daslib/regex_boost
require daslib/defer
require fio

// CXTranslationUnit
let CXTranslationUnit_None = 0x0
let CXTranslationUnit_DetailedPreprocessingRecord = 0x01
let CXTranslationUnit_Incomplete = 0x02
let CXTranslationUnit_PrecompiledPreamble = 0x04
let CXTranslationUnit_CacheCompletionResults = 0x08
let CXTranslationUnit_ForSerialization = 0x10
let CXTranslationUnit_CXXChainedPCH = 0x20
let CXTranslationUnit_SkipFunctionBodies = 0x40
let CXTranslationUnit_IncludeBriefCommentsInCodeCompletion = 0x80
let CXTranslationUnit_CreatePreambleOnFirstParse = 0x100
let CXTranslationUnit_KeepGoing = 0x200
let CXTranslationUnit_SingleFileParse = 0x400
let CXTranslationUnit_LimitSkipFunctionBodiesToPreamble = 0x800
let CXTranslationUnit_IncludeAttributedTypes = 0x1000
let CXTranslationUnit_VisitImplicitAttributes = 0x2000
let CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles = 0x4000
let CXTranslationUnit_RetainExcludedConditionalBlocks = 0x8000

def describe ( var c:CXCursor )
    var sp = clang_getCursorSpelling(c)
    var kd = clang_getCursorKindSpelling(clang_getCursorKind(c))
    return "<{string(sp)}>:`{string(kd)}`"

bitfield TypeRules
    top_level_uint8_is_bool             // uint8 is bool
    char_ptr_is_string                  // char* is string
    uchar_ptr_is_string                 // uchar* is string
    function_proto_is_void_ptr          // and blah ( * ) ( blah, blah ) is void?
    structure_ptr_is_void_ptr           // for any structure Foo * output void?
    top_level_pointers_are_implicit     // for any pointer make it implicit

def clang_typeToDasTypeUnqEx ( var t:CXType; topLevel:bool; rules:TypeRules; var error:string& )
    if t.kind==CXTypeKind CXType_Void
        return "void"
    elif t.kind==CXTypeKind CXType_Bool
        return "bool"
    elif t.kind==CXTypeKind CXType_Char_U || t.kind==CXTypeKind CXType_UChar
        if topLevel && rules.top_level_uint8_is_bool
            return "bool"
        return "uint8"
    elif t.kind==CXTypeKind CXType_Char_S || t.kind==CXTypeKind CXType_SChar
        return "int8"
    elif t.kind==CXTypeKind CXType_Char16 || t.kind==CXTypeKind CXType_Short
        return "int16"
    elif t.kind==CXTypeKind CXType_WChar || t.kind==CXTypeKind CXType_UShort
        return "uint16"
    elif t.kind==CXTypeKind CXType_Char32 || t.kind==CXTypeKind CXType_Int
        return "int"
    elif t.kind==CXTypeKind CXType_UInt
        return "uint"
    elif t.kind==CXTypeKind CXType_Long || t.kind==CXTypeKind CXType_LongLong
        return "int64"
    elif t.kind==CXTypeKind CXType_ULong || t.kind==CXTypeKind CXType_ULongLong
        return "uint64"
    elif t.kind==CXTypeKind CXType_Float
        return "float"
    elif t.kind==CXTypeKind CXType_Double
        return "double"
    elif t.kind==CXTypeKind CXType_NullPtr
        return "void?"
    elif t.kind==CXTypeKind CXType_Pointer
        var pt = clang_getPointeeType(t)
        var rest : string
        if rules.char_ptr_is_string && (pt.kind==CXTypeKind CXType_Char_S || pt.kind==CXTypeKind CXType_SChar)
            rest = "string"
        elif rules.uchar_ptr_is_string && (pt.kind==CXTypeKind CXType_Char_U || pt.kind==CXTypeKind CXType_UChar)
            rest = "string"
        elif rules.structure_ptr_is_void_ptr && pt.kind==CXTypeKind CXType_Record
            rest = "void?"
        else
            rest = "{clang_typeToDasTypeEx(pt,false,rules,error)}?"
        if topLevel && rules.top_level_pointers_are_implicit
            return "{rest} implicit"
        else
            return rest
    elif t.kind==CXTypeKind CXType_Record
        let rname = string(clang_getTypeSpelling(t))
        if rname |> starts_with("struct ")
            return rname |> slice(7)
        else
            error += "unsupported CXXRecord name {rname}\n"
            return "\u00BF"
    elif t.kind==CXTypeKind CXType_FunctionProto
        if rules.function_proto_is_void_ptr
            return "void?"
        else
            error += "unsupported CXType_FunctionProto\n"
            return "\u00BF"
    else
        error += "unsupported type {t.kind}\n"
        return "\u00BF"

def clang_typeToDasTypeEx ( var t:CXType; topLevel:bool; rules:TypeRules; var error:string& )
    var unqType = clang_typeToDasTypeUnqEx(t, topLevel, rules, error )
    if clang_isConstQualifiedType(t)!=0u
        unqType += " const"
    return unqType

def clang_typeToDasType ( var t:CXType; rules:TypeRules )
    var err : string
    let result = clang_typeToDasTypeEx(t,true,rules,err)
    if err!=""
        to_log(LOG_ERROR, "\nTYPE ERROR:\n{err}")
    return result

let private unsafe_names <- {{
    "type"      =>  "_type";
    "string"    =>  "_string";
    "array"     =>  "_array";
    "range"     =>  "_range";
    "label"     =>  "_label"
}}

def clang_argNameToDasArgName ( name:string; ai:uint )
    if name==""
        return "_argument_{ai}"
    else
        return unsafe_names?[name] ?? name

// handling defines

var blocked_defines : array<string> <- [{string []
    "true";
    "false"
}]

var private blocked_defines_table : table<string;bool>

var is_const_blocked : lambda<(name:string):bool>

var const_type_table : table<string; string>

def genDefineConstants(fname,prefix:string)
    var fnames <- [{ auto[] fname }]
    defer <| { delete fnames; }
    fnames |> genDefineConstants <| prefix

def getGenConstType ( name, default_name:string )
    return const_type_table?[name] ?? default_name

def searchAndGenConst ( var regex_def:Regex; defTName,suffix:string; hex:bool; var ofs:table<int; bool>; data:string; var dup:table<string;bool>; hf:FILE const? )
    regex_foreach(regex_def,data) <| $ ( r )
        if ! ofs |> key_exists(r.x)
            let DEF = regex_group(regex_def,1,data)
            let VAL = regex_group(regex_def,2,data)
            if DEF |> starts_with("__")
                to_log(LOG_WARNING, "skipping {DEF} due to name limitations")
                return true
            var suf = suffix
            if hex && suffix=="u"
                let v = to_uint64(VAL,true)
                if v > 0xfffffffful
                    to_log(LOG_INFO, "{DEF} promoted to ul")
                    suf = "ul"
            if !blocked_defines_table |> key_exists(DEF)
                if is_const_blocked==null || !invoke(is_const_blocked,DEF)
                    let ctt = getGenConstType(DEF,defTName)
                    if !dup[DEF]
                        fprint(hf,"let {DEF} = {ctt}({VAL}{suf})\n")
                        dup[DEF] = true
        return true

def genDefineConstants(fnames:array<string>; gen_file:string)
    delete blocked_defines_table
    for bd in blocked_defines
        blocked_defines_table[bd] = true
    var reg_def_hex <- %regex~#define\s+(\w+)\s+(0x[0-9A-Fa-f]+)%%
    var reg_def_dec <- %regex~#define\s+(\w+)\s+(\d+)%%
    var reg_def_UINT8 <- %regex~#define\s+(\w+)\s+UINT8_C\s*\((0x[0-9A-Fa-f]+)\)%%
    var reg_def_UINT16 <- %regex~#define\s+(\w+)\s+UINT16_C\s*\((0x[0-9A-Fa-f]+)\)%%
    var reg_def_UINT32 <- %regex~#define\s+(\w+)\s+UINT32_C\s*\((0x[0-9A-Fa-f]+)\)%%
    var reg_def_UINT64 <- %regex~#define\s+(\w+)\s+UINT64_C\s*\((0x[0-9A-Fa-f]+)\)%%
    fopen(gen_file,"wb") <| $(hf)
        if hf==null
            panic("can't write {gen_file}")
        for fname in fnames
            fopen(fname,"rb") <| $( f )
                if f == null
                    panic("can't open {fname}")
                var ofs : table<int; bool>
                let data = fread(f)
                var dup : table<string; bool>
                searchAndGenConst(reg_def_hex, "uint", "u", true, ofs, data, dup, hf)
                searchAndGenConst(reg_def_dec, "int", "", false, ofs, data, dup, hf)
                searchAndGenConst(reg_def_UINT8, "uint8", "u", true, ofs, data, dup, hf)
                searchAndGenConst(reg_def_UINT16, "uint16", "u", true, ofs, data, dup, hf)
                searchAndGenConst(reg_def_UINT32, "uint", "u", true, ofs, data, dup, hf)
                searchAndGenConst(reg_def_UINT64, "uint64", "ul", true, ofs, data, dup, hf)