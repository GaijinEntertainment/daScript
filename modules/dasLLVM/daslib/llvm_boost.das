options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module llvm_boost shared

require llvm public

require daslib/safe_addr

def LLVMBuildGEP ( builder:LLVMOpaqueBuilder?; ptr:LLVMOpaqueValue?; var index:LLVMOpaqueValue?; name:string )
    return LLVMBuildGEP(builder, ptr, unsafe(addr(index)), 1u, name)

def LLVMFloat4Type
    return LLVMVectorType(LLVMFloatType(), 4u)

def LLVMFunctionType ( var ret_type:LLVMTypeRef; var param_types:LLVMTypeRef[]; isVariadic:bool = false )
    return LLVMFunctionType( ret_type, unsafe(addr(param_types[0])), uint(length(param_types)), isVariadic ? 1 : 0)

def LLVMVerifyModule(mod:LLVMOpaqueModule?; action:LLVMVerifierFailureAction)
    var error : string
    if LLVMVerifyModule(mod, action, safe_addr(error)) != 0
        let msg = clone_string(error)
        LLVMDisposeMessage(error)
        panic(error)

def LLVMCreateJITCompilerForModule(var engine:LLVMExecutionEngineRef&; mod:LLVMOpaqueModule?; optlevel:uint=2u )
    var error : string
    if LLVMCreateJITCompilerForModule(unsafe(addr(engine)), mod, optlevel, safe_addr(error)) != 0
        let msg = clone_string(error)
        LLVMDisposeMessage(error)
        panic(msg)

def LLVMCreateExecutionEngineForModule(var engine:LLVMExecutionEngineRef&; mod:LLVMOpaqueModule? )
    var error : string
    if LLVMCreateExecutionEngineForModule(unsafe(addr(engine)), mod, safe_addr(error)) != 0
        let msg = clone_string(error)
        LLVMDisposeMessage(error)
        panic(msg)
