#include "daScript/daScript.h"

#include "imgui_stub.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "../imgui/backends/imgui_impl_glfw.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#include "browser_input.h"
#else
#include "native_input.h"
#endif

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
MAKE_EXTERNAL_TYPE_FACTORY(ImFont,ImFont);

#endif

DAS_MOD_API void glfw_error_callback(int error, const char* description) {
    printf("Glfw Error %d: %s\n", error, description);
}

#if defined(__EMSCRIPTEN__) && !defined(EMSCRIPTEN_USE_PORT_CONTRIB_GLFW3)
static ImGuiContext * g_emscripten_callback_owner = nullptr;

// modules/dasImgui/ARCHITECTURE.md#browser-callback-routing
static void imgui_browser_mouse_button(GLFWwindow * window, int button, int action, int mods) {
    if (!g_emscripten_callback_owner) return;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    ImGui::GetIO(g_emscripten_callback_owner).AddMousePosEvent((float)x, (float)y);
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}
#endif

#ifdef __EMSCRIPTEN__
static void imgui_browser_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_RELEASE && das_browser_block_key()) return;
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}
static void imgui_browser_character(GLFWwindow* window, unsigned int codepoint) {
    if (das_browser_block_character()) return;
    ImGui_ImplGlfw_CharCallback(window, codepoint);
}
#endif

#ifdef __EMSCRIPTEN__
static void install_imgui_browser_callbacks(GLFWwindow * window) {
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
    browser_clipboard_install();
    glfwSetKeyCallback(window, imgui_browser_key);
    glfwSetCharCallback(window, imgui_browser_character);
#ifndef EMSCRIPTEN_USE_PORT_CONTRIB_GLFW3
    g_emscripten_callback_owner = ImGui::GetCurrentContext();
    glfwSetMouseButtonCallback(window, imgui_browser_mouse_button);
#endif
}
#endif

static bool imgui_glfw_initialized ( bool ok, GLFWwindow * window ) {
#ifndef __EMSCRIPTEN__
    if ( ok ) native_ime_attach(window);
#else
    (void)window;
#endif
    return ok;
}

// modules/dasImgui/ARCHITECTURE.md#browser-callback-routing
DAS_MOD_API bool das_imgui_init_glfw_for_opengl ( GLFWwindow * window, bool install_callbacks ) {
    const bool ok = ImGui_ImplGlfw_InitForOpenGL(window, install_callbacks);
#ifdef __EMSCRIPTEN__
    if ( ok && install_callbacks )
        install_imgui_browser_callbacks(window);
#endif
    return imgui_glfw_initialized(ok, window);
}

DAS_MOD_API bool das_imgui_init_glfw_for_vulkan ( GLFWwindow * window, bool install_callbacks ) {
    return imgui_glfw_initialized(ImGui_ImplGlfw_InitForVulkan(window, install_callbacks), window);
}

DAS_MOD_API bool das_imgui_init_glfw_for_other ( GLFWwindow * window, bool install_callbacks ) {
    return imgui_glfw_initialized(ImGui_ImplGlfw_InitForOther(window, install_callbacks), window);
}

// modules/dasImgui/ARCHITECTURE.md#browser-callback-routing
DAS_MOD_API void das_imgui_shutdown_glfw() {
#ifdef __EMSCRIPTEN__
    browser_clipboard_shutdown();
#endif
#if defined(__EMSCRIPTEN__) && !defined(EMSCRIPTEN_USE_PORT_CONTRIB_GLFW3)
    if (g_emscripten_callback_owner == ImGui::GetCurrentContext()) {
        emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, false, nullptr);
        emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, false, nullptr);
        g_emscripten_callback_owner = nullptr;
    }
#endif
#ifndef __EMSCRIPTEN__
    if (native_ime.owner == ImGui::GetCurrentContext()) native_ime_detach();
#endif
    ImGui_ImplGlfw_Shutdown();
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
// modules/dasImgui/ARCHITECTURE.md#browser-callback-routing
DAS_MOD_API void das_imgui_set_real_input_callbacks ( bool enabled ) {
#ifndef __EMSCRIPTEN__
    native_ime_set_enabled(enabled);
#endif
    GLFWwindow * w = glfwGetCurrentContext();
    if ( !w ) return;
    if ( enabled ) ImGui_ImplGlfw_InstallCallbacks(w);
    else ImGui_ImplGlfw_RestoreCallbacks(w);
#ifdef __EMSCRIPTEN__
    browser_clipboard_enabled = enabled;
    browser_clipboard_tick();
    if ( enabled ) install_imgui_browser_callbacks(w);
    else emscripten_set_wheel_callback("#canvas", nullptr, false, nullptr);
#endif
}

