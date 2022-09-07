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
void Module_dasLLVM::initFunctions_25() {
// from D:\Work\libclang\include\llvm-c/Core.h:2884:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue **,unsigned int) , LLVMMDNode >(*this,lib,"LLVMMDNode",SideEffects::worstDefault,"LLVMMDNode")
		->args({"Vals","Count"});
// from D:\Work\libclang\include\llvm-c/Core.h:2910:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBasicBlock *) , LLVMBasicBlockAsValue >(*this,lib,"LLVMBasicBlockAsValue",SideEffects::worstDefault,"LLVMBasicBlockAsValue")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:2915:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMValueIsBasicBlock >(*this,lib,"LLVMValueIsBasicBlock",SideEffects::worstDefault,"LLVMValueIsBasicBlock")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:2920:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *) , LLVMValueAsBasicBlock >(*this,lib,"LLVMValueAsBasicBlock",SideEffects::worstDefault,"LLVMValueAsBasicBlock")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:2925:13
	addExtern< const char * (*)(LLVMOpaqueBasicBlock *) , LLVMGetBasicBlockName >(*this,lib,"LLVMGetBasicBlockName",SideEffects::worstDefault,"LLVMGetBasicBlockName")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:2932:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBasicBlock *) , LLVMGetBasicBlockParent >(*this,lib,"LLVMGetBasicBlockParent",SideEffects::worstDefault,"LLVMGetBasicBlockParent")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:2944:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBasicBlock *) , LLVMGetBasicBlockTerminator >(*this,lib,"LLVMGetBasicBlockTerminator",SideEffects::worstDefault,"LLVMGetBasicBlockTerminator")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:2951:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMCountBasicBlocks >(*this,lib,"LLVMCountBasicBlocks",SideEffects::worstDefault,"LLVMCountBasicBlocks")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2961:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueBasicBlock **) , LLVMGetBasicBlocks >(*this,lib,"LLVMGetBasicBlocks",SideEffects::worstDefault,"LLVMGetBasicBlocks")
		->args({"Fn","BasicBlocks"});
// from D:\Work\libclang\include\llvm-c/Core.h:2971:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *) , LLVMGetFirstBasicBlock >(*this,lib,"LLVMGetFirstBasicBlock",SideEffects::worstDefault,"LLVMGetFirstBasicBlock")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2978:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *) , LLVMGetLastBasicBlock >(*this,lib,"LLVMGetLastBasicBlock",SideEffects::worstDefault,"LLVMGetLastBasicBlock")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2983:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueBasicBlock *) , LLVMGetNextBasicBlock >(*this,lib,"LLVMGetNextBasicBlock",SideEffects::worstDefault,"LLVMGetNextBasicBlock")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:2988:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueBasicBlock *) , LLVMGetPreviousBasicBlock >(*this,lib,"LLVMGetPreviousBasicBlock",SideEffects::worstDefault,"LLVMGetPreviousBasicBlock")
		->args({"BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:2996:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *) , LLVMGetEntryBasicBlock >(*this,lib,"LLVMGetEntryBasicBlock",SideEffects::worstDefault,"LLVMGetEntryBasicBlock")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:3005:6
	addExtern< void (*)(LLVMOpaqueBuilder *,LLVMOpaqueBasicBlock *) , LLVMInsertExistingBasicBlockAfterInsertBlock >(*this,lib,"LLVMInsertExistingBasicBlockAfterInsertBlock",SideEffects::worstDefault,"LLVMInsertExistingBasicBlockAfterInsertBlock")
		->args({"Builder","BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:3013:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMAppendExistingBasicBlock >(*this,lib,"LLVMAppendExistingBasicBlock",SideEffects::worstDefault,"LLVMAppendExistingBasicBlock")
		->args({"Fn","BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:3021:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueContext *,const char *) , LLVMCreateBasicBlockInContext >(*this,lib,"LLVMCreateBasicBlockInContext",SideEffects::worstDefault,"LLVMCreateBasicBlockInContext")
		->args({"C","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3029:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueContext *,LLVMOpaqueValue *,const char *) , LLVMAppendBasicBlockInContext >(*this,lib,"LLVMAppendBasicBlockInContext",SideEffects::worstDefault,"LLVMAppendBasicBlockInContext")
		->args({"C","Fn","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3039:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *,const char *) , LLVMAppendBasicBlock >(*this,lib,"LLVMAppendBasicBlock",SideEffects::worstDefault,"LLVMAppendBasicBlock")
		->args({"Fn","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3049:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueContext *,LLVMOpaqueBasicBlock *,const char *) , LLVMInsertBasicBlockInContext >(*this,lib,"LLVMInsertBasicBlockInContext",SideEffects::worstDefault,"LLVMInsertBasicBlockInContext")
		->args({"C","BB","Name"});
}
}

