options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module cbind_boost shared

require cbind public
require daslib/strings_boost
require daslib/regex_boost
require daslib/defer
require daslib/safe_addr
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
    if t.kind==CXTypeKind Void
        return "void"
    elif t.kind==CXTypeKind Bool
        return "bool"
    elif t.kind==CXTypeKind Char_U || t.kind==CXTypeKind UChar
        if topLevel && rules.top_level_uint8_is_bool
            return "bool"
        return "uint8"
    elif t.kind==CXTypeKind Char_S || t.kind==CXTypeKind SChar
        return "int8"
    elif t.kind==CXTypeKind Char16 || t.kind==CXTypeKind Short
        return "int16"
    elif t.kind==CXTypeKind WChar || t.kind==CXTypeKind UShort
        return "uint16"
    elif t.kind==CXTypeKind Char32 || t.kind==CXTypeKind Int
        return "int"
    elif t.kind==CXTypeKind UInt
        return "uint"
    elif t.kind==CXTypeKind Long || t.kind==CXTypeKind LongLong
        return "int64"
    elif t.kind==CXTypeKind ULong || t.kind==CXTypeKind ULongLong
        return "uint64"
    elif t.kind==CXTypeKind Float
        return "float"
    elif t.kind==CXTypeKind Double
        return "double"
    elif t.kind==CXTypeKind NullPtr
        return "void?"
    elif t.kind==CXTypeKind Pointer
        var pt = clang_getPointeeType(t)
        var rest : string
        if rules.char_ptr_is_string && (pt.kind==CXTypeKind Char_S || pt.kind==CXTypeKind SChar)
            rest = "string"
        elif rules.uchar_ptr_is_string && (pt.kind==CXTypeKind Char_U || pt.kind==CXTypeKind UChar)
            rest = "string"
        elif rules.structure_ptr_is_void_ptr && pt.kind==CXTypeKind Record
            rest = "void?"
        else
            rest = "{clang_typeToDasTypeEx(pt,false,rules,error)}?"
        if topLevel && rules.top_level_pointers_are_implicit
            return "{rest} implicit"
        else
            return rest
    elif t.kind==CXTypeKind Record
        let rname = string(clang_getTypeSpelling(t))
        if rname |> starts_with("struct ")
            return rname |> slice(7)
        elif rname |> starts_with("class ")
            return rname |> slice(7)
        else
            error += "unsupported CXXRecord name {rname}\n"
            return "\u00BF"
    elif t.kind==CXTypeKind FunctionProto
        if rules.function_proto_is_void_ptr
            return "void?"
        else
            error += "unsupported FunctionProto\n"
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

def clang_getCursorLocationDescription ( var c:CXCursor )
    var file : CXFile
    var line, column, offset : uint
    clang_getSpellingLocation(clang_getCursorLocation(c), safe_addr(file), safe_addr(line), safe_addr(column), safe_addr(offset))
    var fname = clang_getFileName(file)
    return "{string(fname)}:{int(line)}:{int(column)}"

def clang_isForwardDeclaration ( var cursor:CXCursor )
    var definition = clang_getCursorDefinition(cursor)
    if clang_equalCursors(definition, clang_getNullCursor())!=0u
        return true
    return clang_equalCursors(cursor, definition)==0u

def clang_das_isRef ( var c : CXType )
    if c.kind == CXTypeKind LValueReference
        return true
    return false

def clang_das_isCopyOnReturn ( var c : CXType )
    if c.kind == CXTypeKind Record
        return true
    return false

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

