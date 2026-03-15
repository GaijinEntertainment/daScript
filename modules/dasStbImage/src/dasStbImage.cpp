#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"

#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize2.h"

// stb_image_write config variable wrappers (defined in dasStbImage_impl.cpp)
void stbi_write_set_png_compression_level ( int level );
int  stbi_write_get_png_compression_level ( );
void stbi_write_set_force_png_filter ( int filter );
int  stbi_write_get_force_png_filter ( );
void stbi_write_set_tga_with_rle ( int rle );
int  stbi_write_get_tga_with_rle ( );

// stb_image_resize2 enums — anonymous typedef enums, need unqualified access + proxy types
DAS_BIND_ENUM_CAST_98(stbir_pixel_layout)
DAS_BASE_BIND_ENUM_BOTH(DAS_BIND_ENUM_UNQUALIFIED_HELPER, stbir_pixel_layout, stbir_pixel_layout, Enumerationstbir_pixel_layout,
    STBIR_1CHANNEL, STBIR_2CHANNEL, STBIR_RGB, STBIR_BGR, STBIR_4CHANNEL,
    STBIR_RGBA, STBIR_BGRA, STBIR_ARGB, STBIR_ABGR, STBIR_RA, STBIR_AR,
    STBIR_RGBA_PM, STBIR_BGRA_PM, STBIR_ARGB_PM, STBIR_ABGR_PM, STBIR_RA_PM, STBIR_AR_PM
)
DAS_BASE_BIND_ENUM_FACTORY(stbir_pixel_layout, "stbir_pixel_layout")
DAS_BASE_BIND_ENUM_FACTORY(stbir_pixel_layout_DasProxy, "stbir_pixel_layout")

DAS_BIND_ENUM_CAST_98(stbir_edge)
DAS_BASE_BIND_ENUM_BOTH(DAS_BIND_ENUM_UNQUALIFIED_HELPER, stbir_edge, stbir_edge, Enumerationstbir_edge,
    STBIR_EDGE_CLAMP, STBIR_EDGE_REFLECT, STBIR_EDGE_WRAP, STBIR_EDGE_ZERO
)
DAS_BASE_BIND_ENUM_FACTORY(stbir_edge, "stbir_edge")
DAS_BASE_BIND_ENUM_FACTORY(stbir_edge_DasProxy, "stbir_edge")

DAS_BIND_ENUM_CAST_98(stbir_filter)
DAS_BASE_BIND_ENUM_BOTH(DAS_BIND_ENUM_UNQUALIFIED_HELPER, stbir_filter, stbir_filter, Enumerationstbir_filter,
    STBIR_FILTER_DEFAULT, STBIR_FILTER_BOX, STBIR_FILTER_TRIANGLE,
    STBIR_FILTER_CUBICBSPLINE, STBIR_FILTER_CATMULLROM, STBIR_FILTER_MITCHELL,
    STBIR_FILTER_POINT_SAMPLE, STBIR_FILTER_OTHER
)
DAS_BASE_BIND_ENUM_FACTORY(stbir_filter, "stbir_filter")
DAS_BASE_BIND_ENUM_FACTORY(stbir_filter_DasProxy, "stbir_filter")

DAS_BIND_ENUM_CAST_98(stbir_datatype)
DAS_BASE_BIND_ENUM_BOTH(DAS_BIND_ENUM_UNQUALIFIED_HELPER, stbir_datatype, stbir_datatype, Enumerationstbir_datatype,
    STBIR_TYPE_UINT8, STBIR_TYPE_UINT8_SRGB, STBIR_TYPE_UINT8_SRGB_ALPHA,
    STBIR_TYPE_UINT16, STBIR_TYPE_FLOAT, STBIR_TYPE_HALF_FLOAT
)
DAS_BASE_BIND_ENUM_FACTORY(stbir_datatype, "stbir_datatype")
DAS_BASE_BIND_ENUM_FACTORY(stbir_datatype_DasProxy, "stbir_datatype")

// stbi_write_*_to_func callback (defined in dasStbImage_impl.cpp)
void stbi_write_to_vec_callback ( void * context, void * data, int size );

