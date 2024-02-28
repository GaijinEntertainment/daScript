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
void Module_dasClangBind::initFunctions_8() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3487:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isInlineNamespace , SimNode_ExtFuncCall >(lib,"clang_Cursor_isInlineNamespace","clang_Cursor_isInlineNamespace")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3502:20
	makeExtern< int (*)(CXType) , clang_Type_getNumTemplateArguments , SimNode_ExtFuncCall >(lib,"clang_Type_getNumTemplateArguments","clang_Type_getNumTemplateArguments")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3511:23
	makeExtern< CXType (*)(CXType,unsigned int) , clang_Type_getTemplateArgumentAsType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getTemplateArgumentAsType","clang_Type_getTemplateArgumentAsType")
		->args({"T","i"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3520:40
	makeExtern< CXRefQualifierKind (*)(CXType) , clang_Type_getCXXRefQualifier , SimNode_ExtFuncCall >(lib,"clang_Type_getCXXRefQualifier","clang_Type_getCXXRefQualifier")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3526:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isBitField , SimNode_ExtFuncCall >(lib,"clang_Cursor_isBitField","clang_Cursor_isBitField")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3532:25
	makeExtern< unsigned int (*)(CXCursor) , clang_isVirtualBase , SimNode_ExtFuncCall >(lib,"clang_isVirtualBase","clang_isVirtualBase")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3552:43
	makeExtern< CX_CXXAccessSpecifier (*)(CXCursor) , clang_getCXXAccessSpecifier , SimNode_ExtFuncCall >(lib,"clang_getCXXAccessSpecifier","clang_getCXXAccessSpecifier")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3575:37
	makeExtern< CX_StorageClass (*)(CXCursor) , clang_Cursor_getStorageClass , SimNode_ExtFuncCall >(lib,"clang_Cursor_getStorageClass","clang_Cursor_getStorageClass")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3586:25
	makeExtern< unsigned int (*)(CXCursor) , clang_getNumOverloadedDecls , SimNode_ExtFuncCall >(lib,"clang_getNumOverloadedDecls","clang_getNumOverloadedDecls")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3602:25
	makeExtern< CXCursor (*)(CXCursor,unsigned int) , clang_getOverloadedDecl , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getOverloadedDecl","clang_getOverloadedDecl")
		->args({"cursor","index"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3620:23
	makeExtern< CXType (*)(CXCursor) , clang_getIBOutletCollectionType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getIBOutletCollectionType","clang_getIBOutletCollectionType")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3748:25
	makeExtern< CXString (*)(CXCursor) , clang_getCursorUSR , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorUSR","clang_getCursorUSR")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3753:25
	makeExtern< CXString (*)(const char *) , clang_constructUSR_ObjCClass , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCClass","clang_constructUSR_ObjCClass")
		->args({"class_name"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3758:25
	makeExtern< CXString (*)(const char *,const char *) , clang_constructUSR_ObjCCategory , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCCategory","clang_constructUSR_ObjCCategory")
		->args({"class_name","category_name"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3765:1
	makeExtern< CXString (*)(const char *) , clang_constructUSR_ObjCProtocol , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCProtocol","clang_constructUSR_ObjCProtocol")
		->args({"protocol_name"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3771:25
	makeExtern< CXString (*)(const char *,CXString) , clang_constructUSR_ObjCIvar , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCIvar","clang_constructUSR_ObjCIvar")
		->args({"name","classUSR"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3778:25
	makeExtern< CXString (*)(const char *,unsigned int,CXString) , clang_constructUSR_ObjCMethod , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCMethod","clang_constructUSR_ObjCMethod")
		->args({"name","isInstanceMethod","classUSR"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3786:25
	makeExtern< CXString (*)(const char *,CXString) , clang_constructUSR_ObjCProperty , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCProperty","clang_constructUSR_ObjCProperty")
		->args({"property","classUSR"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3792:25
	makeExtern< CXString (*)(CXCursor) , clang_getCursorSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorSpelling","clang_getCursorSpelling")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3805:30
	makeExtern< CXSourceRange (*)(CXCursor,unsigned int,unsigned int) , clang_Cursor_getSpellingNameRange , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getSpellingNameRange","clang_Cursor_getSpellingNameRange")
		->args({"","pieceIndex","options"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

