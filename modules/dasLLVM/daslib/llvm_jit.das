options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module llvm_jit shared private

require llvm/llvm_boost
require daslib/ast_boost
require daslib/templates_boost
require daslib/safe_addr
require daslib/strings_boost
require daslib/defer
require daslib/fio

let LLVM_DEBUG_RESULT = false
let LLVM_DEBUG_EVERYTHING = false

let LLVM_ENABLE_OPT_PASS = LLVM_DEBUG_EVERYTHING ? false : true
let LLVM_OPT_LEVEL = LLVM_DEBUG_EVERYTHING ? 0u : 2u
let LLVM_JIT_OPT_LEVEL = LLVM_DEBUG_EVERYTHING ? 0u : 2u
let LLVM_LOG_RESULT = LLVM_DEBUG_EVERYTHING || LLVM_DEBUG_RESULT

struct JitEngine
    mod : LLVMOpaqueModule?
    engine : LLVMExecutionEngineRef
    builder : LLVMOpaqueBuilder?

var private g_all_engines : array<JitEngine>

var g_mod : LLVMOpaqueModule?
var g_engine : LLVMExecutionEngineRef
var g_builder : LLVMOpaqueBuilder?
var g_failed : bool = false
var g_str2v : table<string; LLVMOpaqueValue?>

let FN_JIT_EXCEPTION = "`jit`exception`"
let FN_JIT_CALL_OR_FASTCALL = "`jit`call_or_fastcall`"
let FN_JIT_CALL_WITH_CMRES = "`jit`call_with_cmres`"
let FN_JIT_STRING_BUILDER = "`jit`string_builder`"
let FN_JIT_GET_GLOBAL_MNH = "`jit`get_global_mnh`"

def private get_expr_ptr ( expr:ExpressionPtr )
    return get_ptr(expr)

[macro_function]
def private init_jit
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
    var je = LLVMAddFunction(g_mod, FN_JIT_EXCEPTION,    // jit_exception(text,context *)
        LLVMFunctionType(LLVMVoidType(),
            [[LLVMTypeRef LLVMPointerType(LLVMInt8Type(),0u); LLVMPointerType(LLVMVoidType(),0u)]]))
    LLVMAddGlobalMapping(g_engine, je, get_jit_exception())

    var jc = LLVMAddFunction(g_mod, FN_JIT_CALL_OR_FASTCALL, // jit_call_or_fastcall(func,args *,context *)
        LLVMFunctionType(LLVMFloat4Type(),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u); LLVMPointerType(LLVMVoidType(),0u)]]))
    LLVMAddGlobalMapping(g_engine, jc, get_jit_call_or_fastcall())

    var jcm = LLVMAddFunction(g_mod, FN_JIT_CALL_WITH_CMRES, // jit_call_with_cmres(func,args *,cmres *,context *)
        LLVMFunctionType(LLVMFloat4Type(),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u); LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMVoidType(),0u)]]))
    LLVMAddGlobalMapping(g_engine, jcm, get_jit_call_with_cmres())

    var jsb = LLVMAddFunction(g_mod, FN_JIT_STRING_BUILDER, // jit_string_builder ( context *, node *, args * )
        LLVMFunctionType(LLVMPointerType(LLVMInt8Type(),0u),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u) ]]))
    LLVMAddGlobalMapping(g_engine, jsb, get_jit_string_builder())

    // TODO: set sideeffects of this function to none
    var jgm = LLVMAddFunction(g_mod, FN_JIT_GET_GLOBAL_MNH,  // jit_get_global_mnh ( uint64_t mnh, context * )
        LLVMFunctionType(LLVMPointerType(LLVMVoidType(),0u),
            [[LLVMTypeRef LLVMInt64Type(); LLVMPointerType(LLVMVoidType(),0u) ]]))
    LLVMAddGlobalMapping(g_engine, jgm, get_jit_get_global_mnh())


[macro_function]
def private finalize_jit
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

struct private IteBlock
    if_true  : LLVMOpaqueBasicBlock?
    if_false : LLVMOpaqueBasicBlock?
    if_end   : LLVMOpaqueBasicBlock?
    if_true_terminates  : bool
    if_false_terminates : bool

struct private LoopBlock
    loop_start : LLVMOpaqueBasicBlock?
    loop_body  : LLVMOpaqueBasicBlock?
    loop_end   : LLVMOpaqueBasicBlock?

