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
void Module_dasClangBind::initFunctions_14() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5430:25
	addExtern< CXString (*)(void *,unsigned int) , clang_getCompletionChunkText ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionChunkText",SideEffects::worstDefault,"clang_getCompletionChunkText")
		->args({"completion_string","chunk_number"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5444:35
	addExtern< void * (*)(void *,unsigned int) , clang_getCompletionChunkCompletionString >(*this,lib,"clang_getCompletionChunkCompletionString",SideEffects::worstDefault,"clang_getCompletionChunkCompletionString")
		->args({"completion_string","chunk_number"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5451:1
	addExtern< unsigned int (*)(void *) , clang_getNumCompletionChunks >(*this,lib,"clang_getNumCompletionChunks",SideEffects::worstDefault,"clang_getNumCompletionChunks")
		->args({"completion_string"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5466:1
	addExtern< unsigned int (*)(void *) , clang_getCompletionPriority >(*this,lib,"clang_getCompletionPriority",SideEffects::worstDefault,"clang_getCompletionPriority")
		->args({"completion_string"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5477:1
	addExtern< CXAvailabilityKind (*)(void *) , clang_getCompletionAvailability >(*this,lib,"clang_getCompletionAvailability",SideEffects::worstDefault,"clang_getCompletionAvailability")
		->args({"completion_string"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5489:1
	addExtern< unsigned int (*)(void *) , clang_getCompletionNumAnnotations >(*this,lib,"clang_getCompletionNumAnnotations",SideEffects::worstDefault,"clang_getCompletionNumAnnotations")
		->args({"completion_string"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5502:25
	addExtern< CXString (*)(void *,unsigned int) , clang_getCompletionAnnotation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionAnnotation",SideEffects::worstDefault,"clang_getCompletionAnnotation")
		->args({"completion_string","annotation_number"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5521:25
	addExtern< CXString (*)(void *,CXCursorKind *) , clang_getCompletionParent ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionParent",SideEffects::worstDefault,"clang_getCompletionParent")
		->args({"completion_string","kind"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5529:1
	addExtern< CXString (*)(void *) , clang_getCompletionBriefComment ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionBriefComment",SideEffects::worstDefault,"clang_getCompletionBriefComment")
		->args({"completion_string"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5541:1
	addExtern< void * (*)(CXCursor) , clang_getCursorCompletionString >(*this,lib,"clang_getCursorCompletionString",SideEffects::worstDefault,"clang_getCursorCompletionString")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5577:1
	addExtern< unsigned int (*)(CXCodeCompleteResults *,unsigned int) , clang_getCompletionNumFixIts >(*this,lib,"clang_getCompletionNumFixIts",SideEffects::worstDefault,"clang_getCompletionNumFixIts")
		->args({"results","completion_index"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5623:25
	addExtern< CXString (*)(CXCodeCompleteResults *,unsigned int,unsigned int,CXSourceRange *) , clang_getCompletionFixIt ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionFixIt",SideEffects::worstDefault,"clang_getCompletionFixIt")
		->args({"results","completion_index","fixit_index","replacement_range"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5807:25
	addExtern< unsigned int (*)() , clang_defaultCodeCompleteOptions >(*this,lib,"clang_defaultCodeCompleteOptions",SideEffects::worstDefault,"clang_defaultCodeCompleteOptions");
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5879:1
	addExtern< CXCodeCompleteResults * (*)(CXTranslationUnitImpl *,const char *,unsigned int,unsigned int,CXUnsavedFile *,unsigned int,unsigned int) , clang_codeCompleteAt >(*this,lib,"clang_codeCompleteAt",SideEffects::worstDefault,"clang_codeCompleteAt")
		->args({"TU","complete_filename","complete_line","complete_column","unsaved_files","num_unsaved_files","options"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5892:6
	addExtern< void (*)(CXCompletionResult *,unsigned int) , clang_sortCodeCompletionResults >(*this,lib,"clang_sortCodeCompletionResults",SideEffects::worstDefault,"clang_sortCodeCompletionResults")
		->args({"Results","NumResults"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5899:6
	addExtern< void (*)(CXCodeCompleteResults *) , clang_disposeCodeCompleteResults >(*this,lib,"clang_disposeCodeCompleteResults",SideEffects::worstDefault,"clang_disposeCodeCompleteResults")
		->args({"Results"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5906:10
	addExtern< unsigned int (*)(CXCodeCompleteResults *) , clang_codeCompleteGetNumDiagnostics >(*this,lib,"clang_codeCompleteGetNumDiagnostics",SideEffects::worstDefault,"clang_codeCompleteGetNumDiagnostics")
		->args({"Results"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5918:14
	addExtern< void * (*)(CXCodeCompleteResults *,unsigned int) , clang_codeCompleteGetDiagnostic >(*this,lib,"clang_codeCompleteGetDiagnostic",SideEffects::worstDefault,"clang_codeCompleteGetDiagnostic")
		->args({"Results","Index"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5932:1
	addExtern< unsigned long long (*)(CXCodeCompleteResults *) , clang_codeCompleteGetContexts >(*this,lib,"clang_codeCompleteGetContexts",SideEffects::worstDefault,"clang_codeCompleteGetContexts")
		->args({"Results"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5952:1
	addExtern< CXCursorKind (*)(CXCodeCompleteResults *,unsigned int *) , clang_codeCompleteGetContainerKind >(*this,lib,"clang_codeCompleteGetContainerKind",SideEffects::worstDefault,"clang_codeCompleteGetContainerKind")
		->args({"Results","IsIncomplete"});
}
}