namespace das {

// write-to-memory wrappers — encode image, pass temp array<uint8> to block
void stbi_write_png_to_memory ( int x, int y, int comp, const void * data, int stride_bytes,
        const TBlock<void,TTemporary<const TArray<uint8_t>>> & blk, Context * context, LineInfoArg * at ) {
    std::vector<uint8_t> buf;
    stbi_write_png_to_func(stbi_write_to_vec_callback, &buf, x, y, comp, data, stride_bytes);
    Array arr;
    array_mark_locked(arr, buf.data(), uint32_t(buf.size()));
    vec4f args[1];
    args[0] = cast<Array *>::from(&arr);
    context->invoke(blk, args, nullptr, at);
}

void stbi_write_bmp_to_memory ( int x, int y, int comp, const void * data,
        const TBlock<void,TTemporary<const TArray<uint8_t>>> & blk, Context * context, LineInfoArg * at ) {
    std::vector<uint8_t> buf;
    stbi_write_bmp_to_func(stbi_write_to_vec_callback, &buf, x, y, comp, data);
    Array arr;
    array_mark_locked(arr, buf.data(), uint32_t(buf.size()));
    vec4f args[1];
    args[0] = cast<Array *>::from(&arr);
    context->invoke(blk, args, nullptr, at);
}

void stbi_write_tga_to_memory ( int x, int y, int comp, const void * data,
        const TBlock<void,TTemporary<const TArray<uint8_t>>> & blk, Context * context, LineInfoArg * at ) {
    std::vector<uint8_t> buf;
    stbi_write_tga_to_func(stbi_write_to_vec_callback, &buf, x, y, comp, data);
    Array arr;
    array_mark_locked(arr, buf.data(), uint32_t(buf.size()));
    vec4f args[1];
    args[0] = cast<Array *>::from(&arr);
    context->invoke(blk, args, nullptr, at);
}

void stbi_write_jpg_to_memory ( int x, int y, int comp, const void * data, int quality,
        const TBlock<void,TTemporary<const TArray<uint8_t>>> & blk, Context * context, LineInfoArg * at ) {
    std::vector<uint8_t> buf;
    stbi_write_jpg_to_func(stbi_write_to_vec_callback, &buf, x, y, comp, data, quality);
    Array arr;
    array_mark_locked(arr, buf.data(), uint32_t(buf.size()));
    vec4f args[1];
    args[0] = cast<Array *>::from(&arr);
    context->invoke(blk, args, nullptr, at);
}

class Module_StbImage : public Module {
public:
    Module_StbImage() : Module("stbimage") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();