class LlvmJitVisitor : AstVisitor
    adapter : smart_ptr<VisitorAdapter>
    e2v : table<Expression?;LLVMOpaqueValue?>
    v2v : table<Variable?;LLVMOpaqueValue?>
    call2cmres : table<Expression?;LLVMOpaqueValue?>
    errors : array<string>
    fnmna : string
    [[do_not_delete]] ffunc, wfunc : LLVMOpaqueValue?
    ite2blocks : table<Expression?; IteBlock>
    [[do_not_delete]] thisFunc : Function?
    subblocks : table<LLVMOpaqueBasicBlock?; LLVMOpaqueBasicBlock?>
    sbstack : array<LLVMOpaqueBasicBlock?>
    loop_stack : array<LoopBlock>
    [[do_not_delete]] local_stack : LLVMOpaqueValue?
    [[do_not_delete]] jit_context : Context?

    def LlvmJitVisitor ( ctx:Context? )
        jit_context = ctx

    def add_llvm_functions ( fun : FunctionPtr )
        thisFunc = get_ptr(fun)
        defer <|
            thisFunc = null
        fnmna = get_mangled_name(fun)
        // implementation function
        var fun_args <-  [{for a in fun.arguments; type_to_llvm_abi_type(a._type)}]
        fun_args |> push(LLVMPointerType(LLVMVoidType(), 0u))  // context
        var fun_type : LLVMOpaqueType?
        if isCMRES(fun)
            fun_args |> push(LLVMPointerType(LLVMVoidType(), 0u))  // CMRES
            fun_type = LLVMFunctionType(LLVMVoidType(), fun_args)
        else
            fun_type = LLVMFunctionType(type_to_llvm_abi_type(fun.result), fun_args)
        ffunc = LLVMAddFunction(g_mod, "{fnmna} implementation", fun_type)
        for a,ai in fun.arguments,urange(100500)
            if a._type.isRef
                LLVMSetParamAlignment(LLVMGetParam(ffunc,ai), uint(a._type.alignOf))
            elif a._type.isPointer && a._type.firstType!=null
                LLVMSetParamAlignment(LLVMGetParam(ffunc,ai), uint(a._type.firstType.alignOf))
        LLVMSetParamAlignment(get_context_param(), 16u)
        if isCMRES(fun)
            LLVMSetParamAlignment(get_cmres_param(), uint(fun.result.alignOf))
        // wrapper function
        var ret_type = LLVMFunctionType(LLVMFloat4Type(),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u);   // context
            LLVMPointerType(LLVMFloat4Type(),0u);               // args
            LLVMPointerType(LLVMVoidType(),0u)                  // cmres
        ]])
        wfunc = LLVMAddFunction(g_mod, fnmna, ret_type)
        LLVMSetParamAlignment(LLVMGetParam(wfunc, 0u), 16u)
        LLVMSetParamAlignment(LLVMGetParam(wfunc, 1u), 16u)
        if isCMRES(fun)
            LLVMSetParamAlignment(LLVMGetParam(wfunc, 2u), uint(fun.result.alignOf))

    def append_basic_block ( name:string; add_to_phi:bool=false )
        var blk = LLVMAppendBasicBlock(ffunc,name)
        subblocks[blk] = blk
        if add_to_phi
            for st in sbstack
                subblocks[st] = blk
        return blk

    def failed ( msg:string )
        errors |> push("{msg}")

    def failed_E ( e:ExpressionPtr; msg:string )
        failed ( "{msg}. expr = {e.__rtti} at {describe(e.at)}")

    def failed_V ( v:VariablePtr; msg:string )
        failed ( "{msg}. var = {describe(v)} at {describe(v.at)}")

    def setE ( e : ExpressionPtr; ov:LLVMOpaqueValue? )
        e2v[get_ptr(e)] = ov

    def getE ( e : ExpressionPtr )
        var res : LLVMOpaqueValue?
        e2v |> find_if_exists(get_ptr(e)) <| $ ( val )
            res = *val
        if res == null
            res = LLVMGetUndef(LLVMInt32Type())
            failed_E(e, "unresolved expression {describe(e)}")
        return res

    def setV ( e : VariablePtr; ov:LLVMOpaqueValue? )
        v2v[get_ptr(e)] = ov

    def getV ( e : VariablePtr )
        var res : LLVMOpaqueValue?
        v2v |> find_if_exists(get_ptr(e)) <| $ ( val )
            res = *val
        if res == null
            res = LLVMGetUndef(LLVMInt32Type())
            failed_V(e, "unresolved variable {describe(e)}")
        return res

    def tryGetV ( e : VariablePtr )
        var res : LLVMOpaqueValue?
        v2v |> find_if_exists(get_ptr(e)) <| $ ( val )
            res = *val
        return res

    def base_type_to_llvm_type ( t:Type )
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

    def type_to_llvm_abi_type ( t:TypeDeclPtr )
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

    def type_to_llvm_type ( t:TypeDeclPtr )
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

    def override preVisitFunction(fun:FunctionPtr) : void
        assert(g_builder!=null, "missing builder")
        thisFunc = get_ptr(fun)
        fnmna = get_mangled_name(fun)
        ffunc = LLVMGetNamedFunction(g_mod, "{fnmna} implementation")
        wfunc = LLVMGetNamedFunction(g_mod, fnmna)
        // build wrapper function
        var wentry = LLVMAppendBasicBlock(wfunc, "entry")
        LLVMPositionBuilderAtEnd(g_builder, wentry)
        var wargs : array<LLVMOpaqueValue?>
        var args = LLVMGetParam(wfunc, 1u)
        for a,ai in fun.arguments, range(100500)
            var arg_ptr = LLVMBuildGEP(g_builder, args, LLVMConstInt(LLVMInt32Type(), uint64(ai), 0), "arg_ptr_{ai}")
            var arg_v : LLVMOpaqueValue?
            if a._type.flags.ref
                arg_v = LLVMBuildLoad2(g_builder, type_to_llvm_abi_type(a._type), arg_ptr, "arg_ref_value_{ai}")
            elif !a._type.isRef
                arg_v = LLVMBuildLoad2(g_builder, type_to_llvm_abi_type(a._type), arg_ptr, "arg_value_{ai}")
            else
                // arg_v = LLVMBuildPointerCast(g_builder, arg_ptr, LLVMPointerType(LLVMVoidType(),0u), "arg_ptr_{ai}" )
                arg_v = LLVMBuildLoad2(g_builder, type_to_llvm_abi_type(a._type), arg_ptr, "arg_any_{ai}")
            wargs |> push(arg_v)
        wargs |> push(LLVMGetParam(wfunc, 0u))  // context
        if isCMRES(fun)
            wargs |> push(LLVMGetParam(wfunc, 2u))  // cmres
        var ret_v = LLVMBuildCall(g_builder, ffunc, wargs, "")
        if !fun.result.isVoid
            if isCMRES(fun)
                LLVMBuildRet(g_builder, cast_ptr_to_vec4f(LLVMGetParam(wfunc,2u)))
            else
                var tres = cast_to_vec4f(fun.result, ret_v)
                if tres != null
                    LLVMBuildRet(g_builder, tres)
                else
                    LLVMBuildRet(g_builder, LLVMGetUndef(LLVMFloat4Type()))
        else
            LLVMBuildRet(g_builder, LLVMGetUndef(LLVMFloat4Type()))
        // now write to regular function
        var entry = LLVMAppendBasicBlock(ffunc, "entry")
        LLVMPositionBuilderAtEnd(g_builder, entry)
        // allocate local stack
        if uint(fun.totalStackSize) != SIZE_OF_PROLOGUE
            local_stack = LLVMBuildAlloca(g_builder, LLVMArrayType(LLVMInt8Type(), uint(fun.totalStackSize)), "")
            LLVMSetAlignment(local_stack, 16u)
            local_stack = LLVMBuildPointerCast(g_builder, local_stack, LLVMPointerType(LLVMInt8Type(),0u), "LOCAL_STACK")
        else
            local_stack = null
        // allocate arguments, which need to be promoted to local variables
        for a,ai in fun.arguments,range(10050)
            if (a.access_flags.access_ref || a.access_flags.access_pass) && !a._type.isRef
                var tryV = LLVMBuildAlloca(g_builder, type_to_llvm_abi_type(a._type), "var_{a.name}")
                LLVMSetAlignment(tryV, 16u)
                LLVMBuildStore(g_builder, LLVMGetParam(ffunc, uint(ai)), tryV)
                setV(a, tryV)
        // go through all used global variables, and access them
        get_use_global_variables(fun) <| $ ( variable )
            if !variable._module.moduleFlags.isSolidContext
                let mangled_name = get_mangled_name(variable)
                let MNH = hash(mangled_name)
                var params = [[auto
                    LLVMConstInt(LLVMInt64Type(), MNH, 0);         // mnh
                    get_context_param()     // context
                ]]
                var vptr = LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, FN_JIT_GET_GLOBAL_MNH), params, "")
                vptr = LLVMBuildPointerCast(g_builder, vptr, LLVMPointerType(type_to_llvm_type(variable._type),0u), "global_var_{variable.name}")
                setV(variable, vptr)

    def override visitFunction(fun:FunctionPtr) : FunctionPtr
        if fun.body != null && !is_terminator_expression(fun.body)
            LLVMBuildRetVoid(g_builder)
        ffunc = null
        thisFunc = null
        return fun

    def isCall2CMRES ( expr:ExpressionPtr )
        return (expr is ExprCall) && isCMRES((expr as ExprCall).func)

    def make_call_to_cmres ( expr:ExpressionPtr; v_ptr:LLVMOpaqueValue? )
        if isCall2CMRES(expr)
            call2cmres[get_ptr(expr)] = LLVMBuildPointerCast(g_builder, v_ptr, LLVMPointerType(LLVMVoidType(),0u), "call_result_{(expr as ExprCall).func.name}")
            return true
        return false

    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : void
        var vtype = arg._type.flags.ref ?  LLVMPointerType(type_to_llvm_type(arg._type),0u) : type_to_llvm_type(arg._type)
        var v_ptr : LLVMOpaqueValue?
        if arg.flags.aliasCMRES
            v_ptr = get_cmres_param()
            v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, LLVMPointerType(vtype,0u), "variable_cmres_{arg.name}")
        else
            v_ptr = LLVMBuildGEP(g_builder, local_stack, LLVMConstInt(LLVMInt32Type(), uint64(arg.stackTop), 0), "")
            v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, LLVMPointerType(vtype,0u), "variable_{arg.name}")
        setV(arg, v_ptr)
        if arg.init == null
            var vsize = arg._type.flags.ref ?  typeinfo(sizeof type<void?>) : arg._type.sizeOf
            LLVMBuildMemSet(g_builder, v_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(vsize), 0), 16u)
        elif make_call_to_cmres(arg.init, v_ptr)
            pass    // do nothing

    def override visitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
        var tdst = getV(arg)
        if isCall2CMRES(expr)
            pass    // this variable is initialized via cmres call
        elif arg._type.flags.ref
            var src = getE(expr)
            LLVMBuildStore(g_builder, src, tdst)
        else
            build_copy(tdst, expr)
        return expr

    def override visitExprVar(expr:smart_ptr<ExprVar>) : ExpressionPtr
        assume varType = expr.variable._type
        if expr.varFlags._block
            failed_E(expr, "{expr.variable.name} support block variable")
        elif expr.varFlags.local    // local varaibels
            var var_ptr = getV(expr.variable)
            if expr.variable.flags.aliasCMRES
                pass
            if varType.flags.ref
                var_ptr = LLVMBuildLoad(g_builder, var_ptr, "")
            if expr.varFlags.r2v
                assert(expr._type.isWorkhorseType,"r2v only for workhorse types")
                setE(expr, LLVMBuildLoad(g_builder, var_ptr, "var_r2v_{expr.variable.name}"))
            else
                setE(expr, var_ptr)
        elif expr.varFlags.argument // arguments
            var tryV = tryGetV(expr.variable)
            if tryV != null
                if expr.varFlags.r2v
                    assert(expr._type.isWorkhorseType,"r2v only for workhorse types")
                    setE(expr, LLVMBuildLoad(g_builder, tryV, "var_r2v_{expr.variable.name}"))
                else
                    setE(expr, tryV)
                return expr
            var arg = LLVMGetParam(ffunc, uint(expr.argumentIndex))
            if expr.varFlags.r2v
                assert(expr._type.isWorkhorseType,"r2v only for workhorse types")
                if varType.flags.ref
                    setE(expr, LLVMBuildLoad(g_builder, arg, "var_ref_r2v_{expr.variable.name}"))
                else
                    setE(expr, arg)
            else
                setE(expr, arg)
        else
            var vptr : LLVMOpaqueValue?
            if expr.variable._module.moduleFlags.isSolidContext
                var ctx_ptr = LLVMBuildPointerCast(g_builder, get_context_param(), LLVMPointerType(LLVMInt8Type(),0u), "")
                var global_ptr = LLVMBuildGEP(g_builder, ctx_ptr, LLVMConstInt(LLVMInt32Type(), uint64(CONTEXT_OFFSET_OF_GLOBALS), 0), "")
                global_ptr = LLVMBuildLoad2(g_builder, LLVMPointerType(LLVMInt8Type(),0u), global_ptr, "")
                vptr = LLVMBuildGEP(g_builder, global_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.variable.stackTop), 0), "")
                vptr = LLVMBuildPointerCast(g_builder, vptr, LLVMPointerType(type_to_llvm_type(expr.variable._type),0u), "global_var_{expr.variable.name}")
            else
                vptr = getV(expr.variable)
            if expr.varFlags.r2v
                setE(expr, LLVMBuildLoad(g_builder, vptr, "global_var_r2v_{expr.variable.name}"))
            else
                setE(expr, vptr)
        return expr

    def cast_from_vec4f ( expr:ExpressionPtr; etype:TypeDeclPtr; v4f:LLVMOpaqueValue? )
        var tres : LLVMOpaqueValue?
        if etype.isWorkhorseType
            let bt = etype.baseType
            if bt==Type tInt || bt==Type tUInt || bt==Type tBitfield
                var tbit = LLVMBuildExtractElement(g_builder, v4f, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "cast_from_vec4f_bits")
                tres = LLVMBuildBitCast(g_builder, tbit, LLVMInt32Type(), "cast_from_vec4f")
            elif bt == Type tFloat4
                return v4f
            elif bt == Type tBool
                var tbit = LLVMBuildExtractElement(g_builder, v4f, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "cast_from_vec4f_bits")
                tres = LLVMBuildBitCast(g_builder, tbit, LLVMInt32Type(), "cast_from_vec4f")
                tres = LLVMBuildTruncOrBitCast(g_builder, tres, LLVMInt1Type(), "down_to_1_bit")
            elif bt == Type tString
                if typeinfo(sizeof type<void?>)==8
                    var v2 = LLVMBuildBitCast(g_builder, v4f, LLVMVectorType(LLVMInt64Type(),2u), "ptr_vec")
                    var tbit = LLVMBuildExtractElement(g_builder, v2, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "ptr_bits")
                    tres = LLVMBuildIntToPtr(g_builder, tbit, LLVMPointerType(LLVMInt8Type(),0u), "cast_from_vec4f")
                else
                    var tbit = LLVMBuildExtractElement(g_builder, v4f, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "cast_from_vec4f_bits")
                    tres = LLVMBuildBitCast(g_builder, tbit, LLVMInt32Type(), "ptr_bits")
                    tres = LLVMBuildIntToPtr(g_builder, tres, LLVMPointerType(LLVMInt8Type(),0u), "cast_from_vec4f")
            else
                failed_E(expr, "unsupported cast_from_vec4f cast {describe(etype)}")
        else
            failed_E(expr, "unsupported cast_from_vec4f type {describe(etype)}")
        return tres

    def cast_ptr_to_vec4f ( tsrc:LLVMOpaqueValue? )
        static_if typeinfo(sizeof type<void?>)==8
            var vvec = LLVMGetUndef(LLVMVectorType(LLVMInt64Type(), 2u))
            var tval = LLVMBuildPtrToInt(g_builder, tsrc, LLVMInt64Type(), "")
            var vsrc = LLVMBuildInsertElement(g_builder, vvec, tval, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "")
            return LLVMBuildBitCast(g_builder, vsrc, LLVMFloat4Type(), "ptr_to_vec4f")
        else
            var tval = LLVMBuildPtrToInt(g_builder, tsrc, LLVMInt32Type(), "")
            var tbit = LLVMBuildBitCast(g_builder, tval, LLVMFloatType(), "")
            return LLVMBuildInsertElement(g_builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "ptr_to_vec4f")

    def cast_to_vec4f ( etype:TypeDeclPtr; tsrc:LLVMOpaqueValue? )
        var vres = LLVMGetUndef(LLVMFloat4Type())
        var tres : LLVMOpaqueValue?
        if etype.isRef
            tres = cast_ptr_to_vec4f(tsrc)
        elif etype.isWorkhorseType
            let bt = etype.baseType
            if bt==Type tInt || bt==Type tUInt || bt==Type tBitfield
                var tbit = LLVMBuildBitCast(g_builder, tsrc, LLVMFloatType(), "tbit")
                tres = LLVMBuildInsertElement(g_builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "return_result")
            elif bt==Type tBool || bt==Type tInt8 || bt==Type tInt16 || bt==Type tUInt8 || bt==Type tUInt16
                var tibit = LLVMBuildZExtOrBitCast(g_builder, tsrc, LLVMInt32Type(), "int_bit")
                var tbit = LLVMBuildBitCast(g_builder, tibit, LLVMFloatType(), "tbit")
                tres = LLVMBuildInsertElement(g_builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "return_result")
            elif bt==Type tInt64 || bt==Type tUInt64
                var vvec = LLVMGetUndef(LLVMVectorType(LLVMInt64Type(), 2u))
                var vsrc = LLVMBuildInsertElement(g_builder, vvec, tsrc, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "int64_vec")
                tres = LLVMBuildBitCast(g_builder, vsrc, LLVMFloat4Type(), "return_result")
            elif bt==Type tFloat
                tres = LLVMBuildInsertElement(g_builder, vres, tsrc, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "return_result")
            elif bt==Type tDouble
                var vvec = LLVMGetUndef(LLVMVectorType(LLVMDoubleType(), 2u))
                var vsrc = LLVMBuildInsertElement(g_builder, vvec, tsrc, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "double_vec")
                tres = LLVMBuildBitCast(g_builder, vsrc, LLVMFloat4Type(), "return_result")
            elif bt==Type tInt2 || bt==Type tUInt2 || bt==Type tRange || bt==Type tURange
                var tbit = LLVMBuildShuffleVector(g_builder, tsrc, tsrc, [{auto 0;1;-1;-1}], "int2_vec")
                tres = LLVMBuildBitCast(g_builder, tbit, LLVMFloat4Type(), "return_result")
            elif bt==Type tInt3 || bt==Type tUInt3
                var tbit = LLVMBuildShuffleVector(g_builder, tsrc, tsrc, [{auto 0;1;2;-1}], "int3_vec")
                tres = LLVMBuildBitCast(g_builder, tbit, LLVMFloat4Type(), "return_result")
            elif bt==Type tInt4 || bt==Type tUInt4
                tres = LLVMBuildBitCast(g_builder, tsrc, LLVMFloat4Type(), "return_result")
            elif bt==Type tFloat2
                tres = LLVMBuildShuffleVector(g_builder, tsrc, tsrc, [{auto 0;1;-1;-1}], "return_result")
            elif bt==Type tFloat3
                tres = LLVMBuildShuffleVector(g_builder, tsrc, tsrc, [{auto 0;1;2;-1}], "return_result")
            elif bt==Type tFloat4
                tres = tsrc
            elif bt==Type tString || bt==Type tPointer
                tres = cast_ptr_to_vec4f(tsrc)
            else
                failed("unsupported workhorse cast_to_vec4f cast {describe(etype)}")
        else
            failed("unsupported cast_to_vec4f type {describe(etype)}")
        return tres

    def override preVisitExprReturn(expr:smart_ptr<ExprReturn>)
        if isCMRES(thisFunc) && expr.returnFlags.returnCallCMRES
            make_call_to_cmres(expr.subexpr, get_cmres_param())

    def override visitExprReturn(expr:smart_ptr<ExprReturn>) : ExpressionPtr
        if !thisFunc.result.isVoid
            if isCMRES(thisFunc)
                if !expr.returnFlags.returnCMRES
                    build_copy(get_cmres_param(),expr.subexpr)
                LLVMBuildRetVoid(g_builder)
            else
                LLVMBuildRet(g_builder, getE(expr.subexpr))
        else
            LLVMBuildRetVoid(g_builder)
        return expr

    def build_string_constant ( message:string )
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

    def get_string_constant_ptr ( message:string )
        var str = build_string_constant(message)
        return LLVMBuildPointerCast(g_builder, str, LLVMPointerType(LLVMInt8Type(), 0u), "string_constant {message}")

    def get_context_param : LLVMOpaqueValue?
        return LLVMGetParam(ffunc,uint(thisFunc.arguments.length))

    def get_cmres_param : LLVMOpaqueValue?
        return LLVMGetParam(ffunc,uint(thisFunc.arguments.length+1))

    def build_exception ( message:string )
        var params = [[auto
            get_string_constant_ptr(message);   // message
            get_context_param()                 // context
        ]]
        LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, FN_JIT_EXCEPTION), params, "")

    def build_copy ( tdst:LLVMOpaqueValue?; right:ExpressionPtr )
        var tsrc = getE(right)
        if right._type.isRef
            var rsize = LLVMConstInt(LLVMInt32Type(), uint64(right._type.sizeOf), 0)
            var ralign = uint(right._type.alignOf)
            LLVMBuildMemCpy(g_builder, tdst, ralign, tsrc, ralign, rsize)
        else
            LLVMBuildStore(g_builder, tsrc, tdst)

    def override preVisitExprCopyRight(expr:smart_ptr<ExprCopy>;right:ExpressionPtr): void
        var tdst = getE(expr.left)
        make_call_to_cmres(expr.right, tdst)

    def override visitExprCopy(expr:smart_ptr<ExprCopy>) : ExpressionPtr
        if isCall2CMRES(expr.right)
            pass    // already a call to cmres, no need to copy
        else
            var tdst = getE(expr.left)
            build_copy(tdst, expr.right)
        return expr

    def check_ptr_zero ( tsrc:LLVMOpaqueValue? )
        var check_null_ptr = append_basic_block("check_null_ptr")
        var check_true = append_basic_block("check_true")
        var check_end = append_basic_block("check_end")
        LLVMBuildBr(g_builder, check_null_ptr)
        LLVMPositionBuilderAtEnd(g_builder, check_null_ptr)
        var cond = LLVMBuildIsNull(g_builder, tsrc, "null_check")
        LLVMBuildCondBr(g_builder, cond, check_true, check_end)
        LLVMPositionBuilderAtEnd(g_builder, check_true)
        build_exception("dereferencing null pointer")
        LLVMBuildBr(g_builder, check_end)
        LLVMPositionBuilderAtEnd(g_builder, check_end)

    def override visitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>) : ExpressionPtr
        var tsrc = getE(expr.subexpr)
        check_ptr_zero(tsrc)
        setE(expr, tsrc)
        return expr

    def check_range ( tidx, maxIdx:LLVMOpaqueValue? )
        var check_null_ptr = append_basic_block("check_dim_range")
        var check_true = append_basic_block("check_true")
        var check_end = append_basic_block("check_end")
        LLVMBuildBr(g_builder, check_null_ptr)
        LLVMPositionBuilderAtEnd(g_builder, check_null_ptr)
        var cond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntUGE, tidx, maxIdx, "range_check")
        LLVMBuildCondBr(g_builder, cond, check_true, check_end)
        LLVMPositionBuilderAtEnd(g_builder, check_true)
        build_exception("dim index out of range")
        LLVMBuildBr(g_builder, check_end)
        LLVMPositionBuilderAtEnd(g_builder, check_end)

    def override visitExprAt(expr:smart_ptr<ExprAt>) : ExpressionPtr
        var tsrc = getE(expr.subexpr)
        var tidx = getE(expr.index)
        assume subExprT = expr.subexpr._type
        if subExprT.isPointer
            check_ptr_zero(tsrc)
            var element_type = type_to_llvm_type(subExprT.firstType)
            var ptr_idx = LLVMBuildGEP(g_builder, tsrc, tidx, "ptr_at")
            if expr.atFlags.r2v
                var at_r2v = LLVMBuildLoad(g_builder, ptr_idx, "at_r2v")
                setE(expr, at_r2v)
            else
                setE(expr, ptr_idx)
        elif subExprT.dim.length != 0
            var etype <- clone_type(subExprT); defer_delete(etype)
            let maxIndex = etype.dim[etype.dim.length-1]
            etype.dim |> pop()
            var element_type = type_to_llvm_type(etype)
            check_range(tidx, LLVMConstInt(LLVMInt32Type(), uint64(maxIndex), 0))
            var ptr_idx = LLVMBuildGEP2(g_builder, element_type, tsrc, tidx, "dim_at")
            if expr.atFlags.r2v
                var at_r2v = LLVMBuildLoad(g_builder, ptr_idx, "dim_at_r2v")
                setE(expr, at_r2v)
            else
                setE(expr, ptr_idx)
        else
            failed("unsupported visitExprAt type {describe(subExprT)}")
        return expr

    def isReadModifyWriteOp2 ( name:das_string )
        return (name=="+=" || name=="-=" || name=="*=" || name=="/=" || name=="%=" ||
            name=="&=" || name=="|=" || name=="^=" || name=="<<=" || name==">>=")

    def override visitExprOp1(expr:smart_ptr<ExprOp1>) : ExpressionPtr
        var sexpr = getE(expr.subexpr)
        assume opType = expr.subexpr._type
        if expr.op=="+++" || expr.op=="++"
            var expr_t = base_type_to_llvm_type(opType.baseType)
            var sexpr_v = LLVMBuildLoad(g_builder, sexpr, "")
            var res_v : LLVMOpaqueValue?
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                res_v = LLVMBuildAdd(g_builder, sexpr_v, LLVMConstInt(expr_t,1ul,0), "")
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                res_v = LLVMBuildFAdd(g_builder, sexpr_v, LLVMConstReal(expr_t,1.lf), "")
            else
                failed_E(expr, "unsupported ++ type {opType}")
            LLVMBuildStore(g_builder, res_v, sexpr)
            setE(expr, sexpr_v)
        elif expr.op=="---" || expr.op=="--"
            var expr_t = base_type_to_llvm_type(opType.baseType)
            var sexpr_v = LLVMBuildLoad(g_builder, sexpr, "")
            var res_v : LLVMOpaqueValue?
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                res_v = LLVMBuildSub(g_builder, sexpr_v, LLVMConstInt(expr_t,1ul,0), "")
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                res_v = LLVMBuildFSub(g_builder, sexpr_v, LLVMConstReal(expr_t,1.lf), "")
            else
                failed_E(expr, "unsupported -- type {opType}")
            LLVMBuildStore(g_builder, res_v, sexpr)
            setE(expr, sexpr_v)
        else
            failed_E(expr, "unsupported unary operator {expr.op}")
        return expr

    def check_divide_by_0 ( right:LLVMOpaqueValue?; opType:TypeDeclPtr )
        var check_div_0 = append_basic_block("check_div_0")
        var check_true = append_basic_block("check_true")
        var check_end = append_basic_block("check_end")
        LLVMBuildBr(g_builder, check_div_0)
        LLVMPositionBuilderAtEnd(g_builder, check_div_0)
        let zero =  LLVMConstInt(base_type_to_llvm_type(opType.baseType), 0ul, 0)
        let cond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, right, zero, "cmp_is_zero")
        LLVMBuildCondBr(g_builder, cond, check_true, check_end)
        LLVMPositionBuilderAtEnd(g_builder, check_true)
        build_exception("division by zero")
        LLVMBuildBr(g_builder, check_end)
        LLVMPositionBuilderAtEnd(g_builder, check_end)

    def expand_scalar ( scalar:LLVMOpaqueValue?; vecType:TypeDeclPtr )
        var vec = LLVMGetUndef(type_to_llvm_type(vecType))
        for d in range(vecType.vectorDim)
            var idx = LLVMConstInt(LLVMInt32Type(), uint64(d), 0)
            vec = LLVMBuildInsertElement(g_builder, vec, scalar, idx, "")
        return vec

    def override visitExprOp2(expr:smart_ptr<ExprOp2>) : ExpressionPtr
        var left = getE(expr.left)
        var right = getE(expr.right)
        assume opType = expr.right._type
        var r2v_left = isReadModifyWriteOp2(expr.op) ? LLVMBuildLoad(g_builder, left, "") : null

        // note: we need to handle vector vs scalar operations here
        // the way we do it, is we expand scalar up to vector type
        if expr.op=="*" || expr.op=="*=" || expr.op=="/" || expr.op=="/="
            if expr.left._type.baseType != expr.right._type.baseType // vec * scalar || scalar * vec, and so on
                if opType.isVectorType
                    assert(expr.op=="*" || expr.op=="/")    // scalar * vec || scalar / vec only
                    left = expand_scalar(left, opType)
                else
                    right = expand_scalar(right, expr.left._type)

        if expr.op=="+"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildAdd(g_builder, left, right, ""))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFAdd(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported + type {opType}")
        elif expr.op=="+="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildAdd(g_builder, r2v_left, right, ""), left))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildFAdd(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported += type {opType}")
        elif expr.op=="-"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildSub(g_builder, left, right, ""))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFSub(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported - type {opType}")
        elif expr.op=="-="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildSub(g_builder, r2v_left, right, ""), left))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildFSub(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported -= type {opType}")
        elif expr.op=="*"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildMul(g_builder, left, right, ""))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFMul(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported * type {opType}")
        elif expr.op=="*="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildMul(g_builder, r2v_left, right, ""), left))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildFMul(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported *= type {opType}")
        elif expr.op=="/" || expr.op=="%" || expr.op=="/=" || expr.op=="%="
            // for integer division we check division by zero
            if opType.isInteger && expr.left._type.isInteger
                check_divide_by_0(right, opType)
            if expr.op=="/"
                if opType.isSignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildSDiv(g_builder, left, right, ""))
                elif opType.isUnsignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildUDiv(g_builder, left, right, ""))
                elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildFDiv(g_builder, left, right, ""))
                else
                    failed_E(expr, "unsupported /= type {opType}")
            elif expr.op=="/="
                if opType.isSignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildSDiv(g_builder, r2v_left, right, ""), left))
                elif opType.isUnsignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildUDiv(g_builder, r2v_left, right, ""), left))
                elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildFDiv(g_builder, r2v_left, right, ""), left))
                else
                    failed_E(expr, "unsupported /= type {opType}")
            elif expr.op=="%"
                if opType.isSignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildSRem(g_builder, left, right, ""))
                elif opType.isUnsignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildURem(g_builder, left, right, ""))
                elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildFRem(g_builder, left, right, ""))
                else
                    failed_E(expr, "unsupported % type {opType}")
            elif expr.op=="%="
                if opType.isSignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildSRem(g_builder, r2v_left, right, ""), left))
                elif opType.isUnsignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildURem(g_builder, r2v_left, right, ""), left))
                elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildFRem(g_builder, r2v_left, right, ""), left))
                else
                    failed_E(expr, "unsupported %= type {opType}")
            else
                failed_E(expr, "unsupported division type operator {expr.op}")
        elif expr.op=="<" || expr.op==">" || expr.op=="<=" || expr.op==">=" || expr.op=="==" || expr.op=="!="
            var opR : LLVMOpaqueValue?
            if expr.left._type.isSignedInteger
                opR = LLVMBuildICmp(g_builder, get_int_compare_op(expr,string(expr.op)), left, right, "")
            elif expr.left._type.isUnsignedInteger
                opR = LLVMBuildICmp(g_builder, get_uint_compare_op(expr,string(expr.op)), left, right, "")
            elif expr.left._type.isFloatOrDouble
                opR = LLVMBuildFCmp(g_builder, get_float_compare_op(expr,string(expr.op)), left, right, "")
            elif expr.left._type.baseType == Type tBool
                opR = LLVMBuildICmp(g_builder, get_bool_compare_op(expr,string(expr.op)), left, right, "")
            else
                failed_E(expr, "unsupported compare type {opType}")
            if opR == null
                opR = LLVMGetUndef(LLVMInt8Type())
                failed_E(expr, "unsupported cmp type {expr.left._type} {expr.op} {expr.right._type}")
            setE(expr, opR)
        else
            failed_E(expr, "unsupported binary operator {expr.op}")
        return expr

    def override visitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : ExpressionPtr
        assert(expr._type.isWorkhorseType)
        setE(expr, LLVMBuildLoad(g_builder, getE(expr.subexpr), "r2v"))
        return expr

    def get_bool_compare_op ( e:ExpressionPtr; op:string )
        return LLVMIntPredicate LLVMIntEQ   if op=="=="
        return LLVMIntPredicate LLVMIntNE   if op=="!="
        failed_E(e, "unsupported bool compare operator {op}")
        return LLVMIntPredicate LLVMIntEQ

    def get_int_compare_op ( e:ExpressionPtr; op:string )
        return LLVMIntPredicate LLVMIntEQ   if op=="=="
        return LLVMIntPredicate LLVMIntNE   if op=="!="
        return LLVMIntPredicate LLVMIntSLT  if op=="<"
        return LLVMIntPredicate LLVMIntSLE  if op=="<="
        return LLVMIntPredicate LLVMIntSGT  if op==">"
        return LLVMIntPredicate LLVMIntSGE  if op==">="
        failed_E(e, "unsupported int compare operator {op}")
        return LLVMIntPredicate LLVMIntEQ

    def get_uint_compare_op ( e:ExpressionPtr; op:string )
        return LLVMIntPredicate LLVMIntEQ   if op=="=="
        return LLVMIntPredicate LLVMIntNE   if op=="!="
        return LLVMIntPredicate LLVMIntULT  if op=="<"
        return LLVMIntPredicate LLVMIntULE  if op=="<="
        return LLVMIntPredicate LLVMIntUGT  if op==">"
        return LLVMIntPredicate LLVMIntUGE  if op==">="
        failed_E(e, "unsupported uint compare operator {op}")
        return LLVMIntPredicate LLVMIntEQ

    def get_float_compare_op ( e:ExpressionPtr; op:string )
        return LLVMRealPredicate LLVMRealOEQ    if op=="=="
        return LLVMRealPredicate LLVMRealONE    if op=="!="
        return LLVMRealPredicate LLVMRealOLT    if op=="<"
        return LLVMRealPredicate LLVMRealOLE    if op=="<="
        return LLVMRealPredicate LLVMRealOGT    if op==">"
        return LLVMRealPredicate LLVMRealOGE    if op==">="
        failed_E(e, "unsupported float compare operator {op}")
        return LLVMRealPredicate LLVMRealOEQ

