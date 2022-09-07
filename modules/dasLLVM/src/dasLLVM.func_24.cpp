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
void Module_dasLLVM::initFunctions_24() {
// from D:\Work\libclang\include\llvm-c/Core.h:2717:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *,const char *,size_t,LLVMOpaqueType *,unsigned int,LLVMOpaqueValue *) , LLVMAddGlobalIFunc >(*this,lib,"LLVMAddGlobalIFunc",SideEffects::worstDefault,"LLVMAddGlobalIFunc")
		->args({"M","Name","NameLen","Ty","AddrSpace","Resolver"});
// from D:\Work\libclang\include\llvm-c/Core.h:2729:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *,const char *,size_t) , LLVMGetNamedGlobalIFunc >(*this,lib,"LLVMGetNamedGlobalIFunc",SideEffects::worstDefault,"LLVMGetNamedGlobalIFunc")
		->args({"M","Name","NameLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:2737:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *) , LLVMGetFirstGlobalIFunc >(*this,lib,"LLVMGetFirstGlobalIFunc",SideEffects::worstDefault,"LLVMGetFirstGlobalIFunc")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:2744:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *) , LLVMGetLastGlobalIFunc >(*this,lib,"LLVMGetLastGlobalIFunc",SideEffects::worstDefault,"LLVMGetLastGlobalIFunc")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:2752:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetNextGlobalIFunc >(*this,lib,"LLVMGetNextGlobalIFunc",SideEffects::worstDefault,"LLVMGetNextGlobalIFunc")
		->args({"IFunc"});
// from D:\Work\libclang\include\llvm-c/Core.h:2760:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetPreviousGlobalIFunc >(*this,lib,"LLVMGetPreviousGlobalIFunc",SideEffects::worstDefault,"LLVMGetPreviousGlobalIFunc")
		->args({"IFunc"});
// from D:\Work\libclang\include\llvm-c/Core.h:2768:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetGlobalIFuncResolver >(*this,lib,"LLVMGetGlobalIFuncResolver",SideEffects::worstDefault,"LLVMGetGlobalIFuncResolver")
		->args({"IFunc"});
// from D:\Work\libclang\include\llvm-c/Core.h:2775:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMSetGlobalIFuncResolver >(*this,lib,"LLVMSetGlobalIFuncResolver",SideEffects::worstDefault,"LLVMSetGlobalIFuncResolver")
		->args({"IFunc","Resolver"});
// from D:\Work\libclang\include\llvm-c/Core.h:2782:6
	addExtern< void (*)(LLVMOpaqueValue *) , LLVMEraseGlobalIFunc >(*this,lib,"LLVMEraseGlobalIFunc",SideEffects::worstDefault,"LLVMEraseGlobalIFunc")
		->args({"IFunc"});
// from D:\Work\libclang\include\llvm-c/Core.h:2792:6
	addExtern< void (*)(LLVMOpaqueValue *) , LLVMRemoveGlobalIFunc >(*this,lib,"LLVMRemoveGlobalIFunc",SideEffects::worstDefault,"LLVMRemoveGlobalIFunc")
		->args({"IFunc"});
// from D:\Work\libclang\include\llvm-c/Core.h:2824:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueContext *,const char *,size_t) , LLVMMDStringInContext2 >(*this,lib,"LLVMMDStringInContext2",SideEffects::worstDefault,"LLVMMDStringInContext2")
		->args({"C","Str","SLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:2832:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueContext *,LLVMOpaqueMetadata **,size_t) , LLVMMDNodeInContext2 >(*this,lib,"LLVMMDNodeInContext2",SideEffects::worstDefault,"LLVMMDNodeInContext2")
		->args({"C","MDs","Count"});
// from D:\Work\libclang\include\llvm-c/Core.h:2838:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueContext *,LLVMOpaqueMetadata *) , LLVMMetadataAsValue >(*this,lib,"LLVMMetadataAsValue",SideEffects::worstDefault,"LLVMMetadataAsValue")
		->args({"C","MD"});
// from D:\Work\libclang\include\llvm-c/Core.h:2843:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueValue *) , LLVMValueAsMetadata >(*this,lib,"LLVMValueAsMetadata",SideEffects::worstDefault,"LLVMValueAsMetadata")
		->args({"Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:2852:13
	addExtern< const char * (*)(LLVMOpaqueValue *,unsigned int *) , LLVMGetMDString >(*this,lib,"LLVMGetMDString",SideEffects::worstDefault,"LLVMGetMDString")
		->args({"V","Length"});
// from D:\Work\libclang\include\llvm-c/Core.h:2860:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetMDNodeNumOperands >(*this,lib,"LLVMGetMDNodeNumOperands",SideEffects::worstDefault,"LLVMGetMDNodeNumOperands")
		->args({"V"});
// from D:\Work\libclang\include\llvm-c/Core.h:2873:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue **) , LLVMGetMDNodeOperands >(*this,lib,"LLVMGetMDNodeOperands",SideEffects::worstDefault,"LLVMGetMDNodeOperands")
		->args({"V","Dest"});
// from D:\Work\libclang\include\llvm-c/Core.h:2876:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueContext *,const char *,unsigned int) , LLVMMDStringInContext >(*this,lib,"LLVMMDStringInContext",SideEffects::worstDefault,"LLVMMDStringInContext")
		->args({"C","Str","SLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:2879:14
	addExtern< LLVMOpaqueValue * (*)(const char *,unsigned int) , LLVMMDString >(*this,lib,"LLVMMDString",SideEffects::worstDefault,"LLVMMDString")
		->args({"Str","SLen"});
// from D:\Work\libclang\include\llvm-c/Core.h:2881:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueContext *,LLVMOpaqueValue **,unsigned int) , LLVMMDNodeInContext >(*this,lib,"LLVMMDNodeInContext",SideEffects::worstDefault,"LLVMMDNodeInContext")
		->args({"C","Vals","Count"});
}
}

