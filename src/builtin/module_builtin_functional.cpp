#include "daScript/misc/platform.h"

#include "module_builtin.h"

namespace das {

    #include "functional.das.inc"

    class Module_Functional : public Module {
    public:
        Module_Functional() : Module("functional") {
            // add builtin module
            compileBuiltinModule("functional.das",functional_das, sizeof(functional_das));
            // lets verify all names
            uint32_t verifyFlags = uint32_t(VerifyBuiltinFlags::verifyAll);
            // verifyFlags &= ~VerifyBuiltinFlags::verifyHandleTypes;  // we skip annotatins due to FILE and FStat
            verifyBuiltinNames(verifyFlags);
            // and now its aot ready
            verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            // tw << "#include \"daScript/simulate/aot_builtin_fio.h\"\n";
            return ModuleAotType::cpp;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Functional,das);

