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
void Module_dasLLVM::initFunctions_31() {
// from D:\Work\libclang\include\llvm-c/Core.h:3733:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetArgOperand >(*this,lib,"LLVMGetArgOperand",SideEffects::worstDefault,"LLVMGetArgOperand")
		->args({"Funclet","i"});
// from D:\Work\libclang\include\llvm-c/Core.h:3736:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueValue *) , LLVMSetArgOperand >(*this,lib,"LLVMSetArgOperand",SideEffects::worstDefault,"LLVMSetArgOperand")
		->args({"Funclet","i","value"});
// from D:\Work\libclang\include\llvm-c/Core.h:3745:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetParentCatchSwitch >(*this,lib,"LLVMGetParentCatchSwitch",SideEffects::worstDefault,"LLVMGetParentCatchSwitch")
		->args({"CatchPad"});
// from D:\Work\libclang\include\llvm-c/Core.h:3754:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMSetParentCatchSwitch >(*this,lib,"LLVMSetParentCatchSwitch",SideEffects::worstDefault,"LLVMSetParentCatchSwitch")
		->args({"CatchPad","CatchSwitch"});
// from D:\Work\libclang\include\llvm-c/Core.h:3757:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildAdd >(*this,lib,"LLVMBuildAdd",SideEffects::worstDefault,"LLVMBuildAdd")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3759:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildNSWAdd >(*this,lib,"LLVMBuildNSWAdd",SideEffects::worstDefault,"LLVMBuildNSWAdd")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3761:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildNUWAdd >(*this,lib,"LLVMBuildNUWAdd",SideEffects::worstDefault,"LLVMBuildNUWAdd")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3763:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildFAdd >(*this,lib,"LLVMBuildFAdd",SideEffects::worstDefault,"LLVMBuildFAdd")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3765:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildSub >(*this,lib,"LLVMBuildSub",SideEffects::worstDefault,"LLVMBuildSub")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3767:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildNSWSub >(*this,lib,"LLVMBuildNSWSub",SideEffects::worstDefault,"LLVMBuildNSWSub")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3769:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildNUWSub >(*this,lib,"LLVMBuildNUWSub",SideEffects::worstDefault,"LLVMBuildNUWSub")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3771:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildFSub >(*this,lib,"LLVMBuildFSub",SideEffects::worstDefault,"LLVMBuildFSub")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3773:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildMul >(*this,lib,"LLVMBuildMul",SideEffects::worstDefault,"LLVMBuildMul")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3775:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildNSWMul >(*this,lib,"LLVMBuildNSWMul",SideEffects::worstDefault,"LLVMBuildNSWMul")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3777:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildNUWMul >(*this,lib,"LLVMBuildNUWMul",SideEffects::worstDefault,"LLVMBuildNUWMul")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3779:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildFMul >(*this,lib,"LLVMBuildFMul",SideEffects::worstDefault,"LLVMBuildFMul")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3781:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildUDiv >(*this,lib,"LLVMBuildUDiv",SideEffects::worstDefault,"LLVMBuildUDiv")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3783:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildExactUDiv >(*this,lib,"LLVMBuildExactUDiv",SideEffects::worstDefault,"LLVMBuildExactUDiv")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3785:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildSDiv >(*this,lib,"LLVMBuildSDiv",SideEffects::worstDefault,"LLVMBuildSDiv")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3787:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildExactSDiv >(*this,lib,"LLVMBuildExactSDiv",SideEffects::worstDefault,"LLVMBuildExactSDiv")
		->args({"","LHS","RHS","Name"});
}
}

