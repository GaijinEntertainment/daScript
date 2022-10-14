options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module llvm_jit_common shared private

require llvm/llvm_boost
require daslib/ast_boost
require daslib/strings
require daslib/defer
require daslib/fio

let public LLVM_JIT_ENABLED = true

let public LLVM_DEBUG_RESULT = false
let public LLVM_DEBUG_EVERYTHING = false

let public LLVM_JIT_ALWAYS_SOLID = true

let public LLVM_ENABLE_OPT_PASS = LLVM_DEBUG_EVERYTHING ? false : true
let public LLVM_OPT_LEVEL = LLVM_DEBUG_EVERYTHING ? 0u : 2u
let public LLVM_JIT_OPT_LEVEL = LLVM_DEBUG_EVERYTHING ? 0u : 2u
let public LLVM_LOG_RESULT = LLVM_DEBUG_EVERYTHING || LLVM_DEBUG_RESULT
let public LLVM_INLINE_THRESHOLD = 225u

let public FN_JIT_EXCEPTION = "`jit`exception`"
let public FN_JIT_CALL_OR_FASTCALL = "`jit`call_or_fastcall`"
let public FN_JIT_CALL_WITH_CMRES = "`jit`call_with_cmres`"
let public FN_JIT_STRING_BUILDER = "`jit`string_builder`"
let public FN_JIT_GET_GLOBAL_MNH = "`jit`get_global_mnh`"
let public FN_JIT_ALLOC_HEAP = "`jit`alloc_heap`"
let public FN_JIT_ALLOC_PERSISTENT = "`jit`alloc_persistent`"
let public FN_JIT_FREE_HEAP = "`jit`free_heap`"
let public FN_JIT_FREE_PERSISTENT = "`jit`free_persistent`"

struct JitEngine
    mod : LLVMOpaqueModule?
    engine : LLVMExecutionEngineRef
    builder : LLVMOpaqueBuilder?

var private g_all_engines : array<JitEngine>

var public g_mod : LLVMOpaqueModule?
var public g_engine : LLVMExecutionEngineRef
var public g_builder : LLVMOpaqueBuilder?
var public g_failed : bool = false
var public g_errors : array<string>

var g_str2v : table<string; LLVMOpaqueValue?>

