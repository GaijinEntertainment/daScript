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
void Module_dasLLVM::initFunctions_36() {
// from D:\Work\libclang\include\llvm-c/Core.h:3988:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMAtomicOrdering,int,const char *) , LLVMBuildFence >(*this,lib,"LLVMBuildFence",SideEffects::worstDefault,"LLVMBuildFence")
		->args({"B","ordering","singleThread","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3990:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMAtomicRMWBinOp,LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMAtomicOrdering,int) , LLVMBuildAtomicRMW >(*this,lib,"LLVMBuildAtomicRMW",SideEffects::worstDefault,"LLVMBuildAtomicRMW")
		->args({"B","op","PTR","Val","ordering","singleThread"});
// from D:\Work\libclang\include\llvm-c/Core.h:3994:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMAtomicOrdering,LLVMAtomicOrdering,int) , LLVMBuildAtomicCmpXchg >(*this,lib,"LLVMBuildAtomicCmpXchg",SideEffects::worstDefault,"LLVMBuildAtomicCmpXchg")
		->args({"B","Ptr","Cmp","New","SuccessOrdering","FailureOrdering","SingleThread"});
// from D:\Work\libclang\include\llvm-c/Core.h:4003:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetNumMaskElements >(*this,lib,"LLVMGetNumMaskElements",SideEffects::worstDefault,"LLVMGetNumMaskElements")
		->args({"ShuffleVectorInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:4009:5
	addExtern< int (*)() , LLVMGetUndefMaskElem >(*this,lib,"LLVMGetUndefMaskElem",SideEffects::worstDefault,"LLVMGetUndefMaskElem");
// from D:\Work\libclang\include\llvm-c/Core.h:4018:5
	addExtern< int (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetMaskValue >(*this,lib,"LLVMGetMaskValue",SideEffects::worstDefault,"LLVMGetMaskValue")
		->args({"ShuffleVectorInst","Elt"});
// from D:\Work\libclang\include\llvm-c/Core.h:4020:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsAtomicSingleThread >(*this,lib,"LLVMIsAtomicSingleThread",SideEffects::worstDefault,"LLVMIsAtomicSingleThread")
		->args({"AtomicInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:4021:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetAtomicSingleThread >(*this,lib,"LLVMSetAtomicSingleThread",SideEffects::worstDefault,"LLVMSetAtomicSingleThread")
		->args({"AtomicInst","SingleThread"});
// from D:\Work\libclang\include\llvm-c/Core.h:4023:20
	addExtern< LLVMAtomicOrdering (*)(LLVMOpaqueValue *) , LLVMGetCmpXchgSuccessOrdering >(*this,lib,"LLVMGetCmpXchgSuccessOrdering",SideEffects::worstDefault,"LLVMGetCmpXchgSuccessOrdering")
		->args({"CmpXchgInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:4024:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMAtomicOrdering) , LLVMSetCmpXchgSuccessOrdering >(*this,lib,"LLVMSetCmpXchgSuccessOrdering",SideEffects::worstDefault,"LLVMSetCmpXchgSuccessOrdering")
		->args({"CmpXchgInst","Ordering"});
// from D:\Work\libclang\include\llvm-c/Core.h:4026:20
	addExtern< LLVMAtomicOrdering (*)(LLVMOpaqueValue *) , LLVMGetCmpXchgFailureOrdering >(*this,lib,"LLVMGetCmpXchgFailureOrdering",SideEffects::worstDefault,"LLVMGetCmpXchgFailureOrdering")
		->args({"CmpXchgInst"});
// from D:\Work\libclang\include\llvm-c/Core.h:4027:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMAtomicOrdering) , LLVMSetCmpXchgFailureOrdering >(*this,lib,"LLVMSetCmpXchgFailureOrdering",SideEffects::worstDefault,"LLVMSetCmpXchgFailureOrdering")
		->args({"CmpXchgInst","Ordering"});
// from D:\Work\libclang\include\llvm-c/Core.h:4045:1
	addExtern< LLVMOpaqueModuleProvider * (*)(LLVMOpaqueModule *) , LLVMCreateModuleProviderForExistingModule >(*this,lib,"LLVMCreateModuleProviderForExistingModule",SideEffects::worstDefault,"LLVMCreateModuleProviderForExistingModule")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:4050:6
	addExtern< void (*)(LLVMOpaqueModuleProvider *) , LLVMDisposeModuleProvider >(*this,lib,"LLVMDisposeModuleProvider",SideEffects::worstDefault,"LLVMDisposeModuleProvider")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:4062:10
	addExtern< int (*)(const char *,LLVMOpaqueMemoryBuffer **,char **) , LLVMCreateMemoryBufferWithContentsOfFile >(*this,lib,"LLVMCreateMemoryBufferWithContentsOfFile",SideEffects::worstDefault,"LLVMCreateMemoryBufferWithContentsOfFile")
		->args({"Path","OutMemBuf","OutMessage"});
// from D:\Work\libclang\include\llvm-c/Core.h:4065:10
	addExtern< int (*)(LLVMOpaqueMemoryBuffer **,char **) , LLVMCreateMemoryBufferWithSTDIN >(*this,lib,"LLVMCreateMemoryBufferWithSTDIN",SideEffects::worstDefault,"LLVMCreateMemoryBufferWithSTDIN")
		->args({"OutMemBuf","OutMessage"});
// from D:\Work\libclang\include\llvm-c/Core.h:4067:21
	addExtern< LLVMOpaqueMemoryBuffer * (*)(const char *,size_t,const char *,int) , LLVMCreateMemoryBufferWithMemoryRange >(*this,lib,"LLVMCreateMemoryBufferWithMemoryRange",SideEffects::worstDefault,"LLVMCreateMemoryBufferWithMemoryRange")
		->args({"InputData","InputDataLength","BufferName","RequiresNullTerminator"});
// from D:\Work\libclang\include\llvm-c/Core.h:4071:21
	addExtern< LLVMOpaqueMemoryBuffer * (*)(const char *,size_t,const char *) , LLVMCreateMemoryBufferWithMemoryRangeCopy >(*this,lib,"LLVMCreateMemoryBufferWithMemoryRangeCopy",SideEffects::worstDefault,"LLVMCreateMemoryBufferWithMemoryRangeCopy")
		->args({"InputData","InputDataLength","BufferName"});
// from D:\Work\libclang\include\llvm-c/Core.h:4074:13
	addExtern< const char * (*)(LLVMOpaqueMemoryBuffer *) , LLVMGetBufferStart >(*this,lib,"LLVMGetBufferStart",SideEffects::worstDefault,"LLVMGetBufferStart")
		->args({"MemBuf"});
// from D:\Work\libclang\include\llvm-c/Core.h:4075:8
	addExtern< size_t (*)(LLVMOpaqueMemoryBuffer *) , LLVMGetBufferSize >(*this,lib,"LLVMGetBufferSize",SideEffects::worstDefault,"LLVMGetBufferSize")
		->args({"MemBuf"});
}
}

