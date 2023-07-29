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
void Module_dasClangBind::initFunctions_11() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4478:33
	addExtern< void * (*)(CXCursor) , clang_getCursorPrintingPolicy >(*this,lib,"clang_getCursorPrintingPolicy",SideEffects::worstDefault,"clang_getCursorPrintingPolicy")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4483:21
	addExtern< void (*)(void *) , clang_PrintingPolicy_dispose >(*this,lib,"clang_PrintingPolicy_dispose",SideEffects::worstDefault,"clang_PrintingPolicy_dispose")
		->args({"Policy"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4496:25
	addExtern< CXString (*)(CXCursor,void *) , clang_getCursorPrettyPrinted ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorPrettyPrinted",SideEffects::worstDefault,"clang_getCursorPrettyPrinted")
		->args({"Cursor","Policy"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4506:25
	addExtern< CXString (*)(CXCursor) , clang_getCursorDisplayName ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorDisplayName",SideEffects::worstDefault,"clang_getCursorDisplayName")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4518:25
	addExtern< CXCursor (*)(CXCursor) , clang_getCursorReferenced ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorReferenced",SideEffects::worstDefault,"clang_getCursorReferenced")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4548:25
	addExtern< CXCursor (*)(CXCursor) , clang_getCursorDefinition ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorDefinition",SideEffects::worstDefault,"clang_getCursorDefinition")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4554:25
	addExtern< unsigned int (*)(CXCursor) , clang_isCursorDefinition >(*this,lib,"clang_isCursorDefinition",SideEffects::worstDefault,"clang_isCursorDefinition")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4580:25
	addExtern< CXCursor (*)(CXCursor) , clang_getCanonicalCursor ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCanonicalCursor",SideEffects::worstDefault,"clang_getCanonicalCursor")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4593:20
	addExtern< int (*)(CXCursor) , clang_Cursor_getObjCSelectorIndex >(*this,lib,"clang_Cursor_getObjCSelectorIndex",SideEffects::worstDefault,"clang_Cursor_getObjCSelectorIndex")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4606:20
	addExtern< int (*)(CXCursor) , clang_Cursor_isDynamicCall >(*this,lib,"clang_Cursor_isDynamicCall",SideEffects::worstDefault,"clang_Cursor_isDynamicCall")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4612:23
	addExtern< CXType (*)(CXCursor) , clang_Cursor_getReceiverType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getReceiverType",SideEffects::worstDefault,"clang_Cursor_getReceiverType")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4642:1
	addExtern< unsigned int (*)(CXCursor,unsigned int) , clang_Cursor_getObjCPropertyAttributes >(*this,lib,"clang_Cursor_getObjCPropertyAttributes",SideEffects::worstDefault,"clang_Cursor_getObjCPropertyAttributes")
		->args({"C","reserved"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4648:25
	addExtern< CXString (*)(CXCursor) , clang_Cursor_getObjCPropertyGetterName ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getObjCPropertyGetterName",SideEffects::worstDefault,"clang_Cursor_getObjCPropertyGetterName")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4654:25
	addExtern< CXString (*)(CXCursor) , clang_Cursor_getObjCPropertySetterName ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getObjCPropertySetterName",SideEffects::worstDefault,"clang_Cursor_getObjCPropertySetterName")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4676:25
	addExtern< unsigned int (*)(CXCursor) , clang_Cursor_getObjCDeclQualifiers >(*this,lib,"clang_Cursor_getObjCDeclQualifiers",SideEffects::worstDefault,"clang_Cursor_getObjCDeclQualifiers")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4683:25
	addExtern< unsigned int (*)(CXCursor) , clang_Cursor_isObjCOptional >(*this,lib,"clang_Cursor_isObjCOptional",SideEffects::worstDefault,"clang_Cursor_isObjCOptional")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4688:25
	addExtern< unsigned int (*)(CXCursor) , clang_Cursor_isVariadic >(*this,lib,"clang_Cursor_isVariadic",SideEffects::worstDefault,"clang_Cursor_isVariadic")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4703:25
	addExtern< unsigned int (*)(CXCursor,CXString *,CXString *,unsigned int *) , clang_Cursor_isExternalSymbol >(*this,lib,"clang_Cursor_isExternalSymbol",SideEffects::worstDefault,"clang_Cursor_isExternalSymbol")
		->args({"C","language","definedIn","isGenerated"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4713:30
	addExtern< CXSourceRange (*)(CXCursor) , clang_Cursor_getCommentRange ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getCommentRange",SideEffects::worstDefault,"clang_Cursor_getCommentRange")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4719:25
	addExtern< CXString (*)(CXCursor) , clang_Cursor_getRawCommentText ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getRawCommentText",SideEffects::worstDefault,"clang_Cursor_getRawCommentText")
		->args({"C"});
}
}

