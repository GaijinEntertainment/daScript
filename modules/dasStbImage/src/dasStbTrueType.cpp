#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"

#include "stb_truetype.h"
#include "dasStbTrueType.h"

MAKE_TYPE_FACTORY(stbtt_packedchar, stbtt_packedchar);
MAKE_TYPE_FACTORY(stbtt_aligned_quad, stbtt_aligned_quad);
MAKE_TYPE_FACTORY(stbtt_fontinfo, stbtt_fontinfo);
MAKE_TYPE_FACTORY(stbtt_pack_context, stbtt_pack_context);
MAKE_TYPE_FACTORY(stbtt_vertex_das, stbtt_vertex_das);

namespace das {

// ---- Struct annotations ----

struct StbttPackedCharAnnotation : ManagedStructureAnnotation<stbtt_packedchar> {
    StbttPackedCharAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("stbtt_packedchar", ml) {
        addField<DAS_BIND_MANAGED_FIELD(x0)>("x0", "x0");
        addField<DAS_BIND_MANAGED_FIELD(y0)>("y0", "y0");
        addField<DAS_BIND_MANAGED_FIELD(x1)>("x1", "x1");
        addField<DAS_BIND_MANAGED_FIELD(y1)>("y1", "y1");
        addField<DAS_BIND_MANAGED_FIELD(xoff)>("xoff", "xoff");
        addField<DAS_BIND_MANAGED_FIELD(yoff)>("yoff", "yoff");
        addField<DAS_BIND_MANAGED_FIELD(xadvance)>("xadvance", "xadvance");
        addField<DAS_BIND_MANAGED_FIELD(xoff2)>("xoff2", "xoff2");
        addField<DAS_BIND_MANAGED_FIELD(yoff2)>("yoff2", "yoff2");
    }
};

struct StbttAlignedQuadAnnotation : ManagedStructureAnnotation<stbtt_aligned_quad> {
    StbttAlignedQuadAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("stbtt_aligned_quad", ml) {
        addField<DAS_BIND_MANAGED_FIELD(x0)>("x0", "x0");
        addField<DAS_BIND_MANAGED_FIELD(y0)>("y0", "y0");
        addField<DAS_BIND_MANAGED_FIELD(s0)>("s0", "s0");
        addField<DAS_BIND_MANAGED_FIELD(t0)>("t0", "t0");
        addField<DAS_BIND_MANAGED_FIELD(x1)>("x1", "x1");
        addField<DAS_BIND_MANAGED_FIELD(y1)>("y1", "y1");
        addField<DAS_BIND_MANAGED_FIELD(s1)>("s1", "s1");
        addField<DAS_BIND_MANAGED_FIELD(t1)>("t1", "t1");
    }
};

struct StbttFontinfoAnnotation : ManagedStructureAnnotation<stbtt_fontinfo> {
    StbttFontinfoAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("stbtt_fontinfo", ml) {
        addField<DAS_BIND_MANAGED_FIELD(numGlyphs)>("numGlyphs", "numGlyphs");
    }
};

struct StbttPackContextAnnotation : ManagedStructureAnnotation<stbtt_pack_context> {
    StbttPackContextAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("stbtt_pack_context", ml) {
    }
};

struct StbttVertexDasAnnotation : ManagedStructureAnnotation<stbtt_vertex_das> {
    StbttVertexDasAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("stbtt_vertex_das", ml) {
        addField<DAS_BIND_MANAGED_FIELD(x)>("x", "x");
        addField<DAS_BIND_MANAGED_FIELD(y)>("y", "y");
        addField<DAS_BIND_MANAGED_FIELD(cx)>("cx", "cx");
        addField<DAS_BIND_MANAGED_FIELD(cy)>("cy", "cy");
        addField<DAS_BIND_MANAGED_FIELD(cx1)>("cx1", "cx1");
        addField<DAS_BIND_MANAGED_FIELD(cy1)>("cy1", "cy1");
        addField<DAS_BIND_MANAGED_FIELD(type)>("vtype", "type");
    }
};

// Wrapper: get glyph shape as callback per vertex
void stbtt_GetCodepointShape_foreach(const stbtt_fontinfo * info, int codepoint,
    const TBlock<void, const stbtt_vertex_das &> & block, Context * ctx, LineInfoArg * at) {
    stbtt_vertex * vertices = nullptr;
    int num = stbtt_GetCodepointShape(info, codepoint, &vertices);
    for (int i = 0; i < num; i++) {
        stbtt_vertex_das v;
        v.x = vertices[i].x;
        v.y = vertices[i].y;
        v.cx = vertices[i].cx;
        v.cy = vertices[i].cy;
        v.cx1 = vertices[i].cx1;
        v.cy1 = vertices[i].cy1;
        v.type = vertices[i].type;
        das_invoke<void>::invoke<const stbtt_vertex_das &>(ctx, at, block, v);
    }
    if (vertices) stbtt_FreeShape(info, vertices);
}

int stbtt_GetCodepointShape_count(const stbtt_fontinfo * info, int codepoint) {
    stbtt_vertex * vertices = nullptr;
    int num = stbtt_GetCodepointShape(info, codepoint, &vertices);
    if (vertices) stbtt_FreeShape(info, vertices);
    return num;
}

// ---- Module ----

class Module_StbTrueType : public Module {
public:
    Module_StbTrueType() : Module("stbtruetype") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();

