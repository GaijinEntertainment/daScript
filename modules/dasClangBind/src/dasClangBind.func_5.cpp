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
void Module_dasClangBind::initFunctions_5() {
// from clang-c/Index.h:1177:20
	makeExtern< int (*)(CXTargetInfoImpl *) , clang_TargetInfo_getPointerWidth , SimNode_ExtFuncCall >(lib,"clang_TargetInfo_getPointerWidth","clang_TargetInfo_getPointerWidth")
		->args({"Info"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2356:25
	makeExtern< CXCursor (*)() , clang_getNullCursor , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getNullCursor","clang_getNullCursor")
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2364:25
	makeExtern< CXCursor (*)(CXTranslationUnitImpl *) , clang_getTranslationUnitCursor , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTranslationUnitCursor","clang_getTranslationUnitCursor")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2369:25
	makeExtern< unsigned int (*)(CXCursor,CXCursor) , clang_equalCursors , SimNode_ExtFuncCall >(lib,"clang_equalCursors","clang_equalCursors")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2374:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_isNull , SimNode_ExtFuncCall >(lib,"clang_Cursor_isNull","clang_Cursor_isNull")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2379:25
	makeExtern< unsigned int (*)(CXCursor) , clang_hashCursor , SimNode_ExtFuncCall >(lib,"clang_hashCursor","clang_hashCursor")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2384:34
	makeExtern< CXCursorKind (*)(CXCursor) , clang_getCursorKind , SimNode_ExtFuncCall >(lib,"clang_getCursorKind","clang_getCursorKind")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2389:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isDeclaration , SimNode_ExtFuncCall >(lib,"clang_isDeclaration","clang_isDeclaration")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2399:25
	makeExtern< unsigned int (*)(CXCursor) , clang_isInvalidDeclaration , SimNode_ExtFuncCall >(lib,"clang_isInvalidDeclaration","clang_isInvalidDeclaration")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2409:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isReference , SimNode_ExtFuncCall >(lib,"clang_isReference","clang_isReference")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2414:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isExpression , SimNode_ExtFuncCall >(lib,"clang_isExpression","clang_isExpression")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2419:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isStatement , SimNode_ExtFuncCall >(lib,"clang_isStatement","clang_isStatement")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2424:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isAttribute , SimNode_ExtFuncCall >(lib,"clang_isAttribute","clang_isAttribute")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2429:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_hasAttrs , SimNode_ExtFuncCall >(lib,"clang_Cursor_hasAttrs","clang_Cursor_hasAttrs")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2435:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isInvalid , SimNode_ExtFuncCall >(lib,"clang_isInvalid","clang_isInvalid")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2441:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_isTranslationUnit","clang_isTranslationUnit")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2447:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isPreprocessing , SimNode_ExtFuncCall >(lib,"clang_isPreprocessing","clang_isPreprocessing")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2453:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isUnexposed , SimNode_ExtFuncCall >(lib,"clang_isUnexposed","clang_isUnexposed")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2479:35
	makeExtern< CXLinkageKind (*)(CXCursor) , clang_getCursorLinkage , SimNode_ExtFuncCall >(lib,"clang_getCursorLinkage","clang_getCursorLinkage")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:2505:38
	makeExtern< CXVisibilityKind (*)(CXCursor) , clang_getCursorVisibility , SimNode_ExtFuncCall >(lib,"clang_getCursorVisibility","clang_getCursorVisibility")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

