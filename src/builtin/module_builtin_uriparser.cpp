#include "daScript/misc/platform.h"

#include "module_builtin_uriparser.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_builtin_string.h"
#include "daScript/simulate/aot_builtin_uriparser.h"

#include "daScript/daScriptBind.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/hash.h"
#include "daScript/misc/uric.h"

#ifndef URI_STATIC_BUILD
#define URI_STATIC_BUILD
#endif
#include "uriparser/Uri.h"

IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriTextRangeA,UriTextRangeA)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriIp4Struct,UriIp4Struct)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriIp6Struct,UriIp6Struct)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriHostDataA,UriHostDataA)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriPathSegmentStructA,UriPathSegmentStructA)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriUriA,UriUriA)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(Uri,das::Uri)


// We shall honor ITU-T Recommendation X.667 and keep our generated uuids lowercase:
// 6.5.4 Software generating the hexadecimal representation of a UUID shall not use upper case letters.
// https://www.itu.int/rec/T-REC-X.667-201210-I/en

#if defined(_MSC_VER) && !defined(_GAMING_XBOX) && !defined(_DURANGO)

#include <rpc.h>
#pragma comment(lib,"Rpcrt4")

char * das::makeNewGuid( das::Context * context, LineInfoArg * at ) {
    UUID id;
    if ( UuidCreate(&id)!=RPC_S_OK ) context->throw_error_at(at, "can't create UUID");
    CHAR* uuidstr = NULL;
    if ( UuidToStringA(&id, (RPC_CSTR *)&uuidstr)!=RPC_S_OK ) context->throw_error_at(at, "can't convert UUID to string");
    // UuidToStringA is not guaranteed to yield a lowercase string, even though currently it does
    das::builtin_string_tolower_in_place(uuidstr);
    char * res = context->allocateString(uuidstr, at);
    RpcStringFreeA((RPC_CSTR *)&uuidstr);
    return res;
}

#elif defined(__linux__) && defined(LINUX_UUID) || defined __HAIKU__ || defined(__APPLE__)

#include <uuid/uuid.h>

// older versions of libuuid do not define that
#ifndef UUID_STR_LEN
#define UUID_STR_LEN 37
#endif

char * das::makeNewGuid( das::Context * context, LineInfoArg * at ) {
    uuid_t uuid;
    char uuidStr[UUID_STR_LEN];
    uuid_generate(uuid);
    uuid_unparse_lower(uuid, uuidStr);
    return context->allocateString(uuidStr, at);
}

#else

char * das::makeNewGuid( Context * context, LineInfoArg * at ) {
    context->throw_error_at(at, "GUID generation is not implemented for this platform");
    return nullptr;
}

