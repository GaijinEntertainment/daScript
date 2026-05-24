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
void Module_dasClangBind::initFunctions_13() {
// from clang-c/Index.h:4602:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_getGCCAssemblyNumClobbers , SimNode_ExtFuncCall >(lib,"clang_Cursor_getGCCAssemblyNumClobbers","clang_Cursor_getGCCAssemblyNumClobbers")
		->args({"Cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4613:25
	makeExtern< CXString (*)(CXCursor,unsigned int) , clang_Cursor_getGCCAssemblyClobber , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Cursor_getGCCAssemblyClobber","clang_Cursor_getGCCAssemblyClobber")
		->args({"Cursor","Index"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4623:25
	makeExtern< unsigned int (*)(CXCursor) , clang_Cursor_isGCCAssemblyVolatile , SimNode_ExtFuncCall >(lib,"clang_Cursor_isGCCAssemblyVolatile","clang_Cursor_isGCCAssemblyVolatile")
		->args({"Cursor"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4642:25
	makeExtern< void * (*)(CXCursor) , clang_Cursor_getModule , SimNode_ExtFuncCall >(lib,"clang_Cursor_getModule","clang_Cursor_getModule")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4648:25
	makeExtern< void * (*)(CXTranslationUnitImpl *,void *) , clang_getModuleForFile , SimNode_ExtFuncCall >(lib,"clang_getModuleForFile","clang_getModuleForFile")
		->args({"",""})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4655:23
	makeExtern< void * (*)(void *) , clang_Module_getASTFile , SimNode_ExtFuncCall >(lib,"clang_Module_getASTFile","clang_Module_getASTFile")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4663:25
	makeExtern< void * (*)(void *) , clang_Module_getParent , SimNode_ExtFuncCall >(lib,"clang_Module_getParent","clang_Module_getParent")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4671:25
	makeExtern< CXString (*)(void *) , clang_Module_getName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Module_getName","clang_Module_getName")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4678:25
	makeExtern< CXString (*)(void *) , clang_Module_getFullName , SimNode_ExtFuncCallAndCopyOrMove >(lib,"clang_Module_getFullName","clang_Module_getFullName")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4685:20
	makeExtern< int (*)(void *) , clang_Module_isSystem , SimNode_ExtFuncCall >(lib,"clang_Module_isSystem","clang_Module_isSystem")
		->args({"Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4692:25
	makeExtern< unsigned int (*)(CXTranslationUnitImpl *,void *) , clang_Module_getNumTopLevelHeaders , SimNode_ExtFuncCall >(lib,"clang_Module_getNumTopLevelHeaders","clang_Module_getNumTopLevelHeaders")
		->args({"","Module"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4703:8
	makeExtern< void * (*)(CXTranslationUnitImpl *,void *,unsigned int) , clang_Module_getTopLevelHeader , SimNode_ExtFuncCall >(lib,"clang_Module_getTopLevelHeader","clang_Module_getTopLevelHeader")
		->args({"","Module","Index"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4723:1
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXConstructor_isConvertingConstructor , SimNode_ExtFuncCall >(lib,"clang_CXXConstructor_isConvertingConstructor","clang_CXXConstructor_isConvertingConstructor")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4728:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXConstructor_isCopyConstructor , SimNode_ExtFuncCall >(lib,"clang_CXXConstructor_isCopyConstructor","clang_CXXConstructor_isCopyConstructor")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4733:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXConstructor_isDefaultConstructor , SimNode_ExtFuncCall >(lib,"clang_CXXConstructor_isDefaultConstructor","clang_CXXConstructor_isDefaultConstructor")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4738:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXConstructor_isMoveConstructor , SimNode_ExtFuncCall >(lib,"clang_CXXConstructor_isMoveConstructor","clang_CXXConstructor_isMoveConstructor")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4743:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXField_isMutable , SimNode_ExtFuncCall >(lib,"clang_CXXField_isMutable","clang_CXXField_isMutable")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4748:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isDefaulted , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isDefaulted","clang_CXXMethod_isDefaulted")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4753:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isDeleted , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isDeleted","clang_CXXMethod_isDeleted")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
// from clang-c/Index.h:4759:25
	makeExtern< unsigned int (*)(CXCursor) , clang_CXXMethod_isPureVirtual , SimNode_ExtFuncCall >(lib,"clang_CXXMethod_isPureVirtual","clang_CXXMethod_isPureVirtual")
		->args({"C"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

