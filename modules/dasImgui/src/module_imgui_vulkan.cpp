#include "daScript/daScript.h"
#include "daScript/ast/ast_typefactory_bind.h"

#include "..\imgui\backends\imgui_impl_vulkan.h"

using namespace das;

#define USE_GENERATED 1

#if USE_GENERATED

#include "..\..\dasVulkan\include\dasVulkan\module.h"

// NOTE: this module requires GLFW module
// #include "../../dasGlfw/src/module_glfw_include.h"

MAKE_EXTERNAL_TYPE_FACTORY(ImDrawData,ImDrawData);

#endif

#if USE_GENERATED

#include "module_imgui_vulkan.h"

#include "module_imgui_vulkan.enum.cpp_inc"
#include "module_imgui_vulkan.ann.cpp_inc"

#endif

// making custom builtin module
class Module_imgui_vulkan: public Module {
    ModuleLibrary lib;
public:
    Module_imgui_vulkan() : Module("imgui_vulkan") {
    }
    bool initialized = false;
    virtual bool initDependencies() override {
        if ( initialized ) return true;
        // GLFW
        auto mod_glfw = Module::require("glfw");
        if ( !mod_glfw ) return false;
        if ( !mod_glfw->initDependencies() ) return false;
        // IMGUI
        auto mod_imgui = Module::require("imgui");
        if ( !mod_imgui ) return false;
        if ( !mod_imgui->initDependencies() ) return false;
#if USE_GENERATED
        // Vulkan
        auto mod_vulkan = Module::require("vulkan");
        if ( !mod_vulkan ) return false;
        if ( !mod_vulkan->initDependencies() ) return false;
#endif
        // time to initialize
        initialized = true;
        // make basic module
        lib.addModule(this);
        lib.addBuiltInModule();
        lib.addModule(mod_glfw);
        lib.addModule(mod_imgui);
#if USE_GENERATED
        lib.addModule(mod_vulkan);
#endif
        // generated
#if USE_GENERATED
#include "module_imgui_vulkan.enum.inc"
#include "module_imgui_vulkan.ann.inc"
#include "module_imgui_vulkan.method.inc"
#include "module_imgui_vulkan.inc"
        addExtern<DAS_BIND_FUN(ImGui_ImplVulkanH_SelectPresentMode)>(*this, lib, "ImGui_ImplVulkanH_SelectPresentMode",
                SideEffects::worstDefault, "ImGui_ImplVulkanH_SelectPresentMode");
        addExtern<DAS_BIND_FUN(ImGui_ImplVulkanH_SelectSurfaceFormat),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ImGui_ImplVulkanH_SelectSurfaceFormat",
                SideEffects::worstDefault, "ImGui_ImplVulkanH_SelectSurfaceFormat");
#endif
        // all good
        return true;
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"imgui_stub.h\"\n";
        return ModuleAotType::cpp;
    }
};

// registering module, so that its available via 'NEED_MODULE' macro
REGISTER_MODULE(Module_imgui_vulkan);

