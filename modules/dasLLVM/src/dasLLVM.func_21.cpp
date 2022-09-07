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
void Module_dasLLVM::initFunctions_21() {
// from D:\Work\libclang\include\llvm-c/Core.h:2330:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueValueMetadataEntry *,unsigned int) , LLVMValueMetadataEntriesGetMetadata >(*this,lib,"LLVMValueMetadataEntriesGetMetadata",SideEffects::worstDefault,"LLVMValueMetadataEntriesGetMetadata")
		->args({"Entries","Index"});
// from D:\Work\libclang\include\llvm-c/Core.h:2346:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *,LLVMOpaqueType *,const char *) , LLVMAddGlobal >(*this,lib,"LLVMAddGlobal",SideEffects::worstDefault,"LLVMAddGlobal")
		->args({"M","Ty","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:2347:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *,LLVMOpaqueType *,const char *,unsigned int) , LLVMAddGlobalInAddressSpace >(*this,lib,"LLVMAddGlobalInAddressSpace",SideEffects::worstDefault,"LLVMAddGlobalInAddressSpace")
		->args({"M","Ty","Name","AddressSpace"});
// from D:\Work\libclang\include\llvm-c/Core.h:2350:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *,const char *) , LLVMGetNamedGlobal >(*this,lib,"LLVMGetNamedGlobal",SideEffects::worstDefault,"LLVMGetNamedGlobal")
		->args({"M","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:2351:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *) , LLVMGetFirstGlobal >(*this,lib,"LLVMGetFirstGlobal",SideEffects::worstDefault,"LLVMGetFirstGlobal")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:2352:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *) , LLVMGetLastGlobal >(*this,lib,"LLVMGetLastGlobal",SideEffects::worstDefault,"LLVMGetLastGlobal")
		->args({"M"});
// from D:\Work\libclang\include\llvm-c/Core.h:2353:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetNextGlobal >(*this,lib,"LLVMGetNextGlobal",SideEffects::worstDefault,"LLVMGetNextGlobal")
		->args({"GlobalVar"});
// from D:\Work\libclang\include\llvm-c/Core.h:2354:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetPreviousGlobal >(*this,lib,"LLVMGetPreviousGlobal",SideEffects::worstDefault,"LLVMGetPreviousGlobal")
		->args({"GlobalVar"});
// from D:\Work\libclang\include\llvm-c/Core.h:2355:6
	addExtern< void (*)(LLVMOpaqueValue *) , LLVMDeleteGlobal >(*this,lib,"LLVMDeleteGlobal",SideEffects::worstDefault,"LLVMDeleteGlobal")
		->args({"GlobalVar"});
// from D:\Work\libclang\include\llvm-c/Core.h:2356:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *) , LLVMGetInitializer >(*this,lib,"LLVMGetInitializer",SideEffects::worstDefault,"LLVMGetInitializer")
		->args({"GlobalVar"});
// from D:\Work\libclang\include\llvm-c/Core.h:2357:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMSetInitializer >(*this,lib,"LLVMSetInitializer",SideEffects::worstDefault,"LLVMSetInitializer")
		->args({"GlobalVar","ConstantVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:2358:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsThreadLocal >(*this,lib,"LLVMIsThreadLocal",SideEffects::worstDefault,"LLVMIsThreadLocal")
		->args({"GlobalVar"});
// from D:\Work\libclang\include\llvm-c/Core.h:2359:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetThreadLocal >(*this,lib,"LLVMSetThreadLocal",SideEffects::worstDefault,"LLVMSetThreadLocal")
		->args({"GlobalVar","IsThreadLocal"});
// from D:\Work\libclang\include\llvm-c/Core.h:2360:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsGlobalConstant >(*this,lib,"LLVMIsGlobalConstant",SideEffects::worstDefault,"LLVMIsGlobalConstant")
		->args({"GlobalVar"});
// from D:\Work\libclang\include\llvm-c/Core.h:2361:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetGlobalConstant >(*this,lib,"LLVMSetGlobalConstant",SideEffects::worstDefault,"LLVMSetGlobalConstant")
		->args({"GlobalVar","IsConstant"});
// from D:\Work\libclang\include\llvm-c/Core.h:2362:21
	addExtern< LLVMThreadLocalMode (*)(LLVMOpaqueValue *) , LLVMGetThreadLocalMode >(*this,lib,"LLVMGetThreadLocalMode",SideEffects::worstDefault,"LLVMGetThreadLocalMode")
		->args({"GlobalVar"});
// from D:\Work\libclang\include\llvm-c/Core.h:2363:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMThreadLocalMode) , LLVMSetThreadLocalMode >(*this,lib,"LLVMSetThreadLocalMode",SideEffects::worstDefault,"LLVMSetThreadLocalMode")
		->args({"GlobalVar","Mode"});
// from D:\Work\libclang\include\llvm-c/Core.h:2364:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsExternallyInitialized >(*this,lib,"LLVMIsExternallyInitialized",SideEffects::worstDefault,"LLVMIsExternallyInitialized")
		->args({"GlobalVar"});
// from D:\Work\libclang\include\llvm-c/Core.h:2365:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetExternallyInitialized >(*this,lib,"LLVMSetExternallyInitialized",SideEffects::worstDefault,"LLVMSetExternallyInitialized")
		->args({"GlobalVar","IsExtInit"});
// from D:\Work\libclang\include\llvm-c/Core.h:2380:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueModule *,LLVMOpaqueType *,LLVMOpaqueValue *,const char *) , LLVMAddAlias >(*this,lib,"LLVMAddAlias",SideEffects::worstDefault,"LLVMAddAlias")
		->args({"M","Ty","Aliasee","Name"});
}
}

