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
void Module_dasGLFW::initFunctions_4() {
// from GLFW/glfw3.h:4832:14
	makeExtern< void (*)(GLFWwindow *,int,int) , glfwSetInputMode , SimNode_ExtFuncCall >(lib,"glfwSetInputMode","glfwSetInputMode")
		->args({"window","mode","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4861:13
	makeExtern< int (*)() , glfwRawMouseMotionSupported , SimNode_ExtFuncCall >(lib,"glfwRawMouseMotionSupported","glfwRawMouseMotionSupported")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4929:21
	makeExtern< const char * (*)(int,int) , glfwGetKeyName , SimNode_ExtFuncCall >(lib,"glfwGetKeyName","glfwGetKeyName")
		->args({"key","scancode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:4956:13
	makeExtern< int (*)(int) , glfwGetKeyScancode , SimNode_ExtFuncCall >(lib,"glfwGetKeyScancode","glfwGetKeyScancode")
		->args({"key"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5027:13
	makeExtern< int (*)(GLFWwindow *,int) , glfwGetMouseButton , SimNode_ExtFuncCall >(lib,"glfwGetMouseButton","glfwGetMouseButton")
		->args({"window","button"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5065:14
	makeExtern< void (*)(GLFWwindow *,double *,double *) , glfwGetCursorPos , SimNode_ExtFuncCall >(lib,"glfwGetCursorPos","glfwGetCursorPos")
		->args({"window","xpos","ypos"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5105:14
	makeExtern< void (*)(GLFWwindow *,double,double) , glfwSetCursorPos , SimNode_ExtFuncCall >(lib,"glfwSetCursorPos","glfwSetCursorPos")
		->args({"window","xpos","ypos"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5143:21
	makeExtern< GLFWcursor * (*)(const GLFWimage *,int,int) , glfwCreateCursor , SimNode_ExtFuncCall >(lib,"glfwCreateCursor","glfwCreateCursor")
		->args({"image","xhot","yhot"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5191:21
	makeExtern< GLFWcursor * (*)(int) , glfwCreateStandardCursor , SimNode_ExtFuncCall >(lib,"glfwCreateStandardCursor","glfwCreateStandardCursor")
		->args({"shape"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5218:14
	makeExtern< void (*)(GLFWcursor *) , glfwDestroyCursor , SimNode_ExtFuncCall >(lib,"glfwDestroyCursor","glfwDestroyCursor")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5245:14
	makeExtern< void (*)(GLFWwindow *,GLFWcursor *) , glfwSetCursor , SimNode_ExtFuncCall >(lib,"glfwSetCursor","glfwSetCursor")
		->args({"window","cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5267:14
	makeExtern< void (*)(GLFWwindow *,int *,int *,int *,int *) , glfwGetPreeditCursorRectangle , SimNode_ExtFuncCall >(lib,"glfwGetPreeditCursorRectangle","glfwGetPreeditCursorRectangle")
		->args({"window","x","y","w","h"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5289:14
	makeExtern< void (*)(GLFWwindow *,int,int,int,int) , glfwSetPreeditCursorRectangle , SimNode_ExtFuncCall >(lib,"glfwSetPreeditCursorRectangle","glfwSetPreeditCursorRectangle")
		->args({"window","x","y","w","h"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5311:14
	makeExtern< void (*)(GLFWwindow *) , glfwResetPreeditText , SimNode_ExtFuncCall >(lib,"glfwResetPreeditText","glfwResetPreeditText")
		->args({"window"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5348:14
	makeExtern< void (*)(GLFWwindow *,int) , glfwSetTextInputFocus , SimNode_ExtFuncCall >(lib,"glfwSetTextInputFocus","glfwSetTextInputFocus")
		->args({"window","focused"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5376:23
	makeExtern< unsigned int * (*)(GLFWwindow *,int,int *) , glfwGetPreeditCandidate , SimNode_ExtFuncCall >(lib,"glfwGetPreeditCandidate","glfwGetPreeditCandidate")
		->args({"window","index","textCount"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5827:13
	makeExtern< int (*)(int) , glfwJoystickPresent , SimNode_ExtFuncCall >(lib,"glfwJoystickPresent","glfwJoystickPresent")
		->args({"jid"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5860:22
	makeExtern< const float * (*)(int,int *) , glfwGetJoystickAxes , SimNode_ExtFuncCall >(lib,"glfwGetJoystickAxes","glfwGetJoystickAxes")
		->args({"jid","count"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5901:30
	makeExtern< const unsigned char * (*)(int,int *) , glfwGetJoystickButtons , SimNode_ExtFuncCall >(lib,"glfwGetJoystickButtons","glfwGetJoystickButtons")
		->args({"jid","count"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:5958:30
	makeExtern< const unsigned char * (*)(int,int *) , glfwGetJoystickHats , SimNode_ExtFuncCall >(lib,"glfwGetJoystickHats","glfwGetJoystickHats")
		->args({"jid","count"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

