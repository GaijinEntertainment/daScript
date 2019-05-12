#include "daScript/misc/platform.h"

#include "module_builtin.h"

namespace das {

    #include "fio.das.inc"

    class Module_FIO : public Module {
    public:
        Module_FIO() : Module("fio") {
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // add builtin module
            compileBuiltinModule("fio.das",fio_das, fio_das_len);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_FIO,das);
