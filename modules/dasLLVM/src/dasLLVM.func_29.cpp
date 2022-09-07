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
void Module_dasLLVM::initFunctions_29() {
// from D:\Work\libclang\include\llvm-c/Core.h:3583:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueBasicBlock *) , LLVMPositionBuilderAtEnd >(*this,lib,"LLVMPositionBuilderAtEnd",SideEffects::worstDefault,"LLVMPositionBuilderAtEnd")
		->args({"Builder","Block"});
// from D:\Work\libclang\include\llvm-c/Core.h:3584:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueBuilder *) , LLVMGetInsertBlock >(*this,lib,"LLVMGetInsertBlock",SideEffects::worstDefault,"LLVMGetInsertBlock")
		->args({"Builder"});
// from D:\Work\libclang\include\llvm-c/Core.h:3585:6
	addExtern< void (*)(LLVMOpaqueBuilder *) , LLVMClearInsertionPosition >(*this,lib,"LLVMClearInsertionPosition",SideEffects::worstDefault,"LLVMClearInsertionPosition")
		->args({"Builder"});
// from D:\Work\libclang\include\llvm-c/Core.h:3586:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *) , LLVMInsertIntoBuilder >(*this,lib,"LLVMInsertIntoBuilder",SideEffects::worstDefault,"LLVMInsertIntoBuilder")
		->args({"Builder","Instr"});
// from D:\Work\libclang\include\llvm-c/Core.h:3587:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMInsertIntoBuilderWithName >(*this,lib,"LLVMInsertIntoBuilderWithName",SideEffects::worstDefault,"LLVMInsertIntoBuilderWithName")
		->args({"Builder","Instr","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3589:6
	addExtern< void (*)(LLVMOpaqueBuilder *) , LLVMDisposeBuilder >(*this,lib,"LLVMDisposeBuilder",SideEffects::worstDefault,"LLVMDisposeBuilder")
		->args({"Builder"});
// from D:\Work\libclang\include\llvm-c/Core.h:3598:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueBuilder *) , LLVMGetCurrentDebugLocation2 >(*this,lib,"LLVMGetCurrentDebugLocation2",SideEffects::worstDefault,"LLVMGetCurrentDebugLocation2")
		->args({"Builder"});
// from D:\Work\libclang\include\llvm-c/Core.h:3607:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueMetadata *) , LLVMSetCurrentDebugLocation2 >(*this,lib,"LLVMSetCurrentDebugLocation2",SideEffects::worstDefault,"LLVMSetCurrentDebugLocation2")
		->args({"Builder","Loc"});
// from D:\Work\libclang\include\llvm-c/Core.h:3616:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *) , LLVMSetInstDebugLocation >(*this,lib,"LLVMSetInstDebugLocation",SideEffects::worstDefault,"LLVMSetInstDebugLocation")
		->args({"Builder","Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3623:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueBuilder *) , LLVMBuilderGetDefaultFPMathTag >(*this,lib,"LLVMBuilderGetDefaultFPMathTag",SideEffects::worstDefault,"LLVMBuilderGetDefaultFPMathTag")
		->args({"Builder"});
// from D:\Work\libclang\include\llvm-c/Core.h:3632:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueMetadata *) , LLVMBuilderSetDefaultFPMathTag >(*this,lib,"LLVMBuilderSetDefaultFPMathTag",SideEffects::worstDefault,"LLVMBuilderSetDefaultFPMathTag")
		->args({"Builder","FPMathTag"});
// from D:\Work\libclang\include\llvm-c/Core.h:3639:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *) , LLVMSetCurrentDebugLocation >(*this,lib,"LLVMSetCurrentDebugLocation",SideEffects::worstDefault,"LLVMSetCurrentDebugLocation")
		->args({"Builder","L"});
// from D:\Work\libclang\include\llvm-c/Core.h:3644:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *) , LLVMGetCurrentDebugLocation >(*this,lib,"LLVMGetCurrentDebugLocation",SideEffects::worstDefault,"LLVMGetCurrentDebugLocation")
		->args({"Builder"});
// from D:\Work\libclang\include\llvm-c/Core.h:3647:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *) , LLVMBuildRetVoid >(*this,lib,"LLVMBuildRetVoid",SideEffects::worstDefault,"LLVMBuildRetVoid")
		->args({""});
// from D:\Work\libclang\include\llvm-c/Core.h:3648:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *) , LLVMBuildRet >(*this,lib,"LLVMBuildRet",SideEffects::worstDefault,"LLVMBuildRet")
		->args({"","V"});
// from D:\Work\libclang\include\llvm-c/Core.h:3649:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue **,unsigned int) , LLVMBuildAggregateRet >(*this,lib,"LLVMBuildAggregateRet",SideEffects::worstDefault,"LLVMBuildAggregateRet")
		->args({"","RetVals","N"});
// from D:\Work\libclang\include\llvm-c/Core.h:3651:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueBasicBlock *) , LLVMBuildBr >(*this,lib,"LLVMBuildBr",SideEffects::worstDefault,"LLVMBuildBr")
		->args({"","Dest"});
// from D:\Work\libclang\include\llvm-c/Core.h:3652:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueBasicBlock *,LLVMOpaqueBasicBlock *) , LLVMBuildCondBr >(*this,lib,"LLVMBuildCondBr",SideEffects::worstDefault,"LLVMBuildCondBr")
		->args({"","If","Then","Else"});
// from D:\Work\libclang\include\llvm-c/Core.h:3654:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueBasicBlock *,unsigned int) , LLVMBuildSwitch >(*this,lib,"LLVMBuildSwitch",SideEffects::worstDefault,"LLVMBuildSwitch")
		->args({"","V","Else","NumCases"});
// from D:\Work\libclang\include\llvm-c/Core.h:3656:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,unsigned int) , LLVMBuildIndirectBr >(*this,lib,"LLVMBuildIndirectBr",SideEffects::worstDefault,"LLVMBuildIndirectBr")
		->args({"B","Addr","NumDests"});
}
}

