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
void Module_dasLLVM::initFunctions_4() {
// from D:\Work\libclang\include\llvm-c/Core.h:701:13
	addExtern< const char * (*)(LLVMOpaqueModule *,size_t *) , LLVMGetModuleIdentifier >(*this,lib,"LLVMGetModuleIdentifier",SideEffects::worstDefault,"LLVMGetModuleIdentifier")
		->args({"M","Len"});
// from D:\Work\libclang\include\llvm-c/Core.h:711:6
	addExtern< void (*)(LLVMOpaqueModule *,const char *,size_t) , LLVMSetModuleIdentifier >(*this,lib,"LLVMSetModuleIdentifier",SideEffects::worstDefault,"LLVMSetModuleIdentifier")
		->args({"M","Ident","Len"});
// from D:\Work\libclang\include\llvm-c/Core.h:721:13
	addExtern< const char * (*)(LLVMOpaqueModule *,size_t *) , LLVMGetSourceFileName >(*this,lib,"LLVMGetSourceFileName",SideEffects::worstDefault,"LLVMGetSourceFileName")
		->args({"M","Len"});
// from D:\Work\libclang\include\llvm-c/Core.h:732:6
	addExtern< void (*)(LLVMOpaqueModule *,const char *,size_t) , LLVMSetSourceFileName >(*this,lib,"LLVMSetSourceFileName",SideEffects::worstDefault,"LLVMSetSourceFileName")
		->args({"M","Name","Len"});
// from D:\Work\libclang\include\llvm-c/Core.h:743:13
	addExtern< const char * (*)(LLVMOpaqueModule *) , LLVMGetDataLayoutStr >(*this,lib,"LLVMGetDataLayoutStr",SideEffects::worstDefault,"LLVMGetDataLayoutStr")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:744:13
	addExtern< const char * (*)(LLVMOpaqueModule *) , LLVMGetDataLayout >(*this,lib,"LLVMGetDataLayout",SideEffects::worstDefault,"LLVMGetDataLayout")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:751:6
	addExtern< void (*)(LLVMOpaqueModule *,const char *) , LLVMSetDataLayout >(*this,lib,"LLVMSetDataLayout",SideEffects::worstDefault,"LLVMSetDataLayout")
		->args({"M","DataLayoutStr"});
// from D:\Work\libclang\include\llvm-c/Core.h:758:13
	addExtern< const char * (*)(LLVMOpaqueModule *) , LLVMGetTarget >(*this,lib,"LLVMGetTarget",SideEffects::worstDefault,"LLVMGetTarget")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:765:6
	addExtern< void (*)(LLVMOpaqueModule *,const char *) , LLVMSetTarget >(*this,lib,"LLVMSetTarget",SideEffects::worstDefault,"LLVMSetTarget")
		->args({"M","Triple"});
// from D:\Work\libclang\include\llvm-c/Core.h:774:22
	addExtern< LLVMOpaqueModuleFlagEntry * (*)(LLVMOpaqueModule *,size_t *) , LLVMCopyModuleFlagsMetadata >(*this,lib,"LLVMCopyModuleFlagsMetadata",SideEffects::worstDefault,"LLVMCopyModuleFlagsMetadata")
		->args({"M","Len"});
// from D:\Work\libclang\include\llvm-c/Core.h:779:6
	addExtern< void (*)(LLVMOpaqueModuleFlagEntry *) , LLVMDisposeModuleFlagsMetadata >(*this,lib,"LLVMDisposeModuleFlagsMetadata",SideEffects::worstDefault,"LLVMDisposeModuleFlagsMetadata")
		->args({"Entries"});
// from D:\Work\libclang\include\llvm-c/Core.h:787:1
	addExtern< LLVMModuleFlagBehavior (*)(LLVMOpaqueModuleFlagEntry *,unsigned int) , LLVMModuleFlagEntriesGetFlagBehavior >(*this,lib,"LLVMModuleFlagEntriesGetFlagBehavior",SideEffects::worstDefault,"LLVMModuleFlagEntriesGetFlagBehavior")
		->args({"Entries","Index"});
// from D:\Work\libclang\include\llvm-c/Core.h:795:13
	addExtern< const char * (*)(LLVMOpaqueModuleFlagEntry *,unsigned int,size_t *) , LLVMModuleFlagEntriesGetKey >(*this,lib,"LLVMModuleFlagEntriesGetKey",SideEffects::worstDefault,"LLVMModuleFlagEntriesGetKey")
		->args({"Entries","Index","Len"});
// from D:\Work\libclang\include\llvm-c/Core.h:803:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueModuleFlagEntry *,unsigned int) , LLVMModuleFlagEntriesGetMetadata >(*this,lib,"LLVMModuleFlagEntriesGetMetadata",SideEffects::worstDefault,"LLVMModuleFlagEntriesGetMetadata")
		->args({"Entries","Index"});
// from D:\Work\libclang\include\llvm-c/Core.h:812:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueModule *,const char *,size_t) , LLVMGetModuleFlag >(*this,lib,"LLVMGetModuleFlag",SideEffects::worstDefault,"LLVMGetModuleFlag")
		->args({"M","Key","KeyLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:821:6
	addExtern< void (*)(LLVMOpaqueModule *,LLVMModuleFlagBehavior,const char *,size_t,LLVMOpaqueMetadata *) , LLVMAddModuleFlag >(*this,lib,"LLVMAddModuleFlag",SideEffects::worstDefault,"LLVMAddModuleFlag")
		->args({"M","Behavior","Key","KeyLen","Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:830:6
	addExtern< void (*)(LLVMOpaqueModule *) , LLVMDumpModule >(*this,lib,"LLVMDumpModule",SideEffects::worstDefault,"LLVMDumpModule")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:838:10
	addExtern< int (*)(LLVMOpaqueModule *,const char *,char **) , LLVMPrintModuleToFile >(*this,lib,"LLVMPrintModuleToFile",SideEffects::worstDefault,"LLVMPrintModuleToFile")
		->args({"M","Filename","ErrorMessage"});
// from D:\Work\libclang\include\llvm-c/Core.h:847:7
	addExtern< char * (*)(LLVMOpaqueModule *) , LLVMPrintModuleToString >(*this,lib,"LLVMPrintModuleToString",SideEffects::worstDefault,"LLVMPrintModuleToString")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:854:13
	addExtern< const char * (*)(LLVMOpaqueModule *,size_t *) , LLVMGetModuleInlineAsm >(*this,lib,"LLVMGetModuleInlineAsm",SideEffects::worstDefault,"LLVMGetModuleInlineAsm")
		->args({"M","Len"});
}
}

