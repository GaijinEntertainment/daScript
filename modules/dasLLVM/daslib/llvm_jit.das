options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module llvm_jit shared private

require llvm/llvm_boost
require daslib/ast_boost
require daslib/templates_boost
require daslib/ast public
require daslib/safe_addr
require daslib/strings_boost
require daslib/defer

let LLVM_ENABLE_OPT_PASS = true
let LLVM_OPT_LEVEL = 2u
let LLVM_JIT_OPT_LEVEL = 2u
let LLVM_LOG_RESULT = true

var g_mod : LLVMOpaqueModule?
var g_engine : LLVMExecutionEngineRef
var g_builder : LLVMOpaqueBuilder?

def private get_expr_ptr ( expr:ExpressionPtr )
    return get_ptr(expr)

[init]
def private init_jit
    return if is_in_completion() || is_compiling_macros()
    return if g_builder != null    // prevent double init
    g_mod = LLVMModuleCreateWithName("llvm_jit_module")
    g_builder = LLVMCreateBuilder()
    LLVMLinkInMCJIT()
    LLVMInitializeNativeTarget()
    LLVMInitializeNativeAsmPrinter()
    LLVMInitializeNativeAsmParser()
    LLVMCreateJITCompilerForModule(g_engine, g_mod, LLVM_JIT_OPT_LEVEL)
    // add default functions
    var je = LLVMAddFunction(g_mod, "jit_exception", LLVMFunctionType(LLVMVoidType(),[[LLVMTypeRef LLVMPointerType(LLVMInt8Type(),0u); LLVMPointerType(LLVMVoidType(),0u)]]))
    LLVMAddGlobalMapping(g_engine, je, get_jit_exception())

[finalize]
def private finalize_jit
    return if is_in_completion() || is_compiling_macros()
    return if g_builder == null    // prevent double finalize
    if LLVM_LOG_RESULT
        LLVMDumpModule(g_mod)
    g_mod = null
    LLVMDisposeBuilder(g_builder); g_builder = null
    LLVMDisposeExecutionEngine(g_engine); g_engine = null

struct private IteBlock
    if_true  : LLVMOpaqueBasicBlock?
    if_false : LLVMOpaqueBasicBlock?
    if_end   : LLVMOpaqueBasicBlock?
    result : LLVMOpaqueValue?
    if_true_terminates  : bool
    if_false_terminates : bool

