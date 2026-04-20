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
void Module_dasGLFW::initFunctions_4() {
// from GLFW/glfw3.h:4673:13
	makeExtern< int (*)(GLFWwindow *,int) , glfwGetInputMode , SimNode_ExtFuncCall >(lib,"glfwGetInputMode","glfwGetInputMode")
		->args({"window","mode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4738:14
	makeExtern< void (*)(GLFWwindow *,int,int) , glfwSetInputMode , SimNode_ExtFuncCall >(lib,"glfwSetInputMode","glfwSetInputMode")
		->args({"window","mode","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4767:13
	makeExtern< int (*)() , glfwRawMouseMotionSupported , SimNode_ExtFuncCall >(lib,"glfwRawMouseMotionSupported","glfwRawMouseMotionSupported")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4835:21
	makeExtern< const char * (*)(int,int) , glfwGetKeyName , SimNode_ExtFuncCall >(lib,"glfwGetKeyName","glfwGetKeyName")
		->args({"key","scancode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4862:13
	makeExtern< int (*)(int) , glfwGetKeyScancode , SimNode_ExtFuncCall >(lib,"glfwGetKeyScancode","glfwGetKeyScancode")
		->args({"key"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4901:13
	makeExtern< int (*)(GLFWwindow *,int) , glfwGetKey , SimNode_ExtFuncCall >(lib,"glfwGetKey","glfwGetKey")
		->args({"window","key"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4930:13
	makeExtern< int (*)(GLFWwindow *,int) , glfwGetMouseButton , SimNode_ExtFuncCall >(lib,"glfwGetMouseButton","glfwGetMouseButton")
		->args({"window","button"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4968:14
	makeExtern< void (*)(GLFWwindow *,double *,double *) , glfwGetCursorPos , SimNode_ExtFuncCall >(lib,"glfwGetCursorPos","glfwGetCursorPos")
		->args({"window","xpos","ypos"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5008:14
	makeExtern< void (*)(GLFWwindow *,double,double) , glfwSetCursorPos , SimNode_ExtFuncCall >(lib,"glfwSetCursorPos","glfwSetCursorPos")
		->args({"window","xpos","ypos"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5046:21
	makeExtern< GLFWcursor * (*)(const GLFWimage *,int,int) , glfwCreateCursor , SimNode_ExtFuncCall >(lib,"glfwCreateCursor","glfwCreateCursor")
		->args({"image","xhot","yhot"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5094:21
	makeExtern< GLFWcursor * (*)(int) , glfwCreateStandardCursor , SimNode_ExtFuncCall >(lib,"glfwCreateStandardCursor","glfwCreateStandardCursor")
		->args({"shape"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5121:14
	makeExtern< void (*)(GLFWcursor *) , glfwDestroyCursor , SimNode_ExtFuncCall >(lib,"glfwDestroyCursor","glfwDestroyCursor")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5148:14
	makeExtern< void (*)(GLFWwindow *,GLFWcursor *) , glfwSetCursor , SimNode_ExtFuncCall >(lib,"glfwSetCursor","glfwSetCursor")
		->args({"window","cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5476:13
	makeExtern< int (*)(int) , glfwJoystickPresent , SimNode_ExtFuncCall >(lib,"glfwJoystickPresent","glfwJoystickPresent")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5509:22
	makeExtern< const float * (*)(int,int *) , glfwGetJoystickAxes , SimNode_ExtFuncCall >(lib,"glfwGetJoystickAxes","glfwGetJoystickAxes")
		->args({"jid","count"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5550:30
	makeExtern< const unsigned char * (*)(int,int *) , glfwGetJoystickButtons , SimNode_ExtFuncCall >(lib,"glfwGetJoystickButtons","glfwGetJoystickButtons")
		->args({"jid","count"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5607:30
	makeExtern< const unsigned char * (*)(int,int *) , glfwGetJoystickHats , SimNode_ExtFuncCall >(lib,"glfwGetJoystickHats","glfwGetJoystickHats")
		->args({"jid","count"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5638:21
	makeExtern< const char * (*)(int) , glfwGetJoystickName , SimNode_ExtFuncCall >(lib,"glfwGetJoystickName","glfwGetJoystickName")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5679:21
	makeExtern< const char * (*)(int) , glfwGetJoystickGUID , SimNode_ExtFuncCall >(lib,"glfwGetJoystickGUID","glfwGetJoystickGUID")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5705:14
	makeExtern< void (*)(int,void *) , glfwSetJoystickUserPointer , SimNode_ExtFuncCall >(lib,"glfwSetJoystickUserPointer","glfwSetJoystickUserPointer")
		->args({"jid","pointer"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

