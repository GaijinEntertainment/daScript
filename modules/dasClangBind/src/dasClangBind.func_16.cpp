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
void Module_dasClangBind::initFunctions_16() {
// from clang-c/Index.h:5889:14
	makeExtern< void * (*)(CXCodeCompleteResults *,unsigned int) , clang_codeCompleteGetDiagnostic , SimNode_ExtFuncCall >(lib,"clang_codeCompleteGetDiagnostic","clang_codeCompleteGetDiagnostic")
		->args({"Results","Index"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5903:1
	makeExtern< unsigned long long (*)(CXCodeCompleteResults *) , clang_codeCompleteGetContexts , SimNode_ExtFuncCall >(lib,"clang_codeCompleteGetContexts","clang_codeCompleteGetContexts")
		->args({"Results"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5923:1
	makeExtern< CXCursorKind (*)(CXCodeCompleteResults *,unsigned int *) , clang_codeCompleteGetContainerKind , SimNode_ExtFuncCall >(lib,"clang_codeCompleteGetContainerKind","clang_codeCompleteGetContainerKind")
		->args({"Results","IsIncomplete"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5936:10
	makeExtern< CXString (*)(CXCodeCompleteResults *) , clang_codeCompleteGetContainerUSR , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_codeCompleteGetContainerUSR","clang_codeCompleteGetContainerUSR")
		->args({"Results"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5950:10
	makeExtern< CXString (*)(CXCodeCompleteResults *) , clang_codeCompleteGetObjCSelector , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_codeCompleteGetObjCSelector","clang_codeCompleteGetObjCSelector")
		->args({"Results"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5966:25
	makeExtern< CXString (*)() , clang_getClangVersion , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getClangVersion","clang_getClangVersion")
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:5974:21
	makeExtern< void (*)(unsigned int) , clang_toggleCrashRecovery , SimNode_ExtFuncCall >(lib,"clang_toggleCrashRecovery","clang_toggleCrashRecovery")
		->args({"isEnabled"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6025:29
	makeExtern< void * (*)(CXCursor) , clang_Cursor_Evaluate , SimNode_ExtFuncCall >(lib,"clang_Cursor_Evaluate","clang_Cursor_Evaluate")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6030:33
	makeExtern< CXEvalResultKind (*)(void *) , clang_EvalResult_getKind , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getKind","clang_EvalResult_getKind")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6036:20
	makeExtern< int (*)(void *) , clang_EvalResult_getAsInt , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsInt","clang_EvalResult_getAsInt")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6043:26
	makeExtern< long long (*)(void *) , clang_EvalResult_getAsLongLong , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsLongLong","clang_EvalResult_getAsLongLong")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6049:25
	makeExtern< unsigned int (*)(void *) , clang_EvalResult_isUnsignedInt , SimNode_ExtFuncCall >(lib,"clang_EvalResult_isUnsignedInt","clang_EvalResult_isUnsignedInt")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6056:1
	makeExtern< unsigned long long (*)(void *) , clang_EvalResult_getAsUnsigned , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsUnsigned","clang_EvalResult_getAsUnsigned")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6062:23
	makeExtern< double (*)(void *) , clang_EvalResult_getAsDouble , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsDouble","clang_EvalResult_getAsDouble")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6070:28
	makeExtern< const char * (*)(void *) , clang_EvalResult_getAsStr , SimNode_ExtFuncCall >(lib,"clang_EvalResult_getAsStr","clang_EvalResult_getAsStr")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6075:21
	makeExtern< void (*)(void *) , clang_EvalResult_dispose , SimNode_ExtFuncCall >(lib,"clang_EvalResult_dispose","clang_EvalResult_dispose")
		->args({"E"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6123:25
	makeExtern< CXResult (*)(CXCursor,void *,CXCursorAndRangeVisitor) , clang_findReferencesInFile , SimNode_ExtFuncCall >(lib,"clang_findReferencesInFile","clang_findReferencesInFile")
		->args({"cursor","file","visitor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6138:25
	makeExtern< CXResult (*)(CXTranslationUnitImpl *,void *,CXCursorAndRangeVisitor) , clang_findIncludesInFile , SimNode_ExtFuncCall >(lib,"clang_findIncludesInFile","clang_findIncludesInFile")
		->args({"TU","file","visitor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6149:10
	makeExtern< CXResult (*)(CXCursor,void *,_CXCursorAndRangeVisitorBlock *) , clang_findReferencesInFileWithBlock , SimNode_ExtFuncCall >(lib,"clang_findReferencesInFileWithBlock","clang_findReferencesInFileWithBlock")
		->args({"","",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6153:10
	makeExtern< CXResult (*)(CXTranslationUnitImpl *,void *,_CXCursorAndRangeVisitorBlock *) , clang_findIncludesInFileWithBlock , SimNode_ExtFuncCall >(lib,"clang_findIncludesInFileWithBlock","clang_findIncludesInFileWithBlock")
		->args({"","",""})
		->addToModule(*this, SideEffects::worstDefault);
}
}

