// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasLLVM.h"
#include "need_dasLLVM.h"
namespace das {
#include "dasLLVM.func.aot.decl.inc"
void Module_dasLLVM::initFunctions_30() {
// from D:\Work\libclang\include\llvm-c/Core.h:3660:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,LLVMOpaqueBasicBlock *,LLVMOpaqueBasicBlock *,const char *) , LLVMBuildInvoke >(*this,lib,"LLVMBuildInvoke",SideEffects::worstDefault,"LLVMBuildInvoke")
		->args({"","Fn","Args","NumArgs","Then","Catch","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3664:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,LLVMOpaqueBasicBlock *,LLVMOpaqueBasicBlock *,const char *) , LLVMBuildInvoke2 >(*this,lib,"LLVMBuildInvoke2",SideEffects::worstDefault,"LLVMBuildInvoke2")
		->args({"","Ty","Fn","Args","NumArgs","Then","Catch","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3668:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *) , LLVMBuildUnreachable >(*this,lib,"LLVMBuildUnreachable",SideEffects::worstDefault,"LLVMBuildUnreachable")
		->args({""});
// from D:\Work\libclang\include\llvm-c/Core.h:3671:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *) , LLVMBuildResume >(*this,lib,"LLVMBuildResume",SideEffects::worstDefault,"LLVMBuildResume")
		->args({"B","Exn"});
// from D:\Work\libclang\include\llvm-c/Core.h:3672:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueType *,LLVMOpaqueValue *,unsigned int,const char *) , LLVMBuildLandingPad >(*this,lib,"LLVMBuildLandingPad",SideEffects::worstDefault,"LLVMBuildLandingPad")
		->args({"B","Ty","PersFn","NumClauses","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3675:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMBuildCleanupRet >(*this,lib,"LLVMBuildCleanupRet",SideEffects::worstDefault,"LLVMBuildCleanupRet")
		->args({"B","CatchPad","BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:3677:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMBuildCatchRet >(*this,lib,"LLVMBuildCatchRet",SideEffects::worstDefault,"LLVMBuildCatchRet")
		->args({"B","CatchPad","BB"});
// from D:\Work\libclang\include\llvm-c/Core.h:3679:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,const char *) , LLVMBuildCatchPad >(*this,lib,"LLVMBuildCatchPad",SideEffects::worstDefault,"LLVMBuildCatchPad")
		->args({"B","ParentPad","Args","NumArgs","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3682:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueValue **,unsigned int,const char *) , LLVMBuildCleanupPad >(*this,lib,"LLVMBuildCleanupPad",SideEffects::worstDefault,"LLVMBuildCleanupPad")
		->args({"B","ParentPad","Args","NumArgs","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3685:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueBuilder *,LLVMOpaqueValue *,LLVMOpaqueBasicBlock *,unsigned int,const char *) , LLVMBuildCatchSwitch >(*this,lib,"LLVMBuildCatchSwitch",SideEffects::worstDefault,"LLVMBuildCatchSwitch")
		->args({"B","ParentPad","UnwindBB","NumHandlers","Name"});
// from D:\Work\libclang\include\llvm-c/Core.h:3690:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMAddCase >(*this,lib,"LLVMAddCase",SideEffects::worstDefault,"LLVMAddCase")
		->args({"Switch","OnVal","Dest"});
// from D:\Work\libclang\include\llvm-c/Core.h:3694:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMAddDestination >(*this,lib,"LLVMAddDestination",SideEffects::worstDefault,"LLVMAddDestination")
		->args({"IndirectBr","Dest"});
// from D:\Work\libclang\include\llvm-c/Core.h:3697:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetNumClauses >(*this,lib,"LLVMGetNumClauses",SideEffects::worstDefault,"LLVMGetNumClauses")
		->args({"LandingPad"});
// from D:\Work\libclang\include\llvm-c/Core.h:3700:14
	addExtern< LLVMOpaqueValue * (*)(LLVMOpaqueValue *,unsigned int) , LLVMGetClause >(*this,lib,"LLVMGetClause",SideEffects::worstDefault,"LLVMGetClause")
		->args({"LandingPad","Idx"});
// from D:\Work\libclang\include\llvm-c/Core.h:3703:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueValue *) , LLVMAddClause >(*this,lib,"LLVMAddClause",SideEffects::worstDefault,"LLVMAddClause")
		->args({"LandingPad","ClauseVal"});
// from D:\Work\libclang\include\llvm-c/Core.h:3706:10
	addExtern< int (*)(LLVMOpaqueValue *) , LLVMIsCleanup >(*this,lib,"LLVMIsCleanup",SideEffects::worstDefault,"LLVMIsCleanup")
		->args({"LandingPad"});
// from D:\Work\libclang\include\llvm-c/Core.h:3709:6
	addExtern< void (*)(LLVMOpaqueValue *,int) , LLVMSetCleanup >(*this,lib,"LLVMSetCleanup",SideEffects::worstDefault,"LLVMSetCleanup")
		->args({"LandingPad","Val"});
// from D:\Work\libclang\include\llvm-c/Core.h:3712:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueBasicBlock *) , LLVMAddHandler >(*this,lib,"LLVMAddHandler",SideEffects::worstDefault,"LLVMAddHandler")
		->args({"CatchSwitch","Dest"});
// from D:\Work\libclang\include\llvm-c/Core.h:3715:10
	addExtern< unsigned int (*)(LLVMOpaqueValue *) , LLVMGetNumHandlers >(*this,lib,"LLVMGetNumHandlers",SideEffects::worstDefault,"LLVMGetNumHandlers")
		->args({"CatchSwitch"});
// from D:\Work\libclang\include\llvm-c/Core.h:3728:6
	addExtern< void (*)(LLVMOpaqueValue *,LLVMOpaqueBasicBlock **) , LLVMGetHandlers >(*this,lib,"LLVMGetHandlers",SideEffects::worstDefault,"LLVMGetHandlers")
		->args({"CatchSwitch","Handlers"});
}
}

