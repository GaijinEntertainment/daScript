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
void Module_dasClangBind::initFunctions_5() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2851:25
	makeExtern< CXString (*)(CXType) , clang_getTypeSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypeSpelling","clang_getTypeSpelling")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2859:23
	makeExtern< CXType (*)(CXCursor) , clang_getTypedefDeclUnderlyingType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypedefDeclUnderlyingType","clang_getTypedefDeclUnderlyingType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2867:23
	makeExtern< CXType (*)(CXCursor) , clang_getEnumDeclIntegerType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getEnumDeclIntegerType","clang_getEnumDeclIntegerType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2877:26
	makeExtern< long long (*)(CXCursor) , clang_getEnumConstantDeclValue , SimNode_ExtFuncCall >(lib,"clang_getEnumConstantDeclValue","clang_getEnumConstantDeclValue")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2888:1
	makeExtern< unsigned long long (*)(CXCursor) , clang_getEnumConstantDeclUnsignedValue , SimNode_ExtFuncCall >(lib,"clang_getEnumConstantDeclUnsignedValue","clang_getEnumConstantDeclUnsignedValue")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2895:20
	makeExtern< int (*)(CXCursor) , clang_getFieldDeclBitWidth , SimNode_ExtFuncCall >(lib,"clang_getFieldDeclBitWidth","clang_getFieldDeclBitWidth")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2904:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_getNumArguments , SimNode_ExtFuncCall >(lib,"clang_Cursor_getNumArguments","clang_Cursor_getNumArguments")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2913:25
	makeExtern< CXCursor (*)(CXCursor,unsigned int) , clang_Cursor_getArgument , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getArgument","clang_Cursor_getArgument")
		->args({"C","i"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2951:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_getNumTemplateArguments , SimNode_ExtFuncCall >(lib,"clang_Cursor_getNumTemplateArguments","clang_Cursor_getNumTemplateArguments")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2971:1
	makeExtern< CXTemplateArgumentKind (*)(CXCursor,unsigned int) , clang_Cursor_getTemplateArgumentKind , SimNode_ExtFuncCall >(lib,"clang_Cursor_getTemplateArgumentKind","clang_Cursor_getTemplateArgumentKind")
		->args({"C","I"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2991:23
	makeExtern< CXType (*)(CXCursor,unsigned int) , clang_Cursor_getTemplateArgumentType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getTemplateArgumentType","clang_Cursor_getTemplateArgumentType")
		->args({"C","I"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3012:26
	makeExtern< long long (*)(CXCursor,unsigned int) , clang_Cursor_getTemplateArgumentValue , SimNode_ExtFuncCall >(lib,"clang_Cursor_getTemplateArgumentValue","clang_Cursor_getTemplateArgumentValue")
		->args({"C","I"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3034:1
	makeExtern< unsigned long long (*)(CXCursor,unsigned int) , clang_Cursor_getTemplateArgumentUnsignedValue , SimNode_ExtFuncCall >(lib,"clang_Cursor_getTemplateArgumentUnsignedValue","clang_Cursor_getTemplateArgumentUnsignedValue")
		->args({"C","I"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3042:25
	makeExtern< unsigned int (*)(CXType,CXType) , clang_equalTypes , SimNode_ExtFuncCall >(lib,"clang_equalTypes","clang_equalTypes")
		->args({"A","B"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3052:23
	makeExtern< CXType (*)(CXType) , clang_getCanonicalType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCanonicalType","clang_getCanonicalType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3059:25
	makeExtern< unsigned int (*)(CXType) , clang_isConstQualifiedType , SimNode_ExtFuncCall >(lib,"clang_isConstQualifiedType","clang_isConstQualifiedType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3065:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isMacroFunctionLike , SimNode_ExtFuncCall >(lib,"clang_Cursor_isMacroFunctionLike","clang_Cursor_isMacroFunctionLike")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3071:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isMacroBuiltin , SimNode_ExtFuncCall >(lib,"clang_Cursor_isMacroBuiltin","clang_Cursor_isMacroBuiltin")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3077:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isFunctionInlined , SimNode_ExtFuncCall >(lib,"clang_Cursor_isFunctionInlined","clang_Cursor_isFunctionInlined")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:3084:25
	makeExtern< unsigned int (*)(CXType) , clang_isVolatileQualifiedType , SimNode_ExtFuncCall >(lib,"clang_isVolatileQualifiedType","clang_isVolatileQualifiedType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

