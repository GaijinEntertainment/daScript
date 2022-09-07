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
void Module_dasLLVM::initFunctions_34() {
// from D:\Work\libclang\include\llvm-c/Core.h:3892:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMAtomicOrdering) , LLVMSetOrdering >(*this,lib,"LLVMSetOrdering",SideEffects::worstDefault,"LLVMSetOrdering")
		->args({"MemoryAccessInst","Ordering"});
// from D:\Work\libclang\include\llvm-c/Core.h:3893:20
	addExtern< LLVMAtomicRMWBinOp (*)(LLVMOpaqueValue *) , LLVMGetAtomicRMWBinOp >(*this,lib,"LLVMGetAtomicRMWBinOp",SideEffects::worstDefault,"LLVMGetAtomicRMWBinOp")
		->args({"AtomicRMWInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3894:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMAtomicRMWBinOp) , LLVMSetAtomicRMWBinOp >(*this,lib,"LLVMSetAtomicRMWBinOp",SideEffects::worstDefault,"LLVMSetAtomicRMWBinOp")
		->args({"AtomicRMWInst","BinOp"});
// from D:\Work\libclang\include\llvm-c/Core.h:3897:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildTrunc >(*this,lib,"LLVMBuildTrunc",SideEffects::worstDefault,"LLVMBuildTrunc")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3899:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildZExt >(*this,lib,"LLVMBuildZExt",SideEffects::worstDefault,"LLVMBuildZExt")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3901:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildSExt >(*this,lib,"LLVMBuildSExt",SideEffects::worstDefault,"LLVMBuildSExt")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3903:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildFPToUI >(*this,lib,"LLVMBuildFPToUI",SideEffects::worstDefault,"LLVMBuildFPToUI")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3905:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildFPToSI >(*this,lib,"LLVMBuildFPToSI",SideEffects::worstDefault,"LLVMBuildFPToSI")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3907:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildUIToFP >(*this,lib,"LLVMBuildUIToFP",SideEffects::worstDefault,"LLVMBuildUIToFP")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3909:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildSIToFP >(*this,lib,"LLVMBuildSIToFP",SideEffects::worstDefault,"LLVMBuildSIToFP")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3911:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildFPTrunc >(*this,lib,"LLVMBuildFPTrunc",SideEffects::worstDefault,"LLVMBuildFPTrunc")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3913:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildFPExt >(*this,lib,"LLVMBuildFPExt",SideEffects::worstDefault,"LLVMBuildFPExt")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3915:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildPtrToInt >(*this,lib,"LLVMBuildPtrToInt",SideEffects::worstDefault,"LLVMBuildPtrToInt")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3917:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildIntToPtr >(*this,lib,"LLVMBuildIntToPtr",SideEffects::worstDefault,"LLVMBuildIntToPtr")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3919:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildBitCast >(*this,lib,"LLVMBuildBitCast",SideEffects::worstDefault,"LLVMBuildBitCast")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3921:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildAddrSpaceCast >(*this,lib,"LLVMBuildAddrSpaceCast",SideEffects::worstDefault,"LLVMBuildAddrSpaceCast")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3923:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildZExtOrBitCast >(*this,lib,"LLVMBuildZExtOrBitCast",SideEffects::worstDefault,"LLVMBuildZExtOrBitCast")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3925:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildSExtOrBitCast >(*this,lib,"LLVMBuildSExtOrBitCast",SideEffects::worstDefault,"LLVMBuildSExtOrBitCast")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3927:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildTruncOrBitCast >(*this,lib,"LLVMBuildTruncOrBitCast",SideEffects::worstDefault,"LLVMBuildTruncOrBitCast")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3929:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpcode,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildCast >(*this,lib,"LLVMBuildCast",SideEffects::worstDefault,"LLVMBuildCast")
		->args({"B","Op","Val","DestTy","Name"});
}
}

