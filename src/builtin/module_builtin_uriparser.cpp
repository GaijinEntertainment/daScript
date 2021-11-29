#include "daScript/misc/platform.h"

#include "module_builtin_uriparser.h"
#include "daScript/simulate/aot_builtin_uriparser.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/hash.h"
#include "daScript/misc/uric.h"

#define URI_STATIC_BUILD
#include "uriparser/Uri.h"

IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriTextRangeA,UriTextRangeA)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriIp4Struct,UriIp4Struct)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriIp6Struct,UriIp6Struct)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriHostDataA,UriHostDataA)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriPathSegmentStructA,UriPathSegmentStructA)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(UriUriA,UriUriA)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(Uri,das::Uri)

#if defined(_MSC_VER) && !defined(_GAMING_XBOX) && !defined(_DURANGO)

#include <rpc.h>
#pragma comment(lib,"Rpcrt4")

char * das::makeNewGuid( das::Context * context ) {
	UUID id;
	if ( UuidCreate(&id)!=RPC_S_OK ) context->throw_error("can't create UUID");
    CHAR* uuidstr = NULL;
    UuidToStringA(&id, (RPC_CSTR *)&uuidstr);
    char * res = context->stringHeap->allocateString(uuidstr);
    RpcStringFreeA((RPC_CSTR *)&uuidstr);
    return res;
}

#elif defined(__linux__) && defined(LINUX_UUID)

#include <uuid/uuid.h>

char * das::makeNewGuid( das::Context * context ) {
    union {
        unsigned char   data[16];
        uint32_t        data32[4];
    } data;
    uuid_generate(data.data);
    TextWriter tw;
    tw << HEX << data.data32[0] << "-" << data.data32[1] << "-" << data.data32[2] << "-" << data.data32[3] << DEC;
    return context->stringHeap->allocateString(tw.str());
}

#elif defined(__APPLE__)

#include <uuid/uuid.h>

char * das::makeNewGuid( das::Context * context ) {
    union {
        unsigned char   data[16];
        uint32_t        data32[4];
    } data;
    uuid_generate(data.data);
    TextWriter tw;
    tw << HEX << data.data32[0] << "-" << data.data32[1] << "-" << data.data32[2] << "-" << data.data32[3] << DEC;
    return context->stringHeap->allocateString(tw.str());
}

#else

char * das::makeNewGuid( Context * context ) {
    context->throw_error("GUID generation is not implemented for this platform");
    return nullptr;
}

#endif
namespace das {

char * uri_to_unix_file_name ( char * uristr, Context * context ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[len + 1];
    char * result = nullptr;
    if ( uriUriStringToUnixFilenameA(uristr, buf) == URI_SUCCESS ) {
        result = context->stringHeap->allocateString(buf, uint32_t(strlen(buf)));
    }
    delete [] buf;
    return result;
}

char * uri_to_windows_file_name ( char * uristr, Context * context ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[len + 1];
    char * result = nullptr;
    if ( uriUriStringToWindowsFilenameA(uristr, buf) == URI_SUCCESS ) {
        result = context->stringHeap->allocateString(buf, uint32_t(strlen(buf)));
    }
    delete [] buf;
    return result;
}

char * unix_file_name_to_uri ( char * uristr, Context * context ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[3 * len + 1];
    char * result = nullptr;
    if ( uriUnixFilenameToUriStringA(uristr, buf) == URI_SUCCESS ) {
        result = context->stringHeap->allocateString(buf, uint32_t(strlen(buf)));
    }
    delete [] buf;
    return result;
}

char * windows_file_name_to_uri ( char * uristr, Context * context ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[8 + 3 * len + 1];
    char * result = nullptr;
    if ( uriWindowsFilenameToUriStringA(uristr, buf) == URI_SUCCESS ) {
        result = context->stringHeap->allocateString(buf, uint32_t(strlen(buf)));
    }
    delete [] buf;
    return result;
}

char * escape_uri ( char * uristr, bool spaceToPlus, bool normalizeBreaks, Context * context ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[len*6];
    char * result = nullptr;
    if ( uriEscapeA(uristr, buf, spaceToPlus, normalizeBreaks) ) {
        result = context->stringHeap->allocateString(buf, uint32_t(strlen(buf)));
    }
    delete [] buf;
    return result;
}

char * unescape_uri ( char * uristr,Context * context ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    char * buf = new char[len+1];
    memcpy(buf, uristr, len);
    buf[len] = 0;
    char * result = nullptr;
    if ( uriUnescapeInPlaceA(buf) ) {
        result = context->stringHeap->allocateString(buf, uint32_t(strlen(buf)));
    }
    delete [] buf;
    return result;
}

char* normalize_uri(char* uristr, Context* context) {
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
        result = context->stringHeap->allocateString(buf, uint32_t(strlen(buf)));
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
        addField<DAS_BIND_MANAGED_FIELD(data)>("data");
    }
};

struct UriIp6StructAnnotation : ManagedStructureAnnotation<UriIp6Struct> {
    UriIp6StructAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriIp6Struct>("UriIp6Struct",ml) {
        addField<DAS_BIND_MANAGED_FIELD(data)>("data");
    }
};

