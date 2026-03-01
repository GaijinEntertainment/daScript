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
void Module_dasGLFW::initFunctions_5() {
	addExtern< int (*)(int,GLFWgamepadstate *) , glfwGetGamepadState >(*this,lib,"glfwGetGamepadState",SideEffects::worstDefault,"glfwGetGamepadState")
		->args({"jid","state"});
	addExtern< void (*)(GLFWwindow *,const char *) , glfwSetClipboardString >(*this,lib,"glfwSetClipboardString",SideEffects::worstDefault,"glfwSetClipboardString")
		->args({"window","string"});
	addExtern< const char * (*)(GLFWwindow *) , glfwGetClipboardString >(*this,lib,"glfwGetClipboardString",SideEffects::worstDefault,"glfwGetClipboardString")
		->args({"window"});
	addExtern< double (*)() , glfwGetTime >(*this,lib,"glfwGetTime",SideEffects::worstDefault,"glfwGetTime");
	addExtern< void (*)(double) , glfwSetTime >(*this,lib,"glfwSetTime",SideEffects::worstDefault,"glfwSetTime")
		->args({"time"});
	addExtern< uint64_t (*)() , glfwGetTimerValue >(*this,lib,"glfwGetTimerValue",SideEffects::worstDefault,"glfwGetTimerValue");
	addExtern< uint64_t (*)() , glfwGetTimerFrequency >(*this,lib,"glfwGetTimerFrequency",SideEffects::worstDefault,"glfwGetTimerFrequency");
	addExtern< void (*)(GLFWwindow *) , glfwMakeContextCurrent >(*this,lib,"glfwMakeContextCurrent",SideEffects::worstDefault,"glfwMakeContextCurrent")
		->args({"window"});
	addExtern< GLFWwindow * (*)() , glfwGetCurrentContext >(*this,lib,"glfwGetCurrentContext",SideEffects::worstDefault,"glfwGetCurrentContext");
	addExtern< void (*)(GLFWwindow *) , glfwSwapBuffers >(*this,lib,"glfwSwapBuffers",SideEffects::worstDefault,"glfwSwapBuffers")
		->args({"window"});
	addExtern< void (*)(int) , glfwSwapInterval >(*this,lib,"glfwSwapInterval",SideEffects::worstDefault,"glfwSwapInterval")
		->args({"interval"});
	addExtern< int (*)(const char *) , glfwExtensionSupported >(*this,lib,"glfwExtensionSupported",SideEffects::worstDefault,"glfwExtensionSupported")
		->args({"extension"});
	addExtern< int (*)() , glfwVulkanSupported >(*this,lib,"glfwVulkanSupported",SideEffects::worstDefault,"glfwVulkanSupported");
	addExtern< const char ** (*)(unsigned int *) , glfwGetRequiredInstanceExtensions >(*this,lib,"glfwGetRequiredInstanceExtensions",SideEffects::worstDefault,"glfwGetRequiredInstanceExtensions")
		->args({"count"});
}
}

