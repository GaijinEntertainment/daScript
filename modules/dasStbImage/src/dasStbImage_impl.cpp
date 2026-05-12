#include "daScript/misc/platform.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

// stb_image_write config variable wrappers
void stbi_write_set_png_compression_level ( int level ) { stbi_write_png_compression_level = level; }
int  stbi_write_get_png_compression_level ( ) { return stbi_write_png_compression_level; }
void stbi_write_set_force_png_filter ( int filter ) { stbi_write_force_png_filter = filter; }
int  stbi_write_get_force_png_filter ( ) { return stbi_write_force_png_filter; }
void stbi_write_set_tga_with_rle ( int rle ) { stbi_write_tga_with_rle = rle; }
int  stbi_write_get_tga_with_rle ( ) { return stbi_write_tga_with_rle; }

// stbi_write_*_to_func callback — accumulates into a vector
void stbi_write_to_vec_callback ( void * context, void * data, int size ) {
    auto * vec = (std::vector<uint8_t> *) context;
    auto * src = (const uint8_t *) data;
    vec->insert(vec->end(), src, src + size);
}

// APNG (animated PNG) writer — defined here so stb's static helpers are in scope.
#include "apng_write_impl.h"
