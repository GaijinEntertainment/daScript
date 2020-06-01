#include "daScript/misc/platform.h"

#include "module_uriparser.h"

#include "daScript/ast/ast.h"

using namespace das;

class Module_UriParser : public Module {
public:
    Module_UriParser() : Module("uriparser") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();

    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "// #include \"module_uriparser.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_MODULE(Module_UriParser);

