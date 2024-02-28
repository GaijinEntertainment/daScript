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
void Module_dasClangBind::initFunctions_10() {
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4106:30
	makeExtern< CXSourceRange (*)(CXCursor) , clang_Cursor_getCommentRange , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getCommentRange","clang_Cursor_getCommentRange")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4112:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getRawCommentText , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getRawCommentText","clang_Cursor_getRawCommentText")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4119:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getBriefCommentText , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getBriefCommentText","clang_Cursor_getBriefCommentText")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4133:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getMangling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getMangling","clang_Cursor_getMangling")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4139:29
	makeExtern< CXStringSet * (*)(CXCursor) , clang_Cursor_getCXXManglings , SimNode_ExtFuncCall >(lib,"clang_Cursor_getCXXManglings","clang_Cursor_getCXXManglings")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4145:29
	makeExtern< CXStringSet * (*)(CXCursor) , clang_Cursor_getObjCManglings , SimNode_ExtFuncCall >(lib,"clang_Cursor_getObjCManglings","clang_Cursor_getObjCManglings")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4164:25
	makeExtern< void * (*)(CXCursor) , clang_Cursor_getModule , SimNode_ExtFuncCall >(lib,"clang_Cursor_getModule","clang_Cursor_getModule")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4170:25
	makeExtern< void * (*)(CXTranslationUnitImpl *,void *) , clang_getModuleForFile , SimNode_ExtFuncCall >(lib,"clang_getModuleForFile","clang_getModuleForFile")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4177:23
	makeExtern< void * (*)(void *) , clang_Module_getASTFile , SimNode_ExtFuncCall >(lib,"clang_Module_getASTFile","clang_Module_getASTFile")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4185:25
	makeExtern< void * (*)(void *) , clang_Module_getParent , SimNode_ExtFuncCall >(lib,"clang_Module_getParent","clang_Module_getParent")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4193:25
	makeExtern< CXString (*)(void *) , clang_Module_getName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Module_getName","clang_Module_getName")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4200:25
	makeExtern< CXString (*)(void *) , clang_Module_getFullName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Module_getFullName","clang_Module_getFullName")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4207:20
	makeExtern< int (*)(void *) , clang_Module_isSystem , SimNode_ExtFuncCall >(lib,"clang_Module_isSystem","clang_Module_isSystem")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4214:25
	makeExtern< unsigned int (*)(CXTranslationUnitImpl *,void *) , clang_Module_getNumTopLevelHeaders , SimNode_ExtFuncCall >(lib,"clang_Module_getNumTopLevelHeaders","clang_Module_getNumTopLevelHeaders")
		->args({"","Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4225:8
	makeExtern< void * (*)(CXTranslationUnitImpl *,void *,unsigned int) , clang_Module_getTopLevelHeader , SimNode_ExtFuncCall >(lib,"clang_Module_getTopLevelHeader","clang_Module_getTopLevelHeader")
		->args({"","Module","Index"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4245:1
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXConstructor_isConvertingConstructor , SimNode_ExtFuncCall >(lib,"clang_CXXConstructor_isConvertingConstructor","clang_CXXConstructor_isConvertingConstructor")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4250:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXConstructor_isCopyConstructor , SimNode_ExtFuncCall >(lib,"clang_CXXConstructor_isCopyConstructor","clang_CXXConstructor_isCopyConstructor")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4255:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXConstructor_isDefaultConstructor , SimNode_ExtFuncCall >(lib,"clang_CXXConstructor_isDefaultConstructor","clang_CXXConstructor_isDefaultConstructor")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4260:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXConstructor_isMoveConstructor , SimNode_ExtFuncCall >(lib,"clang_CXXConstructor_isMoveConstructor","clang_CXXConstructor_isMoveConstructor")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4265:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXField_isMutable , SimNode_ExtFuncCall >(lib,"clang_CXXField_isMutable","clang_CXXField_isMutable")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

