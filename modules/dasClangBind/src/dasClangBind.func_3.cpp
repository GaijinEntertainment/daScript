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
void Module_dasClangBind::initFunctions_3() {
	addExtern< void * (*)(void *) , clang_getChildDiagnostics >(*this,lib,"clang_getChildDiagnostics",SideEffects::worstDefault,"clang_getChildDiagnostics")
		->args({"D"});
	addExtern< unsigned int (*)(CXTranslationUnitImpl *) , clang_getNumDiagnostics >(*this,lib,"clang_getNumDiagnostics",SideEffects::worstDefault,"clang_getNumDiagnostics")
		->args({"Unit"});
	addExtern< void * (*)(CXTranslationUnitImpl *,unsigned int) , clang_getDiagnostic >(*this,lib,"clang_getDiagnostic",SideEffects::worstDefault,"clang_getDiagnostic")
		->args({"Unit","Index"});
	addExtern< void * (*)(CXTranslationUnitImpl *) , clang_getDiagnosticSetFromTU >(*this,lib,"clang_getDiagnosticSetFromTU",SideEffects::worstDefault,"clang_getDiagnosticSetFromTU")
		->args({"Unit"});
	addExtern< void (*)(void *) , clang_disposeDiagnostic >(*this,lib,"clang_disposeDiagnostic",SideEffects::worstDefault,"clang_disposeDiagnostic")
		->args({"Diagnostic"});
	addExtern< CXString (*)(void *,unsigned int) , clang_formatDiagnostic ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_formatDiagnostic",SideEffects::worstDefault,"clang_formatDiagnostic")
		->args({"Diagnostic","Options"});
	addExtern< unsigned int (*)() , clang_defaultDiagnosticDisplayOptions >(*this,lib,"clang_defaultDiagnosticDisplayOptions",SideEffects::worstDefault,"clang_defaultDiagnosticDisplayOptions");
	addExtern< CXDiagnosticSeverity (*)(void *) , clang_getDiagnosticSeverity >(*this,lib,"clang_getDiagnosticSeverity",SideEffects::worstDefault,"clang_getDiagnosticSeverity")
		->args({""});
	addExtern< CXSourceLocation (*)(void *) , clang_getDiagnosticLocation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticLocation",SideEffects::worstDefault,"clang_getDiagnosticLocation")
		->args({""});
	addExtern< CXString (*)(void *) , clang_getDiagnosticSpelling ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticSpelling",SideEffects::worstDefault,"clang_getDiagnosticSpelling")
		->args({""});
	addExtern< CXString (*)(void *,CXString *) , clang_getDiagnosticOption ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticOption",SideEffects::worstDefault,"clang_getDiagnosticOption")
		->args({"Diag","Disable"});
	addExtern< unsigned int (*)(void *) , clang_getDiagnosticCategory >(*this,lib,"clang_getDiagnosticCategory",SideEffects::worstDefault,"clang_getDiagnosticCategory")
		->args({""});
	addExtern< CXString (*)(unsigned int) , clang_getDiagnosticCategoryName ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticCategoryName",SideEffects::worstDefault,"clang_getDiagnosticCategoryName")
		->args({"Category"});
	addExtern< CXString (*)(void *) , clang_getDiagnosticCategoryText ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticCategoryText",SideEffects::worstDefault,"clang_getDiagnosticCategoryText")
		->args({""});
	addExtern< unsigned int (*)(void *) , clang_getDiagnosticNumRanges >(*this,lib,"clang_getDiagnosticNumRanges",SideEffects::worstDefault,"clang_getDiagnosticNumRanges")
		->args({""});
	addExtern< CXSourceRange (*)(void *,unsigned int) , clang_getDiagnosticRange ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticRange",SideEffects::worstDefault,"clang_getDiagnosticRange")
		->args({"Diagnostic","Range"});
	addExtern< unsigned int (*)(void *) , clang_getDiagnosticNumFixIts >(*this,lib,"clang_getDiagnosticNumFixIts",SideEffects::worstDefault,"clang_getDiagnosticNumFixIts")
		->args({"Diagnostic"});
	addExtern< CXString (*)(void *,unsigned int,CXSourceRange *) , clang_getDiagnosticFixIt ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticFixIt",SideEffects::worstDefault,"clang_getDiagnosticFixIt")
		->args({"Diagnostic","FixIt","ReplacementRange"});
	addExtern< CXString (*)(CXTranslationUnitImpl *) , clang_getTranslationUnitSpelling ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTranslationUnitSpelling",SideEffects::worstDefault,"clang_getTranslationUnitSpelling")
		->args({"CTUnit"});
	addExtern< CXTranslationUnitImpl * (*)(void *,const char *,int,const char *const *,unsigned int,CXUnsavedFile *) , clang_createTranslationUnitFromSourceFile >(*this,lib,"clang_createTranslationUnitFromSourceFile",SideEffects::worstDefault,"clang_createTranslationUnitFromSourceFile")
		->args({"CIdx","source_filename","num_clang_command_line_args","clang_command_line_args","num_unsaved_files","unsaved_files"});
}
}

