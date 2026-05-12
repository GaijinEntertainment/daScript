#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "dasGLFW.h"
#include "need_dasGLFW.h"
#include "aot_dasGLFW.h"

#if defined(_MSC_VER)

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace das {
    void * DAS_glfwGetNativeWindow(GLFWwindow* window) {
        auto wnd = glfwGetWin32Window(window);
        return (void *) wnd;
    }

    void * DAS_glfwGetNativeDisplay() { return nullptr; }
}

#elif __APPLE__

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

namespace das {
    void * DAS_glfwGetNativeWindow(GLFWwindow* window) {
        auto wnd = glfwGetCocoaWindow(window);
        return (void *) wnd;
    }

    void * DAS_glfwGetNativeDisplay() { return nullptr; }

}

#elif defined(__linux__)

// forward declaration only, cause allowing native headers breaks compilation here
typedef long Window;
extern "C" GLFWAPI Window glfwGetX11Window(GLFWwindow* window);
typedef long Display;
extern "C" GLFWAPI Display* glfwGetX11Display(void);

namespace das {
    void * DAS_glfwGetNativeWindow(GLFWwindow* window) {
        auto wnd = glfwGetX11Window(window);
        return (void *) wnd;
    }

    void * DAS_glfwGetNativeDisplay() {
        auto wnd = glfwGetX11Display();
        return (void *) wnd;
    }
}


#else
namespace das {
    void * DAS_glfwGetNativeWindow(GLFWwindow* window) {
        return nullptr;
    }
}

#endif


namespace das {

    void DasGlfw_KeyFunction ( GLFWwindow* window, int keyCode, int scanCode, int action, int modes ) {
        auto & cb = Module_dasGLFW::g_Callbacks[window].keyCB;
        if ( cb.context ) {
            das_invoke_lambda<void>::invoke<GLFWwindow *,int,int,int,int>(cb.context,nullptr,cb.lambda,
                window,keyCode,scanCode,action,modes);
        }
    }

    void DasGlfw_CharFunction ( GLFWwindow* window, unsigned int cp ) {
        auto & cb = Module_dasGLFW::g_Callbacks[window].charCB;
        if ( cb.context ) {
            das_invoke_lambda<void>::invoke<GLFWwindow *,unsigned int>(cb.context,nullptr,cb.lambda,
                window,cp);
        }
    }

    void DasGlfw_CursorPosFunction ( GLFWwindow* window, double x, double y ) {
        auto & cb = Module_dasGLFW::g_Callbacks[window].cursorCB;
        if ( cb.context ) {
            das_invoke_lambda<void>::invoke<GLFWwindow *,double,double>(cb.context,nullptr,cb.lambda,
                window,x,y);
        }
    }

    void DasGlfw_CursorEnterFunction ( GLFWwindow* window, int enter ) {
        auto & cb = Module_dasGLFW::g_Callbacks[window].cursorEnterCB;
        if ( cb.context ) {
            das_invoke_lambda<void>::invoke<GLFWwindow *,bool>(cb.context,nullptr,cb.lambda,
                window,enter!=0);
        }
    }

    void DasGlfw_MouseFunction ( GLFWwindow* window, int buttons, int action, int modes ) {
        auto & cb = Module_dasGLFW::g_Callbacks[window].mouseCB;
        if ( cb.context ) {
            das_invoke_lambda<void>::invoke<GLFWwindow *,int,int,int>(cb.context,nullptr,cb.lambda,
                window,buttons,action,modes);
        }
    }

    void DasGlfw_ScrollFunction ( GLFWwindow* window, double x, double y ) {
        auto & cb = Module_dasGLFW::g_Callbacks[window].scrollCB;
        if ( cb.context ) {
            das_invoke_lambda<void>::invoke<GLFWwindow *,double,double>(cb.context,nullptr,cb.lambda,
                window,x,y);
        }
    }

