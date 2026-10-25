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
void Module_dasGLFW::initFunctions_5() {
// from GLFW/glfw3.h:5989:21
	makeExtern< const char * (*)(int) , glfwGetJoystickName , SimNode_ExtFuncCall >(lib,"glfwGetJoystickName","glfwGetJoystickName")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6030:21
	makeExtern< const char * (*)(int) , glfwGetJoystickGUID , SimNode_ExtFuncCall >(lib,"glfwGetJoystickGUID","glfwGetJoystickGUID")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6056:14
	makeExtern< void (*)(int,void *) , glfwSetJoystickUserPointer , SimNode_ExtFuncCall >(lib,"glfwSetJoystickUserPointer","glfwSetJoystickUserPointer")
		->args({"jid","pointer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6080:15
	makeExtern< void * (*)(int) , glfwGetJoystickUserPointer , SimNode_ExtFuncCall >(lib,"glfwGetJoystickUserPointer","glfwGetJoystickUserPointer")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6108:13
	makeExtern< int (*)(int) , glfwJoystickIsGamepad , SimNode_ExtFuncCall >(lib,"glfwJoystickIsGamepad","glfwJoystickIsGamepad")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6178:13
	makeExtern< int (*)(const char *) , glfwUpdateGamepadMappings , SimNode_ExtFuncCall >(lib,"glfwUpdateGamepadMappings","glfwUpdateGamepadMappings")
		->args({"string"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6210:21
	makeExtern< const char * (*)(int) , glfwGetGamepadName , SimNode_ExtFuncCall >(lib,"glfwGetGamepadName","glfwGetGamepadName")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6248:13
	makeExtern< int (*)(int,GLFWgamepadstate *) , glfwGetGamepadState , SimNode_ExtFuncCall >(lib,"glfwGetGamepadState","glfwGetGamepadState")
		->args({"jid","state"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6278:14
	makeExtern< void (*)(GLFWwindow *,const char *) , glfwSetClipboardString , SimNode_ExtFuncCall >(lib,"glfwSetClipboardString","glfwSetClipboardString")
		->args({"window","string"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6313:21
	makeExtern< const char * (*)(GLFWwindow *) , glfwGetClipboardString , SimNode_ExtFuncCall >(lib,"glfwGetClipboardString","glfwGetClipboardString")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6343:16
	makeExtern< double (*)() , glfwGetTime , SimNode_ExtFuncCall >(lib,"glfwGetTime","glfwGetTime")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6373:14
	makeExtern< void (*)(double) , glfwSetTime , SimNode_ExtFuncCall >(lib,"glfwSetTime","glfwSetTime")
		->args({"time"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6395:18
	makeExtern< uint64_t (*)() , glfwGetTimerValue , SimNode_ExtFuncCall >(lib,"glfwGetTimerValue","glfwGetTimerValue")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6415:18
	makeExtern< uint64_t (*)() , glfwGetTimerFrequency , SimNode_ExtFuncCall >(lib,"glfwGetTimerFrequency","glfwGetTimerFrequency")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6460:14
	makeExtern< void (*)(GLFWwindow *) , glfwMakeContextCurrent , SimNode_ExtFuncCall >(lib,"glfwMakeContextCurrent","glfwMakeContextCurrent")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6481:21
	makeExtern< GLFWwindow * (*)() , glfwGetCurrentContext , SimNode_ExtFuncCall >(lib,"glfwGetCurrentContext","glfwGetCurrentContext")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6519:14
	makeExtern< void (*)(GLFWwindow *) , glfwSwapBuffers , SimNode_ExtFuncCall >(lib,"glfwSwapBuffers","glfwSwapBuffers")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6565:14
	makeExtern< void (*)(int) , glfwSwapInterval , SimNode_ExtFuncCall >(lib,"glfwSwapInterval","glfwSwapInterval")
		->args({"interval"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6603:13
	makeExtern< int (*)(const char *) , glfwExtensionSupported , SimNode_ExtFuncCall >(lib,"glfwExtensionSupported","glfwExtensionSupported")
		->args({"extension"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:6671:13
	makeExtern< int (*)() , glfwVulkanSupported , SimNode_ExtFuncCall >(lib,"glfwVulkanSupported","glfwVulkanSupported")
		->addToModule(*this, SideEffects::worstDefault);
}
}

