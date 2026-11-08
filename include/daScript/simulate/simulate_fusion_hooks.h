#pragma once

#include "daScript/misc/platform.h"

namespace das {
    class Context;
    class TextWriter;

    // function pointers for fusion library decoupling
    // runtime defines these; fusion lib sets them via register_fusion()
    DAS_API extern void (*g_fusionContextFn) ( Context & context, TextWriter & logs, bool enableFusion );
    DAS_API extern void (*g_resetFusionEngineFn) ( bool orphan );   // orphan: forget the table without freeing it (a process about to exit)
}
