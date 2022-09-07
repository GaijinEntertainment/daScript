#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"


namespace das {

class Module_dasLlvm : public Module {
public:
    Module_dasLlvm() : Module("llvm") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
//        addExtern<DAS_BIND_FUN(stbi_load_from_memory)> (*this, lib, "stbi_load_from_memory",
//            SideEffects::worstDefault, "stbi_load_from_memory")
//                ->args({"buffer","len","x","y","comp","req_comp"});
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasLLVM/src/dasLLVM.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_dasLlvm,das);

