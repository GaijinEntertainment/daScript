// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasClangBind.h"
#include "need_dasClangBind.h"
namespace das {
#include "dasClangBind.func.aot.decl.inc"
void Module_dasClangBind::initFunctions_2() {
// from D:\Work\libclang\include\clang-c/CXSourceLocation.h:247:21
	makeExtern< void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *) , clang_getFileLocation , SimNode_ExtFuncCall >(lib,"clang_getFileLocation","clang_getFileLocation")
		->args({"location","file","line","column","offset"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXSourceLocation.h:255:33
	makeExtern< CXSourceLocation (*)(CXSourceRange) , clang_getRangeStart , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getRangeStart","clang_getRangeStart")
		->args({"range"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXSourceLocation.h:261:33
	makeExtern< CXSourceLocation (*)(CXSourceRange) , clang_getRangeEnd , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getRangeEnd","clang_getRangeEnd")
		->args({"range"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXSourceLocation.h:278:21
	makeExtern< void (*)(CXSourceRangeList *) , clang_disposeSourceRangeList , SimNode_ExtFuncCall >(lib,"clang_disposeSourceRangeList","clang_disposeSourceRangeList")
		->args({"ranges"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:79:25
	makeExtern< unsigned int (*)(void *) , clang_getNumDiagnosticsInSet , SimNode_ExtFuncCall >(lib,"clang_getNumDiagnosticsInSet","clang_getNumDiagnosticsInSet")
		->args({"Diags"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:90:29
	makeExtern< void * (*)(void *,unsigned int) , clang_getDiagnosticInSet , SimNode_ExtFuncCall >(lib,"clang_getDiagnosticInSet","clang_getDiagnosticInSet")
		->args({"Diags","Index"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:135:32
	makeExtern< void * (*)(const char *,CXLoadDiag_Error *,CXString *) , clang_loadDiagnostics , SimNode_ExtFuncCall >(lib,"clang_loadDiagnostics","clang_loadDiagnostics")
		->args({"file","error","errorString"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:141:21
	makeExtern< void (*)(void *) , clang_disposeDiagnosticSet , SimNode_ExtFuncCall >(lib,"clang_disposeDiagnosticSet","clang_disposeDiagnosticSet")
		->args({"Diags"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:149:32
	makeExtern< void * (*)(void *) , clang_getChildDiagnostics , SimNode_ExtFuncCall >(lib,"clang_getChildDiagnostics","clang_getChildDiagnostics")
		->args({"D"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:154:21
	makeExtern< void (*)(void *) , clang_disposeDiagnostic , SimNode_ExtFuncCall >(lib,"clang_disposeDiagnostic","clang_disposeDiagnostic")
		->args({"Diagnostic"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:239:25
	makeExtern< CXString (*)(void *,unsigned int) , clang_formatDiagnostic , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_formatDiagnostic","clang_formatDiagnostic")
		->args({"Diagnostic","Options"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:249:25
	makeExtern< unsigned int (*)() , clang_defaultDiagnosticDisplayOptions , SimNode_ExtFuncCall >(lib,"clang_defaultDiagnosticDisplayOptions","clang_defaultDiagnosticDisplayOptions")
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:255:5
	makeExtern< CXDiagnosticSeverity (*)(void *) , clang_getDiagnosticSeverity , SimNode_ExtFuncCall >(lib,"clang_getDiagnosticSeverity","clang_getDiagnosticSeverity")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:263:33
	makeExtern< CXSourceLocation (*)(void *) , clang_getDiagnosticLocation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getDiagnosticLocation","clang_getDiagnosticLocation")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:268:25
	makeExtern< CXString (*)(void *) , clang_getDiagnosticSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getDiagnosticSpelling","clang_getDiagnosticSpelling")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:282:25
	makeExtern< CXString (*)(void *,CXString *) , clang_getDiagnosticOption , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getDiagnosticOption","clang_getDiagnosticOption")
		->args({"Diag","Disable"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:295:25
	makeExtern< unsigned int (*)(void *) , clang_getDiagnosticCategory , SimNode_ExtFuncCall >(lib,"clang_getDiagnosticCategory","clang_getDiagnosticCategory")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:308:1
	makeExtern< CXString (*)(unsigned int) , clang_getDiagnosticCategoryName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getDiagnosticCategoryName","clang_getDiagnosticCategoryName")
		->args({"Category"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:315:25
	makeExtern< CXString (*)(void *) , clang_getDiagnosticCategoryText , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getDiagnosticCategoryText","clang_getDiagnosticCategoryText")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\libclang\include\clang-c/CXDiagnostic.h:321:25
	makeExtern< unsigned int (*)(void *) , clang_getDiagnosticNumRanges , SimNode_ExtFuncCall >(lib,"clang_getDiagnosticNumRanges","clang_getDiagnosticNumRanges")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
}
}

