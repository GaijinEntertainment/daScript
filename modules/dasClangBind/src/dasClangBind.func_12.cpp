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
void Module_dasClangBind::initFunctions_12() {
// from clang-c/Index.h:4360:23
	makeExtern< CXType (*)(CXCursor) , clang_Cursor_getReceiverType , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getReceiverType","clang_Cursor_getReceiverType")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4390:1
	makeExtern< unsigned int (*)(CXCursor,unsigned int) , clang_Cursor_getObjCPropertyAttributes , SimNode_ExtFuncCall >(lib,"clang_Cursor_getObjCPropertyAttributes","clang_Cursor_getObjCPropertyAttributes")
		->args({"C","reserved"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4396:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getObjCPropertyGetterName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getObjCPropertyGetterName","clang_Cursor_getObjCPropertyGetterName")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4402:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getObjCPropertySetterName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getObjCPropertySetterName","clang_Cursor_getObjCPropertySetterName")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4424:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_getObjCDeclQualifiers , SimNode_ExtFuncCall >(lib,"clang_Cursor_getObjCDeclQualifiers","clang_Cursor_getObjCDeclQualifiers")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4431:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isObjCOptional , SimNode_ExtFuncCall >(lib,"clang_Cursor_isObjCOptional","clang_Cursor_isObjCOptional")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4436:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isVariadic , SimNode_ExtFuncCall >(lib,"clang_Cursor_isVariadic","clang_Cursor_isVariadic")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4451:25
	makeExtern< unsigned int (*)(CXCursor,CXString *,CXString *,unsigned int *) , clang_Cursor_isExternalSymbol , SimNode_ExtFuncCall >(lib,"clang_Cursor_isExternalSymbol","clang_Cursor_isExternalSymbol")
		->args({"C","language","definedIn","isGenerated"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4461:30
	makeExtern< CXSourceRange (*)(CXCursor) , clang_Cursor_getCommentRange , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getCommentRange","clang_Cursor_getCommentRange")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4467:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getRawCommentText , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getRawCommentText","clang_Cursor_getRawCommentText")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4474:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getBriefCommentText , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getBriefCommentText","clang_Cursor_getBriefCommentText")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4488:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getMangling , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getMangling","clang_Cursor_getMangling")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4494:29
	makeExtern< CXStringSet * (*)(CXCursor) , clang_Cursor_getCXXManglings , SimNode_ExtFuncCall >(lib,"clang_Cursor_getCXXManglings","clang_Cursor_getCXXManglings")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4500:29
	makeExtern< CXStringSet * (*)(CXCursor) , clang_Cursor_getObjCManglings , SimNode_ExtFuncCall >(lib,"clang_Cursor_getObjCManglings","clang_Cursor_getObjCManglings")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4533:25
	makeExtern< CXString (*)(CXCursor) , clang_Cursor_getGCCAssemblyTemplate , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getGCCAssemblyTemplate","clang_Cursor_getGCCAssemblyTemplate")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4542:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isGCCAssemblyHasGoto , SimNode_ExtFuncCall >(lib,"clang_Cursor_isGCCAssemblyHasGoto","clang_Cursor_isGCCAssemblyHasGoto")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4550:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_getGCCAssemblyNumOutputs , SimNode_ExtFuncCall >(lib,"clang_Cursor_getGCCAssemblyNumOutputs","clang_Cursor_getGCCAssemblyNumOutputs")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4558:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_getGCCAssemblyNumInputs , SimNode_ExtFuncCall >(lib,"clang_Cursor_getGCCAssemblyNumInputs","clang_Cursor_getGCCAssemblyNumInputs")
		->args({""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4573:25
	makeExtern< unsigned int (*)(CXCursor,unsigned int,CXString *,CXCursor *) , clang_Cursor_getGCCAssemblyInput , SimNode_ExtFuncCall >(lib,"clang_Cursor_getGCCAssemblyInput","clang_Cursor_getGCCAssemblyInput")
		->args({"Cursor","Index","Constraint","Expr"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4591:25
	makeExtern< unsigned int (*)(CXCursor,unsigned int,CXString *,CXCursor *) , clang_Cursor_getGCCAssemblyOutput , SimNode_ExtFuncCall >(lib,"clang_Cursor_getGCCAssemblyOutput","clang_Cursor_getGCCAssemblyOutput")
		->args({"Cursor","Index","Constraint","Expr"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