class AnyGenBind
    PARSE_FILE_NAME : string
    PARSE_FILE_PREFIX : string
    CONST_FILE_NAMES : array<string>
    ARGV : array<string>
    prevFileName : string
    nspaces : array<string>
    emptyStruct : CXCursor
    hasEmptyStruct : bool
    emptyEnum : CXCursor
    hasEmptyEnum : bool
    const_type_table : table<string; string>
    define_const_file : FILE const?
    define_const_style_das = true
    enum2enum : table<string; string>
    const_uint8_cpp_type = "uint8_t"
    const_uint16_cpp_type = "uint16_t"
    const_uint8_type = "uint8"
    const_uint16_type = "uint16"
    def AnyGenBind ( pfn,pfp:string; args:array<string> )
        init_args(pfn,pfp,args )
    def init_args ( pfn,pfp:string; args:array<string> )
        PARSE_FILE_NAME = pfn
        PARSE_FILE_PREFIX = pfp
        ARGV := args
    def open_file_name : string
        return "{PARSE_FILE_PREFIX}{PARSE_FILE_NAME}"
    def skip_file ( fname:string ) : bool
        return ! fname |> ends_with(PARSE_FILE_NAME)
    def generate
        var index = clang_createIndex(0,1)
        var unit = clang_parseTranslationUnit(
            index,
            open_file_name(),
            unsafe(addr(ARGV[0])), length(ARGV),
            null, 0u,
            CXTranslationUnit_None)
        if unit==null
            panic("unable to parse translation unit {open_file_name()}")
        var cursor = clang_getTranslationUnitCursor(unit)
        parse(cursor)
        clang_disposeTranslationUnit(unit)
        clang_disposeIndex(index)
    def namespace_name ( name:string ) : string
        return build_string <| $ ( wr )
            for n in nspaces
                wr |> write(n)
                wr |> write("::")
            wr |> write(name)
    def skip_function ( var c : CXCursor )
        let function_name = string(clang_getCursorSpelling(c))
        var fun_type = clang_getCursorType(c)
        let narg = clang_Cursor_getNumArguments(c)
        for ai in urange(narg)
            var carg = clang_Cursor_getArgument(c, ai)
            var arg_type = clang_getCanonicalType(clang_getCursorType(carg))
            if skip_type(arg_type)
                let arg_type_name = string(clang_getTypeSpelling(arg_type))
                let arg_name = string(clang_getCursorSpelling(carg))
                to_log(LOG_INFO,"skipping function {function_name} due to argument {arg_name} type {arg_type_name} at {clang_getCursorLocationDescription(c)}")
                return true
        var res_type = clang_getCanonicalType(clang_getResultType(fun_type))
        if skip_type(res_type)
            let res_type_name = string(clang_getTypeSpelling(res_type))
            to_log(LOG_INFO,"skipping function {function_name} due to result type {res_type_name} at {clang_getCursorLocationDescription(c)}")
            return true
        return false
    def skip_struct ( name : string )
        return false
    def skip_type ( var c : CXType )
        if c.kind == CXTypeKind Pointer
            var pc = clang_getPointeeType(c)
            if pc.kind==CXTypeKind FunctionProto
                return true
            return skip_type(pc)
        elif c.kind == CXTypeKind Record
            var rname = string(clang_getTypeSpelling(c))
            if rname |> starts_with("struct ")
                rname = rname |> slice(7)
            elif rname |> starts_with("class ")
                rname = rname |> slice(6)
            if skip_struct(rname)
                return true
        return false
    def parse_FunctionDecl ( var c : CXCursor )
        pass
    def parse_Namespace ( var c : CXCursor )
        let ns = string(clang_getCursorDisplayName(c))
        nspaces |> push(ns)
        parse(c)
        nspaces |> pop
    def parse_Enum ( var cursor : CXCursor; enum_name : string )
        pass
    def parse_Struct ( var c : CXCursor; struct_name : string )
        nspaces |> push(struct_name)
        parse(c)
        nspaces |> pop
    def parse_Field ( var c : CXCursor )
        pass
    def parse_Alias ( var c : CXCursor )
        pass
    def parse ( var cursor : CXCursor )
        clang_visitChildren(cursor) <| $ ( var c,parent )
            var file : CXFile
            var line, column, offset : uint
            clang_getSpellingLocation(clang_getCursorLocation(c), safe_addr(file), safe_addr(line), safe_addr(column), safe_addr(offset))
            var fname = clang_getFileName(file)
            peek(fname) <| $ ( fn )
                if prevFileName != fn
                    prevFileName := fn
                    if skip_file(prevFileName)
                        to_log(LOG_INFO, "skipping {prevFileName}")
            if !skip_file(prevFileName)
                let kind = clang_getCursorKind(c)
                if kind == CXCursorKind TypedefDecl
                    let tdn = string(clang_getCursorDisplayName(c))
                    var tdt = clang_getCanonicalType(clang_getCursorType(c))
                    if hasEmptyStruct
                        if tdt.kind == CXTypeKind Record
                            if !skip_struct(tdn)
                                parse_Struct(emptyStruct, tdn)
                            hasEmptyStruct = false
                            return CXChildVisitResult Continue
                        else
                            to_log(LOG_ERROR, "typedef of type {tdt.kind} with nameless structure at {clang_getCursorLocationDescription(c)}")
                            hasEmptyStruct = false
                    elif hasEmptyEnum
                        if tdt.kind == CXTypeKind Enum
                            parse_Enum(emptyEnum, tdn)
                            hasEmptyEnum = false
                            return CXChildVisitResult Continue
                        else
                            to_log(LOG_ERROR, "typedef of type {tdt.kind} with nameless enumeration at {clang_getCursorLocationDescription(c)}")
                            hasEmptyEnum = false
                    elif tdt.kind == CXTypeKind Record
                        let sn = string(clang_getTypeSpelling(tdt))
                        if sn == tdn
                            to_log(LOG_INFO, "skipping typedef struct {sn} {tdn};")
                            return CXChildVisitResult Continue
                        if skip_struct(sn) || skip_struct(tdn)
                            to_log(LOG_INFO, "skipping typedef struct {sn} {tdn};")
                            return CXChildVisitResult Continue
                    elif tdt.kind == CXTypeKind Enum
                        let sn = string(clang_getTypeSpelling(tdt))
                        if sn == tdn
                            to_log(LOG_INFO, "skipping typedef enum {sn} {tdn};")
                            return CXChildVisitResult Continue
                        else
                            enum2enum[namespace_name(sn)] = tdn
                            return CXChildVisitResult Continue
                    parse_Alias(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind FunctionDecl
                    if !skip_function(c)
                        parse_FunctionDecl(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind StructDecl || kind == CXCursorKind ClassDecl
                    let struct_name = string(clang_getCursorSpelling(c))
                    if empty(struct_name)
                        if hasEmptyStruct
                            to_log(LOG_ERROR, "already in empty struct mode from {clang_getCursorLocationDescription(emptyStruct)}, at {clang_getCursorLocationDescription(c)}")
                        emptyStruct = c
                        hasEmptyStruct = true
                    else
                        if !skip_struct(struct_name)
                            parse_Struct(c, struct_name)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind Namespace
                    parse_Namespace(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind EnumDecl
                    let enum_name = string(clang_getCursorSpelling(c))
                    if empty(enum_name)
                        if hasEmptyEnum
                            to_log(LOG_ERROR, "already in empty enum mode from {clang_getCursorLocationDescription(emptyEnum)}, at {clang_getCursorLocationDescription(c)}")
                        emptyEnum = c
                        hasEmptyEnum = true
                    else
                        parse_Enum(c, enum_name)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind FieldDecl
                    let acs = clang_getCXXAccessSpecifier(c)
                    if acs != CX_CXXAccessSpecifier Public
                        let field_name = string(clang_getCursorSpelling(c))
                        to_log(LOG_INFO, "skipping {namespace_name(field_name)} due to access specifier {acs}")
                    else
                        var field_type = clang_getCanonicalType(clang_getCursorType(c))
                        if skip_type(field_type)
                            let field_name = string(clang_getCursorSpelling(c))
                            let field_type_name = string(clang_getTypeSpelling(field_type))
                            to_log(LOG_INFO, "skipping {namespace_name(field_name)} due to type {field_type_name}")
                        else
                            parse_Field(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind VarDecl
                    let var_decl_name = string(clang_getCursorSpelling(c))
                    var var_decl_type = clang_getCanonicalType(clang_getCursorType(c))
                    let var_decl_type_name = string(clang_getTypeSpelling(var_decl_type))
                    to_log(LOG_WARNING, "VarDecl {var_decl_name}:{var_decl_type_name} at {clang_getCursorLocationDescription(c)}")
                    return CXChildVisitResult Recurse
                elif kind == CXCursorKind UnexposedDecl
                    // nada?
                    return CXChildVisitResult Recurse
                else
                    to_log(LOG_WARNING, "unsupported {c.kind}: {describe(c)} at {clang_getCursorLocationDescription(c)}")
                return CXChildVisitResult Recurse
            else
                return CXChildVisitResult Recurse
    def skip_const ( name:string ) : bool
        return false
    def getGenConstType ( name, default_name:string )
        return const_type_table?[name] ?? default_name
    def searchAndGenConst ( var regex_def:Regex; defTName,suffix:string; hex:bool; var ofs:table<int; bool>; data:string; var dup:table<string;bool> )
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
                if !skip_const(DEF)
                    let ctt = getGenConstType(DEF,defTName)
                    if !dup[DEF]
                        if define_const_style_das
                            fprint(define_const_file,"let {DEF} = {ctt}({VAL}{suf})\n")
                        else
                            fprint(define_const_file,"addConstant<{ctt}>(*this,\"{DEF}\",{ctt}({VAL}{suf}));\n")
                        dup[DEF] = true
            return true
    def generateConstantsFrom(fnames:array<string>)
        var reg_def_hex <- %regex~#define\s+(\w+)\s+(0x[0-9A-Fa-f]+)%%
        var reg_def_dec <- %regex~#define\s+(\w+)\s+(\d+)%%
        var reg_def_UINT8 <- %regex~#define\s+(\w+)\s+UINT8_C\s*\((0x[0-9A-Fa-f]+)\)%%
        var reg_def_UINT16 <- %regex~#define\s+(\w+)\s+UINT16_C\s*\((0x[0-9A-Fa-f]+)\)%%
        var reg_def_UINT32 <- %regex~#define\s+(\w+)\s+UINT32_C\s*\((0x[0-9A-Fa-f]+)\)%%
        var reg_def_UINT64 <- %regex~#define\s+(\w+)\s+UINT64_C\s*\((0x[0-9A-Fa-f]+)\)%%
        // for all the files where we need to generate
        for fname in fnames
            fopen(fname,"rb") <| $( f )
                if f == null
                    panic("can't open {fname}")
                var ofs : table<int; bool>
                let data = fread(f)
                var dup : table<string; bool>
                searchAndGenConst(reg_def_hex, define_const_style_das ? "uint" : "uint32_t", "u", true, ofs, data, dup)
                searchAndGenConst(reg_def_dec, define_const_style_das ? "int" : "int32_t", "", false, ofs, data, dup)
                searchAndGenConst(reg_def_UINT8, define_const_style_das ? const_uint8_type : const_uint8_cpp_type, "u", true, ofs, data, dup)
                searchAndGenConst(reg_def_UINT16, define_const_style_das ? const_uint16_type : const_uint16_cpp_type, "u", true, ofs, data, dup)
                searchAndGenConst(reg_def_UINT32, define_const_style_das ? "uint" : "uint32_t", "u", true, ofs, data, dup)
                searchAndGenConst(reg_def_UINT64, define_const_style_das ? "uint64" : "uint64_t", "ul", true, ofs, data, dup)
    def generateConstants(fname:string)
        var fnames <- [{ auto[] fname }]
        defer <| { delete fnames; }
        fnames |> generateConstantsFrom()

class DasGenBind : AnyGenBind
    known_type : table<string; string>
    func_decl_file : FILE const?
    def DasGenBind ( pfn,pfp:string; args:array<string> )
        init_args(pfn,pfp,args)
        define_const_file = fstdout()
        func_decl_file = fstdout()
    def setFiles ( dcf, fdf:FILE const? )
        define_const_file = dcf
        func_decl_file = fdf
    def functionArgumentRules : TypeRules
        return TypeRules char_ptr_is_string | TypeRules function_proto_is_void_ptr | TypeRules top_level_pointers_are_implicit
    def functionResultRules : TypeRules
        return TypeRules char_ptr_is_string | TypeRules function_proto_is_void_ptr
    def clang_knownType ( var nctype:CXType; rules:TypeRules )
        let ncname = string(clang_getTypeSpelling(nctype))
        let tname = known_type?[ncname] ?? ""
        if tname != ""
            return tname
        var ctype = clang_getCanonicalType(nctype)
        return clang_typeToDasType(ctype,rules)
    def output_dasbind ( function_name:string )
        fprint(func_decl_file, "[extern(name=\"{function_name}\")]\n")
    def override parse_FunctionDecl ( var c : CXCursor )
        let function_name = string(clang_getCursorSpelling(c))
        var fun_type = clang_getCursorType(c)
        output_dasbind(function_name)
        fprint(func_decl_file, "def {function_name} ( ")
        let narg = clang_Cursor_getNumArguments(c)
        for ai in urange(narg)
            if ai != 0u
                fprint(func_decl_file, "; ")
            var carg = clang_Cursor_getArgument(c, ai)
            let arg_name = string(clang_getCursorSpelling(carg))
            var arg_type = clang_getCursorType(carg)
            let darg_type = clang_knownType(arg_type,functionArgumentRules())
            let darg_name = clang_argNameToDasArgName(arg_name,ai)
            fprint(func_decl_file, "{darg_name}:{darg_type}")
        fprint(func_decl_file, " ) : ")
        var res_type = clang_getResultType(fun_type)
        let dres_type = clang_knownType(res_type,functionResultRules())
        fprint(func_decl_file, "{dres_type}\n")
        if res_type.kind==CXTypeKind Void
            fprint(func_decl_file, "\tpass\n\n")
        else
            fprint(func_decl_file, "\treturn [[{dres_type}]]\n\n")
    def override generate
        AnyGenBind`generate(self)
        if empty(CONST_FILE_NAMES)
            generateConstants(open_file_name())
        else
            generateConstantsFrom(CONST_FILE_NAMES)


let DEFAULT_CLANG_ARGUMENTS <- [{string
    "-xc++-header";
    "-std=c++1z"
}]

class CppGenBind : AnyGenBind
    override define_const_style_das = false
    enum_decl_file : FILE const?
    enum_class_file : FILE const?
    enum_add_file : FILE const?
    enum_prefix : table<string; string>
    struct_decl_file : FILE const?
    struct_impl_file : FILE const?
    struct_class_file : FILE const?
    struct_add_file : FILE const?
    struct_postadd_file : FILE const?
    struct_cast_file : FILE const?
    alias_add_file : FILE const?
    dummy_add_file : FILE const?
    func_file : FILE const?
    func_file_reg : FILE const?
    func_file_decl : FILE const?
    module_cpp_file : FILE const?
    module_h_file : FILE const?
    module_need_h_file : FILE const?
    func_count : int = INT_MAX-1
    func_per_chunk : int = 100500
    func_chunk_index : int
    func_to_stdout : bool = true
    swrite : array<StringBuilderWriter?>
    struct_ann : table<string; bool>
    bind_root : string
    bind_module : string
    bind_das_module : string
    generate_constant_decl : bool = false
    enumDecl : array<tuple<ns_en:string;enum_name:string>>
    def CppGenBind ( pfn,pfp:string; args:array<string> )
        init_args(pfn,pfp,args)
        setDefaultFiles()
    def setDefaultFiles
        enum_decl_file = fstdout()
        enum_class_file = fstdout()
        enum_add_file = fstdout()
        struct_decl_file = fstdout()
        struct_impl_file = fstdout()
        struct_class_file = fstdout()
        struct_add_file = fstdout()
        struct_postadd_file = fstdout()
        struct_cast_file = fstdout()
        alias_add_file = fstdout()
        dummy_add_file = fstdout()
        func_file_reg = fstdout()
        func_file_decl = fstdout()
        module_cpp_file = fstdout()
        module_h_file = fstdout()
        module_need_h_file = fstdout()
        define_const_file = fstdout()
    def openFile ( path:string )
        var f = fopen(path,"wb")
        if f == null
            to_log(LOG_ERROR, "can't open CppGenBind output at {path}, defaulting to stdout")
            f = fstdout()
        f |> fwrite("// this file is generated via daScript automatic C++ binder\n")
        f |> fwrite("// all user modifications will be lost after this file is re-generated\n\n")
        return f
    def closeFile ( var f:FILE const? & )
        if f != null && f != fstdout()
            fclose(f)
        f = null
    def openAllFiles
        setEnumFiles(
            openFile("{bind_root}/{bind_module}.enum.decl.inc"),
            openFile("{bind_root}/{bind_module}.enum.class.inc"),
            openFile("{bind_root}/{bind_module}.enum.add.inc"))
        setStructFiles (
            openFile("{bind_root}/{bind_module}.struct.decl.inc"),
            openFile("{bind_root}/{bind_module}.struct.impl.inc"),
            openFile("{bind_root}/{bind_module}.struct.class.inc"),
            openFile("{bind_root}/{bind_module}.struct.add.inc"),
            openFile("{bind_root}/{bind_module}.struct.postadd.inc"),
            openFile("{bind_root}/{bind_module}.struct.cast.inc"))
        setAliasFiles (
            openFile("{bind_root}/{bind_module}.alias.add.inc"),
            openFile("{bind_root}/{bind_module}.dummy.add.inc"))
        setFuncFiles (
            openFile("{bind_root}/{bind_module}.func.reg.inc"),
            openFile("{bind_root}/{bind_module}.func.decl.inc"))
        setModuleFiles(
            openFile("{bind_root}/{bind_module}.h"),
            openFile("{bind_root}/need_{bind_module}.h"),
            openFile("{bind_root}/{bind_module}.cpp"))
        if generate_constant_decl
            define_const_file = openFile("{bind_root}/{bind_module}.const.inc")
    def closeAllFiles
        closeFile(enum_decl_file)
        closeFile(enum_class_file)
        closeFile(enum_add_file)
        closeFile(struct_decl_file)
        closeFile(struct_impl_file)
        closeFile(struct_class_file)
        closeFile(struct_add_file)
        closeFile(struct_postadd_file)
        closeFile(alias_add_file)
        closeFile(dummy_add_file)
        closeFile(module_cpp_file)
        closeFile(module_h_file)
        closeFile(module_need_h_file)
        closeFile(define_const_file)
        closeLastFuncFile()
    def setEnumFiles ( edecl,eclass,eadd:FILE const? )
        enum_decl_file = edecl
        enum_class_file = eclass
        enum_add_file = eadd
    def setStructFiles ( sdecl,simpl,sclass,sadd,spostadd,scast:FILE const? )
        struct_decl_file = sdecl
        struct_impl_file = simpl
        struct_class_file = sclass
        struct_add_file = sadd
        struct_postadd_file = spostadd
        struct_cast_file = scast
    def setAliasFiles ( af,df:FILE const? )
        alias_add_file = af
        dummy_add_file = df
    def setFuncFiles ( ffr,ffd:FILE const? )
        func_file_reg = ffr
        func_file_decl = ffd
    def setModuleFiles ( mh,mnh,mcpp:FILE const? )
        module_h_file = mh
        module_need_h_file = mnh
        module_cpp_file = mcpp
    def nextFuncFile
        if func_to_stdout
            return fstdout()
        else
            return openFile("{bind_root}/{bind_module}.func_{func_chunk_index}.cpp")
    def closeLastFuncFile
        if func_file != null
            writeFuncPostfix()
            closeFile(func_file)
    def getBindFldName ( field_name : string ) : string     // note: namespaces form struct
        return unsafe_names?[field_name] ?? field_name
    def substFieldType ( field_name : string ) : string     // note: namespaces form struct
        return ""
    def override parse_Field ( var c : CXCursor )
        let field_name = string(clang_getCursorSpelling(c))
        var field_type = clang_getCanonicalType(clang_getCursorType(c))
        let field_type_name = string(clang_getTypeSpelling(field_type))
        var wr = swrite |> back
        let substType = substFieldType(field_name)
        if substType |> empty()
            *wr |> write("\t\taddField<DAS_BIND_MANAGED_FIELD({field_name})>(\"{getBindFldName(field_name)}\",\"{field_name}\");\n")
        else
            *wr |> write("\t\taddField<{substType},offsetof(ManagedType,{field_name})>(\"{getBindFldName(field_name)}\",\"{field_name}\")\n")
            *wr |> write("\t\t\t.adjustAot(\"das_reinterpret<{substType}>::pass(\",\")\");\n")
    def isLocalType ( name,cppname:string )
        return false
    def isArgByValue ( name:string )
        return false
    def override parse_Struct ( var c : CXCursor; struct_name : string )
        let struct_namespace_name = namespace_name(struct_name)
        if !struct_ann |> key_exists(struct_namespace_name)
            fprint(struct_decl_file, "MAKE_EXTERNAL_TYPE_FACTORY({struct_name},{struct_namespace_name});\n")
            fprint(struct_impl_file, "IMPLEMENT_EXTERNAL_TYPE_FACTORY({struct_name},{struct_namespace_name});\n")
        if clang_isForwardDeclaration(c)
            struct_ann[struct_namespace_name] = false
            return
        struct_ann[struct_namespace_name] = true
        let st = build_string() <| $ ( var writer:StringBuilderWriter )
            swrite |> push(unsafe(addr(writer)))
            writer |> write("struct {struct_name}_GeneratedAnnotation : ManagedStructureAnnotation<{struct_namespace_name}> \{\n")
            if isLocalType(struct_name,struct_namespace_name)
                writer |> write("\tvirtual bool isLocal() const override \{ return true; \}\n")
                writer |> write("\tvirtual bool canBePlacedInContainer() const override \{ return true; \}\n")
            writer |> write("\t{struct_name}_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation (\"{struct_name}\", ml) \{\n")
            writer |> write("\t\}\n")
            writer |> write("\tvoid init () \{\n")
            AnyGenBind`parse_Struct(self, c, struct_name)
            writer |> write("\t\}\n\};\n")
            if isArgByValue(struct_namespace_name)
                fprint(struct_cast_file, "template <> struct cast_arg<{struct_namespace_name}> \{\n")
                fprint(struct_cast_file, "\tstatic __forceinline {struct_namespace_name} to ( Context & ctx, SimNode * node ) \{\n")
                fprint(struct_cast_file, "\t\tvec4f res = node->eval(ctx);\n")
                fprint(struct_cast_file, "\t\treturn * cast<{struct_namespace_name} *>::to(res);\n")
                fprint(struct_cast_file, "\t\}\n")
                fprint(struct_cast_file, "\};\n")
            swrite |> pop()
        fprint(struct_class_file, "{st}\n")
        fprint(struct_add_file,"auto ann_{struct_name} = make_smart<{struct_name}_GeneratedAnnotation>(lib);\n")
        fprint(struct_add_file,"addAnnotation(ann_{struct_name});\n")
        fprint(struct_postadd_file,"initRecAnnotation(ann_{struct_name},lib);\n")
    def typeSpelling ( var c : CXType )
        // TODO: proper type spelling?
        var spl = string(clang_getTypeSpelling(c))
        if c.kind == CXTypeKind Enum
            spl = enum2enum?[spl] ?? spl
        return spl
    def functionPtrSpelling ( var c : CXCursor )
        // TODO: calling convention?
        var fun_type = clang_getCanonicalType(clang_getCursorType(c))
        var res_type = clang_getResultType(fun_type)
        let function_result_type = typeSpelling(res_type)
        return build_string() <| $ ( str )
            str |> write("{function_result_type} (*)(")
            let narg = clang_Cursor_getNumArguments(c)
            for ai in urange(narg)
                if ai != 0u
                    str |> write(",")
                var carg = clang_Cursor_getArgument(c, ai)
                var arg_type = clang_getCanonicalType(clang_getCursorType(carg))
                let argument_type = typeSpelling(arg_type)
                str |> write("{argument_type}")
            str |> write(")")
    def override parse_FunctionDecl ( var c : CXCursor )
        func_count ++
        if func_count >= func_per_chunk
            func_count = 0
            closeLastFuncFile()
        if func_file==null
            func_chunk_index ++
            func_file = nextFuncFile()
            writeFuncPrefix()
            fwrite(func_file_reg, "initFunctions_{func_chunk_index}();\n")
            fwrite(func_file_decl, "void initFunctions_{func_chunk_index}();\n")
            func_count = 0
        let function_name = string(clang_getCursorSpelling(c))
        var fun_type = clang_getCanonicalType(clang_getCursorType(c))
        var res_type = clang_getResultType(fun_type)
        let bind_enchantation = "{functionPtrSpelling(c)},{function_name}"
        var extra_enchantation = ""
        let cpp_enchantation = namespace_name(function_name)
        if clang_das_isRef(res_type)
            extra_enchantation = ", SimNode_ExtFuncCallRef"
        elif clang_das_isCopyOnReturn(res_type)
            extra_enchantation = ",SimNode_ExtFuncCallAndCopyOrMove"
        fprint(func_file, "\taddExtern<{bind_enchantation}{extra_enchantation}>(*this,lib,\"{function_name}\",SideEffects::worstDefault,\"{cpp_enchantation}\")")
        let narg = clang_Cursor_getNumArguments(c)
        if narg!=0
            fprint(func_file,"\n\t\t->args(\{")
            for ai in urange(narg)
                if ai != 0u
                    fprint(func_file,",")
                var carg = clang_Cursor_getArgument(c, ai)
                let arg_name = string(clang_getCursorSpelling(carg))
                fprint(func_file,"\"{arg_name}\"")
            fprint(func_file,"\});\n")
        else
            fprint(func_file,";\n")
    def enum_entry_name ( en, enn:string ) : string
        let prefix = enum_prefix?[en] ?? en
        let uprefix = to_upper(prefix)
        var name = enn
        if enn|>starts_with("{prefix}_") || enn|>starts_with("{uprefix}_")
            name = enn |> slice(length(prefix)+1)
        elif enn|>starts_with(prefix) || enn|>starts_with(uprefix)
            name = enn |> slice(length(prefix))
        return unsafe_names?[name] ?? name
    def generateEnumDecl
        for decl in enumDecl
            let cppEnumName = enum2enum?[decl.ns_en] ?? decl.ns_en
            fprint(enum_decl_file, "DAS_BIND_ENUM_CAST({cppEnumName});\n")
            fprint(enum_decl_file, "DAS_BASE_BIND_ENUM_GEN({cppEnumName},{decl.enum_name});\n\n")
    def override parse_Enum ( var cursor : CXCursor; enum_name : string )
        let ns_en = namespace_name(enum_name)
        enumDecl |> push([[auto ns_en,enum_name]])
        fprint(enum_class_file, "class Enumeration_{enum_name} : public das::Enumeration \{\n")
        fprint(enum_class_file, "public:\n")
        fprint(enum_class_file, "\tEnumeration_{enum_name}() : das::Enumeration(\"{enum_name}\") \{\n")
        fprint(enum_class_file, "\t\texternal = true;\n")
        fprint(enum_class_file, "\t\tcppName = \"{ns_en}\";\n")
        let itype = string(clang_getTypeSpelling(clang_getEnumDeclIntegerType(cursor)))
        fprint(enum_class_file, "\t\tbaseType = (das::Type) das::ToBasicType<{itype}>::type;\n")
        clang_visitChildren(cursor) <| $ ( var c,parent )
            if c.kind==CXCursorKind EnumConstantDecl
                let een = string(clang_getCursorDisplayName(c))
                let deen = enum_entry_name(enum_name, een)
                fprint(enum_class_file, "\t\taddIEx(\"{deen}\", \"{een}\", int64_t({ns_en}::{een}), das::LineInfo());\n")
                return CXChildVisitResult Continue
            else
                return CXChildVisitResult Recurse
        fprint(enum_class_file, "\t\}\n\};\n\n")
        fprint(enum_add_file, "addEnumeration(make_smart<Enumeration_{enum_name}>());\n");
    def override parse_Alias ( var c : CXCursor )
        let tdn = string(clang_getCursorDisplayName(c))
        var tdt = clang_getCanonicalType(clang_getCursorType(c))
        let tdts = string(clang_getTypeSpelling(tdt))
        fprint(alias_add_file, "auto alias_{tdn} = typeFactory<{tdts}>::make(lib);\n")
        fprint(alias_add_file, "alias_{tdn}->alias = \"{tdn}\";\n")
        fprint(alias_add_file, "addAlias(alias_{tdn});\n\n")
    def override generate
        AnyGenBind`generate(self)
        generateEnumDecl()
        generateDummy()
        generateModuleH()
        generateModuleNeedH()
        generateModuleCpp()
        if generate_constant_decl
            if empty(CONST_FILE_NAMES)
                generateConstants(open_file_name())
            else
                generateConstantsFrom(CONST_FILE_NAMES)
        closeAllFiles()
    def generateDummy
        for k,v in keys(struct_ann),values(struct_ann)
            if !v
                let kname = k   // TODO: struct name from sturct namespaced name
                fprint(dummy_add_file, "addAnnotation(make_smart<DummyTypeAnnotation>(\"{kname}\", \"{k}\", 1, 1));\n")
    def writeFuncPrefix
        fwrite(func_file, "#include \"daScript/misc/platform.h\"\n")
        fwrite(func_file, "#include \"daScript/ast/ast.h\"\n")
        fwrite(func_file, "#include \"daScript/ast/ast_interop.h\"\n")
        fwrite(func_file, "#include \"daScript/ast/ast_handle.h\"\n")
        fwrite(func_file, "#include \"daScript/ast/ast_typefactory_bind.h\"\n")
        fwrite(func_file, "#include \"daScript/simulate/bind_enum.h\"\n")
        fwrite(func_file, "#include \"{bind_module}.h\"\n")
        fwrite(func_file, "#include \"need_{bind_module}.h\"\n")
        fwrite(func_file, "namespace das \{\n")
        fwrite(func_file, "void Module_{bind_module}::initFunctions_{func_chunk_index}() \{\n")
    def writeFuncPostfix
        fwrite(func_file, "\}\n\}\n\n")
    def generateModuleNeedH
        module_need_h_file |> fwrite("#pragma once\n")
        module_need_h_file |> fwrite("#include <{PARSE_FILE_NAME}>\n")
        module_need_h_file |> fwrite("#include \"{bind_module}.enum.decl.inc\"\n")
        module_need_h_file |> fwrite("#include \"{bind_module}.struct.decl.inc\"\n")
        module_need_h_file |> fwrite("namespace das \{\n")
        module_need_h_file |> fwrite("\t#include \"{bind_module}.struct.cast.inc\"\n")
        module_need_h_file |> fwrite("\}\n\n")
    def generateModuleCpp
        module_cpp_file |> fwrite("#include \"daScript/misc/platform.h\"\n")
        module_cpp_file |> fwrite("#include \"daScript/ast/ast.h\"\n")
        module_cpp_file |> fwrite("#include \"daScript/ast/ast_interop.h\"\n")
        module_cpp_file |> fwrite("#include \"daScript/ast/ast_handle.h\"\n")
        module_cpp_file |> fwrite("#include \"daScript/ast/ast_typefactory_bind.h\"\n")
        module_cpp_file |> fwrite("#include \"daScript/simulate/bind_enum.h\"\n")
        module_cpp_file |> fwrite("#include \"{bind_module}.h\"\n")
        module_cpp_file |> fwrite("#include \"need_{bind_module}.h\"\n")
        module_cpp_file |> fwrite("#include \"{bind_module}.struct.impl.inc\"\n")
        module_cpp_file |> fwrite("namespace das \{\n")
        module_cpp_file |> fwrite("#include \"{bind_module}.enum.class.inc\"\n")
        module_cpp_file |> fwrite("#include \"{bind_module}.struct.class.inc\"\n")
        module_cpp_file |> fwrite("Module_{bind_module}::Module_{bind_module}() : Module(\"{bind_das_module}\") \{\n")  // TODO: das module name
        module_cpp_file |> fwrite("\tlib.addModule(this);\n")
        module_cpp_file |> fwrite("\tlib.addBuiltInModule();\n")
        module_cpp_file |> fwrite("\}\n")
        module_cpp_file |> fwrite("bool Module_{bind_module}::initDependencies() \{\n")
        module_cpp_file |> fwrite("\tif ( initialized ) return true;\n")
        module_cpp_file |> fwrite("\tinitialized = true;\n")
        if generate_constant_decl
            module_cpp_file |> fwrite("\t#include \"{bind_module}.const.inc\"\n")
        module_cpp_file |> fwrite("\t#include \"{bind_module}.enum.add.inc\"\n")
        module_cpp_file |> fwrite("\t#include \"{bind_module}.dummy.add.inc\"\n")
        module_cpp_file |> fwrite("\t#include \"{bind_module}.struct.add.inc\"\n")
        module_cpp_file |> fwrite("\t#include \"{bind_module}.struct.postadd.inc\"\n")
        module_cpp_file |> fwrite("\t#include \"{bind_module}.alias.add.inc\"\n")
        module_cpp_file |> fwrite("\t#include \"{bind_module}.func.reg.inc\"\n")
        module_cpp_file |> fwrite("\tinitMain();\n")
        module_cpp_file |> fwrite("\treturn true;\n")
        module_cpp_file |> fwrite("\}\n")
        module_cpp_file |> fwrite("}\n")
        module_cpp_file |> fwrite("REGISTER_MODULE_IN_NAMESPACE(Module_{bind_module},das);\n\n")
    def generateModuleHSuffix
        pass
    def generateModuleHPrefix
        pass
    def generateModuleH
        module_h_file |> fwrite("#pragma once\n")
        generateModuleHPrefix()
        module_h_file |> fwrite("namespace das \{\n")
        module_h_file |> fwrite("class Module_{bind_module} : public Module \{\n")
        module_h_file |> fwrite("public:\n")
        module_h_file |> fwrite("\tModule_{bind_module}();\n")
        module_h_file |> fwrite("protected:\n")
        module_h_file |> fwrite("virtual bool initDependencies() override;\n")
        module_h_file |> fwrite("\tvoid initMain ();\n")
        module_h_file |> fwrite("\tvirtual ModuleAotType aotRequire ( TextWriter & tw ) const override;\n")
        module_h_file |> fwrite("\t#include \"{bind_module}.func.decl.inc\"\n")
        module_h_file |> fwrite("public:\n")
        module_h_file |> fwrite("\tModuleLibrary lib;\n")
        module_h_file |> fwrite("\tbool initialized = false;\n")
        generateModuleHSuffix()
        module_h_file |> fwrite("};\n")
        module_h_file |> fwrite("}\n\n")
    def genCMakeDecl(MODULE_VAR:string)
        print("\n\n")
        print("\tSET({MODULE_VAR}_MODULE_SRC\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/need_{bind_module}.h\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.h\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.cpp\n")
        print("\t# user include\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.main.cpp\n")
        print("\t# generated binding includes\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.alias.add.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.dummy.add.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.enum.add.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.enum.class.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.enum.decl.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.add.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.postadd.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.class.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.decl.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.impl.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.cast.inc\n")
        if generate_constant_decl
            print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.const.inc\n")
        print("\t# generated functions\n")
        for index in range(1,func_chunk_index+1)
            print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.func_{index}.cpp\n")
        print("\t)\n\n")


/*
print("{function_result_type} ")
if !empty(nspaces)
    for n in nspaces
        print("{n}::")
print("{function_name} ( ")
let narg = clang_Cursor_getNumArguments(c)
for ai in urange(narg)
    if ai != 0u
        print(", ")
    var carg = clang_Cursor_getArgument(c, ai)
    let arg_name = string(clang_getCursorSpelling(carg))
    var arg_type = clang_getCanonicalType(clang_getCursorType(carg))
    let argument_type = string(clang_getTypeSpelling(arg_type))
    print("{argument_type} {arg_name}")
print(" );\n")
*/

options debugger
