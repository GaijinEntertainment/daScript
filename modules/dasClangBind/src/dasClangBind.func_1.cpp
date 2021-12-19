#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasClangBind.h"
#include "needClangBind.h"
namespace das {
void Module_ClangBind::initFunctions_1( ModuleLibrary & lib ) {
	addExtern<const char * (*)(CXString),clang_getCString>(*this,lib,"clang_getCString",SideEffects::worstDefault,"clang_getCString")
		->args({"string"});
	addExtern<void (*)(CXString),clang_disposeString>(*this,lib,"clang_disposeString",SideEffects::worstDefault,"clang_disposeString")
		->args({"string"});
	addExtern<void (*)(CXStringSet *),clang_disposeStringSet>(*this,lib,"clang_disposeStringSet",SideEffects::worstDefault,"clang_disposeStringSet")
		->args({"set"});
	addExtern<void * (*)(int,int),clang_createIndex>(*this,lib,"clang_createIndex",SideEffects::worstDefault,"clang_createIndex")
		->args({"excludeDeclarationsFromPCH","displayDiagnostics"});
	addExtern<void (*)(void *),clang_disposeIndex>(*this,lib,"clang_disposeIndex",SideEffects::worstDefault,"clang_disposeIndex")
		->args({"index"});
	addExtern<void (*)(void *,unsigned int),clang_CXIndex_setGlobalOptions>(*this,lib,"clang_CXIndex_setGlobalOptions",SideEffects::worstDefault,"clang_CXIndex_setGlobalOptions")
		->args({"","options"});
	addExtern<unsigned int (*)(void *),clang_CXIndex_getGlobalOptions>(*this,lib,"clang_CXIndex_getGlobalOptions",SideEffects::worstDefault,"clang_CXIndex_getGlobalOptions")
		->args({""});
	addExtern<void (*)(void *,const char *),clang_CXIndex_setInvocationEmissionPathOption>(*this,lib,"clang_CXIndex_setInvocationEmissionPathOption",SideEffects::worstDefault,"clang_CXIndex_setInvocationEmissionPathOption")
		->args({"","Path"});
	addExtern<CXString (*)(void *),clang_getFileName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getFileName",SideEffects::worstDefault,"clang_getFileName")
		->args({"SFile"});
	addExtern<long long (*)(void *),clang_getFileTime>(*this,lib,"clang_getFileTime",SideEffects::worstDefault,"clang_getFileTime")
		->args({"SFile"});
	addExtern<int (*)(void *,CXFileUniqueID *),clang_getFileUniqueID>(*this,lib,"clang_getFileUniqueID",SideEffects::worstDefault,"clang_getFileUniqueID")
		->args({"file","outID"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *,void *),clang_isFileMultipleIncludeGuarded>(*this,lib,"clang_isFileMultipleIncludeGuarded",SideEffects::worstDefault,"clang_isFileMultipleIncludeGuarded")
		->args({"tu","file"});
	addExtern<void * (*)(CXTranslationUnitImpl *,const char *),clang_getFile>(*this,lib,"clang_getFile",SideEffects::worstDefault,"clang_getFile")
		->args({"tu","file_name"});
	addExtern<const char * (*)(CXTranslationUnitImpl *,void *,unsigned long long *),clang_getFileContents>(*this,lib,"clang_getFileContents",SideEffects::worstDefault,"clang_getFileContents")
		->args({"tu","file","size"});
	addExtern<int (*)(void *,void *),clang_File_isEqual>(*this,lib,"clang_File_isEqual",SideEffects::worstDefault,"clang_File_isEqual")
		->args({"file1","file2"});
	addExtern<CXString (*)(void *),clang_File_tryGetRealPathName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_File_tryGetRealPathName",SideEffects::worstDefault,"clang_File_tryGetRealPathName")
		->args({"file"});
	addExtern<CXSourceLocation (*)(),clang_getNullLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getNullLocation",SideEffects::worstDefault,"clang_getNullLocation");
	addExtern<unsigned int (*)(CXSourceLocation,CXSourceLocation),clang_equalLocations>(*this,lib,"clang_equalLocations",SideEffects::worstDefault,"clang_equalLocations")
		->args({"loc1","loc2"});
	addExtern<CXSourceLocation (*)(CXTranslationUnitImpl *,void *,unsigned int,unsigned int),clang_getLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getLocation",SideEffects::worstDefault,"clang_getLocation")
		->args({"tu","file","line","column"});
	addExtern<CXSourceLocation (*)(CXTranslationUnitImpl *,void *,unsigned int),clang_getLocationForOffset,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getLocationForOffset",SideEffects::worstDefault,"clang_getLocationForOffset")
		->args({"tu","file","offset"});
}
}