// if-then-else
    def override preVisitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>) : void
        var before_if = append_basic_block("if_cond_at_{int(expr.at.line)}")
        LLVMBuildBr(g_builder, before_if)
        LLVMPositionBuilderAtEnd(g_builder, before_if)
        let itt = is_terminator_or_break_expression(expr.if_true)
        let ift = expr.if_false != null ? is_terminator_or_break_expression(expr.if_false) : false
        var blk = [[IteBlock
            if_true  = append_basic_block("if_true_at_line_{int(expr.at.line)}"),
            if_false = append_basic_block("if_false_at_line_{int(expr.at.line)}"),
            if_end   = (!itt || !ift) ? append_basic_block("if_end_at_line_{int(expr.at.line)}") : null,
            if_true_terminates = itt,
            if_false_terminates = ift
        ]]
        ite2blocks[get_expr_ptr(expr)] = blk
    def override preVisitExprIfThenElseIfBlock(expr:smart_ptr<ExprIfThenElse>;ifBlock:ExpressionPtr) : void
        var blk = ite2blocks[get_expr_ptr(expr)]                        // we start if_true block
        LLVMBuildCondBr(g_builder, getE(expr.cond), blk.if_true, blk.if_false)
        LLVMPositionBuilderAtEnd(g_builder, blk.if_true)
    def override preVisitExprIfThenElseElseBlock(expr:smart_ptr<ExprIfThenElse>;elseBlock:ExpressionPtr) : void
        var blk = ite2blocks[get_expr_ptr(expr)]
        if !blk.if_true_terminates
            LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_true block we jump to if_end
        LLVMPositionBuilderAtEnd(g_builder, blk.if_false)
    def override visitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>) : ExpressionPtr
        var blk = ite2blocks[get_expr_ptr(expr)]
        if expr.if_false == null
            if !blk.if_true_terminates
                LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_true block we jump to if_end
            LLVMPositionBuilderAtEnd(g_builder, blk.if_false)
        if !blk.if_false_terminates
            LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_false block we jump to if_end
        if blk.if_end != null
            LLVMPositionBuilderAtEnd(g_builder, blk.if_end)
        return expr

