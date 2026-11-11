#pragma once

#include "daScript/misc/platform.h"

namespace das {
    class Context;
    class TextWriter;

    // function pointer for fusion library decoupling
    // runtime defines it; fusion lib sets it via register_fusion()
    DAS_API extern void (*g_fusionContextFn) ( Context & context, TextWriter & logs, bool enableFusion );
}