#endif
namespace das {

char * uri_to_unix_file_name ( char * uristr, Context * context, LineInfoArg * at ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[len + 1];
    char * result = nullptr;
    if ( uriUriStringToUnixFilenameA(uristr, buf) == URI_SUCCESS ) {
        result = context->allocateString(buf, uint32_t(strlen(buf)), at);
    }
    delete [] buf;
    return result;
}

char * uri_to_windows_file_name ( char * uristr, Context * context, LineInfoArg * at ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[len + 1];
    char * result = nullptr;
    if ( uriUriStringToWindowsFilenameA(uristr, buf) == URI_SUCCESS ) {
        result = context->allocateString(buf, uint32_t(strlen(buf)), at);
    }
    delete [] buf;
    return result;
}

char * unix_file_name_to_uri ( char * uristr, Context * context, LineInfoArg * at ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[3 * len + 1];
    char * result = nullptr;
    if ( uriUnixFilenameToUriStringA(uristr, buf) == URI_SUCCESS ) {
        result = context->allocateString(buf, uint32_t(strlen(buf)), at);
    }
    delete [] buf;
    return result;
}

char * windows_file_name_to_uri ( char * uristr, Context * context, LineInfoArg * at ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[8 + 3 * len + 1];
    char * result = nullptr;
    if ( uriWindowsFilenameToUriStringA(uristr, buf) == URI_SUCCESS ) {
        result = context->allocateString(buf, uint32_t(strlen(buf)), at);
    }
    delete [] buf;
    return result;
}

char * escape_uri ( char * uristr, bool spaceToPlus, bool normalizeBreaks, Context * context, LineInfoArg * at ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[len*6];
    char * result = nullptr;
    if ( uriEscapeA(uristr, buf, spaceToPlus, normalizeBreaks) ) {
        result = context->allocateString(buf, uint32_t(strlen(buf)), at);
    }
    delete [] buf;
    return result;
}

char * unescape_uri ( char * uristr,Context * context, LineInfoArg * at ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    char * buf = new char[len+1];
    memcpy(buf, uristr, len);
    buf[len] = 0;
    char * result = nullptr;
    if ( uriUnescapeInPlaceA(buf) ) {
        result = context->allocateString(buf, uint32_t(strlen(buf)), at);
    }
    delete [] buf;
    return result;
}

char* normalize_uri(char* uristr, Context* context, LineInfoArg * at) {
    if (!uristr) return nullptr;
    UriUriA uri;
    char* result = nullptr;
    if (uriParseSingleUriA(&uri, uristr, nullptr) != URI_SUCCESS) {
        return result;
    }
    const unsigned int dirtyParts = uriNormalizeSyntaxMaskRequiredA(&uri);
    if (uriNormalizeSyntaxExA(&uri, dirtyParts) != URI_SUCCESS) {
        uriFreeUriMembersA(&uri);
        return result;
    }
    int charsRequired;
    if (uriToStringCharsRequiredA(&uri, &charsRequired) != URI_SUCCESS) {
        uriFreeUriMembersA(&uri);
        return result;
    }
    charsRequired++;
    char* buf = new char[charsRequired];
    if (uriToStringA(buf, &uri, charsRequired, nullptr) == URI_SUCCESS) {
        result = context->allocateString(buf, uint32_t(strlen(buf)), at);
    }
    delete[] buf;
    uriFreeUriMembersA(&uri);
    return result;
}

struct UriTextRangeAAnnotation : ManagedStructureAnnotation<UriTextRangeA> {
    UriTextRangeAAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriTextRangeA>("UriTextRangeA",ml) {
    }
    virtual void walk(DataWalker & walker, void * data) override {
        auto tr = (UriTextRangeA *) data;
        string str = string(tr->first, tr->afterLast - tr->first);
        char * strc = (char *) str.c_str();
        walker.String(strc);
    }
};

struct UriIp4StructAnnotation : ManagedStructureAnnotation<UriIp4Struct> {
    UriIp4StructAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriIp4Struct>("UriIp4Struct",ml) {
        DAS_ADD_FIELD_BIND(data);
    }
};

struct UriIp6StructAnnotation : ManagedStructureAnnotation<UriIp6Struct> {
    UriIp6StructAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriIp6Struct>("UriIp6Struct",ml) {
        DAS_ADD_FIELD_BIND(data);
    }
};

struct UriHostDataAAnnotation : ManagedStructureAnnotation<UriHostDataA> {
    UriHostDataAAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriHostDataA>("UriHostDataA",ml) {
        DAS_ADD_FIELD_BIND(ip4);
        DAS_ADD_FIELD_BIND(ip6);
        DAS_ADD_FIELD_BIND(ipFuture);
    }
};

struct UriPathSegmentStructAAnnotation : ManagedStructureAnnotation<UriPathSegmentStructA> {
    UriPathSegmentStructAAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriPathSegmentStructA>("UriPathSegmentStructA",ml) {
    }
    void init () {
        DAS_ADD_FIELD_BIND(text);
        DAS_ADD_FIELD_BIND(next);
    }
};

struct UriUriAAnnotation  : ManagedStructureAnnotation<UriUriA> {
    UriUriAAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriUriA>("UriUriA",ml) {
        DAS_ADD_FIELD_BIND(scheme);
        DAS_ADD_FIELD_BIND(userInfo);
        DAS_ADD_FIELD_BIND(hostText);
        DAS_ADD_FIELD_BIND(hostData);
        DAS_ADD_FIELD_BIND(portText);
        DAS_ADD_FIELD_BIND(pathHead);
        DAS_ADD_FIELD_BIND(pathTail);
        DAS_ADD_FIELD_BIND(query);
        DAS_ADD_FIELD_BIND(fragment);
        DAS_ADD_FIELD_BIND(absolutePath);
        DAS_ADD_FIELD_BIND(owner);
    }
};

struct UriAnnotation  : ManagedStructureAnnotation<Uri> {
    UriAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<Uri>("Uri",ml) {
        DAS_ADD_PROP_BIND("empty", empty);
        DAS_ADD_PROP_BIND("size", size);
        DAS_ADD_PROP_BIND("status", status);
        DAS_ADD_FIELD_BIND(uri);
    }
    virtual bool canMove() const override { return true; }
};

void delete_uri ( Uri & uri ) {
    uri.reset();
}

void clone_uri ( Uri & uri, const Uri & uriS ) {
    uri = uriS;
}

char * uri_to_string ( const Uri & uri, Context * context, LineInfoArg * at ) {
    return context->allocateString(uri.str(), at);
}

char * text_range_to_string ( const UriTextRangeA & trange, Context * context, LineInfoArg * at ) {
    if ( auto slen = trange.afterLast - trange.first ) {
        return context->allocateString(trange.first, uint32_t(slen), at);
    } else {
        return nullptr;
    }
}

char * to_unix_file_name ( const Uri & uri, Context * context, LineInfoArg * at ) {
    return context->allocateString(uri.toUnixFileName(), at);
}

char * to_windows_file_name ( const Uri & uri, Context * context, LineInfoArg * at ) {
    return context->allocateString(uri.toWindowsFileName(), at);
}

char * to_file_name ( const Uri & uri, Context * context, LineInfoArg * at ) {
    return context->allocateString(uri.toFileName(), at);
}

Uri from_file_name ( const char * str ) {
    Uri uri;
    if ( !str ) uri.fromFileNameStr(str);
    return uri;
}

Uri from_windows_file_name ( const char * str ) {
    Uri uri;
    if ( str ) uri.fromWindowsFileNameStr(str);
    return uri;
}

Uri from_unix_file_name ( const char * str ) {
    Uri uri;
    if ( str ) uri.fromUnixFileNameStr(str);
    return uri;
}

void uri_for_each_query_kv ( const Uri & uri, const TBlock<void,TTemporary<char *>,TTemporary<char*>> & blk, Context * context, LineInfoArg * at ) {
    for ( const auto & kv : uri.query() ) {
        vec4f args[2];
        args[0] = cast<char *>::from(kv.first.c_str());
        args[1] = cast<char *>::from(kv.second.c_str());
        context->invoke(blk, args, nullptr, at);
    }
}

class Module_UriParser : public Module {
public:
    Module_UriParser() : Module("uriparser") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        // uri
        addAnnotation(make_smart<UriTextRangeAAnnotation>(lib));
        addAnnotation(make_smart<UriIp4StructAnnotation>(lib));
        addAnnotation(make_smart<UriIp6StructAnnotation>(lib));
        addAnnotation(make_smart<UriHostDataAAnnotation>(lib));
        auto psa = make_smart<UriPathSegmentStructAAnnotation>(lib);
        addAnnotation(psa);
        initRecAnnotation(psa,lib);
        addAnnotation(make_smart<UriUriAAnnotation>(lib));
        addAnnotation(make_smart<UriAnnotation>(lib));
        addCtorAndUsing<Uri>(*this,lib,"Uri","Uri");
        addCtorAndUsing<Uri,const char *>(*this,lib,"Uri","Uri");
        DAS_ADD_FUN_BIND("finalize", modifyArgument, das::delete_uri)->args({"uri"});
        DAS_ADD_FUN_BIND("clone", modifyArgument, das::clone_uri)->args({"dest","src"});
        DAS_ADD_METHOD_BIND_VALUE_RET("strip_uri", none, das::Uri::strip)->args({"uri","query","fragment"});
        DAS_ADD_METHOD_BIND_VALUE_RET("add_base_uri", none, das::Uri::addBaseUri)->args({"base","relative"});
        DAS_ADD_METHOD_BIND_VALUE_RET("remove_base_uri", none, das::Uri::removeBaseUri)->args({"base","relative"});
        DAS_ADD_METHOD_BIND("normalize", modifyArgument, das::Uri::normalize)->args({"uri"});
        DAS_ADD_FUN_BIND("string", none, das::uri_to_string)->args({"uri","context","at"});
        DAS_ADD_FUN_BIND("string", none, das::text_range_to_string)->args({"range","context","at"});
        DAS_ADD_FUN_BIND("to_unix_file_name", none, das::to_unix_file_name)->args({"uri","context","at"});
        DAS_ADD_FUN_BIND("to_windows_file_name", none, das::to_windows_file_name)->args({"uri","context","at"});
        DAS_ADD_FUN_BIND("to_file_name", none, das::to_file_name)->args({"uri","context","at"});
        DAS_ADD_FUN_BIND_VALUE_RET("uri_from_file_name", none, das::from_file_name)->args({"filename"});
        DAS_ADD_FUN_BIND_VALUE_RET("uri_from_windows_file_name", none, das::from_windows_file_name)->args({"filename"});
        DAS_ADD_FUN_BIND_VALUE_RET("uri_from_unix_file_name", none, das::from_unix_file_name)->args({"filename"});
        DAS_ADD_FUN_BIND("uri_for_each_query_kv", invoke, das::uri_for_each_query_kv)->args({"uri","block","context","lineinfo"});
        // guid
        DAS_ADD_FUN_BIND("make_new_guid", accessExternal, das::makeNewGuid)->args({"context","at"});
        DAS_ADD_FUN_BIND("uri_to_unix_file_name", none, das::uri_to_unix_file_name)->args({"uriStr","context","at"});
        DAS_ADD_FUN_BIND("uri_to_windows_file_name", none, das::uri_to_windows_file_name)->args({"uriStr","context","at"});
        DAS_ADD_FUN_BIND("unix_file_name_to_uri", none, das::unix_file_name_to_uri)->args({"uriStr","context","at"});
        DAS_ADD_FUN_BIND("windows_file_name_to_uri", none, das::windows_file_name_to_uri)->args({"uriStr","context","at"});
#ifdef _WIN32
        DAS_ADD_FUN_BIND("uri_to_file_name", none, das::uri_to_windows_file_name)->args({"uriStr","context","at"});
        DAS_ADD_FUN_BIND("file_name_to_uri", none, das::windows_file_name_to_uri)->args({"uriStr","context","at"});
#else
        DAS_ADD_FUN_BIND("uri_to_file_name", none, das::uri_to_unix_file_name)->args({"uriStr","context","at"});
        DAS_ADD_FUN_BIND("file_name_to_uri", none, das::unix_file_name_to_uri)->args({"uriStr","context","at"});
#endif
        DAS_ADD_FUN_BIND("escape_uri", none, das::escape_uri)->args({"uriStr","spaceToPlus","normalizeBreaks","context","at"});
        DAS_ADD_FUN_BIND("unescape_uri", none, das::unescape_uri)->args({"uriStr","context","at"});
        DAS_ADD_FUN_BIND("normalize_uri", none, das::normalize_uri)->args({"uriStr","context","at"});
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"daScript/simulate/aot_builtin_uriparser.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_UriParser,das);

