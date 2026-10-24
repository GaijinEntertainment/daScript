// cuda - the witness that this build was CONFIGURED with dasCUDA. The package itself is pure
// das (dasbind late externs over the driver and NVRTC), so nothing else in it can answer "is
// CUDA part of this build" - its files sit on disk in every checkout, whatever
// DAS_CUDA_DISABLED says. This module is compiled in only when the package is, so
// `require ?cuda <target>`, `builtin_module_exists(cuda)` and `has_module("cuda")` follow the
// configure, never the filesystem: a build without CUDA loads no CUDA home into dasLLAMA and
// runs no tests/cuda cell.

#include "daScript/daScript.h"

namespace das {

class Module_CUDA : public Module {
public:
    Module_CUDA() : Module("cuda") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
    }
    virtual ModuleAotType aotRequire ( TextWriter & ) const override {
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_CUDA, Module_CUDA);

} // namespace das

REGISTER_MODULE_IN_NAMESPACE(Module_CUDA, das);
