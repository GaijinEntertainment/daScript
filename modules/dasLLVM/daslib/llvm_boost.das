options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module llvm_boost shared

require llvm public

require daslib/safe_addr
require daslib/defer

def LLVMBuildShuffleVector(builder:LLVMOpaqueBuilder?; v1, v2:LLVMOpaqueValue?; mask:array<int>; name:string )
    var elem : array<LLVMOpaqueValue?>
    for m in mask
        if m!=-1
            elem |> push(LLVMConstInt(LLVMInt32Type(), uint64(m), 0))
        else
            elem |> push(LLVMGetUndef(LLVMInt32Type()))
    defer_delete(elem)
    return LLVMBuildShuffleVector(builder, v1, v2, LLVMConstVector(unsafe(addr(elem[0])), 4u), name)

def LLVMAddIncoming(phi:LLVMOpaqueValue?; var phi_vals:LLVMOpaqueValue? []; var phi_blocks:LLVMOpaqueBasicBlock? [])
    static_if typeinfo(dim phi_vals) != typeinfo(dim phi_blocks)
        concept_assert(false, "LLVMAddIncoming: phi_vals and phi_blocks must have the same length")
    LLVMAddIncoming(phi, unsafe(addr(phi_vals[0])), unsafe(addr(phi_blocks[0])), uint(length(phi_vals)))

def LLVMAddIncoming(phi:LLVMOpaqueValue?; var phi_vals:array<LLVMOpaqueValue?>; var phi_blocks:array<LLVMOpaqueBasicBlock?>)
    assert(length(phi_vals) == length(phi_blocks), "LLVMAddIncoming: phi_vals and phi_blocks must have the same length")
    LLVMAddIncoming(phi, unsafe(addr(phi_vals[0])), unsafe(addr(phi_blocks[0])), uint(length(phi_vals)))

def LLVMBuildGEP ( builder:LLVMOpaqueBuilder?; ptr:LLVMOpaqueValue?; var index:LLVMOpaqueValue?; name:string )
    return LLVMBuildGEP(builder, ptr, unsafe(addr(index)), 1u, name)

def LLVMBuildGEP2 ( builder:LLVMOpaqueBuilder?; ty:LLVMOpaqueType?; ptr:LLVMOpaqueValue?; var index:LLVMOpaqueValue?; name:string )
    return LLVMBuildGEP2(builder, ty, ptr, unsafe(addr(index)), 1u, name)

def LLVMBuildStore2 ( builder:LLVMOpaqueBuilder?; typ:LLVMOpaqueType?; val:LLVMOpaqueValue?; ptr:LLVMOpaqueValue?  )
    var tptr = LLVMBuildPointerCast(builder, ptr, LLVMPointerType(typ,0u), "")
    return LLVMBuildStore(builder, val, tptr)

def LLVMFloat4Type
    return LLVMVectorType(LLVMFloatType(), 4u)

def LLVMFloat3Type
    return LLVMVectorType(LLVMFloatType(), 3u)

def LLVMFloat2Type
    return LLVMVectorType(LLVMFloatType(), 2u)

def LLVMInt4Type
    return LLVMVectorType(LLVMInt32Type(), 4u)

def LLVMInt3Type
    return LLVMVectorType(LLVMInt32Type(), 3u)

def LLVMInt2Type
    return LLVMVectorType(LLVMInt32Type(), 2u)

def LLVMIntPtrType : LLVMOpaqueType?
    static_if typeinfo(sizeof type<void?>)==8
        return LLVMInt64Type()
    else
        return LLVMInt32Type()

def LLVMFunctionType ( var ret_type:LLVMTypeRef; var param_types:LLVMTypeRef[]; isVariadic:bool = false )
    return LLVMFunctionType( ret_type, unsafe(addr(param_types[0])), uint(length(param_types)), isVariadic ? 1 : 0)

def LLVMFunctionType ( var ret_type:LLVMTypeRef; var param_types:array<LLVMTypeRef>; isVariadic:bool = false )
    let nparam = length(param_types)
    if nparam == 0
        return LLVMFunctionType( ret_type, null, 0u, isVariadic ? 1 : 0)
    else
        return LLVMFunctionType( ret_type, unsafe(addr(param_types[0])), uint(nparam), isVariadic ? 1 : 0)

def LLVMFunctionType ( var ret_type:LLVMTypeRef; isVariadic:bool = false )
    return LLVMFunctionType( ret_type, null, 0u, isVariadic ? 1 : 0)

