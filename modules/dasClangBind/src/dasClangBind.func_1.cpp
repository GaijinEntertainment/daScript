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
// from D:\Work\libclang\include\clang-c/CXString.h:50:28
	addExtern< const char * (*)(CXString) , clang_getCString >(*this,lib,"clang_getCString",SideEffects::worstDefault,"clang_getCString")
		->args({"string"});
// from D:\Work\libclang\include\clang-c/CXString.h:55:21
	addExtern< void (*)(CXString) , clang_disposeString >(*this,lib,"clang_disposeString",SideEffects::worstDefault,"clang_disposeString")
		->args({"string"});
// from D:\Work\libclang\include\clang-c/CXString.h:60:21
	addExtern< void (*)(CXStringSet *) , clang_disposeStringSet >(*this,lib,"clang_disposeStringSet",SideEffects::worstDefault,"clang_disposeStringSet")
		->args({"set"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:266:24
	addExtern< void * (*)(int,int) , clang_createIndex >(*this,lib,"clang_createIndex",SideEffects::worstDefault,"clang_createIndex")
		->args({"excludeDeclarationsFromPCH","displayDiagnostics"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:275:21
	addExtern< void (*)(void *) , clang_disposeIndex >(*this,lib,"clang_disposeIndex",SideEffects::worstDefault,"clang_disposeIndex")
		->args({"index"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:324:21
	addExtern< void (*)(void *,unsigned int) , clang_CXIndex_setGlobalOptions >(*this,lib,"clang_CXIndex_setGlobalOptions",SideEffects::worstDefault,"clang_CXIndex_setGlobalOptions")
		->args({"","options"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:332:25
	addExtern< unsigned int (*)(void *) , clang_CXIndex_getGlobalOptions >(*this,lib,"clang_CXIndex_getGlobalOptions",SideEffects::worstDefault,"clang_CXIndex_getGlobalOptions")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:342:1
	addExtern< void (*)(void *,const char *) , clang_CXIndex_setInvocationEmissionPathOption >(*this,lib,"clang_CXIndex_setInvocationEmissionPathOption",SideEffects::worstDefault,"clang_CXIndex_setInvocationEmissionPathOption")
		->args({"","Path"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:358:25
	addExtern< CXString (*)(void *) , clang_getFileName ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getFileName",SideEffects::worstDefault,"clang_getFileName")
		->args({"SFile"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:363:23
	addExtern< time_t (*)(void *) , clang_getFileTime >(*this,lib,"clang_getFileTime",SideEffects::worstDefault,"clang_getFileTime")
		->args({"SFile"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:381:20
	addExtern< int (*)(void *,CXFileUniqueID *) , clang_getFileUniqueID >(*this,lib,"clang_getFileUniqueID",SideEffects::worstDefault,"clang_getFileUniqueID")
		->args({"file","outID"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:388:25
	addExtern< unsigned int (*)(CXTranslationUnitImpl *,void *) , clang_isFileMultipleIncludeGuarded >(*this,lib,"clang_isFileMultipleIncludeGuarded",SideEffects::worstDefault,"clang_isFileMultipleIncludeGuarded")
		->args({"tu","file"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:401:23
	addExtern< void * (*)(CXTranslationUnitImpl *,const char *) , clang_getFile >(*this,lib,"clang_getFile",SideEffects::worstDefault,"clang_getFile")
		->args({"tu","file_name"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:416:28
	addExtern< const char * (*)(CXTranslationUnitImpl *,void *,size_t *) , clang_getFileContents >(*this,lib,"clang_getFileContents",SideEffects::worstDefault,"clang_getFileContents")
		->args({"tu","file","size"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:423:20
	addExtern< int (*)(void *,void *) , clang_File_isEqual >(*this,lib,"clang_File_isEqual",SideEffects::worstDefault,"clang_File_isEqual")
		->args({"file1","file2"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:430:25
	addExtern< CXString (*)(void *) , clang_File_tryGetRealPathName ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_File_tryGetRealPathName",SideEffects::worstDefault,"clang_File_tryGetRealPathName")
		->args({"file"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:476:33
	addExtern< CXSourceLocation (*)() , clang_getNullLocation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getNullLocation",SideEffects::worstDefault,"clang_getNullLocation");
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:486:25
	addExtern< unsigned int (*)(CXSourceLocation,CXSourceLocation) , clang_equalLocations >(*this,lib,"clang_equalLocations",SideEffects::worstDefault,"clang_equalLocations")
		->args({"loc1","loc2"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:493:33
	addExtern< CXSourceLocation (*)(CXTranslationUnitImpl *,void *,unsigned int,unsigned int) , clang_getLocation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getLocation",SideEffects::worstDefault,"clang_getLocation")
		->args({"tu","file","line","column"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:500:33
	addExtern< CXSourceLocation (*)(CXTranslationUnitImpl *,void *,unsigned int) , clang_getLocationForOffset ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getLocationForOffset",SideEffects::worstDefault,"clang_getLocationForOffset")
		->args({"tu","file","offset"});
}
}

