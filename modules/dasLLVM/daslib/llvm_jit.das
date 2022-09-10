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
    LLVMCreateJITCompilerForModule(g_engine, g_mod)

[finalize]
def private finalize_jit
    if is_in_completion() || is_compiling_macros()
        return
    if g_builder == null    // prevent double finalize
        return
    LLVMVerifyModule(g_mod, LLVMVerifierFailureAction LLVMReturnStatusAction, false)
    LLVMDumpModule(g_mod); g_mod = null
    LLVMDisposeBuilder(g_builder); g_builder = null
    LLVMDisposeExecutionEngine(g_engine); g_engine = null


class LlvmJitVisitor : AstVisitor
    e2v : table<Expression?;LLVMOpaqueValue?>
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
        if t == Type tInt8 || t == Type tUInt8
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

def public generate_llvm ( fn : FunctionPtr )
    if is_in_completion() || is_compiling_macros()
        return null

    init_jit()  // we can't be sure init is called before this

    print("generate_llvm\n")

    var astVisitor = new LlvmJitVisitor()
    var astVisitorAdapter <- make_visitor(*astVisitor)
    visit(fn, astVisitorAdapter)

    var failed = false

    if length(astVisitor.errors) > 0
        let errors = astVisitor.errors |> join("\n")
        print("\n\nLLVM JIT FAILED:\n{errors}\n\n")
        failed = true

    if !LLVMVerifyModule(g_mod, LLVMVerifierFailureAction LLVMReturnStatusAction, false)
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
