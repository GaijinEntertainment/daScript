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
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2366:1
	makeExtern< void (*)(CXPlatformAvailability *) , clang_disposeCXPlatformAvailability , SimNode_ExtFuncCall >(lib,"clang_disposeCXPlatformAvailability","clang_disposeCXPlatformAvailability")
		->args({"availability"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2372:25
	makeExtern< CXCursor (*)(CXCursor) , clang_Cursor_getVarDeclInitializer , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getVarDeclInitializer","clang_Cursor_getVarDeclInitializer")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2379:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_hasVarDeclGlobalStorage , SimNode_ExtFuncCall >(lib,"clang_Cursor_hasVarDeclGlobalStorage","clang_Cursor_hasVarDeclGlobalStorage")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2386:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_hasVarDeclExternalStorage , SimNode_ExtFuncCall >(lib,"clang_Cursor_hasVarDeclExternalStorage","clang_Cursor_hasVarDeclExternalStorage")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2401:36
	makeExtern< CXLanguageKind (*)(CXCursor) , clang_getCursorLanguage , SimNode_ExtFuncCall >(lib,"clang_getCursorLanguage","clang_getCursorLanguage")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2413:31
	makeExtern< CXTLSKind (*)(CXCursor) , clang_getCursorTLSKind , SimNode_ExtFuncCall >(lib,"clang_getCursorTLSKind","clang_getCursorTLSKind")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2418:34
	makeExtern< CXTranslationUnitImpl * (*)(CXCursor) , clang_Cursor_getTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_Cursor_getTranslationUnit","clang_Cursor_getTranslationUnit")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2428:28
	makeExtern< CXCursorSetImpl * (*)() , clang_createCXCursorSet , SimNode_ExtFuncCall >(lib,"clang_createCXCursorSet","clang_createCXCursorSet")
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2433:21
	makeExtern< void (*)(CXCursorSetImpl *) , clang_disposeCXCursorSet , SimNode_ExtFuncCall >(lib,"clang_disposeCXCursorSet","clang_disposeCXCursorSet")
		->args({"cset"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2440:25
	makeExtern< unsigned int (*)(CXCursorSetImpl *,CXCursor) , clang_CXCursorSet_contains , SimNode_ExtFuncCall >(lib,"clang_CXCursorSet_contains","clang_CXCursorSet_contains")
		->args({"cset","cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2448:25
	makeExtern< unsigned int (*)(CXCursorSetImpl *,CXCursor) , clang_CXCursorSet_insert , SimNode_ExtFuncCall >(lib,"clang_CXCursorSet_insert","clang_CXCursorSet_insert")
		->args({"cset","cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2484:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCursorSemanticParent , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorSemanticParent","clang_getCursorSemanticParent")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2520:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCursorLexicalParent , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorLexicalParent","clang_getCursorLexicalParent")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2565:21
	makeExtern< void (*)(CXCursor,CXCursor **,unsigned int *) , clang_getOverriddenCursors , SimNode_ExtFuncCall >(lib,"clang_getOverriddenCursors","clang_getOverriddenCursors")
		->args({"cursor","overridden","num_overridden"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2573:21
	makeExtern< void (*)(CXCursor *) , clang_disposeOverriddenCursors , SimNode_ExtFuncCall >(lib,"clang_disposeOverriddenCursors","clang_disposeOverriddenCursors")
		->args({"overridden"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2579:23
	makeExtern< void * (*)(CXCursor) , clang_getIncludedFile , SimNode_ExtFuncCall >(lib,"clang_getIncludedFile","clang_getIncludedFile")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2611:25
	makeExtern< CXCursor (*)(CXTranslationUnitImpl *,CXSourceLocation) , clang_getCursor , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursor","clang_getCursor")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2623:33
	makeExtern< CXSourceLocation (*)(CXCursor) , clang_getCursorLocation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorLocation","clang_getCursorLocation")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2636:30
	makeExtern< CXSourceRange (*)(CXCursor) , clang_getCursorExtent , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorExtent","clang_getCursorExtent")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2843:23
	makeExtern< CXType (*)(CXCursor) , clang_getCursorType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorType","clang_getCursorType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