// op3
    def override preVisitExprOp3(expr:smart_ptr<ExprOp3>): void
        var before_if = append_basic_block("op3_cond_at_{expr.at.line}")
        LLVMBuildBr(g_builder, before_if)
        LLVMPositionBuilderAtEnd(g_builder, before_if)
        var blk = [[IteBlock
            if_true  = append_basic_block("op3_true_at_line_{int(expr.at.line)}"),
            if_false = append_basic_block("op3_false_at_line_{int(expr.at.line)}"),
            if_end   = append_basic_block("op3_end_at_line_{int(expr.at.line)}", true),
            if_true_terminates = false,
            if_false_terminates = false
        ]]
        ite2blocks[get_expr_ptr(expr)] = blk
    def override preVisitExprOp3Left(expr:smart_ptr<ExprOp3>;left:ExpressionPtr): void
        var blk = ite2blocks[get_expr_ptr(expr)]                        // we start if_true block
        LLVMBuildCondBr(g_builder, getE(expr.subexpr), blk.if_true, blk.if_false)
        LLVMPositionBuilderAtEnd(g_builder, blk.if_true)
        sbstack |> push(blk.if_true)
    def override preVisitExprOp3Right(expr:smart_ptr<ExprOp3>;right:ExpressionPtr): void
        var blk = ite2blocks[get_expr_ptr(expr)]
        LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_true block we jump to if_end
        LLVMPositionBuilderAtEnd(g_builder, blk.if_false)
        sbstack |> pop()
        sbstack |> push(blk.if_false)
    def override visitExprOp3(expr:smart_ptr<ExprOp3>) : ExpressionPtr
        var blk = ite2blocks[get_expr_ptr(expr)]
        LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)
        LLVMPositionBuilderAtEnd(g_builder, blk.if_end)
        sbstack |> pop()
        var res = LLVMBuildPhi(g_builder, type_to_llvm_abi_type(expr._type), "op3")
        var phi_val = [[auto
            getE(expr.left);
            getE(expr.right)
        ]]
        var phi_blk = [[auto
            subblocks[blk.if_true];
            subblocks[blk.if_false]
        ]]
        LLVMAddIncoming(res, phi_val, phi_blk)
        setE(expr, res)
        return expr

