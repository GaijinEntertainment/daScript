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
void Module_dasClangBind::initFunctions_10() {
// from clang-c/Index.h:3744:26
	makeExtern< long long (*)(CXCursor) , clang_Cursor_getOffsetOfField , SimNode_ExtFuncCall >(lib,"clang_Cursor_getOffsetOfField","clang_Cursor_getOffsetOfField")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3750:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isAnonymous , SimNode_ExtFuncCall >(lib,"clang_Cursor_isAnonymous","clang_Cursor_isAnonymous")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3756:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isAnonymousRecordDecl , SimNode_ExtFuncCall >(lib,"clang_Cursor_isAnonymousRecordDecl","clang_Cursor_isAnonymousRecordDecl")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3762:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isInlineNamespace , SimNode_ExtFuncCall >(lib,"clang_Cursor_isInlineNamespace","clang_Cursor_isInlineNamespace")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3777:20
	makeExtern< int (*)(CXType) , clang_Type_getNumTemplateArguments , SimNode_ExtFuncCall >(lib,"clang_Type_getNumTemplateArguments","clang_Type_getNumTemplateArguments")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3786:23
	makeExtern< CXType (*)(CXType,unsigned int) , clang_Type_getTemplateArgumentAsType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getTemplateArgumentAsType","clang_Type_getTemplateArgumentAsType")
		->args({"T","i"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3795:40
	makeExtern< CXRefQualifierKind (*)(CXType) , clang_Type_getCXXRefQualifier , SimNode_ExtFuncCall >(lib,"clang_Type_getCXXRefQualifier","clang_Type_getCXXRefQualifier")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3801:25
	makeExtern< unsigned int (*)(CXCursor) , clang_isVirtualBase , SimNode_ExtFuncCall >(lib,"clang_isVirtualBase","clang_isVirtualBase")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3810:26
	makeExtern< long long (*)(CXCursor,CXCursor) , clang_getOffsetOfBase , SimNode_ExtFuncCall >(lib,"clang_getOffsetOfBase","clang_getOffsetOfBase")
		->args({"Parent","Base"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3830:43
	makeExtern< CX_CXXAccessSpecifier (*)(CXCursor) , clang_getCXXAccessSpecifier , SimNode_ExtFuncCall >(lib,"clang_getCXXAccessSpecifier","clang_getCXXAccessSpecifier")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3894:1
	makeExtern< CX_BinaryOperatorKind (*)(CXCursor) , clang_Cursor_getBinaryOpcode , SimNode_ExtFuncCall >(lib,"clang_Cursor_getBinaryOpcode","clang_Cursor_getBinaryOpcode")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3902:1
	makeExtern< CXString (*)(CX_BinaryOperatorKind) , clang_Cursor_getBinaryOpcodeStr , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getBinaryOpcodeStr","clang_Cursor_getBinaryOpcodeStr")
		->args({"Op"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3910:37
	makeExtern< CX_StorageClass (*)(CXCursor) , clang_Cursor_getStorageClass , SimNode_ExtFuncCall >(lib,"clang_Cursor_getStorageClass","clang_Cursor_getStorageClass")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3921:25
	makeExtern< unsigned int (*)(CXCursor) , clang_getNumOverloadedDecls , SimNode_ExtFuncCall >(lib,"clang_getNumOverloadedDecls","clang_getNumOverloadedDecls")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3937:25
	makeExtern< CXCursor (*)(CXCursor,unsigned int) , clang_getOverloadedDecl , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getOverloadedDecl","clang_getOverloadedDecl")
		->args({"cursor","index"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3955:23
	makeExtern< CXType (*)(CXCursor) , clang_getIBOutletCollectionType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getIBOutletCollectionType","clang_getIBOutletCollectionType")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4057:1
	makeExtern< unsigned int (*)(CXCursor,_CXChildVisitResult *) , clang_visitChildrenWithBlock , SimNode_ExtFuncCall >(lib,"clang_visitChildrenWithBlock","clang_visitChildrenWithBlock")
		->args({"parent","block"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4083:25
	makeExtern< CXString (*)(CXCursor) , clang_getCursorUSR , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorUSR","clang_getCursorUSR")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4088:25
	makeExtern< CXString (*)(const char *) , clang_constructUSR_ObjCClass , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCClass","clang_constructUSR_ObjCClass")
		->args({"class_name"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4093:25
	makeExtern< CXString (*)(const char *,const char *) , clang_constructUSR_ObjCCategory , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_constructUSR_ObjCCategory","clang_constructUSR_ObjCCategory")
		->args({"class_name","category_name"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

