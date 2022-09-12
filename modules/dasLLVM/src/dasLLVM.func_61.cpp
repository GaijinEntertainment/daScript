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
void Module_dasLLVM::initFunctions_61() {
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:68:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetLoopInterleaving >(*this,lib,"LLVMPassBuilderOptionsSetLoopInterleaving",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetLoopInterleaving")
		->args({"Options","LoopInterleaving"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:71:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetLoopVectorization >(*this,lib,"LLVMPassBuilderOptionsSetLoopVectorization",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetLoopVectorization")
		->args({"Options","LoopVectorization"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:74:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetSLPVectorization >(*this,lib,"LLVMPassBuilderOptionsSetSLPVectorization",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetSLPVectorization")
		->args({"Options","SLPVectorization"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:77:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetLoopUnrolling >(*this,lib,"LLVMPassBuilderOptionsSetLoopUnrolling",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetLoopUnrolling")
		->args({"Options","LoopUnrolling"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:80:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll >(*this,lib,"LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll")
		->args({"Options","ForgetAllSCEVInLoopUnroll"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:83:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,unsigned int) , LLVMPassBuilderOptionsSetLicmMssaOptCap >(*this,lib,"LLVMPassBuilderOptionsSetLicmMssaOptCap",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetLicmMssaOptCap")
		->args({"Options","LicmMssaOptCap"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:86:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,unsigned int) , LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap >(*this,lib,"LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap")
		->args({"Options","LicmMssaNoAccForPromotionCap"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:89:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetCallGraphProfile >(*this,lib,"LLVMPassBuilderOptionsSetCallGraphProfile",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetCallGraphProfile")
		->args({"Options","CallGraphProfile"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:92:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *,int) , LLVMPassBuilderOptionsSetMergeFunctions >(*this,lib,"LLVMPassBuilderOptionsSetMergeFunctions",SideEffects::worstDefault,"LLVMPassBuilderOptionsSetMergeFunctions")
		->args({"Options","MergeFunctions"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassBuilder.h:98:6
	addExtern< void (*)(LLVMOpaquePassBuilderOptions *) , LLVMDisposePassBuilderOptions >(*this,lib,"LLVMDisposePassBuilderOptions",SideEffects::worstDefault,"LLVMDisposePassBuilderOptions")
		->args({"Options"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:35:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddAggressiveDCEPass >(*this,lib,"LLVMAddAggressiveDCEPass",SideEffects::worstDefault,"LLVMAddAggressiveDCEPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:38:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddDCEPass >(*this,lib,"LLVMAddDCEPass",SideEffects::worstDefault,"LLVMAddDCEPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:41:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddBitTrackingDCEPass >(*this,lib,"LLVMAddBitTrackingDCEPass",SideEffects::worstDefault,"LLVMAddBitTrackingDCEPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:44:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddAlignmentFromAssumptionsPass >(*this,lib,"LLVMAddAlignmentFromAssumptionsPass",SideEffects::worstDefault,"LLVMAddAlignmentFromAssumptionsPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:47:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddCFGSimplificationPass >(*this,lib,"LLVMAddCFGSimplificationPass",SideEffects::worstDefault,"LLVMAddCFGSimplificationPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:50:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddDeadStoreEliminationPass >(*this,lib,"LLVMAddDeadStoreEliminationPass",SideEffects::worstDefault,"LLVMAddDeadStoreEliminationPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:53:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddScalarizerPass >(*this,lib,"LLVMAddScalarizerPass",SideEffects::worstDefault,"LLVMAddScalarizerPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:56:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddMergedLoadStoreMotionPass >(*this,lib,"LLVMAddMergedLoadStoreMotionPass",SideEffects::worstDefault,"LLVMAddMergedLoadStoreMotionPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:59:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddGVNPass >(*this,lib,"LLVMAddGVNPass",SideEffects::worstDefault,"LLVMAddGVNPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Scalar.h:62:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddNewGVNPass >(*this,lib,"LLVMAddNewGVNPass",SideEffects::worstDefault,"LLVMAddNewGVNPass")
		->args({"PM"});
}
}

