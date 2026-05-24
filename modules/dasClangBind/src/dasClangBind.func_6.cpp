// this file is generated via Daslang automatic binder
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
// from clang-c/Index.h:2516:1
	makeExtern< CXAvailabilityKind (*)(CXCursor) , clang_getCursorAvailability , SimNode_ExtFuncCall >(lib,"clang_getCursorAvailability","clang_getCursorAvailability")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2591:20
	makeExtern< int (*)(CXCursor,int *,CXString *,int *,CXString *,CXPlatformAvailability *,int) , clang_getCursorPlatformAvailability , SimNode_ExtFuncCall >(lib,"clang_getCursorPlatformAvailability","clang_getCursorPlatformAvailability")
		->args({"cursor","always_deprecated","deprecated_message","always_unavailable","unavailable_message","availability","availability_size"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2600:1
	makeExtern< void (*)(CXPlatformAvailability *) , clang_disposeCXPlatformAvailability , SimNode_ExtFuncCall >(lib,"clang_disposeCXPlatformAvailability","clang_disposeCXPlatformAvailability")
		->args({"availability"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2606:25
	makeExtern< CXCursor (*)(CXCursor) , clang_Cursor_getVarDeclInitializer , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getVarDeclInitializer","clang_Cursor_getVarDeclInitializer")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2613:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_hasVarDeclGlobalStorage , SimNode_ExtFuncCall >(lib,"clang_Cursor_hasVarDeclGlobalStorage","clang_Cursor_hasVarDeclGlobalStorage")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2620:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_hasVarDeclExternalStorage , SimNode_ExtFuncCall >(lib,"clang_Cursor_hasVarDeclExternalStorage","clang_Cursor_hasVarDeclExternalStorage")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2635:36
	makeExtern< CXLanguageKind (*)(CXCursor) , clang_getCursorLanguage , SimNode_ExtFuncCall >(lib,"clang_getCursorLanguage","clang_getCursorLanguage")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2647:31
	makeExtern< CXTLSKind (*)(CXCursor) , clang_getCursorTLSKind , SimNode_ExtFuncCall >(lib,"clang_getCursorTLSKind","clang_getCursorTLSKind")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2652:34
	makeExtern< CXTranslationUnitImpl * (*)(CXCursor) , clang_Cursor_getTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_Cursor_getTranslationUnit","clang_Cursor_getTranslationUnit")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2662:28
	makeExtern< CXCursorSetImpl * (*)() , clang_createCXCursorSet , SimNode_ExtFuncCall >(lib,"clang_createCXCursorSet","clang_createCXCursorSet")
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2667:21
	makeExtern< void (*)(CXCursorSetImpl *) , clang_disposeCXCursorSet , SimNode_ExtFuncCall >(lib,"clang_disposeCXCursorSet","clang_disposeCXCursorSet")
		->args({"cset"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2674:25
	makeExtern< unsigned int (*)(CXCursorSetImpl *,CXCursor) , clang_CXCursorSet_contains , SimNode_ExtFuncCall >(lib,"clang_CXCursorSet_contains","clang_CXCursorSet_contains")
		->args({"cset","cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2682:25
	makeExtern< unsigned int (*)(CXCursorSetImpl *,CXCursor) , clang_CXCursorSet_insert , SimNode_ExtFuncCall >(lib,"clang_CXCursorSet_insert","clang_CXCursorSet_insert")
		->args({"cset","cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2718:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCursorSemanticParent , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorSemanticParent","clang_getCursorSemanticParent")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2754:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCursorLexicalParent , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorLexicalParent","clang_getCursorLexicalParent")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2799:21
	makeExtern< void (*)(CXCursor,CXCursor **,unsigned int *) , clang_getOverriddenCursors , SimNode_ExtFuncCall >(lib,"clang_getOverriddenCursors","clang_getOverriddenCursors")
		->args({"cursor","overridden","num_overridden"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2807:21
	makeExtern< void (*)(CXCursor *) , clang_disposeOverriddenCursors , SimNode_ExtFuncCall >(lib,"clang_disposeOverriddenCursors","clang_disposeOverriddenCursors")
		->args({"overridden"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2813:23
	makeExtern< void * (*)(CXCursor) , clang_getIncludedFile , SimNode_ExtFuncCall >(lib,"clang_getIncludedFile","clang_getIncludedFile")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2845:25
	makeExtern< CXCursor (*)(CXTranslationUnitImpl *,CXSourceLocation) , clang_getCursor , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursor","clang_getCursor")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2857:33
	makeExtern< CXSourceLocation (*)(CXCursor) , clang_getCursorLocation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorLocation","clang_getCursorLocation")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
}
}

