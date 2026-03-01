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
void Module_dasGLFW::initFunctions_1() {
	addExtern< int (*)() , glfwInit >(*this,lib,"glfwInit",SideEffects::worstDefault,"glfwInit");
	addExtern< void (*)() , glfwTerminate >(*this,lib,"glfwTerminate",SideEffects::worstDefault,"glfwTerminate");
	addExtern< void (*)(int,int) , glfwInitHint >(*this,lib,"glfwInitHint",SideEffects::worstDefault,"glfwInitHint")
		->args({"hint","value"});
	addExtern< void (*)(int *,int *,int *) , glfwGetVersion >(*this,lib,"glfwGetVersion",SideEffects::worstDefault,"glfwGetVersion")
		->args({"major","minor","rev"});
	addExtern< const char * (*)() , glfwGetVersionString >(*this,lib,"glfwGetVersionString",SideEffects::worstDefault,"glfwGetVersionString");
	addExtern< int (*)(const char **) , glfwGetError >(*this,lib,"glfwGetError",SideEffects::worstDefault,"glfwGetError")
		->args({"description"});
	addExtern< GLFWmonitor ** (*)(int *) , glfwGetMonitors >(*this,lib,"glfwGetMonitors",SideEffects::worstDefault,"glfwGetMonitors")
		->args({"count"});
	addExtern< GLFWmonitor * (*)() , glfwGetPrimaryMonitor >(*this,lib,"glfwGetPrimaryMonitor",SideEffects::worstDefault,"glfwGetPrimaryMonitor");
	addExtern< void (*)(GLFWmonitor *,int *,int *) , glfwGetMonitorPos >(*this,lib,"glfwGetMonitorPos",SideEffects::worstDefault,"glfwGetMonitorPos")
		->args({"monitor","xpos","ypos"});
	addExtern< void (*)(GLFWmonitor *,int *,int *,int *,int *) , glfwGetMonitorWorkarea >(*this,lib,"glfwGetMonitorWorkarea",SideEffects::worstDefault,"glfwGetMonitorWorkarea")
		->args({"monitor","xpos","ypos","width","height"});
	addExtern< void (*)(GLFWmonitor *,int *,int *) , glfwGetMonitorPhysicalSize >(*this,lib,"glfwGetMonitorPhysicalSize",SideEffects::worstDefault,"glfwGetMonitorPhysicalSize")
		->args({"monitor","widthMM","heightMM"});
	addExtern< void (*)(GLFWmonitor *,float *,float *) , glfwGetMonitorContentScale >(*this,lib,"glfwGetMonitorContentScale",SideEffects::worstDefault,"glfwGetMonitorContentScale")
		->args({"monitor","xscale","yscale"});
	addExtern< const char * (*)(GLFWmonitor *) , glfwGetMonitorName >(*this,lib,"glfwGetMonitorName",SideEffects::worstDefault,"glfwGetMonitorName")
		->args({"monitor"});
	addExtern< void (*)(GLFWmonitor *,void *) , glfwSetMonitorUserPointer >(*this,lib,"glfwSetMonitorUserPointer",SideEffects::worstDefault,"glfwSetMonitorUserPointer")
		->args({"monitor","pointer"});
	addExtern< void * (*)(GLFWmonitor *) , glfwGetMonitorUserPointer >(*this,lib,"glfwGetMonitorUserPointer",SideEffects::worstDefault,"glfwGetMonitorUserPointer")
		->args({"monitor"});
	addExtern< const GLFWvidmode * (*)(GLFWmonitor *,int *) , glfwGetVideoModes >(*this,lib,"glfwGetVideoModes",SideEffects::worstDefault,"glfwGetVideoModes")
		->args({"monitor","count"});
	addExtern< const GLFWvidmode * (*)(GLFWmonitor *) , glfwGetVideoMode >(*this,lib,"glfwGetVideoMode",SideEffects::worstDefault,"glfwGetVideoMode")
		->args({"monitor"});
	addExtern< void (*)(GLFWmonitor *,float) , glfwSetGamma >(*this,lib,"glfwSetGamma",SideEffects::worstDefault,"glfwSetGamma")
		->args({"monitor","gamma"});
	addExtern< const GLFWgammaramp * (*)(GLFWmonitor *) , glfwGetGammaRamp >(*this,lib,"glfwGetGammaRamp",SideEffects::worstDefault,"glfwGetGammaRamp")
		->args({"monitor"});
	addExtern< void (*)(GLFWmonitor *,const GLFWgammaramp *) , glfwSetGammaRamp >(*this,lib,"glfwSetGammaRamp",SideEffects::worstDefault,"glfwSetGammaRamp")
		->args({"monitor","ramp"});
}
}

