#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"

#ifdef STB_IMPLEMENTATION_ALREADY_LINKED
    #include "stb_image.h"
    #include "stb_image_write.h"
#else
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include "stb_image_write.h"
#endif

namespace das {

class Module_StbImage : public Module {
public:
    Module_StbImage() : Module("stbimage") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        addExtern<DAS_BIND_FUN(stbi_load_from_memory)> (*this, lib, "stbi_load_from_memory",
            SideEffects::worstDefault, "stbi_load_from_memory")
                ->args({"buffer","len","x","y","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_load)> (*this, lib, "stbi_load",
            SideEffects::worstDefault, "stbi_load")
                ->args({"filename","x","y","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_image_free)> (*this, lib, "stbi_image_free",
            SideEffects::worstDefault, "stbi_image_free")
                ->arg("retval_from_stbi_load");
        addExtern<DAS_BIND_FUN(stbi_failure_reason)> (*this, lib, "stbi_failure_reason",
            SideEffects::worstDefault, "stbi_failure_reason");
        addExtern<DAS_BIND_FUN(stbi_write_png)> (*this, lib, "stbi_write_png",
            SideEffects::worstDefault, "stbi_write_png")
                ->args({"filename","x","y","comp","data","stride_bytes"});
        addExtern<DAS_BIND_FUN(stbi_write_bmp)> (*this, lib, "stbi_write_bmp",
            SideEffects::worstDefault, "stbi_write_bmp")
                ->args({"filename","x","y","comp","data"});
        addExtern<DAS_BIND_FUN(stbi_write_tga)> (*this, lib, "stbi_write_tga",
            SideEffects::worstDefault, "stbi_write_tga")
                ->args({"filename","x","y","comp","data"});
        addExtern<DAS_BIND_FUN(stbi_write_jpg)> (*this, lib, "stbi_write_jpg",
            SideEffects::worstDefault, "stbi_write_jpg")
                ->args({"filename","x","y","comp","data","quality"});
        addExtern<DAS_BIND_FUN(stbi_write_hdr)> (*this, lib, "stbi_write_hdr",
            SideEffects::worstDefault, "stbi_write_hdr")
                ->args({"filename","x","y","comp","data"});;
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasStbImage/src/dasStbImage.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_StbImage,das);

