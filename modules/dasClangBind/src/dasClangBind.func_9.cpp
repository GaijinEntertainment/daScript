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
void Module_dasClangBind::initFunctions_9() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3854:1
	makeExtern< unsigned int (*)(void *,CXPrintingPolicyProperty) , clang_PrintingPolicy_getProperty , SimNode_ExtFuncCall >(lib,"clang_PrintingPolicy_getProperty","clang_PrintingPolicy_getProperty")
		->args({"Policy","Property"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3861:1
	makeExtern< void (*)(void *,CXPrintingPolicyProperty,unsigned int) , clang_PrintingPolicy_setProperty , SimNode_ExtFuncCall >(lib,"clang_PrintingPolicy_setProperty","clang_PrintingPolicy_setProperty")
		->args({"Policy","Property","Value"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3871:33
	makeExtern< void * (*)(CXCursor) , clang_getCursorPrintingPolicy , SimNode_ExtFuncCall >(lib,"clang_getCursorPrintingPolicy","clang_getCursorPrintingPolicy")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3876:21
	makeExtern< void (*)(void *) , clang_PrintingPolicy_dispose , SimNode_ExtFuncCall >(lib,"clang_PrintingPolicy_dispose","clang_PrintingPolicy_dispose")
		->args({"Policy"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3889:25
	makeExtern< CXString (*)(CXCursor,void *) , clang_getCursorPrettyPrinted , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorPrettyPrinted","clang_getCursorPrettyPrinted")
		->args({"Cursor","Policy"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3899:25
	makeExtern< CXString (*)(CXCursor) , clang_getCursorDisplayName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorDisplayName","clang_getCursorDisplayName")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3911:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCursorReferenced , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorReferenced","clang_getCursorReferenced")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3941:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCursorDefinition , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorDefinition","clang_getCursorDefinition")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3947:25
	makeExtern< unsigned int (*)(CXCursor) , clang_isCursorDefinition , SimNode_ExtFuncCall >(lib,"clang_isCursorDefinition","clang_isCursorDefinition")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3973:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCanonicalCursor , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCanonicalCursor","clang_getCanonicalCursor")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3986:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_getObjCSelectorIndex , SimNode_ExtFuncCall >(lib,"clang_Cursor_getObjCSelectorIndex","clang_Cursor_getObjCSelectorIndex")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3999:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_isDynamicCall , SimNode_ExtFuncCall >(lib,"clang_Cursor_isDynamicCall","clang_Cursor_isDynamicCall")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4005:23
	makeExtern< CXType (*)(CXCursor) , clang_Cursor_getReceiverType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getReceiverType","clang_Cursor_getReceiverType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4035:1
	makeExtern< unsigned int (*)(CXCursor,unsigned int) , clang_Cursor_getObjCPropertyAttributes , SimNode_ExtFuncCall >(lib,"clang_Cursor_getObjCPropertyAttributes","clang_Cursor_getObjCPropertyAttributes")
		->args({"C","reserved"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4041:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getObjCPropertyGetterName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getObjCPropertyGetterName","clang_Cursor_getObjCPropertyGetterName")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4047:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getObjCPropertySetterName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getObjCPropertySetterName","clang_Cursor_getObjCPropertySetterName")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4069:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_getObjCDeclQualifiers , SimNode_ExtFuncCall >(lib,"clang_Cursor_getObjCDeclQualifiers","clang_Cursor_getObjCDeclQualifiers")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4076:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isObjCOptional , SimNode_ExtFuncCall >(lib,"clang_Cursor_isObjCOptional","clang_Cursor_isObjCOptional")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4081:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isVariadic , SimNode_ExtFuncCall >(lib,"clang_Cursor_isVariadic","clang_Cursor_isVariadic")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4096:25
	makeExtern< unsigned int (*)(CXCursor,CXString *,CXString *,unsigned int *) , clang_Cursor_isExternalSymbol , SimNode_ExtFuncCall >(lib,"clang_Cursor_isExternalSymbol","clang_Cursor_isExternalSymbol")
		->args({"C","language","definedIn","isGenerated"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