// function calls

    def intrinsic_math_sqrt(expr:smart_ptr<ExprCall>)
        var sqrt_name = "llvm.sqrt.f32"
        assume opType = expr.arguments[0]._type
        if opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
            if opType.baseType==Type tDouble
                sqrt_name = "llvm.sqrt.f64"
        else
            return null
        var args <- [{auto getE(expr.arguments[0])}]
        var argTypes <- [{auto type_to_llvm_abi_type(expr.arguments[0]._type)}]
        var id = LLVMLookupIntrinsicID(sqrt_name)
        var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
        if decl==null
            return null
        return LLVMBuildCall(g_builder, decl, args, "sqrt")

    def intrinsic_builtin_range(expr:smart_ptr<ExprCall>)
        var x, y : LLVMOpaqueValue?
        if expr.arguments.length==1
            x = LLVMConstInt(LLVMInt32Type(), 0ul, 0)
            y = getE(expr.arguments[0])
        else
            x = getE(expr.arguments[0])
            y = getE(expr.arguments[1])
        return LLVMBuildInt2_xy(g_builder, x, y, string(expr.name))

    def build_array_of_arguments ( arguments:dasvector`smart_ptr`Expression ) : LLVMOpaqueValue?
        let nargs = length(arguments)
        var args : LLVMOpaqueValue?
        if nargs != 0
            args = LLVMBuildAlloca(g_builder, LLVMArrayType(LLVMFloat4Type(), uint(nargs)), "args")
            LLVMSetAlignment(args, 16u)
            args = LLVMBuildPointerCast(g_builder, args, LLVMPointerType(LLVMFloat4Type(), 0u), "call_args_ptr")
            for a,ai in arguments,range(100500)
                var arg_ptr = LLVMBuildGEP(g_builder, args, LLVMConstInt(LLVMInt32Type(), uint64(ai), 0), "call_arg_{ai}_ptr")
                var arg_val = getE(a)
                var arg_cast_type = type_to_llvm_abi_type(a._type)
                LLVMBuildStore2(g_builder, arg_cast_type, arg_val, arg_ptr)
        else
            args = LLVMConstPointerNull(LLVMPointerType(LLVMFloat4Type(), 0u))
        return args

    def register_extern_function ( expr:smart_ptr<ExprCall> ) : LLVMOpaqueValue?
        let mangled_name = get_mangled_name(FunctionPtr(expr.func))
        var ofunc = LLVMGetNamedFunction(g_mod, mangled_name)
        if ofunc == null
            var arg_types : array<LLVMOpaqueType?>
            for arg in expr.func.arguments
                arg_types |> push(type_to_llvm_abi_type(arg._type))
            var res_type = type_to_llvm_type(expr.func.result)
            ofunc = LLVMAddFunction(g_mod, mangled_name, LLVMFunctionType(res_type,arg_types))
            let FUNC_PTR = get_builtin_function_address(expr.func)
            if FUNC_PTR==null
                failed_E(expr, "missing interop function pointer for {expr.func.name}")
            LLVMAddGlobalMapping(g_engine, ofunc, FUNC_PTR)
        return ofunc

    def register_interop_function ( expr:smart_ptr<ExprCall> ) : LLVMOpaqueValue?
        let mangled_name = get_mangled_name(FunctionPtr(expr.func))
        var ofunc = LLVMGetNamedFunction(g_mod, mangled_name)
        if ofunc == null
            ofunc = LLVMAddFunction(g_mod, mangled_name, // InteropFunction(context *, node *, args *)
                LLVMFunctionType(LLVMFloat4Type(),
                    [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u)]]))
            let FUNC_PTR = get_builtin_function_address(expr.func)
            if FUNC_PTR==null
                failed_E(expr, "missing interop function pointer for {expr.func.name}")
            LLVMAddGlobalMapping(g_engine, ofunc, FUNC_PTR)
        return ofunc

    def build_call_params ( expr:smart_ptr<ExprCall> )
        var params : array<LLVMOpaqueValue?>
        for a,ai in expr.arguments,range(100500)
            if a._type.baseType==Type fakeContext
                params |> push(LLVMGetParam(ffunc, uint(length(thisFunc.arguments))))
            elif a._type.baseType==Type fakeLineInfo
                params |> push(LLVMConstPointerNull(LLVMVoidType()))
            elif a._type.baseType==Type tPointer
                assume funcArg = expr.func.arguments[ai]
                var ptrType = type_to_llvm_abi_type(funcArg._type)
                var ptrValue = LLVMBuildPointerCast(g_builder, getE(a), ptrType, "arg_{funcArg.name}_ptr")
                params |> push( ptrValue )
            else
                params |> push( getE(a) )
        return <- params

    def override visitExprCall(expr:smart_ptr<ExprCall>) : ExpressionPtr

        // TODO: build table of intrinsic functions, which build LLVMOpaqueValue? from ExprCall
        let call_name = "{expr.func._module.name}::{expr.func.name}"
        var intrin_call : LLVMOpaqueValue?
        if call_name=="$::range" || call_name=="$::urange"
            intrin_call = intrinsic_builtin_range(expr)
        elif call_name=="math::sqrt"
            intrin_call = intrinsic_math_sqrt(expr)
        // return intrinsic if its set
        if intrin_call!=null
            setE(expr, intrin_call)
            return expr

        // get CMRES, if its local
        var cmresEval : LLVMOpaqueValue?
        if isCMRES(expr.func)
            if !expr.doesNotNeedSp && expr.stackTop!=0u
                cmresEval = LLVMBuildGEP(g_builder, local_stack, LLVMConstInt(LLVMInt32Type(), uint64(expr.stackTop), 0), "")
                cmresEval = LLVMBuildPointerCast(g_builder, cmresEval, LLVMPointerType(LLVMVoidType(),0u), "cmres_eval_{call_name}")
            elif call2cmres |> key_exists(get_ptr(expr))
                cmresEval = call2cmres[get_ptr(ExpressionPtr(expr))]
            else
                cmresEval = LLVMConstPointerNull(LLVMPointerType(LLVMVoidType(),0u))
                failed_E(expr, "CMRES is not local, not supported yet")

        if expr.func.flags.builtIn
            if expr.func.flags.interopFn
                var args = build_array_of_arguments(expr.arguments)
                var func = register_interop_function(expr)
                let NODE_PTR = make_interop_node(*jit_context,get_ptr(expr))
                if NODE_PTR==null
                    failed_E(expr, "missing interop function node pointer for {expr.func.name}")
                let NODE_ADDR = intptr(NODE_PTR)
                var params <- [{LLVMOpaqueValue?
                    LLVMGetParam(ffunc, uint(length(thisFunc.arguments)));  // Context *
                    LLVMConstIntToPtr(LLVMConstInt(LLVMInt64Type(), NODE_ADDR, 0), LLVMPointerType(LLVMVoidType(), 0u)); // Node *
                    args                                                   // args
                }]
                var ccall = LLVMBuildCall(g_builder, func, params, "")
                if expr.func.result.isVoid
                    setE(expr, ccall)
                else
                    var cres = cast_from_vec4f(expr, expr._type, ccall)
                    setE(expr, cres)
            else
                var params <- build_call_params(expr)
                var func = register_extern_function(expr)
                var ccall = LLVMBuildCall(g_builder, func, params, "")
                if cmresEval!=null
                    LLVMBuildStore2(g_builder, type_to_llvm_type(expr.func.result), ccall, cmresEval)
                    setE(expr, cmresEval)
                else
                    setE(expr, ccall)
        else
            if expr.func.moreFlags.requestJit
                var params <- build_call_params(expr)
                params |> push(get_context_param()) // context
                if cmresEval!=null
                    params |> push(cmresEval)       // cmres
                let fmna = get_mangled_name(FunctionPtr(expr.func))
                var func = LLVMGetNamedFunction(g_mod, "{fmna} implementation")
                var ccall = LLVMBuildCall(g_builder, func, params, "")
                if cmresEval != null
                    setE(expr, cmresEval)
                else
                    setE(expr, ccall)
            else
                var args = build_array_of_arguments(expr.arguments)
                var MNH_ADDR : uint64
                unsafe
                    let mangled_name = get_mangled_name(FunctionPtr(expr.func))
                    let MNH = hash(mangled_name)
                    MNH_ADDR = get_function_address(MNH,*jit_context)
                if MNH_ADDR==0ul
                    failed_E(expr, "missing function pointer for {expr.func.name}")
                var params : array<LLVMOpaqueValue?>
                params |> push(LLVMConstIntToPtr(LLVMConstInt(LLVMInt64Type(), MNH_ADDR, 0), LLVMPointerType(LLVMVoidType(), 0u)))  // mnh
                params |> push(args)
                if cmresEval!=null
                    params |> push(cmresEval)
                params |> push(LLVMGetParam(ffunc, uint(length(thisFunc.arguments))))   // context
                var func = LLVMGetNamedFunction(g_mod, cmresEval!=null ? FN_JIT_CALL_WITH_CMRES : FN_JIT_CALL_OR_FASTCALL)
                var ccall = LLVMBuildCall(g_builder, func, params, "")
                if cmresEval != null
                    setE(expr, cmresEval)
                elif expr.func.result.isVoid
                    setE(expr, ccall)
                else
                    var cres = cast_from_vec4f(expr, expr._type, ccall)
                    setE(expr, cres)
        return expr

    def override visitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>) : ExpressionPtr
        var args = build_array_of_arguments(expr.elements)
        var func = LLVMGetNamedFunction(g_mod, FN_JIT_STRING_BUILDER)
        assert(func!=null)
        let NODE_PTR = make_interop_node(*jit_context,get_ptr(expr))
        if NODE_PTR==null
            failed_E(expr, "missing interop function node pointer for String Builder")
        let NODE_ADDR = intptr(NODE_PTR)
        var params <- [{LLVMOpaqueValue?
            LLVMGetParam(ffunc, uint(length(thisFunc.arguments)));  // Context *
            LLVMConstIntToPtr(LLVMConstInt(LLVMInt64Type(), NODE_ADDR, 0), LLVMPointerType(LLVMVoidType(), 0u)); // Node *
            args                                                   // args
        }]
        setE(expr, LLVMBuildCall(g_builder, func, params, "string_builder"))
        return expr

