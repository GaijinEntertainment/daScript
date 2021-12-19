#pragma once

#include <clang-c/Index.h>

#include "dasClangBind.enum.decl.inc"
#include "dasClangBind.struct.decl.inc"

namespace das {
    #include "dasClangBind.struct.cast.inc"
}