#pragma once

namespace das {
    class Module_ClangBind : public Module {
    public:
        Module_ClangBind();
    protected:
#if 1
#include "dasClangBind.func.decl.inc"
#endif
    };
}
