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
void Module_dasLLVM::initFunctions_32() {
// from D:\Work\libclang\include\llvm-c/Core.h:3789:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildFDiv >(*this,lib,"LLVMBuildFDiv",SideEffects::worstDefault,"LLVMBuildFDiv")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3791:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildURem >(*this,lib,"LLVMBuildURem",SideEffects::worstDefault,"LLVMBuildURem")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3793:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildSRem >(*this,lib,"LLVMBuildSRem",SideEffects::worstDefault,"LLVMBuildSRem")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3795:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildFRem >(*this,lib,"LLVMBuildFRem",SideEffects::worstDefault,"LLVMBuildFRem")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3797:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildShl >(*this,lib,"LLVMBuildShl",SideEffects::worstDefault,"LLVMBuildShl")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3799:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildLShr >(*this,lib,"LLVMBuildLShr",SideEffects::worstDefault,"LLVMBuildLShr")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3801:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildAShr >(*this,lib,"LLVMBuildAShr",SideEffects::worstDefault,"LLVMBuildAShr")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3803:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildAnd >(*this,lib,"LLVMBuildAnd",SideEffects::worstDefault,"LLVMBuildAnd")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3805:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildOr >(*this,lib,"LLVMBuildOr",SideEffects::worstDefault,"LLVMBuildOr")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3807:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildXor >(*this,lib,"LLVMBuildXor",SideEffects::worstDefault,"LLVMBuildXor")
		->args({"","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3809:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpcode,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildBinOp >(*this,lib,"LLVMBuildBinOp",SideEffects::worstDefault,"LLVMBuildBinOp")
		->args({"B","Op","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3812:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildNeg >(*this,lib,"LLVMBuildNeg",SideEffects::worstDefault,"LLVMBuildNeg")
		->args({"","V","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3813:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildNSWNeg >(*this,lib,"LLVMBuildNSWNeg",SideEffects::worstDefault,"LLVMBuildNSWNeg")
		->args({"B","V","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3815:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildNUWNeg >(*this,lib,"LLVMBuildNUWNeg",SideEffects::worstDefault,"LLVMBuildNUWNeg")
		->args({"B","V","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3817:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildFNeg >(*this,lib,"LLVMBuildFNeg",SideEffects::worstDefault,"LLVMBuildFNeg")
		->args({"","V","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3818:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildNot >(*this,lib,"LLVMBuildNot",SideEffects::worstDefault,"LLVMBuildNot")
		->args({"","V","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3821:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,const char *) , LLVMBuildMalloc >(*this,lib,"LLVMBuildMalloc",SideEffects::worstDefault,"LLVMBuildMalloc")
		->args({"","Ty","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3822:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,const char *) , LLVMBuildArrayMalloc >(*this,lib,"LLVMBuildArrayMalloc",SideEffects::worstDefault,"LLVMBuildArrayMalloc")
		->args({"","Ty","Val","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3831:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueValue *,unsigned int) , LLVMBuildMemSet >(*this,lib,"LLVMBuildMemSet",SideEffects::worstDefault,"LLVMBuildMemSet")
		->args({"B","Ptr","Val","Len","Align"});
// from D:\Work\libclang\include\llvm-c/Core.h:3839:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,unsigned int,LLVMOpaqueValue *,unsigned int,LLVMOpaqueValue *) , LLVMBuildMemCpy >(*this,lib,"LLVMBuildMemCpy",SideEffects::worstDefault,"LLVMBuildMemCpy")
		->args({"B","Dst","DstAlign","Src","SrcAlign","Size"});
}
}

