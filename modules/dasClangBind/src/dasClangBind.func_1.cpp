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
void Module_dasClangBind::initFunctions_1() {
// from clang-c/CXString.h:54:28
	makeExtern< const char * (*)(CXString) , clang_getCString , SimNode_ExtFuncCall >(lib,"clang_getCString","clang_getCString")
		->args({"string"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXString.h:59:21
	makeExtern< void (*)(CXString) , clang_disposeString , SimNode_ExtFuncCall >(lib,"clang_disposeString","clang_disposeString")
		->args({"string"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXString.h:64:21
	makeExtern< void (*)(CXStringSet *) , clang_disposeStringSet , SimNode_ExtFuncCall >(lib,"clang_disposeStringSet","clang_disposeStringSet")
		->args({"set"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXFile.h:39:25
	makeExtern< CXString (*)(void *) , clang_getFileName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getFileName","clang_getFileName")
		->args({"SFile"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXFile.h:44:23
	makeExtern< time_t (*)(void *) , clang_getFileTime , SimNode_ExtFuncCall >(lib,"clang_getFileTime","clang_getFileTime")
		->args({"SFile"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXFile.h:62:20
	makeExtern< int (*)(void *,CXFileUniqueID *) , clang_getFileUniqueID , SimNode_ExtFuncCall >(lib,"clang_getFileUniqueID","clang_getFileUniqueID")
		->args({"file","outID"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXFile.h:68:20
	makeExtern< int (*)(void *,void *) , clang_File_isEqual , SimNode_ExtFuncCall >(lib,"clang_File_isEqual","clang_File_isEqual")
		->args({"file1","file2"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXFile.h:75:25
	makeExtern< CXString (*)(void *) , clang_File_tryGetRealPathName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_File_tryGetRealPathName","clang_File_tryGetRealPathName")
		->args({"file"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:64:33
	makeExtern< CXSourceLocation (*)() , clang_getNullLocation , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getNullLocation","clang_getNullLocation")
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:74:25
	makeExtern< unsigned int (*)(CXSourceLocation,CXSourceLocation) , clang_equalLocations , SimNode_ExtFuncCall >(lib,"clang_equalLocations","clang_equalLocations")
		->args({"loc1","loc2"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:84:25
	makeExtern< unsigned int (*)(CXSourceLocation,CXSourceLocation) , clang_isBeforeInTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_isBeforeInTranslationUnit","clang_isBeforeInTranslationUnit")
		->args({"loc1","loc2"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:90:20
	makeExtern< int (*)(CXSourceLocation) , clang_Location_isInSystemHeader , SimNode_ExtFuncCall >(lib,"clang_Location_isInSystemHeader","clang_Location_isInSystemHeader")
		->args({"location"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:96:20
	makeExtern< int (*)(CXSourceLocation) , clang_Location_isFromMainFile , SimNode_ExtFuncCall >(lib,"clang_Location_isFromMainFile","clang_Location_isFromMainFile")
		->args({"location"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:101:30
	makeExtern< CXSourceRange (*)() , clang_getNullRange , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getNullRange","clang_getNullRange")
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:107:30
	makeExtern< CXSourceRange (*)(CXSourceLocation,CXSourceLocation) , clang_getRange , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getRange","clang_getRange")
		->args({"begin","end"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:115:25
	makeExtern< unsigned int (*)(CXSourceRange,CXSourceRange) , clang_equalRanges , SimNode_ExtFuncCall >(lib,"clang_equalRanges","clang_equalRanges")
		->args({"range1","range2"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:121:20
	makeExtern< int (*)(CXSourceRange) , clang_Range_isNull , SimNode_ExtFuncCall >(lib,"clang_Range_isNull","clang_Range_isNull")
		->args({"range"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:145:21
	makeExtern< void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *) , clang_getExpansionLocation , SimNode_ExtFuncCall >(lib,"clang_getExpansionLocation","clang_getExpansionLocation")
		->args({"location","file","line","column","offset"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:190:21
	makeExtern< void (*)(CXSourceLocation,CXString *,unsigned int *,unsigned int *) , clang_getPresumedLocation , SimNode_ExtFuncCall >(lib,"clang_getPresumedLocation","clang_getPresumedLocation")
		->args({"location","filename","line","column"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/CXSourceLocation.h:202:21
	makeExtern< void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *) , clang_getInstantiationLocation , SimNode_ExtFuncCall >(lib,"clang_getInstantiationLocation","clang_getInstantiationLocation")
		->args({"location","file","line","column","offset"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

