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
void Module_dasClangBind::initFunctions_5( ModuleLibrary & lib ) {
	addExtern<CXCursor (*)(CXTranslationUnitImpl *),clang_getTranslationUnitCursor,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTranslationUnitCursor",SideEffects::worstDefault,"clang_getTranslationUnitCursor")
		->args({""});
	addExtern<unsigned int (*)(CXCursor,CXCursor),clang_equalCursors>(*this,lib,"clang_equalCursors",SideEffects::worstDefault,"clang_equalCursors")
		->args({"",""});
	addExtern<int (*)(CXCursor),clang_Cursor_isNull>(*this,lib,"clang_Cursor_isNull",SideEffects::worstDefault,"clang_Cursor_isNull")
		->args({"cursor"});
	addExtern<unsigned int (*)(CXCursor),clang_hashCursor>(*this,lib,"clang_hashCursor",SideEffects::worstDefault,"clang_hashCursor")
		->args({""});
	addExtern<CXCursorKind (*)(CXCursor),clang_getCursorKind>(*this,lib,"clang_getCursorKind",SideEffects::worstDefault,"clang_getCursorKind")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isDeclaration>(*this,lib,"clang_isDeclaration",SideEffects::worstDefault,"clang_isDeclaration")
		->args({""});
	addExtern<unsigned int (*)(CXCursor),clang_isInvalidDeclaration>(*this,lib,"clang_isInvalidDeclaration",SideEffects::worstDefault,"clang_isInvalidDeclaration")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isReference>(*this,lib,"clang_isReference",SideEffects::worstDefault,"clang_isReference")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isExpression>(*this,lib,"clang_isExpression",SideEffects::worstDefault,"clang_isExpression")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isStatement>(*this,lib,"clang_isStatement",SideEffects::worstDefault,"clang_isStatement")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isAttribute>(*this,lib,"clang_isAttribute",SideEffects::worstDefault,"clang_isAttribute")
		->args({""});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_hasAttrs>(*this,lib,"clang_Cursor_hasAttrs",SideEffects::worstDefault,"clang_Cursor_hasAttrs")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursorKind),clang_isInvalid>(*this,lib,"clang_isInvalid",SideEffects::worstDefault,"clang_isInvalid")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isTranslationUnit>(*this,lib,"clang_isTranslationUnit",SideEffects::worstDefault,"clang_isTranslationUnit")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isPreprocessing>(*this,lib,"clang_isPreprocessing",SideEffects::worstDefault,"clang_isPreprocessing")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isUnexposed>(*this,lib,"clang_isUnexposed",SideEffects::worstDefault,"clang_isUnexposed")
		->args({""});
	addExtern<CXLinkageKind (*)(CXCursor),clang_getCursorLinkage>(*this,lib,"clang_getCursorLinkage",SideEffects::worstDefault,"clang_getCursorLinkage")
		->args({"cursor"});
	addExtern<CXVisibilityKind (*)(CXCursor),clang_getCursorVisibility>(*this,lib,"clang_getCursorVisibility",SideEffects::worstDefault,"clang_getCursorVisibility")
		->args({"cursor"});
	addExtern<CXAvailabilityKind (*)(CXCursor),clang_getCursorAvailability>(*this,lib,"clang_getCursorAvailability",SideEffects::worstDefault,"clang_getCursorAvailability")
		->args({"cursor"});
	addExtern<int (*)(CXCursor,int *,CXString *,int *,CXString *,CXPlatformAvailability *,int),clang_getCursorPlatformAvailability>(*this,lib,"clang_getCursorPlatformAvailability",SideEffects::worstDefault,"clang_getCursorPlatformAvailability")
		->args({"cursor","always_deprecated","deprecated_message","always_unavailable","unavailable_message","availability","availability_size"});
}
}

