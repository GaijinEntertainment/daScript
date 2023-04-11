#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"

#include "dasTORCH.h"

#pragma warning(disable:4251)
#pragma warning(disable:4275)
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4100)
#pragma warning(disable:4624)

#include <torch/torch.h>

namespace das {

class Module_TORCH : public Module {
public:
    Module_TORCH() : Module("torch") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();

        torch::Tensor tensor = torch::rand({2, 3});
        std::cout << tensor << std::endl;
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasTORCH/src/dasTORCH.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_TORCH,das);

