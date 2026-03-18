#pragma once

#include "stb_truetype.h"

// Wrapper functions for multi-out-param stbtt functions.
// daslang can bind int& params directly, but these wrappers
// provide a cleaner interface for common operations.

// Glyph shape vertex — exposed to daScript as a simple struct
struct stbtt_vertex_das {
    int16_t x, y;       // endpoint
    int16_t cx, cy;     // quadratic control point
    int16_t cx1, cy1;   // cubic control point
    uint8_t type;       // STBTT_vmove=1, vline=2, vcurve=3, vcubic=4
};

namespace das {
    void stbtt_GetCodepointShape_foreach(const stbtt_fontinfo * info, int codepoint,
        const TBlock<void, const stbtt_vertex_das &> & block, Context * ctx, LineInfoArg * at);
    int stbtt_GetCodepointShape_count(const stbtt_fontinfo * info, int codepoint);
}
