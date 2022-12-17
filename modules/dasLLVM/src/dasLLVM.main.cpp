#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasLLVM.h"
#include "need_dasLLVM.h"
#include "cb_dasLLVM.h"

namespace das {


LLVMDisasmContextRef das_LLVMCreateDisasm (const char *TripleName, void *DisInfo, int TagType) {
    return LLVMCreateDisasm(TripleName,DisInfo,TagType,nullptr,nullptr);
}

___noinline float3 test_abi_mad ( float3 a, float3 b, float3 c ) {
    /*
    printf("a = %f,%f,%f\n", v_extract_x(a), v_extract_y(a), v_extract_z(a));
    printf("b = %f,%f,%f\n", v_extract_x(b), v_extract_y(b), v_extract_z(b));
    printf("c = %f,%f,%f\n", v_extract_x(c), v_extract_y(c), v_extract_z(c));
    */
    return v_add(v_mul(a,b),c);
}

void Module_dasLLVM::initMain() {
	addExtern< void (*)(LLVMOpaquePassRegistry *) , LLVMInitializeCore >(*this,lib,"LLVMInitializeCore",SideEffects::worstDefault,"LLVMInitializeCore")
		->args({"R"});
    // disasm
	addExtern<DAS_BIND_FUN(das_LLVMCreateDisasm) >(*this,lib,"LLVMCreateDisasm",SideEffects::worstDefault,"das_LLVMCreateDisasm")
		->args({"TripleName","DisInfo","TagType"});
    // abi tests
	addExtern<DAS_BIND_FUN(test_abi_mad) >(*this,lib,"test_abi_mad",SideEffects::worstDefault,"test_abi_mad")
		->args({"a","b","c"});
}

ModuleAotType Module_dasLLVM::aotRequire ( TextWriter & tw ) const {
    // add your stuff here
    // tw << "#include <clang-c/Index.h>\n";
    tw << "#include \"../modules/dasLLVM/src/cb_dasClangBind.h\"\n";
    // tw << "#include \"../modules/dasLLVM/src/dasClangBind.enum.decl.cast.inc\"\n";
    return ModuleAotType::cpp;
}

}


