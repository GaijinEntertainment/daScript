// this file is generated via daScript automatic C++ binder
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
void Module_dasGLFW::initFunctions_4() {
	addExtern< int (*)(int) , glfwGetKeyScancode >(*this,lib,"glfwGetKeyScancode",SideEffects::worstDefault,"glfwGetKeyScancode")
		->args({"key"});
	addExtern< int (*)(GLFWwindow *,int) , glfwGetKey >(*this,lib,"glfwGetKey",SideEffects::worstDefault,"glfwGetKey")
		->args({"window","key"});
	addExtern< int (*)(GLFWwindow *,int) , glfwGetMouseButton >(*this,lib,"glfwGetMouseButton",SideEffects::worstDefault,"glfwGetMouseButton")
		->args({"window","button"});
	addExtern< void (*)(GLFWwindow *,double *,double *) , glfwGetCursorPos >(*this,lib,"glfwGetCursorPos",SideEffects::worstDefault,"glfwGetCursorPos")
		->args({"window","xpos","ypos"});
	addExtern< void (*)(GLFWwindow *,double,double) , glfwSetCursorPos >(*this,lib,"glfwSetCursorPos",SideEffects::worstDefault,"glfwSetCursorPos")
		->args({"window","xpos","ypos"});
	addExtern< GLFWcursor * (*)(const GLFWimage *,int,int) , glfwCreateCursor >(*this,lib,"glfwCreateCursor",SideEffects::worstDefault,"glfwCreateCursor")
		->args({"image","xhot","yhot"});
	addExtern< GLFWcursor * (*)(int) , glfwCreateStandardCursor >(*this,lib,"glfwCreateStandardCursor",SideEffects::worstDefault,"glfwCreateStandardCursor")
		->args({"shape"});
	addExtern< void (*)(GLFWcursor *) , glfwDestroyCursor >(*this,lib,"glfwDestroyCursor",SideEffects::worstDefault,"glfwDestroyCursor")
		->args({"cursor"});
	addExtern< void (*)(GLFWwindow *,GLFWcursor *) , glfwSetCursor >(*this,lib,"glfwSetCursor",SideEffects::worstDefault,"glfwSetCursor")
		->args({"window","cursor"});
	addExtern< int (*)(int) , glfwJoystickPresent >(*this,lib,"glfwJoystickPresent",SideEffects::worstDefault,"glfwJoystickPresent")
		->args({"jid"});
	addExtern< const float * (*)(int,int *) , glfwGetJoystickAxes >(*this,lib,"glfwGetJoystickAxes",SideEffects::worstDefault,"glfwGetJoystickAxes")
		->args({"jid","count"});
	addExtern< const unsigned char * (*)(int,int *) , glfwGetJoystickButtons >(*this,lib,"glfwGetJoystickButtons",SideEffects::worstDefault,"glfwGetJoystickButtons")
		->args({"jid","count"});
	addExtern< const unsigned char * (*)(int,int *) , glfwGetJoystickHats >(*this,lib,"glfwGetJoystickHats",SideEffects::worstDefault,"glfwGetJoystickHats")
		->args({"jid","count"});
	addExtern< const char * (*)(int) , glfwGetJoystickName >(*this,lib,"glfwGetJoystickName",SideEffects::worstDefault,"glfwGetJoystickName")
		->args({"jid"});
	addExtern< const char * (*)(int) , glfwGetJoystickGUID >(*this,lib,"glfwGetJoystickGUID",SideEffects::worstDefault,"glfwGetJoystickGUID")
		->args({"jid"});
	addExtern< void (*)(int,void *) , glfwSetJoystickUserPointer >(*this,lib,"glfwSetJoystickUserPointer",SideEffects::worstDefault,"glfwSetJoystickUserPointer")
		->args({"jid","pointer"});
	addExtern< void * (*)(int) , glfwGetJoystickUserPointer >(*this,lib,"glfwGetJoystickUserPointer",SideEffects::worstDefault,"glfwGetJoystickUserPointer")
		->args({"jid"});
	addExtern< int (*)(int) , glfwJoystickIsGamepad >(*this,lib,"glfwJoystickIsGamepad",SideEffects::worstDefault,"glfwJoystickIsGamepad")
		->args({"jid"});
	addExtern< int (*)(const char *) , glfwUpdateGamepadMappings >(*this,lib,"glfwUpdateGamepadMappings",SideEffects::worstDefault,"glfwUpdateGamepadMappings")
		->args({"string"});
	addExtern< const char * (*)(int) , glfwGetGamepadName >(*this,lib,"glfwGetGamepadName",SideEffects::worstDefault,"glfwGetGamepadName")
		->args({"jid"});
}
}

