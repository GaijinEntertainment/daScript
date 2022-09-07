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
    error := ""
    LLVMInitializeAllTargetMCs()
    if LLVMCreateExecutionEngineForModule(safe_addr(engine), mod, safe_addr(error)) != 0
        panic("failed to create execution engine")
    if error != ""
        LLVMDisposeMessage(error)
        panic("error")

    var x = 10ul
    var y = 11ul

    var args <- [[LLVMGenericValueRef
        LLVMCreateGenericValueOfInt(LLVMInt32Type(), x, 0);
        LLVMCreateGenericValueOfInt(LLVMInt32Type(), y, 0)
    ]]
    var res = LLVMRunFunction(engine, sum, 2u, safe_addr(args[0]))
    print("result = {int(LLVMGenericValueToInt(res, 0))}\n")

    LLVMDisposeBuilder(builder)
    LLVMDisposeExecutionEngine(engine)