// swizzle

    def override visitExprSwizzle(expr:smart_ptr<ExprSwizzle>) : ExpressionPtr
        var val = getE(expr.value)
        if expr._type.flags.ref
            failed_E(expr, "return ref swizzle is not supported yet")
            setE(expr, LLVMGetUndef(LLVMInt32Type()))
        else
            var isSequencial = true
            for i in range(1,length(expr.fields))
                if (int(expr.fields[i-1])+1)!=int(expr.fields[i])
                    isSequencial = false
                    break
            if expr.value._type.flags.ref && isSequencial
                var vec_element_type = base_type_to_llvm_type(expr.value._type.vectorBaseType)
                var val_ptr = LLVMBuildBitCast(g_builder, val, LLVMPointerType(vec_element_type, 0u), "")
                val_ptr = LLVMBuildGEP(g_builder, val_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.fields[0]), 0), "")
                setE(expr, LLVMBuildLoad2(g_builder, type_to_llvm_type(expr._type), val_ptr, "swizzle_{expr.fields}"))
            else
                failed_E(expr, "val.swizzle is not supported yet, sequencial: {isSequencial} ref:{expr.value._type.flags.ref}")
                setE(expr, LLVMGetUndef(LLVMInt32Type()))
        return expr

// field
    def override visitExprField(expr:smart_ptr<ExprField>) : ExpressionPtr
        if expr.value._type.isBitfield
            setE(expr, LLVMGetUndef(LLVMInt32Type()))
            failed_E(expr, "bitfields are not supported yet")
        elif expr.field==null && expr.fieldIndex==-1
            setE(expr, LLVMGetUndef(type_to_llvm_type(expr._type)))
            failed_E(expr, "annotations are not supported yet")
            /*
            if ( r2v ) {
                auto resN = annotation->simulateGetFieldR2V(name, context, at, value);
                if ( !resN ) {
                    context.thisProgram->error("integration error, simulateGetFieldR2V returned null", "", "",
                                               at, CompilationError::missing_node );
                }
                return resN;
            } else {
                auto resN = annotation->simulateGetField(name, context, at, value);
                if ( !resN ) {
                    context.thisProgram->error("integration error, simulateGetField returned null", "", "",
                                               at, CompilationError::missing_node );
                }
                return resN;
            }
            */
        else
            assert(expr.field!=null)
            var val_ptr = getE(expr.value)
            if expr.value._type.isPointer && !expr.derefFlags.unsafeDeref
                check_ptr_zero(val_ptr)
            val_ptr = LLVMBuildPointerCast(g_builder, val_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            var field_ptr = LLVMBuildGEP(g_builder, val_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.field.offset), 0), "")
            field_ptr = LLVMBuildPointerCast(g_builder, field_ptr, LLVMPointerType(type_to_llvm_type(expr.field._type), 0u), "{describe(expr.value._type,false,false,false)}.{expr.field.name}")
            if expr.fieldFlags.r2v
                setE(expr, LLVMBuildLoad(g_builder, field_ptr, ""))
            else
                setE(expr, field_ptr)
        return expr


