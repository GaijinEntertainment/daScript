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
void Module_dasClangBind::initFunctions_7() {
	addExtern<CXString (*)(CXType),clang_getTypeSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypeSpelling",SideEffects::worstDefault,"clang_getTypeSpelling")
		->args({"CT"});
	addExtern<CXType (*)(CXCursor),clang_getTypedefDeclUnderlyingType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypedefDeclUnderlyingType",SideEffects::worstDefault,"clang_getTypedefDeclUnderlyingType")
		->args({"C"});
	addExtern<CXType (*)(CXCursor),clang_getEnumDeclIntegerType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getEnumDeclIntegerType",SideEffects::worstDefault,"clang_getEnumDeclIntegerType")
		->args({"C"});
	addExtern<long long (*)(CXCursor),clang_getEnumConstantDeclValue>(*this,lib,"clang_getEnumConstantDeclValue",SideEffects::worstDefault,"clang_getEnumConstantDeclValue")
		->args({"C"});
	addExtern<unsigned long long (*)(CXCursor),clang_getEnumConstantDeclUnsignedValue>(*this,lib,"clang_getEnumConstantDeclUnsignedValue",SideEffects::worstDefault,"clang_getEnumConstantDeclUnsignedValue")
		->args({"C"});
	addExtern<int (*)(CXCursor),clang_getFieldDeclBitWidth>(*this,lib,"clang_getFieldDeclBitWidth",SideEffects::worstDefault,"clang_getFieldDeclBitWidth")
		->args({"C"});
	addExtern<int (*)(CXCursor),clang_Cursor_getNumArguments>(*this,lib,"clang_Cursor_getNumArguments",SideEffects::worstDefault,"clang_Cursor_getNumArguments")
		->args({"C"});
	addExtern<CXCursor (*)(CXCursor,unsigned int),clang_Cursor_getArgument,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getArgument",SideEffects::worstDefault,"clang_Cursor_getArgument")
		->args({"C","i"});
	addExtern<int (*)(CXCursor),clang_Cursor_getNumTemplateArguments>(*this,lib,"clang_Cursor_getNumTemplateArguments",SideEffects::worstDefault,"clang_Cursor_getNumTemplateArguments")
		->args({"C"});
	addExtern<CXTemplateArgumentKind (*)(CXCursor,unsigned int),clang_Cursor_getTemplateArgumentKind>(*this,lib,"clang_Cursor_getTemplateArgumentKind",SideEffects::worstDefault,"clang_Cursor_getTemplateArgumentKind")
		->args({"C","I"});
	addExtern<CXType (*)(CXCursor,unsigned int),clang_Cursor_getTemplateArgumentType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getTemplateArgumentType",SideEffects::worstDefault,"clang_Cursor_getTemplateArgumentType")
		->args({"C","I"});
	addExtern<long long (*)(CXCursor,unsigned int),clang_Cursor_getTemplateArgumentValue>(*this,lib,"clang_Cursor_getTemplateArgumentValue",SideEffects::worstDefault,"clang_Cursor_getTemplateArgumentValue")
		->args({"C","I"});
	addExtern<unsigned long long (*)(CXCursor,unsigned int),clang_Cursor_getTemplateArgumentUnsignedValue>(*this,lib,"clang_Cursor_getTemplateArgumentUnsignedValue",SideEffects::worstDefault,"clang_Cursor_getTemplateArgumentUnsignedValue")
		->args({"C","I"});
	addExtern<unsigned int (*)(CXType,CXType),clang_equalTypes>(*this,lib,"clang_equalTypes",SideEffects::worstDefault,"clang_equalTypes")
		->args({"A","B"});
	addExtern<CXType (*)(CXType),clang_getCanonicalType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCanonicalType",SideEffects::worstDefault,"clang_getCanonicalType")
		->args({"T"});
	addExtern<unsigned int (*)(CXType),clang_isConstQualifiedType>(*this,lib,"clang_isConstQualifiedType",SideEffects::worstDefault,"clang_isConstQualifiedType")
		->args({"T"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isMacroFunctionLike>(*this,lib,"clang_Cursor_isMacroFunctionLike",SideEffects::worstDefault,"clang_Cursor_isMacroFunctionLike")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isMacroBuiltin>(*this,lib,"clang_Cursor_isMacroBuiltin",SideEffects::worstDefault,"clang_Cursor_isMacroBuiltin")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isFunctionInlined>(*this,lib,"clang_Cursor_isFunctionInlined",SideEffects::worstDefault,"clang_Cursor_isFunctionInlined")
		->args({"C"});
	addExtern<unsigned int (*)(CXType),clang_isVolatileQualifiedType>(*this,lib,"clang_isVolatileQualifiedType",SideEffects::worstDefault,"clang_isVolatileQualifiedType")
		->args({"T"});
}
}

