// this file is generated via Daslang automatic binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasGLFW.h"
#include "need_dasGLFW.h"
namespace das {
#include "dasGLFW.func.aot.decl.inc"
void Module_dasGLFW::initFunctions_3() {
// from GLFW/glfw3.h:3733:14
	makeExtern< void (*)(GLFWwindow *,float *,float *) , glfwGetWindowContentScale , SimNode_ExtFuncCall >(lib,"glfwGetWindowContentScale","glfwGetWindowContentScale")
		->args({"window","xscale","yscale"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3760:15
	makeExtern< float (*)(GLFWwindow *) , glfwGetWindowOpacity , SimNode_ExtFuncCall >(lib,"glfwGetWindowOpacity","glfwGetWindowOpacity")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3792:14
	makeExtern< void (*)(GLFWwindow *,float) , glfwSetWindowOpacity , SimNode_ExtFuncCall >(lib,"glfwSetWindowOpacity","glfwSetWindowOpacity")
		->args({"window","opacity"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3824:14
	makeExtern< void (*)(GLFWwindow *) , glfwIconifyWindow , SimNode_ExtFuncCall >(lib,"glfwIconifyWindow","glfwIconifyWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3851:14
	makeExtern< void (*)(GLFWwindow *) , glfwRestoreWindow , SimNode_ExtFuncCall >(lib,"glfwRestoreWindow","glfwRestoreWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3876:14
	makeExtern< void (*)(GLFWwindow *) , glfwMaximizeWindow , SimNode_ExtFuncCall >(lib,"glfwMaximizeWindow","glfwMaximizeWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3908:14
	makeExtern< void (*)(GLFWwindow *) , glfwShowWindow , SimNode_ExtFuncCall >(lib,"glfwShowWindow","glfwShowWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3930:14
	makeExtern< void (*)(GLFWwindow *) , glfwHideWindow , SimNode_ExtFuncCall >(lib,"glfwHideWindow","glfwHideWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3969:14
	makeExtern< void (*)(GLFWwindow *) , glfwFocusWindow , SimNode_ExtFuncCall >(lib,"glfwFocusWindow","glfwFocusWindow")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:3996:14
	makeExtern< void (*)(GLFWwindow *) , glfwRequestWindowAttention , SimNode_ExtFuncCall >(lib,"glfwRequestWindowAttention","glfwRequestWindowAttention")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4018:22
	makeExtern< GLFWmonitor * (*)(GLFWwindow *) , glfwGetWindowMonitor , SimNode_ExtFuncCall >(lib,"glfwGetWindowMonitor","glfwGetWindowMonitor")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4074:14
	makeExtern< void (*)(GLFWwindow *,GLFWmonitor *,int,int,int,int,int) , glfwSetWindowMonitor , SimNode_ExtFuncCall >(lib,"glfwSetWindowMonitor","glfwSetWindowMonitor")
		->args({"window","monitor","xpos","ypos","width","height","refreshRate"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4111:13
	makeExtern< int (*)(GLFWwindow *,int) , glfwGetWindowAttrib , SimNode_ExtFuncCall >(lib,"glfwGetWindowAttrib","glfwGetWindowAttrib")
		->args({"window","attrib"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4153:14
	makeExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowAttrib , SimNode_ExtFuncCall >(lib,"glfwSetWindowAttrib","glfwSetWindowAttrib")
		->args({"window","attrib","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4176:14
	makeExtern< void (*)(GLFWwindow *,void *) , glfwSetWindowUserPointer , SimNode_ExtFuncCall >(lib,"glfwSetWindowUserPointer","glfwSetWindowUserPointer")
		->args({"window","pointer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4197:15
	makeExtern< void * (*)(GLFWwindow *) , glfwGetWindowUserPointer , SimNode_ExtFuncCall >(lib,"glfwGetWindowUserPointer","glfwGetWindowUserPointer")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4534:14
	makeExtern< void (*)() , glfwPollEvents , SimNode_ExtFuncCall >(lib,"glfwPollEvents","glfwPollEvents")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4579:14
	makeExtern< void (*)() , glfwWaitEvents , SimNode_ExtFuncCall >(lib,"glfwWaitEvents","glfwWaitEvents")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4628:14
	makeExtern< void (*)(double) , glfwWaitEventsTimeout , SimNode_ExtFuncCall >(lib,"glfwWaitEventsTimeout","glfwWaitEventsTimeout")
		->args({"timeout"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4648:14
	makeExtern< void (*)() , glfwPostEmptyEvent , SimNode_ExtFuncCall >(lib,"glfwPostEmptyEvent","glfwPostEmptyEvent")
		->addToModule(*this, SideEffects::worstDefault);
}
}