// break
    def override preVisitExprBreak(expr:smart_ptr<ExprBreak>) : void
        var lblk = loop_stack |> back()
        LLVMBuildBr(g_builder, lblk.loop_end)
// continue
    def override preVisitExprContinue(expr:smart_ptr<ExprContinue>) : void
        var lblk = loop_stack |> back()
        LLVMBuildBr(g_builder, lblk.loop_start)

// block
    def override preVisitExprBlock(var blk:smart_ptr<ExprBlock>) : void
        if blk.blockFlags.inTheLoop
            blk.blockFlags |= ExprBlockFlags finallyDisabled
    def override visitExprBlock(var blk:smart_ptr<ExprBlock>) : ExpressionPtr
        if blk.blockFlags.inTheLoop
            blk.blockFlags ^= ExprBlockFlags finallyDisabled
        return blk

// for loop

    def override preVisitExprFor(expr:smart_ptr<ExprFor>) : void
        var lblk = [[LoopBlock
            loop_start = LLVMAppendBasicBlock(ffunc, "for_body"),
            loop_body = LLVMAppendBasicBlock(ffunc, "for_body"),
            loop_end = LLVMAppendBasicBlock(ffunc, "for_end")
        ]]
        loop_stack |> push(lblk)

    def override visitExprFor(expr:smart_ptr<ExprFor>) : ExpressionPtr
        var lblk = loop_stack |> back()
        loop_stack |> pop()

        LLVMBuildBr(g_builder, lblk.loop_start)
        LLVMPositionBuilderAtEnd(g_builder, lblk.loop_end)
        if expr.body is ExprBlock
            let body = expr.body as ExprBlock
            if length(body.finalList) != 0
                visit_finally(body, adapter)
        return expr

    def override preVisitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool) : void
        // add variable to be looked up
        var vtype = type_to_llvm_type(svar._type)
        if svar._type.isRef
            vtype = LLVMPointerType(vtype,0u)
        var v_ptr = LLVMBuildGEP(g_builder, local_stack, LLVMConstInt(LLVMInt32Type(), uint64(svar.stackTop), 0), "")
        var vmem = LLVMBuildPointerCast(g_builder, v_ptr, LLVMPointerType(vtype,0u), "for_loop_variable_{svar.name}")
        setV(svar, vmem)
        // initialize its memory with 0 (should we?)
        var vsize = svar._type.isRef ?  typeinfo(sizeof type<void?>) : svar._type.sizeOf
        LLVMBuildMemSet(g_builder, vmem, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(vsize), 0), 16u)

    def override preVisitExprForBody(expr:smart_ptr<ExprFor>) : void
        var lblk = loop_stack |> back()
        var range2 : table<Expression?; LLVMOpaqueValue?>

        for svar,ssrc in expr.iteratorVariables,expr.sources
            if ssrc._type.isRange           // range()
                var srange = getE(ssrc)
                var sfrom = LLVMBuildExtractElement(g_builder, srange, LLVMConstInt(LLVMInt32Type(),0ul,0), "range_from_{svar.name}")
                var sto = LLVMBuildExtractElement(g_builder, srange, LLVMConstInt(LLVMInt32Type(),1ul,0), "range_to_{svar.name}")
                var cmp_from_to = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntSLT, sfrom, sto, "")
                var okay = LLVMAppendBasicBlock(ffunc, "for_{svar.name}_not_empty")
                LLVMBuildCondBr(g_builder, cmp_from_to, okay, lblk.loop_end)
                LLVMPositionBuilderAtEnd(g_builder, okay)
                range2[get_ptr(ssrc)] = sto
                LLVMBuildStore(g_builder, sfrom, getV(svar))
            elif ssrc._type.dim.length != 0  // []
                var element_type = type_to_llvm_type(svar._type)
                var sdim = getE(ssrc)
                var s0 = LLVMBuildGEP2(g_builder, element_type, sdim, LLVMConstInt(LLVMInt32Type(),0ul,0), "dim_0")
                LLVMBuildStore(g_builder, s0, getV(svar))
                let dimSize = ssrc._type.dim[ssrc._type.dim.length-1]
                var sto = LLVMBuildGEP2(g_builder, element_type, sdim, LLVMConstInt(LLVMInt32Type(),uint64(dimSize),0), "")
                sto = LLVMBuildPtrToInt(g_builder, sto, LLVMIntPtrType(), "dim_end")
                range2[get_ptr(ssrc)] = sto
            else
                failed_E(ssrc, "unsupported loop source {describe(ssrc._type)}")

        var for_iterate = LLVMAppendBasicBlock(ffunc, "for_body_loop")
        LLVMBuildBr(g_builder, for_iterate)
        LLVMPositionBuilderAtEnd(g_builder, lblk.loop_start)

        for svar,ssrc in expr.iteratorVariables,expr.sources
            if ssrc._type.isRange           // range()
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                svar_v = LLVMBuildAdd(g_builder, svar_v, LLVMConstInt(LLVMInt32Type(), 1ul, 0), "")
                LLVMBuildStore(g_builder, svar_v, getV(svar))
            elif ssrc._type.dim.length != 0 // []
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                var svar_i = LLVMBuildGEP2(g_builder, type_to_llvm_type(svar._type), svar_v, LLVMConstInt(LLVMInt32Type(),1ul,0), "")
                LLVMBuildStore(g_builder, svar_i, getV(svar))
            else
                failed_E(ssrc, "unsupported loop source {describe(ssrc._type)}")

        LLVMBuildBr(g_builder, for_iterate)
        LLVMPositionBuilderAtEnd(g_builder, for_iterate)

        for svar,ssrc in expr.iteratorVariables,expr.sources
            if ssrc._type.isRange                       // range
                var sto = range2[get_ptr(ssrc)]
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                var rcond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, svar_v, sto, "")
                var okay = LLVMAppendBasicBlock(ffunc, "for_{svar.name}_okay")
                LLVMBuildCondBr(g_builder, rcond, lblk.loop_end, okay)
                LLVMPositionBuilderAtEnd(g_builder, okay)
            elif ssrc._type.dim.length != 0             // []
                var sto = range2[get_ptr(ssrc)]
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                var svar_i = LLVMBuildPtrToInt(g_builder, svar_v, LLVMIntPtrType(), "")
                var rcond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, svar_i, sto, "")
                var okay = LLVMAppendBasicBlock(ffunc, "for_{svar.name}_okay")
                LLVMBuildCondBr(g_builder, rcond, lblk.loop_end, okay)
                LLVMPositionBuilderAtEnd(g_builder, okay)
            else
                failed_E(ssrc, "unsupported loop source {describe(ssrc._type)}")

        LLVMBuildBr(g_builder, lblk.loop_body)
        LLVMPositionBuilderAtEnd(g_builder, lblk.loop_body)

// while loop
    def override preVisitExprWhile(expr:smart_ptr<ExprWhile>) : void
        var lblk = [[LoopBlock
            loop_start = LLVMAppendBasicBlock(ffunc, "while_start"),
            loop_body = LLVMAppendBasicBlock(ffunc, "while_body"),
            loop_end = LLVMAppendBasicBlock(ffunc, "while_end")
        ]]
        loop_stack |> push(lblk)
        LLVMBuildBr(g_builder, lblk.loop_start)
        LLVMPositionBuilderAtEnd(g_builder, lblk.loop_start)
    def override preVisitExprWhileBody(expr:smart_ptr<ExprWhile>;right:ExpressionPtr) : void
        var lblk = loop_stack |> back()
        LLVMBuildCondBr(g_builder, getE(expr.cond), lblk.loop_body, lblk.loop_end)
        LLVMPositionBuilderAtEnd(g_builder, lblk.loop_body)
    def override visitExprWhile(expr:smart_ptr<ExprWhile>) : ExpressionPtr
        var lblk = loop_stack |> back()
        loop_stack |> pop()
        LLVMBuildBr(g_builder, lblk.loop_start)
        LLVMPositionBuilderAtEnd(g_builder, lblk.loop_end)
        if expr.body is ExprBlock
            let body = expr.body as ExprBlock
            if length(body.finalList) != 0
                visit_finally(body, adapter)
        return expr

