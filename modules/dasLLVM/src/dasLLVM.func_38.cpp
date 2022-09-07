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
void Module_dasLLVM::initFunctions_38() {
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:261:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMDWARFSourceLanguage,LLVMOpaqueMetadata *,const char *,size_t,int,const char *,size_t,unsigned int,const char *,size_t,LLVMDWARFEmissionKind,unsigned int,int,int,const char *,size_t,const char *,size_t) , LLVMDIBuilderCreateCompileUnit >(*this,lib,"LLVMDIBuilderCreateCompileUnit",SideEffects::worstDefault,"LLVMDIBuilderCreateCompileUnit")
		->args({"Builder","Lang","FileRef","Producer","ProducerLen","isOptimized","Flags","FlagsLen","RuntimeVer","SplitName","SplitNameLen","Kind","DWOId","SplitDebugInlining","DebugInfoForProfiling","SysRoot","SysRootLen","SDK","SDKLen"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:279:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,const char *,size_t,const char *,size_t) , LLVMDIBuilderCreateFile >(*this,lib,"LLVMDIBuilderCreateFile",SideEffects::worstDefault,"LLVMDIBuilderCreateFile")
		->args({"Builder","Filename","FilenameLen","Directory","DirectoryLen"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:298:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,const char *,size_t,const char *,size_t,const char *,size_t,const char *,size_t) , LLVMDIBuilderCreateModule >(*this,lib,"LLVMDIBuilderCreateModule",SideEffects::worstDefault,"LLVMDIBuilderCreateModule")
		->args({"Builder","ParentScope","Name","NameLen","ConfigMacros","ConfigMacrosLen","IncludePath","IncludePathLen","APINotesFile","APINotesFileLen"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:314:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,const char *,size_t,int) , LLVMDIBuilderCreateNameSpace >(*this,lib,"LLVMDIBuilderCreateNameSpace",SideEffects::worstDefault,"LLVMDIBuilderCreateNameSpace")
		->args({"Builder","ParentScope","Name","NameLen","ExportSymbols"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:337:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,const char *,size_t,const char *,size_t,LLVMOpaqueMetadata *,unsigned int,LLVMOpaqueMetadata *,int,int,unsigned int,LLVMDIFlags,int) , LLVMDIBuilderCreateFunction >(*this,lib,"LLVMDIBuilderCreateFunction",SideEffects::worstDefault,"LLVMDIBuilderCreateFunction")
		->args({"Builder","Scope","Name","NameLen","LinkageName","LinkageNameLen","File","LineNo","Ty","IsLocalToUnit","IsDefinition","ScopeLine","Flags","IsOptimized"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:352:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,unsigned int,unsigned int) , LLVMDIBuilderCreateLexicalBlock >(*this,lib,"LLVMDIBuilderCreateLexicalBlock",SideEffects::worstDefault,"LLVMDIBuilderCreateLexicalBlock")
		->args({"Builder","Scope","File","Line","Column"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:364:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,unsigned int) , LLVMDIBuilderCreateLexicalBlockFile >(*this,lib,"LLVMDIBuilderCreateLexicalBlockFile",SideEffects::worstDefault,"LLVMDIBuilderCreateLexicalBlockFile")
		->args({"Builder","Scope","File","Discriminator"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:378:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,unsigned int) , LLVMDIBuilderCreateImportedModuleFromNamespace >(*this,lib,"LLVMDIBuilderCreateImportedModuleFromNamespace",SideEffects::worstDefault,"LLVMDIBuilderCreateImportedModuleFromNamespace")
		->args({"Builder","Scope","NS","File","Line"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:394:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,unsigned int) , LLVMDIBuilderCreateImportedModuleFromAlias >(*this,lib,"LLVMDIBuilderCreateImportedModuleFromAlias",SideEffects::worstDefault,"LLVMDIBuilderCreateImportedModuleFromAlias")
		->args({"Builder","Scope","ImportedEntity","File","Line"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:409:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,unsigned int) , LLVMDIBuilderCreateImportedModuleFromModule >(*this,lib,"LLVMDIBuilderCreateImportedModuleFromModule",SideEffects::worstDefault,"LLVMDIBuilderCreateImportedModuleFromModule")
		->args({"Builder","Scope","M","File","Line"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:428:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueDIBuilder *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *,unsigned int,const char *,size_t) , LLVMDIBuilderCreateImportedDeclaration >(*this,lib,"LLVMDIBuilderCreateImportedDeclaration",SideEffects::worstDefault,"LLVMDIBuilderCreateImportedDeclaration")
		->args({"Builder","Scope","Decl","File","Line","Name","NameLen"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:446:1
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueContext *,unsigned int,unsigned int,LLVMOpaqueMetadata *,LLVMOpaqueMetadata *) , LLVMDIBuilderCreateDebugLocation >(*this,lib,"LLVMDIBuilderCreateDebugLocation",SideEffects::worstDefault,"LLVMDIBuilderCreateDebugLocation")
		->args({"Ctx","Line","Column","Scope","InlinedAt"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:456:10
	addExtern< unsigned int (*)(LLVMOpaqueMetadata *) , LLVMDILocationGetLine >(*this,lib,"LLVMDILocationGetLine",SideEffects::worstDefault,"LLVMDILocationGetLine")
		->args({"Location"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:464:10
	addExtern< unsigned int (*)(LLVMOpaqueMetadata *) , LLVMDILocationGetColumn >(*this,lib,"LLVMDILocationGetColumn",SideEffects::worstDefault,"LLVMDILocationGetColumn")
		->args({"Location"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:472:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueMetadata *) , LLVMDILocationGetScope >(*this,lib,"LLVMDILocationGetScope",SideEffects::worstDefault,"LLVMDILocationGetScope")
		->args({"Location"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:480:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueMetadata *) , LLVMDILocationGetInlinedAt >(*this,lib,"LLVMDILocationGetInlinedAt",SideEffects::worstDefault,"LLVMDILocationGetInlinedAt")
		->args({"Location"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:488:17
	addExtern< LLVMOpaqueMetadata * (*)(LLVMOpaqueMetadata *) , LLVMDIScopeGetFile >(*this,lib,"LLVMDIScopeGetFile",SideEffects::worstDefault,"LLVMDIScopeGetFile")
		->args({"Scope"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:497:13
	addExtern< const char * (*)(LLVMOpaqueMetadata *,unsigned int *) , LLVMDIFileGetDirectory >(*this,lib,"LLVMDIFileGetDirectory",SideEffects::worstDefault,"LLVMDIFileGetDirectory")
		->args({"File","Len"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:506:13
	addExtern< const char * (*)(LLVMOpaqueMetadata *,unsigned int *) , LLVMDIFileGetFilename >(*this,lib,"LLVMDIFileGetFilename",SideEffects::worstDefault,"LLVMDIFileGetFilename")
		->args({"File","Len"});
// from D:\Work\libclang\include\llvm-c/DebugInfo.h:515:13
	addExtern< const char * (*)(LLVMOpaqueMetadata *,unsigned int *) , LLVMDIFileGetSource >(*this,lib,"LLVMDIFileGetSource",SideEffects::worstDefault,"LLVMDIFileGetSource")
		->args({"File","Len"});
}
}

