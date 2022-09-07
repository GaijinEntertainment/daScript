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
void Module_dasLLVM::initFunctions_37() {
// from D:\Work\libclang\include\llvm-c/Core.h:4076:6
	addExtern< void (*)(LLVMOpaqueMemoryBuffer *) , LLVMDisposeMemoryBuffer >(*this,lib,"LLVMDisposeMemoryBuffer",SideEffects::worstDefault,"LLVMDisposeMemoryBuffer")
		->args({"MemBuf"});
// from D:\Work\libclang\include\llvm-c/Core.h:4090:21
	addExtern< LLVMOpaquePassRegistry * (*)() , LLVMGetGlobalPassRegistry >(*this,lib,"LLVMGetGlobalPassRegistry",SideEffects::worstDefault,"LLVMGetGlobalPassRegistry");
// from D:\Work\libclang\include\llvm-c/Core.h:4105:20
	addExtern< LLVMOpaquePassManager * (*)() , LLVMCreatePassManager >(*this,lib,"LLVMCreatePassManager",SideEffects::worstDefault,"LLVMCreatePassManager");
// from D:\Work\libclang\include\llvm-c/Core.h:4111:20
	addExtern< LLVMOpaquePassManager * (*)(LLVMOpaqueModule *) , LLVMCreateFunctionPassManagerForModule >(*this,lib,"LLVMCreateFunctionPassManagerForModule",SideEffects::worstDefault,"LLVMCreateFunctionPassManagerForModule")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:4114:20
	addExtern< LLVMOpaquePassManager * (*)(LLVMOpaqueModuleProvider *) , LLVMCreateFunctionPassManager >(*this,lib,"LLVMCreateFunctionPassManager",SideEffects::worstDefault,"LLVMCreateFunctionPassManager")
		->args({"MP"});
// from D:\Work\libclang\include\llvm-c/Core.h:4120:10
	addExtern< int (*)(LLVMOpaquePassManager *,LLVMOpaqueModule *) , LLVMRunPassManager >(*this,lib,"LLVMRunPassManager",SideEffects::worstDefault,"LLVMRunPassManager")
		->args({"PM","M"});
// from D:\Work\libclang\include\llvm-c/Core.h:4125:10
	addExtern< int (*)(LLVMOpaquePassManager *) , LLVMInitializeFunctionPassManager >(*this,lib,"LLVMInitializeFunctionPassManager",SideEffects::worstDefault,"LLVMInitializeFunctionPassManager")
		->args({"FPM"});
// from D:\Work\libclang\include\llvm-c/Core.h:4131:10
	addExtern< int (*)(LLVMOpaquePassManager *,LLVMOpaqueValue *) , LLVMRunFunctionPassManager >(*this,lib,"LLVMRunFunctionPassManager",SideEffects::worstDefault,"LLVMRunFunctionPassManager")
		->args({"FPM","F"});
// from D:\Work\libclang\include\llvm-c/Core.h:4136:10
	addExtern< int (*)(LLVMOpaquePassManager *) , LLVMFinalizeFunctionPassManager >(*this,lib,"LLVMFinalizeFunctionPassManager",SideEffects::worstDefault,"LLVMFinalizeFunctionPassManager")
		->args({"FPM"});
// from D:\Work\libclang\include\llvm-c/Core.h:4141:6
	addExtern< void (*)(LLVMOpaquePassManager *) , LLVMDisposePassManager >(*this,lib,"LLVMDisposePassManager",SideEffects::worstDefault,"LLVMDisposePassManager")
		->args({"PM"});
// from D:\Work\libclang\include\llvm-c/Core.h:4158:10
	addExtern< int (*)() , LLVMStartMultithreaded >(*this,lib,"LLVMStartMultithreaded",SideEffects::worstDefault,"LLVMStartMultithreaded");
// from D:\Work\libclang\include\llvm-c/Core.h:4162:6
	addExtern< void (*)() , LLVMStopMultithreaded >(*this,lib,"LLVMStopMultithreaded",SideEffects::worstDefault,"LLVMStopMultithreaded");
// from D:\Work\libclang\include\llvm-c/Core.h:4166:10
	addExtern< int (*)() , LLVMIsMultithreaded >(*this,lib,"LLVMIsMultithreaded",SideEffects::worstDefault,"LLVMIsMultithreaded");
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:190:10
	addExtern< unsigned int (*)() , LLVMDebugMetadataVersion >(*this,lib,"LLVMDebugMetadataVersion",SideEffects::worstDefault,"LLVMDebugMetadataVersion");
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:195:10
	addExtern< unsigned int (*)(LLVMOpaqueModule *) , LLVMGetModuleDebugMetadataVersion >(*this,lib,"LLVMGetModuleDebugMetadataVersion",SideEffects::worstDefault,"LLVMGetModuleDebugMetadataVersion")
		->args({"Module"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:203:10
	addExtern< int (*)(LLVMOpaqueModule *) , LLVMStripModuleDebugInfo >(*this,lib,"LLVMStripModuleDebugInfo",SideEffects::worstDefault,"LLVMStripModuleDebugInfo")
		->args({"Module"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:209:18
	addExtern< LLVMOpaqueDIBuilder * (*)(LLVMOpaqueModule *) , LLVMCreateDIBuilderDisallowUnresolved >(*this,lib,"LLVMCreateDIBuilderDisallowUnresolved",SideEffects::worstDefault,"LLVMCreateDIBuilderDisallowUnresolved")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:216:18
	addExtern< LLVMOpaqueDIBuilder * (*)(LLVMOpaqueModule *) , LLVMCreateDIBuilder >(*this,lib,"LLVMCreateDIBuilder",SideEffects::worstDefault,"LLVMCreateDIBuilder")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:222:6
	addExtern< void (*)(LLVMOpaqueDIBuilder *) , LLVMDisposeDIBuilder >(*this,lib,"LLVMDisposeDIBuilder",SideEffects::worstDefault,"LLVMDisposeDIBuilder")
		->args({"Builder"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:227:6
	addExtern< void (*)(LLVMOpaqueDIBuilder *) , LLVMDIBuilderFinalize >(*this,lib,"LLVMDIBuilderFinalize",SideEffects::worstDefault,"LLVMDIBuilderFinalize")
		->args({"Builder"});
}
}

