require daslib/safe_addr
require daslib/math_bits
require llvm

[export]
def main
    var mod = LLVMModuleCreateWithName("my_module")
    // NOTE: we are making this example compatible with upcomming JIT, i.e. function signature is that of a JIT function
    // Context * context, vec4f * args, void * CMRES
    var float4type <- LLVMVectorType(LLVMFloatType(), 4u)
    var param_types <- [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(float4type,0u); LLVMPointerType(LLVMVoidType(),0u) ]]
    var ret_type = LLVMFunctionType(float4type, safe_addr(param_types[0]), 3u, 0)
    var sum = LLVMAddFunction(mod, "sum", ret_type)
    var entry = LLVMAppendBasicBlock(sum, "entry")
    var builder = LLVMCreateBuilder()
    LLVMPositionBuilderAtEnd(builder, entry)
    var args = LLVMGetParam(sum, 1u)
    var i0 = LLVMConstInt(LLVMInt32Type(), 0ul, 0)
    var i1 = LLVMConstInt(LLVMInt32Type(), 1ul, 0)
    var a1 = LLVMBuildGEP(builder, args, safe_addr(i0), 1u, "a1")
    var a2 = LLVMBuildGEP(builder, args, safe_addr(i1), 1u, "a2")
    var v1 = LLVMBuildLoad2(builder, LLVMInt32Type(), a1, "v1")
    var v2 = LLVMBuildLoad2(builder, LLVMInt32Type(), a2, "v2")
    var t1 = LLVMBuildAdd(builder, v1, v2, "t1")
    var tbit = LLVMBuildBitCast(builder, t1, LLVMFloatType(), "tbit")
    var vres = LLVMGetUndef(float4type)
    var tb = LLVMBuildInsertElement(builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "tb")
    LLVMBuildRet(builder, tb)
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
    var values <- [[int4
        int4(11,0,0,0);
        int4(12,0,0,0)
    ]]
    var res : float4
    unsafe
        res = invoke_code ( reinterpret<void?> pfun, safe_addr(values[0]), null)
    print("result = {float_bits_to_int(res)}\n")
    LLVMDumpModule(mod)
    LLVMDisposeBuilder(builder)
    LLVMDisposeExecutionEngine(engine)
