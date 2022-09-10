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

def init_jit
    g_mod = LLVMModuleCreateWithName("llvm_jit_module")
    g_builder = LLVMCreateBuilder()
    LLVMLinkInMCJIT()
    LLVMInitializeNativeTarget()
    LLVMInitializeNativeAsmPrinter()
    LLVMInitializeNativeAsmParser()
    LLVMCreateJITCompilerForModule(g_engine, g_mod)

def finalize_jit
    LLVMDumpModule(g_mod)
    LLVMDisposeBuilder(g_builder)
    LLVMDisposeExecutionEngine(g_engine)

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

    def override preVisitFunction(fun:FunctionPtr) : void
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
                let bt = expr._type.baseType
                if bt == Type tInt
                    var arg_value = LLVMBuildLoad2(g_builder, LLVMInt32Type(), arg_ptr, "arg_r2v_{expr.argumentIndex}")
                    setR(expr, arg_value)
                else
                    failed(expr, "{expr.variable.name} support r2v for {bt}")
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
                if etype.baseType == Type tInt
                    var tbit = LLVMBuildBitCast(g_builder, tsrc, LLVMFloatType(), "tbit")
                    tres = LLVMBuildInsertElement(g_builder, vres, tbit, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "return_result")
                else
                    failed(expr.subexpr, "unsupported workhorse return type {describe(etype)}")
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
            setR(expr, LLVMBuildAdd(g_builder, left, right, "add_result"))
        else
            failed(expr, "unsupported binary operator {expr.op}")
        return expr

def public generate_llvm ( fn : FunctionPtr )
    if is_in_completion()
        return null

    init_jit()

    var astVisitor = new LlvmJitVisitor()
    var astVisitorAdapter <- make_visitor(*astVisitor)
    visit(fn, astVisitorAdapter)

    var failed = false

    var error : string
    if LLVMVerifyModule(g_mod, LLVMVerifierFailureAction LLVMReturnStatusAction, safe_addr(error)) != 0
        let msg = clone_string(error)
        LLVMDisposeMessage(error)
        print("\n\nLLVM JIT FAILED:\n{error}\n\n")
        failed = true

    if length(astVisitor.errors) > 0
        let errors = astVisitor.errors |> join("\n")
        print("\n\nLLVM JIT FAILED:\n{errors}\n\n")
        failed = true

    var code = failed ? 0ul : LLVMGetFunctionAddress(g_engine, astVisitor.fnmna)

    astVisitorAdapter := null
    unsafe
        delete astVisitor

    // finalize_jit()
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
        let show_disasm = find_arg("asm",args) ?as tBool ?? false
        let verbose = find_arg("verbose",args) ?as tBool ?? false
        let opt_level = find_arg("o",args) ?as tInt ?? 3
        let no_exceptions = find_arg("no_exceptions",args) ?as tBool ?? false
        apply_template(qblock) <| $ ( rules )
            rules |> replaceVariable("PFUN") <| new [[ExprAddr() target:=func.name]]
            rules |> replaceVariable("PMOD") <| new [[ExprConstString() value:=compiling_module().name]]
        var qres <- move_unquote_block(qblock)
        qres.blockFlags ^= ExprBlockFlags isClosure
        var reg <- setup_call_list("all`things`jit", func.at, true)
        reg.list |> emplace(qres)
        return true
