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
void Module_dasLLVM::initFunctions_17() {
// from D:\Work\libclang\include\llvm-c/Core.h:2124:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstAdd >(*this,lib,"LLVMConstAdd",SideEffects::worstDefault,"LLVMConstAdd")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2125:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstNSWAdd >(*this,lib,"LLVMConstNSWAdd",SideEffects::worstDefault,"LLVMConstNSWAdd")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2126:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstNUWAdd >(*this,lib,"LLVMConstNUWAdd",SideEffects::worstDefault,"LLVMConstNUWAdd")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2127:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstFAdd >(*this,lib,"LLVMConstFAdd",SideEffects::worstDefault,"LLVMConstFAdd")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2128:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstSub >(*this,lib,"LLVMConstSub",SideEffects::worstDefault,"LLVMConstSub")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2129:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstNSWSub >(*this,lib,"LLVMConstNSWSub",SideEffects::worstDefault,"LLVMConstNSWSub")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2130:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstNUWSub >(*this,lib,"LLVMConstNUWSub",SideEffects::worstDefault,"LLVMConstNUWSub")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2131:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstFSub >(*this,lib,"LLVMConstFSub",SideEffects::worstDefault,"LLVMConstFSub")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2132:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstMul >(*this,lib,"LLVMConstMul",SideEffects::worstDefault,"LLVMConstMul")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2133:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstNSWMul >(*this,lib,"LLVMConstNSWMul",SideEffects::worstDefault,"LLVMConstNSWMul")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2134:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstNUWMul >(*this,lib,"LLVMConstNUWMul",SideEffects::worstDefault,"LLVMConstNUWMul")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2135:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstFMul >(*this,lib,"LLVMConstFMul",SideEffects::worstDefault,"LLVMConstFMul")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2136:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstUDiv >(*this,lib,"LLVMConstUDiv",SideEffects::worstDefault,"LLVMConstUDiv")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2137:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstExactUDiv >(*this,lib,"LLVMConstExactUDiv",SideEffects::worstDefault,"LLVMConstExactUDiv")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2138:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstSDiv >(*this,lib,"LLVMConstSDiv",SideEffects::worstDefault,"LLVMConstSDiv")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2139:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstExactSDiv >(*this,lib,"LLVMConstExactSDiv",SideEffects::worstDefault,"LLVMConstExactSDiv")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2140:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstFDiv >(*this,lib,"LLVMConstFDiv",SideEffects::worstDefault,"LLVMConstFDiv")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2141:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstURem >(*this,lib,"LLVMConstURem",SideEffects::worstDefault,"LLVMConstURem")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2142:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstSRem >(*this,lib,"LLVMConstSRem",SideEffects::worstDefault,"LLVMConstSRem")
		->args({"LHSConstant","RHSConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2143:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMConstFRem >(*this,lib,"LLVMConstFRem",SideEffects::worstDefault,"LLVMConstFRem")
		->args({"LHSConstant","RHSConstant"});
}
}

