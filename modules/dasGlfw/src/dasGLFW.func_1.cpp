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
void Module_dasGLFW::initFunctions_1() {
// from GLFW/glfw3.h:2220:13
	makeExtern< int (*)() , glfwInit , SimNode_ExtFuncCall >(lib,"glfwInit","glfwInit")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2254:14
	makeExtern< void (*)() , glfwTerminate , SimNode_ExtFuncCall >(lib,"glfwTerminate","glfwTerminate")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2286:14
	makeExtern< void (*)(int,int) , glfwInitHint , SimNode_ExtFuncCall >(lib,"glfwInitHint","glfwInitHint")
		->args({"hint","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2317:14
	makeExtern< void (*)(const GLFWallocator *) , glfwInitAllocator , SimNode_ExtFuncCall >(lib,"glfwInitAllocator","glfwInitAllocator")
		->args({"allocator"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2392:14
	makeExtern< void (*)(int *,int *,int *) , glfwGetVersion , SimNode_ExtFuncCall >(lib,"glfwGetVersion","glfwGetVersion")
		->args({"major","minor","rev"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2426:21
	makeExtern< const char * (*)() , glfwGetVersionString , SimNode_ExtFuncCall >(lib,"glfwGetVersionString","glfwGetVersionString")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2457:13
	makeExtern< int (*)(const char **) , glfwGetError , SimNode_ExtFuncCall >(lib,"glfwGetError","glfwGetError")
		->args({"description"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2524:13
	makeExtern< int (*)() , glfwGetPlatform , SimNode_ExtFuncCall >(lib,"glfwGetPlatform","glfwGetPlatform")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2548:13
	makeExtern< int (*)(int) , glfwPlatformSupported , SimNode_ExtFuncCall >(lib,"glfwPlatformSupported","glfwPlatformSupported")
		->args({"platform"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2577:23
	makeExtern< GLFWmonitor ** (*)(int *) , glfwGetMonitors , SimNode_ExtFuncCall >(lib,"glfwGetMonitors","glfwGetMonitors")
		->args({"count"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2601:22
	makeExtern< GLFWmonitor * (*)() , glfwGetPrimaryMonitor , SimNode_ExtFuncCall >(lib,"glfwGetPrimaryMonitor","glfwGetPrimaryMonitor")
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2626:14
	makeExtern< void (*)(GLFWmonitor *,int *,int *) , glfwGetMonitorPos , SimNode_ExtFuncCall >(lib,"glfwGetMonitorPos","glfwGetMonitorPos")
		->args({"monitor","xpos","ypos"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2657:14
	makeExtern< void (*)(GLFWmonitor *,int *,int *,int *,int *) , glfwGetMonitorWorkarea , SimNode_ExtFuncCall >(lib,"glfwGetMonitorWorkarea","glfwGetMonitorWorkarea")
		->args({"monitor","xpos","ypos","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2692:14
	makeExtern< void (*)(GLFWmonitor *,int *,int *) , glfwGetMonitorPhysicalSize , SimNode_ExtFuncCall >(lib,"glfwGetMonitorPhysicalSize","glfwGetMonitorPhysicalSize")
		->args({"monitor","widthMM","heightMM"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2727:14
	makeExtern< void (*)(GLFWmonitor *,float *,float *) , glfwGetMonitorContentScale , SimNode_ExtFuncCall >(lib,"glfwGetMonitorContentScale","glfwGetMonitorContentScale")
		->args({"monitor","xscale","yscale"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2753:21
	makeExtern< const char * (*)(GLFWmonitor *) , glfwGetMonitorName , SimNode_ExtFuncCall >(lib,"glfwGetMonitorName","glfwGetMonitorName")
		->args({"monitor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2779:14
	makeExtern< void (*)(GLFWmonitor *,void *) , glfwSetMonitorUserPointer , SimNode_ExtFuncCall >(lib,"glfwSetMonitorUserPointer","glfwSetMonitorUserPointer")
		->args({"monitor","pointer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2803:15
	makeExtern< void * (*)(GLFWmonitor *) , glfwGetMonitorUserPointer , SimNode_ExtFuncCall >(lib,"glfwGetMonitorUserPointer","glfwGetMonitorUserPointer")
		->args({"monitor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2867:28
	makeExtern< const GLFWvidmode * (*)(GLFWmonitor *,int *) , glfwGetVideoModes , SimNode_ExtFuncCall >(lib,"glfwGetVideoModes","glfwGetVideoModes")
		->args({"monitor","count"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLFW/glfw3.h:2895:28
	makeExtern< const GLFWvidmode * (*)(GLFWmonitor *) , glfwGetVideoMode , SimNode_ExtFuncCall >(lib,"glfwGetVideoMode","glfwGetVideoMode")
		->args({"monitor"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