    void DasGlfw_SetKeyCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,int,int,int,int> func, Context * ctx ) {
        glfwSetKeyCallback(window,DasGlfw_KeyFunction);
        Module_dasGLFW::g_Callbacks[window].keyCB = { func, ctx };
    }

    void DasGlfw_SetCharCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,unsigned int> func, Context * ctx ) {
        glfwSetCharCallback(window,DasGlfw_CharFunction);
        Module_dasGLFW::g_Callbacks[window].charCB = { func, ctx };
    }

    void DasGlfw_SetCursorPosCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,double,double> func, Context * ctx ) {
        glfwSetCursorPosCallback(window,DasGlfw_CursorPosFunction);
        Module_dasGLFW::g_Callbacks[window].cursorCB = { func, ctx };
    }

    void DasGlfw_SetCursorEnterCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,bool> func, Context * ctx ) {
        glfwSetCursorEnterCallback(window,DasGlfw_CursorEnterFunction);
        Module_dasGLFW::g_Callbacks[window].cursorEnterCB = { func, ctx };
    }

    void DasGlfw_SetMouseButtonCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,int,int,int> func, Context * ctx ) {
        glfwSetMouseButtonCallback(window,DasGlfw_MouseFunction);
        Module_dasGLFW::g_Callbacks[window].mouseCB = { func, ctx };
    }

    void DasGlfw_SetScrollCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,double,double> func, Context * ctx ) {
        glfwSetScrollCallback(window,DasGlfw_ScrollFunction);
        Module_dasGLFW::g_Callbacks[window].scrollCB = { func, ctx };
    }

    // === Chain callbacks ===
    // Multi-listener registry alongside the legacy single-slot g_Callbacks.
    // Captures any pre-installed C-callback (e.g. ImGui_ImplGlfw) so synthetic
    // events posted via DasGlfw_Post* reach every real listener too.

    struct GlfwChainEntry {
        Lambda    lambda;
        Context * context = nullptr;
    };

    struct GlfwChainState {
        GLFWcursorposfun                prev_cursor_pos    = nullptr;
        GLFWmousebuttonfun              prev_mouse_button  = nullptr;
        GLFWscrollfun                   prev_scroll        = nullptr;
        GLFWkeyfun                      prev_key           = nullptr;
        GLFWcharfun                     prev_char          = nullptr;
        // Per-callback install flags. ChainClear must NOT touch a slot the chain
        // never installed — otherwise it would clobber a foreign callback (e.g.
        // ImGui_ImplGlfw_KeyCallback) that was set up independently of us.
        bool                            installed_cursor_pos   = false;
        bool                            installed_mouse_button = false;
        bool                            installed_scroll       = false;
        bool                            installed_key          = false;
        bool                            installed_char         = false;
        std::vector<GlfwChainEntry>     cursor_pos_chain;
        std::vector<GlfwChainEntry>     mouse_button_chain;
        std::vector<GlfwChainEntry>     scroll_chain;
        std::vector<GlfwChainEntry>     key_chain;
        std::vector<GlfwChainEntry>     char_chain;
    };

    static thread_local das_map<void *, GlfwChainState> g_GlfwChain;

    void DasGlfw_ChainCursorPosDispatch ( GLFWwindow * w, double x, double y ) {
        auto it = g_GlfwChain.find(w);
        if ( it == g_GlfwChain.end() ) return;
        auto & st = it->second;
        if ( st.prev_cursor_pos ) st.prev_cursor_pos(w, x, y);
        for ( auto & e : st.cursor_pos_chain ) {
            if ( e.context ) {
                das_invoke_lambda<void>::invoke<GLFWwindow *, double, double>(
                    e.context, nullptr, e.lambda, w, x, y);
            }
        }
    }

    void DasGlfw_ChainMouseButtonDispatch ( GLFWwindow * w, int button, int action, int mods ) {
        auto it = g_GlfwChain.find(w);
        if ( it == g_GlfwChain.end() ) return;
        auto & st = it->second;
        if ( st.prev_mouse_button ) st.prev_mouse_button(w, button, action, mods);
        for ( auto & e : st.mouse_button_chain ) {
            if ( e.context ) {
                das_invoke_lambda<void>::invoke<GLFWwindow *, int, int, int>(
                    e.context, nullptr, e.lambda, w, button, action, mods);
            }
        }
    }

    void DasGlfw_ChainScrollDispatch ( GLFWwindow * w, double xoff, double yoff ) {
        auto it = g_GlfwChain.find(w);
        if ( it == g_GlfwChain.end() ) return;
        auto & st = it->second;
        if ( st.prev_scroll ) st.prev_scroll(w, xoff, yoff);
        for ( auto & e : st.scroll_chain ) {
            if ( e.context ) {
                das_invoke_lambda<void>::invoke<GLFWwindow *, double, double>(
                    e.context, nullptr, e.lambda, w, xoff, yoff);
            }
        }
    }

    void DasGlfw_ChainKeyDispatch ( GLFWwindow * w, int key, int scancode, int action, int mods ) {
        auto it = g_GlfwChain.find(w);
        if ( it == g_GlfwChain.end() ) return;
        auto & st = it->second;
        if ( st.prev_key ) st.prev_key(w, key, scancode, action, mods);
        for ( auto & e : st.key_chain ) {
            if ( e.context ) {
                das_invoke_lambda<void>::invoke<GLFWwindow *, int, int, int, int>(
                    e.context, nullptr, e.lambda, w, key, scancode, action, mods);
            }
        }
    }

    void DasGlfw_ChainCharDispatch ( GLFWwindow * w, unsigned int codepoint ) {
        auto it = g_GlfwChain.find(w);
        if ( it == g_GlfwChain.end() ) return;
        auto & st = it->second;
        if ( st.prev_char ) st.prev_char(w, codepoint);
        for ( auto & e : st.char_chain ) {
            if ( e.context ) {
                das_invoke_lambda<void>::invoke<GLFWwindow *, unsigned int>(
                    e.context, nullptr, e.lambda, w, codepoint);
            }
        }
    }

    // Idempotent dispatcher install: always re-calls glfwSet*Callback so a
    // non-self previous is captured as the chain's "prev" (preserves whatever
    // ImGui_ImplGlfw or other backend installed). Self-reinstalls are no-ops
    // for prev.
    static void ensure_chain_cursor_pos_installed ( GLFWwindow * w ) {
        auto & st = g_GlfwChain[w];
        auto previous = glfwSetCursorPosCallback(w, DasGlfw_ChainCursorPosDispatch);
        if ( previous && previous != &DasGlfw_ChainCursorPosDispatch ) {
            st.prev_cursor_pos = previous;
        }
        st.installed_cursor_pos = true;
    }
    static void ensure_chain_mouse_button_installed ( GLFWwindow * w ) {
        auto & st = g_GlfwChain[w];
        auto previous = glfwSetMouseButtonCallback(w, DasGlfw_ChainMouseButtonDispatch);
        if ( previous && previous != &DasGlfw_ChainMouseButtonDispatch ) {
            st.prev_mouse_button = previous;
        }
        st.installed_mouse_button = true;
    }
    static void ensure_chain_scroll_installed ( GLFWwindow * w ) {
        auto & st = g_GlfwChain[w];
        auto previous = glfwSetScrollCallback(w, DasGlfw_ChainScrollDispatch);
        if ( previous && previous != &DasGlfw_ChainScrollDispatch ) {
            st.prev_scroll = previous;
        }
        st.installed_scroll = true;
    }
    static void ensure_chain_key_installed ( GLFWwindow * w ) {
        auto & st = g_GlfwChain[w];
        auto previous = glfwSetKeyCallback(w, DasGlfw_ChainKeyDispatch);
        if ( previous && previous != &DasGlfw_ChainKeyDispatch ) {
            st.prev_key = previous;
        }
        st.installed_key = true;
    }
    static void ensure_chain_char_installed ( GLFWwindow * w ) {
        auto & st = g_GlfwChain[w];
        auto previous = glfwSetCharCallback(w, DasGlfw_ChainCharDispatch);
        if ( previous && previous != &DasGlfw_ChainCharDispatch ) {
            st.prev_char = previous;
        }
        st.installed_char = true;
    }

    void DasGlfw_ChainAddCursorPos ( GLFWwindow * w, TLambda<void,const GLFWwindow*,double,double> func, Context * ctx ) {
        ensure_chain_cursor_pos_installed(w);
        g_GlfwChain[w].cursor_pos_chain.push_back({ func, ctx });
    }

    void DasGlfw_ChainAddMouseButton ( GLFWwindow * w, TLambda<void,const GLFWwindow*,int,int,int> func, Context * ctx ) {
        ensure_chain_mouse_button_installed(w);
        g_GlfwChain[w].mouse_button_chain.push_back({ func, ctx });
    }

    void DasGlfw_ChainAddScroll ( GLFWwindow * w, TLambda<void,const GLFWwindow*,double,double> func, Context * ctx ) {
        ensure_chain_scroll_installed(w);
        g_GlfwChain[w].scroll_chain.push_back({ func, ctx });
    }

    void DasGlfw_ChainAddKey ( GLFWwindow * w, TLambda<void,const GLFWwindow*,int,int,int,int> func, Context * ctx ) {
        ensure_chain_key_installed(w);
        g_GlfwChain[w].key_chain.push_back({ func, ctx });
    }

    void DasGlfw_ChainAddChar ( GLFWwindow * w, TLambda<void,const GLFWwindow*,unsigned int> func, Context * ctx ) {
        ensure_chain_char_installed(w);
        g_GlfwChain[w].char_chain.push_back({ func, ctx });
    }

    void DasGlfw_ChainClear ( GLFWwindow * w ) {
        auto it = g_GlfwChain.find(w);
        if ( it == g_GlfwChain.end() ) return;
        // Restore previous GLFW callbacks (e.g. ImGui_ImplGlfw's) only for the
        // slots we actually installed. Restoring an uninstalled slot would
        // overwrite a foreign callback set up independently of us with the
        // default-null prev_*, silently dropping that input.
        auto & st = it->second;
        if ( st.installed_cursor_pos   ) glfwSetCursorPosCallback   (w, st.prev_cursor_pos);
        if ( st.installed_mouse_button ) glfwSetMouseButtonCallback (w, st.prev_mouse_button);
        if ( st.installed_scroll       ) glfwSetScrollCallback      (w, st.prev_scroll);
        if ( st.installed_key          ) glfwSetKeyCallback         (w, st.prev_key);
        if ( st.installed_char         ) glfwSetCharCallback        (w, st.prev_char);
        g_GlfwChain.erase(it);
    }

    void DasGlfw_PostCursorPos ( GLFWwindow * w, double x, double y ) {
        // ImGui_ImplGlfw_UpdateMouseData re-polls glfwGetCursorPos every frame
        // on focused windows; without glfwSetCursorPos the synthetic position
        // is overwritten one frame after the chain fires. Warps the visible OS
        // cursor as a side effect — call DasGlfw_DispatchCursorPos when that's
        // unwanted (tests / interactive desktops).
        ensure_chain_cursor_pos_installed(w);
        glfwSetCursorPos(w, x, y);
        DasGlfw_ChainCursorPosDispatch(w, x, y);
    }

    void DasGlfw_DispatchCursorPos ( GLFWwindow * w, double x, double y ) {
        ensure_chain_cursor_pos_installed(w);
        DasGlfw_ChainCursorPosDispatch(w, x, y);
    }

    void DasGlfw_PostMouseButton ( GLFWwindow * w, int button, int action, int mods ) {
        ensure_chain_mouse_button_installed(w);
        DasGlfw_ChainMouseButtonDispatch(w, button, action, mods);
    }

    void DasGlfw_PostScroll ( GLFWwindow * w, double xoff, double yoff ) {
        ensure_chain_scroll_installed(w);
        DasGlfw_ChainScrollDispatch(w, xoff, yoff);
    }

    void DasGlfw_PostKey ( GLFWwindow * w, int key, int scancode, int action, int mods ) {
        ensure_chain_key_installed(w);
        DasGlfw_ChainKeyDispatch(w, key, scancode, action, mods);
    }

    void DasGlfw_PostChar ( GLFWwindow * w, unsigned int codepoint ) {
        ensure_chain_char_installed(w);
        DasGlfw_ChainCharDispatch(w, codepoint);
    }

    void DasGlfw_DestroyWindow ( GLFWwindow * window ) {
        auto it = Module_dasGLFW::g_Callbacks.find(window);
        if ( it!=Module_dasGLFW::g_Callbacks.end() ) Module_dasGLFW::g_Callbacks.erase(it);
        DasGlfw_ChainClear(window);
        glfwDestroyWindow(window);
    }

    thread_local das_map<void *, GlswCallbacks> Module_dasGLFW::g_Callbacks;

    Module_dasGLFW::~Module_dasGLFW() {
        Module_dasGLFW::g_Callbacks = das_map<void *, GlswCallbacks>{};
        g_GlfwChain = das_map<void *, GlfwChainState>{};
    }

	void Module_dasGLFW::initMain () {
        // callbacks
        addExtern<DAS_BIND_FUN(DasGlfw_SetKeyCallback)>(*this,lib,"glfwSetKeyCallback",
            SideEffects::worstDefault,"DasGlfw_SetKeyCallback");
        addExtern<DAS_BIND_FUN(DasGlfw_SetCharCallback)>(*this,lib,"glfwSetCharCallback",
            SideEffects::worstDefault,"DasGlfw_SetCharCallback");
        addExtern<DAS_BIND_FUN(DasGlfw_SetCursorPosCallback)>(*this,lib,"glfwSetCursorPosCallback",
            SideEffects::worstDefault,"DasGlfw_SetCursorPosCallback");
        addExtern<DAS_BIND_FUN(DasGlfw_SetCursorEnterCallback)>(*this,lib,"glfwSetCursorEnterCallback",
            SideEffects::worstDefault,"DasGlfw_SetCursorEnterCallback");
        addExtern<DAS_BIND_FUN(DasGlfw_SetMouseButtonCallback)>(*this,lib,"glfwSetMouseButtonCallback",
            SideEffects::worstDefault,"DasGlfw_SetMouseButtonCallback");
        addExtern<DAS_BIND_FUN(DasGlfw_SetScrollCallback)>(*this,lib,"glfwSetScrollCallback",
            SideEffects::worstDefault,"DasGlfw_SetScrollCallback");
        addExtern<DAS_BIND_FUN(DasGlfw_DestroyWindow)>(*this,lib,"glfwDestroyWindow",
            SideEffects::worstDefault,"DasGlfw_DestroyWindow");
        // chain + synthetic event API
        addExtern<DAS_BIND_FUN(DasGlfw_ChainAddCursorPos)>(*this,lib,"glfw_chain_add_cursor_pos",
            SideEffects::worstDefault,"DasGlfw_ChainAddCursorPos");
        addExtern<DAS_BIND_FUN(DasGlfw_ChainAddMouseButton)>(*this,lib,"glfw_chain_add_mouse_button",
            SideEffects::worstDefault,"DasGlfw_ChainAddMouseButton");
        addExtern<DAS_BIND_FUN(DasGlfw_ChainAddScroll)>(*this,lib,"glfw_chain_add_scroll",
            SideEffects::worstDefault,"DasGlfw_ChainAddScroll");
        addExtern<DAS_BIND_FUN(DasGlfw_ChainClear)>(*this,lib,"glfw_chain_clear",
            SideEffects::worstDefault,"DasGlfw_ChainClear");
        addExtern<DAS_BIND_FUN(DasGlfw_PostCursorPos)>(*this,lib,"glfw_post_cursor_pos",
            SideEffects::worstDefault,"DasGlfw_PostCursorPos");
        addExtern<DAS_BIND_FUN(DasGlfw_DispatchCursorPos)>(*this,lib,"glfw_dispatch_cursor_pos",
            SideEffects::worstDefault,"DasGlfw_DispatchCursorPos");
        addExtern<DAS_BIND_FUN(DasGlfw_PostMouseButton)>(*this,lib,"glfw_post_mouse_button",
            SideEffects::worstDefault,"DasGlfw_PostMouseButton");
        addExtern<DAS_BIND_FUN(DasGlfw_PostScroll)>(*this,lib,"glfw_post_scroll",
            SideEffects::worstDefault,"DasGlfw_PostScroll");
        addExtern<DAS_BIND_FUN(DasGlfw_ChainAddKey)>(*this,lib,"glfw_chain_add_key",
            SideEffects::worstDefault,"DasGlfw_ChainAddKey");
        addExtern<DAS_BIND_FUN(DasGlfw_ChainAddChar)>(*this,lib,"glfw_chain_add_char",
            SideEffects::worstDefault,"DasGlfw_ChainAddChar");
        addExtern<DAS_BIND_FUN(DasGlfw_PostKey)>(*this,lib,"glfw_post_key",
            SideEffects::worstDefault,"DasGlfw_PostKey");
        addExtern<DAS_BIND_FUN(DasGlfw_PostChar)>(*this,lib,"glfw_post_char",
            SideEffects::worstDefault,"DasGlfw_PostChar");
        addExtern<DAS_BIND_FUN(DAS_glfwGetNativeWindow)>(*this, lib, "glfwGetNativeWindow",SideEffects::worstDefault, "DAS_glfwGetNativeWindow")
            ->args({"window"});
        addExtern<DAS_BIND_FUN(DAS_glfwGetNativeDisplay)>(*this, lib, "glfwGetNativeDisplay",SideEffects::worstDefault, "DAS_glfwGetNativeDisplay");
    }

	ModuleAotType Module_dasGLFW::aotRequire ( TextWriter & tw ) const {
        // add your stuff here
        tw << "#include <GLFW/glfw3.h>\n";
        tw << "#include \"../modules/dasGlfw/src/aot_dasGLFW.h\"\n";
        tw << "#include \"daScript/simulate/bind_enum.h\"\n";
        tw << "#include \"../modules/dasGlfw/src/dasGLFW.enum.decl.cast.inc\"\n";
        return ModuleAotType::cpp;
    }

}
