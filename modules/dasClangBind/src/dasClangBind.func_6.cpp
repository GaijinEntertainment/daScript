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
void Module_dasClangBind::initFunctions_6() {
	addExtern< void (*)(CXPlatformAvailability *) , clang_disposeCXPlatformAvailability >(*this,lib,"clang_disposeCXPlatformAvailability",SideEffects::worstDefault,"clang_disposeCXPlatformAvailability")
		->args({"availability"});
	addExtern< CXCursor (*)(CXCursor) , clang_Cursor_getVarDeclInitializer ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getVarDeclInitializer",SideEffects::worstDefault,"clang_Cursor_getVarDeclInitializer")
		->args({"cursor"});
	addExtern< int (*)(CXCursor) , clang_Cursor_hasVarDeclGlobalStorage >(*this,lib,"clang_Cursor_hasVarDeclGlobalStorage",SideEffects::worstDefault,"clang_Cursor_hasVarDeclGlobalStorage")
		->args({"cursor"});
	addExtern< int (*)(CXCursor) , clang_Cursor_hasVarDeclExternalStorage >(*this,lib,"clang_Cursor_hasVarDeclExternalStorage",SideEffects::worstDefault,"clang_Cursor_hasVarDeclExternalStorage")
		->args({"cursor"});
	addExtern< CXLanguageKind (*)(CXCursor) , clang_getCursorLanguage >(*this,lib,"clang_getCursorLanguage",SideEffects::worstDefault,"clang_getCursorLanguage")
		->args({"cursor"});
	addExtern< CXTLSKind (*)(CXCursor) , clang_getCursorTLSKind >(*this,lib,"clang_getCursorTLSKind",SideEffects::worstDefault,"clang_getCursorTLSKind")
		->args({"cursor"});
	addExtern< CXTranslationUnitImpl * (*)(CXCursor) , clang_Cursor_getTranslationUnit >(*this,lib,"clang_Cursor_getTranslationUnit",SideEffects::worstDefault,"clang_Cursor_getTranslationUnit")
		->args({""});
	addExtern< CXCursorSetImpl * (*)() , clang_createCXCursorSet >(*this,lib,"clang_createCXCursorSet",SideEffects::worstDefault,"clang_createCXCursorSet");
	addExtern< void (*)(CXCursorSetImpl *) , clang_disposeCXCursorSet >(*this,lib,"clang_disposeCXCursorSet",SideEffects::worstDefault,"clang_disposeCXCursorSet")
		->args({"cset"});
	addExtern< unsigned int (*)(CXCursorSetImpl *,CXCursor) , clang_CXCursorSet_contains >(*this,lib,"clang_CXCursorSet_contains",SideEffects::worstDefault,"clang_CXCursorSet_contains")
		->args({"cset","cursor"});
	addExtern< unsigned int (*)(CXCursorSetImpl *,CXCursor) , clang_CXCursorSet_insert >(*this,lib,"clang_CXCursorSet_insert",SideEffects::worstDefault,"clang_CXCursorSet_insert")
		->args({"cset","cursor"});
	addExtern< CXCursor (*)(CXCursor) , clang_getCursorSemanticParent ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorSemanticParent",SideEffects::worstDefault,"clang_getCursorSemanticParent")
		->args({"cursor"});
	addExtern< CXCursor (*)(CXCursor) , clang_getCursorLexicalParent ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorLexicalParent",SideEffects::worstDefault,"clang_getCursorLexicalParent")
		->args({"cursor"});
	addExtern< void (*)(CXCursor,CXCursor **,unsigned int *) , clang_getOverriddenCursors >(*this,lib,"clang_getOverriddenCursors",SideEffects::worstDefault,"clang_getOverriddenCursors")
		->args({"cursor","overridden","num_overridden"});
	addExtern< void (*)(CXCursor *) , clang_disposeOverriddenCursors >(*this,lib,"clang_disposeOverriddenCursors",SideEffects::worstDefault,"clang_disposeOverriddenCursors")
		->args({"overridden"});
	addExtern< void * (*)(CXCursor) , clang_getIncludedFile >(*this,lib,"clang_getIncludedFile",SideEffects::worstDefault,"clang_getIncludedFile")
		->args({"cursor"});
	addExtern< CXCursor (*)(CXTranslationUnitImpl *,CXSourceLocation) , clang_getCursor ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursor",SideEffects::worstDefault,"clang_getCursor")
		->args({"",""});
	addExtern< CXSourceLocation (*)(CXCursor) , clang_getCursorLocation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorLocation",SideEffects::worstDefault,"clang_getCursorLocation")
		->args({""});
	addExtern< CXSourceRange (*)(CXCursor) , clang_getCursorExtent ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorExtent",SideEffects::worstDefault,"clang_getCursorExtent")
		->args({""});
	addExtern< CXType (*)(CXCursor) , clang_getCursorType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorType",SideEffects::worstDefault,"clang_getCursorType")
		->args({"C"});
}
}

