#pragma once

#include "daScript/daScript.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/interop.h"
#include "daScript/simulate/bind_enum.h"

#include <volk.h>

// type factories + enum cast/factory specializations for every bound Vulkan type
#include "dasVULKAN.gen.h"

namespace das {

// implemented across the generated dasVULKAN.gen_*.cpp chunks
void das_vulkan_init_generated(Module & mod, ModuleLibrary & lib);

class Module_dasVULKAN : public Module {
public:
    Module_dasVULKAN();
    virtual ModuleAotType aotRequire(TextWriter & tw) const override;
};

}
