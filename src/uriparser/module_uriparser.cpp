#include "daScript/misc/platform.h"

#include "module_uriparser.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/hash.h"

#define URI_STATIC_BUILD
#include "uriparser/Uri.h"

using namespace das;

char * uri_to_unix_file_name ( char * uristr, Context * context ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[len + 1];
    char * result = nullptr;
    if ( uriUriStringToUnixFilenameA(uristr, buf) == URI_SUCCESS ) {
        result = context->stringHeap.allocateString(buf, uint32_t(strlen(buf)));
    }
    delete buf;
    return result;
}

char * uri_to_windows_file_name ( char * uristr, Context * context ) {
    if ( !uristr ) return nullptr;
    int len = stringLength(*context,uristr);
    auto buf = new char[len + 1];
    char * result = nullptr;
    if ( uriUriStringToWindowsFilenameA(uristr, buf) == URI_SUCCESS ) {
        result = context->stringHeap.allocateString(buf, uint32_t(strlen(buf)));
    }
    delete buf;
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
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "// #include \"module_uriparser.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_MODULE(Module_UriParser);

