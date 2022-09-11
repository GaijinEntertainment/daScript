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

let LLVM_JIT_OPT_LEVEL = 2u // 0u

var g_mod : LLVMOpaqueModule?
var g_engine : LLVMExecutionEngineRef
var g_builder : LLVMOpaqueBuilder?

[init]
def private init_jit
    if is_in_completion() || is_compiling_macros()
        return
    if g_builder != null    // prevent double init
        return
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
    if is_in_completion() || is_compiling_macros()
        return
    if g_builder == null    // prevent double finalize
        return
    LLVMDumpModule(g_mod); g_mod = null
    LLVMDisposeBuilder(g_builder); g_builder = null
    LLVMDisposeExecutionEngine(g_engine); g_engine = null

class LlvmJitVisitor : AstVisitor
    e2v : table<Expression?;LLVMOpaqueValue?>
    str2v : table<string; LLVMOpaqueValue?>
    errors : array<string>
    fnmna : string
    ffunc : LLVMOpaqueValue?
    def LlvmJitVisitor
        pass

    def failed ( e:ExpressionPtr; msg:string )
        errors |> push("{msg}, expr = {e.__rtti} at {describe(e.at)}")

    def setR ( e : ExpressionPtr; ov:LLVMOpaqueValue? )
        e2v[get_ptr(e)] = ov

    def getR ( e : ExpressionPtr )
        var res : LLVMOpaqueValue?
        e2v |> find_if_exists(get_ptr(e)) <| $ ( val )
            res = *val
        if res == null
            res = LLVMGetUndef(LLVMInt32Type())
            failed(e, "unresolved expression")
        return res

    def base_type_to_llvm_type ( e:ExpressionPtr; t:Type )
        if t == Type tInt || t == Type tUInt || t == Type tBitfield
            return LLVMInt32Type()
        if t == Type tInt64 || t == Type tUInt64
            return LLVMInt64Type()
        if t == Type tInt16 || t == Type tUInt16
            return LLVMInt16Type()
        if t == Type tInt8 || t == Type tUInt8 || t == Type tBool
            return LLVMInt8Type()
        elif t == Type tFloat
            return LLVMFloatType()
        elif t == Type tDouble
            return LLVMDoubleType()
        else
            failed(e, "unsupported type {t}")
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

    def override visitExprVar(expr:smart_ptr<ExprVar>) : ExpressionPtr
        if expr.varFlags._block
            failed(expr, "{expr.variable.name} support block variable")
        elif expr.varFlags.local    // local varaibels
            failed(expr, "{expr.variable.name} support local variable")
        elif expr.varFlags.argument // arguments
            var args = LLVMGetParam(ffunc, 1u)
            var arg_ptr = LLVMBuildGEP(g_builder, args, LLVMConstInt(LLVMInt32Type(), uint64(expr.argumentIndex), 0), "arg_ptr_{expr.argumentIndex}")
            if expr.varFlags.r2v
                assert(expr._type.isWorkhorseType,"r2v only for workhorse types")
                setR(expr, LLVMBuildLoad2(g_builder, base_type_to_llvm_type(expr, expr._type.baseType), arg_ptr, "arg_r2v_{expr.argumentIndex}"))
            else
                setR(expr, arg_ptr)
        else
            failed(expr, "ExprLetVariable {expr.variable.name} : {describe(expr.variable._type)}")
        return expr

    def override visitExprReturn(expr:smart_ptr<ExprReturn>) : ExpressionPtr
        if expr.subexpr != null
            assume etype = expr.subexpr._type
            var vres = LLVMGetUndef(LLVMFloat4Type())
            var tres : LLVMOpaqueValue?
            var tsrc = getR(expr.subexpr)
            if etype.isWorkhorseType
                var tbit = LLVMBuildBitCast(g_builder, tsrc, LLVMFloatType(), "tbit")
                tres = LLVMBuildInsertElement(g_builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "return_result")
            else
                failed(expr.subexpr, "unsupported return type {describe(etype)}")
            LLVMBuildRet(g_builder, tres)
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

    def override visitExprOp2(expr:smart_ptr<ExprOp2>) : ExpressionPtr
        var left = getR(expr.left)
        var right = getR(expr.right)
        if expr.op=="+"
            if expr._type.isInteger
                setR(expr, LLVMBuildAdd(g_builder, left, right, "op_add"))
            elif expr._type.isFloatOrDouble
                setR(expr, LLVMBuildFAdd(g_builder, left, right, "op_add"))
            else
                failed(expr, "unsupported add type {expr._type}")
        elif expr.op=="-"
            if expr._type.isInteger
                setR(expr, LLVMBuildSub(g_builder, left, right, "op_sub"))
            elif expr._type.isFloatOrDouble
                setR(expr, LLVMBuildFSub(g_builder, left, right, "op_sub"))
            else
                failed(expr, "unsupported sub type {expr._type}")
        elif expr.op=="*"
            if expr._type.isInteger
                setR(expr, LLVMBuildMul(g_builder, left, right, "op_sub"))
            elif expr._type.isFloatOrDouble
                setR(expr, LLVMBuildFMul(g_builder, left, right, "op_sub"))
            else
                failed(expr, "unsupported mul type {expr._type}")
        elif expr.op=="/"
            if expr._type.isInteger // for integer division we check division by zero
                // if right==0
                //  jit_exception("division by zero", context)
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
            if expr._type.isSignedInteger
                setR(expr, LLVMBuildSDiv(g_builder, left, right, "op_sub"))
            elif expr._type.isUnsignedInteger
                setR(expr, LLVMBuildUDiv(g_builder, left, right, "op_sub"))
            elif expr._type.isFloatOrDouble
                setR(expr, LLVMBuildFDiv(g_builder, left, right, "op_sub"))
            else
                failed(expr, "unsupported div type {expr._type}")
        elif expr.op=="%"
            if expr._type.isSignedInteger
                setR(expr, LLVMBuildSRem(g_builder, left, right, "op_sub"))
            elif expr._type.isUnsignedInteger
                setR(expr, LLVMBuildURem(g_builder, left, right, "op_sub"))
            elif expr._type.isFloatOrDouble
                setR(expr, LLVMBuildFRem(g_builder, left, right, "op_sub"))
            else
                failed(expr, "unsupported div type {expr._type}")
        else
            failed(expr, "unsupported binary operator {expr.op}")
        return expr

