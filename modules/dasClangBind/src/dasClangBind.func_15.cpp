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
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6224:20
	makeExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,CXTranslationUnitImpl **,unsigned int) , clang_indexSourceFileFullArgv , SimNode_ExtFuncCall >(lib,"clang_indexSourceFileFullArgv","clang_indexSourceFileFullArgv")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","out_TU","TU_options"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6247:20
	makeExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,CXTranslationUnitImpl *) , clang_indexTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_indexTranslationUnit","clang_indexTranslationUnit")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options",""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6259:21
	makeExtern< void (*)(CXIdxLoc,void **,void **,unsigned int *,unsigned int *,unsigned int *) , clang_indexLoc_getFileLocation , SimNode_ExtFuncCall >(lib,"clang_indexLoc_getFileLocation","clang_indexLoc_getFileLocation")
		->args({"loc","indexFile","file","line","column","offset"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6269:18
	makeExtern< CXSourceLocation (*)(CXIdxLoc) , clang_indexLoc_getCXSourceLocation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_indexLoc_getCXSourceLocation","clang_indexLoc_getCXSourceLocation")
		->args({"loc"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

