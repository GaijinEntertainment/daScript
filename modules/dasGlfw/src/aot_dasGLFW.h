#pragma once

namespace das {
    DAS_MOD_API void DasGlfw_KeyFunction ( GLFWwindow* window, int keyCode, int scanCode, int action, int modes );
    DAS_MOD_API void DasGlfw_CharFunction ( GLFWwindow* window, unsigned int cp );
    DAS_MOD_API void DasGlfw_CursorPosFunction ( GLFWwindow* window, double x, double y );
    DAS_MOD_API void DasGlfw_CursorEnterFunction ( GLFWwindow* window, int enter );
    DAS_MOD_API void DasGlfw_MouseFunction ( GLFWwindow* window, int buttons, int action, int modes );
    DAS_MOD_API void DasGlfw_ScrollFunction ( GLFWwindow* window, double x, double y );
    DAS_MOD_API void DasGlfw_SetKeyCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,int,int,int,int> func, Context * ctx );
    DAS_MOD_API void DasGlfw_SetCharCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,unsigned int> func, Context * ctx );
    DAS_MOD_API void DasGlfw_SetCursorPosCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,double,double> func, Context * ctx );
    DAS_MOD_API void DasGlfw_SetCursorEnterCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,bool> func, Context * ctx );
    DAS_MOD_API void DasGlfw_SetMouseButtonCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,int,int,int> func, Context * ctx );
    DAS_MOD_API void DasGlfw_SetScrollCallback ( GLFWwindow * window, TLambda<void,const GLFWwindow*,double,double> func, Context * ctx );
    DAS_MOD_API void DasGlfw_Shutdown();
    DAS_MOD_API void DasGlfw_DestroyWindow ( GLFWwindow * window );
    DAS_MOD_API void * DAS_glfwGetNativeWindow ( GLFWwindow* window );
}
