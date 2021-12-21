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
void Module_dasClangBind::initFunctions_12() {
	addExtern<CXString (*)(CXCursor),clang_Cursor_getBriefCommentText,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getBriefCommentText",SideEffects::worstDefault,"clang_Cursor_getBriefCommentText")
		->args({"C"});
	addExtern<CXString (*)(CXCursor),clang_Cursor_getMangling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getMangling",SideEffects::worstDefault,"clang_Cursor_getMangling")
		->args({""});
	addExtern<CXStringSet * (*)(CXCursor),clang_Cursor_getCXXManglings>(*this,lib,"clang_Cursor_getCXXManglings",SideEffects::worstDefault,"clang_Cursor_getCXXManglings")
		->args({""});
	addExtern<CXStringSet * (*)(CXCursor),clang_Cursor_getObjCManglings>(*this,lib,"clang_Cursor_getObjCManglings",SideEffects::worstDefault,"clang_Cursor_getObjCManglings")
		->args({""});
	addExtern<void * (*)(CXCursor),clang_Cursor_getModule>(*this,lib,"clang_Cursor_getModule",SideEffects::worstDefault,"clang_Cursor_getModule")
		->args({"C"});
	addExtern<void * (*)(CXTranslationUnitImpl *,void *),clang_getModuleForFile>(*this,lib,"clang_getModuleForFile",SideEffects::worstDefault,"clang_getModuleForFile")
		->args({"",""});
	addExtern<void * (*)(void *),clang_Module_getASTFile>(*this,lib,"clang_Module_getASTFile",SideEffects::worstDefault,"clang_Module_getASTFile")
		->args({"Module"});
	addExtern<void * (*)(void *),clang_Module_getParent>(*this,lib,"clang_Module_getParent",SideEffects::worstDefault,"clang_Module_getParent")
		->args({"Module"});
	addExtern<CXString (*)(void *),clang_Module_getName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Module_getName",SideEffects::worstDefault,"clang_Module_getName")
		->args({"Module"});
	addExtern<CXString (*)(void *),clang_Module_getFullName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Module_getFullName",SideEffects::worstDefault,"clang_Module_getFullName")
		->args({"Module"});
	addExtern<int (*)(void *),clang_Module_isSystem>(*this,lib,"clang_Module_isSystem",SideEffects::worstDefault,"clang_Module_isSystem")
		->args({"Module"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *,void *),clang_Module_getNumTopLevelHeaders>(*this,lib,"clang_Module_getNumTopLevelHeaders",SideEffects::worstDefault,"clang_Module_getNumTopLevelHeaders")
		->args({"","Module"});
	addExtern<void * (*)(CXTranslationUnitImpl *,void *,unsigned int),clang_Module_getTopLevelHeader>(*this,lib,"clang_Module_getTopLevelHeader",SideEffects::worstDefault,"clang_Module_getTopLevelHeader")
		->args({"","Module","Index"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXConstructor_isConvertingConstructor>(*this,lib,"clang_CXXConstructor_isConvertingConstructor",SideEffects::worstDefault,"clang_CXXConstructor_isConvertingConstructor")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXConstructor_isCopyConstructor>(*this,lib,"clang_CXXConstructor_isCopyConstructor",SideEffects::worstDefault,"clang_CXXConstructor_isCopyConstructor")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXConstructor_isDefaultConstructor>(*this,lib,"clang_CXXConstructor_isDefaultConstructor",SideEffects::worstDefault,"clang_CXXConstructor_isDefaultConstructor")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXConstructor_isMoveConstructor>(*this,lib,"clang_CXXConstructor_isMoveConstructor",SideEffects::worstDefault,"clang_CXXConstructor_isMoveConstructor")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXField_isMutable>(*this,lib,"clang_CXXField_isMutable",SideEffects::worstDefault,"clang_CXXField_isMutable")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXMethod_isDefaulted>(*this,lib,"clang_CXXMethod_isDefaulted",SideEffects::worstDefault,"clang_CXXMethod_isDefaulted")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXMethod_isPureVirtual>(*this,lib,"clang_CXXMethod_isPureVirtual",SideEffects::worstDefault,"clang_CXXMethod_isPureVirtual")
		->args({"C"});
}
}