[macro_function]
def public init_jit
    return if is_in_completion() || is_compiling_macros()
    return if g_builder != null    // prevent double init
    g_failed = false
    g_mod = LLVMModuleCreateWithName("llvm_jit_module")
    g_builder = LLVMCreateBuilder()
    LLVMLinkInMCJIT()
    LLVMInitializeNativeTarget()
    LLVMInitializeNativeAsmPrinter()
    LLVMInitializeNativeAsmParser()
    LLVMCreateJITCompilerForModule(g_engine, g_mod, LLVM_JIT_OPT_LEVEL)
    // add default functions
    var jit_exception = LLVMAddFunction(g_mod, FN_JIT_EXCEPTION,    // jit_exception(text,context *)
        LLVMFunctionType(LLVMVoidType(),
            [[LLVMTypeRef LLVMPointerType(LLVMInt8Type(),0u); LLVMPointerType(LLVMVoidType(),0u)]]))
    LLVMAddGlobalMapping(g_engine, jit_exception, get_jit_exception())

    var jit_call_or_fastcall = LLVMAddFunction(g_mod, FN_JIT_CALL_OR_FASTCALL, // jit_call_or_fastcall(func,args *,context *)
        LLVMFunctionType(LLVMFloat4Type(),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u); LLVMPointerType(LLVMVoidType(),0u)]]))
    LLVMAddGlobalMapping(g_engine, jit_call_or_fastcall, get_jit_call_or_fastcall())

    var jit_call_with_cmres = LLVMAddFunction(g_mod, FN_JIT_CALL_WITH_CMRES, // jit_call_with_cmres(func,args *,cmres *,context *)
        LLVMFunctionType(LLVMFloat4Type(),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u); LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMVoidType(),0u)]]))
    LLVMAddGlobalMapping(g_engine, jit_call_with_cmres, get_jit_call_with_cmres())

    var jit_string_builder = LLVMAddFunction(g_mod, FN_JIT_STRING_BUILDER, // jit_string_builder ( context *, node *, args * )
        LLVMFunctionType(LLVMPointerType(LLVMInt8Type(),0u),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u) ]]))
    LLVMAddGlobalMapping(g_engine, jit_string_builder, get_jit_string_builder())

    // TODO: set sideeffects of this function to none
    var jit_get_global_mnh = LLVMAddFunction(g_mod, FN_JIT_GET_GLOBAL_MNH,  // jit_get_global_mnh ( uint64_t mnh, context * )
        LLVMFunctionType(LLVMPointerType(LLVMVoidType(),0u),
            [[LLVMTypeRef LLVMInt64Type(); LLVMPointerType(LLVMVoidType(),0u) ]]))
    LLVMAddGlobalMapping(g_engine, jit_get_global_mnh, get_jit_get_global_mnh())

    var jit_alloc_heap = LLVMAddFunction(g_mod, FN_JIT_ALLOC_HEAP,  // jit_alloc_heap ( uint64_t mnh, context * )
        LLVMFunctionType(LLVMPointerType(LLVMVoidType(),0u),
            [[LLVMTypeRef LLVMInt32Type(); LLVMPointerType(LLVMVoidType(),0u) ]]))
    LLVMAddGlobalMapping(g_engine, jit_alloc_heap, get_jit_alloc_heap())

    var jit_alloc_persistent = LLVMAddFunction(g_mod, FN_JIT_ALLOC_PERSISTENT,  // jit_alloc_persistent ( uint64_t mnh, context * )
        LLVMFunctionType(LLVMPointerType(LLVMVoidType(),0u),
            [[LLVMTypeRef LLVMInt32Type(); LLVMPointerType(LLVMVoidType(),0u) ]]))
    LLVMAddGlobalMapping(g_engine, jit_alloc_persistent, get_jit_alloc_persistent())

    var jit_free_heap = LLVMAddFunction(g_mod, FN_JIT_FREE_HEAP,  // void jit_free_heap ( void * bytes, uint32_t size, Context * context )
        LLVMFunctionType(LLVMPointerType(LLVMVoidType(),0u),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMInt32Type(); LLVMPointerType(LLVMVoidType(),0u) ]]))
    LLVMAddGlobalMapping(g_engine, jit_free_heap, get_jit_free_heap())

    var jit_free_persistent = LLVMAddFunction(g_mod, FN_JIT_FREE_PERSISTENT,  // void jit_free_persistent ( void * bytes, Context * context )
        LLVMFunctionType(LLVMPointerType(LLVMVoidType(),0u),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMVoidType(),0u) ]]))
    LLVMAddGlobalMapping(g_engine, jit_free_persistent, get_jit_free_persistent())

[macro_function]
def public finalize_jit
    return if is_in_completion() || is_compiling_macros()
    return if g_builder == null    // prevent double finalize
    if LLVM_LOG_RESULT
        LLVMDumpModule(g_mod)
        fflush(fstderr())
    g_all_engines |> push([[ JitEngine
        mod <- g_mod,
        engine <- g_engine,
        builder <- g_builder
    ]])
    delete g_str2v

[finalize]
def private finalize_all_jit
    for je in g_all_engines
        LLVMDisposeExecutionEngine(je.engine); je.engine = null
        LLVMDisposeBuilder(je.builder); je.builder = null
        // TODO: figure out why this crashes
        // LLVMDisposeModule(je.mod); je.mod = null
    delete g_all_engines

def public build_string_constant ( message:string )
    if g_str2v |> key_exists(message)
        return g_str2v[message]
    let msg_size = uint(length(message))
    var strType = LLVMArrayType( LLVMInt8Type(), msg_size+1u )
    var str = LLVMAddGlobal(g_mod, strType, "")
    LLVMSetInitializer(str, LLVMConstString(message, msg_size, 0))
    LLVMSetGlobalConstant(str, 1)
    LLVMSetLinkage(str, LLVMLinkage LLVMPrivateLinkage)
    LLVMSetUnnamedAddress(str, LLVMUnnamedAddr LLVMGlobalUnnamedAddr)
    LLVMSetAlignment(str, 1u)
    g_str2v[message] = str
    return str

