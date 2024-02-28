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
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5594:25
	makeExtern< unsigned int (*)(void *) , clang_remap_getNumFiles , SimNode_ExtFuncCall >(lib,"clang_remap_getNumFiles","clang_remap_getNumFiles")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5604:21
	makeExtern< void (*)(void *,unsigned int,CXString *,CXString *) , clang_remap_getFilenames , SimNode_ExtFuncCall >(lib,"clang_remap_getFilenames","clang_remap_getFilenames")
		->args({"","index","original","transformed"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5611:21
	makeExtern< void (*)(void *) , clang_remap_dispose , SimNode_ExtFuncCall >(lib,"clang_remap_dispose","clang_remap_dispose")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5660:25
	makeExtern< CXResult (*)(CXCursor,void *,CXCursorAndRangeVisitor) , clang_findReferencesInFile , SimNode_ExtFuncCall >(lib,"clang_findReferencesInFile","clang_findReferencesInFile")
		->args({"cursor","file","visitor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5675:25
	makeExtern< CXResult (*)(CXTranslationUnitImpl *,void *,CXCursorAndRangeVisitor) , clang_findIncludesInFile , SimNode_ExtFuncCall >(lib,"clang_findIncludesInFile","clang_findIncludesInFile")
		->args({"TU","file","visitor"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6077:20
	makeExtern< int (*)(CXIdxEntityKind) , clang_index_isEntityObjCContainerKind , SimNode_ExtFuncCall >(lib,"clang_index_isEntityObjCContainerKind","clang_index_isEntityObjCContainerKind")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6079:1
	makeExtern< const CXIdxObjCContainerDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCContainerDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCContainerDeclInfo","clang_index_getObjCContainerDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6082:1
	makeExtern< const CXIdxObjCInterfaceDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCInterfaceDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCInterfaceDeclInfo","clang_index_getObjCInterfaceDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6086:1
	makeExtern< const CXIdxObjCCategoryDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCCategoryDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCCategoryDeclInfo","clang_index_getObjCCategoryDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6089:1
	makeExtern< const CXIdxObjCProtocolRefListInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCProtocolRefListInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCProtocolRefListInfo","clang_index_getObjCProtocolRefListInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6092:1
	makeExtern< const CXIdxObjCPropertyDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getObjCPropertyDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getObjCPropertyDeclInfo","clang_index_getObjCPropertyDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6095:1
	makeExtern< const CXIdxIBOutletCollectionAttrInfo * (*)(const CXIdxAttrInfo *) , clang_index_getIBOutletCollectionAttrInfo , SimNode_ExtFuncCall >(lib,"clang_index_getIBOutletCollectionAttrInfo","clang_index_getIBOutletCollectionAttrInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6098:1
	makeExtern< const CXIdxCXXClassDeclInfo * (*)(const CXIdxDeclInfo *) , clang_index_getCXXClassDeclInfo , SimNode_ExtFuncCall >(lib,"clang_index_getCXXClassDeclInfo","clang_index_getCXXClassDeclInfo")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6105:1
	makeExtern< void * (*)(const CXIdxContainerInfo *) , clang_index_getClientContainer , SimNode_ExtFuncCall >(lib,"clang_index_getClientContainer","clang_index_getClientContainer")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6111:21
	makeExtern< void (*)(const CXIdxContainerInfo *,void *) , clang_index_setClientContainer , SimNode_ExtFuncCall >(lib,"clang_index_setClientContainer","clang_index_setClientContainer")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6118:1
	makeExtern< void * (*)(const CXIdxEntityInfo *) , clang_index_getClientEntity , SimNode_ExtFuncCall >(lib,"clang_index_getClientEntity","clang_index_getClientEntity")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6123:21
	makeExtern< void (*)(const CXIdxEntityInfo *,void *) , clang_index_setClientEntity , SimNode_ExtFuncCall >(lib,"clang_index_setClientEntity","clang_index_setClientEntity")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6138:30
	makeExtern< void * (*)(void *) , clang_IndexAction_create , SimNode_ExtFuncCall >(lib,"clang_IndexAction_create","clang_IndexAction_create")
		->args({"CIdx"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6146:21
	makeExtern< void (*)(void *) , clang_IndexAction_dispose , SimNode_ExtFuncCall >(lib,"clang_IndexAction_dispose","clang_IndexAction_dispose")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6212:20
	makeExtern< int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,CXTranslationUnitImpl **,unsigned int) , clang_indexSourceFile , SimNode_ExtFuncCall >(lib,"clang_indexSourceFile","clang_indexSourceFile")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","out_TU","TU_options"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

