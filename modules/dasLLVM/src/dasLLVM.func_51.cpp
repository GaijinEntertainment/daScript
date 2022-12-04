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
void Module_dasLLVM::initFunctions_51() {
// from D:\Work\libclang\include\llvm-c/Orc.h:1133:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueDumpObjects *,LLVMOpaqueMemoryBuffer **) , LLVMOrcDumpObjects_CallOperator >(*this,lib,"LLVMOrcDumpObjects_CallOperator",SideEffects::worstDefault,"LLVMOrcDumpObjects_CallOperator")
		->args({"DumpObjects","ObjBuffer"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:67:24
	addExtern< LLVMOrcOpaqueLLJITBuilder * (*)() , LLVMOrcCreateLLJITBuilder >(*this,lib,"LLVMOrcCreateLLJITBuilder",SideEffects::worstDefault,"LLVMOrcCreateLLJITBuilder");
// from D:\Work\libclang\include\llvm-c/LLJIT.h:74:6
	addExtern< void (*)(LLVMOrcOpaqueLLJITBuilder *) , LLVMOrcDisposeLLJITBuilder >(*this,lib,"LLVMOrcDisposeLLJITBuilder",SideEffects::worstDefault,"LLVMOrcDisposeLLJITBuilder")
		->args({"Builder"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:85:6
	addExtern< void (*)(LLVMOrcOpaqueLLJITBuilder *,LLVMOrcOpaqueJITTargetMachineBuilder *) , LLVMOrcLLJITBuilderSetJITTargetMachineBuilder >(*this,lib,"LLVMOrcLLJITBuilderSetJITTargetMachineBuilder",SideEffects::worstDefault,"LLVMOrcLLJITBuilderSetJITTargetMachineBuilder")
		->args({"Builder","JTMB"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:109:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueLLJIT **,LLVMOrcOpaqueLLJITBuilder *) , LLVMOrcCreateLLJIT >(*this,lib,"LLVMOrcCreateLLJIT",SideEffects::worstDefault,"LLVMOrcCreateLLJIT")
		->args({"Result","Builder"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:115:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcDisposeLLJIT >(*this,lib,"LLVMOrcDisposeLLJIT",SideEffects::worstDefault,"LLVMOrcDisposeLLJIT")
		->args({"J"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:123:28
	addExtern< LLVMOrcOpaqueExecutionSession * (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcLLJITGetExecutionSession >(*this,lib,"LLVMOrcLLJITGetExecutionSession",SideEffects::worstDefault,"LLVMOrcLLJITGetExecutionSession")
		->args({"J"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:131:20
	addExtern< LLVMOrcOpaqueJITDylib * (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcLLJITGetMainJITDylib >(*this,lib,"LLVMOrcLLJITGetMainJITDylib",SideEffects::worstDefault,"LLVMOrcLLJITGetMainJITDylib")
		->args({"J"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:137:13
	addExtern< const char * (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcLLJITGetTripleString >(*this,lib,"LLVMOrcLLJITGetTripleString",SideEffects::worstDefault,"LLVMOrcLLJITGetTripleString")
		->args({"J"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:142:6
#ifndef __APPLE__
	addExtern< char (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcLLJITGetGlobalPrefix >(*this,lib,"LLVMOrcLLJITGetGlobalPrefix",SideEffects::worstDefault,"LLVMOrcLLJITGetGlobalPrefix")
		->args({"J"});
#endif
// from D:\Work\libclang\include\llvm-c/LLJIT.h:152:1
	addExtern< LLVMOrcOpaqueSymbolStringPoolEntry * (*)(LLVMOrcOpaqueLLJIT *,const char *) , LLVMOrcLLJITMangleAndIntern >(*this,lib,"LLVMOrcLLJITMangleAndIntern",SideEffects::worstDefault,"LLVMOrcLLJITMangleAndIntern")
		->args({"J","UnmangledName"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:163:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueLLJIT *,LLVMOrcOpaqueJITDylib *,LLVMOpaqueMemoryBuffer *) , LLVMOrcLLJITAddObjectFile >(*this,lib,"LLVMOrcLLJITAddObjectFile",SideEffects::worstDefault,"LLVMOrcLLJITAddObjectFile")
		->args({"J","JD","ObjBuffer"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:175:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueLLJIT *,LLVMOrcOpaqueResourceTracker *,LLVMOpaqueMemoryBuffer *) , LLVMOrcLLJITAddObjectFileWithRT >(*this,lib,"LLVMOrcLLJITAddObjectFileWithRT",SideEffects::worstDefault,"LLVMOrcLLJITAddObjectFileWithRT")
		->args({"J","RT","ObjBuffer"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:188:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueLLJIT *,LLVMOrcOpaqueJITDylib *,LLVMOrcOpaqueThreadSafeModule *) , LLVMOrcLLJITAddLLVMIRModule >(*this,lib,"LLVMOrcLLJITAddLLVMIRModule",SideEffects::worstDefault,"LLVMOrcLLJITAddLLVMIRModule")
		->args({"J","JD","TSM"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:201:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueLLJIT *,LLVMOrcOpaqueResourceTracker *,LLVMOrcOpaqueThreadSafeModule *) , LLVMOrcLLJITAddLLVMIRModuleWithRT >(*this,lib,"LLVMOrcLLJITAddLLVMIRModuleWithRT",SideEffects::worstDefault,"LLVMOrcLLJITAddLLVMIRModuleWithRT")
		->args({"J","JD","TSM"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:210:14
	addExtern< LLVMOpaqueError * (*)(LLVMOrcOpaqueLLJIT *,unsigned long long *,const char *) , LLVMOrcLLJITLookup >(*this,lib,"LLVMOrcLLJITLookup",SideEffects::worstDefault,"LLVMOrcLLJITLookup")
		->args({"J","Result","Name"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:217:23
	addExtern< LLVMOrcOpaqueObjectLayer * (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcLLJITGetObjLinkingLayer >(*this,lib,"LLVMOrcLLJITGetObjLinkingLayer",SideEffects::worstDefault,"LLVMOrcLLJITGetObjLinkingLayer")
		->args({"J"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:223:1
	addExtern< LLVMOrcOpaqueObjectTransformLayer * (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcLLJITGetObjTransformLayer >(*this,lib,"LLVMOrcLLJITGetObjTransformLayer",SideEffects::worstDefault,"LLVMOrcLLJITGetObjTransformLayer")
		->args({"J"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:228:28
	addExtern< LLVMOrcOpaqueIRTransformLayer * (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcLLJITGetIRTransformLayer >(*this,lib,"LLVMOrcLLJITGetIRTransformLayer",SideEffects::worstDefault,"LLVMOrcLLJITGetIRTransformLayer")
		->args({"J"});
// from D:\Work\libclang\include\llvm-c/LLJIT.h:236:13
	addExtern< const char * (*)(LLVMOrcOpaqueLLJIT *) , LLVMOrcLLJITGetDataLayoutStr >(*this,lib,"LLVMOrcLLJITGetDataLayoutStr",SideEffects::worstDefault,"LLVMOrcLLJITGetDataLayoutStr")
		->args({"J"});
}
}

