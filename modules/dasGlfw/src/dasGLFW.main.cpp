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

    void DasGlfw_DestroyWindow ( GLFWwindow * window ) {
        auto it = Module_dasGLFW::g_Callbacks.find(window);
        if ( it!=Module_dasGLFW::g_Callbacks.end() ) Module_dasGLFW::g_Callbacks.erase(it);
        glfwDestroyWindow(window);
    }

    thread_local das_map<void *, GlswCallbacks> Module_dasGLFW::g_Callbacks;

    Module_dasGLFW::~Module_dasGLFW() {
        g_Callbacks.clear();
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
