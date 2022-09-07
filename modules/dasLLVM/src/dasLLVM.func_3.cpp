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
void Module_dasLLVM::initFunctions_3() {
// from D:\Work\libclang\include\llvm-c/Core.h:572:10
	addExtern< unsigned int (*)(LLVMOpaqueContext *,const char *,unsigned int) , LLVMGetMDKindIDInContext >(*this,lib,"LLVMGetMDKindIDInContext",SideEffects::worstDefault,"LLVMGetMDKindIDInContext")
		->args({"C","Name","SLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:574:10
	addExtern< unsigned int (*)(const char *,unsigned int) , LLVMGetMDKindID >(*this,lib,"LLVMGetMDKindID",SideEffects::worstDefault,"LLVMGetMDKindID")
		->args({"Name","SLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:587:10
	addExtern< unsigned int (*)(const char *,size_t) , LLVMGetEnumAttributeKindForName >(*this,lib,"LLVMGetEnumAttributeKindForName",SideEffects::worstDefault,"LLVMGetEnumAttributeKindForName")
		->args({"Name","SLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:588:10
	addExtern< unsigned int (*)() , LLVMGetLastEnumAttributeKind >(*this,lib,"LLVMGetLastEnumAttributeKind",SideEffects::worstDefault,"LLVMGetLastEnumAttributeKind");
// from D:\Work\libclang\include\llvm-c/Core.h:593:18
	addExtern< LLVMOpaqueAttributeRef * (*)(LLVMOpaqueContext *,unsigned int,uint64_t) , LLVMCreateEnumAttribute >(*this,lib,"LLVMCreateEnumAttribute",SideEffects::worstDefault,"LLVMCreateEnumAttribute")
		->args({"C","KindID","Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:600:10
	addExtern< unsigned int (*)(LLVMOpaqueAttributeRef *) , LLVMGetEnumAttributeKind >(*this,lib,"LLVMGetEnumAttributeKind",SideEffects::worstDefault,"LLVMGetEnumAttributeKind")
		->args({"A"});
// from D:\Work\libclang\include\llvm-c/Core.h:605:10
	addExtern< uint64_t (*)(LLVMOpaqueAttributeRef *) , LLVMGetEnumAttributeValue >(*this,lib,"LLVMGetEnumAttributeValue",SideEffects::worstDefault,"LLVMGetEnumAttributeValue")
		->args({"A"});
// from D:\Work\libclang\include\llvm-c/Core.h:610:18
	addExtern< LLVMOpaqueAttributeRef * (*)(LLVMOpaqueContext *,unsigned int,LLVMOpaqueType *) , LLVMCreateTypeAttribute >(*this,lib,"LLVMCreateTypeAttribute",SideEffects::worstDefault,"LLVMCreateTypeAttribute")
		->args({"C","KindID","type_ref"});
// from D:\Work\libclang\include\llvm-c/Core.h:616:13
	addExtern< LLVMOpaqueType * (*)(LLVMOpaqueAttributeRef *) , LLVMGetTypeAttributeValue >(*this,lib,"LLVMGetTypeAttributeValue",SideEffects::worstDefault,"LLVMGetTypeAttributeValue")
		->args({"A"});
// from D:\Work\libclang\include\llvm-c/Core.h:621:18
	addExtern< LLVMOpaqueAttributeRef * (*)(LLVMOpaqueContext *,const char *,unsigned int,const char *,unsigned int) , LLVMCreateStringAttribute >(*this,lib,"LLVMCreateStringAttribute",SideEffects::worstDefault,"LLVMCreateStringAttribute")
		->args({"C","K","KLength","V","VLength"});
// from D:\Work\libclang\include\llvm-c/Core.h:628:13
	addExtern< const char * (*)(LLVMOpaqueAttributeRef *,unsigned int *) , LLVMGetStringAttributeKind >(*this,lib,"LLVMGetStringAttributeKind",SideEffects::worstDefault,"LLVMGetStringAttributeKind")
		->args({"A","Length"});
// from D:\Work\libclang\include\llvm-c/Core.h:633:13
	addExtern< const char * (*)(LLVMOpaqueAttributeRef *,unsigned int *) , LLVMGetStringAttributeValue >(*this,lib,"LLVMGetStringAttributeValue",SideEffects::worstDefault,"LLVMGetStringAttributeValue")
		->args({"A","Length"});
// from D:\Work\libclang\include\llvm-c/Core.h:638:10
	addExtern< int (*)(LLVMOpaqueAttributeRef *) , LLVMIsEnumAttribute >(*this,lib,"LLVMIsEnumAttribute",SideEffects::worstDefault,"LLVMIsEnumAttribute")
		->args({"A"});
// from D:\Work\libclang\include\llvm-c/Core.h:639:10
	addExtern< int (*)(LLVMOpaqueAttributeRef *) , LLVMIsStringAttribute >(*this,lib,"LLVMIsStringAttribute",SideEffects::worstDefault,"LLVMIsStringAttribute")
		->args({"A"});
// from D:\Work\libclang\include\llvm-c/Core.h:640:10
	addExtern< int (*)(LLVMOpaqueAttributeRef *) , LLVMIsTypeAttribute >(*this,lib,"LLVMIsTypeAttribute",SideEffects::worstDefault,"LLVMIsTypeAttribute")
		->args({"A"});
// from D:\Work\libclang\include\llvm-c/Core.h:645:13
	addExtern< LLVMOpaqueType * (*)(LLVMOpaqueContext *,const char *) , LLVMGetTypeByName2 >(*this,lib,"LLVMGetTypeByName2",SideEffects::worstDefault,"LLVMGetTypeByName2")
		->args({"C","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:670:15
	addExtern< LLVMOpaqueModule * (*)(const char *) , LLVMModuleCreateWithName >(*this,lib,"LLVMModuleCreateWithName",SideEffects::worstDefault,"LLVMModuleCreateWithName")
		->args({"ModuleID"});
// from D:\Work\libclang\include\llvm-c/Core.h:678:15
	addExtern< LLVMOpaqueModule * (*)(const char *,LLVMOpaqueContext *) , LLVMModuleCreateWithNameInContext >(*this,lib,"LLVMModuleCreateWithNameInContext",SideEffects::worstDefault,"LLVMModuleCreateWithNameInContext")
		->args({"ModuleID","C"});
// from D:\Work\libclang\include\llvm-c/Core.h:683:15
	addExtern< LLVMOpaqueModule * (*)(LLVMOpaqueModule *) , LLVMCloneModule >(*this,lib,"LLVMCloneModule",SideEffects::worstDefault,"LLVMCloneModule")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:691:6
	addExtern< void (*)(LLVMOpaqueModule *) , LLVMDisposeModule >(*this,lib,"LLVMDisposeModule",SideEffects::worstDefault,"LLVMDisposeModule")
		->args({"M"});
}
}

