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
void Module_dasClangBind::initFunctions_15() {
	addExtern< CXString (*)(CXCodeCompleteResults *) , clang_codeCompleteGetContainerUSR ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_codeCompleteGetContainerUSR",SideEffects::worstDefault,"clang_codeCompleteGetContainerUSR")
		->args({"Results"});
	addExtern< CXString (*)(CXCodeCompleteResults *) , clang_codeCompleteGetObjCSelector ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_codeCompleteGetObjCSelector",SideEffects::worstDefault,"clang_codeCompleteGetObjCSelector")
		->args({"Results"});
	addExtern< CXString (*)() , clang_getClangVersion ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getClangVersion",SideEffects::worstDefault,"clang_getClangVersion");
	addExtern< void (*)(unsigned int) , clang_toggleCrashRecovery >(*this,lib,"clang_toggleCrashRecovery",SideEffects::worstDefault,"clang_toggleCrashRecovery")
		->args({"isEnabled"});
	addExtern< void * (*)(CXCursor) , clang_Cursor_Evaluate >(*this,lib,"clang_Cursor_Evaluate",SideEffects::worstDefault,"clang_Cursor_Evaluate")
		->args({"C"});
	addExtern< CXEvalResultKind (*)(void *) , clang_EvalResult_getKind >(*this,lib,"clang_EvalResult_getKind",SideEffects::worstDefault,"clang_EvalResult_getKind")
		->args({"E"});
	addExtern< int (*)(void *) , clang_EvalResult_getAsInt >(*this,lib,"clang_EvalResult_getAsInt",SideEffects::worstDefault,"clang_EvalResult_getAsInt")
		->args({"E"});
	addExtern< long long (*)(void *) , clang_EvalResult_getAsLongLong >(*this,lib,"clang_EvalResult_getAsLongLong",SideEffects::worstDefault,"clang_EvalResult_getAsLongLong")
		->args({"E"});
	addExtern< unsigned int (*)(void *) , clang_EvalResult_isUnsignedInt >(*this,lib,"clang_EvalResult_isUnsignedInt",SideEffects::worstDefault,"clang_EvalResult_isUnsignedInt")
		->args({"E"});
	addExtern< unsigned long long (*)(void *) , clang_EvalResult_getAsUnsigned >(*this,lib,"clang_EvalResult_getAsUnsigned",SideEffects::worstDefault,"clang_EvalResult_getAsUnsigned")
		->args({"E"});
	addExtern< double (*)(void *) , clang_EvalResult_getAsDouble >(*this,lib,"clang_EvalResult_getAsDouble",SideEffects::worstDefault,"clang_EvalResult_getAsDouble")
		->args({"E"});
	addExtern< const char * (*)(void *) , clang_EvalResult_getAsStr >(*this,lib,"clang_EvalResult_getAsStr",SideEffects::worstDefault,"clang_EvalResult_getAsStr")
		->args({"E"});
	addExtern< void (*)(void *) , clang_EvalResult_dispose >(*this,lib,"clang_EvalResult_dispose",SideEffects::worstDefault,"clang_EvalResult_dispose")
		->args({"E"});
	addExtern< void * (*)(const char *) , clang_getRemappings >(*this,lib,"clang_getRemappings",SideEffects::worstDefault,"clang_getRemappings")
		->args({"path"});
	addExtern< void * (*)(const char **,unsigned int) , clang_getRemappingsFromFileList >(*this,lib,"clang_getRemappingsFromFileList",SideEffects::worstDefault,"clang_getRemappingsFromFileList")
		->args({"filePaths","numFiles"});
	addExtern< unsigned int (*)(void *) , clang_remap_getNumFiles >(*this,lib,"clang_remap_getNumFiles",SideEffects::worstDefault,"clang_remap_getNumFiles")
		->args({""});
	addExtern< void (*)(void *,unsigned int,CXString *,CXString *) , clang_remap_getFilenames >(*this,lib,"clang_remap_getFilenames",SideEffects::worstDefault,"clang_remap_getFilenames")
		->args({"","index","original","transformed"});
	addExtern< void (*)(void *) , clang_remap_dispose >(*this,lib,"clang_remap_dispose",SideEffects::worstDefault,"clang_remap_dispose")
		->args({""});
	addExtern< CXResult (*)(CXCursor,void *,CXCursorAndRangeVisitor) , clang_findReferencesInFile >(*this,lib,"clang_findReferencesInFile",SideEffects::worstDefault,"clang_findReferencesInFile")
		->args({"cursor","file","visitor"});
	addExtern< CXResult (*)(CXTranslationUnitImpl *,void *,CXCursorAndRangeVisitor) , clang_findIncludesInFile >(*this,lib,"clang_findIncludesInFile",SideEffects::worstDefault,"clang_findIncludesInFile")
		->args({"TU","file","visitor"});
}
}

