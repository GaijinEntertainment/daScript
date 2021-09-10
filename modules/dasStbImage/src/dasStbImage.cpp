#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

namespace das {

class Module_StbImage : public Module {
public:
    Module_StbImage() : Module("stbimage") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        addExtern<DAS_BIND_FUN(stbi_load_from_memory)> (*this, lib, "stbi_load_from_memory",
            SideEffects::worstDefault, "stbi_load_from_memory");
        addExtern<DAS_BIND_FUN(stbi_load)> (*this, lib, "stbi_load",
            SideEffects::worstDefault, "stbi_load");
        addExtern<DAS_BIND_FUN(stbi_image_free)> (*this, lib, "stbi_image_free",
            SideEffects::worstDefault, "stbi_image_free");
        addExtern<DAS_BIND_FUN(stbi_failure_reason)> (*this, lib, "stbi_failure_reason",
            SideEffects::worstDefault, "stbi_failure_reason");
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"modules/dasStbImage/src/dasStbImage.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_StbImage,das);

