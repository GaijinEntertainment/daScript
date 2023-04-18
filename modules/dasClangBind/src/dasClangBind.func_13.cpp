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
void Module_dasClangBind::initFunctions_13() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4889:25
	addExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isStatic >(*this,lib,"clang_CXXMethod_isStatic",SideEffects::worstDefault,"clang_CXXMethod_isStatic")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4896:25
	addExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isVirtual >(*this,lib,"clang_CXXMethod_isVirtual",SideEffects::worstDefault,"clang_CXXMethod_isVirtual")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4902:25
	addExtern< unsigned int (*)(CXCursor) , clang_CXXRecord_isAbstract >(*this,lib,"clang_CXXRecord_isAbstract",SideEffects::worstDefault,"clang_CXXRecord_isAbstract")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4907:25
	addExtern< unsigned int (*)(CXCursor) , clang_EnumDecl_isScoped >(*this,lib,"clang_EnumDecl_isScoped",SideEffects::worstDefault,"clang_EnumDecl_isScoped")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4913:25
	addExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isConst >(*this,lib,"clang_CXXMethod_isConst",SideEffects::worstDefault,"clang_CXXMethod_isConst")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4932:34
	addExtern< CXCursorKind (*)(CXCursor) , clang_getTemplateCursorKind >(*this,lib,"clang_getTemplateCursorKind",SideEffects::worstDefault,"clang_getTemplateCursorKind")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4962:25
	addExtern< CXCursor (*)(CXCursor) , clang_getSpecializedCursorTemplate ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getSpecializedCursorTemplate",SideEffects::worstDefault,"clang_getSpecializedCursorTemplate")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4982:30
	addExtern< CXSourceRange (*)(CXCursor,unsigned int,unsigned int) , clang_getCursorReferenceNameRange ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorReferenceNameRange",SideEffects::worstDefault,"clang_getCursorReferenceNameRange")
		->args({"C","NameFlags","PieceIndex"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5074:25
	addExtern< CXToken * (*)(CXTranslationUnitImpl *,CXSourceLocation) , clang_getToken >(*this,lib,"clang_getToken",SideEffects::worstDefault,"clang_getToken")
		->args({"TU","Location"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5080:28
	addExtern< CXTokenKind (*)(CXToken) , clang_getTokenKind >(*this,lib,"clang_getTokenKind",SideEffects::worstDefault,"clang_getTokenKind")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5088:25
	addExtern< CXString (*)(CXTranslationUnitImpl *,CXToken) , clang_getTokenSpelling ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTokenSpelling",SideEffects::worstDefault,"clang_getTokenSpelling")
		->args({"",""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5093:33
	addExtern< CXSourceLocation (*)(CXTranslationUnitImpl *,CXToken) , clang_getTokenLocation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTokenLocation",SideEffects::worstDefault,"clang_getTokenLocation")
		->args({"",""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5099:30
	addExtern< CXSourceRange (*)(CXTranslationUnitImpl *,CXToken) , clang_getTokenExtent ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTokenExtent",SideEffects::worstDefault,"clang_getTokenExtent")
		->args({"",""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5118:21
	addExtern< void (*)(CXTranslationUnitImpl *,CXSourceRange,CXToken **,unsigned int *) , clang_tokenize >(*this,lib,"clang_tokenize",SideEffects::worstDefault,"clang_tokenize")
		->args({"TU","Range","Tokens","NumTokens"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5151:21
	addExtern< void (*)(CXTranslationUnitImpl *,CXToken *,unsigned int,CXCursor *) , clang_annotateTokens >(*this,lib,"clang_annotateTokens",SideEffects::worstDefault,"clang_annotateTokens")
		->args({"TU","Tokens","NumTokens","Cursors"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5157:21
	addExtern< void (*)(CXTranslationUnitImpl *,CXToken *,unsigned int) , clang_disposeTokens >(*this,lib,"clang_disposeTokens",SideEffects::worstDefault,"clang_disposeTokens")
		->args({"TU","Tokens","NumTokens"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5174:25
	addExtern< CXString (*)(CXCursorKind) , clang_getCursorKindSpelling ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorKindSpelling",SideEffects::worstDefault,"clang_getCursorKindSpelling")
		->args({"Kind"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5175:21
	addExtern< void (*)(CXCursor,const char **,const char **,unsigned int *,unsigned int *,unsigned int *,unsigned int *) , clang_getDefinitionSpellingAndExtent >(*this,lib,"clang_getDefinitionSpellingAndExtent",SideEffects::worstDefault,"clang_getDefinitionSpellingAndExtent")
		->args({"","startBuf","endBuf","startLine","startColumn","endLine","endColumn"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5178:21
	addExtern< void (*)() , clang_enableStackTraces >(*this,lib,"clang_enableStackTraces",SideEffects::worstDefault,"clang_enableStackTraces");
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5417:1
	addExtern< CXCompletionChunkKind (*)(void *,unsigned int) , clang_getCompletionChunkKind >(*this,lib,"clang_getCompletionChunkKind",SideEffects::worstDefault,"clang_getCompletionChunkKind")
		->args({"completion_string","chunk_number"});
}
}