        // ---- Struct annotations ----
        addAnnotation(make_smart<StbttPackedCharAnnotation>(lib));
        addAnnotation(make_smart<StbttAlignedQuadAnnotation>(lib));
        addAnnotation(make_smart<StbttFontinfoAnnotation>(lib));
        addAnnotation(make_smart<StbttPackContextAnnotation>(lib));
        addAnnotation(make_smart<StbttVertexDasAnnotation>(lib));

        // ---- Glyph shape constants ----
        addConstant<int>(*this, "STBTT_vmove", STBTT_vmove);
        addConstant<int>(*this, "STBTT_vline", STBTT_vline);
        addConstant<int>(*this, "STBTT_vcurve", STBTT_vcurve);
        addConstant<int>(*this, "STBTT_vcubic", STBTT_vcubic);

        // ---- Font loading ----
        addExtern<DAS_BIND_FUN(stbtt_GetNumberOfFonts)>(*this, lib, "stbtt_GetNumberOfFonts",
            SideEffects::none, "stbtt_GetNumberOfFonts")
                ->arg("data");
        addExtern<DAS_BIND_FUN(stbtt_GetFontOffsetForIndex)>(*this, lib, "stbtt_GetFontOffsetForIndex",
            SideEffects::none, "stbtt_GetFontOffsetForIndex")
                ->args({"data", "index"});
        addExtern<DAS_BIND_FUN(stbtt_InitFont)>(*this, lib, "stbtt_InitFont",
            SideEffects::modifyArgument, "stbtt_InitFont")
                ->args({"info", "data", "offset"});
        addExtern<DAS_BIND_FUN(stbtt_FindGlyphIndex)>(*this, lib, "stbtt_FindGlyphIndex",
            SideEffects::none, "stbtt_FindGlyphIndex")
                ->args({"info", "unicode_codepoint"});

        // ---- Packing API ----
        addExtern<DAS_BIND_FUN(stbtt_PackBegin)>(*this, lib, "stbtt_PackBegin",
            SideEffects::modifyArgument, "stbtt_PackBegin")
                ->args({"spc", "pixels", "width", "height", "stride_in_bytes", "padding", "alloc_context"});
        addExtern<DAS_BIND_FUN(stbtt_PackEnd)>(*this, lib, "stbtt_PackEnd",
            SideEffects::modifyArgument, "stbtt_PackEnd")
                ->arg("spc");
        addExtern<DAS_BIND_FUN(stbtt_PackFontRange)>(*this, lib, "stbtt_PackFontRange",
            SideEffects::modifyArgumentAndExternal, "stbtt_PackFontRange")
                ->args({"spc", "fontdata", "font_index", "font_size", "first_unicode_char_in_range", "num_chars_in_range", "chardata_for_range"});
        addExtern<DAS_BIND_FUN(stbtt_PackSetOversampling)>(*this, lib, "stbtt_PackSetOversampling",
            SideEffects::modifyArgument, "stbtt_PackSetOversampling")
                ->args({"spc", "h_oversample", "v_oversample"});
        addExtern<DAS_BIND_FUN(stbtt_PackSetSkipMissingCodepoints)>(*this, lib, "stbtt_PackSetSkipMissingCodepoints",
            SideEffects::modifyArgument, "stbtt_PackSetSkipMissingCodepoints")
                ->args({"spc", "skip"});
        addExtern<DAS_BIND_FUN(stbtt_GetPackedQuad)>(*this, lib, "stbtt_GetPackedQuad",
            SideEffects::modifyArgument, "stbtt_GetPackedQuad")
                ->args({"chardata", "pw", "ph", "char_index", "xpos", "ypos", "q", "align_to_integer"});

