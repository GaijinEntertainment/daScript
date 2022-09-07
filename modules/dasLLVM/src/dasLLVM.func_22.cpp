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
void Module_dasLLVM::initFunctions_22() {
// from D:\Work\libclang\include\llvm-c/Core.h:2390:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *,const char *,size_t) , LLVMGetNamedGlobalAlias >(*this,lib,"LLVMGetNamedGlobalAlias",SideEffects::worstDefault,"LLVMGetNamedGlobalAlias")
		->args({"M","Name","NameLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:2398:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *) , LLVMGetFirstGlobalAlias >(*this,lib,"LLVMGetFirstGlobalAlias",SideEffects::worstDefault,"LLVMGetFirstGlobalAlias")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:2405:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *) , LLVMGetLastGlobalAlias >(*this,lib,"LLVMGetLastGlobalAlias",SideEffects::worstDefault,"LLVMGetLastGlobalAlias")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:2413:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetNextGlobalAlias >(*this,lib,"LLVMGetNextGlobalAlias",SideEffects::worstDefault,"LLVMGetNextGlobalAlias")
		->args({"GA"});
// from D:\Work\libclang\include\llvm-c/Core.h:2421:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetPreviousGlobalAlias >(*this,lib,"LLVMGetPreviousGlobalAlias",SideEffects::worstDefault,"LLVMGetPreviousGlobalAlias")
		->args({"GA"});
// from D:\Work\libclang\include\llvm-c/Core.h:2426:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMAliasGetAliasee >(*this,lib,"LLVMAliasGetAliasee",SideEffects::worstDefault,"LLVMAliasGetAliasee")
		->args({"Alias"});
// from D:\Work\libclang\include\llvm-c/Core.h:2431:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMAliasSetAliasee >(*this,lib,"LLVMAliasSetAliasee",SideEffects::worstDefault,"LLVMAliasSetAliasee")
		->args({"Alias","Aliasee"});
// from D:\Work\libclang\include\llvm-c/Core.h:2453:6
	addExtern< void (*)(LLVMOpaqueValue *) , LLVMDeleteFunction >(*this,lib,"LLVMDeleteFunction",SideEffects::worstDefault,"LLVMDeleteFunction")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2460:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMHasPersonalityFn >(*this,lib,"LLVMHasPersonalityFn",SideEffects::worstDefault,"LLVMHasPersonalityFn")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2467:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetPersonalityFn >(*this,lib,"LLVMGetPersonalityFn",SideEffects::worstDefault,"LLVMGetPersonalityFn")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2474:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMSetPersonalityFn >(*this,lib,"LLVMSetPersonalityFn",SideEffects::worstDefault,"LLVMSetPersonalityFn")
		->args({"Fn","PersonalityFn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2481:10
	addExtern< unsigned int (*)(const char *,size_t) , LLVMLookupIntrinsicID >(*this,lib,"LLVMLookupIntrinsicID",SideEffects::worstDefault,"LLVMLookupIntrinsicID")
		->args({"Name","NameLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:2488:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetIntrinsicID >(*this,lib,"LLVMGetIntrinsicID",SideEffects::worstDefault,"LLVMGetIntrinsicID")
		->args({"Fn"});
// from D:\Work\libclang\include\llvm-c/Core.h:2496:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *,unsigned int,LLVMOpaqueType **,size_t) , LLVMGetIntrinsicDeclaration >(*this,lib,"LLVMGetIntrinsicDeclaration",SideEffects::worstDefault,"LLVMGetIntrinsicDeclaration")
		->args({"Mod","ID","ParamTypes","ParamCount"});
// from D:\Work\libclang\include\llvm-c/Core.h:2507:13
	addExtern< LLVMOpaqueType * (*)(LLVMOpaqueContext *,unsigned int,LLVMOpaqueType **,size_t) , LLVMIntrinsicGetType >(*this,lib,"LLVMIntrinsicGetType",SideEffects::worstDefault,"LLVMIntrinsicGetType")
		->args({"Ctx","ID","ParamTypes","ParamCount"});
// from D:\Work\libclang\include\llvm-c/Core.h:2515:13
	addExtern< const char * (*)(unsigned int,size_t *) , LLVMIntrinsicGetName >(*this,lib,"LLVMIntrinsicGetName",SideEffects::worstDefault,"LLVMIntrinsicGetName")
		->args({"ID","NameLength"});
// from D:\Work\libclang\include\llvm-c/Core.h:2518:13
	addExtern< const char * (*)(unsigned int,LLVMOpaqueType **,size_t,size_t *) , LLVMIntrinsicCopyOverloadedName >(*this,lib,"LLVMIntrinsicCopyOverloadedName",SideEffects::worstDefault,"LLVMIntrinsicCopyOverloadedName")
		->args({"ID","ParamTypes","ParamCount","NameLength"});
// from D:\Work\libclang\include\llvm-c/Core.h:2534:13
	addExtern< const char * (*)(LLVMOpaqueModule *,unsigned int,LLVMOpaqueType **,size_t,size_t *) , LLVMIntrinsicCopyOverloadedName2 >(*this,lib,"LLVMIntrinsicCopyOverloadedName2",SideEffects::worstDefault,"LLVMIntrinsicCopyOverloadedName2")
		->args({"Mod","ID","ParamTypes","ParamCount","NameLength"});
// from D:\Work\libclang\include\llvm-c/Core.h:2544:10
	addExtern< int (*)(unsigned int) , LLVMIntrinsicIsOverloaded >(*this,lib,"LLVMIntrinsicIsOverloaded",SideEffects::worstDefault,"LLVMIntrinsicIsOverloaded")
		->args({"ID"});
// from D:\Work\libclang\include\llvm-c/Core.h:2553:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetFunctionCallConv >(*this,lib,"LLVMGetFunctionCallConv",SideEffects::worstDefault,"LLVMGetFunctionCallConv")
		->args({"Fn"});
}
}

