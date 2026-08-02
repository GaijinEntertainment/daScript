#include "daScript/daScript.h"

#include "imgui_stub.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "../imgui/backends/imgui_impl_glfw.h"

using namespace das;

#define USE_GENERATED 1

#if USE_GENERATED

// NOTE: this module requires GLFW module
#include "need_dasGLFW.h"
// In a daslang superbuild dasGlfw's own binding TU already provides the strong
// typeFactory<GLFWwindow>::make definition — implementing it here too is a duplicate
// symbol in the static link (the web build papers over the same collision with
// -Wl,--allow-multiple-definition). The superbuild CMake branch sets the guard; the
// standalone .shared_module build (no dasGlfw objects in its link) keeps its own copy.
#ifndef DAS_IMGUI_APP_EXTERN_GLFW_TYPE_FACTORY
IMPLEMENT_EXTERNAL_TYPE_FACTORY(GLFWwindow,GLFWwindow)
#endif

MAKE_EXTERNAL_TYPE_FACTORY(ImDrawData,ImDrawData);

#endif

DAS_MOD_API void glfw_error_callback(int error, const char* description) {
    printf("Glfw Error %d: %s\n", error, description);
}

// =====================================================================
// Synthetic IO bypass — direct injection into ImGui's input queue,
// skipping the GLFW backend chain. Used by imgui_live's synth driver to
// fix the click-then-Ctrl+A shortcut-routing bug (project memory:
// project_imgui_synth_click_breaks_ctrl_shortcuts). The driver appends
// synth events AFTER the backend's per-frame poll so synth wins the
// last-write-wins race at NewFrame drain time.
// =====================================================================

DAS_MOD_API void das_imgui_synth_mouse_pos ( float x, float y ) {
    ImGui::GetIO().AddMousePosEvent(x, y);
}

DAS_MOD_API void das_imgui_synth_mouse_button ( int button, bool down ) {
    ImGui::GetIO().AddMouseButtonEvent(button, down);
}

DAS_MOD_API void das_imgui_synth_mouse_wheel ( float dx, float dy ) {
    ImGui::GetIO().AddMouseWheelEvent(dx, dy);
}

DAS_MOD_API void das_imgui_synth_key ( int key, bool down ) {
    ImGui::GetIO().AddKeyEvent((ImGuiKey)key, down);
}

DAS_MOD_API void das_imgui_synth_input_char ( uint32_t cp ) {
    ImGui::GetIO().AddInputCharacter(cp);
}

// Detach (false) / reattach (true) the GLFW backend's input callbacks for the
// current-context window, so when imgui_live_core disables user control real
// mouse/keyboard stop reaching ImGui AT THE SOURCE (no callbacks fire) — closing
// the race a per-frame event-queue clear can't win. Synth IO uses io.Add*Event
// directly and is unaffected. No-op when there is no current GLFW window (headless).
// Caller toggles only on a state change, honoring ImGui_ImplGlfw's
// InstalledCallbacks invariant (Install asserts when already installed, and
// vice-versa). Must run on the render/main thread — glfwSet*Callback requires it.
DAS_MOD_API void das_imgui_set_real_input_callbacks ( bool enabled ) {
    GLFWwindow * w = glfwGetCurrentContext();
    if ( !w ) return;
    if ( enabled ) ImGui_ImplGlfw_InstallCallbacks(w);
    else ImGui_ImplGlfw_RestoreCallbacks(w);
}

// Set a defined Arrow cursor on the window at init, before the first frame. The GLFW
// backend's per-frame UpdateMouseCursor only kicks in from frame 1, and on macOS Cocoa
// only refreshes the cursor on movement — so without this the window shows whatever
// shape was inherited at creation until the mouse moves. One standard cursor, created
// once; GLFW frees it at glfwTerminate.
DAS_MOD_API void das_imgui_set_default_cursor ( GLFWwindow * window ) {
    if ( !window ) return;
    static GLFWcursor * arrow = nullptr;
    if ( !arrow ) arrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    if ( arrow ) glfwSetCursor(window, arrow);
}

