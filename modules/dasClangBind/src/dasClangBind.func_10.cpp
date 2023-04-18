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
void Module_dasClangBind::initFunctions_10() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4118:23
	addExtern< CXType (*)(CXType,unsigned int) , clang_Type_getTemplateArgumentAsType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getTemplateArgumentAsType",SideEffects::worstDefault,"clang_Type_getTemplateArgumentAsType")
		->args({"T","i"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4127:40
	addExtern< CXRefQualifierKind (*)(CXType) , clang_Type_getCXXRefQualifier >(*this,lib,"clang_Type_getCXXRefQualifier",SideEffects::worstDefault,"clang_Type_getCXXRefQualifier")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4133:25
	addExtern< unsigned int (*)(CXCursor) , clang_Cursor_isBitField >(*this,lib,"clang_Cursor_isBitField",SideEffects::worstDefault,"clang_Cursor_isBitField")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4139:25
	addExtern< unsigned int (*)(CXCursor) , clang_isVirtualBase >(*this,lib,"clang_isVirtualBase",SideEffects::worstDefault,"clang_isVirtualBase")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4159:43
	addExtern< CX_CXXAccessSpecifier (*)(CXCursor) , clang_getCXXAccessSpecifier >(*this,lib,"clang_getCXXAccessSpecifier",SideEffects::worstDefault,"clang_getCXXAccessSpecifier")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4182:37
	addExtern< CX_StorageClass (*)(CXCursor) , clang_Cursor_getStorageClass >(*this,lib,"clang_Cursor_getStorageClass",SideEffects::worstDefault,"clang_Cursor_getStorageClass")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4193:25
	addExtern< unsigned int (*)(CXCursor) , clang_getNumOverloadedDecls >(*this,lib,"clang_getNumOverloadedDecls",SideEffects::worstDefault,"clang_getNumOverloadedDecls")
		->args({"cursor"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4209:25
	addExtern< CXCursor (*)(CXCursor,unsigned int) , clang_getOverloadedDecl ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getOverloadedDecl",SideEffects::worstDefault,"clang_getOverloadedDecl")
		->args({"cursor","index"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4227:23
	addExtern< CXType (*)(CXCursor) , clang_getIBOutletCollectionType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getIBOutletCollectionType",SideEffects::worstDefault,"clang_getIBOutletCollectionType")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4355:25
	addExtern< CXString (*)(CXCursor) , clang_getCursorUSR ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorUSR",SideEffects::worstDefault,"clang_getCursorUSR")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4360:25
	addExtern< CXString (*)(const char *) , clang_constructUSR_ObjCClass ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCClass",SideEffects::worstDefault,"clang_constructUSR_ObjCClass")
		->args({"class_name"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4365:25
	addExtern< CXString (*)(const char *,const char *) , clang_constructUSR_ObjCCategory ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCCategory",SideEffects::worstDefault,"clang_constructUSR_ObjCCategory")
		->args({"class_name","category_name"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4372:1
	addExtern< CXString (*)(const char *) , clang_constructUSR_ObjCProtocol ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCProtocol",SideEffects::worstDefault,"clang_constructUSR_ObjCProtocol")
		->args({"protocol_name"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4378:25
	addExtern< CXString (*)(const char *,CXString) , clang_constructUSR_ObjCIvar ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCIvar",SideEffects::worstDefault,"clang_constructUSR_ObjCIvar")
		->args({"name","classUSR"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4385:25
	addExtern< CXString (*)(const char *,unsigned int,CXString) , clang_constructUSR_ObjCMethod ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCMethod",SideEffects::worstDefault,"clang_constructUSR_ObjCMethod")
		->args({"name","isInstanceMethod","classUSR"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4393:25
	addExtern< CXString (*)(const char *,CXString) , clang_constructUSR_ObjCProperty ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCProperty",SideEffects::worstDefault,"clang_constructUSR_ObjCProperty")
		->args({"property","classUSR"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4399:25
	addExtern< CXString (*)(CXCursor) , clang_getCursorSpelling ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorSpelling",SideEffects::worstDefault,"clang_getCursorSpelling")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4412:30
	addExtern< CXSourceRange (*)(CXCursor,unsigned int,unsigned int) , clang_Cursor_getSpellingNameRange ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getSpellingNameRange",SideEffects::worstDefault,"clang_Cursor_getSpellingNameRange")
		->args({"","pieceIndex","options"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4461:1
	addExtern< unsigned int (*)(void *,CXPrintingPolicyProperty) , clang_PrintingPolicy_getProperty >(*this,lib,"clang_PrintingPolicy_getProperty",SideEffects::worstDefault,"clang_PrintingPolicy_getProperty")
		->args({"Policy","Property"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4468:1
	addExtern< void (*)(void *,CXPrintingPolicyProperty,unsigned int) , clang_PrintingPolicy_setProperty >(*this,lib,"clang_PrintingPolicy_setProperty",SideEffects::worstDefault,"clang_PrintingPolicy_setProperty")
		->args({"Policy","Property","Value"});
}
}

