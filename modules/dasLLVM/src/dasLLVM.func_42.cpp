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
void Module_dasLLVM::initFunctions_42() {
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:1361:18
	addExtern< unsigned int (*)(LLVMOpaqueMetadata *) , LLVMGetMetadataKind >(*this,lib,"LLVMGetMetadataKind",SideEffects::worstDefault,"LLVMGetMetadataKind")
		->args({"Metadata"});
// from D:\Work\libclang\include\llvm-c/Disassembler.h:72:5
	addExtern< int (*)(void *,uint64_t) , LLVMSetDisasmOptions >(*this,lib,"LLVMSetDisasmOptions",SideEffects::worstDefault,"LLVMSetDisasmOptions")
		->args({"DC","Options"});
// from D:\Work\libclang\include\llvm-c/Disassembler.h:88:6
	addExtern< void (*)(void *) , LLVMDisasmDispose >(*this,lib,"LLVMDisasmDispose",SideEffects::worstDefault,"LLVMDisasmDispose")
		->args({"DC"});
// from D:\Work\libclang\include\llvm-c/Disassembler.h:100:8
	addExtern< size_t (*)(void *,unsigned char *,uint64_t,uint64_t,char *,size_t) , LLVMDisasmInstruction >(*this,lib,"LLVMDisasmInstruction",SideEffects::worstDefault,"LLVMDisasmInstruction")
		->args({"DC","Bytes","BytesSize","PC","OutString","OutStringSize"});
// from D:\Work\libclang\include\llvm-c/Error.h:37:17
	addExtern< const void * (*)(LLVMOpaqueError *) , LLVMGetErrorTypeId >(*this,lib,"LLVMGetErrorTypeId",SideEffects::worstDefault,"LLVMGetErrorTypeId")
		->args({"Err"});
// from D:\Work\libclang\include\llvm-c/Error.h:45:6
	addExtern< void (*)(LLVMOpaqueError *) , LLVMConsumeError >(*this,lib,"LLVMConsumeError",SideEffects::worstDefault,"LLVMConsumeError")
		->args({"Err"});
// from D:\Work\libclang\include\llvm-c/Error.h:53:7
	addExtern< char * (*)(LLVMOpaqueError *) , LLVMGetErrorMessage >(*this,lib,"LLVMGetErrorMessage",SideEffects::worstDefault,"LLVMGetErrorMessage")
		->args({"Err"});
// from D:\Work\libclang\include\llvm-c/Error.h:58:6
	addExtern< void (*)(char *) , LLVMDisposeErrorMessage >(*this,lib,"LLVMDisposeErrorMessage",SideEffects::worstDefault,"LLVMDisposeErrorMessage")
		->args({"ErrMsg"});
// from D:\Work\libclang\include\llvm-c/Error.h:63:17
	addExtern< const void * (*)() , LLVMGetStringErrorTypeId >(*this,lib,"LLVMGetStringErrorTypeId",SideEffects::worstDefault,"LLVMGetStringErrorTypeId");
// from D:\Work\libclang\include\llvm-c/Error.h:68:14
	addExtern< LLVMOpaqueError * (*)(const char *) , LLVMCreateStringError >(*this,lib,"LLVMCreateStringError",SideEffects::worstDefault,"LLVMCreateStringError")
		->args({"ErrMsg"});
// from D:\Work\libclang\include\llvm/Config/Targets.def:26:1
	addExtern< void (*)() , LLVMInitializeX86TargetInfo >(*this,lib,"LLVMInitializeX86TargetInfo",SideEffects::worstDefault,"LLVMInitializeX86TargetInfo");
// from D:\Work\libclang\include\llvm/Config/Targets.def:26:1
	addExtern< void (*)() , LLVMInitializeX86Target >(*this,lib,"LLVMInitializeX86Target",SideEffects::worstDefault,"LLVMInitializeX86Target");
// from D:\Work\libclang\include\llvm/Config/Targets.def:26:1
	addExtern< void (*)() , LLVMInitializeX86TargetMC >(*this,lib,"LLVMInitializeX86TargetMC",SideEffects::worstDefault,"LLVMInitializeX86TargetMC");
// from D:\Work\libclang\include\llvm/Config/AsmPrinters.def:27:1
	addExtern< void (*)() , LLVMInitializeX86AsmPrinter >(*this,lib,"LLVMInitializeX86AsmPrinter",SideEffects::worstDefault,"LLVMInitializeX86AsmPrinter");
// from D:\Work\libclang\include\llvm/Config/AsmParsers.def:27:1
	addExtern< void (*)() , LLVMInitializeX86AsmParser >(*this,lib,"LLVMInitializeX86AsmParser",SideEffects::worstDefault,"LLVMInitializeX86AsmParser");
// from D:\Work\libclang\include\llvm/Config/Disassemblers.def:27:1
	addExtern< void (*)() , LLVMInitializeX86Disassembler >(*this,lib,"LLVMInitializeX86Disassembler",SideEffects::worstDefault,"LLVMInitializeX86Disassembler");
// from D:\Work\libclang\include\llvm-c/Target.h:76:20
	addExtern< void (*)() , LLVMInitializeAllTargetInfos >(*this,lib,"LLVMInitializeAllTargetInfos",SideEffects::worstDefault,"LLVMInitializeAllTargetInfos");
// from D:\Work\libclang\include\llvm-c/Target.h:85:20
	addExtern< void (*)() , LLVMInitializeAllTargets >(*this,lib,"LLVMInitializeAllTargets",SideEffects::worstDefault,"LLVMInitializeAllTargets");
// from D:\Work\libclang\include\llvm-c/Target.h:94:20
	addExtern< void (*)() , LLVMInitializeAllTargetMCs >(*this,lib,"LLVMInitializeAllTargetMCs",SideEffects::worstDefault,"LLVMInitializeAllTargetMCs");
// from D:\Work\libclang\include\llvm-c/Target.h:103:20
	addExtern< void (*)() , LLVMInitializeAllAsmPrinters >(*this,lib,"LLVMInitializeAllAsmPrinters",SideEffects::worstDefault,"LLVMInitializeAllAsmPrinters");
}
}

