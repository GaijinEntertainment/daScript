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
void Module_dasLLVM::initFunctions_26() {
// from D:\Work\libclang\include\llvm-c/Core.h:3058:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueBasicBlock *,const char *) , LLVMInsertBasicBlock >(*this,lib,"LLVMInsertBasicBlock",SideEffects::worstDefault,"LLVMInsertBasicBlock")
		->args({"InsertBeforeBB","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3069:6
	addExtern< void (*)(LLVMOpaqueBasicBlock *) , LLVMDeleteBasicBlock >(*this,lib,"LLVMDeleteBasicBlock",SideEffects::worstDefault,"LLVMDeleteBasicBlock")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:3079:6
	addExtern< void (*)(LLVMOpaqueBasicBlock *) , LLVMRemoveBasicBlockFromParent >(*this,lib,"LLVMRemoveBasicBlockFromParent",SideEffects::worstDefault,"LLVMRemoveBasicBlockFromParent")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:3086:6
	addExtern< void (*)(LLVMOpaqueBasicBlock *,LLVMOpaqueBasicBlock *) , LLVMMoveBasicBlockBefore >(*this,lib,"LLVMMoveBasicBlockBefore",SideEffects::worstDefault,"LLVMMoveBasicBlockBefore")
		->args({"BB","MovePos"});
// from D:\Work\libclang\include\llvm-c/Core.h:3093:6
	addExtern< void (*)(LLVMOpaqueBasicBlock *,LLVMOpaqueBasicBlock *) , LLVMMoveBasicBlockAfter >(*this,lib,"LLVMMoveBasicBlockAfter",SideEffects::worstDefault,"LLVMMoveBasicBlockAfter")
		->args({"BB","MovePos"});
// from D:\Work\libclang\include\llvm-c/Core.h:3101:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBasicBlock *) , LLVMGetFirstInstruction >(*this,lib,"LLVMGetFirstInstruction",SideEffects::worstDefault,"LLVMGetFirstInstruction")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:3108:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBasicBlock *) , LLVMGetLastInstruction >(*this,lib,"LLVMGetLastInstruction",SideEffects::worstDefault,"LLVMGetLastInstruction")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:3134:5
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMHasMetadata >(*this,lib,"LLVMHasMetadata",SideEffects::worstDefault,"LLVMHasMetadata")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:3139:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetMetadata >(*this,lib,"LLVMGetMetadata",SideEffects::worstDefault,"LLVMGetMetadata")
		->args({"Val","KindID"});
// from D:\Work\libclang\include\llvm-c/Core.h:3144:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueValue *) , LLVMSetMetadata >(*this,lib,"LLVMSetMetadata",SideEffects::worstDefault,"LLVMSetMetadata")
		->args({"Val","KindID","Node"});
// from D:\Work\libclang\include\llvm-c/Core.h:3153:1
	addExtern< LLVMOpaqueValueMetadataEntry * (*)(LLVMOpaqueValue *,size_t *) , LLVMInstructionGetAllMetadataOtherThanDebugLoc >(*this,lib,"LLVMInstructionGetAllMetadataOtherThanDebugLoc",SideEffects::worstDefault,"LLVMInstructionGetAllMetadataOtherThanDebugLoc")
		->args({"Instr","NumEntries"});
// from D:\Work\libclang\include\llvm-c/Core.h:3161:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *) , LLVMGetInstructionParent >(*this,lib,"LLVMGetInstructionParent",SideEffects::worstDefault,"LLVMGetInstructionParent")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3171:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetNextInstruction >(*this,lib,"LLVMGetNextInstruction",SideEffects::worstDefault,"LLVMGetNextInstruction")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3179:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetPreviousInstruction >(*this,lib,"LLVMGetPreviousInstruction",SideEffects::worstDefault,"LLVMGetPreviousInstruction")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3189:6
	addExtern< void (*)(LLVMOpaqueValue *) , LLVMInstructionRemoveFromParent >(*this,lib,"LLVMInstructionRemoveFromParent",SideEffects::worstDefault,"LLVMInstructionRemoveFromParent")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3199:6
	addExtern< void (*)(LLVMOpaqueValue *) , LLVMInstructionEraseFromParent >(*this,lib,"LLVMInstructionEraseFromParent",SideEffects::worstDefault,"LLVMInstructionEraseFromParent")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3206:12
	addExtern< LLVMOpcode (*)(LLVMOpaqueValue *) , LLVMGetInstructionOpcode >(*this,lib,"LLVMGetInstructionOpcode",SideEffects::worstDefault,"LLVMGetInstructionOpcode")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3216:18
	addExtern< LLVMIntPredicate (*)(LLVMOpaqueValue *) , LLVMGetICmpPredicate >(*this,lib,"LLVMGetICmpPredicate",SideEffects::worstDefault,"LLVMGetICmpPredicate")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3226:19
	addExtern< LLVMRealPredicate (*)(LLVMOpaqueValue *) , LLVMGetFCmpPredicate >(*this,lib,"LLVMGetFCmpPredicate",SideEffects::worstDefault,"LLVMGetFCmpPredicate")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3236:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMInstructionClone >(*this,lib,"LLVMInstructionClone",SideEffects::worstDefault,"LLVMInstructionClone")
		->args({"Inst"});
}
}

