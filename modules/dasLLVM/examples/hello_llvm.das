require daslib/safe_addr
require llvm/llvm_boost

[export]
def main
    var mod = LLVMModuleCreateWithName("my_module")
    var ret_type = LLVMFunctionType(LLVMInt32Type(), [[LLVMTypeRef LLVMInt32Type(); LLVMInt32Type() ]])
    var sum = LLVMAddFunction(mod, "sum", ret_type)
    var entry = LLVMAppendBasicBlock(sum, "entry")
    var builder = LLVMCreateBuilder()
    LLVMPositionBuilderAtEnd(builder, entry)
    var tmp = LLVMBuildAdd(builder, LLVMGetParam(sum, 0u), LLVMGetParam(sum, 1u), "tmp")
    LLVMBuildRet(builder, tmp)
    LLVMVerifyModule(mod, LLVMVerifierFailureAction LLVMAbortProcessAction)
    var engine : LLVMExecutionEngineRef
    var error : string
    LLVMInitializeAllTargets()
    LLVMInitializeAllTargetMCs()
    LLVMCreateExecutionEngineForModule(engine, mod)
    var x = 10ul
    var y = 11ul
    var args <- [[LLVMGenericValueRef
        LLVMCreateGenericValueOfInt(LLVMInt32Type(), x, 0);
        LLVMCreateGenericValueOfInt(LLVMInt32Type(), y, 0)
    ]]
    var res = LLVMRunFunction(engine, sum, 2u, safe_addr(args[0]))
    print("result = {int(LLVMGenericValueToInt(res, 0))}\n")
    LLVMDumpModule(mod)
    LLVMDisposeBuilder(builder)
    LLVMDisposeExecutionEngine(engine)
