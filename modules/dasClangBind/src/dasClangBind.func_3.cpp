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
void Module_dasClangBind::initFunctions_3() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2130:25
	makeExtern< CXCursor (*)(CXTranslationUnitImpl *) , clang_getTranslationUnitCursor , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTranslationUnitCursor","clang_getTranslationUnitCursor")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2135:25
	makeExtern< unsigned int (*)(CXCursor,CXCursor) , clang_equalCursors , SimNode_ExtFuncCall >(lib,"clang_equalCursors","clang_equalCursors")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2140:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_isNull , SimNode_ExtFuncCall >(lib,"clang_Cursor_isNull","clang_Cursor_isNull")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2145:25
	makeExtern< unsigned int (*)(CXCursor) , clang_hashCursor , SimNode_ExtFuncCall >(lib,"clang_hashCursor","clang_hashCursor")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2150:34
	makeExtern< CXCursorKind (*)(CXCursor) , clang_getCursorKind , SimNode_ExtFuncCall >(lib,"clang_getCursorKind","clang_getCursorKind")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2155:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isDeclaration , SimNode_ExtFuncCall >(lib,"clang_isDeclaration","clang_isDeclaration")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2165:25
	makeExtern< unsigned int (*)(CXCursor) , clang_isInvalidDeclaration , SimNode_ExtFuncCall >(lib,"clang_isInvalidDeclaration","clang_isInvalidDeclaration")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2175:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isReference , SimNode_ExtFuncCall >(lib,"clang_isReference","clang_isReference")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2180:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isExpression , SimNode_ExtFuncCall >(lib,"clang_isExpression","clang_isExpression")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2185:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isStatement , SimNode_ExtFuncCall >(lib,"clang_isStatement","clang_isStatement")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2190:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isAttribute , SimNode_ExtFuncCall >(lib,"clang_isAttribute","clang_isAttribute")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2195:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_hasAttrs , SimNode_ExtFuncCall >(lib,"clang_Cursor_hasAttrs","clang_Cursor_hasAttrs")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2201:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isInvalid , SimNode_ExtFuncCall >(lib,"clang_isInvalid","clang_isInvalid")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2207:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_isTranslationUnit","clang_isTranslationUnit")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2213:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isPreprocessing , SimNode_ExtFuncCall >(lib,"clang_isPreprocessing","clang_isPreprocessing")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2219:25
	makeExtern< unsigned int (*)(CXCursorKind) , clang_isUnexposed , SimNode_ExtFuncCall >(lib,"clang_isUnexposed","clang_isUnexposed")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2245:35
	makeExtern< CXLinkageKind (*)(CXCursor) , clang_getCursorLinkage , SimNode_ExtFuncCall >(lib,"clang_getCursorLinkage","clang_getCursorLinkage")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2271:38
	makeExtern< CXVisibilityKind (*)(CXCursor) , clang_getCursorVisibility , SimNode_ExtFuncCall >(lib,"clang_getCursorVisibility","clang_getCursorVisibility")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2282:1
	makeExtern< CXAvailabilityKind (*)(CXCursor) , clang_getCursorAvailability , SimNode_ExtFuncCall >(lib,"clang_getCursorAvailability","clang_getCursorAvailability")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2357:20
	makeExtern< int (*)(CXCursor,int *,CXString *,int *,CXString *,CXPlatformAvailability *,int) , clang_getCursorPlatformAvailability , SimNode_ExtFuncCall >(lib,"clang_getCursorPlatformAvailability","clang_getCursorPlatformAvailability")
		->args({"cursor","always_deprecated","deprecated_message","always_unavailable","unavailable_message","availability","availability_size"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

