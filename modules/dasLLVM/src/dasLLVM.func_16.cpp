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
void Module_dasLLVM::initFunctions_16() {
// from D:\Work\libclang\include\llvm-c/Core.h:1996:11
	addExtern< long long (*)(LLVMOpaqueValue *) , LLVMConstIntGetSExtValue >(*this,lib,"LLVMConstIntGetSExtValue",SideEffects::worstDefault,"LLVMConstIntGetSExtValue")
		->args({"ConstantVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:2004:8
	addExtern< double (*)(LLVMOpaqueValue *,int *) , LLVMConstRealGetDouble >(*this,lib,"LLVMConstRealGetDouble",SideEffects::worstDefault,"LLVMConstRealGetDouble")
		->args({"ConstantVal","losesInfo"});
// from D:\Work\libclang\include\llvm-c/Core.h:2023:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueContext *,const char *,unsigned int,int) , LLVMConstStringInContext >(*this,lib,"LLVMConstStringInContext",SideEffects::worstDefault,"LLVMConstStringInContext")
		->args({"C","Str","Length","DontNullTerminate"});
// from D:\Work\libclang\include\llvm-c/Core.h:2035:14
	addExtern< LLVMOpaqueValue * (*)(const char *,unsigned int,int) , LLVMConstString >(*this,lib,"LLVMConstString",SideEffects::worstDefault,"LLVMConstString")
		->args({"Str","Length","DontNullTerminate"});
// from D:\Work\libclang\include\llvm-c/Core.h:2043:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsConstantString >(*this,lib,"LLVMIsConstantString",SideEffects::worstDefault,"LLVMIsConstantString")
		->args({"c"});
// from D:\Work\libclang\include\llvm-c/Core.h:2050:13
	addExtern< const char * (*)(LLVMOpaqueValue *,size_t *) , LLVMGetAsString >(*this,lib,"LLVMGetAsString",SideEffects::worstDefault,"LLVMGetAsString")
		->args({"c","Length"});
// from D:\Work\libclang\include\llvm-c/Core.h:2057:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueContext *,LLVMOpaqueValue **,unsigned int,int) , LLVMConstStructInContext >(*this,lib,"LLVMConstStructInContext",SideEffects::worstDefault,"LLVMConstStructInContext")
		->args({"C","ConstantVals","Count","Packed"});
// from D:\Work\libclang\include\llvm-c/Core.h:2069:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue **,unsigned int,int) , LLVMConstStruct >(*this,lib,"LLVMConstStruct",SideEffects::worstDefault,"LLVMConstStruct")
		->args({"ConstantVals","Count","Packed"});
// from D:\Work\libclang\include\llvm-c/Core.h:2077:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,LLVMOpaqueValue **,unsigned int) , LLVMConstArray >(*this,lib,"LLVMConstArray",SideEffects::worstDefault,"LLVMConstArray")
		->args({"ElementTy","ConstantVals","Length"});
// from D:\Work\libclang\include\llvm-c/Core.h:2085:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *,LLVMOpaqueValue **,unsigned int) , LLVMConstNamedStruct >(*this,lib,"LLVMConstNamedStruct",SideEffects::worstDefault,"LLVMConstNamedStruct")
		->args({"StructTy","ConstantVals","Count"});
// from D:\Work\libclang\include\llvm-c/Core.h:2094:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetElementAsConstant >(*this,lib,"LLVMGetElementAsConstant",SideEffects::worstDefault,"LLVMGetElementAsConstant")
		->args({"C","idx"});
// from D:\Work\libclang\include\llvm-c/Core.h:2101:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue **,unsigned int) , LLVMConstVector >(*this,lib,"LLVMConstVector",SideEffects::worstDefault,"LLVMConstVector")
		->args({"ScalarConstantVals","Size"});
// from D:\Work\libclang\include\llvm-c/Core.h:2116:12
	addExtern< LLVMOpcode (*)(LLVMOpaqueValue *) , LLVMGetConstOpcode >(*this,lib,"LLVMGetConstOpcode",SideEffects::worstDefault,"LLVMGetConstOpcode")
		->args({"ConstantVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:2117:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *) , LLVMAlignOf >(*this,lib,"LLVMAlignOf",SideEffects::worstDefault,"LLVMAlignOf")
		->args({"Ty"});
// from D:\Work\libclang\include\llvm-c/Core.h:2118:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueType *) , LLVMSizeOf >(*this,lib,"LLVMSizeOf",SideEffects::worstDefault,"LLVMSizeOf")
		->args({"Ty"});
// from D:\Work\libclang\include\llvm-c/Core.h:2119:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMConstNeg >(*this,lib,"LLVMConstNeg",SideEffects::worstDefault,"LLVMConstNeg")
		->args({"ConstantVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:2120:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMConstNSWNeg >(*this,lib,"LLVMConstNSWNeg",SideEffects::worstDefault,"LLVMConstNSWNeg")
		->args({"ConstantVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:2121:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMConstNUWNeg >(*this,lib,"LLVMConstNUWNeg",SideEffects::worstDefault,"LLVMConstNUWNeg")
		->args({"ConstantVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:2122:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMConstFNeg >(*this,lib,"LLVMConstFNeg",SideEffects::worstDefault,"LLVMConstFNeg")
		->args({"ConstantVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:2123:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMConstNot >(*this,lib,"LLVMConstNot",SideEffects::worstDefault,"LLVMConstNot")
		->args({"ConstantVal"});
}
}

