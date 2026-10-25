// this file is generated via Daslang automatic binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"
#include "dasGLFW.h"
#include "need_dasGLFW.h"
namespace das {
#include "dasGLFW.func.aot.decl.inc"
void Module_dasGLFW::initFunctions_2() {
// from GLFW/glfw3.h:3005:14
	makeExtern< void (*)(GLFWmonitor *,float) , glfwSetGamma , SimNode_ExtFuncCall >(lib,"glfwSetGamma","glfwSetGamma")
		->args({"monitor","gamma"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3035:30
	makeExtern< const GLFWgammaramp * (*)(GLFWmonitor *) , glfwGetGammaRamp , SimNode_ExtFuncCall >(lib,"glfwGetGammaRamp","glfwGetGammaRamp")
		->args({"monitor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3076:14
	makeExtern< void (*)(GLFWmonitor *,const GLFWgammaramp *) , glfwSetGammaRamp , SimNode_ExtFuncCall >(lib,"glfwSetGammaRamp","glfwSetGammaRamp")
		->args({"monitor","ramp"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3095:14
	makeExtern< void (*)() , glfwDefaultWindowHints , SimNode_ExtFuncCall >(lib,"glfwDefaultWindowHints","glfwDefaultWindowHints")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3130:14
	makeExtern< void (*)(int,int) , glfwWindowHint , SimNode_ExtFuncCall >(lib,"glfwWindowHint","glfwWindowHint")
		->args({"hint","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3168:14
	makeExtern< void (*)(int,const char *) , glfwWindowHintString , SimNode_ExtFuncCall >(lib,"glfwWindowHintString","glfwWindowHintString")
		->args({"hint","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3312:21
	makeExtern< GLFWwindow * (*)(int,int,const char *,GLFWmonitor *,GLFWwindow *) , glfwCreateWindow , SimNode_ExtFuncCall >(lib,"glfwCreateWindow","glfwCreateWindow")
		->args({"width","height","title","monitor","share"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3361:13
	makeExtern< int (*)(GLFWwindow *) , glfwWindowShouldClose , SimNode_ExtFuncCall >(lib,"glfwWindowShouldClose","glfwWindowShouldClose")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3383:14
	makeExtern< void (*)(GLFWwindow *,int) , glfwSetWindowShouldClose , SimNode_ExtFuncCall >(lib,"glfwSetWindowShouldClose","glfwSetWindowShouldClose")
		->args({"window","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3415:21
	makeExtern< const char * (*)(GLFWwindow *) , glfwGetWindowTitle , SimNode_ExtFuncCall >(lib,"glfwGetWindowTitle","glfwGetWindowTitle")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3441:14
	makeExtern< void (*)(GLFWwindow *,const char *) , glfwSetWindowTitle , SimNode_ExtFuncCall >(lib,"glfwSetWindowTitle","glfwSetWindowTitle")
		->args({"window","title"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3490:14
	makeExtern< void (*)(GLFWwindow *,int,const GLFWimage *) , glfwSetWindowIcon , SimNode_ExtFuncCall >(lib,"glfwSetWindowIcon","glfwSetWindowIcon")
		->args({"window","count","images"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3522:14
	makeExtern< void (*)(GLFWwindow *,int *,int *) , glfwGetWindowPos , SimNode_ExtFuncCall >(lib,"glfwGetWindowPos","glfwGetWindowPos")
		->args({"window","xpos","ypos"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3557:14
	makeExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowPos , SimNode_ExtFuncCall >(lib,"glfwSetWindowPos","glfwSetWindowPos")
		->args({"window","xpos","ypos"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3587:14
	makeExtern< void (*)(GLFWwindow *,int *,int *) , glfwGetWindowSize , SimNode_ExtFuncCall >(lib,"glfwGetWindowSize","glfwGetWindowSize")
		->args({"window","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3630:14
	makeExtern< void (*)(GLFWwindow *,int,int,int,int) , glfwSetWindowSizeLimits , SimNode_ExtFuncCall >(lib,"glfwSetWindowSizeLimits","glfwSetWindowSizeLimits")
		->args({"window","minwidth","minheight","maxwidth","maxheight"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3673:14
	makeExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowAspectRatio , SimNode_ExtFuncCall >(lib,"glfwSetWindowAspectRatio","glfwSetWindowAspectRatio")
		->args({"window","numer","denom"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3711:14
	makeExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowSize , SimNode_ExtFuncCall >(lib,"glfwSetWindowSize","glfwSetWindowSize")
		->args({"window","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3740:14
	makeExtern< void (*)(GLFWwindow *,int *,int *) , glfwGetFramebufferSize , SimNode_ExtFuncCall >(lib,"glfwGetFramebufferSize","glfwGetFramebufferSize")
		->args({"window","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3777:14
	makeExtern< void (*)(GLFWwindow *,int *,int *,int *,int *) , glfwGetWindowFrameSize , SimNode_ExtFuncCall >(lib,"glfwGetWindowFrameSize","glfwGetWindowFrameSize")
		->args({"window","left","top","right","bottom"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

