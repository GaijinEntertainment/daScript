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
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3746:25
	addExtern< unsigned int (*)(CXType) , clang_isRestrictQualifiedType >(*this,lib,"clang_isRestrictQualifiedType",SideEffects::worstDefault,"clang_isRestrictQualifiedType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3751:25
	addExtern< unsigned int (*)(CXType) , clang_getAddressSpace >(*this,lib,"clang_getAddressSpace",SideEffects::worstDefault,"clang_getAddressSpace")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3756:25
	addExtern< CXString (*)(CXType) , clang_getTypedefName ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypedefName",SideEffects::worstDefault,"clang_getTypedefName")
		->args({"CT"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3761:23
	addExtern< CXType (*)(CXType) , clang_getPointeeType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getPointeeType",SideEffects::worstDefault,"clang_getPointeeType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3766:25
	addExtern< CXCursor (*)(CXType) , clang_getTypeDeclaration ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypeDeclaration",SideEffects::worstDefault,"clang_getTypeDeclaration")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3771:25
	addExtern< CXString (*)(CXCursor) , clang_getDeclObjCTypeEncoding ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDeclObjCTypeEncoding",SideEffects::worstDefault,"clang_getDeclObjCTypeEncoding")
		->args({"C"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3776:25
	addExtern< CXString (*)(CXType) , clang_Type_getObjCEncoding ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCEncoding",SideEffects::worstDefault,"clang_Type_getObjCEncoding")
		->args({"type"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3781:25
	addExtern< CXString (*)(CXTypeKind) , clang_getTypeKindSpelling ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypeKindSpelling",SideEffects::worstDefault,"clang_getTypeKindSpelling")
		->args({"K"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3788:35
	addExtern< CXCallingConv (*)(CXType) , clang_getFunctionTypeCallingConv >(*this,lib,"clang_getFunctionTypeCallingConv",SideEffects::worstDefault,"clang_getFunctionTypeCallingConv")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3795:23
	addExtern< CXType (*)(CXType) , clang_getResultType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getResultType",SideEffects::worstDefault,"clang_getResultType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3803:20
	addExtern< int (*)(CXType) , clang_getExceptionSpecificationType >(*this,lib,"clang_getExceptionSpecificationType",SideEffects::worstDefault,"clang_getExceptionSpecificationType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3811:20
	addExtern< int (*)(CXType) , clang_getNumArgTypes >(*this,lib,"clang_getNumArgTypes",SideEffects::worstDefault,"clang_getNumArgTypes")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3819:23
	addExtern< CXType (*)(CXType,unsigned int) , clang_getArgType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getArgType",SideEffects::worstDefault,"clang_getArgType")
		->args({"T","i"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3826:23
	addExtern< CXType (*)(CXType) , clang_Type_getObjCObjectBaseType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCObjectBaseType",SideEffects::worstDefault,"clang_Type_getObjCObjectBaseType")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3833:25
	addExtern< unsigned int (*)(CXType) , clang_Type_getNumObjCProtocolRefs >(*this,lib,"clang_Type_getNumObjCProtocolRefs",SideEffects::worstDefault,"clang_Type_getNumObjCProtocolRefs")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3841:25
	addExtern< CXCursor (*)(CXType,unsigned int) , clang_Type_getObjCProtocolDecl ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCProtocolDecl",SideEffects::worstDefault,"clang_Type_getObjCProtocolDecl")
		->args({"T","i"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3848:25
	addExtern< unsigned int (*)(CXType) , clang_Type_getNumObjCTypeArgs >(*this,lib,"clang_Type_getNumObjCTypeArgs",SideEffects::worstDefault,"clang_Type_getNumObjCTypeArgs")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3856:23
	addExtern< CXType (*)(CXType,unsigned int) , clang_Type_getObjCTypeArg ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCTypeArg",SideEffects::worstDefault,"clang_Type_getObjCTypeArg")
		->args({"T","i"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3861:25
	addExtern< unsigned int (*)(CXType) , clang_isFunctionTypeVariadic >(*this,lib,"clang_isFunctionTypeVariadic",SideEffects::worstDefault,"clang_isFunctionTypeVariadic")
		->args({"T"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3868:23
	addExtern< CXType (*)(CXCursor) , clang_getCursorResultType ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorResultType",SideEffects::worstDefault,"clang_getCursorResultType")
		->args({"C"});
}
}

