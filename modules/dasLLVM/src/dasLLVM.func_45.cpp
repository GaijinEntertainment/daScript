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
void Module_dasLLVM::initFunctions_45() {
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:110:7
	addExtern< char * (*)(LLVMOpaqueTargetMachine *) , LLVMGetTargetMachineTriple >(*this,lib,"LLVMGetTargetMachineTriple",SideEffects::worstDefault,"LLVMGetTargetMachineTriple")
		->args({"T"});
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:115:7
	addExtern< char * (*)(LLVMOpaqueTargetMachine *) , LLVMGetTargetMachineCPU >(*this,lib,"LLVMGetTargetMachineCPU",SideEffects::worstDefault,"LLVMGetTargetMachineCPU")
		->args({"T"});
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:120:7
	addExtern< char * (*)(LLVMOpaqueTargetMachine *) , LLVMGetTargetMachineFeatureString >(*this,lib,"LLVMGetTargetMachineFeatureString",SideEffects::worstDefault,"LLVMGetTargetMachineFeatureString")
		->args({"T"});
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:123:19
	addExtern< LLVMOpaqueTargetData * (*)(LLVMOpaqueTargetMachine *) , LLVMCreateTargetDataLayout >(*this,lib,"LLVMCreateTargetDataLayout",SideEffects::worstDefault,"LLVMCreateTargetDataLayout")
		->args({"T"});
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:126:6
	addExtern< void (*)(LLVMOpaqueTargetMachine *,int) , LLVMSetTargetMachineAsmVerbosity >(*this,lib,"LLVMSetTargetMachineAsmVerbosity",SideEffects::worstDefault,"LLVMSetTargetMachineAsmVerbosity")
		->args({"T","VerboseAsm"});
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:132:10
	addExtern< int (*)(LLVMOpaqueTargetMachine *,LLVMOpaqueModule *,char *,LLVMCodeGenFileType,char **) , LLVMTargetMachineEmitToFile >(*this,lib,"LLVMTargetMachineEmitToFile",SideEffects::worstDefault,"LLVMTargetMachineEmitToFile")
		->args({"T","M","Filename","codegen","ErrorMessage"});
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:136:10
	addExtern< int (*)(LLVMOpaqueTargetMachine *,LLVMOpaqueModule *,LLVMCodeGenFileType,char **,LLVMOpaqueMemoryBuffer **) , LLVMTargetMachineEmitToMemoryBuffer >(*this,lib,"LLVMTargetMachineEmitToMemoryBuffer",SideEffects::worstDefault,"LLVMTargetMachineEmitToMemoryBuffer")
		->args({"T","M","codegen","ErrorMessage","OutMemBuf"});
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:142:7
	addExtern< char * (*)() , LLVMGetDefaultTargetTriple >(*this,lib,"LLVMGetDefaultTargetTriple",SideEffects::worstDefault,"LLVMGetDefaultTargetTriple");
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:146:7
	addExtern< char * (*)(const char *) , LLVMNormalizeTargetTriple >(*this,lib,"LLVMNormalizeTargetTriple",SideEffects::worstDefault,"LLVMNormalizeTargetTriple")
		->args({"triple"});
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:150:7
	addExtern< char * (*)() , LLVMGetHostCPUName >(*this,lib,"LLVMGetHostCPUName",SideEffects::worstDefault,"LLVMGetHostCPUName");
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:154:7
	addExtern< char * (*)() , LLVMGetHostCPUFeatures >(*this,lib,"LLVMGetHostCPUFeatures",SideEffects::worstDefault,"LLVMGetHostCPUFeatures");
// from D:\Work\libclang\include\llvm-c/TargetMachine.h:157:6
	addExtern< void (*)(LLVMOpaqueTargetMachine *,LLVMOpaquePassManager *) , LLVMAddAnalysisPasses >(*this,lib,"LLVMAddAnalysisPasses",SideEffects::worstDefault,"LLVMAddAnalysisPasses")
		->args({"T","PM"});
// from D:\Work\libclang\include\llvm-c/ExecutionEngine.h:36:6
	addExtern< void (*)() , LLVMLinkInMCJIT >(*this,lib,"LLVMLinkInMCJIT",SideEffects::worstDefault,"LLVMLinkInMCJIT");
// from D:\Work\libclang\include\llvm-c/ExecutionEngine.h:37:6
	addExtern< void (*)() , LLVMLinkInInterpreter >(*this,lib,"LLVMLinkInInterpreter",SideEffects::worstDefault,"LLVMLinkInInterpreter");
// from D:\Work\libclang\include\llvm-c/ExecutionEngine.h:53:21
	addExtern< LLVMOpaqueGenericValue * (*)(LLVMOpaqueType *,unsigned long long,int) , LLVMCreateGenericValueOfInt >(*this,lib,"LLVMCreateGenericValueOfInt",SideEffects::worstDefault,"LLVMCreateGenericValueOfInt")
		->args({"Ty","N","IsSigned"});
// from D:\Work\libclang\include\llvm-c/ExecutionEngine.h:57:21
	addExtern< LLVMOpaqueGenericValue * (*)(void *) , LLVMCreateGenericValueOfPointer >(*this,lib,"LLVMCreateGenericValueOfPointer",SideEffects::worstDefault,"LLVMCreateGenericValueOfPointer")
		->args({"P"});
// from D:\Work\libclang\include\llvm-c/ExecutionEngine.h:59:21
	addExtern< LLVMOpaqueGenericValue * (*)(LLVMOpaqueType *,double) , LLVMCreateGenericValueOfFloat >(*this,lib,"LLVMCreateGenericValueOfFloat",SideEffects::worstDefault,"LLVMCreateGenericValueOfFloat")
		->args({"Ty","N"});
// from D:\Work\libclang\include\llvm-c/ExecutionEngine.h:61:10
	addExtern< unsigned int (*)(LLVMOpaqueGenericValue *) , LLVMGenericValueIntWidth >(*this,lib,"LLVMGenericValueIntWidth",SideEffects::worstDefault,"LLVMGenericValueIntWidth")
		->args({"GenValRef"});
// from D:\Work\libclang\include\llvm-c/ExecutionEngine.h:63:20
	addExtern< unsigned long long (*)(LLVMOpaqueGenericValue *,int) , LLVMGenericValueToInt >(*this,lib,"LLVMGenericValueToInt",SideEffects::worstDefault,"LLVMGenericValueToInt")
		->args({"GenVal","IsSigned"});
// from D:\Work\libclang\include\llvm-c/ExecutionEngine.h:66:7
	addExtern< void * (*)(LLVMOpaqueGenericValue *) , LLVMGenericValueToPointer >(*this,lib,"LLVMGenericValueToPointer",SideEffects::worstDefault,"LLVMGenericValueToPointer")
		->args({"GenVal"});
}
}

