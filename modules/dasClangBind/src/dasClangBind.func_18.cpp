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
void Module_dasClangBind::initFunctions_18() {
// from clang-c/Index.h:6900:1
	makeExtern< CXBinaryOperatorKind (*)(CXCursor) , clang_getCursorBinaryOperatorKind , SimNode_ExtFuncCall >(lib,"clang_getCursorBinaryOperatorKind","clang_getCursorBinaryOperatorKind")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6942:1
	makeExtern< CXString (*)(CXUnaryOperatorKind) , clang_getUnaryOperatorKindSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getUnaryOperatorKindSpelling","clang_getUnaryOperatorKindSpelling")
		->args({"kind"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6950:1
	makeExtern< CXUnaryOperatorKind (*)(CXCursor) , clang_getCursorUnaryOperatorKind , SimNode_ExtFuncCall >(lib,"clang_getCursorUnaryOperatorKind","clang_getCursorUnaryOperatorKind")
		->args({"cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6963:46
	makeExtern< void * (*)(const char *) , clang_getRemappings , SimNode_ExtFuncCall >(lib,"clang_getRemappings","clang_getRemappings")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6966:1
	makeExtern< void * (*)(const char **,unsigned int) , clang_getRemappingsFromFileList , SimNode_ExtFuncCall >(lib,"clang_getRemappingsFromFileList","clang_getRemappingsFromFileList")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6968:43
	makeExtern< unsigned int (*)(void *) , clang_remap_getNumFiles , SimNode_ExtFuncCall >(lib,"clang_remap_getNumFiles","clang_remap_getNumFiles")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6971:1
	makeExtern< void (*)(void *,unsigned int,CXString *,CXString *) , clang_remap_getFilenames , SimNode_ExtFuncCall >(lib,"clang_remap_getFilenames","clang_remap_getFilenames")
		->args({"","","",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6973:39
	makeExtern< void (*)(void *) , clang_remap_dispose , SimNode_ExtFuncCall >(lib,"clang_remap_dispose","clang_remap_dispose")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
}
}

