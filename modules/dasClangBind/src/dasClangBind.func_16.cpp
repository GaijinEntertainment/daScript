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
void Module_dasClangBind::initFunctions_16() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6629:20
	addExtern< int (*)(CXIdxEntityKind) , clang_index_isEntityObjCContainerKind >(*this,lib,"clang_index_isEntityObjCContainerKind",SideEffects::worstDefault,"clang_index_isEntityObjCContainerKind")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6631:1
	addExtern< const CXIdxObjCContainerDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCContainerDeclInfo >(*this,lib,"clang_index_getObjCContainerDeclInfo",SideEffects::worstDefault,"clang_index_getObjCContainerDeclInfo")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6634:1
	addExtern< const CXIdxObjCInterfaceDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCInterfaceDeclInfo >(*this,lib,"clang_index_getObjCInterfaceDeclInfo",SideEffects::worstDefault,"clang_index_getObjCInterfaceDeclInfo")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6638:1
	addExtern< const CXIdxObjCCategoryDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCCategoryDeclInfo >(*this,lib,"clang_index_getObjCCategoryDeclInfo",SideEffects::worstDefault,"clang_index_getObjCCategoryDeclInfo")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6641:1
	addExtern< const CXIdxObjCProtocolRefListInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCProtocolRefListInfo >(*this,lib,"clang_index_getObjCProtocolRefListInfo",SideEffects::worstDefault,"clang_index_getObjCProtocolRefListInfo")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6644:1
	addExtern< const CXIdxObjCPropertyDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCPropertyDeclInfo >(*this,lib,"clang_index_getObjCPropertyDeclInfo",SideEffects::worstDefault,"clang_index_getObjCPropertyDeclInfo")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6647:1
	addExtern< const CXIdxIBOutletCollectionAttrInfo * (*)(const CXIdxAttrInfo *) , clang_index_getIBOutletCollectionAttrInfo >(*this,lib,"clang_index_getIBOutletCollectionAttrInfo",SideEffects::worstDefault,"clang_index_getIBOutletCollectionAttrInfo")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6650:1
	addExtern< const CXIdxCXXClassDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getCXXClassDeclInfo >(*this,lib,"clang_index_getCXXClassDeclInfo",SideEffects::worstDefault,"clang_index_getCXXClassDeclInfo")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6657:1
	addExtern< void * (*)(const CXIdxContainerInfo *) , clang_index_getClientContainer >(*this,lib,"clang_index_getClientContainer",SideEffects::worstDefault,"clang_index_getClientContainer")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6663:21
	addExtern< void (*)(const CXIdxContainerInfo *,void *) , clang_index_setClientContainer >(*this,lib,"clang_index_setClientContainer",SideEffects::worstDefault,"clang_index_setClientContainer")
		->args({"",""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6670:1
	addExtern< void * (*)(const CXIdxEntityInfo *) , clang_index_getClientEntity >(*this,lib,"clang_index_getClientEntity",SideEffects::worstDefault,"clang_index_getClientEntity")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6675:21
	addExtern< void (*)(const CXIdxEntityInfo *,void *) , clang_index_setClientEntity >(*this,lib,"clang_index_setClientEntity",SideEffects::worstDefault,"clang_index_setClientEntity")
		->args({"",""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6690:30
	addExtern< void * (*)(void *) , clang_IndexAction_create >(*this,lib,"clang_IndexAction_create",SideEffects::worstDefault,"clang_IndexAction_create")
		->args({"CIdx"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6698:21
	addExtern< void (*)(void *) , clang_IndexAction_dispose >(*this,lib,"clang_IndexAction_dispose",SideEffects::worstDefault,"clang_IndexAction_dispose")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6764:20
	addExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,CXTranslationUnitImpl **,unsigned int) , clang_indexSourceFile >(*this,lib,"clang_indexSourceFile",SideEffects::worstDefault,"clang_indexSourceFile")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","out_TU","TU_options"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6776:20
	addExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,CXTranslationUnitImpl **,unsigned int) , clang_indexSourceFileFullArgv >(*this,lib,"clang_indexSourceFileFullArgv",SideEffects::worstDefault,"clang_indexSourceFileFullArgv")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","out_TU","TU_options"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6799:20
	addExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,CXTranslationUnitImpl *) , clang_indexTranslationUnit >(*this,lib,"clang_indexTranslationUnit",SideEffects::worstDefault,"clang_indexTranslationUnit")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options",""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6811:21
	addExtern< void (*)(CXIdxLoc,void **,void **,unsigned int *,unsigned int *,unsigned int *) , clang_indexLoc_getFileLocation >(*this,lib,"clang_indexLoc_getFileLocation",SideEffects::worstDefault,"clang_indexLoc_getFileLocation")
		->args({"loc","indexFile","file","line","column","offset"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6821:18
	addExtern< CXSourceLocation (*)(CXIdxLoc) , clang_indexLoc_getCXSourceLocation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_indexLoc_getCXSourceLocation",SideEffects::worstDefault,"clang_indexLoc_getCXSourceLocation")
		->args({"loc"});
}
}

