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
void Module_dasClangBind::initFunctions_10() {
	addExtern<CXType (*)(CXType,unsigned int),clang_Type_getTemplateArgumentAsType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getTemplateArgumentAsType",SideEffects::worstDefault,"clang_Type_getTemplateArgumentAsType")
		->args({"T","i"});
	addExtern<CXRefQualifierKind (*)(CXType),clang_Type_getCXXRefQualifier>(*this,lib,"clang_Type_getCXXRefQualifier",SideEffects::worstDefault,"clang_Type_getCXXRefQualifier")
		->args({"T"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isBitField>(*this,lib,"clang_Cursor_isBitField",SideEffects::worstDefault,"clang_Cursor_isBitField")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_isVirtualBase>(*this,lib,"clang_isVirtualBase",SideEffects::worstDefault,"clang_isVirtualBase")
		->args({""});
	addExtern<CX_CXXAccessSpecifier (*)(CXCursor),clang_getCXXAccessSpecifier>(*this,lib,"clang_getCXXAccessSpecifier",SideEffects::worstDefault,"clang_getCXXAccessSpecifier")
		->args({""});
	addExtern<CX_StorageClass (*)(CXCursor),clang_Cursor_getStorageClass>(*this,lib,"clang_Cursor_getStorageClass",SideEffects::worstDefault,"clang_Cursor_getStorageClass")
		->args({""});
	addExtern<unsigned int (*)(CXCursor),clang_getNumOverloadedDecls>(*this,lib,"clang_getNumOverloadedDecls",SideEffects::worstDefault,"clang_getNumOverloadedDecls")
		->args({"cursor"});
	addExtern<CXCursor (*)(CXCursor,unsigned int),clang_getOverloadedDecl,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getOverloadedDecl",SideEffects::worstDefault,"clang_getOverloadedDecl")
		->args({"cursor","index"});
	addExtern<CXType (*)(CXCursor),clang_getIBOutletCollectionType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getIBOutletCollectionType",SideEffects::worstDefault,"clang_getIBOutletCollectionType")
		->args({""});
	addExtern<CXString (*)(CXCursor),clang_getCursorUSR,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorUSR",SideEffects::worstDefault,"clang_getCursorUSR")
		->args({""});
	addExtern<CXString (*)(const char *),clang_constructUSR_ObjCClass,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCClass",SideEffects::worstDefault,"clang_constructUSR_ObjCClass")
		->args({"class_name"});
	addExtern<CXString (*)(const char *,const char *),clang_constructUSR_ObjCCategory,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCCategory",SideEffects::worstDefault,"clang_constructUSR_ObjCCategory")
		->args({"class_name","category_name"});
	addExtern<CXString (*)(const char *),clang_constructUSR_ObjCProtocol,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCProtocol",SideEffects::worstDefault,"clang_constructUSR_ObjCProtocol")
		->args({"protocol_name"});
	addExtern<CXString (*)(const char *,CXString),clang_constructUSR_ObjCIvar,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCIvar",SideEffects::worstDefault,"clang_constructUSR_ObjCIvar")
		->args({"name","classUSR"});
	addExtern<CXString (*)(const char *,unsigned int,CXString),clang_constructUSR_ObjCMethod,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCMethod",SideEffects::worstDefault,"clang_constructUSR_ObjCMethod")
		->args({"name","isInstanceMethod","classUSR"});
	addExtern<CXString (*)(const char *,CXString),clang_constructUSR_ObjCProperty,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCProperty",SideEffects::worstDefault,"clang_constructUSR_ObjCProperty")
		->args({"property","classUSR"});
	addExtern<CXString (*)(CXCursor),clang_getCursorSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorSpelling",SideEffects::worstDefault,"clang_getCursorSpelling")
		->args({""});
	addExtern<CXSourceRange (*)(CXCursor,unsigned int,unsigned int),clang_Cursor_getSpellingNameRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getSpellingNameRange",SideEffects::worstDefault,"clang_Cursor_getSpellingNameRange")
		->args({"","pieceIndex","options"});
	addExtern<unsigned int (*)(void *,CXPrintingPolicyProperty),clang_PrintingPolicy_getProperty>(*this,lib,"clang_PrintingPolicy_getProperty",SideEffects::worstDefault,"clang_PrintingPolicy_getProperty")
		->args({"Policy","Property"});
	addExtern<void (*)(void *,CXPrintingPolicyProperty,unsigned int),clang_PrintingPolicy_setProperty>(*this,lib,"clang_PrintingPolicy_setProperty",SideEffects::worstDefault,"clang_PrintingPolicy_setProperty")
		->args({"Policy","Property","Value"});
}
}

