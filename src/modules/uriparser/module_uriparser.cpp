#include "daScript/misc/platform.h"

#include "module_uriparser.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/hash.h"

// todo: use CMAKE to detect linux UUID library
// #define LINUX_UUID

#define URI_STATIC_BUILD
#include "uriparser/Uri.h"

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

class Module_UriParser : public Module {
public:
    Module_UriParser() : Module("uriparser") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
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
        tw << "#include \"modules/uriparser/module_uriparser.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_UriParser,das);

