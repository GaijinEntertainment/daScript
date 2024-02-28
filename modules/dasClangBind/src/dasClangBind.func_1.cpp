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
void Module_dasClangBind::initFunctions_1() {
// from C:\Users\andrei_orazov\libclang\include\clang-c/CXString.h:50:28
	makeExtern< const char * (*)(CXString) , clang_getCString , SimNode_ExtFuncCall >(lib,"clang_getCString","clang_getCString")
		->args({"string"})
		->addToModule(*this, SideEffects::worstDefault);
// from C:\Users\andrei_orazov\libclang\include\clang-c/CXString.h:55:21
	makeExtern< void (*)(CXString) , clang_disposeString , SimNode_ExtFuncCall >(lib,"clang_disposeString","clang_disposeString")
		->args({"string"})
		->addToModule(*this, SideEffects::worstDefault);
// from C:\Users\andrei_orazov\libclang\include\clang-c/CXString.h:60:21
	makeExtern< void (*)(CXStringSet *) , clang_disposeStringSet , SimNode_ExtFuncCall >(lib,"clang_disposeStringSet","clang_disposeStringSet")
		->args({"set"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:267:24
	makeExtern< void * (*)(int,int) , clang_createIndex , SimNode_ExtFuncCall >(lib,"clang_createIndex","clang_createIndex")
		->args({"excludeDeclarationsFromPCH","displayDiagnostics"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:276:21
	makeExtern< void (*)(void *) , clang_disposeIndex , SimNode_ExtFuncCall >(lib,"clang_disposeIndex","clang_disposeIndex")
		->args({"index"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:325:21
	makeExtern< void (*)(void *,unsigned int) , clang_CXIndex_setGlobalOptions , SimNode_ExtFuncCall >(lib,"clang_CXIndex_setGlobalOptions","clang_CXIndex_setGlobalOptions")
		->args({"","options"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:333:25
	makeExtern< unsigned int (*)(void *) , clang_CXIndex_getGlobalOptions , SimNode_ExtFuncCall >(lib,"clang_CXIndex_getGlobalOptions","clang_CXIndex_getGlobalOptions")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:343:1
	makeExtern< void (*)(void *,const char *) , clang_CXIndex_setInvocationEmissionPathOption , SimNode_ExtFuncCall >(lib,"clang_CXIndex_setInvocationEmissionPathOption","clang_CXIndex_setInvocationEmissionPathOption")
		->args({"","Path"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:350:25
	makeExtern< unsigned int (*)(CXTranslationUnitImpl *,void *) , clang_isFileMultipleIncludeGuarded , SimNode_ExtFuncCall >(lib,"clang_isFileMultipleIncludeGuarded","clang_isFileMultipleIncludeGuarded")
		->args({"tu","file"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:363:23
	makeExtern< void * (*)(CXTranslationUnitImpl *,const char *) , clang_getFile , SimNode_ExtFuncCall >(lib,"clang_getFile","clang_getFile")
		->args({"tu","file_name"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:378:28
	makeExtern< const char * (*)(CXTranslationUnitImpl *,void *,size_t *) , clang_getFileContents , SimNode_ExtFuncCall >(lib,"clang_getFileContents","clang_getFileContents")
		->args({"tu","file","size"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:385:33
	makeExtern< CXSourceLocation (*)(CXTranslationUnitImpl *,void *,unsigned int,unsigned int) , clang_getLocation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getLocation","clang_getLocation")
		->args({"tu","file","line","column"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:392:33
	makeExtern< CXSourceLocation (*)(CXTranslationUnitImpl *,void *,unsigned int) , clang_getLocationForOffset , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getLocationForOffset","clang_getLocationForOffset")
		->args({"tu","file","offset"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:402:35
	makeExtern< CXSourceRangeList * (*)(CXTranslationUnitImpl *,void *) , clang_getSkippedRanges , SimNode_ExtFuncCall >(lib,"clang_getSkippedRanges","clang_getSkippedRanges")
		->args({"tu","file"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:413:1
	makeExtern< CXSourceRangeList * (*)(CXTranslationUnitImpl *) , clang_getAllSkippedRanges , SimNode_ExtFuncCall >(lib,"clang_getAllSkippedRanges","clang_getAllSkippedRanges")
		->args({"tu"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:419:25
	makeExtern< unsigned int (*)(CXTranslationUnitImpl *) , clang_getNumDiagnostics , SimNode_ExtFuncCall >(lib,"clang_getNumDiagnostics","clang_getNumDiagnostics")
		->args({"Unit"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:430:29
	makeExtern< void * (*)(CXTranslationUnitImpl *,unsigned int) , clang_getDiagnostic , SimNode_ExtFuncCall >(lib,"clang_getDiagnostic","clang_getDiagnostic")
		->args({"Unit","Index"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:440:1
	makeExtern< void * (*)(CXTranslationUnitImpl *) , clang_getDiagnosticSetFromTU , SimNode_ExtFuncCall >(lib,"clang_getDiagnosticSetFromTU","clang_getDiagnosticSetFromTU")
		->args({"Unit"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:456:1
	makeExtern< CXString (*)(CXTranslationUnitImpl *) , clang_getTranslationUnitSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTranslationUnitSpelling","clang_getTranslationUnitSpelling")
		->args({"CTUnit"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:498:34
	makeExtern< CXTranslationUnitImpl * (*)(void *,const char *,int,const char *const *,unsigned int,CXUnsavedFile *) , clang_createTranslationUnitFromSourceFile , SimNode_ExtFuncCall >(lib,"clang_createTranslationUnitFromSourceFile","clang_createTranslationUnitFromSourceFile")
		->args({"CIdx","source_filename","num_clang_command_line_args","clang_command_line_args","num_unsaved_files","unsaved_files"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

