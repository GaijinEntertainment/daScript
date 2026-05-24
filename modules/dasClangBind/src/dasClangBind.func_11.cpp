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
void Module_dasClangBind::initFunctions_11() {
// from clang-c/Index.h:4100:1
	makeExtern< CXString (*)(const char *) , clang_constructUSR_ObjCProtocol , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCProtocol","clang_constructUSR_ObjCProtocol")
		->args({"protocol_name"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4106:25
	makeExtern< CXString (*)(const char *,CXString) , clang_constructUSR_ObjCIvar , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCIvar","clang_constructUSR_ObjCIvar")
		->args({"name","classUSR"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4113:25
	makeExtern< CXString (*)(const char *,unsigned int,CXString) , clang_constructUSR_ObjCMethod , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCMethod","clang_constructUSR_ObjCMethod")
		->args({"name","isInstanceMethod","classUSR"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4121:25
	makeExtern< CXString (*)(const char *,CXString) , clang_constructUSR_ObjCProperty , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCProperty","clang_constructUSR_ObjCProperty")
		->args({"property","classUSR"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4127:25
	makeExtern< CXString (*)(CXCursor) , clang_getCursorSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorSpelling","clang_getCursorSpelling")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4140:30
	makeExtern< CXSourceRange (*)(CXCursor,unsigned int,unsigned int) , clang_Cursor_getSpellingNameRange , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getSpellingNameRange","clang_Cursor_getSpellingNameRange")
		->args({"","pieceIndex","options"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4189:1
	makeExtern< unsigned int (*)(void *,CXPrintingPolicyProperty) , clang_PrintingPolicy_getProperty , SimNode_ExtFuncCall >(lib,"clang_PrintingPolicy_getProperty","clang_PrintingPolicy_getProperty")
		->args({"Policy","Property"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4196:1
	makeExtern< void (*)(void *,CXPrintingPolicyProperty,unsigned int) , clang_PrintingPolicy_setProperty , SimNode_ExtFuncCall >(lib,"clang_PrintingPolicy_setProperty","clang_PrintingPolicy_setProperty")
		->args({"Policy","Property","Value"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4206:33
	makeExtern< void * (*)(CXCursor) , clang_getCursorPrintingPolicy , SimNode_ExtFuncCall >(lib,"clang_getCursorPrintingPolicy","clang_getCursorPrintingPolicy")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4211:21
	makeExtern< void (*)(void *) , clang_PrintingPolicy_dispose , SimNode_ExtFuncCall >(lib,"clang_PrintingPolicy_dispose","clang_PrintingPolicy_dispose")
		->args({"Policy"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4224:25
	makeExtern< CXString (*)(CXCursor,void *) , clang_getCursorPrettyPrinted , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorPrettyPrinted","clang_getCursorPrettyPrinted")
		->args({"Cursor","Policy"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4232:25
	makeExtern< CXString (*)(CXType,void *) , clang_getTypePrettyPrinted , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypePrettyPrinted","clang_getTypePrettyPrinted")
		->args({"CT","cxPolicy"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4244:25
	makeExtern< CXString (*)(CXType,void *,unsigned int) , clang_getFullyQualifiedName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getFullyQualifiedName","clang_getFullyQualifiedName")
		->args({"CT","Policy","WithGlobalNsPrefix"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4254:25
	makeExtern< CXString (*)(CXCursor) , clang_getCursorDisplayName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorDisplayName","clang_getCursorDisplayName")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4266:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCursorReferenced , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorReferenced","clang_getCursorReferenced")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4296:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCursorDefinition , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorDefinition","clang_getCursorDefinition")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4302:25
	makeExtern< unsigned int (*)(CXCursor) , clang_isCursorDefinition , SimNode_ExtFuncCall >(lib,"clang_isCursorDefinition","clang_isCursorDefinition")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4328:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getCanonicalCursor , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCanonicalCursor","clang_getCanonicalCursor")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4341:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_getObjCSelectorIndex , SimNode_ExtFuncCall >(lib,"clang_Cursor_getObjCSelectorIndex","clang_Cursor_getObjCSelectorIndex")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4354:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_isDynamicCall , SimNode_ExtFuncCall >(lib,"clang_Cursor_isDynamicCall","clang_Cursor_isDynamicCall")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

