#include "daScript/misc/platform.h"

#include "module_uriparser.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/hash.h"

#define URI_STATIC_BUILD
#include "uriparser/Uri.h"

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

class Module_UriParser : public Module {
public:
    Module_UriParser() : Module("uriparser") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
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
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"modules/uriparser/module_uriparser.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_UriParser,das);

