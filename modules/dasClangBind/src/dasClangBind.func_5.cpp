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
void Module_dasClangBind::initFunctions_5() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2788:25
	addExtern< CXCursor (*)(CXTranslationUnitImpl *) , clang_getTranslationUnitCursor ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTranslationUnitCursor",SideEffects::worstDefault,"clang_getTranslationUnitCursor")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2793:25
	addExtern< unsigned int (*)(CXCursor,CXCursor) , clang_equalCursors >(*this,lib,"clang_equalCursors",SideEffects::worstDefault,"clang_equalCursors")
		->args({"",""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2798:20
	addExtern< int (*)(CXCursor) , clang_Cursor_isNull >(*this,lib,"clang_Cursor_isNull",SideEffects::worstDefault,"clang_Cursor_isNull")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2803:25
	addExtern< unsigned int (*)(CXCursor) , clang_hashCursor >(*this,lib,"clang_hashCursor",SideEffects::worstDefault,"clang_hashCursor")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2808:34
	addExtern< CXCursorKind (*)(CXCursor) , clang_getCursorKind >(*this,lib,"clang_getCursorKind",SideEffects::worstDefault,"clang_getCursorKind")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2813:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isDeclaration >(*this,lib,"clang_isDeclaration",SideEffects::worstDefault,"clang_isDeclaration")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2823:25
	addExtern< unsigned int (*)(CXCursor) , clang_isInvalidDeclaration >(*this,lib,"clang_isInvalidDeclaration",SideEffects::worstDefault,"clang_isInvalidDeclaration")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2833:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isReference >(*this,lib,"clang_isReference",SideEffects::worstDefault,"clang_isReference")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2838:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isExpression >(*this,lib,"clang_isExpression",SideEffects::worstDefault,"clang_isExpression")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2843:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isStatement >(*this,lib,"clang_isStatement",SideEffects::worstDefault,"clang_isStatement")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2848:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isAttribute >(*this,lib,"clang_isAttribute",SideEffects::worstDefault,"clang_isAttribute")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2853:25
	addExtern< unsigned int (*)(CXCursor) , clang_Cursor_hasAttrs >(*this,lib,"clang_Cursor_hasAttrs",SideEffects::worstDefault,"clang_Cursor_hasAttrs")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2859:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isInvalid >(*this,lib,"clang_isInvalid",SideEffects::worstDefault,"clang_isInvalid")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2865:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isTranslationUnit >(*this,lib,"clang_isTranslationUnit",SideEffects::worstDefault,"clang_isTranslationUnit")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2871:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isPreprocessing >(*this,lib,"clang_isPreprocessing",SideEffects::worstDefault,"clang_isPreprocessing")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2877:25
	addExtern< unsigned int (*)(CXCursorKind) , clang_isUnexposed >(*this,lib,"clang_isUnexposed",SideEffects::worstDefault,"clang_isUnexposed")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2903:35
	addExtern< CXLinkageKind (*)(CXCursor) , clang_getCursorLinkage >(*this,lib,"clang_getCursorLinkage",SideEffects::worstDefault,"clang_getCursorLinkage")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2929:38
	addExtern< CXVisibilityKind (*)(CXCursor) , clang_getCursorVisibility >(*this,lib,"clang_getCursorVisibility",SideEffects::worstDefault,"clang_getCursorVisibility")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2940:1
	addExtern< CXAvailabilityKind (*)(CXCursor) , clang_getCursorAvailability >(*this,lib,"clang_getCursorAvailability",SideEffects::worstDefault,"clang_getCursorAvailability")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3015:20
	addExtern< int (*)(CXCursor,int *,CXString *,int *,CXString *,CXPlatformAvailability *,int) , clang_getCursorPlatformAvailability >(*this,lib,"clang_getCursorPlatformAvailability",SideEffects::worstDefault,"clang_getCursorPlatformAvailability")
		->args({"cursor","always_deprecated","deprecated_message","always_unavailable","unavailable_message","availability","availability_size"});
}
}

