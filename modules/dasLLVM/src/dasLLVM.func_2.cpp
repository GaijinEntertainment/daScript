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
void Module_dasLLVM::initFunctions_2() {
// from D:\Work\libclang\include\llvm-c/BitWriter.h:45:5
	addExtern< int (*)(LLVMOpaqueModule *,int) , LLVMWriteBitcodeToFileHandle >(*this,lib,"LLVMWriteBitcodeToFileHandle",SideEffects::worstDefault,"LLVMWriteBitcodeToFileHandle")
		->args({"M","Handle"});
// from D:\Work\libclang\include\llvm-c/BitWriter.h:48:21
	addExtern< LLVMOpaqueMemoryBuffer * (*)(LLVMOpaqueModule *) , LLVMWriteBitcodeToMemoryBuffer >(*this,lib,"LLVMWriteBitcodeToMemoryBuffer",SideEffects::worstDefault,"LLVMWriteBitcodeToMemoryBuffer")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Comdat.h:39:15
	addExtern< LLVMComdat * (*)(LLVMOpaqueModule *,const char *) , LLVMGetOrInsertComdat >(*this,lib,"LLVMGetOrInsertComdat",SideEffects::worstDefault,"LLVMGetOrInsertComdat")
		->args({"M","Name"});
// from D:\Work\libclang\include\llvm-c/Comdat.h:46:15
	addExtern< LLVMComdat * (*)(LLVMOpaqueValue *) , LLVMGetComdat >(*this,lib,"LLVMGetComdat",SideEffects::worstDefault,"LLVMGetComdat")
		->args({"V"});
// from D:\Work\libclang\include\llvm-c/Comdat.h:53:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMComdat *) , LLVMSetComdat >(*this,lib,"LLVMSetComdat",SideEffects::worstDefault,"LLVMSetComdat")
		->args({"V","C"});
// from D:\Work\libclang\include\llvm-c/Comdat.h:60:25
	addExtern< LLVMComdatSelectionKind (*)(LLVMComdat *) , LLVMGetComdatSelectionKind >(*this,lib,"LLVMGetComdatSelectionKind",SideEffects::worstDefault,"LLVMGetComdatSelectionKind")
		->args({"C"});
// from D:\Work\libclang\include\llvm-c/Comdat.h:67:6
	addExtern< void (*)(LLVMComdat *,LLVMComdatSelectionKind) , LLVMSetComdatSelectionKind >(*this,lib,"LLVMSetComdatSelectionKind",SideEffects::worstDefault,"LLVMSetComdatSelectionKind")
		->args({"C","Kind"});
// from D:\Work\libclang\include\llvm-c/ErrorHandling.h:36:6
	addExtern< void (*)() , LLVMResetFatalErrorHandler >(*this,lib,"LLVMResetFatalErrorHandler",SideEffects::worstDefault,"LLVMResetFatalErrorHandler");
// from D:\Work\libclang\include\llvm-c/ErrorHandling.h:43:6
	addExtern< void (*)() , LLVMEnablePrettyStackTrace >(*this,lib,"LLVMEnablePrettyStackTrace",SideEffects::worstDefault,"LLVMEnablePrettyStackTrace");
// from D:\Work\libclang\include\llvm-c/Core.h:472:6
	addExtern< void (*)() , LLVMShutdown >(*this,lib,"LLVMShutdown",SideEffects::worstDefault,"LLVMShutdown");
// from D:\Work\libclang\include\llvm-c/Core.h:476:7
	addExtern< char * (*)(const char *) , LLVMCreateMessage >(*this,lib,"LLVMCreateMessage",SideEffects::worstDefault,"LLVMCreateMessage")
		->args({"Message"});
// from D:\Work\libclang\include\llvm-c/Core.h:477:6
	addExtern< void (*)(char *) , LLVMDisposeMessage >(*this,lib,"LLVMDisposeMessage",SideEffects::worstDefault,"LLVMDisposeMessage")
		->args({"Message"});
// from D:\Work\libclang\include\llvm-c/Core.h:500:16
	addExtern< LLVMOpaqueContext * (*)() , LLVMContextCreate >(*this,lib,"LLVMContextCreate",SideEffects::worstDefault,"LLVMContextCreate");
// from D:\Work\libclang\include\llvm-c/Core.h:505:16
	addExtern< LLVMOpaqueContext * (*)() , LLVMGetGlobalContext >(*this,lib,"LLVMGetGlobalContext",SideEffects::worstDefault,"LLVMGetGlobalContext");
// from D:\Work\libclang\include\llvm-c/Core.h:522:7
	addExtern< void * (*)(LLVMOpaqueContext *) , LLVMContextGetDiagnosticContext >(*this,lib,"LLVMContextGetDiagnosticContext",SideEffects::worstDefault,"LLVMContextGetDiagnosticContext")
		->args({"C"});
// from D:\Work\libclang\include\llvm-c/Core.h:537:10
	addExtern< int (*)(LLVMOpaqueContext *) , LLVMContextShouldDiscardValueNames >(*this,lib,"LLVMContextShouldDiscardValueNames",SideEffects::worstDefault,"LLVMContextShouldDiscardValueNames")
		->args({"C"});
// from D:\Work\libclang\include\llvm-c/Core.h:547:6
	addExtern< void (*)(LLVMOpaqueContext *,int) , LLVMContextSetDiscardValueNames >(*this,lib,"LLVMContextSetDiscardValueNames",SideEffects::worstDefault,"LLVMContextSetDiscardValueNames")
		->args({"C","Discard"});
// from D:\Work\libclang\include\llvm-c/Core.h:555:6
	addExtern< void (*)(LLVMOpaqueContext *) , LLVMContextDispose >(*this,lib,"LLVMContextDispose",SideEffects::worstDefault,"LLVMContextDispose")
		->args({"C"});
// from D:\Work\libclang\include\llvm-c/Core.h:563:7
	addExtern< char * (*)(LLVMOpaqueDiagnosticInfo *) , LLVMGetDiagInfoDescription >(*this,lib,"LLVMGetDiagInfoDescription",SideEffects::worstDefault,"LLVMGetDiagInfoDescription")
		->args({"DI"});
// from D:\Work\libclang\include\llvm-c/Core.h:570:24
	addExtern< LLVMDiagnosticSeverity (*)(LLVMOpaqueDiagnosticInfo *) , LLVMGetDiagInfoSeverity >(*this,lib,"LLVMGetDiagInfoSeverity",SideEffects::worstDefault,"LLVMGetDiagInfoSeverity")
		->args({"DI"});
}
}

