#pragma once

#include "dasClangBind.enum.decl.inc"

namespace das {
    class Module_ClangBind : public Module {
    public:
        Module_ClangBind();
    protected:
        void addVoidAlias ( const string & aliasName );
    };
}
