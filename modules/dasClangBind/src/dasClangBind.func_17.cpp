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
void Module_dasClangBind::initFunctions_17() {
// from clang-c/Index.h:6538:20
	makeExtern< int (*)(CXIdxEntityKind) , clang_index_isEntityObjCContainerKind , SimNode_ExtFuncCall >(lib,"clang_index_isEntityObjCContainerKind","clang_index_isEntityObjCContainerKind")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6540:1
	makeExtern< const CXIdxObjCContainerDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCContainerDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCContainerDeclInfo","clang_index_getObjCContainerDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6543:1
	makeExtern< const CXIdxObjCInterfaceDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCInterfaceDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCInterfaceDeclInfo","clang_index_getObjCInterfaceDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6547:1
	makeExtern< const CXIdxObjCCategoryDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCCategoryDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCCategoryDeclInfo","clang_index_getObjCCategoryDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6550:1
	makeExtern< const CXIdxObjCProtocolRefListInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCProtocolRefListInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCProtocolRefListInfo","clang_index_getObjCProtocolRefListInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6553:1
	makeExtern< const CXIdxObjCPropertyDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCPropertyDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCPropertyDeclInfo","clang_index_getObjCPropertyDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6556:1
	makeExtern< const CXIdxIBOutletCollectionAttrInfo * (*)(const CXIdxAttrInfo *) , clang_index_getIBOutletCollectionAttrInfo , SimNode_ExtFuncCall >(lib,"clang_index_getIBOutletCollectionAttrInfo","clang_index_getIBOutletCollectionAttrInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6559:1
	makeExtern< const CXIdxCXXClassDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getCXXClassDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getCXXClassDeclInfo","clang_index_getCXXClassDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6566:1
	makeExtern< void * (*)(const CXIdxContainerInfo *) , clang_index_getClientContainer , SimNode_ExtFuncCall >(lib,"clang_index_getClientContainer","clang_index_getClientContainer")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6572:21
	makeExtern< void (*)(const CXIdxContainerInfo *,void *) , clang_index_setClientContainer , SimNode_ExtFuncCall >(lib,"clang_index_setClientContainer","clang_index_setClientContainer")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6579:1
	makeExtern< void * (*)(const CXIdxEntityInfo *) , clang_index_getClientEntity , SimNode_ExtFuncCall >(lib,"clang_index_getClientEntity","clang_index_getClientEntity")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6584:21
	makeExtern< void (*)(const CXIdxEntityInfo *,void *) , clang_index_setClientEntity , SimNode_ExtFuncCall >(lib,"clang_index_setClientEntity","clang_index_setClientEntity")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6599:30
	makeExtern< void * (*)(void *) , clang_IndexAction_create , SimNode_ExtFuncCall >(lib,"clang_IndexAction_create","clang_IndexAction_create")
		->args({"CIdx"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6607:21
	makeExtern< void (*)(void *) , clang_IndexAction_dispose , SimNode_ExtFuncCall >(lib,"clang_IndexAction_dispose","clang_IndexAction_dispose")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6673:20
	makeExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,CXTranslationUnitImpl **,unsigned int) , clang_indexSourceFile , SimNode_ExtFuncCall >(lib,"clang_indexSourceFile","clang_indexSourceFile")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","out_TU","TU_options"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6685:20
	makeExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,CXTranslationUnitImpl **,unsigned int) , clang_indexSourceFileFullArgv , SimNode_ExtFuncCall >(lib,"clang_indexSourceFileFullArgv","clang_indexSourceFileFullArgv")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","out_TU","TU_options"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6708:20
	makeExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,CXTranslationUnitImpl *) , clang_indexTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_indexTranslationUnit","clang_indexTranslationUnit")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6720:21
	makeExtern< void (*)(CXIdxLoc,void **,void **,unsigned int *,unsigned int *,unsigned int *) , clang_indexLoc_getFileLocation , SimNode_ExtFuncCall >(lib,"clang_indexLoc_getFileLocation","clang_indexLoc_getFileLocation")
		->args({"loc","indexFile","file","line","column","offset"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6730:18
	makeExtern< CXSourceLocation (*)(CXIdxLoc) , clang_indexLoc_getCXSourceLocation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_indexLoc_getCXSourceLocation","clang_indexLoc_getCXSourceLocation")
		->args({"loc"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:6892:1
	makeExtern< CXString (*)(CXBinaryOperatorKind) , clang_getBinaryOperatorKindSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getBinaryOperatorKindSpelling","clang_getBinaryOperatorKindSpelling")
		->args({"kind"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

