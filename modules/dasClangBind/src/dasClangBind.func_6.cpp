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
void Module_dasClangBind::initFunctions_6() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3091:25
	makeExtern< unsigned int (*)(CXType) , clang_isRestrictQualifiedType , SimNode_ExtFuncCall >(lib,"clang_isRestrictQualifiedType","clang_isRestrictQualifiedType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3096:25
	makeExtern< unsigned int (*)(CXType) , clang_getAddressSpace , SimNode_ExtFuncCall >(lib,"clang_getAddressSpace","clang_getAddressSpace")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3101:25
	makeExtern< CXString (*)(CXType) , clang_getTypedefName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypedefName","clang_getTypedefName")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3106:23
	makeExtern< CXType (*)(CXType) , clang_getPointeeType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getPointeeType","clang_getPointeeType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3143:23
	makeExtern< CXType (*)(CXType) , clang_getUnqualifiedType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getUnqualifiedType","clang_getUnqualifiedType")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3154:23
	makeExtern< CXType (*)(CXType) , clang_getNonReferenceType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getNonReferenceType","clang_getNonReferenceType")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3159:25
	makeExtern< CXCursor (*)(CXType) , clang_getTypeDeclaration , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypeDeclaration","clang_getTypeDeclaration")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3164:25
	makeExtern< CXString (*)(CXCursor) , clang_getDeclObjCTypeEncoding , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getDeclObjCTypeEncoding","clang_getDeclObjCTypeEncoding")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3169:25
	makeExtern< CXString (*)(CXType) , clang_Type_getObjCEncoding , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getObjCEncoding","clang_Type_getObjCEncoding")
		->args({"type"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3174:25
	makeExtern< CXString (*)(CXTypeKind) , clang_getTypeKindSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypeKindSpelling","clang_getTypeKindSpelling")
		->args({"K"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3181:35
	makeExtern< CXCallingConv (*)(CXType) , clang_getFunctionTypeCallingConv , SimNode_ExtFuncCall >(lib,"clang_getFunctionTypeCallingConv","clang_getFunctionTypeCallingConv")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3188:23
	makeExtern< CXType (*)(CXType) , clang_getResultType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getResultType","clang_getResultType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3196:20
	makeExtern< int (*)(CXType) , clang_getExceptionSpecificationType , SimNode_ExtFuncCall >(lib,"clang_getExceptionSpecificationType","clang_getExceptionSpecificationType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3204:20
	makeExtern< int (*)(CXType) , clang_getNumArgTypes , SimNode_ExtFuncCall >(lib,"clang_getNumArgTypes","clang_getNumArgTypes")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3212:23
	makeExtern< CXType (*)(CXType,unsigned int) , clang_getArgType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getArgType","clang_getArgType")
		->args({"T","i"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3219:23
	makeExtern< CXType (*)(CXType) , clang_Type_getObjCObjectBaseType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getObjCObjectBaseType","clang_Type_getObjCObjectBaseType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3226:25
	makeExtern< unsigned int (*)(CXType) , clang_Type_getNumObjCProtocolRefs , SimNode_ExtFuncCall >(lib,"clang_Type_getNumObjCProtocolRefs","clang_Type_getNumObjCProtocolRefs")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3234:25
	makeExtern< CXCursor (*)(CXType,unsigned int) , clang_Type_getObjCProtocolDecl , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getObjCProtocolDecl","clang_Type_getObjCProtocolDecl")
		->args({"T","i"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3241:25
	makeExtern< unsigned int (*)(CXType) , clang_Type_getNumObjCTypeArgs , SimNode_ExtFuncCall >(lib,"clang_Type_getNumObjCTypeArgs","clang_Type_getNumObjCTypeArgs")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3249:23
	makeExtern< CXType (*)(CXType,unsigned int) , clang_Type_getObjCTypeArg , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getObjCTypeArg","clang_Type_getObjCTypeArg")
		->args({"T","i"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

