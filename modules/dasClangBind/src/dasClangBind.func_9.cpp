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
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3877:20
	addExtern< int (*)(CXCursor) , clang_getCursorExceptionSpecificationType >(*this,lib,"clang_getCursorExceptionSpecificationType",SideEffects::worstDefault,"clang_getCursorExceptionSpecificationType")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3883:25
	addExtern< unsigned int (*)(CXType) , clang_isPODType >(*this,lib,"clang_isPODType",SideEffects::worstDefault,"clang_isPODType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3891:23
	addExtern< CXType (*)(CXType) , clang_getElementType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getElementType",SideEffects::worstDefault,"clang_getElementType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3899:26
	addExtern< long long (*)(CXType) , clang_getNumElements >(*this,lib,"clang_getNumElements",SideEffects::worstDefault,"clang_getNumElements")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3906:23
	addExtern< CXType (*)(CXType) , clang_getArrayElementType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getArrayElementType",SideEffects::worstDefault,"clang_getArrayElementType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3913:26
	addExtern< long long (*)(CXType) , clang_getArraySize >(*this,lib,"clang_getArraySize",SideEffects::worstDefault,"clang_getArraySize")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3920:23
	addExtern< CXType (*)(CXType) , clang_Type_getNamedType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getNamedType",SideEffects::worstDefault,"clang_Type_getNamedType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3930:25
	addExtern< unsigned int (*)(CXType) , clang_Type_isTransparentTagTypedef >(*this,lib,"clang_Type_isTransparentTagTypedef",SideEffects::worstDefault,"clang_Type_isTransparentTagTypedef")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3965:43
	addExtern< CXTypeNullabilityKind (*)(CXType) , clang_Type_getNullability >(*this,lib,"clang_Type_getNullability",SideEffects::worstDefault,"clang_Type_getNullability")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4014:26
	addExtern< long long (*)(CXType) , clang_Type_getAlignOf >(*this,lib,"clang_Type_getAlignOf",SideEffects::worstDefault,"clang_Type_getAlignOf")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4021:23
	addExtern< CXType (*)(CXType) , clang_Type_getClassType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getClassType",SideEffects::worstDefault,"clang_Type_getClassType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4032:26
	addExtern< long long (*)(CXType) , clang_Type_getSizeOf >(*this,lib,"clang_Type_getSizeOf",SideEffects::worstDefault,"clang_Type_getSizeOf")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4047:26
	addExtern< long long (*)(CXType,const char *) , clang_Type_getOffsetOf >(*this,lib,"clang_Type_getOffsetOf",SideEffects::worstDefault,"clang_Type_getOffsetOf")
		->args({"T","S"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4054:23
	addExtern< CXType (*)(CXType) , clang_Type_getModifiedType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getModifiedType",SideEffects::worstDefault,"clang_Type_getModifiedType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4061:23
	addExtern< CXType (*)(CXType) , clang_Type_getValueType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getValueType",SideEffects::worstDefault,"clang_Type_getValueType")
		->args({"CT"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4076:26
	addExtern< long long (*)(CXCursor) , clang_Cursor_getOffsetOfField >(*this,lib,"clang_Cursor_getOffsetOfField",SideEffects::worstDefault,"clang_Cursor_getOffsetOfField")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4082:25
	addExtern< unsigned int (*)(CXCursor) , clang_Cursor_isAnonymous >(*this,lib,"clang_Cursor_isAnonymous",SideEffects::worstDefault,"clang_Cursor_isAnonymous")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4088:25
	addExtern< unsigned int (*)(CXCursor) , clang_Cursor_isAnonymousRecordDecl >(*this,lib,"clang_Cursor_isAnonymousRecordDecl",SideEffects::worstDefault,"clang_Cursor_isAnonymousRecordDecl")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4094:25
	addExtern< unsigned int (*)(CXCursor) , clang_Cursor_isInlineNamespace >(*this,lib,"clang_Cursor_isInlineNamespace",SideEffects::worstDefault,"clang_Cursor_isInlineNamespace")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4109:20
	addExtern< int (*)(CXType) , clang_Type_getNumTemplateArguments >(*this,lib,"clang_Type_getNumTemplateArguments",SideEffects::worstDefault,"clang_Type_getNumTemplateArguments")
		->args({"T"});
}
}

