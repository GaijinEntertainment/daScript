#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/simulate/aot_builtin_random.h"

#include <dag_noise/dag_uint_noise.h>

#include "daScript/misc/performance_time.h"

namespace das {

    class Module_Random : public Module {
    public:
        Module_Random() : Module("random") {
            DAS_PROFILE_SECTION("Module_Random");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // random functions
            addExtern<DAS_BIND_FUN(randomSeed)>(*this, lib, "random_seed", SideEffects::none, "randomSeed");
            addExtern<DAS_BIND_FUN(randomInt )>(*this, lib, "random_int",  SideEffects::modifyArgument, "randomInt");
            addExtern<DAS_BIND_FUN(randomInt4)>(*this, lib, "random_int4", SideEffects::modifyArgument, "randomInt4");
            addExtern<DAS_BIND_FUN(randomFloat )>(*this, lib, "random_float",  SideEffects::modifyArgument, "randomFloat");
            addExtern<DAS_BIND_FUN(randomFloat4)>(*this, lib, "random_float4", SideEffects::modifyArgument, "randomFloat4");
            addExtern<DAS_BIND_FUN(randomUnitVector  )>(*this, lib, "random_unit_vector",   SideEffects::modifyArgument, "randomUnitVector");
            addExtern<DAS_BIND_FUN(randomInUnitSphere)>(*this, lib, "random_in_unit_sphere", SideEffects::modifyArgument, "randomInUnitSphere");
            addExtern<DAS_BIND_FUN(randomInUnitDisk  )>(*this, lib, "random_in_unit_disk",   SideEffects::modifyArgument, "randomInUnitDisk");
            // lets verify all names
            uint32_t verifyFlags = uint32_t(VerifyBuiltinFlags::verifyAll);
            verifyBuiltinNames(verifyFlags);
            // and now its AOT ready
            verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_random.h\"\n";
            return ModuleAotType::cpp;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Random,das);
