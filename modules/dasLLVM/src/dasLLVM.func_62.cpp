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
void Module_dasLLVM::initFunctions_62() {
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:65:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddIndVarSimplifyPass >(*this,lib,"LLVMAddIndVarSimplifyPass",SideEffects::worstDefault,"LLVMAddIndVarSimplifyPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:71:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddInstructionSimplifyPass >(*this,lib,"LLVMAddInstructionSimplifyPass",SideEffects::worstDefault,"LLVMAddInstructionSimplifyPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:74:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddJumpThreadingPass >(*this,lib,"LLVMAddJumpThreadingPass",SideEffects::worstDefault,"LLVMAddJumpThreadingPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:77:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLICMPass >(*this,lib,"LLVMAddLICMPass",SideEffects::worstDefault,"LLVMAddLICMPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:80:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLoopDeletionPass >(*this,lib,"LLVMAddLoopDeletionPass",SideEffects::worstDefault,"LLVMAddLoopDeletionPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:83:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLoopIdiomPass >(*this,lib,"LLVMAddLoopIdiomPass",SideEffects::worstDefault,"LLVMAddLoopIdiomPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:86:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLoopRotatePass >(*this,lib,"LLVMAddLoopRotatePass",SideEffects::worstDefault,"LLVMAddLoopRotatePass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:89:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLoopRerollPass >(*this,lib,"LLVMAddLoopRerollPass",SideEffects::worstDefault,"LLVMAddLoopRerollPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:92:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLoopUnrollPass >(*this,lib,"LLVMAddLoopUnrollPass",SideEffects::worstDefault,"LLVMAddLoopUnrollPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:95:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLoopUnrollAndJamPass >(*this,lib,"LLVMAddLoopUnrollAndJamPass",SideEffects::worstDefault,"LLVMAddLoopUnrollAndJamPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:98:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLoopUnswitchPass >(*this,lib,"LLVMAddLoopUnswitchPass",SideEffects::worstDefault,"LLVMAddLoopUnswitchPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:101:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddLowerAtomicPass >(*this,lib,"LLVMAddLowerAtomicPass",SideEffects::worstDefault,"LLVMAddLowerAtomicPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:104:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddMemCpyOptPass >(*this,lib,"LLVMAddMemCpyOptPass",SideEffects::worstDefault,"LLVMAddMemCpyOptPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:107:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddPartiallyInlineLibCallsPass >(*this,lib,"LLVMAddPartiallyInlineLibCallsPass",SideEffects::worstDefault,"LLVMAddPartiallyInlineLibCallsPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:110:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddReassociatePass >(*this,lib,"LLVMAddReassociatePass",SideEffects::worstDefault,"LLVMAddReassociatePass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:113:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddSCCPPass >(*this,lib,"LLVMAddSCCPPass",SideEffects::worstDefault,"LLVMAddSCCPPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:116:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddScalarReplAggregatesPass >(*this,lib,"LLVMAddScalarReplAggregatesPass",SideEffects::worstDefault,"LLVMAddScalarReplAggregatesPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:119:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddScalarReplAggregatesPassSSA >(*this,lib,"LLVMAddScalarReplAggregatesPassSSA",SideEffects::worstDefault,"LLVMAddScalarReplAggregatesPassSSA")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:122:6
	addExtern< void (*)(LLVMOpaquePassManager *,int) , LLVMAddScalarReplAggregatesPassWithThreshold >(*this,lib,"LLVMAddScalarReplAggregatesPassWithThreshold",SideEffects::worstDefault,"LLVMAddScalarReplAggregatesPassWithThreshold")
		->args({"PM","Threshold"});
}
}

