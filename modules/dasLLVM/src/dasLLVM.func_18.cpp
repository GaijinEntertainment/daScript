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
void Module_dasLLVM::initFunctions_18() {
// from D:\Work\libclang\include\llvm-c/Core.h:2144:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstAnd >(*this,lib,"LLVMConstAnd",SideEffects::worstDefault,"LLVMConstAnd")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2145:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstOr >(*this,lib,"LLVMConstOr",SideEffects::worstDefault,"LLVMConstOr")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2146:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstXor >(*this,lib,"LLVMConstXor",SideEffects::worstDefault,"LLVMConstXor")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2147:14
	addExtern< LLVMOpaqueValue * (*)(LLVMIntPredicate,LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstICmp >(*this,lib,"LLVMConstICmp",SideEffects::worstDefault,"LLVMConstICmp")
		->args({"Predicate","LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2149:14
	addExtern< LLVMOpaqueValue * (*)(LLVMRealPredicate,LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstFCmp >(*this,lib,"LLVMConstFCmp",SideEffects::worstDefault,"LLVMConstFCmp")
		->args({"Predicate","LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2151:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstShl >(*this,lib,"LLVMConstShl",SideEffects::worstDefault,"LLVMConstShl")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2152:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstLShr >(*this,lib,"LLVMConstLShr",SideEffects::worstDefault,"LLVMConstLShr")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2153:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstAShr >(*this,lib,"LLVMConstAShr",SideEffects::worstDefault,"LLVMConstAShr")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2154:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int) , LLVMConstGEP >(*this,lib,"LLVMConstGEP",SideEffects::worstDefault,"LLVMConstGEP")
		->args({"ConstantVal","ConstantIndices","NumIndices"});
// from D:\Work\libclang\include\llvm-c/Core.h:2158:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int) , LLVMConstInBoundsGEP >(*this,lib,"LLVMConstInBoundsGEP",SideEffects::worstDefault,"LLVMConstInBoundsGEP")
		->args({"ConstantVal","ConstantIndices","NumIndices"});
// from D:\Work\libclang\include\llvm-c/Core.h:2164:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstTrunc >(*this,lib,"LLVMConstTrunc",SideEffects::worstDefault,"LLVMConstTrunc")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2165:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstSExt >(*this,lib,"LLVMConstSExt",SideEffects::worstDefault,"LLVMConstSExt")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2166:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstZExt >(*this,lib,"LLVMConstZExt",SideEffects::worstDefault,"LLVMConstZExt")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2167:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstFPTrunc >(*this,lib,"LLVMConstFPTrunc",SideEffects::worstDefault,"LLVMConstFPTrunc")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2168:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstFPExt >(*this,lib,"LLVMConstFPExt",SideEffects::worstDefault,"LLVMConstFPExt")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2169:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstUIToFP >(*this,lib,"LLVMConstUIToFP",SideEffects::worstDefault,"LLVMConstUIToFP")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2170:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstSIToFP >(*this,lib,"LLVMConstSIToFP",SideEffects::worstDefault,"LLVMConstSIToFP")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2171:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstFPToUI >(*this,lib,"LLVMConstFPToUI",SideEffects::worstDefault,"LLVMConstFPToUI")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2172:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstFPToSI >(*this,lib,"LLVMConstFPToSI",SideEffects::worstDefault,"LLVMConstFPToSI")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2173:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstPtrToInt >(*this,lib,"LLVMConstPtrToInt",SideEffects::worstDefault,"LLVMConstPtrToInt")
		->args({"ConstantVal","ToType"});
}
}

