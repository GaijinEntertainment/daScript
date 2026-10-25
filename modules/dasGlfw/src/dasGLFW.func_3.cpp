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
void Module_dasGLFW::initFunctions_3() {
// from GLFW/glfw3.h:3810:14
	makeExtern< void (*)(GLFWwindow *,float *,float *) , glfwGetWindowContentScale , SimNode_ExtFuncCall >(lib,"glfwGetWindowContentScale","glfwGetWindowContentScale")
		->args({"window","xscale","yscale"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3837:15
	makeExtern< float (*)(GLFWwindow *) , glfwGetWindowOpacity , SimNode_ExtFuncCall >(lib,"glfwGetWindowOpacity","glfwGetWindowOpacity")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3869:14
	makeExtern< void (*)(GLFWwindow *,float) , glfwSetWindowOpacity , SimNode_ExtFuncCall >(lib,"glfwSetWindowOpacity","glfwSetWindowOpacity")
		->args({"window","opacity"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3897:14
	makeExtern< void (*)(GLFWwindow *) , glfwIconifyWindow , SimNode_ExtFuncCall >(lib,"glfwIconifyWindow","glfwIconifyWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3928:14
	makeExtern< void (*)(GLFWwindow *) , glfwRestoreWindow , SimNode_ExtFuncCall >(lib,"glfwRestoreWindow","glfwRestoreWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3953:14
	makeExtern< void (*)(GLFWwindow *) , glfwMaximizeWindow , SimNode_ExtFuncCall >(lib,"glfwMaximizeWindow","glfwMaximizeWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3985:14
	makeExtern< void (*)(GLFWwindow *) , glfwShowWindow , SimNode_ExtFuncCall >(lib,"glfwShowWindow","glfwShowWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4007:14
	makeExtern< void (*)(GLFWwindow *) , glfwHideWindow , SimNode_ExtFuncCall >(lib,"glfwHideWindow","glfwHideWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4046:14
	makeExtern< void (*)(GLFWwindow *) , glfwFocusWindow , SimNode_ExtFuncCall >(lib,"glfwFocusWindow","glfwFocusWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4073:14
	makeExtern< void (*)(GLFWwindow *) , glfwRequestWindowAttention , SimNode_ExtFuncCall >(lib,"glfwRequestWindowAttention","glfwRequestWindowAttention")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4095:22
	makeExtern< GLFWmonitor * (*)(GLFWwindow *) , glfwGetWindowMonitor , SimNode_ExtFuncCall >(lib,"glfwGetWindowMonitor","glfwGetWindowMonitor")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4151:14
	makeExtern< void (*)(GLFWwindow *,GLFWmonitor *,int,int,int,int,int) , glfwSetWindowMonitor , SimNode_ExtFuncCall >(lib,"glfwSetWindowMonitor","glfwSetWindowMonitor")
		->args({"window","monitor","xpos","ypos","width","height","refreshRate"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4189:13
	makeExtern< int (*)(GLFWwindow *,int) , glfwGetWindowAttrib , SimNode_ExtFuncCall >(lib,"glfwGetWindowAttrib","glfwGetWindowAttrib")
		->args({"window","attrib"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4231:14
	makeExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowAttrib , SimNode_ExtFuncCall >(lib,"glfwSetWindowAttrib","glfwSetWindowAttrib")
		->args({"window","attrib","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4254:14
	makeExtern< void (*)(GLFWwindow *,void *) , glfwSetWindowUserPointer , SimNode_ExtFuncCall >(lib,"glfwSetWindowUserPointer","glfwSetWindowUserPointer")
		->args({"window","pointer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4275:15
	makeExtern< void * (*)(GLFWwindow *) , glfwGetWindowUserPointer , SimNode_ExtFuncCall >(lib,"glfwGetWindowUserPointer","glfwGetWindowUserPointer")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4663:14
	makeExtern< void (*)() , glfwWaitEvents , SimNode_ExtFuncCall >(lib,"glfwWaitEvents","glfwWaitEvents")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4713:14
	makeExtern< void (*)(double) , glfwWaitEventsTimeout , SimNode_ExtFuncCall >(lib,"glfwWaitEventsTimeout","glfwWaitEventsTimeout")
		->args({"timeout"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4733:14
	makeExtern< void (*)() , glfwPostEmptyEvent , SimNode_ExtFuncCall >(lib,"glfwPostEmptyEvent","glfwPostEmptyEvent")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4758:13
	makeExtern< int (*)(GLFWwindow *,int) , glfwGetInputMode , SimNode_ExtFuncCall >(lib,"glfwGetInputMode","glfwGetInputMode")
		->args({"window","mode"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