def public get_string_constant_ptr ( message:string )
    var str = build_string_constant(message)
    return LLVMBuildPointerCast(g_builder, str, LLVMPointerType(LLVMInt8Type(), 0u), "string_constant {message}")

def public failed ( msg:string )
    g_errors |> push("{msg}")

def public failed_E ( e:ExpressionPtr; msg:string )
    failed ( "{msg}. expr = {e.__rtti} at {describe(e.at)}")

def public failed_V ( v:VariablePtr; msg:string )
    failed ( "{msg}. var = {describe(v)} at {describe(v.at)}")

def public base_type_to_llvm_type ( t:Type )
    return LLVMInt32Type()  if t == Type tInt || t == Type tUInt || t == Type tBitfield
    return LLVMInt64Type()  if t == Type tInt64 || t == Type tUInt64
    return LLVMInt16Type()  if t == Type tInt16 || t == Type tUInt16
    return LLVMInt8Type()   if t == Type tInt8 || t == Type tUInt8
    return LLVMFloatType()  if t == Type tFloat
    return LLVMDoubleType() if t == Type tDouble
    return LLVMInt1Type()   if t == Type tBool
    return LLVMInt2Type()   if t == Type tInt2 || t == Type tUInt2 || t == Type tRange || t == Type tURange
    return LLVMInt3Type()   if t == Type tInt3 || t == Type tUInt3
    return LLVMInt4Type()   if t == Type tInt4 || t == Type tUInt4
    return LLVMFloat2Type() if t == Type tFloat2
    return LLVMFloat3Type() if t == Type tFloat3
    return LLVMFloat4Type() if t == Type tFloat4
    return LLVMPointerType(LLVMInt8Type(),0u) if t == Type tString
    return LLVMPointerType(LLVMVoidType(),0u) if t == Type tPointer
    failed("base_type_to_llvm_type: unsupported type {t}")
    return LLVMInt32Type()

def public type_to_llvm_abi_type ( t:TypeDeclPtr )
    if t.isRef
        return LLVMPointerType(type_to_llvm_type(t), 0u)
    elif t.isPointer
        return LLVMPointerType(t.firstType!=null ? type_to_llvm_type(t.firstType) : LLVMVoidType(), 0u)
    elif t.isWorkhorseType
        return base_type_to_llvm_type(t.baseType)
    elif t.isVoid
        return LLVMVoidType()
    elif t.baseType==Type fakeContext || t.baseType==Type fakeLineInfo
        return LLVMPointerType(LLVMVoidType(),0u)
    else
        failed("type_to_llvm_abi_type: unsupported type {describe(t)} at {t.at}")
        return LLVMInt32Type()

def public type_to_llvm_type ( t:TypeDeclPtr )
    var res : LLVMOpaqueType?
    if t.dim.length != 0
        var ndt <- clone_type(t); defer_delete(ndt)
        ndt.dim |> clear()
        res = type_to_llvm_type(ndt)
        for d in t.dim
            res = LLVMArrayType(res, uint(d))
    elif t.isPointer
        res = LLVMPointerType(t.firstType!=null ? type_to_llvm_type(t.firstType) : LLVMVoidType(), 0u)
    elif t.isWorkhorseType
        res = base_type_to_llvm_type(t.baseType)
    elif t.isStructure
        res = LLVMArrayType(LLVMInt8Type(), uint(t.sizeOf))
    elif t.isVoid
        res = LLVMVoidType()
    else
        failed("type_to_llvm_type: unsupported type {describe(t)} at {t.at}")
        res = LLVMInt32Type()
    return res

def public build_broadcast_vector ( opType:TypeDeclPtr; right:LLVMOpaqueValue? )
    if opType.isVectorType
        var vres = LLVMGetUndef(type_to_llvm_type(opType))
        for i in range(opType.vectorDim)
            vres = LLVMBuildInsertElement(g_builder, vres, right, LLVMConstInt(LLVMInt32Type(), uint64(i), 0), "")
        return vres
    else
        return right