class LlvmJitVisitor : AstVisitor
    e2v : table<Expression?;LLVMOpaqueValue?>
    v2v : table<Variable?;LLVMOpaqueValue?>
    str2v : table<string; LLVMOpaqueValue?>
    errors : array<string>
    fnmna : string
    ffunc : LLVMOpaqueValue?
    ite2blocks : table<Expression?; IteBlock>

    def LlvmJitVisitor
        pass

    def failed ( e:ExpressionPtr; msg:string )
        errors |> push("{msg}, expr = {e.__rtti} at {describe(e.at)}")

    def failed_V ( v:VariablePtr; msg:string )
        errors |> push("{msg}, var = {describe(v)} at {describe(v.at)}")

    def setE ( e : ExpressionPtr; ov:LLVMOpaqueValue? )
        e2v[get_ptr(e)] = ov

    def getE ( e : ExpressionPtr )
        var res : LLVMOpaqueValue?
        e2v |> find_if_exists(get_ptr(e)) <| $ ( val )
            res = *val
        if res == null
            res = LLVMGetUndef(LLVMInt32Type())
            failed(e, "unresolved expression {describe(e)}")
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

    def base_type_to_llvm_type ( e:ExpressionPtr; t:Type )
        return LLVMInt32Type()  if t == Type tInt || t == Type tUInt || t == Type tBitfield
        return LLVMInt64Type()  if t == Type tInt64 || t == Type tUInt64
        return LLVMInt16Type()  if t == Type tInt16 || t == Type tUInt16
        return LLVMInt8Type()   if t == Type tInt8 || t == Type tUInt8
        return LLVMFloatType()  if t == Type tFloat
        return LLVMDoubleType() if t == Type tDouble
        return LLVMInt1Type()   if t == Type tBool
        return LLVMInt2Type()   if t == Type tInt2 || t == Type tUInt2
        return LLVMInt3Type()   if t == Type tInt3 || t == Type tUInt3
        return LLVMInt4Type()   if t == Type tInt4 || t == Type tUInt4
        return LLVMFloat2Type() if t == Type tFloat2
        return LLVMFloat3Type() if t == Type tFloat3
        return LLVMFloat4Type() if t == Type tFloat4
        failed(e, "unsupported type {t}")
        return LLVMInt32Type()

    def type_to_llvm_abi_type ( e:ExpressionPtr; t:TypeDeclPtr )
        if t.isPointer || t.isRef
            return LLVMPointerType(LLVMVoidType(), 0u)
        elif t.isWorkhorseType
            return base_type_to_llvm_type(e, t.baseType)
        else
            failed(e, "unsupported type {describe(t)}")
            return LLVMInt32Type()

    def type_to_llvm_type ( e:ExpressionPtr; t:TypeDeclPtr )
        if t.isPointer || t.isRef
            return LLVMPointerType(LLVMVoidType(), 0u)
        elif t.isWorkhorseType
            return base_type_to_llvm_type(e, t.baseType)
        else
            failed(e, "unsupported type {describe(t)}")
            return LLVMInt32Type()

    def override preVisitFunction(fun:FunctionPtr) : void
        assert(g_builder!=null, "missing builder")
        fnmna = get_mangled_name(fun)
        var ret_type = LLVMFunctionType(LLVMFloat4Type(), [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u); LLVMPointerType(LLVMFloat4Type(),0u); LLVMPointerType(LLVMVoidType(),0u) ]])
        ffunc = LLVMAddFunction(g_mod, fnmna, ret_type)
        var entry = LLVMAppendBasicBlock(ffunc, "entry")
        LLVMPositionBuilderAtEnd(g_builder, entry)
    def override visitFunction(fun:FunctionPtr) : FunctionPtr
        ffunc = null
        return fun

    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : void
        var vtype = arg._type.isRef ?  LLVMPointerType(LLVMVoidType(),0u) : type_to_llvm_type(expr, arg._type)
        var vmem = LLVMBuildAlloca(g_builder, vtype, "variable_{arg.name}")
        LLVMSetAlignment(vmem, 16u)
        setV(arg, vmem)
        if arg.init == null
            var vsize = arg._type.isRef ?  typeinfo(sizeof type<void?>) : arg._type.sizeOf
            LLVMBuildMemSet(g_builder, vmem, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(vsize), 0), 16u)

    def override visitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
        var tdst = getV(arg)
        build_copy(tdst, expr)
        return blk

    def override visitExprVar(expr:smart_ptr<ExprVar>) : ExpressionPtr
        if expr.varFlags._block
            failed(expr, "{expr.variable.name} support block variable")
        elif expr.varFlags.local    // local varaibels
            var var_ptr = getV(expr.variable)
            if expr.varFlags.r2v
                assert(expr._type.isWorkhorseType,"r2v only for workhorse types")
                setE(expr, LLVMBuildLoad2(g_builder, base_type_to_llvm_type(expr, expr._type.baseType), var_ptr, "var_r2v_{expr.argumentIndex}"))
            else
                setE(expr, LLVMBuildPointerCast(g_builder, var_ptr, LLVMPointerType(LLVMVoidType(),0u), "var_data" ))
        elif expr.varFlags.argument // arguments
            var args = LLVMGetParam(ffunc, 1u)
            var arg_ptr = LLVMBuildGEP(g_builder, args, LLVMConstInt(LLVMInt32Type(), uint64(expr.argumentIndex), 0), "arg_ptr_{expr.argumentIndex}")
            if expr.varFlags.r2v
                assert(expr._type.isWorkhorseType,"r2v only for workhorse types")
                setE(expr, LLVMBuildLoad2(g_builder, base_type_to_llvm_type(expr, expr._type.baseType), arg_ptr, "arg_r2v_{expr.argumentIndex}"))
            else
                setE(expr, LLVMBuildPointerCast(g_builder, arg_ptr, LLVMPointerType(LLVMVoidType(),0u), "arg_to_void" ))
        else
            failed(expr, "ExprLetVariable {expr.variable.name} : {describe(expr.variable._type)}")
        return expr

    def cast_to_vec4f ( expr:ExpressionPtr )
        assume etype = expr._type
        var vres = LLVMGetUndef(LLVMFloat4Type())
        var tres : LLVMOpaqueValue?
        var tsrc = getE(expr)
        if etype.isWorkhorseType
            let bt = etype.baseType
            if bt==Type tInt || bt==Type tUInt || bt==Type tBitfield
                var tbit = LLVMBuildBitCast(g_builder, tsrc, LLVMFloatType(), "tbit")
                tres = LLVMBuildInsertElement(g_builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "return_result")
            elif bt==Type tBool || bt==Type tInt8 || bt==Type tInt16 || bt==Type tUInt8 || bt==Type tUInt16
                var tibit = LLVMBuildZExtOrBitCast(g_builder, tsrc, LLVMInt32Type(), "int_bit")
                var tbit = LLVMBuildBitCast(g_builder, tibit, LLVMFloatType(), "tbit")
                tres = LLVMBuildInsertElement(g_builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "return_result")
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
            else
                failed(expr, "unsupported cast_to_vec4f cast {describe(etype)}")
        else
            failed(expr, "unsupported cast_to_vec4f type {describe(etype)}")
        return tres

    def override visitExprReturn(expr:smart_ptr<ExprReturn>) : ExpressionPtr
        if expr.subexpr != null
            var tres = cast_to_vec4f(expr.subexpr)
            if tres != null
                LLVMBuildRet(g_builder, tres)
            else
                LLVMBuildRetVoid(g_builder)
        else
            LLVMBuildRetVoid(g_builder)
        return expr

    def build_string_constant ( message:string )
        if str2v |> key_exists(message)
            return str2v[message]
        let msg_size = uint(length(message))
        var strType = LLVMArrayType( LLVMInt8Type(), msg_size+1u )
        var str = LLVMAddGlobal(g_mod, strType, "")
        LLVMSetInitializer(str, LLVMConstString(message, msg_size, 0))
        LLVMSetGlobalConstant(str, 1)
        LLVMSetLinkage(str, LLVMLinkage LLVMPrivateLinkage)
        LLVMSetUnnamedAddress(str, LLVMUnnamedAddr LLVMGlobalUnnamedAddr)
        LLVMSetAlignment(str, 1u)
        str2v[message] = str
        return str

    def get_string_constant_ptr ( message:string )
        var str = build_string_constant(message)
        var str_ptr = LLVMBuildBitCast(g_builder, str, LLVMPointerType(LLVMInt8Type(), 0u), "")
        return LLVMBuildGEP(g_builder, str_ptr, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "string_constant {message}")

    def build_exception ( message:string )
        var params = [[auto
            get_string_constant_ptr(message);
            LLVMGetParam(ffunc,0u)
        ]]
        LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, "jit_exception"), unsafe(addr(params[0])), 2u, "")

    def build_copy ( tdst:LLVMOpaqueValue?; right:ExpressionPtr )
        var tsrc = getE(right)
        if right._type.isRef
            let copySize = right._type.sizeOf
            var rsize = LLVMConstInt(LLVMInt32Type(), uint64(copySize), 0)
            LLVMBuildMemCpy(g_builder, tdst, 16u, tsrc, 16u, rsize)
        else
            var dst_type = LLVMPointerType(type_to_llvm_type(right, right._type),0u)
            var dst_ptr = LLVMBuildPointerCast(g_builder, tdst, dst_type, "dst_ptr_type" )
            LLVMBuildStore(g_builder, tsrc, dst_ptr)

    def override visitExprCopy(expr:smart_ptr<ExprCopy>) : ExpressionPtr
        var tdst = getE(expr.left)
        build_copy(tdst, expr.right)
        return expr

    def override visitExprOp2(expr:smart_ptr<ExprOp2>) : ExpressionPtr
        var left = getE(expr.left)
        var right = getE(expr.right)
        if expr.op=="+"
            if expr._type.isInteger || (expr._type.isVectorType && (expr._type.vectorBaseType==Type tInt || expr._type.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildAdd(g_builder, left, right, "op_add"))
            elif expr._type.isFloatOrDouble || (expr._type.isVectorType && expr._type.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFAdd(g_builder, left, right, "op_add"))
            else
                failed(expr, "unsupported add type {expr._type}")
        elif expr.op=="-"
            if expr._type.isInteger || (expr._type.isVectorType && (expr._type.vectorBaseType==Type tInt || expr._type.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildSub(g_builder, left, right, "op_sub"))
            elif expr._type.isFloatOrDouble || (expr._type.isVectorType && expr._type.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFSub(g_builder, left, right, "op_sub"))
            else
                failed(expr, "unsupported sub type {expr._type}")
        elif expr.op=="*"
            if expr._type.isInteger || (expr._type.isVectorType && (expr._type.vectorBaseType==Type tInt || expr._type.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildMul(g_builder, left, right, "op_mul"))
            elif expr._type.isFloatOrDouble || (expr._type.isVectorType && expr._type.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFMul(g_builder, left, right, "op_mul"))
            else
                failed(expr, "unsupported mul type {expr._type}")
        elif expr.op=="/" || expr.op=="%"
            // for integer division we check division by zero
            if expr._type.isInteger
                var check_div_0 = LLVMAppendBasicBlock(ffunc, "check_div_0")
                var check_true = LLVMAppendBasicBlock(ffunc, "check_true")
                var check_end = LLVMAppendBasicBlock(ffunc, "check_end")
                LLVMBuildBr(g_builder, check_div_0)
                LLVMPositionBuilderAtEnd(g_builder, check_div_0)
                let zero =  LLVMConstInt(base_type_to_llvm_type(expr.right, expr._type.baseType), 0ul, 0)
                let cond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, right, zero, "cmp_is_zero")
                LLVMBuildCondBr(g_builder, cond, check_true, check_end)
                LLVMPositionBuilderAtEnd(g_builder, check_true)
                build_exception("division by zero")
                LLVMBuildBr(g_builder, check_end)
                LLVMPositionBuilderAtEnd(g_builder, check_end)
            if expr.op=="/"
                if expr._type.isSignedInteger || (expr._type.isVectorType && expr._type.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildSDiv(g_builder, left, right, "op_div"))
                elif expr._type.isUnsignedInteger || (expr._type.isVectorType && expr._type.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildUDiv(g_builder, left, right, "op_div"))
                elif expr._type.isFloatOrDouble || (expr._type.isVectorType && expr._type.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildFDiv(g_builder, left, right, "op_div"))
                else
                    failed(expr, "unsupported div type {expr._type}")
            else
                if expr._type.isSignedInteger || (expr._type.isVectorType && expr._type.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildSRem(g_builder, left, right, "op_mod"))
                elif expr._type.isUnsignedInteger || (expr._type.isVectorType && expr._type.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildURem(g_builder, left, right, "op_mod"))
                elif expr._type.isFloatOrDouble || (expr._type.isVectorType && expr._type.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildFRem(g_builder, left, right, "op_mod"))
                else
                    failed(expr, "unsupported mod type {expr._type}")
        elif expr.op=="<" || expr.op==">" || expr.op=="<=" || expr.op==">=" || expr.op=="==" || expr.op=="!="
            var opR : LLVMOpaqueValue?
            if expr.left._type.isSignedInteger
                opR = LLVMBuildICmp(g_builder, get_int_compare_op(expr,string(expr.op)), left, right, "op_bool_{expr.op}")
            elif expr.left._type.isUnsignedInteger
                opR = LLVMBuildICmp(g_builder, get_uint_compare_op(expr,string(expr.op)), left, right, "op_bool_{expr.op}")
            elif expr.left._type.isFloatOrDouble
                opR = LLVMBuildFCmp(g_builder, get_float_compare_op(expr,string(expr.op)), left, right, "op_fbool_{expr.op}")
            elif expr.left._type.baseType == Type tBool
                opR = LLVMBuildICmp(g_builder, get_bool_compare_op(expr,string(expr.op)), left, right, "op_bool_{expr.op}")
            else
                failed(expr, "unsupported compare type {expr._type}")
            if opR == null
                opR = LLVMGetUndef(LLVMInt8Type())
                failed(expr, "unsupported cmp type {expr.left._type} {expr.op} {expr.right._type}")
            setE(expr, opR)
        else
            failed(expr, "unsupported binary operator {expr.op}")
        return expr

    def override visitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : ExpressionPtr
        assert(expr._type.isWorkhorseType)
        setE(expr, LLVMBuildLoad2(g_builder, base_type_to_llvm_type(expr, expr._type.baseType), getE(expr.subexpr), "r2v"))
        return expr

    def get_bool_compare_op ( e:ExpressionPtr; op:string )
        return LLVMIntPredicate LLVMIntEQ   if op=="=="
        return LLVMIntPredicate LLVMIntNE   if op=="!="
        failed(e, "unsupported bool compare operator {op}")
        return LLVMIntPredicate LLVMIntEQ

    def get_int_compare_op ( e:ExpressionPtr; op:string )
        return LLVMIntPredicate LLVMIntEQ   if op=="=="
        return LLVMIntPredicate LLVMIntNE   if op=="!="
        return LLVMIntPredicate LLVMIntSLT  if op=="<"
        return LLVMIntPredicate LLVMIntSLE  if op=="<="
        return LLVMIntPredicate LLVMIntSGT  if op==">"
        return LLVMIntPredicate LLVMIntSGE  if op==">="
        failed(e, "unsupported int compare operator {op}")
        return LLVMIntPredicate LLVMIntEQ

    def get_uint_compare_op ( e:ExpressionPtr; op:string )
        return LLVMIntPredicate LLVMIntEQ   if op=="=="
        return LLVMIntPredicate LLVMIntNE   if op=="!="
        return LLVMIntPredicate LLVMIntULT  if op=="<"
        return LLVMIntPredicate LLVMIntULE  if op=="<="
        return LLVMIntPredicate LLVMIntUGT  if op==">"
        return LLVMIntPredicate LLVMIntUGE  if op==">="
        failed(e, "unsupported uint compare operator {op}")
        return LLVMIntPredicate LLVMIntEQ

    def get_float_compare_op ( e:ExpressionPtr; op:string )
        return LLVMRealPredicate LLVMRealOEQ    if op=="=="
        return LLVMRealPredicate LLVMRealONE    if op=="!="
        return LLVMRealPredicate LLVMRealOLT    if op=="<"
        return LLVMRealPredicate LLVMRealOLE    if op=="<="
        return LLVMRealPredicate LLVMRealOGT    if op==">"
        return LLVMRealPredicate LLVMRealOGE    if op==">="
        failed(e, "unsupported float compare operator {op}")
        return LLVMRealPredicate LLVMRealOEQ

// if-then-else
    def override preVisitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>) : void
        var before_if = LLVMAppendBasicBlock(ffunc, "if_cond_at_{expr.at.line}")
        LLVMBuildBr(g_builder, before_if)
        LLVMPositionBuilderAtEnd(g_builder, before_if)
        let itt = is_terminator_expression(expr.if_true)
        let ift = expr.if_false != null ? is_terminator_expression(expr.if_false) : false
        var blk = [[IteBlock
            if_true  = LLVMAppendBasicBlock(ffunc, "if_true_at_line_{int(expr.at.line)}"),
            if_false = LLVMAppendBasicBlock(ffunc, "if_false_at_line_{int(expr.at.line)}"),
            if_end   = (!itt || !ift) ? LLVMAppendBasicBlock(ffunc, "if_end_at_line_{int(expr.at.line)}") : null,
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
        if !blk.if_false_terminates
            LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_false block we jump to if_end
        if blk.if_end != null
            LLVMPositionBuilderAtEnd(g_builder, blk.if_end)
        return expr

// op3
    def override preVisitExprOp3(expr:smart_ptr<ExprOp3>): void
        var before_if = LLVMAppendBasicBlock(ffunc, "op3_cond_at_{expr.at.line}")
        LLVMBuildBr(g_builder, before_if)
        LLVMPositionBuilderAtEnd(g_builder, before_if)
        var blk = [[IteBlock
            if_true  = LLVMAppendBasicBlock(ffunc, "op3_true_at_line_{int(expr.at.line)}"),
            if_false = LLVMAppendBasicBlock(ffunc, "op3_false_at_line_{int(expr.at.line)}"),
            if_end   = LLVMAppendBasicBlock(ffunc, "op3_end_at_line_{int(expr.at.line)}"),
            if_true_terminates = false,
            if_false_terminates = false,
            result = LLVMBuildAlloca(g_builder, type_to_llvm_abi_type(expr,expr._type), "op3_temp_result_at_line_{int(expr.at.line)}")
        ]]
        ite2blocks[get_expr_ptr(expr)] = blk
    def override preVisitExprOp3Left(expr:smart_ptr<ExprOp3>;left:ExpressionPtr): void
        var blk = ite2blocks[get_expr_ptr(expr)]                        // we start if_true block
        LLVMBuildCondBr(g_builder, getE(expr.subexpr), blk.if_true, blk.if_false)
        LLVMPositionBuilderAtEnd(g_builder, blk.if_true)
    def override preVisitExprOp3Right(expr:smart_ptr<ExprOp3>;right:ExpressionPtr): void
        var blk = ite2blocks[get_expr_ptr(expr)]
        LLVMBuildStore(g_builder, getE(expr.left), blk.result)
        LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_true block we jump to if_end
        LLVMPositionBuilderAtEnd(g_builder, blk.if_false)
    def override visitExprOp3(expr:smart_ptr<ExprOp3>) : ExpressionPtr
        var blk = ite2blocks[get_expr_ptr(expr)]
        LLVMBuildStore(g_builder, getE(expr.right), blk.result)
        LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)
        LLVMPositionBuilderAtEnd(g_builder, blk.if_end)
        /*
        var res = LLVMBuildPhi(g_builder, type_to_llvm_abi_type(expr,expr._type), "op3")
        var phi_vals = [[LLVMOpaqueValue? getE(expr.left); getE(expr.right)]]
        var phi_blocks = [[LLVMOpaqueBasicBlock? blk.if_true; blk.if_false]]
        LLVMAddIncoming(res, phi_vals, phi_blocks)
        */
        setE(expr, LLVMBuildLoad(g_builder, blk.result, "op3_result_at_line_{int(expr.at.line)}"))
        return expr

// function calls

    def intrinsic_builtin_range(expr:smart_ptr<ExprCall>)
        var x, y : LLVMOpaqueValue?
        if expr.arguments.length==1
            x = LLVMConstInt(LLVMInt32Type(), 0ul, 0)
            y = getE(expr.arguments[0])
        else
            x = getE(expr.arguments[0])
            y = getE(expr.arguments[1])
        return LLVMBuildInt2_xy(g_builder, x, y, string(expr.name))

    def override visitExprCall(expr:smart_ptr<ExprCall>) : ExpressionPtr
        // TODO: build table of intrinsic functions, which build LLVMOpaqueValue? from ExprCall
        let call_name = "{expr.func._module.name}::{expr.func.name}"
        if call_name=="$::range" || call_name=="$::urange"
            setE(expr, intrinsic_builtin_range(expr))
        else
            failed(expr, "unsupported call {call_name}")
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
        setE(expr, LLVMConstReal(LLVMFloatType(), expr.value))
        return expr
    def override visitExprFakeContext(expr:smart_ptr<ExprFakeContext>) : ExpressionPtr
        setE(expr, LLVMGetParam(ffunc,0u))
        return expr
    def override visitExprFakeLineInfo(expr:smart_ptr<ExprFakeLineInfo>) : ExpressionPtr
        setE(expr, LLVMConstPointerNull(LLVMVoidType()))
        return expr

def public generate_llvm ( fn : FunctionPtr )
    return if is_in_completion() || is_compiling_macros()
    init_jit()  // we can't be sure init is called before this
    var astVisitor = new LlvmJitVisitor()
    var astVisitorAdapter <- make_visitor(*astVisitor)
    visit(fn, astVisitorAdapter)
    var failed = false
    if length(astVisitor.errors) > 0
        let errors = astVisitor.errors |> join("\n")
        to_log(LOG_ERROR,"LLVM JIT FAILED:\n{errors}\n")
        failed = true
    if !LLVMVerifyModule(g_mod, LLVMVerifierFailureAction LLVMPrintMessageAction, false)
        failed = true
    if !failed && LLVM_ENABLE_OPT_PASS
        var pmb = LLVMPassManagerBuilderCreate()
        LLVMPassManagerBuilderSetOptLevel(pmb, LLVM_OPT_LEVEL)
        var fpm = LLVMCreateFunctionPassManagerForModule(g_mod)
        LLVMPassManagerBuilderPopulateFunctionPassManager(pmb, fpm)
        while LLVMRunFunctionPassManager(fpm, LLVMGetNamedFunction(g_mod, string(get_mangled_name(fn))))==1
            pass
        LLVMDisposePassManager(fpm)
        LLVMPassManagerBuilderDispose(pmb)
    astVisitorAdapter := null
    unsafe
        delete astVisitor

def public generate_llvm_code ( fmna:string )
    return null if is_in_completion() || is_compiling_macros()
    var code = LLVMGetFunctionAddress(g_engine, fmna)
    to_log(LOG_ERROR, "LLVM JIT FAILED: {fmna}\n")  if code==0ul
    return unsafe(reinterpret<void?> code)

[function_macro(name="jit_llvm")]
class Jit_LLVM_Macro : AstFunctionAnnotation
    def generate_jit ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        var funt <- function_to_type(func); defer_delete(funt)
        var efun <- qmacro( @@ <$t(funt)> $i(func.name) ); defer_delete(efun)
        let emod = string(compiling_module().name)
        let fmna = get_mangled_name(func)
        var qres <- qmacro_block() <|
            unsafe
                if get_ptr(this_program()) != null
                    var fnptr <- find_module_function_via_rtti(find_module_via_rtti(this_program(),$v(emod)),$e(efun))
                    generate_llvm(fnptr)
                    fnptr := null
                else
                    panic("missing 'options rtti'")
        var reg <- setup_call_list("all`things`jit", func.at, true)
        reg.list |> emplace(qres)
        var qres_late <- qmacro_block() <|
            unsafe
                var gen = generate_llvm_code($v(fmna))
                if gen != null
                    gen |> instrument_jit(reinterpret<function> $e(efun))
                else
                    remove_jit(reinterpret<function> $e(efun))
        var reg_late <- setup_call_list("all`things`jit`late", func.at, true)
        reg_late.list |> emplace(qres_late)
        return true
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        return true if is_in_completion()
        // func.sideEffectFlags |= bitfield(int(SideEffects userScenario))
        return true if func.isGeneric
        return generate_jit(func, group, args, errors)
    def override generic_apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        return true if is_in_completion()
        return generate_jit(func, group, args, errors)

