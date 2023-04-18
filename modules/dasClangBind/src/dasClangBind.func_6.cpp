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
void Module_dasClangBind::initFunctions_6() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3024:1
	addExtern< void (*)(CXPlatformAvailability *) , clang_disposeCXPlatformAvailability >(*this,lib,"clang_disposeCXPlatformAvailability",SideEffects::worstDefault,"clang_disposeCXPlatformAvailability")
		->args({"availability"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3030:25
	addExtern< CXCursor (*)(CXCursor) , clang_Cursor_getVarDeclInitializer ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getVarDeclInitializer",SideEffects::worstDefault,"clang_Cursor_getVarDeclInitializer")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3037:20
	addExtern< int (*)(CXCursor) , clang_Cursor_hasVarDeclGlobalStorage >(*this,lib,"clang_Cursor_hasVarDeclGlobalStorage",SideEffects::worstDefault,"clang_Cursor_hasVarDeclGlobalStorage")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3044:20
	addExtern< int (*)(CXCursor) , clang_Cursor_hasVarDeclExternalStorage >(*this,lib,"clang_Cursor_hasVarDeclExternalStorage",SideEffects::worstDefault,"clang_Cursor_hasVarDeclExternalStorage")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3059:36
	addExtern< CXLanguageKind (*)(CXCursor) , clang_getCursorLanguage >(*this,lib,"clang_getCursorLanguage",SideEffects::worstDefault,"clang_getCursorLanguage")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3071:31
	addExtern< CXTLSKind (*)(CXCursor) , clang_getCursorTLSKind >(*this,lib,"clang_getCursorTLSKind",SideEffects::worstDefault,"clang_getCursorTLSKind")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3076:34
	addExtern< CXTranslationUnitImpl * (*)(CXCursor) , clang_Cursor_getTranslationUnit >(*this,lib,"clang_Cursor_getTranslationUnit",SideEffects::worstDefault,"clang_Cursor_getTranslationUnit")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3086:28
	addExtern< CXCursorSetImpl * (*)() , clang_createCXCursorSet >(*this,lib,"clang_createCXCursorSet",SideEffects::worstDefault,"clang_createCXCursorSet");
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3091:21
	addExtern< void (*)(CXCursorSetImpl *) , clang_disposeCXCursorSet >(*this,lib,"clang_disposeCXCursorSet",SideEffects::worstDefault,"clang_disposeCXCursorSet")
		->args({"cset"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3098:25
	addExtern< unsigned int (*)(CXCursorSetImpl *,CXCursor) , clang_CXCursorSet_contains >(*this,lib,"clang_CXCursorSet_contains",SideEffects::worstDefault,"clang_CXCursorSet_contains")
		->args({"cset","cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3106:25
	addExtern< unsigned int (*)(CXCursorSetImpl *,CXCursor) , clang_CXCursorSet_insert >(*this,lib,"clang_CXCursorSet_insert",SideEffects::worstDefault,"clang_CXCursorSet_insert")
		->args({"cset","cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3142:25
	addExtern< CXCursor (*)(CXCursor) , clang_getCursorSemanticParent ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorSemanticParent",SideEffects::worstDefault,"clang_getCursorSemanticParent")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3178:25
	addExtern< CXCursor (*)(CXCursor) , clang_getCursorLexicalParent ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorLexicalParent",SideEffects::worstDefault,"clang_getCursorLexicalParent")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3223:21
	addExtern< void (*)(CXCursor,CXCursor **,unsigned int *) , clang_getOverriddenCursors >(*this,lib,"clang_getOverriddenCursors",SideEffects::worstDefault,"clang_getOverriddenCursors")
		->args({"cursor","overridden","num_overridden"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3231:21
	addExtern< void (*)(CXCursor *) , clang_disposeOverriddenCursors >(*this,lib,"clang_disposeOverriddenCursors",SideEffects::worstDefault,"clang_disposeOverriddenCursors")
		->args({"overridden"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3237:23
	addExtern< void * (*)(CXCursor) , clang_getIncludedFile >(*this,lib,"clang_getIncludedFile",SideEffects::worstDefault,"clang_getIncludedFile")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3269:25
	addExtern< CXCursor (*)(CXTranslationUnitImpl *,CXSourceLocation) , clang_getCursor ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursor",SideEffects::worstDefault,"clang_getCursor")
		->args({"",""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3281:33
	addExtern< CXSourceLocation (*)(CXCursor) , clang_getCursorLocation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorLocation",SideEffects::worstDefault,"clang_getCursorLocation")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3294:30
	addExtern< CXSourceRange (*)(CXCursor) , clang_getCursorExtent ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorExtent",SideEffects::worstDefault,"clang_getCursorExtent")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3501:23
	addExtern< CXType (*)(CXCursor) , clang_getCursorType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorType",SideEffects::worstDefault,"clang_getCursorType")
		->args({"C"});
}
}

