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
void Module_dasClangBind::initFunctions_8() {
// from clang-c/Index.h:3346:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isMacroBuiltin , SimNode_ExtFuncCall >(lib,"clang_Cursor_isMacroBuiltin","clang_Cursor_isMacroBuiltin")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3352:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isFunctionInlined , SimNode_ExtFuncCall >(lib,"clang_Cursor_isFunctionInlined","clang_Cursor_isFunctionInlined")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3359:25
	makeExtern< unsigned int (*)(CXType) , clang_isVolatileQualifiedType , SimNode_ExtFuncCall >(lib,"clang_isVolatileQualifiedType","clang_isVolatileQualifiedType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3366:25
	makeExtern< unsigned int (*)(CXType) , clang_isRestrictQualifiedType , SimNode_ExtFuncCall >(lib,"clang_isRestrictQualifiedType","clang_isRestrictQualifiedType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3371:25
	makeExtern< unsigned int (*)(CXType) , clang_getAddressSpace , SimNode_ExtFuncCall >(lib,"clang_getAddressSpace","clang_getAddressSpace")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3376:25
	makeExtern< CXString (*)(CXType) , clang_getTypedefName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypedefName","clang_getTypedefName")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3381:23
	makeExtern< CXType (*)(CXType) , clang_getPointeeType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getPointeeType","clang_getPointeeType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3418:23
	makeExtern< CXType (*)(CXType) , clang_getUnqualifiedType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getUnqualifiedType","clang_getUnqualifiedType")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3429:23
	makeExtern< CXType (*)(CXType) , clang_getNonReferenceType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getNonReferenceType","clang_getNonReferenceType")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3434:25
	makeExtern< CXCursor (*)(CXType) , clang_getTypeDeclaration , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypeDeclaration","clang_getTypeDeclaration")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3439:25
	makeExtern< CXString (*)(CXCursor) , clang_getDeclObjCTypeEncoding , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getDeclObjCTypeEncoding","clang_getDeclObjCTypeEncoding")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3444:25
	makeExtern< CXString (*)(CXType) , clang_Type_getObjCEncoding , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getObjCEncoding","clang_Type_getObjCEncoding")
		->args({"type"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3449:25
	makeExtern< CXString (*)(CXTypeKind) , clang_getTypeKindSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypeKindSpelling","clang_getTypeKindSpelling")
		->args({"K"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3456:35
	makeExtern< CXCallingConv (*)(CXType) , clang_getFunctionTypeCallingConv , SimNode_ExtFuncCall >(lib,"clang_getFunctionTypeCallingConv","clang_getFunctionTypeCallingConv")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3463:23
	makeExtern< CXType (*)(CXType) , clang_getResultType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getResultType","clang_getResultType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3471:20
	makeExtern< int (*)(CXType) , clang_getExceptionSpecificationType , SimNode_ExtFuncCall >(lib,"clang_getExceptionSpecificationType","clang_getExceptionSpecificationType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3479:20
	makeExtern< int (*)(CXType) , clang_getNumArgTypes , SimNode_ExtFuncCall >(lib,"clang_getNumArgTypes","clang_getNumArgTypes")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3487:23
	makeExtern< CXType (*)(CXType,unsigned int) , clang_getArgType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getArgType","clang_getArgType")
		->args({"T","i"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3494:23
	makeExtern< CXType (*)(CXType) , clang_Type_getObjCObjectBaseType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Type_getObjCObjectBaseType","clang_Type_getObjCObjectBaseType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3501:25
	makeExtern< unsigned int (*)(CXType) , clang_Type_getNumObjCProtocolRefs , SimNode_ExtFuncCall >(lib,"clang_Type_getNumObjCProtocolRefs","clang_Type_getNumObjCProtocolRefs")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

