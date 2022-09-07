// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasLLVM.h"
#include "need_dasLLVM.h"
namespace das {
#include "dasLLVM.func.aot.decl.inc"
void Module_dasLLVM::initFunctions_59() {
// from D:\Work\libclang\include\llvm-c/Support.h:39:6
	addExtern< void (*)(int,const char *const *,const char *) , LLVMParseCommandLineOptions >(*this,lib,"LLVMParseCommandLineOptions",SideEffects::worstDefault,"LLVMParseCommandLineOptions")
		->args({"argc","argv","Overview"});
// from D:\Work\libclang\include\llvm-c/Support.h:49:7
	addExtern< void * (*)(const char *) , LLVMSearchForAddressOfSymbol >(*this,lib,"LLVMSearchForAddressOfSymbol",SideEffects::worstDefault,"LLVMSearchForAddressOfSymbol")
		->args({"symbolName"});
// from D:\Work\libclang\include\llvm-c/Support.h:58:6
	addExtern< void (*)(const char *,void *) , LLVMAddSymbol >(*this,lib,"LLVMAddSymbol",SideEffects::worstDefault,"LLVMAddSymbol")
		->args({"symbolName","symbolValue"});
}
}

