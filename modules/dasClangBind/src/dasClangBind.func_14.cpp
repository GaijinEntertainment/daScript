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
void Module_dasClangBind::initFunctions_14() {
// from clang-c/Index.h:4765:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isStatic , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isStatic","clang_CXXMethod_isStatic")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4772:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isVirtual , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isVirtual","clang_CXXMethod_isVirtual")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4797:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isCopyAssignmentOperator , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isCopyAssignmentOperator","clang_CXXMethod_isCopyAssignmentOperator")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4822:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isMoveAssignmentOperator , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isMoveAssignmentOperator","clang_CXXMethod_isMoveAssignmentOperator")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4867:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isExplicit , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isExplicit","clang_CXXMethod_isExplicit")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4873:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXRecord_isAbstract , SimNode_ExtFuncCall >(lib,"clang_CXXRecord_isAbstract","clang_CXXRecord_isAbstract")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4878:25
	makeExtern< unsigned int (*)(CXCursor) , clang_EnumDecl_isScoped , SimNode_ExtFuncCall >(lib,"clang_EnumDecl_isScoped","clang_EnumDecl_isScoped")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4884:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isConst , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isConst","clang_CXXMethod_isConst")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4903:34
	makeExtern< CXCursorKind (*)(CXCursor) , clang_getTemplateCursorKind , SimNode_ExtFuncCall >(lib,"clang_getTemplateCursorKind","clang_getTemplateCursorKind")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4933:25
	makeExtern< CXCursor (*)(CXCursor) , clang_getSpecializedCursorTemplate , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getSpecializedCursorTemplate","clang_getSpecializedCursorTemplate")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4953:30
	makeExtern< CXSourceRange (*)(CXCursor,unsigned int,unsigned int) , clang_getCursorReferenceNameRange , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorReferenceNameRange","clang_getCursorReferenceNameRange")
		->args({"C","NameFlags","PieceIndex"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5045:25
	makeExtern< CXToken * (*)(CXTranslationUnitImpl *,CXSourceLocation) , clang_getToken , SimNode_ExtFuncCall >(lib,"clang_getToken","clang_getToken")
		->args({"TU","Location"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5051:28
	makeExtern< CXTokenKind (*)(CXToken) , clang_getTokenKind , SimNode_ExtFuncCall >(lib,"clang_getTokenKind","clang_getTokenKind")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5059:25
	makeExtern< CXString (*)(CXTranslationUnitImpl *,CXToken) , clang_getTokenSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTokenSpelling","clang_getTokenSpelling")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5064:33
	makeExtern< CXSourceLocation (*)(CXTranslationUnitImpl *,CXToken) , clang_getTokenLocation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTokenLocation","clang_getTokenLocation")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5070:30
	makeExtern< CXSourceRange (*)(CXTranslationUnitImpl *,CXToken) , clang_getTokenExtent , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTokenExtent","clang_getTokenExtent")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5089:21
	makeExtern< void (*)(CXTranslationUnitImpl *,CXSourceRange,CXToken **,unsigned int *) , clang_tokenize , SimNode_ExtFuncCall >(lib,"clang_tokenize","clang_tokenize")
		->args({"TU","Range","Tokens","NumTokens"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5122:21
	makeExtern< void (*)(CXTranslationUnitImpl *,CXToken *,unsigned int,CXCursor *) , clang_annotateTokens , SimNode_ExtFuncCall >(lib,"clang_annotateTokens","clang_annotateTokens")
		->args({"TU","Tokens","NumTokens","Cursors"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5128:21
	makeExtern< void (*)(CXTranslationUnitImpl *,CXToken *,unsigned int) , clang_disposeTokens , SimNode_ExtFuncCall >(lib,"clang_disposeTokens","clang_disposeTokens")
		->args({"TU","Tokens","NumTokens"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5145:25
	makeExtern< CXString (*)(CXCursorKind) , clang_getCursorKindSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorKindSpelling","clang_getCursorKindSpelling")
		->args({"Kind"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

