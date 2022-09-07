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
void Module_dasLLVM::initFunctions_48() {
// from D:\Work\libclang\include\llvm-c/Initialization.h:45:6
	addExtern< void (*)(LLVMOpaquePassRegistry *) , LLVMInitializeTarget >(*this,lib,"LLVMInitializeTarget",SideEffects::worstDefault,"LLVMInitializeTarget")
		->args({"R"});
// from D:\Work\libclang\include\llvm-c/IRReader.h:31:10
	addExtern< int (*)(LLVMOpaqueContext *,LLVMOpaqueMemoryBuffer *,LLVMOpaqueModule **,char **) , LLVMParseIRInContext >(*this,lib,"LLVMParseIRInContext",SideEffects::worstDefault,"LLVMParseIRInContext")
		->args({"ContextRef","MemBuf","OutM","OutMessage"});
// from D:\Work\libclang\include\llvm-c/Linker.h:34:10
	addExtern< int (*)(LLVMOpaqueModule *,LLVMOpaqueModule *) , LLVMLinkModules2 >(*this,lib,"LLVMLinkModules2",SideEffects::worstDefault,"LLVMLinkModules2")
		->args({"Dest","Src"});
// from D:\Work\libclang\include\llvm-c/Orc.h:461:1
	addExtern< LLVMOrcOpaqueSymbolStringPool * (*)(LLVMOrcOpaqueExecutionSession *) , LLVMOrcExecutionSessionGetSymbolStringPool >(*this,lib,"LLVMOrcExecutionSessionGetSymbolStringPool",SideEffects::worstDefault,"LLVMOrcExecutionSessionGetSymbolStringPool")
		->args({"ES"});
// from D:\Work\libclang\include\llvm-c/Orc.h:473:6
	addExtern< void (*)(LLVMOrcOpaqueSymbolStringPool *) , LLVMOrcSymbolStringPoolClearDeadEntries >(*this,lib,"LLVMOrcSymbolStringPoolClearDeadEntries",SideEffects::worstDefault,"LLVMOrcSymbolStringPoolClearDeadEntries")
		->args({"SSP"});
// from D:\Work\libclang\include\llvm-c/Orc.h:488:1
	addExtern< LLVMOrcOpaqueSymbolStringPoolEntry * (*)(LLVMOrcOpaqueExecutionSession *,const char *) , LLVMOrcExecutionSessionIntern >(*this,lib,"LLVMOrcExecutionSessionIntern",SideEffects::worstDefault,"LLVMOrcExecutionSessionIntern")
		->args({"ES","Name"});
// from D:\Work\libclang\include\llvm-c/Orc.h:493:6
	addExtern< void (*)(LLVMOrcOpaqueSymbolStringPoolEntry *) , LLVMOrcRetainSymbolStringPoolEntry >(*this,lib,"LLVMOrcRetainSymbolStringPoolEntry",SideEffects::worstDefault,"LLVMOrcRetainSymbolStringPoolEntry")
		->args({"S"});
// from D:\Work\libclang\include\llvm-c/Orc.h:498:6
	addExtern< void (*)(LLVMOrcOpaqueSymbolStringPoolEntry *) , LLVMOrcReleaseSymbolStringPoolEntry >(*this,lib,"LLVMOrcReleaseSymbolStringPoolEntry",SideEffects::worstDefault,"LLVMOrcReleaseSymbolStringPoolEntry")
		->args({"S"});
// from D:\Work\libclang\include\llvm-c/Orc.h:500:13
	addExtern< const char * (*)(LLVMOrcOpaqueSymbolStringPoolEntry *) , LLVMOrcSymbolStringPoolEntryStr >(*this,lib,"LLVMOrcSymbolStringPoolEntryStr",SideEffects::worstDefault,"LLVMOrcSymbolStringPoolEntryStr")
		->args({"S"});
// from D:\Work\libclang\include\llvm-c/Orc.h:505:6
	addExtern< void (*)(LLVMOrcOpaqueResourceTracker *) , LLVMOrcReleaseResourceTracker >(*this,lib,"LLVMOrcReleaseResourceTracker",SideEffects::worstDefault,"LLVMOrcReleaseResourceTracker")
		->args({"RT"});
// from D:\Work\libclang\include\llvm-c/Orc.h:511:6
	addExtern< void (*)(LLVMOrcOpaqueResourceTracker *,LLVMOrcOpaqueResourceTracker *) , LLVMOrcResourceTrackerTransferTo >(*this,lib,"LLVMOrcResourceTrackerTransferTo",SideEffects::worstDefault,"LLVMOrcResourceTrackerTransferTo")
		->args({"SrcRT","DstRT"});
// from D:\Work\libclang\include\llvm-c/Orc.h:518:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueResourceTracker *) , LLVMOrcResourceTrackerRemove >(*this,lib,"LLVMOrcResourceTrackerRemove",SideEffects::worstDefault,"LLVMOrcResourceTrackerRemove")
		->args({"RT"});
// from D:\Work\libclang\include\llvm-c/Orc.h:525:6
	addExtern< void (*)(LLVMOrcOpaqueDefinitionGenerator *) , LLVMOrcDisposeDefinitionGenerator >(*this,lib,"LLVMOrcDisposeDefinitionGenerator",SideEffects::worstDefault,"LLVMOrcDisposeDefinitionGenerator")
		->args({"DG"});
// from D:\Work\libclang\include\llvm-c/Orc.h:530:6
	addExtern< void (*)(LLVMOrcOpaqueMaterializationUnit *) , LLVMOrcDisposeMaterializationUnit >(*this,lib,"LLVMOrcDisposeMaterializationUnit",SideEffects::worstDefault,"LLVMOrcDisposeMaterializationUnit")
		->args({"MU"});
// from D:\Work\libclang\include\llvm-c/Orc.h:594:1
	addExtern< LLVMOrcOpaqueMaterializationUnit * (*)(LLVMJITCSymbolMapPair *,size_t) , LLVMOrcAbsoluteSymbols >(*this,lib,"LLVMOrcAbsoluteSymbols",SideEffects::worstDefault,"LLVMOrcAbsoluteSymbols")
		->args({"Syms","NumPairs"});
// from D:\Work\libclang\include\llvm-c/Orc.h:617:31
	addExtern< LLVMOrcOpaqueMaterializationUnit * (*)(LLVMOrcOpaqueLazyCallThroughManager *,LLVMOrcOpaqueIndirectStubsManager *,LLVMOrcOpaqueJITDylib *,LLVMOrcCSymbolAliasMapPair *,size_t) , LLVMOrcLazyReexports >(*this,lib,"LLVMOrcLazyReexports",SideEffects::worstDefault,"LLVMOrcLazyReexports")
		->args({"LCTM","ISM","SourceRef","CallableAliases","NumPairs"});
// from D:\Work\libclang\include\llvm-c/Orc.h:632:6
	addExtern< void (*)(LLVMOrcOpaqueMaterializationResponsibility *) , LLVMOrcDisposeMaterializationResponsibility >(*this,lib,"LLVMOrcDisposeMaterializationResponsibility",SideEffects::worstDefault,"LLVMOrcDisposeMaterializationResponsibility")
		->args({"MR"});
// from D:\Work\libclang\include\llvm-c/Orc.h:638:20
	addExtern< LLVMOrcOpaqueJITDylib * (*)(LLVMOrcOpaqueMaterializationResponsibility *) , LLVMOrcMaterializationResponsibilityGetTargetDylib >(*this,lib,"LLVMOrcMaterializationResponsibilityGetTargetDylib",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityGetTargetDylib")
		->args({"MR"});
// from D:\Work\libclang\include\llvm-c/Orc.h:645:1
	addExtern< LLVMOrcOpaqueExecutionSession * (*)(LLVMOrcOpaqueMaterializationResponsibility *) , LLVMOrcMaterializationResponsibilityGetExecutionSession >(*this,lib,"LLVMOrcMaterializationResponsibilityGetExecutionSession",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityGetExecutionSession")
		->args({"MR"});
// from D:\Work\libclang\include\llvm-c/Orc.h:658:29
	addExtern< LLVMOrcCSymbolFlagsMapPair * (*)(LLVMOrcOpaqueMaterializationResponsibility *,size_t *) , LLVMOrcMaterializationResponsibilityGetSymbols >(*this,lib,"LLVMOrcMaterializationResponsibilityGetSymbols",SideEffects::worstDefault,"LLVMOrcMaterializationResponsibilityGetSymbols")
		->args({"MR","NumPairs"});
}
}

