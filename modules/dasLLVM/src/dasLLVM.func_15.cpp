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
void Module_dasLLVM::initFunctions_15() {
// from D:\Work\libclang\include\llvm-c/Core.h:1794:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueUse *) , LLVMGetUser >(*this,lib,"LLVMGetUser",SideEffects::worstDefault,"LLVMGetUser")
		->args({"U"});
// from D:\Work\libclang\include\llvm-c/Core.h:1801:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueUse *) , LLVMGetUsedValue >(*this,lib,"LLVMGetUsedValue",SideEffects::worstDefault,"LLVMGetUsedValue")
		->args({"U"});
// from D:\Work\libclang\include\llvm-c/Core.h:1822:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetOperand >(*this,lib,"LLVMGetOperand",SideEffects::worstDefault,"LLVMGetOperand")
		->args({"Val","Index"});
// from D:\Work\libclang\include\llvm-c/Core.h:1829:12
	addExtern< LLVMOpaqueUse * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetOperandUse >(*this,lib,"LLVMGetOperandUse",SideEffects::worstDefault,"LLVMGetOperandUse")
		->args({"Val","Index"});
// from D:\Work\libclang\include\llvm-c/Core.h:1836:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueValue *) , LLVMSetOperand >(*this,lib,"LLVMSetOperand",SideEffects::worstDefault,"LLVMSetOperand")
		->args({"User","Index","Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1843:5
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMGetNumOperands >(*this,lib,"LLVMGetNumOperands",SideEffects::worstDefault,"LLVMGetNumOperands")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1866:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *) , LLVMConstNull >(*this,lib,"LLVMConstNull",SideEffects::worstDefault,"LLVMConstNull")
		->args({"Ty"});
// from D:\Work\libclang\include\llvm-c/Core.h:1876:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *) , LLVMConstAllOnes >(*this,lib,"LLVMConstAllOnes",SideEffects::worstDefault,"LLVMConstAllOnes")
		->args({"Ty"});
// from D:\Work\libclang\include\llvm-c/Core.h:1883:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *) , LLVMGetUndef >(*this,lib,"LLVMGetUndef",SideEffects::worstDefault,"LLVMGetUndef")
		->args({"Ty"});
// from D:\Work\libclang\include\llvm-c/Core.h:1890:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *) , LLVMGetPoison >(*this,lib,"LLVMGetPoison",SideEffects::worstDefault,"LLVMGetPoison")
		->args({"Ty"});
// from D:\Work\libclang\include\llvm-c/Core.h:1897:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsNull >(*this,lib,"LLVMIsNull",SideEffects::worstDefault,"LLVMIsNull")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:1903:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *) , LLVMConstPointerNull >(*this,lib,"LLVMConstPointerNull",SideEffects::worstDefault,"LLVMConstPointerNull")
		->args({"Ty"});
// from D:\Work\libclang\include\llvm-c/Core.h:1932:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,unsigned long long,int) , LLVMConstInt >(*this,lib,"LLVMConstInt",SideEffects::worstDefault,"LLVMConstInt")
		->args({"IntTy","N","SignExtend"});
// from D:\Work\libclang\include\llvm-c/Core.h:1940:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,unsigned int,unsigned long long const[]) , LLVMConstIntOfArbitraryPrecision >(*this,lib,"LLVMConstIntOfArbitraryPrecision",SideEffects::worstDefault,"LLVMConstIntOfArbitraryPrecision")
		->args({"IntTy","NumWords","Words"});
// from D:\Work\libclang\include\llvm-c/Core.h:1953:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,const char *,unsigned char) , LLVMConstIntOfString >(*this,lib,"LLVMConstIntOfString",SideEffects::worstDefault,"LLVMConstIntOfString")
		->args({"IntTy","Text","Radix"});
// from D:\Work\libclang\include\llvm-c/Core.h:1962:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,const char *,unsigned int,unsigned char) , LLVMConstIntOfStringAndSize >(*this,lib,"LLVMConstIntOfStringAndSize",SideEffects::worstDefault,"LLVMConstIntOfStringAndSize")
		->args({"IntTy","Text","SLen","Radix"});
// from D:\Work\libclang\include\llvm-c/Core.h:1968:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,double) , LLVMConstReal >(*this,lib,"LLVMConstReal",SideEffects::worstDefault,"LLVMConstReal")
		->args({"RealTy","N"});
// from D:\Work\libclang\include\llvm-c/Core.h:1976:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,const char *) , LLVMConstRealOfString >(*this,lib,"LLVMConstRealOfString",SideEffects::worstDefault,"LLVMConstRealOfString")
		->args({"RealTy","Text"});
// from D:\Work\libclang\include\llvm-c/Core.h:1981:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,const char *,unsigned int) , LLVMConstRealOfStringAndSize >(*this,lib,"LLVMConstRealOfStringAndSize",SideEffects::worstDefault,"LLVMConstRealOfStringAndSize")
		->args({"RealTy","Text","SLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:1989:20
	addExtern< unsigned long long (*)(LLVMOpaqueValue *) , LLVMConstIntGetZExtValue >(*this,lib,"LLVMConstIntGetZExtValue",SideEffects::worstDefault,"LLVMConstIntGetZExtValue")
		->args({"ConstantVal"});
}
}

