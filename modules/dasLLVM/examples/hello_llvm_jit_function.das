require daslib/safe_addr
require daslib/math_bits

require llvm/llvm_boost
require jit

[sideeffects]
def add_two_ints ( a,b : int ) : int
    panic("not implemented")
    return 0

[export]
def main
    var mod = LLVMModuleCreateWithName("my_module")
    // NOTE: we are making this example compatible with upcomming JIT, i.e. function signature is that of a JIT function
    // Context * context, vec4f * args, void * CMRES
    var ret_type = LLVMFunctionType(LLVMFloat4Type(), [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u); LLVMPointerType(LLVMVoidType(),0u) ]])
    var sum = LLVMAddFunction(mod, "sum", ret_type)
    var entry = LLVMAppendBasicBlock(sum, "entry")
    var builder = LLVMCreateBuilder()
    LLVMPositionBuilderAtEnd(builder, entry)
    var args = LLVMGetParam(sum, 1u)
    var a1 = LLVMBuildGEP(builder, args, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "a1")
    var a2 = LLVMBuildGEP(builder, args, LLVMConstInt(LLVMInt32Type(), 1ul, 0), "a2")
    var v1 = LLVMBuildLoad2(builder, LLVMInt32Type(), a1, "v1")
    var v2 = LLVMBuildLoad2(builder, LLVMInt32Type(), a2, "v2")
    var t1 = LLVMBuildAdd(builder, v1, v2, "t1")
    var tbit = LLVMBuildBitCast(builder, t1, LLVMFloatType(), "tbit")
    var vres = LLVMGetUndef(LLVMFloat4Type())
    var tb = LLVMBuildInsertElement(builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "tb")
    LLVMBuildRet(builder, tb)
    LLVMVerifyModule(mod, LLVMVerifierFailureAction LLVMAbortProcessAction)
    var engine : LLVMExecutionEngineRef
    LLVMLinkInMCJIT()
    LLVMInitializeNativeTarget()
    LLVMInitializeNativeAsmPrinter()
    LLVMInitializeNativeAsmParser()
    LLVMCreateJITCompilerForModule(engine, mod)
    var pfun = LLVMGetFunctionAddress(engine, "sum")
    unsafe
        instrument_jit(reinterpret<void?> pfun, @@add_two_ints)
    let res = add_two_ints(11,12)
    print("result = {res}\n")
    LLVMDumpModule(mod)
    LLVMDisposeBuilder(builder)
    LLVMDisposeExecutionEngine(engine)
