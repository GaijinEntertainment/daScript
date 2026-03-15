#pragma once

#include "stb_truetype.h"

// Wrapper functions for multi-out-param stbtt functions.
// daslang can bind int& params directly, but these wrappers
// provide a cleaner interface for common operations.

namespace das {
    // No additional wrappers needed — stbtt functions with int* out params
    // are bound directly using daslang's pointer/reference parameter support.
}
