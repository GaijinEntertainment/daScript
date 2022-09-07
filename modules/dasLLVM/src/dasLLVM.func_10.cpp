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
void Module_dasLLVM::initFunctions_10() {
// from D:\Work\libclang\include\llvm-c/Core.h:1695:6
	addExtern< void (*)(LLVMOpaqueValue *) , LLVMDumpValue >(*this,lib,"LLVMDumpValue",SideEffects::worstDefault,"LLVMDumpValue")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1703:7
	addExtern< char * (*)(LLVMOpaqueValue *) , LLVMPrintValueToString >(*this,lib,"LLVMPrintValueToString",SideEffects::worstDefault,"LLVMPrintValueToString")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1710:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMReplaceAllUsesWith >(*this,lib,"LLVMReplaceAllUsesWith",SideEffects::worstDefault,"LLVMReplaceAllUsesWith")
		->args({"OldVal","NewVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:1715:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsConstant >(*this,lib,"LLVMIsConstant",SideEffects::worstDefault,"LLVMIsConstant")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1720:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsUndef >(*this,lib,"LLVMIsUndef",SideEffects::worstDefault,"LLVMIsUndef")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1725:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsPoison >(*this,lib,"LLVMIsPoison",SideEffects::worstDefault,"LLVMIsPoison")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAArgument >(*this,lib,"LLVMIsAArgument",SideEffects::worstDefault,"LLVMIsAArgument")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsABasicBlock >(*this,lib,"LLVMIsABasicBlock",SideEffects::worstDefault,"LLVMIsABasicBlock")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAInlineAsm >(*this,lib,"LLVMIsAInlineAsm",SideEffects::worstDefault,"LLVMIsAInlineAsm")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAUser >(*this,lib,"LLVMIsAUser",SideEffects::worstDefault,"LLVMIsAUser")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstant >(*this,lib,"LLVMIsAConstant",SideEffects::worstDefault,"LLVMIsAConstant")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsABlockAddress >(*this,lib,"LLVMIsABlockAddress",SideEffects::worstDefault,"LLVMIsABlockAddress")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstantAggregateZero >(*this,lib,"LLVMIsAConstantAggregateZero",SideEffects::worstDefault,"LLVMIsAConstantAggregateZero")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstantArray >(*this,lib,"LLVMIsAConstantArray",SideEffects::worstDefault,"LLVMIsAConstantArray")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstantDataSequential >(*this,lib,"LLVMIsAConstantDataSequential",SideEffects::worstDefault,"LLVMIsAConstantDataSequential")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstantDataArray >(*this,lib,"LLVMIsAConstantDataArray",SideEffects::worstDefault,"LLVMIsAConstantDataArray")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstantDataVector >(*this,lib,"LLVMIsAConstantDataVector",SideEffects::worstDefault,"LLVMIsAConstantDataVector")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstantExpr >(*this,lib,"LLVMIsAConstantExpr",SideEffects::worstDefault,"LLVMIsAConstantExpr")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstantFP >(*this,lib,"LLVMIsAConstantFP",SideEffects::worstDefault,"LLVMIsAConstantFP")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1740:30
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsAConstantInt >(*this,lib,"LLVMIsAConstantInt",SideEffects::worstDefault,"LLVMIsAConstantInt")
		->args({"Val"});
}
}

