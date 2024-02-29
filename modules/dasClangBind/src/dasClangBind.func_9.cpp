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
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3254:25
	makeExtern< unsigned int (*)(CXType) , clang_isFunctionTypeVariadic , SimNode_ExtFuncCall >(lib,"clang_isFunctionTypeVariadic","clang_isFunctionTypeVariadic")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3261:23
	makeExtern< CXType (*)(CXCursor) , clang_getCursorResultType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorResultType","clang_getCursorResultType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3270:20
	makeExtern< int (*)(CXCursor) , clang_getCursorExceptionSpecificationType , SimNode_ExtFuncCall >(lib,"clang_getCursorExceptionSpecificationType","clang_getCursorExceptionSpecificationType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3276:25
	makeExtern< unsigned int (*)(CXType) , clang_isPODType , SimNode_ExtFuncCall >(lib,"clang_isPODType","clang_isPODType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3284:23
	makeExtern< CXType (*)(CXType) , clang_getElementType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getElementType","clang_getElementType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3292:26
	makeExtern< long long (*)(CXType) , clang_getNumElements , SimNode_ExtFuncCall >(lib,"clang_getNumElements","clang_getNumElements")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3299:23
	makeExtern< CXType (*)(CXType) , clang_getArrayElementType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getArrayElementType","clang_getArrayElementType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3306:26
	makeExtern< long long (*)(CXType) , clang_getArraySize , SimNode_ExtFuncCall >(lib,"clang_getArraySize","clang_getArraySize")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3313:23
	makeExtern< CXType (*)(CXType) , clang_Type_getNamedType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getNamedType","clang_Type_getNamedType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3323:25
	makeExtern< unsigned int (*)(CXType) , clang_Type_isTransparentTagTypedef , SimNode_ExtFuncCall >(lib,"clang_Type_isTransparentTagTypedef","clang_Type_isTransparentTagTypedef")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3358:43
	makeExtern< CXTypeNullabilityKind (*)(CXType) , clang_Type_getNullability , SimNode_ExtFuncCall >(lib,"clang_Type_getNullability","clang_Type_getNullability")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3407:26
	makeExtern< long long (*)(CXType) , clang_Type_getAlignOf , SimNode_ExtFuncCall >(lib,"clang_Type_getAlignOf","clang_Type_getAlignOf")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3414:23
	makeExtern< CXType (*)(CXType) , clang_Type_getClassType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getClassType","clang_Type_getClassType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3425:26
	makeExtern< long long (*)(CXType) , clang_Type_getSizeOf , SimNode_ExtFuncCall >(lib,"clang_Type_getSizeOf","clang_Type_getSizeOf")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3440:26
	makeExtern< long long (*)(CXType,const char *) , clang_Type_getOffsetOf , SimNode_ExtFuncCall >(lib,"clang_Type_getOffsetOf","clang_Type_getOffsetOf")
		->args({"T","S"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3447:23
	makeExtern< CXType (*)(CXType) , clang_Type_getModifiedType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getModifiedType","clang_Type_getModifiedType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3454:23
	makeExtern< CXType (*)(CXType) , clang_Type_getValueType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getValueType","clang_Type_getValueType")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3469:26
	makeExtern< long long (*)(CXCursor) , clang_Cursor_getOffsetOfField , SimNode_ExtFuncCall >(lib,"clang_Cursor_getOffsetOfField","clang_Cursor_getOffsetOfField")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3475:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isAnonymous , SimNode_ExtFuncCall >(lib,"clang_Cursor_isAnonymous","clang_Cursor_isAnonymous")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3481:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isAnonymousRecordDecl , SimNode_ExtFuncCall >(lib,"clang_Cursor_isAnonymousRecordDecl","clang_Cursor_isAnonymousRecordDecl")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

