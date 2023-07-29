#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

MAKE_TYPE_FACTORY(stbtt_bakedchar,stbtt_bakedchar);
MAKE_TYPE_FACTORY(stbtt_aligned_quad,stbtt_aligned_quad);

namespace das {

struct StbttBakedCharTypeAnnotation : ManagedStructureAnnotation <stbtt_bakedchar> {
    StbttBakedCharTypeAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("stbtt_bakedchar",ml) {
        addField<DAS_BIND_MANAGED_FIELD(x0)>("x0","x0");
        addField<DAS_BIND_MANAGED_FIELD(y0)>("y0","y0");
        addField<DAS_BIND_MANAGED_FIELD(x1)>("x1","x1");
        addField<DAS_BIND_MANAGED_FIELD(y1)>("y1","y1");
        addField<DAS_BIND_MANAGED_FIELD(xoff)>("xoff","xoff");
        addField<DAS_BIND_MANAGED_FIELD(yoff)>("yoff","yoff");
        addField<DAS_BIND_MANAGED_FIELD(xadvance)>("xadvance","xadvance");
    }
};

struct StbttAlignedQuadTypeAnnotation : ManagedStructureAnnotation <stbtt_aligned_quad> {
    StbttAlignedQuadTypeAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("stbtt_aligned_quad",ml) {
        addField<DAS_BIND_MANAGED_FIELD(x0)>("x0","x0");
        addField<DAS_BIND_MANAGED_FIELD(y0)>("y0","y0");
        addField<DAS_BIND_MANAGED_FIELD(x1)>("x1","x1");
        addField<DAS_BIND_MANAGED_FIELD(y1)>("y1","y1");
        addField<DAS_BIND_MANAGED_FIELD(s0)>("s0","s0");
        addField<DAS_BIND_MANAGED_FIELD(t0)>("t0","t0");
        addField<DAS_BIND_MANAGED_FIELD(s1)>("s1","s1");
        addField<DAS_BIND_MANAGED_FIELD(t1)>("t1","t1");
    }
};

class Module_StbTrueType : public Module {
public:
    Module_StbTrueType() : Module("stbtruetype") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        addAnnotation(make_smart<StbttBakedCharTypeAnnotation>(lib));
        addAnnotation(make_smart<StbttAlignedQuadTypeAnnotation>(lib));
        addExtern<DAS_BIND_FUN(stbtt_BakeFontBitmap)> (*this, lib, "stbtt_BakeFontBitmap",
            SideEffects::worstDefault, "stbtt_BakeFontBitmap")
                ->args({"data","offset","pixel_height","pixels","pw","ph","firstChar","numChars","chardata"});
        addExtern<DAS_BIND_FUN(stbtt_GetBakedQuad)> (*this, lib, "stbtt_GetBakedQuad",
            SideEffects::worstDefault, "stbtt_GetBakedQuad")
                ->args({"chardata","pw","ph","char_index","xpos","ypos","q","opengl_fillrule"});
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasStbTrueType/src/dasStbTrueType.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_StbTrueType,das);

