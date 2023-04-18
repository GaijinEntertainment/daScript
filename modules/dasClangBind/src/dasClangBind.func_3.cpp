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
void Module_dasClangBind::initFunctions_3() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:855:32
	addExtern< void * (*)(void *) , clang_getChildDiagnostics >(*this,lib,"clang_getChildDiagnostics",SideEffects::worstDefault,"clang_getChildDiagnostics")
		->args({"D"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:861:25
	addExtern< unsigned int (*)(CXTranslationUnitImpl *) , clang_getNumDiagnostics >(*this,lib,"clang_getNumDiagnostics",SideEffects::worstDefault,"clang_getNumDiagnostics")
		->args({"Unit"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:872:29
	addExtern< void * (*)(CXTranslationUnitImpl *,unsigned int) , clang_getDiagnostic >(*this,lib,"clang_getDiagnostic",SideEffects::worstDefault,"clang_getDiagnostic")
		->args({"Unit","Index"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:882:1
	addExtern< void * (*)(CXTranslationUnitImpl *) , clang_getDiagnosticSetFromTU >(*this,lib,"clang_getDiagnosticSetFromTU",SideEffects::worstDefault,"clang_getDiagnosticSetFromTU")
		->args({"Unit"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:887:21
	addExtern< void (*)(void *) , clang_disposeDiagnostic >(*this,lib,"clang_disposeDiagnostic",SideEffects::worstDefault,"clang_disposeDiagnostic")
		->args({"Diagnostic"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:972:25
	addExtern< CXString (*)(void *,unsigned int) , clang_formatDiagnostic ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_formatDiagnostic",SideEffects::worstDefault,"clang_formatDiagnostic")
		->args({"Diagnostic","Options"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:982:25
	addExtern< unsigned int (*)() , clang_defaultDiagnosticDisplayOptions >(*this,lib,"clang_defaultDiagnosticDisplayOptions",SideEffects::worstDefault,"clang_defaultDiagnosticDisplayOptions");
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:988:5
	addExtern< CXDiagnosticSeverity (*)(void *) , clang_getDiagnosticSeverity >(*this,lib,"clang_getDiagnosticSeverity",SideEffects::worstDefault,"clang_getDiagnosticSeverity")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:996:33
	addExtern< CXSourceLocation (*)(void *) , clang_getDiagnosticLocation ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticLocation",SideEffects::worstDefault,"clang_getDiagnosticLocation")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1001:25
	addExtern< CXString (*)(void *) , clang_getDiagnosticSpelling ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticSpelling",SideEffects::worstDefault,"clang_getDiagnosticSpelling")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1015:25
	addExtern< CXString (*)(void *,CXString *) , clang_getDiagnosticOption ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticOption",SideEffects::worstDefault,"clang_getDiagnosticOption")
		->args({"Diag","Disable"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1028:25
	addExtern< unsigned int (*)(void *) , clang_getDiagnosticCategory >(*this,lib,"clang_getDiagnosticCategory",SideEffects::worstDefault,"clang_getDiagnosticCategory")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1041:1
	addExtern< CXString (*)(unsigned int) , clang_getDiagnosticCategoryName ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticCategoryName",SideEffects::worstDefault,"clang_getDiagnosticCategoryName")
		->args({"Category"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1048:25
	addExtern< CXString (*)(void *) , clang_getDiagnosticCategoryText ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticCategoryText",SideEffects::worstDefault,"clang_getDiagnosticCategoryText")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1054:25
	addExtern< unsigned int (*)(void *) , clang_getDiagnosticNumRanges >(*this,lib,"clang_getDiagnosticNumRanges",SideEffects::worstDefault,"clang_getDiagnosticNumRanges")
		->args({""});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1069:30
	addExtern< CXSourceRange (*)(void *,unsigned int) , clang_getDiagnosticRange ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticRange",SideEffects::worstDefault,"clang_getDiagnosticRange")
		->args({"Diagnostic","Range"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1076:25
	addExtern< unsigned int (*)(void *) , clang_getDiagnosticNumFixIts >(*this,lib,"clang_getDiagnosticNumFixIts",SideEffects::worstDefault,"clang_getDiagnosticNumFixIts")
		->args({"Diagnostic"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1103:25
	addExtern< CXString (*)(void *,unsigned int,CXSourceRange *) , clang_getDiagnosticFixIt ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticFixIt",SideEffects::worstDefault,"clang_getDiagnosticFixIt")
		->args({"Diagnostic","FixIt","ReplacementRange"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1124:1
	addExtern< CXString (*)(CXTranslationUnitImpl *) , clang_getTranslationUnitSpelling ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTranslationUnitSpelling",SideEffects::worstDefault,"clang_getTranslationUnitSpelling")
		->args({"CTUnit"});
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1166:34
	addExtern< CXTranslationUnitImpl * (*)(void *,const char *,int,const char *const *,unsigned int,CXUnsavedFile *) , clang_createTranslationUnitFromSourceFile >(*this,lib,"clang_createTranslationUnitFromSourceFile",SideEffects::worstDefault,"clang_createTranslationUnitFromSourceFile")
		->args({"CIdx","source_filename","num_clang_command_line_args","clang_command_line_args","num_unsaved_files","unsaved_files"});
}
}

