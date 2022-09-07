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
void Module_dasLLVM::initFunctions_28() {
// from D:\Work\libclang\include\llvm-c/Core.h:3405:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetNumSuccessors >(*this,lib,"LLVMGetNumSuccessors",SideEffects::worstDefault,"LLVMGetNumSuccessors")
		->args({"Term"});
// from D:\Work\libclang\include\llvm-c/Core.h:3412:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetSuccessor >(*this,lib,"LLVMGetSuccessor",SideEffects::worstDefault,"LLVMGetSuccessor")
		->args({"Term","i"});
// from D:\Work\libclang\include\llvm-c/Core.h:3419:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueBasicBlock *) , LLVMSetSuccessor >(*this,lib,"LLVMSetSuccessor",SideEffects::worstDefault,"LLVMSetSuccessor")
		->args({"Term","i","block"});
// from D:\Work\libclang\include\llvm-c/Core.h:3428:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsConditional >(*this,lib,"LLVMIsConditional",SideEffects::worstDefault,"LLVMIsConditional")
		->args({"Branch"});
// from D:\Work\libclang\include\llvm-c/Core.h:3437:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetCondition >(*this,lib,"LLVMGetCondition",SideEffects::worstDefault,"LLVMGetCondition")
		->args({"Branch"});
// from D:\Work\libclang\include\llvm-c/Core.h:3446:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMSetCondition >(*this,lib,"LLVMSetCondition",SideEffects::worstDefault,"LLVMSetCondition")
		->args({"Branch","Cond"});
// from D:\Work\libclang\include\llvm-c/Core.h:3455:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *) , LLVMGetSwitchDefaultDest >(*this,lib,"LLVMGetSwitchDefaultDest",SideEffects::worstDefault,"LLVMGetSwitchDefaultDest")
		->args({"SwitchInstr"});
// from D:\Work\libclang\include\llvm-c/Core.h:3473:13
	addExtern< LLVMOpaqueType * (*)(LLVMOpaqueValue *) , LLVMGetAllocatedType >(*this,lib,"LLVMGetAllocatedType",SideEffects::worstDefault,"LLVMGetAllocatedType")
		->args({"Alloca"});
// from D:\Work\libclang\include\llvm-c/Core.h:3491:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsInBounds >(*this,lib,"LLVMIsInBounds",SideEffects::worstDefault,"LLVMIsInBounds")
		->args({"GEP"});
// from D:\Work\libclang\include\llvm-c/Core.h:3496:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetIsInBounds >(*this,lib,"LLVMSetIsInBounds",SideEffects::worstDefault,"LLVMSetIsInBounds")
		->args({"GEP","InBounds"});
// from D:\Work\libclang\include\llvm-c/Core.h:3514:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue **,LLVMOpaqueBasicBlock **,unsigned int) , LLVMAddIncoming >(*this,lib,"LLVMAddIncoming",SideEffects::worstDefault,"LLVMAddIncoming")
		->args({"PhiNode","IncomingValues","IncomingBlocks","Count"});
// from D:\Work\libclang\include\llvm-c/Core.h:3520:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMCountIncoming >(*this,lib,"LLVMCountIncoming",SideEffects::worstDefault,"LLVMCountIncoming")
		->args({"PhiNode"});
// from D:\Work\libclang\include\llvm-c/Core.h:3525:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetIncomingValue >(*this,lib,"LLVMGetIncomingValue",SideEffects::worstDefault,"LLVMGetIncomingValue")
		->args({"PhiNode","Index"});
// from D:\Work\libclang\include\llvm-c/Core.h:3530:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetIncomingBlock >(*this,lib,"LLVMGetIncomingBlock",SideEffects::worstDefault,"LLVMGetIncomingBlock")
		->args({"PhiNode","Index"});
// from D:\Work\libclang\include\llvm-c/Core.h:3550:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetNumIndices >(*this,lib,"LLVMGetNumIndices",SideEffects::worstDefault,"LLVMGetNumIndices")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3555:17
	addExtern< const unsigned int * (*)(LLVMOpaqueValue *) , LLVMGetIndices >(*this,lib,"LLVMGetIndices",SideEffects::worstDefault,"LLVMGetIndices")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3578:16
	addExtern< LLVMOpaqueBuilder * (*)(LLVMOpaqueContext *) , LLVMCreateBuilderInContext >(*this,lib,"LLVMCreateBuilderInContext",SideEffects::worstDefault,"LLVMCreateBuilderInContext")
		->args({"C"});
// from D:\Work\libclang\include\llvm-c/Core.h:3579:16
	addExtern< LLVMOpaqueBuilder * (*)() , LLVMCreateBuilder >(*this,lib,"LLVMCreateBuilder",SideEffects::worstDefault,"LLVMCreateBuilder");
// from D:\Work\libclang\include\llvm-c/Core.h:3580:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueBasicBlock *,LLVMOpaqueValue *) , LLVMPositionBuilder >(*this,lib,"LLVMPositionBuilder",SideEffects::worstDefault,"LLVMPositionBuilder")
		->args({"Builder","Block","Instr"});
// from D:\Work\libclang\include\llvm-c/Core.h:3582:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *) , LLVMPositionBuilderBefore >(*this,lib,"LLVMPositionBuilderBefore",SideEffects::worstDefault,"LLVMPositionBuilderBefore")
		->args({"Builder","Instr"});
}
}

