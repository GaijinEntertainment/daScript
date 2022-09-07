require daslib/safe_addr
require llvm

[export]
def main
    var mod = LLVMModuleCreateWithName("my_module")
    var param_types <- [[LLVMTypeRef LLVMInt32Type(); LLVMInt32Type() ]]
    var ret_type = LLVMFunctionType(LLVMInt32Type(), safe_addr(param_types[0]), 2u, 0)
    var sum = LLVMAddFunction(mod, "sum", ret_type)
    var entry = LLVMAppendBasicBlock(sum, "entry")
    var builder = LLVMCreateBuilder()
    LLVMPositionBuilderAtEnd(builder, entry)
    var tmp = LLVMBuildAdd(builder, LLVMGetParam(sum, 0u), LLVMGetParam(sum, 1u), "tmp")
    LLVMBuildRet(builder, tmp)
    var error : string
    LLVMVerifyModule(mod, LLVMVerifierFailureAction LLVMAbortProcessAction, safe_addr(error))
    LLVMDisposeMessage(error)
    var engine : LLVMExecutionEngineRef
    error = ""
    LLVMLinkInMCJIT()
    LLVMInitializeNativeTarget()
    LLVMInitializeNativeAsmPrinter()
    LLVMInitializeNativeAsmParser()
    if LLVMCreateJITCompilerForModule(safe_addr(engine), mod, 2u, safe_addr(error)) != 0
        panic("failed to create JIT compilerdue to {error}")
    if error != ""
        LLVMDisposeMessage(error)
        panic("error")
    var pfun = LLVMGetFunctionAddress(engine, "sum")
    let res = run_int_int_int_function(unsafe(reinterpret<void?> pfun), 11, 12)
    print("result = {res}\n")
    LLVMDumpModule(mod)
    LLVMDisposeBuilder(builder)
    LLVMDisposeExecutionEngine(engine)