// modules/dasImgui/ARCHITECTURE.md#native-composition
DAS_MOD_API void das_imgui_ime_inject_preedit ( const char * text, int32_t caret, int32_t split ) {
#ifndef __EMSCRIPTEN__
    if ( !ImGui::GetCurrentContext() ) return;
    if ( native_ime.owner != ImGui::GetCurrentContext() ) {
        native_ime_attach(nullptr);
        native_ime.owner_id = das::GetTextInputOwner();
    }
    const char * utf8 = text ? text : "";
    std::vector<ImWchar> codepoints(ImTextCountCharsFromUtf8(utf8, nullptr) + 1);
    const int size = ImTextStrFromUtf8(codepoints.data(), (int)codepoints.size(), utf8, nullptr);
    const int whole[1] = { size };
    const int halves[2] = { split, size - split };
    const bool two = split > 0 && split < size;
    native_ime_apply_preedit(size, codepoints.data(), size ? (two ? 2 : 1) : 0, two ? halves : whole, two ? 1 : 0, caret < 0 ? size : caret);
#else
    (void)text; (void)caret; (void)split;
#endif
}

DAS_MOD_API char * das_imgui_ime_preedit ( das::Context * context, das::LineInfoArg * at ) {
    std::string utf8;
#ifndef __EMSCRIPTEN__
    std::vector<ImWchar> text(native_ime.preedit.begin(), native_ime.preedit.end());
    text.push_back(0);
    utf8.resize(ImTextCountUtf8BytesFromStr(text.data(), nullptr) + 1);
    utf8.resize(ImTextStrToUtf8(utf8.data(), (int)utf8.size(), text.data(), nullptr));
#endif
    return context->allocateString(utf8, at);
}

DAS_MOD_API int32_t das_imgui_ime_caret () {
#ifndef __EMSCRIPTEN__
    return native_ime.caret;
#else
    return 0;
#endif
}

DAS_MOD_API uint32_t das_imgui_ime_owner () {
#ifndef __EMSCRIPTEN__
    return native_ime.owner_id;
#else
    return 0;
#endif
}

DAS_MOD_API int32_t das_imgui_ime_text_focus () {
#ifndef __EMSCRIPTEN__
    return native_ime.text_focus;
#else
    return -1;
#endif
}

DAS_MOD_API bool das_imgui_ime_has_window () {
#ifndef __EMSCRIPTEN__
    return native_ime.window != nullptr;
#else
    return false;
#endif
}

DAS_MOD_API float4 das_imgui_ime_cursor_rect () {
#ifndef __EMSCRIPTEN__
    if ( native_ime.active ) {
        int x = 0, y = 0, w = 0, h = 0;
        glfwGetPreeditCursorRectangle(native_ime.active, &x, &y, &w, &h);
        return float4((float)x, (float)y, (float)w, (float)h);
    }
    const ImVec4 & r = native_ime.cursor_rect;
    return float4(r.x, r.y, r.z, r.w);
#else
    return float4(0.0f);
#endif
}

DAS_MOD_API float4 das_imgui_ime_overlay_marks () {
#ifndef __EMSCRIPTEN__
    const ImVec4 & m = native_ime.marks;
    return float4(m.x, m.y, m.z, m.w);
#else
    return float4(0.0f);
#endif
}

DAS_MOD_API ImFont * das_imgui_ime_overlay_font () {
#ifndef __EMSCRIPTEN__
    return native_ime.overlay_font;
#else
    return nullptr;
#endif
}

DAS_MOD_API float4 das_imgui_ime_overlay () {
#ifndef __EMSCRIPTEN__
    const ImVec4 & r = native_ime.overlay;
    return float4(r.x, r.y, r.z, r.w);
#else
    return float4(0.0f);
#endif
}