def LLVMVerifyModule(mod:LLVMOpaqueModule?; action:LLVMVerifierFailureAction; fail:bool=true)
    var error : string
    if LLVMVerifyModule(mod, action, safe_addr(error)) != 0
        if fail
            let msg = clone_string(error)
            LLVMDisposeMessage(error)
            panic(msg)
        else
            LLVMDisposeMessage(error)
        return false
    return true

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

def LLVMBuildInt2_xy ( builder:LLVMOpaqueBuilder?; x,y:LLVMOpaqueValue?; name:string )
    var vres = LLVMGetUndef(LLVMInt2Type())
    vres = LLVMBuildInsertElement(builder, vres, x, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "")
    return LLVMBuildInsertElement(builder, vres, y, LLVMConstInt(LLVMInt32Type(), 1ul, 0), name)

def LLVMBuildInt3_xyz ( builder:LLVMOpaqueBuilder?; x,y,z:LLVMOpaqueValue?; name:string )
    var vres = LLVMGetUndef(LLVMInt3Type())
    vres = LLVMBuildInsertElement(builder, vres, x, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "")
    vres = LLVMBuildInsertElement(builder, vres, y, LLVMConstInt(LLVMInt32Type(), 1ul, 0), "")
    return LLVMBuildInsertElement(builder, vres, z, LLVMConstInt(LLVMInt32Type(), 2ul, 0), name)

def LLVMBuildInt4_xyzw ( builder:LLVMOpaqueBuilder?; x,y,z,w:LLVMOpaqueValue?; name:string )
    var vres = LLVMGetUndef(LLVMInt4Type())
    vres = LLVMBuildInsertElement(builder, vres, x, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "")
    vres = LLVMBuildInsertElement(builder, vres, y, LLVMConstInt(LLVMInt32Type(), 1ul, 0), "")
    vres = LLVMBuildInsertElement(builder, vres, z, LLVMConstInt(LLVMInt32Type(), 2ul, 0), "")
    return LLVMBuildInsertElement(builder, vres, w, LLVMConstInt(LLVMInt32Type(), 3ul, 0), name)

def LLVMBuildFloat2_xy ( builder:LLVMOpaqueBuilder?; x,y:LLVMOpaqueValue?; name:string )
    var vres = LLVMGetUndef(LLVMFloat2Type())
    vres = LLVMBuildInsertElement(builder, vres, x, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "")
    return LLVMBuildInsertElement(builder, vres, y, LLVMConstInt(LLVMInt32Type(), 1ul, 0), name)

def LLVMBuildFloat3_xyz ( builder:LLVMOpaqueBuilder?; x,y,z:LLVMOpaqueValue?; name:string )
    var vres = LLVMGetUndef(LLVMFloat3Type())
    vres = LLVMBuildInsertElement(builder, vres, x, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "")
    vres = LLVMBuildInsertElement(builder, vres, y, LLVMConstInt(LLVMInt32Type(), 1ul, 0), "")
    return LLVMBuildInsertElement(builder, vres, z, LLVMConstInt(LLVMInt32Type(), 2ul, 0), name)

def LLVMBuildFloat4_xyzw ( builder:LLVMOpaqueBuilder?; x,y,z,w:LLVMOpaqueValue?; name:string )
    var vres = LLVMGetUndef(LLVMFloat4Type())
    vres = LLVMBuildInsertElement(builder, vres, x, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "")
    vres = LLVMBuildInsertElement(builder, vres, y, LLVMConstInt(LLVMInt32Type(), 1ul, 0), "")
    vres = LLVMBuildInsertElement(builder, vres, z, LLVMConstInt(LLVMInt32Type(), 2ul, 0), "")
    return LLVMBuildInsertElement(builder, vres, w, LLVMConstInt(LLVMInt32Type(), 3ul, 0), name)

def LLVMBuildCall ( builder:LLVMOpaqueBuilder?; func:LLVMOpaqueValue?; var args:LLVMOpaqueValue? []; name:string )
    return LLVMBuildCall( builder, func, unsafe(addr(args[0])), uint(length(args)), name)

def LLVMBuildCall ( builder:LLVMOpaqueBuilder?; func:LLVMOpaqueValue?; name:string )
    return LLVMBuildCall( builder, func, null, 0u, name)

def LLVMBuildCall ( builder:LLVMOpaqueBuilder?; func:LLVMOpaqueValue?; var args:array<LLVMOpaqueValue?>; name:string )
    let nparams = length(args)
    if nparams != 0
        return LLVMBuildCall( builder, func, unsafe(addr(args[0])), uint(nparams), name)
    else
        return LLVMBuildCall( builder, func, null, 0u, name)
