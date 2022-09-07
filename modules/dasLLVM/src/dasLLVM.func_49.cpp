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
void Module_dasLLVM::initFunctions_49() {
// from D:\Work\libclang\include\llvm-c/Orc.h:666:6
	addExtern< void (*)(LLVMOrcCSymbolFlagsMapPair *) , LLVMOrcDisposeCSymbolFlagsMap >(*this,lib,"LLVMOrcDisposeCSymbolFlagsMap",SideEffects::worstDefault,"LLVMOrcDisposeCSymbolFlagsMap")
		->args({"Pairs"});
// from D:\Work\libclang\include\llvm-c/Orc.h:677:1
	addExtern< LLVMOrcOpaqueSymbolStringPoolEntry * (*)(LLVMOrcOpaqueMaterializationResponsibility *) , LLVMOrcMaterializationResponsibilityGetInitializerSymbol >(*this,lib,"LLVMOrcMaterializationResponsibilityGetInitializerSymbol",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityGetInitializerSymbol")
		->args({"MR"});
// from D:\Work\libclang\include\llvm-c/Orc.h:687:1
	addExtern< LLVMOrcOpaqueSymbolStringPoolEntry ** (*)(LLVMOrcOpaqueMaterializationResponsibility *,size_t *) , LLVMOrcMaterializationResponsibilityGetRequestedSymbols >(*this,lib,"LLVMOrcMaterializationResponsibilityGetRequestedSymbols",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityGetRequestedSymbols")
		->args({"MR","NumSymbols"});
// from D:\Work\libclang\include\llvm-c/Orc.h:695:6
	addExtern< void (*)(LLVMOrcOpaqueSymbolStringPoolEntry **) , LLVMOrcDisposeSymbols >(*this,lib,"LLVMOrcDisposeSymbols",SideEffects::worstDefault,"LLVMOrcDisposeSymbols")
		->args({"Symbols"});
// from D:\Work\libclang\include\llvm-c/Orc.h:713:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueMaterializationResponsibility *,LLVMJITCSymbolMapPair *,size_t) , LLVMOrcMaterializationResponsibilityNotifyResolved >(*this,lib,"LLVMOrcMaterializationResponsibilityNotifyResolved",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityNotifyResolved")
		->args({"MR","Symbols","NumPairs"});
// from D:\Work\libclang\include\llvm-c/Orc.h:730:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueMaterializationResponsibility *) , LLVMOrcMaterializationResponsibilityNotifyEmitted >(*this,lib,"LLVMOrcMaterializationResponsibilityNotifyEmitted",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityNotifyEmitted")
		->args({"MR"});
// from D:\Work\libclang\include\llvm-c/Orc.h:746:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueMaterializationResponsibility *,LLVMOrcCSymbolFlagsMapPair *,size_t) , LLVMOrcMaterializationResponsibilityDefineMaterializing >(*this,lib,"LLVMOrcMaterializationResponsibilityDefineMaterializing",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityDefineMaterializing")
		->args({"MR","Pairs","NumPairs"});
// from D:\Work\libclang\include\llvm-c/Orc.h:757:6
	addExtern< void (*)(LLVMOrcOpaqueMaterializationResponsibility *) , LLVMOrcMaterializationResponsibilityFailMaterialization >(*this,lib,"LLVMOrcMaterializationResponsibilityFailMaterialization",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityFailMaterialization")
		->args({"MR"});
// from D:\Work\libclang\include\llvm-c/Orc.h:767:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueMaterializationResponsibility *,LLVMOrcOpaqueMaterializationUnit *) , LLVMOrcMaterializationResponsibilityReplace >(*this,lib,"LLVMOrcMaterializationResponsibilityReplace",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityReplace")
		->args({"MR","MU"});
// from D:\Work\libclang\include\llvm-c/Orc.h:779:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueMaterializationResponsibility *,LLVMOrcOpaqueSymbolStringPoolEntry **,size_t,LLVMOrcOpaqueMaterializationResponsibility **) , LLVMOrcMaterializationResponsibilityDelegate >(*this,lib,"LLVMOrcMaterializationResponsibilityDelegate",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityDelegate")
		->args({"MR","Symbols","NumSymbols","Result"});
// from D:\Work\libclang\include\llvm-c/Orc.h:802:6
	addExtern< void (*)(LLVMOrcOpaqueMaterializationResponsibility *,LLVMOrcOpaqueSymbolStringPoolEntry *,LLVMOrcCDependenceMapPair *,size_t) , LLVMOrcMaterializationResponsibilityAddDependencies >(*this,lib,"LLVMOrcMaterializationResponsibilityAddDependencies",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityAddDependencies")
		->args({"MR","Name","Dependencies","NumPairs"});
// from D:\Work\libclang\include\llvm-c/Orc.h:812:6
	addExtern< void (*)(LLVMOrcOpaqueMaterializationResponsibility *,LLVMOrcCDependenceMapPair *,size_t) , LLVMOrcMaterializationResponsibilityAddDependenciesForAll >(*this,lib,"LLVMOrcMaterializationResponsibilityAddDependenciesForAll",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityAddDependenciesForAll")
		->args({"MR","Dependencies","NumPairs"});
// from D:\Work\libclang\include\llvm-c/Orc.h:826:1
	addExtern< LLVMOrcOpaqueJITDylib * (*)(LLVMOrcOpaqueExecutionSession *,const char *) , LLVMOrcExecutionSessionCreateBareJITDylib >(*this,lib,"LLVMOrcExecutionSessionCreateBareJITDylib",SideEffects::worstDefault,"LLVMOrcExecutionSessionCreateBareJITDylib")
		->args({"ES","Name"});
// from D:\Work\libclang\include\llvm-c/Orc.h:842:1
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueExecutionSession *,LLVMOrcOpaqueJITDylib **,const char *) , LLVMOrcExecutionSessionCreateJITDylib >(*this,lib,"LLVMOrcExecutionSessionCreateJITDylib",SideEffects::worstDefault,"LLVMOrcExecutionSessionCreateJITDylib")
		->args({"ES","Result","Name"});
// from D:\Work\libclang\include\llvm-c/Orc.h:851:1
	addExtern< LLVMOrcOpaqueJITDylib * (*)(LLVMOrcOpaqueExecutionSession *,const char *) , LLVMOrcExecutionSessionGetJITDylibByName >(*this,lib,"LLVMOrcExecutionSessionGetJITDylibByName",SideEffects::worstDefault,"LLVMOrcExecutionSessionGetJITDylibByName")
		->args({"ES","Name"});
// from D:\Work\libclang\include\llvm-c/Orc.h:860:1
	addExtern< LLVMOrcOpaqueResourceTracker * (*)(LLVMOrcOpaqueJITDylib *) , LLVMOrcJITDylibCreateResourceTracker >(*this,lib,"LLVMOrcJITDylibCreateResourceTracker",SideEffects::worstDefault,"LLVMOrcJITDylibCreateResourceTracker")
		->args({"JD"});
// from D:\Work\libclang\include\llvm-c/Orc.h:868:1
	addExtern< LLVMOrcOpaqueResourceTracker * (*)(LLVMOrcOpaqueJITDylib *) , LLVMOrcJITDylibGetDefaultResourceTracker >(*this,lib,"LLVMOrcJITDylibGetDefaultResourceTracker",SideEffects::worstDefault,"LLVMOrcJITDylibGetDefaultResourceTracker")
		->args({"JD"});
// from D:\Work\libclang\include\llvm-c/Orc.h:877:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueJITDylib *,LLVMOrcOpaqueMaterializationUnit *) , LLVMOrcJITDylibDefine >(*this,lib,"LLVMOrcJITDylibDefine",SideEffects::worstDefault,"LLVMOrcJITDylibDefine")
		->args({"JD","MU"});
// from D:\Work\libclang\include\llvm-c/Orc.h:884:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueJITDylib *) , LLVMOrcJITDylibClear >(*this,lib,"LLVMOrcJITDylibClear",SideEffects::worstDefault,"LLVMOrcJITDylibClear")
		->args({"JD"});
// from D:\Work\libclang\include\llvm-c/Orc.h:892:6
	addExtern< void (*)(LLVMOrcOpaqueJITDylib *,LLVMOrcOpaqueDefinitionGenerator *) , LLVMOrcJITDylibAddGenerator >(*this,lib,"LLVMOrcJITDylibAddGenerator",SideEffects::worstDefault,"LLVMOrcJITDylibAddGenerator")
		->args({"JD","DG"});
}
}

