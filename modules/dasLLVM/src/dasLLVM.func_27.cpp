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
void Module_dasLLVM::initFunctions_27() {
// from D:\Work\libclang\include\llvm-c/Core.h:3245:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMIsATerminatorInst >(*this,lib,"LLVMIsATerminatorInst",SideEffects::worstDefault,"LLVMIsATerminatorInst")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3267:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetNumArgOperands >(*this,lib,"LLVMGetNumArgOperands",SideEffects::worstDefault,"LLVMGetNumArgOperands")
		->args({"Instr"});
// from D:\Work\libclang\include\llvm-c/Core.h:3278:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int) , LLVMSetInstructionCallConv >(*this,lib,"LLVMSetInstructionCallConv",SideEffects::worstDefault,"LLVMSetInstructionCallConv")
		->args({"Instr","CC"});
// from D:\Work\libclang\include\llvm-c/Core.h:3288:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetInstructionCallConv >(*this,lib,"LLVMGetInstructionCallConv",SideEffects::worstDefault,"LLVMGetInstructionCallConv")
		->args({"Instr"});
// from D:\Work\libclang\include\llvm-c/Core.h:3290:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,unsigned int) , LLVMSetInstrParamAlignment >(*this,lib,"LLVMSetInstrParamAlignment",SideEffects::worstDefault,"LLVMSetInstrParamAlignment")
		->args({"Instr","index","Align"});
// from D:\Work\libclang\include\llvm-c/Core.h:3293:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueAttributeRef *) , LLVMAddCallSiteAttribute >(*this,lib,"LLVMAddCallSiteAttribute",SideEffects::worstDefault,"LLVMAddCallSiteAttribute")
		->args({"C","Idx","A"});
// from D:\Work\libclang\include\llvm-c/Core.h:3295:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetCallSiteAttributeCount >(*this,lib,"LLVMGetCallSiteAttributeCount",SideEffects::worstDefault,"LLVMGetCallSiteAttributeCount")
		->args({"C","Idx"});
// from D:\Work\libclang\include\llvm-c/Core.h:3296:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueAttributeRef **) , LLVMGetCallSiteAttributes >(*this,lib,"LLVMGetCallSiteAttributes",SideEffects::worstDefault,"LLVMGetCallSiteAttributes")
		->args({"C","Idx","Attrs"});
// from D:\Work\libclang\include\llvm-c/Core.h:3298:18
	addExtern< LLVMOpaqueAttributeRef * (*)(LLVMOpaqueValue *,unsigned int,unsigned int) , LLVMGetCallSiteEnumAttribute >(*this,lib,"LLVMGetCallSiteEnumAttribute",SideEffects::worstDefault,"LLVMGetCallSiteEnumAttribute")
		->args({"C","Idx","KindID"});
// from D:\Work\libclang\include\llvm-c/Core.h:3301:18
	addExtern< LLVMOpaqueAttributeRef * (*)(LLVMOpaqueValue *,unsigned int,const char *,unsigned int) , LLVMGetCallSiteStringAttribute >(*this,lib,"LLVMGetCallSiteStringAttribute",SideEffects::worstDefault,"LLVMGetCallSiteStringAttribute")
		->args({"C","Idx","K","KLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:3304:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,unsigned int) , LLVMRemoveCallSiteEnumAttribute >(*this,lib,"LLVMRemoveCallSiteEnumAttribute",SideEffects::worstDefault,"LLVMRemoveCallSiteEnumAttribute")
		->args({"C","Idx","KindID"});
// from D:\Work\libclang\include\llvm-c/Core.h:3306:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,const char *,unsigned int) , LLVMRemoveCallSiteStringAttribute >(*this,lib,"LLVMRemoveCallSiteStringAttribute",SideEffects::worstDefault,"LLVMRemoveCallSiteStringAttribute")
		->args({"C","Idx","K","KLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:3314:13
	addExtern< LLVMOpaqueType * (*)(LLVMOpaqueValue *) , LLVMGetCalledFunctionType >(*this,lib,"LLVMGetCalledFunctionType",SideEffects::worstDefault,"LLVMGetCalledFunctionType")
		->args({"C"});
// from D:\Work\libclang\include\llvm-c/Core.h:3325:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetCalledValue >(*this,lib,"LLVMGetCalledValue",SideEffects::worstDefault,"LLVMGetCalledValue")
		->args({"Instr"});
// from D:\Work\libclang\include\llvm-c/Core.h:3334:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsTailCall >(*this,lib,"LLVMIsTailCall",SideEffects::worstDefault,"LLVMIsTailCall")
		->args({"CallInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3343:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetTailCall >(*this,lib,"LLVMSetTailCall",SideEffects::worstDefault,"LLVMSetTailCall")
		->args({"CallInst","IsTailCall"});
// from D:\Work\libclang\include\llvm-c/Core.h:3352:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *) , LLVMGetNormalDest >(*this,lib,"LLVMGetNormalDest",SideEffects::worstDefault,"LLVMGetNormalDest")
		->args({"InvokeInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3364:19
	addExtern< LLVMOpaqueBasicBlock * (*)(LLVMOpaqueValue *) , LLVMGetUnwindDest >(*this,lib,"LLVMGetUnwindDest",SideEffects::worstDefault,"LLVMGetUnwindDest")
		->args({"InvokeInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3373:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMSetNormalDest >(*this,lib,"LLVMSetNormalDest",SideEffects::worstDefault,"LLVMSetNormalDest")
		->args({"InvokeInst","B"});
// from D:\Work\libclang\include\llvm-c/Core.h:3385:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMSetUnwindDest >(*this,lib,"LLVMSetUnwindDest",SideEffects::worstDefault,"LLVMSetUnwindDest")
		->args({"InvokeInst","B"});
}
}