DAS_MOD_API void das_imgui_glfw_new_frame() {
    ImGui_ImplGlfw_NewFrame();
#ifdef __EMSCRIPTEN__
    browser_clipboard_tick();
#endif
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
        addExtern<DAS_BIND_FUN(das_imgui_init_glfw_for_opengl)>(*this,lib,"ImGui_ImplGlfw_InitForOpenGL",
            SideEffects::worstDefault, "das_imgui_init_glfw_for_opengl");
        addExtern<DAS_BIND_FUN(das_imgui_init_glfw_for_vulkan)>(*this,lib,"ImGui_ImplGlfw_InitForVulkan",
            SideEffects::worstDefault, "das_imgui_init_glfw_for_vulkan");
        addExtern<DAS_BIND_FUN(das_imgui_init_glfw_for_other)>(*this,lib,"ImGui_ImplGlfw_InitForOther",
            SideEffects::worstDefault, "das_imgui_init_glfw_for_other");
        addExtern<DAS_BIND_FUN(das_imgui_shutdown_glfw)>(*this,lib,"ImGui_ImplGlfw_Shutdown",
            SideEffects::worstDefault, "das_imgui_shutdown_glfw");
        addExtern<DAS_BIND_FUN(das_imgui_glfw_new_frame)>(*this,lib,"ImGui_ImplGlfw_NewFrame",
            SideEffects::worstDefault, "das_imgui_glfw_new_frame");
        // Detach/reattach the backend's GLFW input callbacks at runtime — used by
        // imgui_live_core to suppress real input AT THE SOURCE when user control is off.
        addExtern<DAS_BIND_FUN(das_imgui_set_real_input_callbacks)>(*this,lib,"imgui_set_real_input_callbacks",
            SideEffects::worstDefault, "das_imgui_set_real_input_callbacks");
        addExtern<DAS_BIND_FUN(das_imgui_ime_inject_preedit)>(*this,lib,"imgui_ime_inject_preedit",
            SideEffects::worstDefault, "das_imgui_ime_inject_preedit");
        addExtern<DAS_BIND_FUN(das_imgui_ime_preedit)>(*this,lib,"imgui_ime_preedit",
            SideEffects::accessExternal, "das_imgui_ime_preedit");
        addExtern<DAS_BIND_FUN(das_imgui_ime_caret)>(*this,lib,"imgui_ime_caret",
            SideEffects::accessExternal, "das_imgui_ime_caret");
        addExtern<DAS_BIND_FUN(das_imgui_ime_owner)>(*this,lib,"imgui_ime_owner",
            SideEffects::accessExternal, "das_imgui_ime_owner");
        addExtern<DAS_BIND_FUN(das_imgui_ime_text_focus)>(*this,lib,"imgui_ime_text_focus",
            SideEffects::accessExternal, "das_imgui_ime_text_focus");
        addExtern<DAS_BIND_FUN(das_imgui_ime_has_window)>(*this,lib,"imgui_ime_has_window",
            SideEffects::accessExternal, "das_imgui_ime_has_window");
        addExtern<DAS_BIND_FUN(das_imgui_ime_cursor_rect)>(*this,lib,"imgui_ime_cursor_rect",
            SideEffects::accessExternal, "das_imgui_ime_cursor_rect");
        addExtern<DAS_BIND_FUN(das_imgui_ime_overlay)>(*this,lib,"imgui_ime_overlay",
            SideEffects::accessExternal, "das_imgui_ime_overlay");
        addExtern<DAS_BIND_FUN(das_imgui_ime_overlay_marks)>(*this,lib,"imgui_ime_overlay_marks",
            SideEffects::accessExternal, "das_imgui_ime_overlay_marks");
        addExtern<DAS_BIND_FUN(das_imgui_ime_overlay_font)>(*this,lib,"imgui_ime_overlay_font",
            SideEffects::accessExternal, "das_imgui_ime_overlay_font");
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
        tw << "DAS_MOD_API void das_imgui_shutdown_glfw();\n";
        tw << "DAS_MOD_API bool das_imgui_init_glfw_for_opengl ( GLFWwindow * window, bool install_callbacks );\n";
        tw << "DAS_MOD_API void das_imgui_synth_mouse_pos ( float x, float y );\n";
        tw << "DAS_MOD_API void das_imgui_synth_mouse_button ( int button, bool down );\n";
        tw << "DAS_MOD_API void das_imgui_synth_mouse_wheel ( float dx, float dy );\n";
        tw << "DAS_MOD_API void das_imgui_synth_key ( int key, bool down );\n";
        tw << "DAS_MOD_API void das_imgui_synth_input_char ( uint32_t cp );\n";
        tw << "DAS_MOD_API void das_imgui_set_real_input_callbacks ( bool enabled );\n";
        tw << "DAS_MOD_API void das_imgui_glfw_new_frame();\n";
        tw << "DAS_MOD_API void das_imgui_set_default_cursor ( GLFWwindow * window );\n";
        tw << "DAS_MOD_API bool das_imgui_init_glfw_for_vulkan ( GLFWwindow * window, bool install_callbacks );\n";
        tw << "DAS_MOD_API bool das_imgui_init_glfw_for_other ( GLFWwindow * window, bool install_callbacks );\n";
        tw << "DAS_MOD_API void das_imgui_ime_inject_preedit ( const char * text, int32_t caret, int32_t split );\n";
        tw << "DAS_MOD_API char * das_imgui_ime_preedit ( das::Context * context, das::LineInfoArg * at );\n";
        tw << "DAS_MOD_API int32_t das_imgui_ime_caret ();\n";
        tw << "DAS_MOD_API uint32_t das_imgui_ime_owner ();\n";
        tw << "DAS_MOD_API int32_t das_imgui_ime_text_focus ();\n";
        tw << "DAS_MOD_API bool das_imgui_ime_has_window ();\n";
        tw << "DAS_MOD_API float4 das_imgui_ime_cursor_rect ();\n";
        tw << "DAS_MOD_API float4 das_imgui_ime_overlay ();\n";
        tw << "DAS_MOD_API float4 das_imgui_ime_overlay_marks ();\n";
        tw << "DAS_MOD_API ImFont * das_imgui_ime_overlay_font ();\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_imgui_app, Module_imgui_app);

// registering module, so that its available via 'NEED_MODULE' macro
REGISTER_MODULE(Module_imgui_app);
