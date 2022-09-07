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
void Module_dasLLVM::initFunctions_19() {
// from D:\Work\libclang\include\llvm-c/Core.h:2174:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstIntToPtr >(*this,lib,"LLVMConstIntToPtr",SideEffects::worstDefault,"LLVMConstIntToPtr")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2175:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstBitCast >(*this,lib,"LLVMConstBitCast",SideEffects::worstDefault,"LLVMConstBitCast")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2176:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstAddrSpaceCast >(*this,lib,"LLVMConstAddrSpaceCast",SideEffects::worstDefault,"LLVMConstAddrSpaceCast")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2177:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstZExtOrBitCast >(*this,lib,"LLVMConstZExtOrBitCast",SideEffects::worstDefault,"LLVMConstZExtOrBitCast")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2179:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstSExtOrBitCast >(*this,lib,"LLVMConstSExtOrBitCast",SideEffects::worstDefault,"LLVMConstSExtOrBitCast")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2181:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstTruncOrBitCast >(*this,lib,"LLVMConstTruncOrBitCast",SideEffects::worstDefault,"LLVMConstTruncOrBitCast")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2183:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstPointerCast >(*this,lib,"LLVMConstPointerCast",SideEffects::worstDefault,"LLVMConstPointerCast")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2185:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *,int) , LLVMConstIntCast >(*this,lib,"LLVMConstIntCast",SideEffects::worstDefault,"LLVMConstIntCast")
		->args({"ConstantVal","ToType","isSigned"});
// from D:\Work\libclang\include\llvm-c/Core.h:2187:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueType *) , LLVMConstFPCast >(*this,lib,"LLVMConstFPCast",SideEffects::worstDefault,"LLVMConstFPCast")
		->args({"ConstantVal","ToType"});
// from D:\Work\libclang\include\llvm-c/Core.h:2188:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstSelect >(*this,lib,"LLVMConstSelect",SideEffects::worstDefault,"LLVMConstSelect")
		->args({"ConstantCondition","ConstantIfTrue","ConstantIfFalse"});
// from D:\Work\libclang\include\llvm-c/Core.h:2191:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstExtractElement >(*this,lib,"LLVMConstExtractElement",SideEffects::worstDefault,"LLVMConstExtractElement")
		->args({"VectorConstant","IndexConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2193:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstInsertElement >(*this,lib,"LLVMConstInsertElement",SideEffects::worstDefault,"LLVMConstInsertElement")
		->args({"VectorConstant","ElementValueConstant","IndexConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2196:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstShuffleVector >(*this,lib,"LLVMConstShuffleVector",SideEffects::worstDefault,"LLVMConstShuffleVector")
		->args({"VectorAConstant","VectorBConstant","MaskConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2199:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,unsigned int *,unsigned int) , LLVMConstExtractValue >(*this,lib,"LLVMConstExtractValue",SideEffects::worstDefault,"LLVMConstExtractValue")
		->args({"AggConstant","IdxList","NumIdx"});
// from D:\Work\libclang\include\llvm-c/Core.h:2201:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *,unsigned int *,unsigned int) , LLVMConstInsertValue >(*this,lib,"LLVMConstInsertValue",SideEffects::worstDefault,"LLVMConstInsertValue")
		->args({"AggConstant","ElementValueConstant","IdxList","NumIdx"});
// from D:\Work\libclang\include\llvm-c/Core.h:2204:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMBlockAddress >(*this,lib,"LLVMBlockAddress",SideEffects::worstDefault,"LLVMBlockAddress")
		->args({"F","BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:2207:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,const char *,const char *,int,int) , LLVMConstInlineAsm >(*this,lib,"LLVMConstInlineAsm",SideEffects::worstDefault,"LLVMConstInlineAsm")
		->args({"Ty","AsmString","Constraints","HasSideEffects","IsAlignStack"});
// from D:\Work\libclang\include\llvm-c/Core.h:2226:15
	addExtern< LLVMOpaqueModule * (*)(LLVMOpaqueValue *) , LLVMGetGlobalParent >(*this,lib,"LLVMGetGlobalParent",SideEffects::worstDefault,"LLVMGetGlobalParent")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2227:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsDeclaration >(*this,lib,"LLVMIsDeclaration",SideEffects::worstDefault,"LLVMIsDeclaration")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2228:13
	addExtern< LLVMLinkage (*)(LLVMOpaqueValue *) , LLVMGetLinkage >(*this,lib,"LLVMGetLinkage",SideEffects::worstDefault,"LLVMGetLinkage")
		->args({"Global"});
}
}

