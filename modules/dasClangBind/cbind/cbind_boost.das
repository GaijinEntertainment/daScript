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
        var pt = clang_getCanonicalType(clang_getPointeeType(t))
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
            error += "unsupported type CXXRecord name {rname}\n"
            return "\u00BF"
    elif t.kind==CXTypeKind FunctionProto
        if rules.function_proto_is_void_ptr
            return "void?"
        else
            error += "unsupported type FunctionProto\n"
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
        to_log(LOG_ERROR, "\nTYPE ERROR:\n{err}\n")
    return result

def clang_getCursorLocationDescription ( var c:CXCursor )
    var file : CXFile
    var line, column, offset : uint
    clang_getSpellingLocation(clang_getCursorLocation(c), safe_addr(file), safe_addr(line), safe_addr(column), safe_addr(offset))
    var fname = clang_getFileName(file)
    return "{string(fname)}:{int(line)}:{int(column)}"

def clang_getCursorLocationFileName ( var c:CXCursor )
    var file : CXFile
    var line, column, offset : uint
    clang_getSpellingLocation(clang_getCursorLocation(c), safe_addr(file), safe_addr(line), safe_addr(column), safe_addr(offset))
    var fname = clang_getFileName(file)
    return "{string(fname)}"

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
    verbose_file_skipping = false
    PARSE_FILE_NAME : string
    PARSE_FILE_PREFIX : string
    CONST_FILE_NAMES : array<string>
    ARGV : array<string>
    prevFileName : string
    nspaces : array<string>
    lsp : table<string; string>
    skip_struct_by_access : table<string>
    interfaces : table<string; array<string>>
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
    inClass : int
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
        var index = clang_createIndex(1,1)
        var unit = clang_parseTranslationUnit(
            index,
            open_file_name(),
            unsafe(addr(ARGV[0])), length(ARGV),
            null, 0u,
            CXTranslationUnit_SkipFunctionBodies)
        if unit==null
            panic("unable to parse translation unit {open_file_name()}")
        var cursor = clang_getTranslationUnitCursor(unit)
        parse(cursor)
        let ndiag = clang_getNumDiagnostics(unit)
        for i in urange(ndiag)
            var diag = clang_getDiagnostic(unit, i)
            var diags = string(clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions()))
            print("{diags}\n")
        clang_disposeTranslationUnit(unit)
        clang_disposeIndex(index)
    def namespace_struct_name ( dash:string="::" ) : string
        return build_string <| $ ( wr )
            for n,i in nspaces,count()
                if i != 0
                    wr |> write(dash)
                wr |> write(n)
    def namespace_name ( name:string; dash:string="::" ) : string
        return build_string <| $ ( wr )
            for n in nspaces
                wr |> write(n)
                wr |> write(dash)
            wr |> write(name)
    def skip_method ( var c : CXCursor ) : bool
        return skip_anyFunction(c, true)
    def skip_constructor ( var c : CXCursor ) : bool
        return skip_anyFunction(c, true)
    def skip_function ( var c : CXCursor ) : bool
        return skip_anyFunction(c, false)
    def skip_anyFunction ( var c : CXCursor; isMethod:bool ) : bool
        let function_name = string(clang_getCursorSpelling(c))
        if isMethod
            if clang_CXXMethod_isPureVirtual(c)!=0u
                to_log(LOG_INFO,"skipping pure virtual {function_name} at {clang_getCursorLocationDescription(c)}\n")
                return true
        var semP = clang_getCursorSemanticParent(c)
        var lexP = clang_getCursorLexicalParent(c)
        if clang_equalCursors(semP,lexP)==0u
            let sT = string(clang_getCursorSpelling(semP))
            let lT = string(clang_getCursorSpelling(lexP))
            to_log(LOG_INFO,"skipping implementation {function_name} at {clang_getCursorLocationDescription(c)} {sT}!={lT}\n")
            return true
        if function_name |> starts_with("operator")
            let Ch = character_at(function_name,8)
            if !(is_alpha(Ch) || is_number(Ch) || Ch=='_')
                if !isMethod
                    to_log(LOG_INFO,"skipping global operator {function_name} at {clang_getCursorLocationDescription(c)}\n")
                    return true
                let op_name = function_name |> slice(8)
                if op_name == "="
                    return true
        var fun_type = clang_getCursorType(c)
        if clang_isFunctionTypeVariadic(fun_type)!=0u
            to_log(LOG_INFO,"skipping function {function_name} due variadic arguments at {clang_getCursorLocationDescription(c)}\n")
            return true
        let narg = clang_Cursor_getNumArguments(c)
        for ai in urange(narg)
            var carg = clang_Cursor_getArgument(c, ai)
            var arg_type = clang_getCursorType(carg)
            if skip_type(arg_type)
                let arg_type_name = string(clang_getTypeSpelling(arg_type))
                let arg_name = string(clang_getCursorSpelling(carg))
                to_log(LOG_INFO,"skipping function {function_name} due to argument {arg_name} type {arg_type_name} at {clang_getCursorLocationDescription(c)}\n")
                return true
        var res_type = clang_getResultType(fun_type)
        if skip_type(res_type)
            let res_type_name = string(clang_getTypeSpelling(res_type))
            to_log(LOG_INFO,"skipping function {function_name} due to result type {res_type_name} at {clang_getCursorLocationDescription(c)}\n")
            return true
        return false
    def skip_struct ( name : string )
        return false
    def skip_type ( var _c : CXType )
        let nc_name = string(clang_getTypeSpelling(_c))
        if nc_name=="va_list"
            return true
        var c = clang_getCanonicalType(_c)
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
            if skip_struct_by_access |> key_exists(rname)
                return true
            if skip_struct(rname)
                return true
        return false
    def parse_FunctionDecl ( var c : CXCursor )
        pass
    def parse_Method ( var c : CXCursor )
        pass
    def parse_Constructor ( var c : CXCursor )
        pass
    def parse_Namespace ( var c : CXCursor )
        let ns = string(clang_getCursorDisplayName(c))
        nspaces |> push(ns)
        parse(c)
        nspaces |> pop
    def parse_Enum ( var cursor : CXCursor; enum_name : string )
        pass
    def parse_Struct ( var c : CXCursor; struct_name : string )
        inClass ++
        nspaces |> push(struct_name)
        parse(c)
        nspaces |> pop
        inClass --
    def parse_Field ( var c : CXCursor )
        pass
    def parse_Alias ( var c : CXCursor )
        pass
    def skip_alias ( var c : CXCursor )
        return true
    def parse ( var cursor : CXCursor )
        clang_visitChildren(cursor) <| $ ( var c,parent )
            var file : CXFile
            var line, column, offset : uint
            clang_getSpellingLocation(clang_getCursorLocation(c), safe_addr(file), safe_addr(line), safe_addr(column), safe_addr(offset))
            var fname = clang_getFileName(file)
            peek(fname) <| $ ( fn )
                if prevFileName != fn
                    prevFileName := fn
                    if skip_file(prevFileName) && verbose_file_skipping
                        to_log(LOG_INFO, "skipping {prevFileName}\n")
            if !skip_file(prevFileName)
                if parent.kind == CXCursorKind ClassTemplate    // skipping just about everything from template body
                    return CXChildVisitResult Continue
                let kind = clang_getCursorKind(c)
                if kind == CXCursorKind TypedefDecl
                    let tdn = string(clang_getCursorDisplayName(c))
                    var tdt = clang_getCanonicalType(clang_getCursorType(c))
                    if hasEmptyStruct
                        if tdt.kind == CXTypeKind Record
                            if !skip_struct(tdn)
                                let acs = clang_getCXXAccessSpecifier(c)
                                if acs != CX_CXXAccessSpecifier Public && acs != CX_CXXAccessSpecifier InvalidAccessSpecifier
                                    to_log(LOG_INFO, "skipping nameless structure {tdn} due to access specifier {acs}\n")
                                else
                                    parse_Struct(emptyStruct, tdn)
                            hasEmptyStruct = false
                            return CXChildVisitResult Continue
                        else
                            to_log(LOG_ERROR, "typedef of type {tdt.kind} with nameless structure at {clang_getCursorLocationDescription(c)}\n")
                            hasEmptyStruct = false
                    elif hasEmptyEnum
                        if tdt.kind == CXTypeKind Enum
                            let acs = clang_getCXXAccessSpecifier(c)
                            if acs != CX_CXXAccessSpecifier Public && acs != CX_CXXAccessSpecifier InvalidAccessSpecifier
                                to_log(LOG_INFO, "skipping nameless enumeration {tdn} due to access specifier {acs}\n")
                            else
                                parse_Enum(emptyEnum, tdn)
                            hasEmptyEnum = false
                            return CXChildVisitResult Continue
                        else
                            to_log(LOG_ERROR, "typedef of type {tdt.kind} with nameless enumeration at {clang_getCursorLocationDescription(c)}\n")
                            hasEmptyEnum = false
                    elif tdt.kind == CXTypeKind Record
                        let sn = string(clang_getTypeSpelling(tdt))
                        if sn == tdn
                            to_log(LOG_INFO, "skipping typedef struct {sn} {tdn};\n")
                            return CXChildVisitResult Continue
                        if skip_struct(sn) || skip_struct(tdn)
                            to_log(LOG_INFO, "skipping typedef struct {sn} {tdn};\n")
                            return CXChildVisitResult Continue
                    elif tdt.kind == CXTypeKind Enum
                        let sn = string(clang_getTypeSpelling(tdt))
                        if sn == tdn
                            to_log(LOG_INFO, "skipping typedef enum {sn} {tdn};\n")
                            return CXChildVisitResult Continue
                        else
                            enum2enum[namespace_name(sn)] = tdn
                            return CXChildVisitResult Continue
                    if inClass == 0
                        if !skip_alias(c)
                            parse_Alias(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind FunctionDecl
                    if !skip_function(c)
                        parse_FunctionDecl(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind CXXMethod
                    let acs = clang_getCXXAccessSpecifier(c)
                    if acs != CX_CXXAccessSpecifier Public
                        let method_name = string(clang_getCursorSpelling(c))
                        to_log(LOG_INFO, "skipping {namespace_name(method_name)} due to access specifier {acs}\n")
                    else
                        if !skip_method(c)
                            parse_Method(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind Constructor
                    let acs = clang_getCXXAccessSpecifier(c)
                    if acs != CX_CXXAccessSpecifier Public
                        let ctor_name = string(clang_getCursorSpelling(c))
                        to_log(LOG_INFO, "skipping {namespace_name(ctor_name)} constructor due to access specifier {acs}\n")
                    else
                        if !skip_constructor(c)
                            parse_Constructor(c)
                    return CXChildVisitResult Continue
                /*
                elif kind == CXCursorKind CXXConversionFunction
                    // TODO: support autobind of cast?
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind CXXDestructor
                    return CXChildVisitResult Continue
                */
                elif kind == CXCursorKind StructDecl || kind == CXCursorKind ClassDecl || kind == CXCursorKind UnionDecl
                    let struct_name = string(clang_getCursorSpelling(c))
                    let nta = clang_Type_getNumTemplateArguments(clang_getCursorType(c))
                    if nta != -1
                        to_log(LOG_INFO, "skipping structure {struct_name} due to being template instance\n")
                        return CXChildVisitResult Continue
                    if empty(struct_name)
                        if hasEmptyStruct
                            to_log(LOG_ERROR, "already in empty struct mode from {clang_getCursorLocationDescription(emptyStruct)}, at {clang_getCursorLocationDescription(c)}\n")
                        emptyStruct = c
                        hasEmptyStruct = true
                    else
                        if !skip_struct(struct_name)
                            let acs = clang_getCXXAccessSpecifier(c)
                            if acs != CX_CXXAccessSpecifier Public && acs != CX_CXXAccessSpecifier InvalidAccessSpecifier
                                var canonical = clang_getCanonicalType(clang_getCursorType(c))
                                let sname = string(clang_getTypeSpelling(clang_getCursorType(c)))
                                to_log(LOG_INFO, "skipping structure {struct_name} aka {sname} due to access specifier {acs}\n")
                                skip_struct_by_access |> insert(sname)
                                return CXChildVisitResult Continue
                            else
                                parse_Struct(c, struct_name)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind Namespace
                    parse_Namespace(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind EnumDecl
                    let enum_name = string(clang_getCursorSpelling(c))
                    let acs = clang_getCXXAccessSpecifier(c)
                    if acs != CX_CXXAccessSpecifier Public && acs != CX_CXXAccessSpecifier InvalidAccessSpecifier
                        to_log(LOG_INFO, "skipping enumeration {enum_name} due to access specifier {acs}\n")
                    elif empty(enum_name)
                        if hasEmptyEnum
                            to_log(LOG_ERROR, "already in empty enum mode from {clang_getCursorLocationDescription(emptyEnum)}, at {clang_getCursorLocationDescription(c)}\n")
                        emptyEnum = c
                        hasEmptyEnum = true
                    else
                        parse_Enum(c, enum_name)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind FieldDecl
                    if skip_type(clang_getCursorType(c))
                        let field_name = string(clang_getCursorSpelling(c))
                        to_log(LOG_INFO, "skipping {namespace_name(field_name)} becuase its type is skipped\n")
                        return CXChildVisitResult Continue
                    if clang_Cursor_isBitField(c)!=0u
                        let field_name = string(clang_getCursorSpelling(c))
                        to_log(LOG_INFO, "skipping {namespace_name(field_name)} because its a bitfield\n")
                        return CXChildVisitResult Continue
                    let acs = clang_getCXXAccessSpecifier(c)
                    if acs != CX_CXXAccessSpecifier Public
                        let field_name = string(clang_getCursorSpelling(c))
                        to_log(LOG_INFO, "skipping {namespace_name(field_name)} due to access specifier {acs}\n")
                        return CXChildVisitResult Continue
                    var field_type = clang_getCursorType(c)
                    if skip_type(field_type)
                        let field_name = string(clang_getCursorSpelling(c))
                        let field_type_name = string(clang_getTypeSpelling(field_type))
                        to_log(LOG_INFO, "skipping {namespace_name(field_name)} due to type {field_type_name}\n")
                    else
                        parse_Field(c)
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind VarDecl
                    /*
                    let var_decl_name = string(clang_getCursorSpelling(c))
                    var var_decl_type = clang_getCanonicalType(clang_getCursorType(c))
                    let var_decl_type_name = string(clang_getTypeSpelling(var_decl_type))
                    to_log(LOG_WARNING, "VarDecl {var_decl_name}:{var_decl_type_name} at {clang_getCursorLocationDescription(c)}\n")
                    */
                    return CXChildVisitResult Continue
                elif kind == CXCursorKind CXXAccessSpecifier
                    // nada?
                    return CXChildVisitResult Recurse
                elif kind == CXCursorKind UnexposedDecl
                    // nada?
                    return CXChildVisitResult Recurse
                elif kind == CXCursorKind FunctionTemplate
                    // nada?
                    CXChildVisitResult Continue
                /*
                elif kind == CXCursorKind TemplateTypeParameter
                    // nada?
                    CXChildVisitResult Continue
                elif kind == CXCursorKind TypeRef
                    // nada?
                    CXChildVisitResult Continue
                */
                elif kind == CXCursorKind CXXBaseSpecifier
                    let class_name = back(nspaces)
                    var tps = string(clang_getTypeSpelling(clang_getCursorType(c)))
                    var tpq <- split(tps,"::")  // TODO: better inheritance?
                    let base_name = back(tpq)
                    delete tpq
                    if !(lsp |> key_exists(class_name))
                        lsp[class_name] = base_name
                    else
                        var acs = clang_getCXXAccessSpecifier(c)
                        if acs == CX_CXXAccessSpecifier Public
                            interfaces[class_name] |> push(base_name)
                        else
                            to_log(LOG_WARNING, "skipping interface {base_name} due to access specifier {acs} at {clang_getCursorLocationDescription(c)}\n")
                    CXChildVisitResult Recurse
                elif kind == CXCursorKind ClassTemplate
                    let template_name = string(clang_getCursorSpelling(c))
                    to_log(LOG_WARNING, "skipping template {template_name} at {clang_getCursorLocationDescription(c)}\n")
                    CXChildVisitResult Continue
                else
                    to_log(LOG_WARNING, "unsupported {describe(c)} <- {describe(parent)} at {clang_getCursorLocationDescription(c)}\n")
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
                    to_log(LOG_WARNING, "skipping {DEF} due to name limitations\n")
                    return true
                var suf = suffix
                if hex && suffix=="u"
                    let v = to_uint64(VAL,true)
                    if v > 0xfffffffful
                        to_log(LOG_INFO, "{DEF} promoted to ul\n")
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
    include_parse_file = true   // need to add #include <PREFIX_FILE_NAME> to the generated file
    include_local_parse_file = false    // if false, #include <PREFIX_FILE_NAME> will be added to the generated file, otherwise #include "PREFIX_FILE_NAME"
    generate_lsp : bool = false
    override define_const_style_das = false
    enum_decl_file : FILE const?
    enum_decl_cast_file : FILE const?
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
    func_file_aot : FILE const?
    func_file_aot_decl : FILE const?
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
    method_name_index : int
    aot_alias : table<string; bool>
    already_defined : table<string; bool>
    nsst2st : table<string; string>
    st2nsst : table<string; string>
    require_modules : array<string>
    alias2type : table<string; string>
    enums_with_flags : table<string; bool>
    abstract_class : array<bool>
    wrapper_count : int
    def CppGenBind ( pfn,pfp:string; args:array<string> )
        init_args(pfn,pfp,args)
        setDefaultFiles()
    def setDefaultFiles
        enum_decl_file = fstdout()
        enum_decl_cast_file = fstdout()
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
        func_file_aot = fstdout()
        func_file_aot_decl = fstdout()
        module_cpp_file = fstdout()
        module_h_file = fstdout()
        module_need_h_file = fstdout()
        define_const_file = fstdout()
    def openFile ( path:string )
        var f = fopen(path,"wb")
        if f == null
            to_log(LOG_ERROR, "can't open CppGenBind output at {path}, defaulting to stdout\n")
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
            openFile("{bind_root}/{bind_module}.enum.decl.cast.inc"),
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
            openFile("{bind_root}/{bind_module}.func.decl.inc"),
            openFile("{bind_root}/{bind_module}.func.aot.inc"),
            openFile("{bind_root}/{bind_module}.func.aot.decl.inc"))
        setModuleFiles(
            openFile("{bind_root}/{bind_module}.h"),
            openFile("{bind_root}/need_{bind_module}.h"),
            openFile("{bind_root}/{bind_module}.cpp"))
        if generate_constant_decl
            define_const_file = openFile("{bind_root}/{bind_module}.const.inc")
    def closeAllFiles
        closeFile(enum_decl_file)
        closeFile(enum_decl_cast_file)
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
    def setEnumFiles ( edecl,edeclc,eclass,eadd:FILE const? )
        enum_decl_file = edecl
        enum_decl_cast_file = edeclc
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
    def setFuncFiles ( ffr,ffd,ffa,ffad:FILE const? )
        func_file_reg = ffr
        func_file_decl = ffd
        func_file_aot = ffa
        func_file_aot_decl = ffad
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
    def substFieldType ( var c : CXCursor ) : string     // note: namespaces form struct
        var t = clang_getCursorType(c)
        let tn = string(clang_getTypeSpelling(t))
        return alias2type?[tn] ?? ""
    def override parse_Field ( var c : CXCursor )
        let field_name = string(clang_getCursorSpelling(c))
        var field_type = clang_getCanonicalType(clang_getCursorType(c))
        let field_type_name = string(clang_getTypeSpelling(field_type))
        var wr = swrite |> back
        let substType = substFieldType(c)
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
        return if struct_name |> starts_with <| "(" // do nothing for anonymous unions/structs
        let struct_namespace_name = namespace_name(struct_name)
        nsst2st[struct_namespace_name] = struct_name
        st2nsst[struct_name] = struct_namespace_name
        let isAotAlias = aot_alias |> key_exists(struct_namespace_name)
        let isAlreadyDefined = already_defined |> key_exists(struct_namespace_name)
        let isForward = clang_isForwardDeclaration(c)
        var isForwardDummy = false
        if isForward
            var stdecl = clang_getTypeDeclaration(clang_getCursorType(c))
            isForwardDummy = clang_isForwardDeclaration(stdecl)
        if !struct_ann |> key_exists(struct_namespace_name)
            if !isAotAlias && !isAlreadyDefined
                if !isForward || isForwardDummy
                    fprint(struct_decl_file, "MAKE_EXTERNAL_TYPE_FACTORY({struct_name},{struct_namespace_name});\n")
                    fprint(struct_impl_file, "IMPLEMENT_EXTERNAL_TYPE_FACTORY({struct_name},{struct_namespace_name});\n")
        if isForward
            if isForwardDummy
                to_log(LOG_INFO, "Forward declaration {struct_namespace_name} at {clang_getCursorLocationDescription(c)}\n")
                struct_ann[struct_namespace_name] ||= false
            return
        struct_ann[struct_namespace_name] = true
        if isAotAlias || isAlreadyDefined
            return
        let dashing_name = namespace_name(struct_name,"_")
        let st = build_string() <| $ ( var writer:StringBuilderWriter )
            swrite |> push(unsafe(addr(writer)))
            writer |> write("// from {clang_getCursorLocationDescription(c)}\n")
            writer |> write("struct {dashing_name}_GeneratedAnnotation : ManagedStructureAnnotation<{struct_namespace_name}> \{\n")
            if isLocalType(struct_name,struct_namespace_name)
                writer |> write("\tvirtual bool isLocal() const override \{ return true; \}\n")
                writer |> write("\tvirtual bool canBePlacedInContainer() const override \{ return true; \}\n")
            writer |> write("\t{dashing_name}_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation (\"{struct_name}\", ml, \"{struct_namespace_name}\") \{\n")
            writer |> write("\t\}\n")
            writer |> write("\tvoid init () \{\n")
            let isAbstract = clang_CXXRecord_isAbstract(c)!=0u
            abstract_class |> push(isAbstract)
            AnyGenBind`parse_Struct(self, c, struct_name)
            abstract_class |> pop()
            writer |> write("\t\}\n\};\n")
            if isArgByValue(struct_namespace_name)
                fprint(struct_cast_file, "template <> struct cast_arg<{struct_namespace_name}> \{\n")
                fprint(struct_cast_file, "\tstatic __forceinline const {struct_namespace_name} & to ( Context & ctx, SimNode * node ) \{\n")
                fprint(struct_cast_file, "\t\tvec4f res = node->eval(ctx);\n")
                fprint(struct_cast_file, "\t\treturn * cast<{struct_namespace_name} *>::to(res);\n")
                fprint(struct_cast_file, "\t\}\n")
                fprint(struct_cast_file, "\};\n")
            swrite |> pop()
        fprint(struct_class_file, "{st}\n")
        fprint(struct_add_file,"auto ann_{struct_name} = make_smart<{dashing_name}_GeneratedAnnotation>(lib);\n")
        fprint(struct_add_file,"addAnnotation(ann_{struct_name});\n")
        fprint(struct_postadd_file,"initRecAnnotation(ann_{struct_name},lib);\n")
    def typeSpelling ( var c : CXType )
        let nspl = string(clang_getTypeSpelling(c))
        if nspl=="size_t" || nspl=="std::size_t" || nspl=="intptr_t" || nspl=="uintptr_t" || nspl=="size_t *"
            return nspl
        var t = clang_getCanonicalType(c)
        var spl = string(clang_getTypeSpelling(t))
        if c.kind == CXTypeKind Enum
            return enum2enum?[spl] ?? spl
        elif t.kind==CXTypeKind Long || t.kind==CXTypeKind LongLong || t.kind==CXTypeKind ULong || t.kind==CXTypeKind ULongLong
            return nspl // if its a fancy type, which may be different on different platform
        return spl
    def functionHeaderSpelling ( var c : CXCursor; isMethod,needMethodQualifier,needArgumentNames:bool; function_name, self_type:string )
        // TODO: calling convention?
        var fun_type = clang_getCursorType(c)
        var res_type = clang_getResultType(fun_type)
        let function_result_type = typeSpelling(res_type)
        let isStatic = clang_Cursor_getStorageClass(c)==CX_StorageClass Static
        return build_string() <| $ ( str )
            str |> write("{function_result_type} {function_name}(")
            if !empty(self_type)
                if isMethod && (clang_CXXMethod_isConst(c)!=0u || isStatic)
                    str |> write("const ")
                str |> write("{self_type} &")
                if needArgumentNames && !isStatic
                    str |> write(" self")
            let narg = clang_Cursor_getNumArguments(c)
            for ai in urange(narg)
                if ai != 0u || !empty(self_type)
                    str |> write(",")
                var carg = clang_Cursor_getArgument(c, ai)
                var arg_type = clang_getCursorType(carg)
                let argument_type = typeSpelling(arg_type)
                str |> write("{argument_type}")
                if needArgumentNames
                    var arg_name = string(clang_getCursorSpelling(carg))
                    if empty(arg_name)
                        arg_name = "_arg_{int(ai)}"
                    str |> write(" {arg_name}")
            str |> write(")")
            if isMethod && needMethodQualifier && clang_CXXMethod_isConst(c)!=0u
                str |> write(" const")
    def functionPtrSpelling ( var c : CXCursor; isMethod:bool )
        let function_name = build_string() <| $ ( str )
            str |> write("(")
            if isMethod
                for n in nspaces
                    str |> write(n)
                    str |> write("::")
            str |> write("*)")
        return functionHeaderSpelling(c, isMethod, true, false, function_name, "")
    def functionWrapperSpelling ( var c : CXCursor; self_type:string )
        return functionHeaderSpelling(c, true, false, false, "(*)", self_type )
    def methodName
        method_name_index ++
        return "_method_{method_name_index}"
    def override parse_Method ( var c : CXCursor )
        parse_anyFunctionDecl(c,true)
    def override parse_FunctionDecl ( var c : CXCursor )
        parse_anyFunctionDecl(c,false)
    def rename_function ( name,nsname:string )
        return name
    def isAotAliasType ( var t : CXType )
        if t.kind == CXTypeKind Record || t.kind == CXTypeKind Typedef
            let tname = string(clang_getTypeSpelling(t))
            if aot_alias |> key_exists(tname)
                return true
        // let tname = string(clang_getTypeSpelling(t))
        // print("NOT AOT ALIAS {tname} {t.kind}\n")
        return false
    def step_Function
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
    def gen_functionWrapper ( var c : CXCursor; self_type:string )
        let function_name = string(clang_getCursorSpelling(c))
        let wrapper_index = wrapper_count ++
        let isStatic = clang_Cursor_getStorageClass(c)==CX_StorageClass Static
        let static_name = isStatic ? "static" : "virtual"
        let wrapper_name = "_{bind_module}_{static_name}_{wrapper_index}_{function_name}"
        let header = functionHeaderSpelling(c,true,false,true,wrapper_name,self_type)
        fprint(func_file_aot_decl, "{header};\n")
        fprint(func_file_aot, "{header} \{\n\t")
        var fun_type = clang_getCursorType(c)
        var res_type = clang_getCanonicalType(clang_getResultType(fun_type))
        if res_type.kind != CXTypeKind Void
            fprint(func_file_aot,"return ")
        if isStatic
            fprint(func_file_aot,"{self_type}::{function_name}(")
        else
            fprint(func_file_aot,"self.{function_name}(")
        let narg = clang_Cursor_getNumArguments(c)
        for ai in urange(narg)
            if ai != 0u
                fprint(func_file_aot,",")
            var carg = clang_Cursor_getArgument(c, ai)
            var arg_name = string(clang_getCursorSpelling(carg))
            if empty(arg_name)
                arg_name = "_arg_{int(ai)}"
            fprint(func_file_aot,"{arg_name}")
        fprint(func_file_aot,");\n\}\n\n")
        return wrapper_name
    def get_module_temp_fn: string
        return ""
    def parse_anyFunctionDecl ( var c : CXCursor; isMethod : bool )
        step_Function()
        let isStatic = clang_Cursor_getStorageClass(c)==CX_StorageClass Static
        let function_name = string(clang_getCursorSpelling(c))
        var das_function_name = function_name
        var isOperator = false
        if function_name |> starts_with("operator")
            let Ch = character_at(function_name,8)
            if !(is_alpha(Ch) || is_number(Ch) || Ch=='_')
                das_function_name = function_name |> slice(8)
                isOperator = true
        let isVirtual = clang_CXXMethod_isVirtual(c)!=0u
        var fun_type = clang_getCanonicalType(clang_getCursorType(c))
        var res_type = clang_getResultType(fun_type)
        var nc_res_type = clang_getResultType(clang_getCursorType(c))
        let function_cpp_name = namespace_name(function_name)
        das_function_name = rename_function(das_function_name,function_cpp_name)
        var bind_enchantation = ""
        var cpp_enchantation = ""
        var extra_enchantation = ", SimNode_ExtFuncCall "
        if clang_das_isRef(res_type)
            extra_enchantation = ", SimNode_ExtFuncCallRef "
        elif clang_das_isCopyOnReturn(res_type) && !isAotAliasType(nc_res_type)
            extra_enchantation = ", SimNode_ExtFuncCallAndCopyOrMove "
        if isMethod
            let using_mname = methodName()
            if isVirtual || isStatic
                let self_type = namespace_struct_name()
                let function_cpp_type = functionWrapperSpelling(c,self_type)
                let wrapper_cpp_name = gen_functionWrapper(c,self_type)
                bind_enchantation = " {function_cpp_type} , {wrapper_cpp_name} "
                cpp_enchantation = wrapper_cpp_name
            else
                let function_cpp_type = functionPtrSpelling(c,true)
                let using_mname_expr = "das::das_call_member< {function_cpp_type},&{function_cpp_name} >"
                bind_enchantation = "DAS_CALL_METHOD({using_mname})"
                cpp_enchantation = "das_call_member< {function_cpp_type} , &{function_cpp_name} >::invoke"
                fprint(func_file,"\tusing {using_mname} = {using_mname_expr};\n");
        else
            let function_cpp_type = functionPtrSpelling(c,false)
            bind_enchantation = " {function_cpp_type} , {function_cpp_name} "
            cpp_enchantation = function_cpp_name
        fprint(func_file, "// from {clang_getCursorLocationDescription(c)}\n")
        fprint(func_file, "\tmakeExtern<{bind_enchantation}{extra_enchantation}{get_module_temp_fn()}>(lib,\"{das_function_name}\",\"{cpp_enchantation}\")")
        let narg = clang_Cursor_getNumArguments(c)
        if narg!=0 || isMethod
            fprint(func_file,"\n\t\t->args(\{")
            if isMethod
                fprint(func_file,"\"self\"")
            for ai in urange(narg)
                if ai != 0u || isMethod
                    fprint(func_file,",")
                var carg = clang_Cursor_getArgument(c, ai)
                let arg_name = string(clang_getCursorSpelling(carg))
                fprint(func_file,"\"{arg_name}\"")
            fprint(func_file,"\})")
            for ai in urange(narg)
                let aii = int(isMethod ? ai+1u : ai)
                var carg = clang_Cursor_getArgument(c, ai)
                let cp = getCursorArgType(clang_getCursorType(carg))     // non-canonical
                if cp != ""
                    fprint(func_file,"\n\t\t->arg_type({aii},makeType<{cp}>(lib))")
                let cv = getCursorInit(carg,cp)
                if cv != ""
                    fprint(func_file,"\n\t\t->arg_init({aii},{cv})")
        let rp = getCursorArgType(nc_res_type)
        if rp != ""
            fprint(func_file,"\n\t\t->res_type(makeType<{rp}>(lib))")
		fprint(func_file, "\n\t\t->addToModule(*this, SideEffects::worstDefault)")
        fprint(func_file,";\n")
    def override parse_Constructor ( var c : CXCursor )
        let ctor_name = string(clang_getCursorSpelling(c))
        if abstract_class |> back
            to_log(LOG_WARNING, "skipping abstract class constructor {ctor_name} at {clang_getCursorLocationDescription(c)}\n")
            return
        step_Function()
        let function_cpp_name = namespace_struct_name()
        fprint(func_file,"\taddCtorAndUsing<{function_cpp_name}")
        let narg = clang_Cursor_getNumArguments(c)
        for ai in urange(narg)
            var carg = clang_Cursor_getArgument(c, ai)
            var ct = clang_getCursorType(carg)
            var cp = getCursorArgType(ct)
            if cp == ""
                cp = string(clang_getTypeSpelling(clang_getCanonicalType(ct)))
            fprint(func_file,",{cp}")
        fprint(func_file,">(*this,lib,\"{ctor_name}\",\"{function_cpp_name}\")")
        if narg!=0
            fprint(func_file,"\n\t\t->args(\{")
            for ai in urange(narg)
                if ai != 0u
                    fprint(func_file,",")
                var carg = clang_Cursor_getArgument(c, ai)
                let arg_name = string(clang_getCursorSpelling(carg))
                fprint(func_file,"\"{arg_name}\"")
            fprint(func_file,"\})")
        for ai in urange(narg)
            let aii = int(ai)
            var carg = clang_Cursor_getArgument(c, ai)
            var ct = clang_getCursorType(carg)
            var cp = getCursorArgType(ct)
            let cv = getCursorInit(carg,cp)
            if cv != ""
                fprint(func_file,"\n\t\t->arg_init({aii},{cv})")
        fprint(func_file,";\n")
    def getCursorArgType ( var t : CXType )
        let tn = string(clang_getTypeSpelling(t))
        return alias2type?[tn] ?? ""
    def getCursorInit ( var c : CXCursor; this_alias:string )
        var result = ""
        var t = clang_getCanonicalType(clang_getCursorType(c))
        var res = clang_Cursor_Evaluate(c)
        let res_kind = clang_EvalResult_getKind(res)
        if res_kind != CXEvalResultKind UnExposed
            if t.kind==CXTypeKind Char32 || t.kind==CXTypeKind Int
                let value = clang_EvalResult_getAsInt(res)
                if this_alias != ""
                    result = "make_smart<ExprConstEnumeration>({value},makeType<{this_alias}>(lib))"
                else
                    result = "make_smart<ExprConstInt>({value})"
            elif t.kind==CXTypeKind UInt
                let value = uint(clang_EvalResult_getAsInt(res))
                result = "make_smart<ExprConstUInt>({value})"
            elif t.kind==CXTypeKind Long || t.kind==CXTypeKind LongLong
                let value = int64(clang_EvalResult_getAsLongLong(res))
                result = "make_smart<ExprConstInt64>({value})"
            elif t.kind==CXTypeKind ULong || t.kind==CXTypeKind ULongLong
                let value = uint64(clang_EvalResult_getAsLongLong(res))
                result = "make_smart<ExprConstUInt64>({value})"
            elif t.kind==CXTypeKind Float
                let value = clang_EvalResult_getAsDouble(res)
                result = "make_smart<ExprConstFloat>({value})"
            elif t.kind==CXTypeKind Double
                let value = clang_EvalResult_getAsDouble(res)
                result = "make_smart<ExprConstDouble>({value})"
            elif t.kind==CXTypeKind Bool
                let value = clang_EvalResult_getAsInt(res)
                result = "make_smart<ExprConstBool>({value != 0})"
            elif t.kind==CXTypeKind Pointer
                var pt = clang_getCanonicalType(clang_getPointeeType(t))
                if pt.kind==CXTypeKind Char_S || pt.kind==CXTypeKind SChar
                    let value = clang_EvalResult_getAsStr(res)
                    result = "make_smart<ExprConstString>(\"{value}\")"
                else
                    to_log(LOG_WARNING, "unsupported default value of pointer type {string(clang_getTypeSpelling(t))} at {clang_getCursorLocationDescription(c)}\n")
            elif t.kind == CXTypeKind Enum
                let value = clang_EvalResult_getAsInt(res)
                let thisArgEnum = string(clang_getTypeSpelling(t))
                result = "make_smart<ExprConstEnumeration>({value},makeType<{thisArgEnum}>(lib))"
            else
                to_log(LOG_WARNING, "unsupported default value of type {string(clang_getTypeSpelling(t))} : {t.kind} at {clang_getCursorLocationDescription(c)}\n")
        else
            // this feels hacky, whoever regular eval results returns nada for foo ( bool * a = NULL )
            //    for the pointer (and pointer only) we walk everything under
            //      if we see CXXNullPtrLiteralExpr - we call it nullptr
            //      if we see IntegerLiteral - we check if it evals to 0, and if it does - we call it NULL
            if t.kind==CXTypeKind Pointer
                var has_null = false
                clang_visitChildren(c) <| $ ( var CC,parent )
                    if CC.kind==CXCursorKind IntegerLiteral
                        var nres = clang_Cursor_Evaluate(CC)
                        let nres_kind = clang_EvalResult_getKind(nres)
                        if nres_kind != CXEvalResultKind UnExposed
                            let value = clang_EvalResult_getAsInt(nres)
                            if value == 0
                                has_null = true
                        clang_EvalResult_dispose(nres)
                        return CXChildVisitResult Break
                    elif CC.kind==CXCursorKind CXXNullPtrLiteralExpr
                        has_null = true
                        return CXChildVisitResult Break
                    return CXChildVisitResult Recurse
                if has_null
                    var pt = clang_getCanonicalType(clang_getPointeeType(t))
                    if pt.kind==CXTypeKind Char_S || pt.kind==CXTypeKind SChar
                        result = "make_smart<ExprConstString>(\"\")"
                    else
                        result = "make_smart<ExprConstPtr>()"
        clang_EvalResult_dispose(res)
        return result
    def enum_entry_name ( en, enn:string ) : string
        let prefix = enum_prefix?[en] ?? en
        let uprefix = to_upper(prefix)
        var name = enn
        if enn|>starts_with("{prefix}_") || enn|>starts_with("{uprefix}_")
            name = enn |> slice(length(prefix)+1)
        elif enn|>starts_with(prefix) || enn|>starts_with(uprefix)
            name = enn |> slice(length(prefix))
        return unsafe_names?[name] ?? name
    def dasEnumName(enum_name,cpp_enum_name:string) : string
        var das_enum_name = enum_name
        if das_enum_name |> ends_with("_")
            das_enum_name = das_enum_name |> slice(0,-1)
        return das_enum_name
    def generateEnumDecl
        for decl in enumDecl
            let cppEnumName = enum2enum?[decl.ns_en] ?? decl.ns_en
            let das_enum_name = dasEnumName(decl.enum_name,cppEnumName)
            fprint(enum_decl_cast_file, "DAS_BIND_ENUM_CAST({cppEnumName});\n")
            fprint(enum_decl_file, "DAS_BASE_BIND_ENUM_GEN({cppEnumName},{das_enum_name});\n\n")
            if enums_with_flags |> key_exists(cppEnumName)
                fprint(enum_add_file,"addEnumFlagOps<{cppEnumName}>(*this,lib,\"{cppEnumName}\");\n");
    def skip_enum ( ns_en, en : string )
        return false
    def override parse_Enum ( var cursor : CXCursor; enum_name : string )
        let ns_en = namespace_name(enum_name)
        let das_enum_name = dasEnumName(enum_name,ns_en)
        if skip_enum(ns_en,enum_name)
            return
        enumDecl |> push([[auto ns_en,enum_name]])
        let dashing_name = namespace_name(enum_name,"_")
        fprint(enum_class_file, "// from {clang_getCursorLocationDescription(cursor)}\n")
        fprint(enum_class_file, "class Enumeration_{dashing_name} : public das::Enumeration \{\n")
        fprint(enum_class_file, "public:\n")
        fprint(enum_class_file, "\tEnumeration_{dashing_name}() : das::Enumeration(\"{das_enum_name}\") \{\n")
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
        fprint(enum_add_file, "addEnumeration(make_smart<Enumeration_{dashing_name}>());\n");
    def override parse_Alias ( var c : CXCursor )
        let tdn = string(clang_getCursorDisplayName(c))
        var tdt = clang_getCanonicalType(clang_getCursorType(c))
        let tdts = string(clang_getTypeSpelling(tdt))
        fprint(alias_add_file, "// from {clang_getCursorLocationDescription(c)}\n")
        fprint(alias_add_file, "auto alias_{tdn} = typeFactory<{tdts}>::make(lib);\n")
        fprint(alias_add_file, "alias_{tdn}->alias = \"{tdn}\";\n")
        fprint(alias_add_file, "addAlias(alias_{tdn});\n\n")
    def override generate
        AnyGenBind`generate(self)
        generateEnumDecl()
        generateDummy()
        if generate_lsp
            generateStructLsp()
            generateStructInterfaces()
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
                if aot_alias |> key_exists(k) || already_defined |> key_exists(k)
                    continue
                let kname = nsst2st?[k] ?? "MissingDummy"
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
        fwrite(func_file, "#include \"{bind_module}.func.aot.decl.inc\"\n")
        fwrite(func_file, "void Module_{bind_module}::initFunctions_{func_chunk_index}() \{\n")
    def writeFuncPostfix
        fwrite(func_file, "\}\n\}\n\n")
    def generateModuleNeedHPrefix
        pass
    def generateModuleNeedH
        module_need_h_file |> fwrite("#pragma once\n")
        generateModuleNeedHPrefix()
        if include_parse_file
            if include_local_parse_file
                module_need_h_file |> fwrite("#include \"{PARSE_FILE_NAME}\"\n")
            else
                module_need_h_file |> fwrite("#include <{PARSE_FILE_NAME}>\n")
        module_need_h_file |> fwrite("#include \"{bind_module}.enum.decl.inc\"\n")
        module_need_h_file |> fwrite("#include \"{bind_module}.enum.decl.cast.inc\"\n")
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
        module_cpp_file |> fwrite("#include \"{bind_module}.func.aot.inc\"\n")
        module_cpp_file |> fwrite("Module_{bind_module}::Module_{bind_module}() : Module(\"{bind_das_module}\") \{\n")  // TODO: das module name
        module_cpp_file |> fwrite("\}\n")
        module_cpp_file |> fwrite("bool Module_{bind_module}::initDependencies() \{\n")
        module_cpp_file |> fwrite("\tif ( initialized ) return true;\n")
        for rm in require_modules
            module_cpp_file |> fwrite("\tauto mod_{rm} = Module::require(\"{rm}\");\n")
            module_cpp_file |> fwrite("\tif ( !mod_{rm} ) return false;\n")
            module_cpp_file |> fwrite("\tif ( !mod_{rm}->initDependencies() ) return false;\n")
        module_cpp_file |> fwrite("\tinitialized = true;\n")
        module_cpp_file |> fwrite("\tlib.addModule(this);\n")
        module_cpp_file |> fwrite("\tlib.addBuiltInModule();\n")
        for rm in require_modules
            module_cpp_file |> fwrite("\tlib.addModule(mod_{rm});\n")
        if length(aot_alias)!=0
            module_cpp_file |> fwrite("\tinitAotAlias();\n")
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
        if length(aot_alias)!=0
            module_h_file |> fwrite("\tvoid initAotAlias ();\n")
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
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.enum.decl.cast.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.add.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.postadd.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.class.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.decl.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.impl.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.struct.cast.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.func.reg.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.func.decl.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.func.aot.inc\n")
        print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.func.aot.decl.inc\n")
        if generate_constant_decl
            print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.const.inc\n")
        print("\t# generated functions\n")
        for index in range(1,func_chunk_index+1)
            print("\t\t$\{{MODULE_VAR}_DIR\}/src/{bind_module}.func_{index}.cpp\n")
        print("\t)\n\n")
    def generateStructLsp
        for k,v in keys(lsp),values(lsp)
            fprint(struct_add_file,"setParents(this,\"{k}\",  \{")
            var pp : array<string>
            pp |> push(v)
            var qv = v
            while lsp |> key_exists(qv)
                qv = lsp[qv]
                pp |> push(qv)
            for p,i in pp,count()
                if i!=0
                    fprint(struct_add_file,",")
                fprint(struct_add_file,"\"{p}\"")
            fprint(struct_add_file,"\});\n")
    def generateStructInterfaces
        for k,v in keys(interfaces),values(interfaces)
            for i in v
                step_Function()
                let ct = st2nsst[k]
                let it = st2nsst[i]
                fprint(func_file,"\taddExtern< decltype(&with_interface<{ct},{it}>), with_interface<{ct},{it}> >")
                fprint(func_file, "(*this,lib,\"interface\",SideEffects::invoke,\"with_interface<{ct},{it}>\");\n")

def clang_reportAst(PARSE_FILE_NAME, PARSE_FILE_PREFIX:string; ARGV:array<string>)
    to_log(LOG_INFO,"--------------------------------------------------------------------------------------------------------\n")
    to_log(LOG_INFO,"// {PARSE_FILE_PREFIX}{PARSE_FILE_NAME} {ARGV}\n")
    var index = clang_createIndex(1,1)
    var unit = clang_parseTranslationUnit(
        index,
        "{PARSE_FILE_PREFIX}{PARSE_FILE_NAME}",
        unsafe(addr(ARGV[0])), length(ARGV),
        null, 0u,
        CXTranslationUnit_SkipFunctionBodies)
    if unit==null
        panic("unable to parse translation unit {PARSE_FILE_PREFIX}{PARSE_FILE_NAME}")
    var cursor = clang_getTranslationUnitCursor(unit)
    var prevFileName : string
    clang_visitChildren(cursor) <| $ ( var c,parent )
        var file : CXFile
        var line, column, offset : uint
        clang_getSpellingLocation(clang_getCursorLocation(c), safe_addr(file), safe_addr(line), safe_addr(column), safe_addr(offset))
        var fname = string(clang_getFileName(file))
        if fname |> ends_with(PARSE_FILE_NAME)
            to_log(LOG_INFO, "{describe(c)} <- {describe(parent)} at {clang_getCursorLocationDescription(c)}\n")
        return CXChildVisitResult Recurse
    to_log(LOG_INFO,"--------------------------------------------------------------------------------------------------------\n")
    let ndiag = clang_getNumDiagnostics(unit)
    for i in urange(ndiag)
        var diag = clang_getDiagnostic(unit, i)
        var diags = string(clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions()))
        print("{diags}\n")
    clang_disposeTranslationUnit(unit)
    clang_disposeIndex(index)
    if ndiag != 0u
        to_log(LOG_INFO,"--------------------------------------------------------------------------------------------------------\n")
