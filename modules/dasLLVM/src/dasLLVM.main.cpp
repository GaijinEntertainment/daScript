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

float2 test_abi_mad2 ( float2 a, float2 b, float2 c ) {
    return v_add(v_mul(a,b),c);
}

float3 test_abi_mad3 ( float3 a, float3 b, float3 c ) {
    return v_add(v_mul(a,b),c);
}

float4 test_abi_mad4 ( float4 a, float4 b, float4 c ) {
    return v_add(v_mul(a,b),c);
}

void Module_dasLLVM::initMain() {
	addExtern< void (*)(LLVMOpaquePassRegistry *) , LLVMInitializeCore >(*this,lib,"LLVMInitializeCore",SideEffects::worstDefault,"LLVMInitializeCore")
		->args({"R"});
    // disasm
	addExtern<DAS_BIND_FUN(das_LLVMCreateDisasm) >(*this,lib,"LLVMCreateDisasm",SideEffects::worstDefault,"das_LLVMCreateDisasm")
		->args({"TripleName","DisInfo","TagType"});
    // abi tests
	addExtern<DAS_BIND_FUN(test_abi_mad2) >(*this,lib,"test_abi_mad",SideEffects::worstDefault,"test_abi_mad2")
		->args({"a","b","c"});
	addExtern<DAS_BIND_FUN(test_abi_mad3) >(*this,lib,"test_abi_mad",SideEffects::worstDefault,"test_abi_mad3")
		->args({"a","b","c"});
	addExtern<DAS_BIND_FUN(test_abi_mad4) >(*this,lib,"test_abi_mad",SideEffects::worstDefault,"test_abi_mad4")
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