struct UriHostDataAAnnotation : ManagedStructureAnnotation<UriHostDataA> {
    UriHostDataAAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriHostDataA>("UriHostDataA",ml) {
        addField<DAS_BIND_MANAGED_FIELD(ip4)>("ip4");
        addField<DAS_BIND_MANAGED_FIELD(ip6)>("ip6");
        addField<DAS_BIND_MANAGED_FIELD(ipFuture)>("ipFuture");
    }
};

struct UriPathSegmentStructAAnnotation : ManagedStructureAnnotation<UriPathSegmentStructA> {
    UriPathSegmentStructAAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriPathSegmentStructA>("UriPathSegmentStructA",ml) {
    }
    void init () {
        addField<DAS_BIND_MANAGED_FIELD(text)>("text");
        addField<DAS_BIND_MANAGED_FIELD(next)>("next");
    }
};

struct UriUriAAnnotation  : ManagedStructureAnnotation<UriUriA> {
    UriUriAAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<UriUriA>("UriUriA",ml) {
        addField<DAS_BIND_MANAGED_FIELD(scheme)>("scheme");
        addField<DAS_BIND_MANAGED_FIELD(userInfo)>("userInfo");
        addField<DAS_BIND_MANAGED_FIELD(hostText)>("hostText");
        addField<DAS_BIND_MANAGED_FIELD(hostData)>("hostData");
        addField<DAS_BIND_MANAGED_FIELD(portText)>("portText");
        addField<DAS_BIND_MANAGED_FIELD(pathHead)>("pathHead");
        addField<DAS_BIND_MANAGED_FIELD(pathTail)>("pathTail");
        addField<DAS_BIND_MANAGED_FIELD(query)>("query");
        addField<DAS_BIND_MANAGED_FIELD(fragment)>("fragment");
        addField<DAS_BIND_MANAGED_FIELD(absolutePath)>("absolutePath");
        addField<DAS_BIND_MANAGED_FIELD(owner)>("owner");
    }
};

struct UriAnnotation  : ManagedStructureAnnotation<Uri> {
    UriAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation<Uri>("Uri",ml) {
        addProperty<DAS_BIND_MANAGED_PROP(empty)>("empty");
        addProperty<DAS_BIND_MANAGED_PROP(size)>("size");
        addProperty<DAS_BIND_MANAGED_PROP(status)>("status");
        addField<DAS_BIND_MANAGED_FIELD(uri)>("uri");
    }
};

void delete_uri ( Uri & uri ) {
    uri.reset();
}

class Module_UriParser : public Module {
public:
    Module_UriParser() : Module("uriparser") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
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
        addExtern<DAS_BIND_FUN(delete_uri)> (*this, lib, "finalize",
            SideEffects::accessExternal, "delete_uri");
        addExtern<DAS_BIND_FUN(makeNewGuid)> (*this, lib, "make_new_guid",
            SideEffects::accessExternal, "makeNewGuid");
        addExtern<DAS_BIND_FUN(uri_to_unix_file_name)> (*this, lib, "uri_to_unix_file_name",
            SideEffects::none, "uri_to_unix_file_name");
        addExtern<DAS_BIND_FUN(uri_to_windows_file_name)> (*this, lib, "uri_to_windows_file_name",
            SideEffects::none, "uri_to_windows_file_name");
        addExtern<DAS_BIND_FUN(unix_file_name_to_uri)> (*this, lib, "unix_file_name_to_uri",
            SideEffects::none, "unix_file_name_to_uri");
        addExtern<DAS_BIND_FUN(windows_file_name_to_uri)> (*this, lib, "windows_file_name_to_uri",
            SideEffects::none, "windows_file_name_to_uri");
#ifdef _WIN32
        addExtern<DAS_BIND_FUN(uri_to_windows_file_name)> (*this, lib, "uri_to_file_name",
            SideEffects::none, "uri_to_windows_file_name");
        addExtern<DAS_BIND_FUN(windows_file_name_to_uri)> (*this, lib, "file_name_to_uri",
            SideEffects::none, "windows_file_name_to_uri");
#else
        addExtern<DAS_BIND_FUN(uri_to_unix_file_name)> (*this, lib, "uri_to_file_name",
            SideEffects::none, "uri_to_unix_file_name");
        addExtern<DAS_BIND_FUN(unix_file_name_to_uri)> (*this, lib, "file_name_to_uri",
            SideEffects::none, "unix_file_name_to_uri");
#endif
        addExtern<DAS_BIND_FUN(escape_uri)> (*this, lib, "escape_uri",
            SideEffects::none, "escape_uri");
        addExtern<DAS_BIND_FUN(unescape_uri)> (*this, lib, "unescape_uri",
            SideEffects::none, "unescape_uri");
        addExtern<DAS_BIND_FUN(normalize_uri)> (*this, lib, "normalize_uri",
            SideEffects::none, "normalize_uri");
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"daScript/simulate/aot_builtin_uriparser.h\"\n";
        tw << "#include \"daScript/misc/uric.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_UriParser,das);