// making custom builtin module
class Module_imgui_app : public Module {
    ModuleLibrary lib;
public:
    Module_imgui_app() : Module("imgui_app") {
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
        // time to initialize
        initialized = true;
        // error callback
        glfwSetErrorCallback(glfw_error_callback);
        // make basic module
        lib.addModule(this);
        lib.addBuiltInModule();
        lib.addModule(mod_glfw);
        lib.addModule(mod_imgui);
#if USE_GENERATED
        // GLFW
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_InitForOpenGL)>(*this,lib,"ImGui_ImplGlfw_InitForOpenGL",
            SideEffects::worstDefault, "ImGui_ImplGlfw_InitForOpenGL");
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_InitForVulkan)>(*this,lib,"ImGui_ImplGlfw_InitForVulkan",
            SideEffects::worstDefault, "ImGui_ImplGlfw_InitForVulkan");
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_InitForOther)>(*this,lib,"ImGui_ImplGlfw_InitForOther",
            SideEffects::worstDefault, "ImGui_ImplGlfw_InitForOther");
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_Shutdown)>(*this,lib,"ImGui_ImplGlfw_Shutdown",
            SideEffects::worstDefault, "ImGui_ImplGlfw_Shutdown");
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_NewFrame)>(*this,lib,"ImGui_ImplGlfw_NewFrame",
            SideEffects::worstDefault, "ImGui_ImplGlfw_NewFrame");
        // Detach/reattach the backend's GLFW input callbacks at runtime — used by
        // imgui_live_core to suppress real input AT THE SOURCE when user control is off.
        addExtern<DAS_BIND_FUN(das_imgui_set_real_input_callbacks)>(*this,lib,"imgui_set_real_input_callbacks",
            SideEffects::worstDefault, "das_imgui_set_real_input_callbacks");
        // Default Arrow cursor at init (covers the pre-first-frame / macOS no-move-refresh gap).
        addExtern<DAS_BIND_FUN(das_imgui_set_default_cursor)>(*this,lib,"imgui_set_default_cursor",
            SideEffects::worstDefault, "das_imgui_set_default_cursor");
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_MouseButtonCallback)>(*this,lib,"ImGui_ImplGlfw_MouseButtonCallback",
            SideEffects::worstDefault, "ImGui_ImplGlfw_MouseButtonCallback");
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_ScrollCallback)>(*this,lib,"ImGui_ImplGlfw_ScrollCallback",
            SideEffects::worstDefault, "ImGui_ImplGlfw_ScrollCallback");
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_KeyCallback)>(*this,lib,"ImGui_ImplGlfw_KeyCallback",
            SideEffects::worstDefault, "ImGui_ImplGlfw_KeyCallback");
        addExtern<DAS_BIND_FUN(ImGui_ImplGlfw_CharCallback)>(*this,lib,"ImGui_ImplGlfw_CharCallback",
            SideEffects::worstDefault, "ImGui_ImplGlfw_CharCallback");
#endif
        // Synth IO bypass (imgui_live driver).
        addExtern<DAS_BIND_FUN(das_imgui_synth_mouse_pos)>(*this,lib,"imgui_synth_mouse_pos",
            SideEffects::worstDefault, "das_imgui_synth_mouse_pos");
        addExtern<DAS_BIND_FUN(das_imgui_synth_mouse_button)>(*this,lib,"imgui_synth_mouse_button",
            SideEffects::worstDefault, "das_imgui_synth_mouse_button");
        addExtern<DAS_BIND_FUN(das_imgui_synth_mouse_wheel)>(*this,lib,"imgui_synth_mouse_wheel",
            SideEffects::worstDefault, "das_imgui_synth_mouse_wheel");
        addExtern<DAS_BIND_FUN(das_imgui_synth_key)>(*this,lib,"imgui_synth_key",
            SideEffects::worstDefault, "das_imgui_synth_key");
        addExtern<DAS_BIND_FUN(das_imgui_synth_input_char)>(*this,lib,"imgui_synth_input_char",
            SideEffects::worstDefault, "das_imgui_synth_input_char");
        // all good
        return true;
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasImgui/src/imgui_stub.h\"\n";
        tw << "#include <backends/imgui_impl_glfw.h>\n";
        tw << "DAS_MOD_API void das_imgui_synth_mouse_pos ( float x, float y );\n";
        tw << "DAS_MOD_API void das_imgui_synth_mouse_button ( int button, bool down );\n";
        tw << "DAS_MOD_API void das_imgui_synth_mouse_wheel ( float dx, float dy );\n";
        tw << "DAS_MOD_API void das_imgui_synth_key ( int key, bool down );\n";
        tw << "DAS_MOD_API void das_imgui_synth_input_char ( uint32_t cp );\n";
        tw << "DAS_MOD_API void das_imgui_set_real_input_callbacks ( bool enabled );\n";
        tw << "DAS_MOD_API void das_imgui_set_default_cursor ( GLFWwindow * window );\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_imgui_app, Module_imgui_app);

// registering module, so that its available via 'NEED_MODULE' macro
REGISTER_MODULE(Module_imgui_app);

