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
void Module_dasClangBind::initFunctions_14( ModuleLibrary & lib ) {
	addExtern<void (*)(void (*)(void *),void *,unsigned int),clang_executeOnThread>(*this,lib,"clang_executeOnThread",SideEffects::worstDefault,"clang_executeOnThread")
		->args({"fn","user_data","stack_size"});
	addExtern<CXCompletionChunkKind (*)(void *,unsigned int),clang_getCompletionChunkKind>(*this,lib,"clang_getCompletionChunkKind",SideEffects::worstDefault,"clang_getCompletionChunkKind")
		->args({"completion_string","chunk_number"});
	addExtern<CXString (*)(void *,unsigned int),clang_getCompletionChunkText,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionChunkText",SideEffects::worstDefault,"clang_getCompletionChunkText")
		->args({"completion_string","chunk_number"});
	addExtern<void * (*)(void *,unsigned int),clang_getCompletionChunkCompletionString>(*this,lib,"clang_getCompletionChunkCompletionString",SideEffects::worstDefault,"clang_getCompletionChunkCompletionString")
		->args({"completion_string","chunk_number"});
	addExtern<unsigned int (*)(void *),clang_getNumCompletionChunks>(*this,lib,"clang_getNumCompletionChunks",SideEffects::worstDefault,"clang_getNumCompletionChunks")
		->args({"completion_string"});
	addExtern<unsigned int (*)(void *),clang_getCompletionPriority>(*this,lib,"clang_getCompletionPriority",SideEffects::worstDefault,"clang_getCompletionPriority")
		->args({"completion_string"});
	addExtern<CXAvailabilityKind (*)(void *),clang_getCompletionAvailability>(*this,lib,"clang_getCompletionAvailability",SideEffects::worstDefault,"clang_getCompletionAvailability")
		->args({"completion_string"});
	addExtern<unsigned int (*)(void *),clang_getCompletionNumAnnotations>(*this,lib,"clang_getCompletionNumAnnotations",SideEffects::worstDefault,"clang_getCompletionNumAnnotations")
		->args({"completion_string"});
	addExtern<CXString (*)(void *,unsigned int),clang_getCompletionAnnotation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionAnnotation",SideEffects::worstDefault,"clang_getCompletionAnnotation")
		->args({"completion_string","annotation_number"});
	addExtern<CXString (*)(void *,CXCursorKind *),clang_getCompletionParent,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionParent",SideEffects::worstDefault,"clang_getCompletionParent")
		->args({"completion_string","kind"});
	addExtern<CXString (*)(void *),clang_getCompletionBriefComment,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionBriefComment",SideEffects::worstDefault,"clang_getCompletionBriefComment")
		->args({"completion_string"});
	addExtern<void * (*)(CXCursor),clang_getCursorCompletionString>(*this,lib,"clang_getCursorCompletionString",SideEffects::worstDefault,"clang_getCursorCompletionString")
		->args({"cursor"});
	addExtern<unsigned int (*)(CXCodeCompleteResults *,unsigned int),clang_getCompletionNumFixIts>(*this,lib,"clang_getCompletionNumFixIts",SideEffects::worstDefault,"clang_getCompletionNumFixIts")
		->args({"results","completion_index"});
	addExtern<CXString (*)(CXCodeCompleteResults *,unsigned int,unsigned int,CXSourceRange *),clang_getCompletionFixIt,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionFixIt",SideEffects::worstDefault,"clang_getCompletionFixIt")
		->args({"results","completion_index","fixit_index","replacement_range"});
	addExtern<unsigned int (*)(),clang_defaultCodeCompleteOptions>(*this,lib,"clang_defaultCodeCompleteOptions",SideEffects::worstDefault,"clang_defaultCodeCompleteOptions");
	addExtern<CXCodeCompleteResults * (*)(CXTranslationUnitImpl *,const char *,unsigned int,unsigned int,CXUnsavedFile *,unsigned int,unsigned int),clang_codeCompleteAt>(*this,lib,"clang_codeCompleteAt",SideEffects::worstDefault,"clang_codeCompleteAt")
		->args({"TU","complete_filename","complete_line","complete_column","unsaved_files","num_unsaved_files","options"});
	addExtern<void (*)(CXCompletionResult *,unsigned int),clang_sortCodeCompletionResults>(*this,lib,"clang_sortCodeCompletionResults",SideEffects::worstDefault,"clang_sortCodeCompletionResults")
		->args({"Results","NumResults"});
	addExtern<void (*)(CXCodeCompleteResults *),clang_disposeCodeCompleteResults>(*this,lib,"clang_disposeCodeCompleteResults",SideEffects::worstDefault,"clang_disposeCodeCompleteResults")
		->args({"Results"});
	addExtern<unsigned int (*)(CXCodeCompleteResults *),clang_codeCompleteGetNumDiagnostics>(*this,lib,"clang_codeCompleteGetNumDiagnostics",SideEffects::worstDefault,"clang_codeCompleteGetNumDiagnostics")
		->args({"Results"});
	addExtern<void * (*)(CXCodeCompleteResults *,unsigned int),clang_codeCompleteGetDiagnostic>(*this,lib,"clang_codeCompleteGetDiagnostic",SideEffects::worstDefault,"clang_codeCompleteGetDiagnostic")
		->args({"Results","Index"});
}
}

