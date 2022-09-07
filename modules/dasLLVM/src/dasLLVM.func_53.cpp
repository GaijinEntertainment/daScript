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
void Module_dasLLVM::initFunctions_53() {
// from D:\Work\libclang\include\llvm-c/lto.h:298:1
	addExtern< const char * (*)(LLVMOpaqueLTOModule *) , lto_module_get_linkeropts >(*this,lib,"lto_module_get_linkeropts",SideEffects::worstDefault,"lto_module_get_linkeropts")
		->args({"mod"});
// from D:\Work\libclang\include\llvm-c/lto.h:311:19
	addExtern< bool (*)(LLVMOpaqueLTOModule *,unsigned int *,unsigned int *) , lto_module_get_macho_cputype >(*this,lib,"lto_module_get_macho_cputype",SideEffects::worstDefault,"lto_module_get_macho_cputype")
		->args({"mod","out_cputype","out_cpusubtype"});
// from D:\Work\libclang\include\llvm-c/lto.h:360:1
	addExtern< LLVMOpaqueLTOCodeGenerator * (*)() , lto_codegen_create >(*this,lib,"lto_codegen_create",SideEffects::worstDefault,"lto_codegen_create");
// from D:\Work\libclang\include\llvm-c/lto.h:372:1
	addExtern< LLVMOpaqueLTOCodeGenerator * (*)() , lto_codegen_create_in_local_context >(*this,lib,"lto_codegen_create_in_local_context",SideEffects::worstDefault,"lto_codegen_create_in_local_context");
// from D:\Work\libclang\include\llvm-c/lto.h:381:1
	addExtern< void (*)(LLVMOpaqueLTOCodeGenerator *) , lto_codegen_dispose >(*this,lib,"lto_codegen_dispose",SideEffects::worstDefault,"lto_codegen_dispose")
		->args({""});
// from D:\Work\libclang\include\llvm-c/lto.h:394:1
	addExtern< bool (*)(LLVMOpaqueLTOCodeGenerator *,LLVMOpaqueLTOModule *) , lto_codegen_add_module >(*this,lib,"lto_codegen_add_module",SideEffects::worstDefault,"lto_codegen_add_module")
		->args({"cg","mod"});
// from D:\Work\libclang\include\llvm-c/lto.h:405:1
	addExtern< void (*)(LLVMOpaqueLTOCodeGenerator *,LLVMOpaqueLTOModule *) , lto_codegen_set_module >(*this,lib,"lto_codegen_set_module",SideEffects::worstDefault,"lto_codegen_set_module")
		->args({"cg","mod"});
// from D:\Work\libclang\include\llvm-c/lto.h:414:1
	addExtern< bool (*)(LLVMOpaqueLTOCodeGenerator *,lto_debug_model) , lto_codegen_set_debug_model >(*this,lib,"lto_codegen_set_debug_model",SideEffects::worstDefault,"lto_codegen_set_debug_model")
		->args({"cg",""});
// from D:\Work\libclang\include\llvm-c/lto.h:423:1
	addExtern< bool (*)(LLVMOpaqueLTOCodeGenerator *,lto_codegen_model) , lto_codegen_set_pic_model >(*this,lib,"lto_codegen_set_pic_model",SideEffects::worstDefault,"lto_codegen_set_pic_model")
		->args({"cg",""});
// from D:\Work\libclang\include\llvm-c/lto.h:431:1
	addExtern< void (*)(LLVMOpaqueLTOCodeGenerator *,const char *) , lto_codegen_set_cpu >(*this,lib,"lto_codegen_set_cpu",SideEffects::worstDefault,"lto_codegen_set_cpu")
		->args({"cg","cpu"});
// from D:\Work\libclang\include\llvm-c/lto.h:440:1
	addExtern< void (*)(LLVMOpaqueLTOCodeGenerator *,const char *) , lto_codegen_set_assembler_path >(*this,lib,"lto_codegen_set_assembler_path",SideEffects::worstDefault,"lto_codegen_set_assembler_path")
		->args({"cg","path"});
// from D:\Work\libclang\include\llvm-c/lto.h:448:1
	addExtern< void (*)(LLVMOpaqueLTOCodeGenerator *,const char **,int) , lto_codegen_set_assembler_args >(*this,lib,"lto_codegen_set_assembler_args",SideEffects::worstDefault,"lto_codegen_set_assembler_args")
		->args({"cg","args","nargs"});
// from D:\Work\libclang\include\llvm-c/lto.h:459:1
	addExtern< void (*)(LLVMOpaqueLTOCodeGenerator *,const char *) , lto_codegen_add_must_preserve_symbol >(*this,lib,"lto_codegen_add_must_preserve_symbol",SideEffects::worstDefault,"lto_codegen_add_must_preserve_symbol")
		->args({"cg","symbol"});
// from D:\Work\libclang\include\llvm-c/lto.h:469:1
	addExtern< bool (*)(LLVMOpaqueLTOCodeGenerator *,const char *) , lto_codegen_write_merged_modules >(*this,lib,"lto_codegen_write_merged_modules",SideEffects::worstDefault,"lto_codegen_write_merged_modules")
		->args({"cg","path"});
// from D:\Work\libclang\include\llvm-c/lto.h:484:1
	addExtern< const void * (*)(LLVMOpaqueLTOCodeGenerator *,size_t *) , lto_codegen_compile >(*this,lib,"lto_codegen_compile",SideEffects::worstDefault,"lto_codegen_compile")
		->args({"cg","length"});
// from D:\Work\libclang\include\llvm-c/lto.h:496:1
	addExtern< bool (*)(LLVMOpaqueLTOCodeGenerator *,const char **) , lto_codegen_compile_to_file >(*this,lib,"lto_codegen_compile_to_file",SideEffects::worstDefault,"lto_codegen_compile_to_file")
		->args({"cg","name"});
// from D:\Work\libclang\include\llvm-c/lto.h:504:1
	addExtern< bool (*)(LLVMOpaqueLTOCodeGenerator *) , lto_codegen_optimize >(*this,lib,"lto_codegen_optimize",SideEffects::worstDefault,"lto_codegen_optimize")
		->args({"cg"});
// from D:\Work\libclang\include\llvm-c/lto.h:519:1
	addExtern< const void * (*)(LLVMOpaqueLTOCodeGenerator *,size_t *) , lto_codegen_compile_optimized >(*this,lib,"lto_codegen_compile_optimized",SideEffects::worstDefault,"lto_codegen_compile_optimized")
		->args({"cg","length"});
// from D:\Work\libclang\include\llvm-c/lto.h:527:1
	addExtern< unsigned int (*)() , lto_api_version >(*this,lib,"lto_api_version",SideEffects::worstDefault,"lto_api_version");
// from D:\Work\libclang\include\llvm-c/lto.h:541:13
	addExtern< void (*)(const char *const *,int) , lto_set_debug_options >(*this,lib,"lto_set_debug_options",SideEffects::worstDefault,"lto_set_debug_options")
		->args({"options","number"});
}
}

