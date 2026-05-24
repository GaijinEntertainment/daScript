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
void Module_dasClangBind::initFunctions_7() {
// from clang-c/Index.h:2870:30
	makeExtern< CXSourceRange (*)(CXCursor) , clang_getCursorExtent , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorExtent","clang_getCursorExtent")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3102:23
	makeExtern< CXType (*)(CXCursor) , clang_getCursorType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCursorType","clang_getCursorType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3110:25
	makeExtern< CXString (*)(CXType) , clang_getTypeSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypeSpelling","clang_getTypeSpelling")
		->args({"CT"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3118:23
	makeExtern< CXType (*)(CXCursor) , clang_getTypedefDeclUnderlyingType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTypedefDeclUnderlyingType","clang_getTypedefDeclUnderlyingType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3126:23
	makeExtern< CXType (*)(CXCursor) , clang_getEnumDeclIntegerType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getEnumDeclIntegerType","clang_getEnumDeclIntegerType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3136:26
	makeExtern< long long (*)(CXCursor) , clang_getEnumConstantDeclValue , SimNode_ExtFuncCall >(lib,"clang_getEnumConstantDeclValue","clang_getEnumConstantDeclValue")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3147:1
	makeExtern< unsigned long long (*)(CXCursor) , clang_getEnumConstantDeclUnsignedValue , SimNode_ExtFuncCall >(lib,"clang_getEnumConstantDeclUnsignedValue","clang_getEnumConstantDeclUnsignedValue")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3152:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isBitField , SimNode_ExtFuncCall >(lib,"clang_Cursor_isBitField","clang_Cursor_isBitField")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3170:20
	makeExtern< int (*)(CXCursor) , clang_getFieldDeclBitWidth , SimNode_ExtFuncCall >(lib,"clang_getFieldDeclBitWidth","clang_getFieldDeclBitWidth")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3179:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_getNumArguments , SimNode_ExtFuncCall >(lib,"clang_Cursor_getNumArguments","clang_Cursor_getNumArguments")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3188:25
	makeExtern< CXCursor (*)(CXCursor,unsigned int) , clang_Cursor_getArgument , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getArgument","clang_Cursor_getArgument")
		->args({"C","i"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3226:20
	makeExtern< int (*)(CXCursor) , clang_Cursor_getNumTemplateArguments , SimNode_ExtFuncCall >(lib,"clang_Cursor_getNumTemplateArguments","clang_Cursor_getNumTemplateArguments")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3246:1
	makeExtern< CXTemplateArgumentKind (*)(CXCursor,unsigned int) , clang_Cursor_getTemplateArgumentKind , SimNode_ExtFuncCall >(lib,"clang_Cursor_getTemplateArgumentKind","clang_Cursor_getTemplateArgumentKind")
		->args({"C","I"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3266:23
	makeExtern< CXType (*)(CXCursor,unsigned int) , clang_Cursor_getTemplateArgumentType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getTemplateArgumentType","clang_Cursor_getTemplateArgumentType")
		->args({"C","I"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3287:26
	makeExtern< long long (*)(CXCursor,unsigned int) , clang_Cursor_getTemplateArgumentValue , SimNode_ExtFuncCall >(lib,"clang_Cursor_getTemplateArgumentValue","clang_Cursor_getTemplateArgumentValue")
		->args({"C","I"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3309:1
	makeExtern< unsigned long long (*)(CXCursor,unsigned int) , clang_Cursor_getTemplateArgumentUnsignedValue , SimNode_ExtFuncCall >(lib,"clang_Cursor_getTemplateArgumentUnsignedValue","clang_Cursor_getTemplateArgumentUnsignedValue")
		->args({"C","I"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3317:25
	makeExtern< unsigned int (*)(CXType,CXType) , clang_equalTypes , SimNode_ExtFuncCall >(lib,"clang_equalTypes","clang_equalTypes")
		->args({"A","B"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3327:23
	makeExtern< CXType (*)(CXType) , clang_getCanonicalType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCanonicalType","clang_getCanonicalType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3334:25
	makeExtern< unsigned int (*)(CXType) , clang_isConstQualifiedType , SimNode_ExtFuncCall >(lib,"clang_isConstQualifiedType","clang_isConstQualifiedType")
		->args({"T"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:3340:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isMacroFunctionLike , SimNode_ExtFuncCall >(lib,"clang_Cursor_isMacroFunctionLike","clang_Cursor_isMacroFunctionLike")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

