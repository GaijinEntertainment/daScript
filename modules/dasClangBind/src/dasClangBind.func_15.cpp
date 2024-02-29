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
void Module_dasClangBind::initFunctions_15() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5347:6
	makeExtern< void (*)(CXCodeCompleteResults *) , clang_disposeCodeCompleteResults , SimNode_ExtFuncCall >(lib,"clang_disposeCodeCompleteResults","clang_disposeCodeCompleteResults")
		->args({"Results"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5354:10
	makeExtern< unsigned int (*)(CXCodeCompleteResults *) , clang_codeCompleteGetNumDiagnostics , SimNode_ExtFuncCall >(lib,"clang_codeCompleteGetNumDiagnostics","clang_codeCompleteGetNumDiagnostics")
		->args({"Results"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5366:14
	makeExtern< void * (*)(CXCodeCompleteResults *,unsigned int) , clang_codeCompleteGetDiagnostic , SimNode_ExtFuncCall >(lib,"clang_codeCompleteGetDiagnostic","clang_codeCompleteGetDiagnostic")
		->args({"Results","Index"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5380:1
	makeExtern< unsigned long long (*)(CXCodeCompleteResults *) , clang_codeCompleteGetContexts , SimNode_ExtFuncCall >(lib,"clang_codeCompleteGetContexts","clang_codeCompleteGetContexts")
		->args({"Results"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5400:1
	makeExtern< CXCursorKind (*)(CXCodeCompleteResults *,unsigned int *) , clang_codeCompleteGetContainerKind , SimNode_ExtFuncCall >(lib,"clang_codeCompleteGetContainerKind","clang_codeCompleteGetContainerKind")
		->args({"Results","IsIncomplete"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5413:10
	makeExtern< CXString (*)(CXCodeCompleteResults *) , clang_codeCompleteGetContainerUSR , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_codeCompleteGetContainerUSR","clang_codeCompleteGetContainerUSR")
		->args({"Results"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5427:10
	makeExtern< CXString (*)(CXCodeCompleteResults *) , clang_codeCompleteGetObjCSelector , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_codeCompleteGetObjCSelector","clang_codeCompleteGetObjCSelector")
		->args({"Results"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5443:25
	makeExtern< CXString (*)() , clang_getClangVersion , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getClangVersion","clang_getClangVersion")
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5451:21
	makeExtern< void (*)(unsigned int) , clang_toggleCrashRecovery , SimNode_ExtFuncCall >(lib,"clang_toggleCrashRecovery","clang_toggleCrashRecovery")
		->args({"isEnabled"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5502:29
	makeExtern< void * (*)(CXCursor) , clang_Cursor_Evaluate , SimNode_ExtFuncCall >(lib,"clang_Cursor_Evaluate","clang_Cursor_Evaluate")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5507:33
	makeExtern< CXEvalResultKind (*)(void *) , clang_EvalResult_getKind , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getKind","clang_EvalResult_getKind")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5513:20
	makeExtern< int (*)(void *) , clang_EvalResult_getAsInt , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsInt","clang_EvalResult_getAsInt")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5520:26
	makeExtern< long long (*)(void *) , clang_EvalResult_getAsLongLong , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsLongLong","clang_EvalResult_getAsLongLong")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5526:25
	makeExtern< unsigned int (*)(void *) , clang_EvalResult_isUnsignedInt , SimNode_ExtFuncCall >(lib,"clang_EvalResult_isUnsignedInt","clang_EvalResult_isUnsignedInt")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5533:1
	makeExtern< unsigned long long (*)(void *) , clang_EvalResult_getAsUnsigned , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsUnsigned","clang_EvalResult_getAsUnsigned")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5539:23
	makeExtern< double (*)(void *) , clang_EvalResult_getAsDouble , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsDouble","clang_EvalResult_getAsDouble")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5547:28
	makeExtern< const char * (*)(void *) , clang_EvalResult_getAsStr , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsStr","clang_EvalResult_getAsStr")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5552:21
	makeExtern< void (*)(void *) , clang_EvalResult_dispose , SimNode_ExtFuncCall >(lib,"clang_EvalResult_dispose","clang_EvalResult_dispose")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5575:28
	makeExtern< void * (*)(const char *) , clang_getRemappings , SimNode_ExtFuncCall >(lib,"clang_getRemappings","clang_getRemappings")
		->args({"path"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5588:13
	makeExtern< void * (*)(const char **,unsigned int) , clang_getRemappingsFromFileList , SimNode_ExtFuncCall >(lib,"clang_getRemappingsFromFileList","clang_getRemappingsFromFileList")
		->args({"filePaths","numFiles"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

