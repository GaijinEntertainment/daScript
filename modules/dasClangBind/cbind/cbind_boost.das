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
    ARGV : array<string>
    prevFileName : string
    nspaces : array<string>
    emptyStruct : CXCursor
    hasEmptyStruct : bool
    emptyEnum : CXCursor
    hasEmptyEnum : bool
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
        var index = clang_createIndex(0,0)
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
                    elif tdt.kind == CXTypeKind Enum
                        let sn = string(clang_getTypeSpelling(tdt))
                        if sn == tdn
                            to_log(LOG_INFO, "skipping typedef enum {sn} {tdn};")
                            return CXChildVisitResult Continue
                    parse_Alias(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind FunctionDecl
                    parse_FunctionDecl(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind StructDecl
                    let struct_name = string(clang_getCursorSpelling(c))
                    if empty(struct_name)
                        if hasEmptyStruct
                            to_log(LOG_ERROR, "already in empty struct mode from {clang_getCursorLocationDescription(emptyStruct)}, at {clang_getCursorLocationDescription(c)}")
                        emptyStruct = c
                        hasEmptyStruct = true
                    else
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
                    parse_Field(c)
                    return CXChildVisitResult Continue
                else
                    to_log(LOG_WARNING, "unsupported {c.kind}: {describe(c)}")
                return CXChildVisitResult Recurse
            else
                return CXChildVisitResult Recurse

class DasGenBind : AnyGenBind
    blocked_defines_table : table<string;bool>
    const_type_table : table<string; string>
    known_type : table<string; string>
    define_const_file : FILE const?
    func_decl_file : FILE const?
    def DasGenBind ( pfn,pfp:string; args:array<string> )
        init_args(pfn,pfp,args)
        define_const_file = fstdout()
        func_decl_file = fstdout()
    def setFiles ( dcf, fdf:FILE const? )
        define_const_file = dcf
        func_decl_file = fdf
    def is_const_blocked ( name:string ) : bool
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
                if !blocked_defines_table |> key_exists(DEF)
                    if is_const_blocked==null || !is_const_blocked(DEF)
                        let ctt = getGenConstType(DEF,defTName)
                        if !dup[DEF]
                            fprint(define_const_file,"let {DEF} = {ctt}({VAL}{suf})\n")
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
                searchAndGenConst(reg_def_hex, "uint", "u", true, ofs, data, dup)
                searchAndGenConst(reg_def_dec, "int", "", false, ofs, data, dup)
                searchAndGenConst(reg_def_UINT8, "uint8", "u", true, ofs, data, dup)
                searchAndGenConst(reg_def_UINT16, "uint16", "u", true, ofs, data, dup)
                searchAndGenConst(reg_def_UINT32, "uint", "u", true, ofs, data, dup)
                searchAndGenConst(reg_def_UINT64, "uint64", "ul", true, ofs, data, dup)
    def generateConstants(fname:string)
        var fnames <- [{ auto[] fname }]
        defer <| { delete fnames; }
        fnames |> generateConstantsFrom()
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
        generateConstants(open_file_name())


let DEFAULT_CLANG_ARGUMENTS <- [{string
    "-xc++-header";
    "-std=c++1z"
}]

class CppGenBind : AnyGenBind
    enum_decl_file : FILE const?
    enum_class_file : FILE const?
    enum_add_file : FILE const?
    enum_prefix : table<string; string>
    struct_decl_file : FILE const?
    struct_impl_file : FILE const?
    struct_class_file : FILE const?
    struct_add_file : FILE const?
    struct_postadd_file : FILE const?
    alias_add_file : FILE const?
    dummy_add_file : FILE const?
    swrite : array<StringBuilderWriter?>
    struct_ann : table<string; bool>
    def CppGenBind ( pfn,pfp:string; args:array<string> )
        init_args(pfn,pfp,args)
        set_default_files()
    def set_default_files
        enum_decl_file = fstdout()
        enum_class_file = fstdout()
        enum_add_file = fstdout()
        struct_decl_file = fstdout()
        struct_impl_file = fstdout()
        struct_class_file = fstdout()
        struct_add_file = fstdout()
        struct_postadd_file = fstdout()
        alias_add_file = fstdout()
        dummy_add_file = fstdout()
    def setEnumFiles ( edecl,eclass,eadd:FILE const? )
        enum_decl_file = edecl
        enum_class_file = eclass
        enum_add_file = eadd
    def setStructFiles ( sdecl,simpl,sclass,sadd,spostadd:FILE const? )
        struct_decl_file = sdecl
        struct_impl_file = simpl
        struct_class_file = sclass
        struct_add_file = sadd
        struct_postadd_file = spostadd
    def setAliasFiles ( af,df:FILE const? )
        alias_add_file = af
        dummy_add_file = df
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
    def override parse_Struct ( var c : CXCursor; struct_name : string )
        let struct_namespace_name = namespace_name(struct_name)
        if !struct_ann |> key_exists(struct_namespace_name)
            fprint(struct_decl_file, "MAKE_EXTERNAL_TYPE_FACTORY({struct_name},{struct_namespace_name});\n")
            fprint(struct_impl_file, "IMPLEMENT_EXTERNAL_TYPE_FACTORY({struct_name},{struct_namespace_name});\n\n")
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
            swrite |> pop()
        fprint(struct_class_file, "{st}\n")
        fprint(struct_add_file,"auto ann_{struct_name} = make_smart<{struct_name}_GeneratedAnnotation>(lib);\n")
        fprint(struct_add_file,"addAnnotation(ann_{struct_name});\n")
        fprint(struct_postadd_file,"initRecAnnotation(ann_{struct_name},lib);\n")
    def override parse_FunctionDecl ( var c : CXCursor )
        let function_name = string(clang_getCursorSpelling(c))
        var fun_type = clang_getCanonicalType(clang_getCursorType(c))
        var res_type = clang_getResultType(fun_type)
        let function_result_type = string(clang_getTypeSpelling(res_type))
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
    def enum_entry_name ( en, enn:string ) : string
        let prefix = enum_prefix?[en] ?? en
        var name = enn
        if enn |> starts_with("{prefix}_")
            name = enn |> slice(length(prefix)+1)
        elif enn |> starts_with(prefix)
            name = enn |> slice(length(prefix))
        return unsafe_names?[name] ?? name
    def override parse_Enum ( var cursor : CXCursor; enum_name : string )
        let ns_en = namespace_name(enum_name)
        fprint(enum_decl_file, "DAS_BIND_ENUM_CAST({ns_en});\n")
        fprint(enum_decl_file, "DAS_BASE_BIND_ENUM_GEN({ns_en},{enum_name});\n\n")
        fprint(enum_class_file, "class Enumeration_{enum_name} : public das::Enumeration \{\n")
        fprint(enum_class_file, "public:\n")
        fprint(enum_class_file, "\tEnumeration_{enum_name}() : das::Enumeration(\"{enum_name}\") \{\n")
        fprint(enum_class_file, "\t\texternal = true;\n")
        fprint(enum_class_file, "\t\tcppName = \"{ns_en}\";\n")
        fprint(enum_class_file, "\t\tbaseType = (das::Type) das::ToBasicType< das::underlying_type< {ns_en} >::type >::type;\n")
        clang_visitChildren(cursor) <| $ ( var c,parent )
            if c.kind==CXCursorKind EnumConstantDecl
                let een = string(clang_getCursorDisplayName(c))
                let deen = enum_entry_name(enum_name, een)
                fprint(enum_class_file, "\t\taddIEx(\"{deen}\", \"{een}\", int64_t({ns_en}::{een}), das::LineInfo());\n")
                return CXChildVisitResult Continue
            else
                return CXChildVisitResult Recurse
        fprint(enum_class_file, "\t\}\n\};\n\n")
        fprint(enum_add_file, "\taddEnumeration(make_smart<Enumeration_{enum_name}>());\n");
    def override parse_Alias ( var c : CXCursor )
        let tdn = string(clang_getCursorDisplayName(c))
        var tdt = clang_getCanonicalType(clang_getCursorType(c))
        let tdts = string(clang_getTypeSpelling(tdt))
        fprint(alias_add_file, "\tauto alias_{tdn} = typeFactory<{tdts}>::make(lib);\n")
        fprint(alias_add_file, "\talias_{tdn}->alias = \"{tdn}\";\n")
        fprint(alias_add_file, "\taddAlias(alias_{tdn});\n")
    def override generate
        AnyGenBind`generate(self)
        generateDummy()
    def generateDummy
        for k,v in keys(struct_ann),values(struct_ann)
            if !v
                let kname = k   // TODO: struct name from sturct namespaced name
                fprint(dummy_add_file, "addAnnotation(make_smart<DummyTypeAnnotation>(\"{kname}\", \"{k}\", 1, 1));\n")

options debugger
