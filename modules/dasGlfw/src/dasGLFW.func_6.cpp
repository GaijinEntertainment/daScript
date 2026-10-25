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
void Module_dasGLFW::initFunctions_6() {
// from GLFW/glfw3.h:6715:22
	makeExtern< const char ** (*)(unsigned int *) , glfwGetRequiredInstanceExtensions , SimNode_ExtFuncCall >(lib,"glfwGetRequiredInstanceExtensions","glfwGetRequiredInstanceExtensions")
		->args({"count"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

