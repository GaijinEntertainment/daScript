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
void Module_dasLLVM::initFunctions_59() {
// from D:\Work\libclang\include\llvm-c/Support.h:39:6
	addExtern< void (*)(int,const char *const *,const char *) , LLVMParseCommandLineOptions >(*this,lib,"LLVMParseCommandLineOptions",SideEffects::worstDefault,"LLVMParseCommandLineOptions")
		->args({"argc","argv","Overview"});
// from D:\Work\libclang\include\llvm-c/Support.h:49:7
	addExtern< void * (*)(const char *) , LLVMSearchForAddressOfSymbol >(*this,lib,"LLVMSearchForAddressOfSymbol",SideEffects::worstDefault,"LLVMSearchForAddressOfSymbol")
		->args({"symbolName"});
// from D:\Work\libclang\include\llvm-c/Support.h:58:6
	addExtern< void (*)(const char *,void *) , LLVMAddSymbol >(*this,lib,"LLVMAddSymbol",SideEffects::worstDefault,"LLVMAddSymbol")
		->args({"symbolName","symbolValue"});
// from D:\Work\libclang\include\llvm-c/Transforms/AggressiveInstCombine.h:31:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddAggressiveInstCombinerPass >(*this,lib,"LLVMAddAggressiveInstCombinerPass",SideEffects::worstDefault,"LLVMAddAggressiveInstCombinerPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:32:27
	addExtern< LLVMOpaquePassManagerBuilder * (*)() , LLVMPassManagerBuilderCreate >(*this,lib,"LLVMPassManagerBuilderCreate",SideEffects::worstDefault,"LLVMPassManagerBuilderCreate");
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:33:6
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *) , LLVMPassManagerBuilderDispose >(*this,lib,"LLVMPassManagerBuilderDispose",SideEffects::worstDefault,"LLVMPassManagerBuilderDispose")
		->args({"PMB"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:37:1
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,unsigned int) , LLVMPassManagerBuilderSetOptLevel >(*this,lib,"LLVMPassManagerBuilderSetOptLevel",SideEffects::worstDefault,"LLVMPassManagerBuilderSetOptLevel")
		->args({"PMB","OptLevel"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:42:1
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,unsigned int) , LLVMPassManagerBuilderSetSizeLevel >(*this,lib,"LLVMPassManagerBuilderSetSizeLevel",SideEffects::worstDefault,"LLVMPassManagerBuilderSetSizeLevel")
		->args({"PMB","SizeLevel"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:47:1
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,int) , LLVMPassManagerBuilderSetDisableUnitAtATime >(*this,lib,"LLVMPassManagerBuilderSetDisableUnitAtATime",SideEffects::worstDefault,"LLVMPassManagerBuilderSetDisableUnitAtATime")
		->args({"PMB","Value"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:52:1
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,int) , LLVMPassManagerBuilderSetDisableUnrollLoops >(*this,lib,"LLVMPassManagerBuilderSetDisableUnrollLoops",SideEffects::worstDefault,"LLVMPassManagerBuilderSetDisableUnrollLoops")
		->args({"PMB","Value"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:57:1
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,int) , LLVMPassManagerBuilderSetDisableSimplifyLibCalls >(*this,lib,"LLVMPassManagerBuilderSetDisableSimplifyLibCalls",SideEffects::worstDefault,"LLVMPassManagerBuilderSetDisableSimplifyLibCalls")
		->args({"PMB","Value"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:62:1
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,unsigned int) , LLVMPassManagerBuilderUseInlinerWithThreshold >(*this,lib,"LLVMPassManagerBuilderUseInlinerWithThreshold",SideEffects::worstDefault,"LLVMPassManagerBuilderUseInlinerWithThreshold")
		->args({"PMB","Threshold"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:67:1
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,LLVMOpaquePassManager *) , LLVMPassManagerBuilderPopulateFunctionPassManager >(*this,lib,"LLVMPassManagerBuilderPopulateFunctionPassManager",SideEffects::worstDefault,"LLVMPassManagerBuilderPopulateFunctionPassManager")
		->args({"PMB","PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:72:1
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,LLVMOpaquePassManager *) , LLVMPassManagerBuilderPopulateModulePassManager >(*this,lib,"LLVMPassManagerBuilderPopulateModulePassManager",SideEffects::worstDefault,"LLVMPassManagerBuilderPopulateModulePassManager")
		->args({"PMB","PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/PassManagerBuilder.h:76:6
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *,LLVMOpaquePassManager *,int,int) , LLVMPassManagerBuilderPopulateLTOPassManager >(*this,lib,"LLVMPassManagerBuilderPopulateLTOPassManager",SideEffects::worstDefault,"LLVMPassManagerBuilderPopulateLTOPassManager")
		->args({"PMB","PM","Internalize","RunInliner"});
// from D:\Work\libclang\include\llvm-c/Transforms/Coroutines.h:36:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddCoroEarlyPass >(*this,lib,"LLVMAddCoroEarlyPass",SideEffects::worstDefault,"LLVMAddCoroEarlyPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Coroutines.h:39:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddCoroSplitPass >(*this,lib,"LLVMAddCoroSplitPass",SideEffects::worstDefault,"LLVMAddCoroSplitPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Coroutines.h:42:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddCoroElidePass >(*this,lib,"LLVMAddCoroElidePass",SideEffects::worstDefault,"LLVMAddCoroElidePass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Coroutines.h:45:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMAddCoroCleanupPass >(*this,lib,"LLVMAddCoroCleanupPass",SideEffects::worstDefault,"LLVMAddCoroCleanupPass")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Transforms/Coroutines.h:48:6
	addExtern< void (*)(LLVMOpaquePassManagerBuilder *) , LLVMPassManagerBuilderAddCoroutinePassesToExtensionPoints >(*this,lib,"LLVMPassManagerBuilderAddCoroutinePassesToExtensionPoints",SideEffects::worstDefault,"LLVMPassManagerBuilderAddCoroutinePassesToExtensionPoints")
		->args({"PMB"});
}
}

