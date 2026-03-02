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
void Module_dasGLFW::initFunctions_3() {
	addExtern< void (*)(GLFWwindow *) , glfwRestoreWindow >(*this,lib,"glfwRestoreWindow",SideEffects::worstDefault,"glfwRestoreWindow")
		->args({"window"});
	addExtern< void (*)(GLFWwindow *) , glfwMaximizeWindow >(*this,lib,"glfwMaximizeWindow",SideEffects::worstDefault,"glfwMaximizeWindow")
		->args({"window"});
	addExtern< void (*)(GLFWwindow *) , glfwShowWindow >(*this,lib,"glfwShowWindow",SideEffects::worstDefault,"glfwShowWindow")
		->args({"window"});
	addExtern< void (*)(GLFWwindow *) , glfwHideWindow >(*this,lib,"glfwHideWindow",SideEffects::worstDefault,"glfwHideWindow")
		->args({"window"});
	addExtern< void (*)(GLFWwindow *) , glfwFocusWindow >(*this,lib,"glfwFocusWindow",SideEffects::worstDefault,"glfwFocusWindow")
		->args({"window"});
	addExtern< void (*)(GLFWwindow *) , glfwRequestWindowAttention >(*this,lib,"glfwRequestWindowAttention",SideEffects::worstDefault,"glfwRequestWindowAttention")
		->args({"window"});
	addExtern< GLFWmonitor * (*)(GLFWwindow *) , glfwGetWindowMonitor >(*this,lib,"glfwGetWindowMonitor",SideEffects::worstDefault,"glfwGetWindowMonitor")
		->args({"window"});
	addExtern< void (*)(GLFWwindow *,GLFWmonitor *,int,int,int,int,int) , glfwSetWindowMonitor >(*this,lib,"glfwSetWindowMonitor",SideEffects::worstDefault,"glfwSetWindowMonitor")
		->args({"window","monitor","xpos","ypos","width","height","refreshRate"});
	addExtern< int (*)(GLFWwindow *,int) , glfwGetWindowAttrib >(*this,lib,"glfwGetWindowAttrib",SideEffects::worstDefault,"glfwGetWindowAttrib")
		->args({"window","attrib"});
	addExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowAttrib >(*this,lib,"glfwSetWindowAttrib",SideEffects::worstDefault,"glfwSetWindowAttrib")
		->args({"window","attrib","value"});
	addExtern< void (*)(GLFWwindow *,void *) , glfwSetWindowUserPointer >(*this,lib,"glfwSetWindowUserPointer",SideEffects::worstDefault,"glfwSetWindowUserPointer")
		->args({"window","pointer"});
	addExtern< void * (*)(GLFWwindow *) , glfwGetWindowUserPointer >(*this,lib,"glfwGetWindowUserPointer",SideEffects::worstDefault,"glfwGetWindowUserPointer")
		->args({"window"});
	addExtern< void (*)() , glfwPollEvents >(*this,lib,"glfwPollEvents",SideEffects::worstDefault,"glfwPollEvents");
	addExtern< void (*)() , glfwWaitEvents >(*this,lib,"glfwWaitEvents",SideEffects::worstDefault,"glfwWaitEvents");
	addExtern< void (*)(double) , glfwWaitEventsTimeout >(*this,lib,"glfwWaitEventsTimeout",SideEffects::worstDefault,"glfwWaitEventsTimeout")
		->args({"timeout"});
	addExtern< void (*)() , glfwPostEmptyEvent >(*this,lib,"glfwPostEmptyEvent",SideEffects::worstDefault,"glfwPostEmptyEvent");
	addExtern< int (*)(GLFWwindow *,int) , glfwGetInputMode >(*this,lib,"glfwGetInputMode",SideEffects::worstDefault,"glfwGetInputMode")
		->args({"window","mode"});
	addExtern< void (*)(GLFWwindow *,int,int) , glfwSetInputMode >(*this,lib,"glfwSetInputMode",SideEffects::worstDefault,"glfwSetInputMode")
		->args({"window","mode","value"});
	addExtern< int (*)() , glfwRawMouseMotionSupported >(*this,lib,"glfwRawMouseMotionSupported",SideEffects::worstDefault,"glfwRawMouseMotionSupported");
	addExtern< const char * (*)(int,int) , glfwGetKeyName >(*this,lib,"glfwGetKeyName",SideEffects::worstDefault,"glfwGetKeyName")
		->args({"key","scancode"});
}
}

