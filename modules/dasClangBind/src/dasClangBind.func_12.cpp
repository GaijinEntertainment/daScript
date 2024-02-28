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
void Module_dasClangBind::initFunctions_12() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4605:21
	makeExtern< void (*)(CXTranslationUnitImpl *,CXToken *,unsigned int) , clang_disposeTokens , SimNode_ExtFuncCall >(lib,"clang_disposeTokens","clang_disposeTokens")
		->args({"TU","Tokens","NumTokens"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4622:25
	makeExtern< CXString (*)(CXCursorKind) , clang_getCursorKindSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorKindSpelling","clang_getCursorKindSpelling")
		->args({"Kind"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4623:21
	makeExtern< void (*)(CXCursor,const char **,const char **,unsigned int *,unsigned int *,unsigned int *,unsigned int *) , clang_getDefinitionSpellingAndExtent , SimNode_ExtFuncCall >(lib,"clang_getDefinitionSpellingAndExtent","clang_getDefinitionSpellingAndExtent")
		->args({"","startBuf","endBuf","startLine","startColumn","endLine","endColumn"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4626:21
	makeExtern< void (*)() , clang_enableStackTraces , SimNode_ExtFuncCall >(lib,"clang_enableStackTraces","clang_enableStackTraces")
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4865:1
	makeExtern< CXCompletionChunkKind (*)(void *,unsigned int) , clang_getCompletionChunkKind , SimNode_ExtFuncCall >(lib,"clang_getCompletionChunkKind","clang_getCompletionChunkKind")
		->args({"completion_string","chunk_number"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4878:25
	makeExtern< CXString (*)(void *,unsigned int) , clang_getCompletionChunkText , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCompletionChunkText","clang_getCompletionChunkText")
		->args({"completion_string","chunk_number"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4892:35
	makeExtern< void * (*)(void *,unsigned int) , clang_getCompletionChunkCompletionString , SimNode_ExtFuncCall >(lib,"clang_getCompletionChunkCompletionString","clang_getCompletionChunkCompletionString")
		->args({"completion_string","chunk_number"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4899:1
	makeExtern< unsigned int (*)(void *) , clang_getNumCompletionChunks , SimNode_ExtFuncCall >(lib,"clang_getNumCompletionChunks","clang_getNumCompletionChunks")
		->args({"completion_string"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4914:1
	makeExtern< unsigned int (*)(void *) , clang_getCompletionPriority , SimNode_ExtFuncCall >(lib,"clang_getCompletionPriority","clang_getCompletionPriority")
		->args({"completion_string"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4925:1
	makeExtern< CXAvailabilityKind (*)(void *) , clang_getCompletionAvailability , SimNode_ExtFuncCall >(lib,"clang_getCompletionAvailability","clang_getCompletionAvailability")
		->args({"completion_string"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4937:1
	makeExtern< unsigned int (*)(void *) , clang_getCompletionNumAnnotations , SimNode_ExtFuncCall >(lib,"clang_getCompletionNumAnnotations","clang_getCompletionNumAnnotations")
		->args({"completion_string"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4950:25
	makeExtern< CXString (*)(void *,unsigned int) , clang_getCompletionAnnotation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCompletionAnnotation","clang_getCompletionAnnotation")
		->args({"completion_string","annotation_number"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4969:25
	makeExtern< CXString (*)(void *,CXCursorKind *) , clang_getCompletionParent , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCompletionParent","clang_getCompletionParent")
		->args({"completion_string","kind"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4977:1
	makeExtern< CXString (*)(void *) , clang_getCompletionBriefComment , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCompletionBriefComment","clang_getCompletionBriefComment")
		->args({"completion_string"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4989:1
	makeExtern< void * (*)(CXCursor) , clang_getCursorCompletionString , SimNode_ExtFuncCall >(lib,"clang_getCursorCompletionString","clang_getCursorCompletionString")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5025:1
	makeExtern< unsigned int (*)(CXCodeCompleteResults *,unsigned int) , clang_getCompletionNumFixIts , SimNode_ExtFuncCall >(lib,"clang_getCompletionNumFixIts","clang_getCompletionNumFixIts")
		->args({"results","completion_index"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5071:25
	makeExtern< CXString (*)(CXCodeCompleteResults *,unsigned int,unsigned int,CXSourceRange *) , clang_getCompletionFixIt , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCompletionFixIt","clang_getCompletionFixIt")
		->args({"results","completion_index","fixit_index","replacement_range"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5255:25
	makeExtern< unsigned int (*)() , clang_defaultCodeCompleteOptions , SimNode_ExtFuncCall >(lib,"clang_defaultCodeCompleteOptions","clang_defaultCodeCompleteOptions")
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5327:1
	makeExtern< CXCodeCompleteResults * (*)(CXTranslationUnitImpl *,const char *,unsigned int,unsigned int,CXUnsavedFile *,unsigned int,unsigned int) , clang_codeCompleteAt , SimNode_ExtFuncCall >(lib,"clang_codeCompleteAt","clang_codeCompleteAt")
		->args({"TU","complete_filename","complete_line","complete_column","unsaved_files","num_unsaved_files","options"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5340:6
	makeExtern< void (*)(CXCompletionResult *,unsigned int) , clang_sortCodeCompletionResults , SimNode_ExtFuncCall >(lib,"clang_sortCodeCompletionResults","clang_sortCodeCompletionResults")
		->args({"Results","NumResults"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