        // ---- stb_image: loading ----
        addExtern<DAS_BIND_FUN(stbi_load)> (*this, lib, "stbi_load",
            SideEffects::worstDefault, "stbi_load")
                ->args({"filename","x","y","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_load_from_memory)> (*this, lib, "stbi_load_from_memory",
            SideEffects::worstDefault, "stbi_load_from_memory")
                ->args({"buffer","len","x","y","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_load_16)> (*this, lib, "stbi_load_16",
            SideEffects::worstDefault, "stbi_load_16")
                ->args({"filename","x","y","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_load_16_from_memory)> (*this, lib, "stbi_load_16_from_memory",
            SideEffects::worstDefault, "stbi_load_16_from_memory")
                ->args({"buffer","len","x","y","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_loadf)> (*this, lib, "stbi_loadf",
            SideEffects::worstDefault, "stbi_loadf")
                ->args({"filename","x","y","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_loadf_from_memory)> (*this, lib, "stbi_loadf_from_memory",
            SideEffects::worstDefault, "stbi_loadf_from_memory")
                ->args({"buffer","len","x","y","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_load_gif_from_memory)> (*this, lib, "stbi_load_gif_from_memory",
            SideEffects::worstDefault, "stbi_load_gif_from_memory")
                ->args({"buffer","len","delays","x","y","z","comp","req_comp"});
        addExtern<DAS_BIND_FUN(stbi_image_free)> (*this, lib, "stbi_image_free",
            SideEffects::worstDefault, "stbi_image_free")
                ->arg("retval_from_stbi_load");
        addExtern<DAS_BIND_FUN(stbi_failure_reason)> (*this, lib, "stbi_failure_reason",
            SideEffects::worstDefault, "stbi_failure_reason");

        // ---- stb_image: image info (no pixel loading) ----
        addExtern<DAS_BIND_FUN(stbi_info)> (*this, lib, "stbi_info",
            SideEffects::worstDefault, "stbi_info")
                ->args({"filename","x","y","comp"});
        addExtern<DAS_BIND_FUN(stbi_info_from_memory)> (*this, lib, "stbi_info_from_memory",
            SideEffects::worstDefault, "stbi_info_from_memory")
                ->args({"buffer","len","x","y","comp"});
        addExtern<DAS_BIND_FUN(stbi_is_hdr)> (*this, lib, "stbi_is_hdr",
            SideEffects::worstDefault, "stbi_is_hdr")
                ->arg("filename");
        addExtern<DAS_BIND_FUN(stbi_is_hdr_from_memory)> (*this, lib, "stbi_is_hdr_from_memory",
            SideEffects::worstDefault, "stbi_is_hdr_from_memory")
                ->args({"buffer","len"});
        addExtern<DAS_BIND_FUN(stbi_is_16_bit)> (*this, lib, "stbi_is_16_bit",
            SideEffects::worstDefault, "stbi_is_16_bit")
                ->arg("filename");
        addExtern<DAS_BIND_FUN(stbi_is_16_bit_from_memory)> (*this, lib, "stbi_is_16_bit_from_memory",
            SideEffects::worstDefault, "stbi_is_16_bit_from_memory")
                ->args({"buffer","len"});

        // ---- stb_image: settings (thread-local versions only) ----
        addExtern<DAS_BIND_FUN(stbi_set_flip_vertically_on_load_thread)> (*this, lib, "stbi_set_flip_vertically_on_load",
            SideEffects::modifyExternal, "stbi_set_flip_vertically_on_load_thread")
                ->arg("flag");
        addExtern<DAS_BIND_FUN(stbi_set_unpremultiply_on_load_thread)> (*this, lib, "stbi_set_unpremultiply_on_load",
            SideEffects::modifyExternal, "stbi_set_unpremultiply_on_load_thread")
                ->arg("flag");
        addExtern<DAS_BIND_FUN(stbi_convert_iphone_png_to_rgb_thread)> (*this, lib, "stbi_convert_iphone_png_to_rgb",
            SideEffects::modifyExternal, "stbi_convert_iphone_png_to_rgb_thread")
                ->arg("flag");

        // ---- stb_image: HDR gamma/scale ----
        addExtern<DAS_BIND_FUN(stbi_hdr_to_ldr_gamma)> (*this, lib, "stbi_hdr_to_ldr_gamma",
            SideEffects::modifyExternal, "stbi_hdr_to_ldr_gamma")
                ->arg("gamma");
        addExtern<DAS_BIND_FUN(stbi_hdr_to_ldr_scale)> (*this, lib, "stbi_hdr_to_ldr_scale",
            SideEffects::modifyExternal, "stbi_hdr_to_ldr_scale")
                ->arg("scale");
        addExtern<DAS_BIND_FUN(stbi_ldr_to_hdr_gamma)> (*this, lib, "stbi_ldr_to_hdr_gamma",
            SideEffects::modifyExternal, "stbi_ldr_to_hdr_gamma")
                ->arg("gamma");
        addExtern<DAS_BIND_FUN(stbi_ldr_to_hdr_scale)> (*this, lib, "stbi_ldr_to_hdr_scale",
            SideEffects::modifyExternal, "stbi_ldr_to_hdr_scale")
                ->arg("scale");

        // ---- stb_image_write: file writers ----
        addExtern<DAS_BIND_FUN(stbi_write_png)> (*this, lib, "stbi_write_png",
            SideEffects::modifyExternal, "stbi_write_png")
                ->args({"filename","x","y","comp","data","stride_bytes"});
        addExtern<DAS_BIND_FUN(stbi_write_bmp)> (*this, lib, "stbi_write_bmp",
            SideEffects::modifyExternal, "stbi_write_bmp")
                ->args({"filename","x","y","comp","data"});
        addExtern<DAS_BIND_FUN(stbi_write_tga)> (*this, lib, "stbi_write_tga",
            SideEffects::modifyExternal, "stbi_write_tga")
                ->args({"filename","x","y","comp","data"});
        addExtern<DAS_BIND_FUN(stbi_write_jpg)> (*this, lib, "stbi_write_jpg",
            SideEffects::modifyExternal, "stbi_write_jpg")
                ->args({"filename","x","y","comp","data","quality"});
        addExtern<DAS_BIND_FUN(stbi_write_hdr)> (*this, lib, "stbi_write_hdr",
            SideEffects::modifyExternal, "stbi_write_hdr")
                ->args({"filename","x","y","comp","data"});

        // ---- stb_image_write: settings ----
        addExtern<DAS_BIND_FUN(stbi_flip_vertically_on_write)> (*this, lib, "stbi_flip_vertically_on_write",
            SideEffects::modifyExternal, "stbi_flip_vertically_on_write")
                ->arg("flag");

        // ---- stb_image_write: config variable accessors ----
        addExtern<DAS_BIND_FUN(stbi_write_set_png_compression_level)> (*this, lib, "stbi_write_set_png_compression_level",
            SideEffects::modifyExternal, "stbi_write_set_png_compression_level")
                ->arg("level");
        addExtern<DAS_BIND_FUN(stbi_write_get_png_compression_level)> (*this, lib, "stbi_write_get_png_compression_level",
            SideEffects::accessExternal, "stbi_write_get_png_compression_level");
        addExtern<DAS_BIND_FUN(stbi_write_set_force_png_filter)> (*this, lib, "stbi_write_set_force_png_filter",
            SideEffects::modifyExternal, "stbi_write_set_force_png_filter")
                ->arg("filter");
        addExtern<DAS_BIND_FUN(stbi_write_get_force_png_filter)> (*this, lib, "stbi_write_get_force_png_filter",
            SideEffects::accessExternal, "stbi_write_get_force_png_filter");
        addExtern<DAS_BIND_FUN(stbi_write_set_tga_with_rle)> (*this, lib, "stbi_write_set_tga_with_rle",
            SideEffects::modifyExternal, "stbi_write_set_tga_with_rle")
                ->arg("rle");
        addExtern<DAS_BIND_FUN(stbi_write_get_tga_with_rle)> (*this, lib, "stbi_write_get_tga_with_rle",
            SideEffects::accessExternal, "stbi_write_get_tga_with_rle");

        // ---- stb_image_write: write to memory (block API) ----
        addExtern<DAS_BIND_FUN(stbi_write_png_to_memory)> (*this, lib, "stbi_write_png_to_memory",
            SideEffects::invoke, "stbi_write_png_to_memory")
                ->args({"x","y","comp","data","stride_bytes","block","context","at"});
        addExtern<DAS_BIND_FUN(stbi_write_bmp_to_memory)> (*this, lib, "stbi_write_bmp_to_memory",
            SideEffects::invoke, "stbi_write_bmp_to_memory")
                ->args({"x","y","comp","data","block","context","at"});
        addExtern<DAS_BIND_FUN(stbi_write_tga_to_memory)> (*this, lib, "stbi_write_tga_to_memory",
            SideEffects::invoke, "stbi_write_tga_to_memory")
                ->args({"x","y","comp","data","block","context","at"});
        addExtern<DAS_BIND_FUN(stbi_write_jpg_to_memory)> (*this, lib, "stbi_write_jpg_to_memory",
            SideEffects::invoke, "stbi_write_jpg_to_memory")
                ->args({"x","y","comp","data","quality","block","context","at"});

        // ---- stb_image_resize2: enums ----
        addEnumeration(make_smart<Enumerationstbir_pixel_layout>());
        addEnumeration(make_smart<Enumerationstbir_edge>());
        addEnumeration(make_smart<Enumerationstbir_filter>());
        addEnumeration(make_smart<Enumerationstbir_datatype>());

        // ---- stb_image_resize2: simple API ----
        addExtern<DAS_BIND_FUN(stbir_resize_uint8_srgb)> (*this, lib, "stbir_resize_uint8_srgb",
            SideEffects::modifyExternal, "stbir_resize_uint8_srgb")
                ->args({"input_pixels","input_w","input_h","input_stride_in_bytes",
                         "output_pixels","output_w","output_h","output_stride_in_bytes","pixel_type"});
        addExtern<DAS_BIND_FUN(stbir_resize_uint8_linear)> (*this, lib, "stbir_resize_uint8_linear",
            SideEffects::modifyExternal, "stbir_resize_uint8_linear")
                ->args({"input_pixels","input_w","input_h","input_stride_in_bytes",
                         "output_pixels","output_w","output_h","output_stride_in_bytes","pixel_type"});
        addExtern<DAS_BIND_FUN(stbir_resize_float_linear)> (*this, lib, "stbir_resize_float_linear",
            SideEffects::modifyExternal, "stbir_resize_float_linear")
                ->args({"input_pixels","input_w","input_h","input_stride_in_bytes",
                         "output_pixels","output_w","output_h","output_stride_in_bytes","pixel_type"});

        // ---- stb_image_resize2: medium API ----
        addExtern<DAS_BIND_FUN(stbir_resize)> (*this, lib, "stbir_resize",
            SideEffects::modifyExternal, "stbir_resize")
                ->args({"input_pixels","input_w","input_h","input_stride_in_bytes",
                         "output_pixels","output_w","output_h","output_stride_in_bytes",
                         "pixel_layout","data_type","edge","filter"});
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasStbImage/src/dasStbImage.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_StbImage,Module_StbImage);

}

REGISTER_MODULE_IN_NAMESPACE(Module_StbImage,das);
