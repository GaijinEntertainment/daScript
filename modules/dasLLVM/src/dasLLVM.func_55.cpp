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
void Module_dasLLVM::initFunctions_55() {
// from D:\Work\libclang\include\llvm-c/lto.h:783:6
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,const char *) , thinlto_set_generated_objects_dir >(*this,lib,"thinlto_set_generated_objects_dir",SideEffects::worstDefault,"thinlto_set_generated_objects_dir")
		->args({"cg","save_temps_dir"});
// from D:\Work\libclang\include\llvm-c/lto.h:791:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,const char *) , thinlto_codegen_set_cpu >(*this,lib,"thinlto_codegen_set_cpu",SideEffects::worstDefault,"thinlto_codegen_set_cpu")
		->args({"cg","cpu"});
// from D:\Work\libclang\include\llvm-c/lto.h:799:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,bool) , thinlto_codegen_disable_codegen >(*this,lib,"thinlto_codegen_disable_codegen",SideEffects::worstDefault,"thinlto_codegen_disable_codegen")
		->args({"cg","disable"});
// from D:\Work\libclang\include\llvm-c/lto.h:807:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,bool) , thinlto_codegen_set_codegen_only >(*this,lib,"thinlto_codegen_set_codegen_only",SideEffects::worstDefault,"thinlto_codegen_set_codegen_only")
		->args({"cg","codegen_only"});
// from D:\Work\libclang\include\llvm-c/lto.h:815:13
	addExtern< void (*)(const char *const *,int) , thinlto_debug_options >(*this,lib,"thinlto_debug_options",SideEffects::worstDefault,"thinlto_debug_options")
		->args({"options","number"});
// from D:\Work\libclang\include\llvm-c/lto.h:822:19
	addExtern< bool (*)(LLVMOpaqueLTOModule *) , lto_module_is_thinlto >(*this,lib,"lto_module_is_thinlto",SideEffects::worstDefault,"lto_module_is_thinlto")
		->args({"mod"});
// from D:\Work\libclang\include\llvm-c/lto.h:832:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,const char *,int) , thinlto_codegen_add_must_preserve_symbol >(*this,lib,"thinlto_codegen_add_must_preserve_symbol",SideEffects::worstDefault,"thinlto_codegen_add_must_preserve_symbol")
		->args({"cg","name","length"});
// from D:\Work\libclang\include\llvm-c/lto.h:844:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,const char *,int) , thinlto_codegen_add_cross_referenced_symbol >(*this,lib,"thinlto_codegen_add_cross_referenced_symbol",SideEffects::worstDefault,"thinlto_codegen_add_cross_referenced_symbol")
		->args({"cg","name","length"});
// from D:\Work\libclang\include\llvm-c/lto.h:875:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,const char *) , thinlto_codegen_set_cache_dir >(*this,lib,"thinlto_codegen_set_cache_dir",SideEffects::worstDefault,"thinlto_codegen_set_cache_dir")
		->args({"cg","cache_dir"});
// from D:\Work\libclang\include\llvm-c/lto.h:885:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,int) , thinlto_codegen_set_cache_pruning_interval >(*this,lib,"thinlto_codegen_set_cache_pruning_interval",SideEffects::worstDefault,"thinlto_codegen_set_cache_pruning_interval")
		->args({"cg","interval"});
// from D:\Work\libclang\include\llvm-c/lto.h:901:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,unsigned int) , thinlto_codegen_set_final_cache_size_relative_to_available_space >(*this,lib,"thinlto_codegen_set_final_cache_size_relative_to_available_space",SideEffects::worstDefault,"thinlto_codegen_set_final_cache_size_relative_to_available_space")
		->args({"cg","percentage"});
// from D:\Work\libclang\include\llvm-c/lto.h:910:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,unsigned int) , thinlto_codegen_set_cache_entry_expiration >(*this,lib,"thinlto_codegen_set_cache_entry_expiration",SideEffects::worstDefault,"thinlto_codegen_set_cache_entry_expiration")
		->args({"cg","expiration"});
// from D:\Work\libclang\include\llvm-c/lto.h:921:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,unsigned int) , thinlto_codegen_set_cache_size_bytes >(*this,lib,"thinlto_codegen_set_cache_size_bytes",SideEffects::worstDefault,"thinlto_codegen_set_cache_size_bytes")
		->args({"cg","max_size_bytes"});
// from D:\Work\libclang\include\llvm-c/lto.h:931:1
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,unsigned int) , thinlto_codegen_set_cache_size_megabytes >(*this,lib,"thinlto_codegen_set_cache_size_megabytes",SideEffects::worstDefault,"thinlto_codegen_set_cache_size_megabytes")
		->args({"cg","max_size_megabytes"});
// from D:\Work\libclang\include\llvm-c/lto.h:940:13
	addExtern< void (*)(LLVMOpaqueThinLTOCodeGenerator *,unsigned int) , thinlto_codegen_set_cache_size_files >(*this,lib,"thinlto_codegen_set_cache_size_files",SideEffects::worstDefault,"thinlto_codegen_set_cache_size_files")
		->args({"cg","max_size_files"});
// from D:\Work\libclang\include\llvm-c/Object.h:74:15
	addExtern< LLVMOpaqueBinary * (*)(LLVMOpaqueMemoryBuffer *,LLVMOpaqueContext *,char **) , LLVMCreateBinary >(*this,lib,"LLVMCreateBinary",SideEffects::worstDefault,"LLVMCreateBinary")
		->args({"MemBuf","Context","ErrorMessage"});
// from D:\Work\libclang\include\llvm-c/Object.h:84:6
	addExtern< void (*)(LLVMOpaqueBinary *) , LLVMDisposeBinary >(*this,lib,"LLVMDisposeBinary",SideEffects::worstDefault,"LLVMDisposeBinary")
		->args({"BR"});
// from D:\Work\libclang\include\llvm-c/Object.h:95:21
	addExtern< LLVMOpaqueMemoryBuffer * (*)(LLVMOpaqueBinary *) , LLVMBinaryCopyMemoryBuffer >(*this,lib,"LLVMBinaryCopyMemoryBuffer",SideEffects::worstDefault,"LLVMBinaryCopyMemoryBuffer")
		->args({"BR"});
// from D:\Work\libclang\include\llvm-c/Object.h:102:16
	addExtern< LLVMBinaryType (*)(LLVMOpaqueBinary *) , LLVMBinaryGetType >(*this,lib,"LLVMBinaryGetType",SideEffects::worstDefault,"LLVMBinaryGetType")
		->args({"BR"});
// from D:\Work\libclang\include\llvm-c/Object.h:115:15
	addExtern< LLVMOpaqueBinary * (*)(LLVMOpaqueBinary *,const char *,size_t,char **) , LLVMMachOUniversalBinaryCopyObjectForArch >(*this,lib,"LLVMMachOUniversalBinaryCopyObjectForArch",SideEffects::worstDefault,"LLVMMachOUniversalBinaryCopyObjectForArch")
		->args({"BR","Arch","ArchLen","ErrorMessage"});
}
}

