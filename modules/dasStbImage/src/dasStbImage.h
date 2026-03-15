#pragma once


#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize2.h"

// stb_image_write config variable wrappers
void stbi_write_set_png_compression_level ( int level );
int  stbi_write_get_png_compression_level ( );
void stbi_write_set_force_png_filter ( int filter );
int  stbi_write_get_force_png_filter ( );
void stbi_write_set_tga_with_rle ( int rle );
int  stbi_write_get_tga_with_rle ( );

namespace das {
    // write-to-memory wrappers
    void stbi_write_png_to_memory ( int x, int y, int comp, const void * data, int stride_bytes,
        const TBlock<void,TTemporary<const TArray<uint8_t>>> & blk, Context * context, LineInfoArg * at );
    void stbi_write_bmp_to_memory ( int x, int y, int comp, const void * data,
        const TBlock<void,TTemporary<const TArray<uint8_t>>> & blk, Context * context, LineInfoArg * at );
    void stbi_write_tga_to_memory ( int x, int y, int comp, const void * data,
        const TBlock<void,TTemporary<const TArray<uint8_t>>> & blk, Context * context, LineInfoArg * at );
    void stbi_write_jpg_to_memory ( int x, int y, int comp, const void * data, int quality,
        const TBlock<void,TTemporary<const TArray<uint8_t>>> & blk, Context * context, LineInfoArg * at );
}
