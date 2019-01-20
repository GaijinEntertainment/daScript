#pragma once

#include "daScript/ast/ast.h"

namespace das {
    class Module_BuiltIn : public Module {
    public:
        Module_BuiltIn();
    protected:
        void addRuntime(ModuleLibrary & lib);
        void addVectorTypes(ModuleLibrary & lib);
        void addMatrixTypes(ModuleLibrary & lib);
        void addArrayTypes(ModuleLibrary & lib);
        void addTime(ModuleLibrary & lib);
        void addMath(ModuleLibrary & lib);
		bool appendCompiledFunctions();
    };
}