// constants
    def override visitExprConstPtr(expr:smart_ptr<ExprConstPtr>) : ExpressionPtr
        setE(expr, LLVMConstPointerNull(LLVMVoidType()))
        return expr
    def override visitExprConstEnumeration(expr:smart_ptr<ExprConstEnumeration>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstBitfield(expr:smart_ptr<ExprConstBitfield>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt32Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt8(expr:smart_ptr<ExprConstInt8>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt8Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt16(expr:smart_ptr<ExprConstInt16>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt16Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt64(expr:smart_ptr<ExprConstInt64>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt64Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt(expr:smart_ptr<ExprConstInt>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt32Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt2(expr:smart_ptr<ExprConstInt2>) : ExpressionPtr
        var x = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.x), 0)
        var y = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.y), 0)
        setE(expr, LLVMBuildInt2_xy(g_builder, x, y, "int2"))
        return expr
    def override visitExprConstInt3(expr:smart_ptr<ExprConstInt3>) : ExpressionPtr
        var x = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.x), 0)
        var y = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.y), 0)
        var z = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.z), 0)
        setE(expr, LLVMBuildInt3_xyz(g_builder, x, y, z, "int3"))
        return expr
    def override visitExprConstInt4(expr:smart_ptr<ExprConstInt4>) : ExpressionPtr
        var x = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.x), 0)
        var y = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.y), 0)
        var z = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.z), 0)
        var w = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.w), 0)
        setE(expr, LLVMBuildInt4_xyzw(g_builder, x, y, z, w, "int4"))
        return expr
    def override visitExprConstUInt8(expr:smart_ptr<ExprConstUInt8>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt8Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstUInt16(expr:smart_ptr<ExprConstUInt16>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt16Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstUInt64(expr:smart_ptr<ExprConstUInt64>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt64Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstUInt(expr:smart_ptr<ExprConstUInt>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt32Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstUInt2(expr:smart_ptr<ExprConstUInt2>) : ExpressionPtr
        var x = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.x), 0)
        var y = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.y), 0)
        setE(expr, LLVMBuildInt2_xy(g_builder, x, y, "uint2"))
        return expr
    def override visitExprConstUInt3(expr:smart_ptr<ExprConstUInt3>) : ExpressionPtr
        var x = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.x), 0)
        var y = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.y), 0)
        var z = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.z), 0)
        setE(expr, LLVMBuildInt3_xyz(g_builder, x, y, z, "uint3"))
        return expr
    def override visitExprConstUInt4(expr:smart_ptr<ExprConstUInt4>) : ExpressionPtr
        var x = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.x), 0)
        var y = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.y), 0)
        var z = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.z), 0)
        var w = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.w), 0)
        setE(expr, LLVMBuildInt4_xyzw(g_builder, x, y, z, w, "uint4"))
        return expr
    def override visitExprConstRange(expr:smart_ptr<ExprConstRange>) : ExpressionPtr
        var x = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.x), 0)
        var y = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.y), 0)
        setE(expr, LLVMBuildInt2_xy(g_builder, x, y, "range"))
        return expr
    def override visitExprConstURange(expr:smart_ptr<ExprConstURange>) : ExpressionPtr
        var x = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.x), 0)
        var y = LLVMConstInt(LLVMInt32Type(), uint64(expr.value.y), 0)
        setE(expr, LLVMBuildInt2_xy(g_builder, x, y, "urange"))
        return expr
    def override visitExprConstBool(expr:smart_ptr<ExprConstBool>) : ExpressionPtr
        setE(expr, LLVMConstInt(LLVMInt1Type(), expr.value ? 1ul : 0ul, 0))
        return expr
    def override visitExprConstFloat(expr:smart_ptr<ExprConstFloat>) : ExpressionPtr
        setE(expr, LLVMConstReal(LLVMFloatType(), double(expr.value)))
        return expr
    def override visitExprConstFloat2(expr:smart_ptr<ExprConstFloat2>) : ExpressionPtr
        var x = LLVMConstReal(LLVMFloatType(), double(expr.value.x))
        var y = LLVMConstReal(LLVMFloatType(), double(expr.value.y))
        setE(expr, LLVMBuildFloat2_xy(g_builder, x, y, "float2"))
        return expr
    def override visitExprConstFloat3(expr:smart_ptr<ExprConstFloat3>) : ExpressionPtr
        var x = LLVMConstReal(LLVMFloatType(), double(expr.value.x))
        var y = LLVMConstReal(LLVMFloatType(), double(expr.value.y))
        var z = LLVMConstReal(LLVMFloatType(), double(expr.value.z))
        setE(expr, LLVMBuildFloat3_xyz(g_builder, x, y, z, "float3"))
        return expr
    def override visitExprConstFloat4(expr:smart_ptr<ExprConstFloat4>) : ExpressionPtr
        var x = LLVMConstReal(LLVMFloatType(), double(expr.value.x))
        var y = LLVMConstReal(LLVMFloatType(), double(expr.value.y))
        var z = LLVMConstReal(LLVMFloatType(), double(expr.value.z))
        var w = LLVMConstReal(LLVMFloatType(), double(expr.value.w))
        setE(expr, LLVMBuildFloat4_xyzw(g_builder, x, y, z, w, "float4"))
        return expr
    def override visitExprConstString(expr:smart_ptr<ExprConstString>) : ExpressionPtr
        setE(expr, get_string_constant_ptr(string(expr.value)))
        return expr
    def override visitExprConstDouble(expr:smart_ptr<ExprConstDouble>) : ExpressionPtr
        setE(expr, LLVMConstReal(LLVMDoubleType(), expr.value))
        return expr
    def override visitExprFakeContext(expr:smart_ptr<ExprFakeContext>) : ExpressionPtr
        setE(expr, LLVMGetParam(ffunc,0u))
        return expr
    def override visitExprFakeLineInfo(expr:smart_ptr<ExprFakeLineInfo>) : ExpressionPtr
        setE(expr, LLVMConstPointerNull(LLVMVoidType()))
        return expr

[macro_function]
def private generate_llvm ( ctx:Context?; fn : FunctionPtr )
    return if is_in_completion() || is_compiling_macros()
    var astVisitor = new LlvmJitVisitor(ctx)
    unsafe
        astVisitor.adapter <- make_visitor(*astVisitor)
    visit(fn, astVisitor.adapter)
    if length(astVisitor.errors) > 0
        let errors = astVisitor.errors |> join("\n")
        to_log(LOG_ERROR,"LLVM JIT FAILED:\n{errors}\n")
        g_failed = true
    if !LLVMVerifyModule(g_mod, LLVMVerifierFailureAction LLVMPrintMessageAction, false)
        g_failed = true
    astVisitor.adapter := null
    unsafe
        delete astVisitor

[macro_function]
def private generate_llvm_code ( fmna:string )
    return null if is_in_completion() || is_compiling_macros()
    return null if g_failed
    var code = LLVMGetFunctionAddress(g_engine, fmna)
    to_log(LOG_ERROR, "LLVM JIT FAILED: {fmna}\n")  if code==0ul
    return unsafe(reinterpret<void?> code)

[macro_function]
def private optimize_llvm_module
    return if is_in_completion() || is_compiling_macros()
    return if g_failed || !LLVM_ENABLE_OPT_PASS
    var pmb = LLVMPassManagerBuilderCreate()
    LLVMPassManagerBuilderSetOptLevel(pmb, LLVM_OPT_LEVEL)
    var pm = LLVMCreatePassManager()
    LLVMPassManagerBuilderPopulateModulePassManager(pmb, pm)
    // LLVMPassManagerBuilderPopulateLTOPassManager(pmb, pm, 1, 1)
    LLVMRunPassManager(pm,g_mod)
    LLVMDisposePassManager(pm)
    LLVMPassManagerBuilderDispose(pmb)

[simulate_macro(name="jit_llvm")]
class JIT_LLVM : AstSimulateMacro
    def override simulate ( prog:Program?; var ctx:Context? ) : bool
        return true if is_in_completion() || is_compiling_macros()
        init_jit()
        var funcs : array<FunctionPtr>
        var fake_visitor = new LlvmJitVisitor(ctx)
        prog |> for_each_module <| $ ( mod )
            mod |> for_each_function("") <| $ ( fun )
                if fun.moreFlags.requestJit
                    fake_visitor->add_llvm_functions(fun)
                    funcs |> emplace(fun)
        unsafe
            delete fake_visitor
        for fun in funcs
            generate_llvm(ctx, fun)
        optimize_llvm_module()
        for fun in funcs
            var mangled_name = get_mangled_name(fun)
            let MNH = hash(mangled_name)
            var simfun = get_function_by_mangled_name_hash(MNH, *ctx)
            var gen = generate_llvm_code(mangled_name)
            unsafe
                if gen != null
                    gen |> instrument_jit(simfun)
                else
                    remove_jit(simfun)
        finalize_jit()
        return true

