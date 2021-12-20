// this file is generated via dasClangBind
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
void Module_dasClangBind::initFunctions_2( ModuleLibrary & lib ) {
	addExtern<int (*)(CXSourceLocation),clang_Location_isInSystemHeader>(*this,lib,"clang_Location_isInSystemHeader",SideEffects::worstDefault,"clang_Location_isInSystemHeader")
		->args({"location"});
	addExtern<int (*)(CXSourceLocation),clang_Location_isFromMainFile>(*this,lib,"clang_Location_isFromMainFile",SideEffects::worstDefault,"clang_Location_isFromMainFile")
		->args({"location"});
	addExtern<CXSourceRange (*)(),clang_getNullRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getNullRange",SideEffects::worstDefault,"clang_getNullRange");
	addExtern<CXSourceRange (*)(CXSourceLocation,CXSourceLocation),clang_getRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getRange",SideEffects::worstDefault,"clang_getRange")
		->args({"begin","end"});
	addExtern<unsigned int (*)(CXSourceRange,CXSourceRange),clang_equalRanges>(*this,lib,"clang_equalRanges",SideEffects::worstDefault,"clang_equalRanges")
		->args({"range1","range2"});
	addExtern<int (*)(CXSourceRange),clang_Range_isNull>(*this,lib,"clang_Range_isNull",SideEffects::worstDefault,"clang_Range_isNull")
		->args({"range"});
	addExtern<void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *),clang_getExpansionLocation>(*this,lib,"clang_getExpansionLocation",SideEffects::worstDefault,"clang_getExpansionLocation")
		->args({"location","file","line","column","offset"});
	addExtern<void (*)(CXSourceLocation,CXString *,unsigned int *,unsigned int *),clang_getPresumedLocation>(*this,lib,"clang_getPresumedLocation",SideEffects::worstDefault,"clang_getPresumedLocation")
		->args({"location","filename","line","column"});
	addExtern<void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *),clang_getInstantiationLocation>(*this,lib,"clang_getInstantiationLocation",SideEffects::worstDefault,"clang_getInstantiationLocation")
		->args({"location","file","line","column","offset"});
	addExtern<void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *),clang_getSpellingLocation>(*this,lib,"clang_getSpellingLocation",SideEffects::worstDefault,"clang_getSpellingLocation")
		->args({"location","file","line","column","offset"});
	addExtern<void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *),clang_getFileLocation>(*this,lib,"clang_getFileLocation",SideEffects::worstDefault,"clang_getFileLocation")
		->args({"location","file","line","column","offset"});
	addExtern<CXSourceLocation (*)(CXSourceRange),clang_getRangeStart,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getRangeStart",SideEffects::worstDefault,"clang_getRangeStart")
		->args({"range"});
	addExtern<CXSourceLocation (*)(CXSourceRange),clang_getRangeEnd,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getRangeEnd",SideEffects::worstDefault,"clang_getRangeEnd")
		->args({"range"});
	addExtern<CXSourceRangeList * (*)(CXTranslationUnitImpl *,void *),clang_getSkippedRanges>(*this,lib,"clang_getSkippedRanges",SideEffects::worstDefault,"clang_getSkippedRanges")
		->args({"tu","file"});
	addExtern<CXSourceRangeList * (*)(CXTranslationUnitImpl *),clang_getAllSkippedRanges>(*this,lib,"clang_getAllSkippedRanges",SideEffects::worstDefault,"clang_getAllSkippedRanges")
		->args({"tu"});
	addExtern<void (*)(CXSourceRangeList *),clang_disposeSourceRangeList>(*this,lib,"clang_disposeSourceRangeList",SideEffects::worstDefault,"clang_disposeSourceRangeList")
		->args({"ranges"});
	addExtern<unsigned int (*)(void *),clang_getNumDiagnosticsInSet>(*this,lib,"clang_getNumDiagnosticsInSet",SideEffects::worstDefault,"clang_getNumDiagnosticsInSet")
		->args({"Diags"});
	addExtern<void * (*)(void *,unsigned int),clang_getDiagnosticInSet>(*this,lib,"clang_getDiagnosticInSet",SideEffects::worstDefault,"clang_getDiagnosticInSet")
		->args({"Diags","Index"});
	addExtern<void * (*)(const char *,CXLoadDiag_Error *,CXString *),clang_loadDiagnostics>(*this,lib,"clang_loadDiagnostics",SideEffects::worstDefault,"clang_loadDiagnostics")
		->args({"file","error","errorString"});
	addExtern<void (*)(void *),clang_disposeDiagnosticSet>(*this,lib,"clang_disposeDiagnosticSet",SideEffects::worstDefault,"clang_disposeDiagnosticSet")
		->args({"Diags"});
}
}