        // ---- Font metrics ----
        addExtern<DAS_BIND_FUN(stbtt_ScaleForPixelHeight)>(*this, lib, "stbtt_ScaleForPixelHeight",
            SideEffects::none, "stbtt_ScaleForPixelHeight")
                ->args({"info", "pixels"});
        addExtern<DAS_BIND_FUN(stbtt_ScaleForMappingEmToPixels)>(*this, lib, "stbtt_ScaleForMappingEmToPixels",
            SideEffects::none, "stbtt_ScaleForMappingEmToPixels")
                ->args({"info", "pixels"});
        addExtern<DAS_BIND_FUN(stbtt_GetFontVMetrics)>(*this, lib, "stbtt_GetFontVMetrics",
            SideEffects::modifyArgument, "stbtt_GetFontVMetrics")
                ->args({"info", "ascent", "descent", "lineGap"});
        addExtern<DAS_BIND_FUN(stbtt_GetFontBoundingBox)>(*this, lib, "stbtt_GetFontBoundingBox",
            SideEffects::modifyArgument, "stbtt_GetFontBoundingBox")
                ->args({"info", "x0", "y0", "x1", "y1"});
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointHMetrics)>(*this, lib, "stbtt_GetCodepointHMetrics",
            SideEffects::modifyArgument, "stbtt_GetCodepointHMetrics")
                ->args({"info", "codepoint", "advanceWidth", "leftSideBearing"});
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointKernAdvance)>(*this, lib, "stbtt_GetCodepointKernAdvance",
            SideEffects::none, "stbtt_GetCodepointKernAdvance")
                ->args({"info", "ch1", "ch2"});
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointBox)>(*this, lib, "stbtt_GetCodepointBox",
            SideEffects::modifyArgument, "stbtt_GetCodepointBox")
                ->args({"info", "codepoint", "x0", "y0", "x1", "y1"});

        // ---- Direct bitmap rendering ----
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointBitmap)>(*this, lib, "stbtt_GetCodepointBitmap",
            SideEffects::modifyArgument, "stbtt_GetCodepointBitmap")
                ->args({"info", "scale_x", "scale_y", "codepoint", "width", "height", "xoff", "yoff"});
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointBitmapSubpixel)>(*this, lib, "stbtt_GetCodepointBitmapSubpixel",
            SideEffects::modifyArgument, "stbtt_GetCodepointBitmapSubpixel")
                ->args({"info", "scale_x", "scale_y", "shift_x", "shift_y", "codepoint", "width", "height", "xoff", "yoff"});
        addExtern<DAS_BIND_FUN(stbtt_MakeCodepointBitmap)>(*this, lib, "stbtt_MakeCodepointBitmap",
            SideEffects::modifyArgument, "stbtt_MakeCodepointBitmap")
                ->args({"info", "output", "out_w", "out_h", "out_stride", "scale_x", "scale_y", "codepoint"});
        addExtern<DAS_BIND_FUN(stbtt_MakeCodepointBitmapSubpixel)>(*this, lib, "stbtt_MakeCodepointBitmapSubpixel",
            SideEffects::modifyArgument, "stbtt_MakeCodepointBitmapSubpixel")
                ->args({"info", "output", "out_w", "out_h", "out_stride", "scale_x", "scale_y", "shift_x", "shift_y", "codepoint"});
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointBitmapBox)>(*this, lib, "stbtt_GetCodepointBitmapBox",
            SideEffects::modifyArgument, "stbtt_GetCodepointBitmapBox")
                ->args({"font", "codepoint", "scale_x", "scale_y", "ix0", "iy0", "ix1", "iy1"});
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointBitmapBoxSubpixel)>(*this, lib, "stbtt_GetCodepointBitmapBoxSubpixel",
            SideEffects::modifyArgument, "stbtt_GetCodepointBitmapBoxSubpixel")
                ->args({"font", "codepoint", "scale_x", "scale_y", "shift_x", "shift_y", "ix0", "iy0", "ix1", "iy1"});
        addExtern<DAS_BIND_FUN(stbtt_FreeBitmap)>(*this, lib, "stbtt_FreeBitmap",
            SideEffects::modifyExternal, "stbtt_FreeBitmap")
                ->args({"bitmap", "userdata"});

        // ---- Glyph shape ----
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointShape_foreach)>(*this, lib, "stbtt_GetCodepointShape",
            SideEffects::invoke, "stbtt_GetCodepointShape_foreach")
                ->args({"info", "codepoint", "block", "context", "at"});
        addExtern<DAS_BIND_FUN(stbtt_GetCodepointShape_count)>(*this, lib, "stbtt_GetCodepointShape_count",
            SideEffects::none, "stbtt_GetCodepointShape_count")
                ->args({"info", "codepoint"});
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasStbImage/src/dasStbTrueType.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_StbTrueType, Module_StbTrueType);

}

REGISTER_MODULE_IN_NAMESPACE(Module_StbTrueType, das);