// constants
    def override visitExprConstPtr(expr:smart_ptr<ExprConstPtr>) : ExpressionPtr
        setR(expr, LLVMConstPointerNull(LLVMVoidType()))
        return expr
    def override visitExprConstEnumeration(expr:smart_ptr<ExprConstEnumeration>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstBitfield(expr:smart_ptr<ExprConstBitfield>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt32Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt8(expr:smart_ptr<ExprConstInt8>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt8Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt16(expr:smart_ptr<ExprConstInt16>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt16Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt64(expr:smart_ptr<ExprConstInt64>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt64Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt(expr:smart_ptr<ExprConstInt>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt32Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstInt2(expr:smart_ptr<ExprConstInt2>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstInt3(expr:smart_ptr<ExprConstInt3>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstInt4(expr:smart_ptr<ExprConstInt4>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstUInt8(expr:smart_ptr<ExprConstUInt8>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt8Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstUInt16(expr:smart_ptr<ExprConstUInt16>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt16Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstUInt64(expr:smart_ptr<ExprConstUInt64>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt64Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstUInt(expr:smart_ptr<ExprConstUInt>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt32Type(), uint64(expr.value), 0))
        return expr
    def override visitExprConstUInt2(expr:smart_ptr<ExprConstUInt2>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstUInt3(expr:smart_ptr<ExprConstUInt3>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstUInt4(expr:smart_ptr<ExprConstUInt4>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstRange(expr:smart_ptr<ExprConstRange>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstURange(expr:smart_ptr<ExprConstURange>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstBool(expr:smart_ptr<ExprConstBool>) : ExpressionPtr
        setR(expr, LLVMConstInt(LLVMInt8Type(), expr.value ? 1ul : 0ul, 0))
        return expr
    def override visitExprConstFloat(expr:smart_ptr<ExprConstFloat>) : ExpressionPtr
        setR(expr, LLVMConstReal(LLVMFloatType(), double(expr.value)))
        return expr
    def override visitExprConstFloat2(expr:smart_ptr<ExprConstFloat2>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstFloat3(expr:smart_ptr<ExprConstFloat3>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstFloat4(expr:smart_ptr<ExprConstFloat4>) : ExpressionPtr
        // TODO: implement
        return expr
    def override visitExprConstString(expr:smart_ptr<ExprConstString>) : ExpressionPtr
        setR(expr, get_string_constant_ptr(string(expr.value)))
        return expr
    def override visitExprConstDouble(expr:smart_ptr<ExprConstDouble>) : ExpressionPtr
        setR(expr, LLVMConstReal(LLVMFloatType(), expr.value))
        return expr
    def override visitExprFakeContext(expr:smart_ptr<ExprFakeContext>) : ExpressionPtr
        setR(expr, LLVMGetParam(ffunc,0u))
        return expr
    def override visitExprFakeLineInfo(expr:smart_ptr<ExprFakeLineInfo>) : ExpressionPtr
        setR(expr, LLVMConstPointerNull(LLVMVoidType()))
        return expr

def public generate_llvm ( fn : FunctionPtr )
    if is_in_completion() || is_compiling_macros()
        return null
    init_jit()  // we can't be sure init is called before this
    var astVisitor = new LlvmJitVisitor()
    var astVisitorAdapter <- make_visitor(*astVisitor)
    visit(fn, astVisitorAdapter)
    var failed = false
    if length(astVisitor.errors) > 0
        let errors = astVisitor.errors |> join("\n")
        print("\n\nLLVM JIT FAILED:\n{errors}\n\n")
        failed = true
    if !LLVMVerifyModule(g_mod, LLVMVerifierFailureAction LLVMAbortProcessAction, false)
        failed = true
    var code = failed ? 0ul : LLVMGetFunctionAddress(g_engine, astVisitor.fnmna)
    astVisitorAdapter := null
    unsafe
        delete astVisitor
    return unsafe(reinterpret<void?> code)

[function_macro(name="jit_llvm")]
class Jit_LLVM_Macro : AstFunctionAnnotation
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        if is_in_completion()
            return true
        func.sideEffectFlags |= bitfield(int(SideEffects userScenario))
        var qblock <- quote() <|
            unsafe
                if get_ptr(this_program()) != null
                    var fnptr <- find_module_function_via_rtti(find_module_via_rtti(this_program(),PMOD),PFUN)
                    var gen = generate_llvm(fnptr)
                    if gen != null
                        gen |> instrument_jit(reinterpret<function> PFUN)
                    else
                        remove_jit(reinterpret<function> PFUN)
                    fnptr := null
                else
                    panic("missing 'options rtti'")
        apply_template(qblock) <| $ ( rules )
            rules |> replaceVariable("PFUN") <| new [[ExprAddr() target:=func.name]]
            rules |> replaceVariable("PMOD") <| new [[ExprConstString() value:=compiling_module().name]]
        var qres <- move_unquote_block(qblock)
        qres.blockFlags ^= ExprBlockFlags isClosure
        var reg <- setup_call_list("all`things`jit", func.at, true)
        reg.list |> emplace(qres)
        return true
