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
void Module_dasLLVM::initFunctions_33() {
// from D:\Work\libclang\include\llvm-c/Core.h:3848:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,unsigned int,LLVMOpaqueValue *,unsigned int,LLVMOpaqueValue *) , LLVMBuildMemMove >(*this,lib,"LLVMBuildMemMove",SideEffects::worstDefault,"LLVMBuildMemMove")
		->args({"B","Dst","DstAlign","Src","SrcAlign","Size"});
// from D:\Work\libclang\include\llvm-c/Core.h:3853:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,const char *) , LLVMBuildAlloca >(*this,lib,"LLVMBuildAlloca",SideEffects::worstDefault,"LLVMBuildAlloca")
		->args({"","Ty","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3854:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,const char *) , LLVMBuildArrayAlloca >(*this,lib,"LLVMBuildArrayAlloca",SideEffects::worstDefault,"LLVMBuildArrayAlloca")
		->args({"","Ty","Val","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3856:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *) , LLVMBuildFree >(*this,lib,"LLVMBuildFree",SideEffects::worstDefault,"LLVMBuildFree")
		->args({"","PointerVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:3859:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildLoad >(*this,lib,"LLVMBuildLoad",SideEffects::worstDefault,"LLVMBuildLoad")
		->args({"","PointerVal","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3861:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,const char *) , LLVMBuildLoad2 >(*this,lib,"LLVMBuildLoad2",SideEffects::worstDefault,"LLVMBuildLoad2")
		->args({"","Ty","PointerVal","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3863:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMBuildStore >(*this,lib,"LLVMBuildStore",SideEffects::worstDefault,"LLVMBuildStore")
		->args({"","Val","Ptr"});
// from D:\Work\libclang\include\llvm-c/Core.h:3866:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,const char *) , LLVMBuildGEP >(*this,lib,"LLVMBuildGEP",SideEffects::worstDefault,"LLVMBuildGEP")
		->args({"B","Pointer","Indices","NumIndices","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3869:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,const char *) , LLVMBuildInBoundsGEP >(*this,lib,"LLVMBuildInBoundsGEP",SideEffects::worstDefault,"LLVMBuildInBoundsGEP")
		->args({"B","Pointer","Indices","NumIndices","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3872:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,unsigned int,const char *) , LLVMBuildStructGEP >(*this,lib,"LLVMBuildStructGEP",SideEffects::worstDefault,"LLVMBuildStructGEP")
		->args({"B","Pointer","Idx","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3874:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,const char *) , LLVMBuildGEP2 >(*this,lib,"LLVMBuildGEP2",SideEffects::worstDefault,"LLVMBuildGEP2")
		->args({"B","Ty","Pointer","Indices","NumIndices","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3877:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,const char *) , LLVMBuildInBoundsGEP2 >(*this,lib,"LLVMBuildInBoundsGEP2",SideEffects::worstDefault,"LLVMBuildInBoundsGEP2")
		->args({"B","Ty","Pointer","Indices","NumIndices","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3880:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,unsigned int,const char *) , LLVMBuildStructGEP2 >(*this,lib,"LLVMBuildStructGEP2",SideEffects::worstDefault,"LLVMBuildStructGEP2")
		->args({"B","Ty","Pointer","Idx","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3883:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,const char *,const char *) , LLVMBuildGlobalString >(*this,lib,"LLVMBuildGlobalString",SideEffects::worstDefault,"LLVMBuildGlobalString")
		->args({"B","Str","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3885:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,const char *,const char *) , LLVMBuildGlobalStringPtr >(*this,lib,"LLVMBuildGlobalStringPtr",SideEffects::worstDefault,"LLVMBuildGlobalStringPtr")
		->args({"B","Str","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3887:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMGetVolatile >(*this,lib,"LLVMGetVolatile",SideEffects::worstDefault,"LLVMGetVolatile")
		->args({"MemoryAccessInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3888:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetVolatile >(*this,lib,"LLVMSetVolatile",SideEffects::worstDefault,"LLVMSetVolatile")
		->args({"MemoryAccessInst","IsVolatile"});
// from D:\Work\libclang\include\llvm-c/Core.h:3889:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMGetWeak >(*this,lib,"LLVMGetWeak",SideEffects::worstDefault,"LLVMGetWeak")
		->args({"CmpXchgInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:3890:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetWeak >(*this,lib,"LLVMSetWeak",SideEffects::worstDefault,"LLVMSetWeak")
		->args({"CmpXchgInst","IsWeak"});
// from D:\Work\libclang\include\llvm-c/Core.h:3891:20
	addExtern< LLVMAtomicOrdering (*)(LLVMOpaqueValue *) , LLVMGetOrdering >(*this,lib,"LLVMGetOrdering",SideEffects::worstDefault,"LLVMGetOrdering")
		->args({"MemoryAccessInst"});
}
}

