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
void Module_dasLLVM::initFunctions_35() {
// from D:\Work\libclang\include\llvm-c/Core.h:3931:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildPointerCast >(*this,lib,"LLVMBuildPointerCast",SideEffects::worstDefault,"LLVMBuildPointerCast")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3933:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,int,const char *) , LLVMBuildIntCast2 >(*this,lib,"LLVMBuildIntCast2",SideEffects::worstDefault,"LLVMBuildIntCast2")
		->args({"","Val","DestTy","IsSigned","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3936:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildFPCast >(*this,lib,"LLVMBuildFPCast",SideEffects::worstDefault,"LLVMBuildFPCast")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3940:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildIntCast >(*this,lib,"LLVMBuildIntCast",SideEffects::worstDefault,"LLVMBuildIntCast")
		->args({"","Val","DestTy","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3944:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMIntPredicate,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildICmp >(*this,lib,"LLVMBuildICmp",SideEffects::worstDefault,"LLVMBuildICmp")
		->args({"","Op","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3947:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMRealPredicate,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildFCmp >(*this,lib,"LLVMBuildFCmp",SideEffects::worstDefault,"LLVMBuildFCmp")
		->args({"","Op","LHS","RHS","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3952:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,const char *) , LLVMBuildPhi >(*this,lib,"LLVMBuildPhi",SideEffects::worstDefault,"LLVMBuildPhi")
		->args({"","Ty","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3955:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,const char *) , LLVMBuildCall >(*this,lib,"LLVMBuildCall",SideEffects::worstDefault,"LLVMBuildCall")
		->args({"","Fn","Args","NumArgs","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3958:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,const char *) , LLVMBuildCall2 >(*this,lib,"LLVMBuildCall2",SideEffects::worstDefault,"LLVMBuildCall2")
		->args({"","","Fn","Args","NumArgs","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3961:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildSelect >(*this,lib,"LLVMBuildSelect",SideEffects::worstDefault,"LLVMBuildSelect")
		->args({"","If","Then","Else","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3964:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueType *,const char *) , LLVMBuildVAArg >(*this,lib,"LLVMBuildVAArg",SideEffects::worstDefault,"LLVMBuildVAArg")
		->args({"","List","Ty","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3966:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildExtractElement >(*this,lib,"LLVMBuildExtractElement",SideEffects::worstDefault,"LLVMBuildExtractElement")
		->args({"","VecVal","Index","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3968:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildInsertElement >(*this,lib,"LLVMBuildInsertElement",SideEffects::worstDefault,"LLVMBuildInsertElement")
		->args({"","VecVal","EltVal","Index","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3971:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildShuffleVector >(*this,lib,"LLVMBuildShuffleVector",SideEffects::worstDefault,"LLVMBuildShuffleVector")
		->args({"","V1","V2","Mask","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3974:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,unsigned int,const char *) , LLVMBuildExtractValue >(*this,lib,"LLVMBuildExtractValue",SideEffects::worstDefault,"LLVMBuildExtractValue")
		->args({"","AggVal","Index","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3976:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,unsigned int,const char *) , LLVMBuildInsertValue >(*this,lib,"LLVMBuildInsertValue",SideEffects::worstDefault,"LLVMBuildInsertValue")
		->args({"","AggVal","EltVal","Index","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3979:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildFreeze >(*this,lib,"LLVMBuildFreeze",SideEffects::worstDefault,"LLVMBuildFreeze")
		->args({"","Val","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3982:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildIsNull >(*this,lib,"LLVMBuildIsNull",SideEffects::worstDefault,"LLVMBuildIsNull")
		->args({"","Val","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3984:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,const char *) , LLVMBuildIsNotNull >(*this,lib,"LLVMBuildIsNotNull",SideEffects::worstDefault,"LLVMBuildIsNotNull")
		->args({"","Val","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3986:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,const char *) , LLVMBuildPtrDiff >(*this,lib,"LLVMBuildPtrDiff",SideEffects::worstDefault,"LLVMBuildPtrDiff")
		->args({"","LHS","RHS","Name"});
}
}

