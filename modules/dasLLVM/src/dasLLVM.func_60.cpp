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
void Module_dasLLVM::initFunctions_60() {
// from D:\Work\libclang\include\llvm-c/Transforms/InstCombine.h:31:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddInstructionCombiningPass >(*this,lib,"LLVMAddInstructionCombiningPass",SideEffects::worstDefault,"LLVMAddInstructionCombiningPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:31:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddArgumentPromotionPass >(*this,lib,"LLVMAddArgumentPromotionPass",SideEffects::worstDefault,"LLVMAddArgumentPromotionPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:34:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddConstantMergePass >(*this,lib,"LLVMAddConstantMergePass",SideEffects::worstDefault,"LLVMAddConstantMergePass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:37:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddMergeFunctionsPass >(*this,lib,"LLVMAddMergeFunctionsPass",SideEffects::worstDefault,"LLVMAddMergeFunctionsPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:40:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddCalledValuePropagationPass >(*this,lib,"LLVMAddCalledValuePropagationPass",SideEffects::worstDefault,"LLVMAddCalledValuePropagationPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:43:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddDeadArgEliminationPass >(*this,lib,"LLVMAddDeadArgEliminationPass",SideEffects::worstDefault,"LLVMAddDeadArgEliminationPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:46:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddFunctionAttrsPass >(*this,lib,"LLVMAddFunctionAttrsPass",SideEffects::worstDefault,"LLVMAddFunctionAttrsPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:49:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddFunctionInliningPass >(*this,lib,"LLVMAddFunctionInliningPass",SideEffects::worstDefault,"LLVMAddFunctionInliningPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:52:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddAlwaysInlinerPass >(*this,lib,"LLVMAddAlwaysInlinerPass",SideEffects::worstDefault,"LLVMAddAlwaysInlinerPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:55:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddGlobalDCEPass >(*this,lib,"LLVMAddGlobalDCEPass",SideEffects::worstDefault,"LLVMAddGlobalDCEPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:58:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddGlobalOptimizerPass >(*this,lib,"LLVMAddGlobalOptimizerPass",SideEffects::worstDefault,"LLVMAddGlobalOptimizerPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:61:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddPruneEHPass >(*this,lib,"LLVMAddPruneEHPass",SideEffects::worstDefault,"LLVMAddPruneEHPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:64:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddIPSCCPPass >(*this,lib,"LLVMAddIPSCCPPass",SideEffects::worstDefault,"LLVMAddIPSCCPPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:67:6
	addExtern< void (*)(LLVMOpaquePassManager *,unsigned int) , LLVMAddInternalizePass >(*this,lib,"LLVMAddInternalizePass",SideEffects::worstDefault,"LLVMAddInternalizePass")
		->args({"","AllButMain"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:85:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddStripDeadPrototypesPass >(*this,lib,"LLVMAddStripDeadPrototypesPass",SideEffects::worstDefault,"LLVMAddStripDeadPrototypesPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/IPO.h:88:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddStripSymbolsPass >(*this,lib,"LLVMAddStripSymbolsPass",SideEffects::worstDefault,"LLVMAddStripSymbolsPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:42:14
	addExtern< LLVMOpaqueError * (*)(LLVMOpaqueModule *,const char *,LLVMOpaqueTargetMachine *,LLVMOpaquePassBuilderOptions *) , LLVMRunPasses >(*this,lib,"LLVMRunPasses",SideEffects::worstDefault,"LLVMRunPasses")
		->args({"M","Passes","TM","Options"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:53:27
	addExtern< LLVMOpaquePassBuilderOptions * (*)() , LLVMCreatePassBuilderOptions >(*this,lib,"LLVMCreatePassBuilderOptions",SideEffects::worstDefault,"LLVMCreatePassBuilderOptions");
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:59:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetVerifyEach >(*this,lib,"LLVMPassBuilderOptionsSetVerifyEach",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetVerifyEach")
		->args({"Options","VerifyEach"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:65:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetDebugLogging >(*this,lib,"LLVMPassBuilderOptionsSetDebugLogging",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetDebugLogging")
		->args({"Options","DebugLogging"});
}
}

