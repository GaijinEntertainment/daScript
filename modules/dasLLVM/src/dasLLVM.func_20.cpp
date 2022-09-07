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
void Module_dasLLVM::initFunctions_20() {
// from D:\Work\libclang\include\llvm-c/Core.h:2229:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMLinkage) , LLVMSetLinkage >(*this,lib,"LLVMSetLinkage",SideEffects::worstDefault,"LLVMSetLinkage")
		->args({"Global","Linkage"});
// from D:\Work\libclang\include\llvm-c/Core.h:2230:13
	addExtern< const char * (*)(LLVMOpaqueValue *) , LLVMGetSection >(*this,lib,"LLVMGetSection",SideEffects::worstDefault,"LLVMGetSection")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2231:6
	addExtern< void (*)(LLVMOpaqueValue *,const char *) , LLVMSetSection >(*this,lib,"LLVMSetSection",SideEffects::worstDefault,"LLVMSetSection")
		->args({"Global","Section"});
// from D:\Work\libclang\include\llvm-c/Core.h:2232:16
	addExtern< LLVMVisibility (*)(LLVMOpaqueValue *) , LLVMGetVisibility >(*this,lib,"LLVMGetVisibility",SideEffects::worstDefault,"LLVMGetVisibility")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2233:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMVisibility) , LLVMSetVisibility >(*this,lib,"LLVMSetVisibility",SideEffects::worstDefault,"LLVMSetVisibility")
		->args({"Global","Viz"});
// from D:\Work\libclang\include\llvm-c/Core.h:2234:21
	addExtern< LLVMDLLStorageClass (*)(LLVMOpaqueValue *) , LLVMGetDLLStorageClass >(*this,lib,"LLVMGetDLLStorageClass",SideEffects::worstDefault,"LLVMGetDLLStorageClass")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2235:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMDLLStorageClass) , LLVMSetDLLStorageClass >(*this,lib,"LLVMSetDLLStorageClass",SideEffects::worstDefault,"LLVMSetDLLStorageClass")
		->args({"Global","Class"});
// from D:\Work\libclang\include\llvm-c/Core.h:2236:17
	addExtern< LLVMUnnamedAddr (*)(LLVMOpaqueValue *) , LLVMGetUnnamedAddress >(*this,lib,"LLVMGetUnnamedAddress",SideEffects::worstDefault,"LLVMGetUnnamedAddress")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2237:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMUnnamedAddr) , LLVMSetUnnamedAddress >(*this,lib,"LLVMSetUnnamedAddress",SideEffects::worstDefault,"LLVMSetUnnamedAddress")
		->args({"Global","UnnamedAddr"});
// from D:\Work\libclang\include\llvm-c/Core.h:2245:13
	addExtern< LLVMOpaqueType * (*)(LLVMOpaqueValue *) , LLVMGlobalGetValueType >(*this,lib,"LLVMGlobalGetValueType",SideEffects::worstDefault,"LLVMGlobalGetValueType")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2248:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMHasUnnamedAddr >(*this,lib,"LLVMHasUnnamedAddr",SideEffects::worstDefault,"LLVMHasUnnamedAddr")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2250:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetUnnamedAddr >(*this,lib,"LLVMSetUnnamedAddr",SideEffects::worstDefault,"LLVMSetUnnamedAddr")
		->args({"Global","HasUnnamedAddr"});
// from D:\Work\libclang\include\llvm-c/Core.h:2268:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetAlignment >(*this,lib,"LLVMGetAlignment",SideEffects::worstDefault,"LLVMGetAlignment")
		->args({"V"});
// from D:\Work\libclang\include\llvm-c/Core.h:2279:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int) , LLVMSetAlignment >(*this,lib,"LLVMSetAlignment",SideEffects::worstDefault,"LLVMSetAlignment")
		->args({"V","Bytes"});
// from D:\Work\libclang\include\llvm-c/Core.h:2287:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int,LLVMOpaqueMetadata *) , LLVMGlobalSetMetadata >(*this,lib,"LLVMGlobalSetMetadata",SideEffects::worstDefault,"LLVMGlobalSetMetadata")
		->args({"Global","Kind","MD"});
// from D:\Work\libclang\include\llvm-c/Core.h:2295:6
	addExtern< void (*)(LLVMOpaqueValue *,unsigned int) , LLVMGlobalEraseMetadata >(*this,lib,"LLVMGlobalEraseMetadata",SideEffects::worstDefault,"LLVMGlobalEraseMetadata")
		->args({"Global","Kind"});
// from D:\Work\libclang\include\llvm-c/Core.h:2302:6
	addExtern< void (*)(LLVMOpaqueValue *) , LLVMGlobalClearMetadata >(*this,lib,"LLVMGlobalClearMetadata",SideEffects::worstDefault,"LLVMGlobalClearMetadata")
		->args({"Global"});
// from D:\Work\libclang\include\llvm-c/Core.h:2311:25
	addExtern< LLVMOpaqueValueMetadataEntry * (*)(LLVMOpaqueValue *,size_t *) , LLVMGlobalCopyAllMetadata >(*this,lib,"LLVMGlobalCopyAllMetadata",SideEffects::worstDefault,"LLVMGlobalCopyAllMetadata")
		->args({"Value","NumEntries"});
// from D:\Work\libclang\include\llvm-c/Core.h:2317:6
	addExtern< void (*)(LLVMOpaqueValueMetadataEntry *) , LLVMDisposeValueMetadataEntries >(*this,lib,"LLVMDisposeValueMetadataEntries",SideEffects::worstDefault,"LLVMDisposeValueMetadataEntries")
		->args({"Entries"});
// from D:\Work\libclang\include\llvm-c/Core.h:2322:10
	addExtern< unsigned int (*)(LLVMOpaqueValueMetadataEntry *,unsigned int) , LLVMValueMetadataEntriesGetKind >(*this,lib,"LLVMValueMetadataEntriesGetKind",SideEffects::worstDefault,"LLVMValueMetadataEntriesGetKind")
		->args({"Entries","Index"});
}
}

