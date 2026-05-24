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
void Module_dasClangBind::initFunctions_4() {
// from clang-c/Index.h:604:1
	makeExtern< CXString (*)(CXTranslationUnitImpl *) , clang_getTranslationUnitSpelling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getTranslationUnitSpelling","clang_getTranslationUnitSpelling")
		->args({"CTUnit"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:646:34
	makeExtern< CXTranslationUnitImpl * (*)(void *,const char *,int,const char *const *,unsigned int,CXUnsavedFile *) , clang_createTranslationUnitFromSourceFile , SimNode_ExtFuncCall >(lib,"clang_createTranslationUnitFromSourceFile","clang_createTranslationUnitFromSourceFile")
		->args({"CIdx","source_filename","num_clang_command_line_args","clang_command_line_args","num_unsaved_files","unsaved_files"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:658:1
	makeExtern< CXTranslationUnitImpl * (*)(void *,const char *) , clang_createTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_createTranslationUnit","clang_createTranslationUnit")
		->args({"CIdx","ast_filename"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:669:1
	makeExtern< CXErrorCode (*)(void *,const char *,CXTranslationUnitImpl **) , clang_createTranslationUnit2 , SimNode_ExtFuncCall >(lib,"clang_createTranslationUnit2","clang_createTranslationUnit2")
		->args({"CIdx","ast_filename","out_TU"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:840:25
	makeExtern< unsigned int (*)() , clang_defaultEditingTranslationUnitOptions , SimNode_ExtFuncCall >(lib,"clang_defaultEditingTranslationUnitOptions","clang_defaultEditingTranslationUnitOptions")
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:848:34
	makeExtern< CXTranslationUnitImpl * (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int) , clang_parseTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_parseTranslationUnit","clang_parseTranslationUnit")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:898:33
	makeExtern< CXErrorCode (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int,CXTranslationUnitImpl **) , clang_parseTranslationUnit2 , SimNode_ExtFuncCall >(lib,"clang_parseTranslationUnit2","clang_parseTranslationUnit2")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options","out_TU"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:909:33
	makeExtern< CXErrorCode (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int,CXTranslationUnitImpl **) , clang_parseTranslationUnit2FullArgv , SimNode_ExtFuncCall >(lib,"clang_parseTranslationUnit2FullArgv","clang_parseTranslationUnit2FullArgv")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options","out_TU"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:938:25
	makeExtern< unsigned int (*)(CXTranslationUnitImpl *) , clang_defaultSaveOptions , SimNode_ExtFuncCall >(lib,"clang_defaultSaveOptions","clang_defaultSaveOptions")
		->args({"TU"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:998:20
	makeExtern< int (*)(CXTranslationUnitImpl *,const char *,unsigned int) , clang_saveTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_saveTranslationUnit","clang_saveTranslationUnit")
		->args({"TU","FileName","options"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1009:25
	makeExtern< unsigned int (*)(CXTranslationUnitImpl *) , clang_suspendTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_suspendTranslationUnit","clang_suspendTranslationUnit")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1014:21
	makeExtern< void (*)(CXTranslationUnitImpl *) , clang_disposeTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_disposeTranslationUnit","clang_disposeTranslationUnit")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1040:25
	makeExtern< unsigned int (*)(CXTranslationUnitImpl *) , clang_defaultReparseOptions , SimNode_ExtFuncCall >(lib,"clang_defaultReparseOptions","clang_defaultReparseOptions")
		->args({"TU"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1082:1
	makeExtern< int (*)(CXTranslationUnitImpl *,unsigned int,CXUnsavedFile *,unsigned int) , clang_reparseTranslationUnit , SimNode_ExtFuncCall >(lib,"clang_reparseTranslationUnit","clang_reparseTranslationUnit")
		->args({"TU","num_unsaved_files","unsaved_files","options"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1117:13
	makeExtern< const char * (*)(CXTUResourceUsageKind) , clang_getTUResourceUsageName , SimNode_ExtFuncCall >(lib,"clang_getTUResourceUsageName","clang_getTUResourceUsageName")
		->args({"kind"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1148:1
	makeExtern< CXTUResourceUsage (*)(CXTranslationUnitImpl *) , clang_getCXTUResourceUsage , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_getCXTUResourceUsage","clang_getCXTUResourceUsage")
		->args({"TU"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1150:21
	makeExtern< void (*)(CXTUResourceUsage) , clang_disposeCXTUResourceUsage , SimNode_ExtFuncCall >(lib,"clang_disposeCXTUResourceUsage","clang_disposeCXTUResourceUsage")
		->args({"usage"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1158:1
	makeExtern< CXTargetInfoImpl * (*)(CXTranslationUnitImpl *) , clang_getTranslationUnitTargetInfo , SimNode_ExtFuncCall >(lib,"clang_getTranslationUnitTargetInfo","clang_getTranslationUnitTargetInfo")
		->args({"CTUnit"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1163:21
	makeExtern< void (*)(CXTargetInfoImpl *) , clang_TargetInfo_dispose , SimNode_ExtFuncCall >(lib,"clang_TargetInfo_dispose","clang_TargetInfo_dispose")
		->args({"Info"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:1170:25
	makeExtern< CXString (*)(CXTargetInfoImpl *) , clang_TargetInfo_getTriple , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_TargetInfo_getTriple","clang_TargetInfo_getTriple")
		->args({"Info"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

