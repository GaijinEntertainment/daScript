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
void Module_dasLLVM::initFunctions_23() {
// from D:\Work\libclang\include\llvm-c/Core.h:2563:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int) , LLVMSetFunctionCallConv >(*this,lib,"LLVMSetFunctionCallConv",SideEffects::worstDefault,"LLVMSetFunctionCallConv")
		->args({"Fn","CC"});
// from D:\Work\libclang\include\llvm-c/Core.h:2571:13
	addExtern< const char * (*)(LLVMOpaqueValue *) , LLVMGetGC >(*this,lib,"LLVMGetGC",SideEffects::worstDefault,"LLVMGetGC")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2578:6
	addExtern< void (*)(LLVMOpaqueValue *,const char *) , LLVMSetGC >(*this,lib,"LLVMSetGC",SideEffects::worstDefault,"LLVMSetGC")
		->args({"Fn","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:2585:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueAttributeRef *) , LLVMAddAttributeAtIndex >(*this,lib,"LLVMAddAttributeAtIndex",SideEffects::worstDefault,"LLVMAddAttributeAtIndex")
		->args({"F","Idx","A"});
// from D:\Work\libclang\include\llvm-c/Core.h:2587:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetAttributeCountAtIndex >(*this,lib,"LLVMGetAttributeCountAtIndex",SideEffects::worstDefault,"LLVMGetAttributeCountAtIndex")
		->args({"F","Idx"});
// from D:\Work\libclang\include\llvm-c/Core.h:2588:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueAttributeRef **) , LLVMGetAttributesAtIndex >(*this,lib,"LLVMGetAttributesAtIndex",SideEffects::worstDefault,"LLVMGetAttributesAtIndex")
		->args({"F","Idx","Attrs"});
// from D:\Work\libclang\include\llvm-c/Core.h:2590:18
	addExtern< LLVMOpaqueAttributeRef * (*)(LLVMOpaqueValue *,unsigned int,unsigned int) , LLVMGetEnumAttributeAtIndex >(*this,lib,"LLVMGetEnumAttributeAtIndex",SideEffects::worstDefault,"LLVMGetEnumAttributeAtIndex")
		->args({"F","Idx","KindID"});
// from D:\Work\libclang\include\llvm-c/Core.h:2593:18
	addExtern< LLVMOpaqueAttributeRef * (*)(LLVMOpaqueValue *,unsigned int,const char *,unsigned int) , LLVMGetStringAttributeAtIndex >(*this,lib,"LLVMGetStringAttributeAtIndex",SideEffects::worstDefault,"LLVMGetStringAttributeAtIndex")
		->args({"F","Idx","K","KLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:2596:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,unsigned int) , LLVMRemoveEnumAttributeAtIndex >(*this,lib,"LLVMRemoveEnumAttributeAtIndex",SideEffects::worstDefault,"LLVMRemoveEnumAttributeAtIndex")
		->args({"F","Idx","KindID"});
// from D:\Work\libclang\include\llvm-c/Core.h:2598:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,const char *,unsigned int) , LLVMRemoveStringAttributeAtIndex >(*this,lib,"LLVMRemoveStringAttributeAtIndex",SideEffects::worstDefault,"LLVMRemoveStringAttributeAtIndex")
		->args({"F","Idx","K","KLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:2605:6
	addExtern< void (*)(LLVMOpaqueValue *,const char *,const char *) , LLVMAddTargetDependentFunctionAttr >(*this,lib,"LLVMAddTargetDependentFunctionAttr",SideEffects::worstDefault,"LLVMAddTargetDependentFunctionAttr")
		->args({"Fn","A","V"});
// from D:\Work\libclang\include\llvm-c/Core.h:2624:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMCountParams >(*this,lib,"LLVMCountParams",SideEffects::worstDefault,"LLVMCountParams")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2637:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue **) , LLVMGetParams >(*this,lib,"LLVMGetParams",SideEffects::worstDefault,"LLVMGetParams")
		->args({"Fn","Params"});
// from D:\Work\libclang\include\llvm-c/Core.h:2646:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetParam >(*this,lib,"LLVMGetParam",SideEffects::worstDefault,"LLVMGetParam")
		->args({"Fn","Index"});
// from D:\Work\libclang\include\llvm-c/Core.h:2657:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetParamParent >(*this,lib,"LLVMGetParamParent",SideEffects::worstDefault,"LLVMGetParamParent")
		->args({"Inst"});
// from D:\Work\libclang\include\llvm-c/Core.h:2664:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetFirstParam >(*this,lib,"LLVMGetFirstParam",SideEffects::worstDefault,"LLVMGetFirstParam")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2671:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetLastParam >(*this,lib,"LLVMGetLastParam",SideEffects::worstDefault,"LLVMGetLastParam")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2680:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetNextParam >(*this,lib,"LLVMGetNextParam",SideEffects::worstDefault,"LLVMGetNextParam")
		->args({"Arg"});
// from D:\Work\libclang\include\llvm-c/Core.h:2687:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetPreviousParam >(*this,lib,"LLVMGetPreviousParam",SideEffects::worstDefault,"LLVMGetPreviousParam")
		->args({"Arg"});
// from D:\Work\libclang\include\llvm-c/Core.h:2695:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int) , LLVMSetParamAlignment >(*this,lib,"LLVMSetParamAlignment",SideEffects::worstDefault,"LLVMSetParamAlignment")
		->args({"Arg","Align"});
}
}

