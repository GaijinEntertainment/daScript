// this file is generated via dasClangBind
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
void Module_dasClangBind::initFunctions_8( ModuleLibrary & lib ) {
	addExtern<unsigned int (*)(CXType),clang_isRestrictQualifiedType>(*this,lib,"clang_isRestrictQualifiedType",SideEffects::worstDefault,"clang_isRestrictQualifiedType")
		->args({"T"});
	addExtern<unsigned int (*)(CXType),clang_getAddressSpace>(*this,lib,"clang_getAddressSpace",SideEffects::worstDefault,"clang_getAddressSpace")
		->args({"T"});
	addExtern<CXString (*)(CXType),clang_getTypedefName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypedefName",SideEffects::worstDefault,"clang_getTypedefName")
		->args({"CT"});
	addExtern<CXType (*)(CXType),clang_getPointeeType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getPointeeType",SideEffects::worstDefault,"clang_getPointeeType")
		->args({"T"});
	addExtern<CXCursor (*)(CXType),clang_getTypeDeclaration,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypeDeclaration",SideEffects::worstDefault,"clang_getTypeDeclaration")
		->args({"T"});
	addExtern<CXString (*)(CXCursor),clang_getDeclObjCTypeEncoding,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDeclObjCTypeEncoding",SideEffects::worstDefault,"clang_getDeclObjCTypeEncoding")
		->args({"C"});
	addExtern<CXString (*)(CXType),clang_Type_getObjCEncoding,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCEncoding",SideEffects::worstDefault,"clang_Type_getObjCEncoding")
		->args({"type"});
	addExtern<CXString (*)(CXTypeKind),clang_getTypeKindSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypeKindSpelling",SideEffects::worstDefault,"clang_getTypeKindSpelling")
		->args({"K"});
	addExtern<CXCallingConv (*)(CXType),clang_getFunctionTypeCallingConv>(*this,lib,"clang_getFunctionTypeCallingConv",SideEffects::worstDefault,"clang_getFunctionTypeCallingConv")
		->args({"T"});
	addExtern<CXType (*)(CXType),clang_getResultType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getResultType",SideEffects::worstDefault,"clang_getResultType")
		->args({"T"});
	addExtern<int (*)(CXType),clang_getExceptionSpecificationType>(*this,lib,"clang_getExceptionSpecificationType",SideEffects::worstDefault,"clang_getExceptionSpecificationType")
		->args({"T"});
	addExtern<int (*)(CXType),clang_getNumArgTypes>(*this,lib,"clang_getNumArgTypes",SideEffects::worstDefault,"clang_getNumArgTypes")
		->args({"T"});
	addExtern<CXType (*)(CXType,unsigned int),clang_getArgType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getArgType",SideEffects::worstDefault,"clang_getArgType")
		->args({"T","i"});
	addExtern<CXType (*)(CXType),clang_Type_getObjCObjectBaseType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCObjectBaseType",SideEffects::worstDefault,"clang_Type_getObjCObjectBaseType")
		->args({"T"});
	addExtern<unsigned int (*)(CXType),clang_Type_getNumObjCProtocolRefs>(*this,lib,"clang_Type_getNumObjCProtocolRefs",SideEffects::worstDefault,"clang_Type_getNumObjCProtocolRefs")
		->args({"T"});
	addExtern<CXCursor (*)(CXType,unsigned int),clang_Type_getObjCProtocolDecl,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCProtocolDecl",SideEffects::worstDefault,"clang_Type_getObjCProtocolDecl")
		->args({"T","i"});
	addExtern<unsigned int (*)(CXType),clang_Type_getNumObjCTypeArgs>(*this,lib,"clang_Type_getNumObjCTypeArgs",SideEffects::worstDefault,"clang_Type_getNumObjCTypeArgs")
		->args({"T"});
	addExtern<CXType (*)(CXType,unsigned int),clang_Type_getObjCTypeArg,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCTypeArg",SideEffects::worstDefault,"clang_Type_getObjCTypeArg")
		->args({"T","i"});
	addExtern<unsigned int (*)(CXType),clang_isFunctionTypeVariadic>(*this,lib,"clang_isFunctionTypeVariadic",SideEffects::worstDefault,"clang_isFunctionTypeVariadic")
		->args({"T"});
	addExtern<CXType (*)(CXCursor),clang_getCursorResultType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorResultType",SideEffects::worstDefault,"clang_getCursorResultType")
		->args({"C"});
}
}

