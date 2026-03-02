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
void Module_dasGLFW::initFunctions_2() {
	addExtern< void (*)() , glfwDefaultWindowHints >(*this,lib,"glfwDefaultWindowHints",SideEffects::worstDefault,"glfwDefaultWindowHints");
	addExtern< void (*)(int,int) , glfwWindowHint >(*this,lib,"glfwWindowHint",SideEffects::worstDefault,"glfwWindowHint")
		->args({"hint","value"});
	addExtern< void (*)(int,const char *) , glfwWindowHintString >(*this,lib,"glfwWindowHintString",SideEffects::worstDefault,"glfwWindowHintString")
		->args({"hint","value"});
	addExtern< GLFWwindow * (*)(int,int,const char *,GLFWmonitor *,GLFWwindow *) , glfwCreateWindow >(*this,lib,"glfwCreateWindow",SideEffects::worstDefault,"glfwCreateWindow")
		->args({"width","height","title","monitor","share"});
	addExtern< int (*)(GLFWwindow *) , glfwWindowShouldClose >(*this,lib,"glfwWindowShouldClose",SideEffects::worstDefault,"glfwWindowShouldClose")
		->args({"window"});
	addExtern< void (*)(GLFWwindow *,int) , glfwSetWindowShouldClose >(*this,lib,"glfwSetWindowShouldClose",SideEffects::worstDefault,"glfwSetWindowShouldClose")
		->args({"window","value"});
	addExtern< void (*)(GLFWwindow *,const char *) , glfwSetWindowTitle >(*this,lib,"glfwSetWindowTitle",SideEffects::worstDefault,"glfwSetWindowTitle")
		->args({"window","title"});
	addExtern< void (*)(GLFWwindow *,int,const GLFWimage *) , glfwSetWindowIcon >(*this,lib,"glfwSetWindowIcon",SideEffects::worstDefault,"glfwSetWindowIcon")
		->args({"window","count","images"});
	addExtern< void (*)(GLFWwindow *,int *,int *) , glfwGetWindowPos >(*this,lib,"glfwGetWindowPos",SideEffects::worstDefault,"glfwGetWindowPos")
		->args({"window","xpos","ypos"});
	addExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowPos >(*this,lib,"glfwSetWindowPos",SideEffects::worstDefault,"glfwSetWindowPos")
		->args({"window","xpos","ypos"});
	addExtern< void (*)(GLFWwindow *,int *,int *) , glfwGetWindowSize >(*this,lib,"glfwGetWindowSize",SideEffects::worstDefault,"glfwGetWindowSize")
		->args({"window","width","height"});
	addExtern< void (*)(GLFWwindow *,int,int,int,int) , glfwSetWindowSizeLimits >(*this,lib,"glfwSetWindowSizeLimits",SideEffects::worstDefault,"glfwSetWindowSizeLimits")
		->args({"window","minwidth","minheight","maxwidth","maxheight"});
	addExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowAspectRatio >(*this,lib,"glfwSetWindowAspectRatio",SideEffects::worstDefault,"glfwSetWindowAspectRatio")
		->args({"window","numer","denom"});
	addExtern< void (*)(GLFWwindow *,int,int) , glfwSetWindowSize >(*this,lib,"glfwSetWindowSize",SideEffects::worstDefault,"glfwSetWindowSize")
		->args({"window","width","height"});
	addExtern< void (*)(GLFWwindow *,int *,int *) , glfwGetFramebufferSize >(*this,lib,"glfwGetFramebufferSize",SideEffects::worstDefault,"glfwGetFramebufferSize")
		->args({"window","width","height"});
	addExtern< void (*)(GLFWwindow *,int *,int *,int *,int *) , glfwGetWindowFrameSize >(*this,lib,"glfwGetWindowFrameSize",SideEffects::worstDefault,"glfwGetWindowFrameSize")
		->args({"window","left","top","right","bottom"});
	addExtern< void (*)(GLFWwindow *,float *,float *) , glfwGetWindowContentScale >(*this,lib,"glfwGetWindowContentScale",SideEffects::worstDefault,"glfwGetWindowContentScale")
		->args({"window","xscale","yscale"});
	addExtern< float (*)(GLFWwindow *) , glfwGetWindowOpacity >(*this,lib,"glfwGetWindowOpacity",SideEffects::worstDefault,"glfwGetWindowOpacity")
		->args({"window"});
	addExtern< void (*)(GLFWwindow *,float) , glfwSetWindowOpacity >(*this,lib,"glfwSetWindowOpacity",SideEffects::worstDefault,"glfwSetWindowOpacity")
		->args({"window","opacity"});
	addExtern< void (*)(GLFWwindow *) , glfwIconifyWindow >(*this,lib,"glfwIconifyWindow",SideEffects::worstDefault,"glfwIconifyWindow")
		->args({"window"});
}
}

