#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/simulate/aot_builtin_random.h"

#include <dag_noise/dag_uint_noise.h>

namespace das {

    class Module_Random : public Module {
    public:
        Module_Random() : Module("random") {
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // random functions
            addExtern<DAS_BIND_FUN(randomSeed)>(*this, lib, "randomSeed", SideEffects::none);
            addExtern<DAS_BIND_FUN(randomInt )>(*this, lib, "randomInt",  SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomInt4)>(*this, lib, "randomInt4", SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomFloat )>(*this, lib, "randomFloat",  SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomFloat4)>(*this, lib, "randomFloat4", SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomUnitVector  )>(*this, lib, "randomUnitVector",   SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomInUnitSphere)>(*this, lib, "randomInUnitSphere", SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomInUnitDisk  )>(*this, lib, "randomInUnitDisk",   SideEffects::modifyArgument);
        }
        virtual bool aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_random.h\"\n";
            return true;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Random,das);
