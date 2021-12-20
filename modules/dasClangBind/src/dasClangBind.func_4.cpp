// this file is generated via dasClangBind
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
void Module_dasClangBind::initFunctions_4( ModuleLibrary & lib ) {
	addExtern<CXTranslationUnitImpl * (*)(void *,const char *),clang_createTranslationUnit>(*this,lib,"clang_createTranslationUnit",SideEffects::worstDefault,"clang_createTranslationUnit")
		->args({"CIdx","ast_filename"});
	addExtern<CXErrorCode (*)(void *,const char *,CXTranslationUnitImpl **),clang_createTranslationUnit2>(*this,lib,"clang_createTranslationUnit2",SideEffects::worstDefault,"clang_createTranslationUnit2")
		->args({"CIdx","ast_filename","out_TU"});
	addExtern<unsigned int (*)(),clang_defaultEditingTranslationUnitOptions>(*this,lib,"clang_defaultEditingTranslationUnitOptions",SideEffects::worstDefault,"clang_defaultEditingTranslationUnitOptions");
	addExtern<CXTranslationUnitImpl * (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int),clang_parseTranslationUnit>(*this,lib,"clang_parseTranslationUnit",SideEffects::worstDefault,"clang_parseTranslationUnit")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options"});
	addExtern<CXErrorCode (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int,CXTranslationUnitImpl **),clang_parseTranslationUnit2>(*this,lib,"clang_parseTranslationUnit2",SideEffects::worstDefault,"clang_parseTranslationUnit2")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options","out_TU"});
	addExtern<CXErrorCode (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int,CXTranslationUnitImpl **),clang_parseTranslationUnit2FullArgv>(*this,lib,"clang_parseTranslationUnit2FullArgv",SideEffects::worstDefault,"clang_parseTranslationUnit2FullArgv")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options","out_TU"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *),clang_defaultSaveOptions>(*this,lib,"clang_defaultSaveOptions",SideEffects::worstDefault,"clang_defaultSaveOptions")
		->args({"TU"});
	addExtern<int (*)(CXTranslationUnitImpl *,const char *,unsigned int),clang_saveTranslationUnit>(*this,lib,"clang_saveTranslationUnit",SideEffects::worstDefault,"clang_saveTranslationUnit")
		->args({"TU","FileName","options"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *),clang_suspendTranslationUnit>(*this,lib,"clang_suspendTranslationUnit",SideEffects::worstDefault,"clang_suspendTranslationUnit")
		->args({""});
	addExtern<void (*)(CXTranslationUnitImpl *),clang_disposeTranslationUnit>(*this,lib,"clang_disposeTranslationUnit",SideEffects::worstDefault,"clang_disposeTranslationUnit")
		->args({""});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *),clang_defaultReparseOptions>(*this,lib,"clang_defaultReparseOptions",SideEffects::worstDefault,"clang_defaultReparseOptions")
		->args({"TU"});
	addExtern<int (*)(CXTranslationUnitImpl *,unsigned int,CXUnsavedFile *,unsigned int),clang_reparseTranslationUnit>(*this,lib,"clang_reparseTranslationUnit",SideEffects::worstDefault,"clang_reparseTranslationUnit")
		->args({"TU","num_unsaved_files","unsaved_files","options"});
	addExtern<const char * (*)(CXTUResourceUsageKind),clang_getTUResourceUsageName>(*this,lib,"clang_getTUResourceUsageName",SideEffects::worstDefault,"clang_getTUResourceUsageName")
		->args({"kind"});
	addExtern<CXTUResourceUsage (*)(CXTranslationUnitImpl *),clang_getCXTUResourceUsage,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCXTUResourceUsage",SideEffects::worstDefault,"clang_getCXTUResourceUsage")
		->args({"TU"});
	addExtern<void (*)(CXTUResourceUsage),clang_disposeCXTUResourceUsage>(*this,lib,"clang_disposeCXTUResourceUsage",SideEffects::worstDefault,"clang_disposeCXTUResourceUsage")
		->args({"usage"});
	addExtern<CXTargetInfoImpl * (*)(CXTranslationUnitImpl *),clang_getTranslationUnitTargetInfo>(*this,lib,"clang_getTranslationUnitTargetInfo",SideEffects::worstDefault,"clang_getTranslationUnitTargetInfo")
		->args({"CTUnit"});
	addExtern<void (*)(CXTargetInfoImpl *),clang_TargetInfo_dispose>(*this,lib,"clang_TargetInfo_dispose",SideEffects::worstDefault,"clang_TargetInfo_dispose")
		->args({"Info"});
	addExtern<CXString (*)(CXTargetInfoImpl *),clang_TargetInfo_getTriple,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_TargetInfo_getTriple",SideEffects::worstDefault,"clang_TargetInfo_getTriple")
		->args({"Info"});
	addExtern<int (*)(CXTargetInfoImpl *),clang_TargetInfo_getPointerWidth>(*this,lib,"clang_TargetInfo_getPointerWidth",SideEffects::worstDefault,"clang_TargetInfo_getPointerWidth")
		->args({"Info"});
	addExtern<CXCursor (*)(),clang_getNullCursor,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getNullCursor",SideEffects::worstDefault,"clang_getNullCursor");
}
}

