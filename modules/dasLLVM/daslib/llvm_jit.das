options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module llvm_jit shared private

require llvm/llvm_boost
require llvm/llvm_jit_intrin
require llvm/llvm_jit_common
require daslib/ast_boost
require daslib/templates_boost
require daslib/safe_addr
require daslib/strings_boost
require daslib/defer
require daslib/fio
require math

[macro_function]
def private get_expr_ptr ( expr:ExpressionPtr )
    return get_ptr(expr)

[macro]
class LlvmJitCaptureBlock : AstVisitor
    [[do_not_delete]] scope : table<Variable?>
    [[do_not_delete]] capture : table<Variable?; ExprVar?>
    def override preVisitExprVar(expr:smart_ptr<ExprVar>) : void
        if !(expr.varFlags.local || expr.varFlags.argument || expr.varFlags._block)
            return
        let vptr = get_ptr(expr.variable)
        if !(scope |> key_exists(vptr))
            capture[vptr] = get_ptr(expr)
    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : void
        scope |> insert(get_ptr(arg))
    def override preVisitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool) : void
        scope |> insert(get_ptr(svar))
    def override preVisitExprBlockArgument(blk:smart_ptr<ExprBlock>;arg:VariablePtr;lastArg:bool): void
        scope |> insert(get_ptr(arg))

[macro_function]
def private capture_block(expr:ExpressionPtr) : array<tuple<variable:Variable?;expression:ExprVar?>>
    var astVisitor = new LlvmJitCaptureBlock()
    var adapter <- make_visitor(*astVisitor)
    visit(expr, adapter)
    var res <- [{for k,v in keys(astVisitor.capture),values(astVisitor.capture); [[auto k,v]] }]
    adapter := null
    unsafe
        delete astVisitor
    return <- res

struct private IteBlock
    [[do_not_delete]] if_true  : LLVMOpaqueBasicBlock?
    [[do_not_delete]] if_false : LLVMOpaqueBasicBlock?
    [[do_not_delete]] if_end   : LLVMOpaqueBasicBlock?
    if_true_terminates  : bool
    if_false_terminates : bool

struct private LoopBlock
    [[do_not_delete]] loop_start : LLVMOpaqueBasicBlock?
    [[do_not_delete]] loop_body  : LLVMOpaqueBasicBlock?
    [[do_not_delete]] loop_end   : LLVMOpaqueBasicBlock?

[macro]
class LlvmJitVisitor : AstVisitor
    adapter : smart_ptr<VisitorAdapter>
    fnmna : string
    [[do_not_delete]] e2v : table<Expression?;LLVMOpaqueValue?>
    [[do_not_delete]] v2v : table<Variable?;LLVMOpaqueValue?>
    [[do_not_delete]] call2cmres : table<Expression?;LLVMOpaqueValue?>
    [[do_not_delete]] ffunc, wfunc : LLVMOpaqueValue?
    [[do_not_delete]] ite2blocks : table<Expression?; IteBlock>
    [[do_not_delete]] thisFunc : Function?
    [[do_not_delete]] subblocks : table<LLVMOpaqueBasicBlock?; LLVMOpaqueBasicBlock?>
    [[do_not_delete]] sbstack : array<LLVMOpaqueBasicBlock?>
    [[do_not_delete]] loop_stack : array<LoopBlock>
    [[do_not_delete]] jit_context : Context?
    [[do_not_delete]] function_entry : LLVMOpaqueBasicBlock?
    [[do_not_delete]] function_body : LLVMOpaqueBasicBlock?
    [[do_not_delete]] current_block : LLVMOpaqueBasicBlock?
    [[do_not_delete]] monad2block : table<Expression?;LLVMOpaqueBasicBlock?>
    [[do_not_delete]] prologue : LLVMOpaqueValue?

    def LlvmJitVisitor ( ctx:Context? )
        jit_context = ctx

    def position_at_end ( blk:LLVMOpaqueBasicBlock? )
        current_block = blk
        LLVMPositionBuilderAtEnd(g_builder, blk)

    def at_function_entry ( blk:block )
        LLVMPositionBuilderAtEnd(g_builder, function_entry)
        invoke(blk)
        LLVMPositionBuilderAtEnd(g_builder, current_block)

    def build_llvm_function_pair ( fun_name:string; isCmres:bool; var arguments:dasvector`smart_ptr`Variable; var result:TypeDeclPtr )
        // implementation function
        var fun_args <-  [{for a in arguments; type_to_llvm_abi_type(a._type)}]
        fun_args |> push(LLVMPointerType(LLVMVoidType(), 0u))  // context
        var fun_type : LLVMOpaqueType?
        if isCmres
            fun_args |> push(LLVMPointerType(LLVMVoidType(), 0u))  // CMRES
            fun_type = LLVMFunctionType(LLVMVoidType(), fun_args)
        else
            fun_type = LLVMFunctionType(type_to_llvm_abi_type(result), fun_args)
        ffunc = LLVMAddFunction(g_mod, "{fun_name} implementation", fun_type)
        // setup parameter alignment
        var readonly_attr = LLVMGetEnumAttribute("readonly")
        for a,ai in arguments,urange(100500)
            if a._type.isRef
                LLVMSetParamAlignment(LLVMGetParam(ffunc,ai), uint(a._type.alignOf))
            elif a._type.isPointer && a._type.firstType!=null
                LLVMSetParamAlignment(LLVMGetParam(ffunc,ai), uint(a._type.firstType.alignOf))
            if (a._type.isRef || a._type.isPointer || a._type.isString) && a._type.flags.constant
                LLVMAddAttributeToFunctionArgument(ffunc, ai, readonly_attr)
            // todo: noalias?
        LLVMSetParamAlignment(get_context_param(), 16u)
        if isCmres
            LLVMSetParamAlignment(get_cmres_param(), uint(result.alignOf))
        // wrapper function
        var ret_type = LLVMFunctionType(LLVMFloat4Type(),
            [[LLVMTypeRef LLVMPointerType(LLVMVoidType(),0u);   // context
            LLVMPointerType(LLVMFloat4Type(),0u);               // args
            LLVMPointerType(LLVMVoidType(),0u)                  // cmres
        ]])
        wfunc = LLVMAddFunction(g_mod, fun_name, ret_type)
        LLVMSetParamAlignment(LLVMGetParam(wfunc,0u), 16u)
        LLVMSetParamAlignment(LLVMGetParam(wfunc,1u), 16u)
        if isCmres
            LLVMSetParamAlignment(LLVMGetParam(wfunc,2u), uint(result.alignOf))

    def add_llvm_functions ( var fun : FunctionPtr )
        thisFunc = get_ptr(fun)
        fnmna = get_mangled_name(fun)
        build_llvm_function_pair(fnmna, isCMRES(fun), fun.arguments, fun.result)
        thisFunc = null

    def append_basic_block ( name:string; add_to_phi:bool=false )
        var blk = LLVMAppendBasicBlock(ffunc,name)
        subblocks[blk] = blk
        if add_to_phi
            for st in sbstack
                subblocks[st] = blk
        return blk

    def setE ( e : ExpressionPtr; ov:LLVMOpaqueValue? )
        e2v[get_ptr(e)] = ov

    def getE_ptr ( e : Expression? )
        var res : LLVMOpaqueValue?
        e2v |> find_if_exists(e) <| $ ( val )
            res = *val
        if res == null
            res = LLVMGetUndef(LLVMInt32Type())
            failed_E(e, "unresolved expression at {describe(e.at)}")
        return res

    def getE ( e : ExpressionPtr )
        var res : LLVMOpaqueValue?
        e2v |> find_if_exists(get_ptr(e)) <| $ ( val )
            res = *val
        if res == null
            res = LLVMGetUndef(LLVMInt32Type())
            failed_E(e, "unresolved expression {describe(e)}")
        return res

    def tryE ( e : ExpressionPtr; blk:block<LLVMOpaqueValue?> )
        var res : LLVMOpaqueValue?
        e2v |> find_if_exists(get_ptr(e)) <| $ ( val )
            res = *val
        if res == null
            res = invoke(blk)
        return res

    def setV ( e : VariablePtr; ov:LLVMOpaqueValue? )
        v2v[get_ptr(e)] = ov

    def getV_ptr ( e : Variable? )
        var res : LLVMOpaqueValue?
        v2v |> find_if_exists(e) <| $ ( val )
            res = *val
        if res == null
            res = LLVMGetUndef(LLVMInt32Type())
            failed_V(e, "unresolved variable at line {int(e.at.line)}")
        return res


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

    def build_wrapper_funcion(var arguments:dasvector`smart_ptr`Variable; var result:TypeDeclPtr; isCmres:bool )
        var wentry = LLVMAppendBasicBlock(wfunc, "entry")
        LLVMPositionBuilderAtEnd(g_builder, wentry)
        var wargs : array<LLVMOpaqueValue?>
        var args = LLVMGetParam(wfunc,1u)
        for a,ai in arguments, range(100500)
            var arg_ptr = LLVMBuildGEP(g_builder, args, LLVMConstInt(LLVMInt32Type(), uint64(ai), 0), "arg_ptr_{ai}")
            var arg_v : LLVMOpaqueValue?
            if a._type.flags.ref
                arg_v = LLVMBuildLoad2(g_builder, type_to_llvm_abi_type(a._type), arg_ptr, "arg_ref_value_{ai}")
            elif !a._type.isRef
                arg_v = LLVMBuildLoad2(g_builder, type_to_llvm_abi_type(a._type), arg_ptr, "arg_value_{ai}")
            else
                arg_v = LLVMBuildLoad2(g_builder, type_to_llvm_abi_type(a._type), arg_ptr, "arg_any_{ai}")
            wargs |> push(arg_v)
        wargs |> push(LLVMGetParam(wfunc,0u))  // context
        if isCmres
            wargs |> push(LLVMGetParam(wfunc,2u))  // cmres
        var ret_v = LLVMBuildCall(g_builder, ffunc, wargs, "")
        if !result.isVoid
            if isCmres
                LLVMBuildRet(g_builder, cast_ptr_to_vec4f(LLVMGetParam(wfunc,2u)))
            else
                var tres = cast_to_vec4f(result, ret_v)
                if tres != null
                    LLVMBuildRet(g_builder, tres)
                else
                    LLVMBuildRet(g_builder, LLVMGetUndef(LLVMFloat4Type()))
        else
            LLVMBuildRet(g_builder, LLVMGetUndef(LLVMFloat4Type()))

    def override preVisitFunction(var fun:FunctionPtr) : void
        assert(g_builder!=null, "missing builder")
        thisFunc = get_ptr(fun)
        fnmna = get_mangled_name(fun)
        ffunc = LLVMGetNamedFunction(g_mod, "{fnmna} implementation")
        wfunc = LLVMGetNamedFunction(g_mod, fnmna)
        // build wrapper function
        build_wrapper_funcion(fun.arguments, fun.result, isCMRES(fun))
        // now write to regular function
        function_entry = LLVMAppendBasicBlock(ffunc, "entry")
        position_at_end(function_entry)
        // allocate prologue if need be
        if fun.flags.hasMakeBlock
            prologue = LLVMBuildAlloca(g_builder, g_t_stack_state, "prologue")
            prologue = LLVMBuildPointerCast(g_builder, prologue, LLVMPointerType(g_t_stack_state,0u), "")
            var params = [[auto
                LLVMConstInt(LLVMInt32Type(), uint64(fun.totalStackSize), 0);
                prologue;
                get_context_param()
            ]]
            LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, FN_JIT_PROLOGUE), params, "")
        // allocate arguments, which need to be promoted to local variables
        for a,ai in fun.arguments,range(10050)
            if (a.access_flags.access_ref || a.access_flags.access_pass) && !a._type.isRef
                var tryV = LLVMBuildAlloca(g_builder, type_to_llvm_abi_type(a._type), "var_{a.name}")
                LLVMSetAlignment(tryV, 16u)
                LLVMBuildStore(g_builder, LLVMGetParam(ffunc,uint(ai)), tryV)
                setV(a, tryV)

        function_body = LLVMAppendBasicBlock(ffunc, "body")
        position_at_end(function_body)

    def build_epilogue
        if prologue != null
            var params = [[auto
                prologue;
                get_context_param()
            ]]
            LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, FN_JIT_EPILOGUE), params, "")

    def override visitFunction(fun:FunctionPtr) : FunctionPtr
        if fun.body != null && !is_terminator_expression(fun.body)
            build_epilogue()
            LLVMBuildRetVoid(g_builder)
        LLVMPositionBuilderAtEnd(g_builder, function_entry)
        LLVMBuildBr(g_builder, function_body)
        ffunc = null
        thisFunc = null
        return fun

    def isCall2CMRES ( expr:ExpressionPtr )
        return (expr is ExprCall) && isCMRES((expr as ExprCall).func)

    def make_call_to_cmres ( expr:ExpressionPtr; v_ptr:LLVMOpaqueValue? )
        if isCall2CMRES(expr)
            call2cmres[get_ptr(expr)] = LLVMBuildPointerCast(g_builder, v_ptr, LLVMPointerType(LLVMVoidType(),0u), "call_result_{(expr as ExprCall).func.name}")
            return true
        elif (expr is ExprInvoke) && isCMRESType(expr._type)
            call2cmres[get_ptr(expr)] = LLVMBuildPointerCast(g_builder, v_ptr, LLVMPointerType(LLVMVoidType(),0u), "invoke_result_{int(expr.at.line)}")
            return true
        return false

    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : void
        var vtype, vptrtype : LLVMOpaqueType?
        if arg._type.flags.ref
            vtype = get_type_pointer(arg._type)
            vptrtype = LLVMPointerType(vtype,0u)
        else
            vtype = type_to_llvm_type(arg._type)
            vptrtype = get_type_pointer(arg._type)
        var v_ptr : LLVMOpaqueValue?
        if arg.flags.aliasCMRES
            v_ptr = get_cmres_param()
            v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, vptrtype, "variable_cmres_{arg.name}")
        else
            at_function_entry <|
                v_ptr = LLVMBuildAlloca(g_builder, vtype, "variable_{arg.name}")
                LLVMSetAlignment(v_ptr, 16u)
                v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, vptrtype, "variable_{arg.name}")
        setV(arg, v_ptr)
        if arg.init == null
            var vsize = arg._type.flags.ref ?  typeinfo(sizeof type<void?>) : arg._type.sizeOf
            LLVMBuildMemSet(g_builder, v_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(vsize), 0), 16u)
        elif make_call_to_cmres(arg.init, v_ptr)
            pass    // do nothing
        elif isMakeLocal(arg.init)
            var lptr = LLVMBuildPointerCast(g_builder, v_ptr, LLVMPointerType(LLVMInt8Type(),0u), "alias_make_local_{arg.name}")
            setE(arg.init, lptr)

    def override visitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr) : ExpressionPtr
        var tdst = getV(arg)
        if isCall2CMRES(expr)
            pass    // this variable is initialized via cmres call
        elif arg._type.flags.ref
            var src = getE(expr)
            LLVMBuildStore(g_builder, src, tdst)
        elif isMakeLocal(arg.init)
            pass
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
                setE(expr, LLVMBuildLoad(g_builder, var_ptr, "var_r2v_{expr.variable.name}"))
            else
                setE(expr, var_ptr)
        elif expr.varFlags.argument // arguments
            var tryV = tryGetV(expr.variable)
            if tryV != null
                if expr.varFlags.r2v
                    setE(expr, LLVMBuildLoad(g_builder, tryV, "var_r2v_{expr.variable.name}"))
                else
                    setE(expr, tryV)
                return expr
            var arg = LLVMGetParam(ffunc,uint(expr.argumentIndex))
            if expr.varFlags.r2v
                if varType.flags.ref
                    setE(expr, LLVMBuildLoad(g_builder, arg, "var_ref_r2v_{expr.variable.name}"))
                else
                    setE(expr, arg)
            else
                setE(expr, arg)
        else
            var vptr : LLVMOpaqueValue?
            var ctx_ptr = LLVMBuildPointerCast(g_builder, get_context_param(), LLVMPointerType(LLVMInt8Type(),0u), "")
            var global_ptr = LLVMBuildGEP(g_builder, ctx_ptr, LLVMConstInt(LLVMInt32Type(), uint64(CONTEXT_OFFSET_OF_GLOBALS), 0), "")
            global_ptr = LLVMBuildLoad2(g_builder, LLVMPointerType(LLVMInt8Type(),0u), global_ptr, "")
            vptr = LLVMBuildGEP(g_builder, global_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.variable.stackTop), 0), "")
            vptr = LLVMBuildPointerCast(g_builder, vptr, get_type_pointer(expr.variable._type), "global_var_{expr.variable.name}")
            if expr.varFlags.r2v
                setE(expr, LLVMBuildLoad(g_builder, vptr, "global_var_r2v_{expr.variable.name}"))
            else
                setE(expr, vptr)
        return expr

    def cast_from_vec4f_ptr ( v4f:LLVMOpaqueValue?; ptr_type:LLVMOpaqueType? ) : LLVMOpaqueValue?
        var tres : LLVMOpaqueValue?
        if typeinfo(sizeof type<void?>)==8
            var v2 = LLVMBuildBitCast(g_builder, v4f, LLVMVectorType(LLVMInt64Type(),2u), "ptr_vec")
            var tbit = LLVMBuildExtractElement(g_builder, v2, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "ptr_bits")
            tres = LLVMBuildIntToPtr(g_builder, tbit, ptr_type, "cast_from_vec4f_ptr")
        else
            var tbit = LLVMBuildExtractElement(g_builder, v4f, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "cast_from_vec4f_bits")
            tres = LLVMBuildBitCast(g_builder, tbit, LLVMInt32Type(), "ptr_bits")
            tres = LLVMBuildIntToPtr(g_builder, tres, ptr_type, "cast_from_vec4f_ptr")
        return tres

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
            elif bt == Type tString || bt == Type tPointer
                var ptr_type = bt==Type tString ? get_type_string() : type_to_llvm_type(etype)
                tres = cast_from_vec4f_ptr(v4f, ptr_type)
            else
                failed_E(expr, "unsupported cast_from_vec4f cast {describe(etype)}")
        elif etype.isRef
            var ptr_type = LLVMPointerType(type_to_llvm_type(etype),0u)
            tres = cast_from_vec4f_ptr(v4f, ptr_type)
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
        build_epilogue()
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

    def get_context_param : LLVMOpaqueValue?
        return LLVMGetParam(ffunc,uint(thisFunc.arguments.length))

    def get_cmres_param : LLVMOpaqueValue?
        return LLVMGetParam(ffunc,uint(thisFunc.arguments.length+1))

    def build_exception ( message:string )
        var params = [[auto
            get_string_constant_ptr(message);   // message
            get_context_param()
        ]]
        LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, FN_JIT_EXCEPTION), params, "")

    def build_array_lock ( array_ptr:LLVMOpaqueValue? )
        var params = [[auto
            LLVMBuildPointerCast(g_builder, array_ptr, LLVMPointerType(LLVMVoidType(),0u), "");              // array
            get_context_param()
        ]]
        LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, FN_JIT_ARRAY_LOCK), params, "")

    def build_array_unlock ( array_ptr:LLVMOpaqueValue? )
        var params = [[auto
            LLVMBuildPointerCast(g_builder, array_ptr, LLVMPointerType(LLVMVoidType(),0u), "");              // array
            get_context_param()
        ]]
        LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, FN_JIT_ARRAY_UNLOCK), params, "")

    def build_copy ( tdst:LLVMOpaqueValue?; right:ExpressionPtr )
        var tsrc = getE(right)
        if right._type.isRef
            if right._type.isWorkhorseType
                var temp = LLVMBuildLoad(g_builder, tsrc, "")
                LLVMBuildStore(g_builder, temp, tdst)
            else
                var rsize = LLVMConstInt(LLVMInt32Type(), uint64(right._type.sizeOf), 0)
                var ralign = uint(right._type.alignOf)
                LLVMBuildMemCpy(g_builder, tdst, ralign, tsrc, ralign, rsize)
        else
            if right is ExprConstPtr
                let null_ptr = LLVMConstPointerNull(LLVMPointerType(LLVMVoidType(),0u))
                LLVMBuildStore2(g_builder, LLVMPointerType(LLVMVoidType(),0u), null_ptr, tdst)
            else
                LLVMBuildStore(g_builder, tsrc, tdst)

    def override preVisitExprCopyRight(expr:smart_ptr<ExprCopy>;right:ExpressionPtr): void
        var tdst = getE(expr.left)
        if make_call_to_cmres(expr.right, tdst)
            pass
        elif expr.takeOverRightStack
            assert(isMakeLocal(expr.right))
            var mk_ptr = LLVMBuildPointerCast(g_builder, tdst, LLVMPointerType(LLVMInt8Type(),0u), "")
            setE(expr.right, mk_ptr)

    def override visitExprCopy(expr:smart_ptr<ExprCopy>) : ExpressionPtr
        if isCall2CMRES(expr.right)
            pass    // already a call to cmres, no need to copy
        else
            var tdst = getE(expr.left)
            build_copy(tdst, expr.right)
        return expr

    def check_ptr_zero ( tsrc:LLVMOpaqueValue?; message:string="dereferencing null pointer")
        var check_null_ptr = append_basic_block("check_null_ptr")
        var check_true = append_basic_block("check_true")
        var check_end = append_basic_block("check_end")
        LLVMBuildBr(g_builder, check_null_ptr)
        position_at_end(check_null_ptr)
        var cond = LLVMBuildIsNull(g_builder, tsrc, "null_check")
        LLVMBuildCondBr(g_builder, cond, check_true, check_end)
        position_at_end(check_true)
        build_exception(message)
        LLVMBuildBr(g_builder, check_end)
        position_at_end(check_end)

    def override visitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>) : ExpressionPtr
        var tsrc = getE(expr.subexpr)
        if !thisFunc.flags.unsafeDeref
            check_ptr_zero(tsrc)
        setE(expr, tsrc)
        return expr

    def override visitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>) : ExpressionPtr
        var tsrc = getE(expr.subexpr)
        setE(expr, tsrc)
        return expr

    def check_range ( tidx, maxIdx:LLVMOpaqueValue?; message:string )
        var check_null_ptr = append_basic_block("check_dim_range")
        var check_true = append_basic_block("check_true")
        var check_end = append_basic_block("check_end")
        LLVMBuildBr(g_builder, check_null_ptr)
        position_at_end(check_null_ptr)
        var cond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntUGE, tidx, maxIdx, "range_check")
        LLVMBuildCondBr(g_builder, cond, check_true, check_end)
        position_at_end(check_true)
        build_exception(message)
        LLVMBuildBr(g_builder, check_end)
        position_at_end(check_end)

    def build_table_at ( ttype:Type; tab:LLVMOpaqueValue?; key:LLVMOpaqueValue?; valueTypeSize:int ) : LLVMOpaqueValue?
        var params = [[auto
            tab;        // table                                        // table ptr
            key;                                                        // key
            LLVMConstInt(LLVMInt32Type(), uint64(valueTypeSize), 0);    // valueTypeSize
            get_context_param()
        ]]
        var tab_fun = LLVMGetNamedFunction(g_mod, FN_JIT_TABLE_AT(ttype))
        if tab_fun==null
            error("table_at {ttype} function not found")
            return LLVMGetUndef(LLVMInt32Type())
        return LLVMBuildCall(g_builder, tab_fun, params, "")

    def override visitExprAt(expr:smart_ptr<ExprAt>) : ExpressionPtr
        var tsrc = getE(expr.subexpr)
        var tidx = getE(expr.index)
        assume subExprT = expr.subexpr._type
        if subExprT.isPointer
            if !thisFunc.flags.unsafeDeref
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
            check_range(tidx, LLVMConstInt(LLVMInt32Type(), uint64(maxIndex), 0), "dim index out of range")
            var ptr_idx = LLVMBuildGEP2(g_builder, element_type, tsrc, tidx, "dim_at")
            if expr.atFlags.r2v
                var at_r2v = LLVMBuildLoad(g_builder, ptr_idx, "dim_at_r2v")
                setE(expr, at_r2v)
            else
                setE(expr, ptr_idx)
        elif subExprT.baseType == Type tArray
            var arr = LLVMBuildLoad(g_builder, tsrc, "arr")
            var size = LLVMBuildExtractValue(g_builder, arr, uint(JIT_ARRAY SIZE), "array.size")
            check_range(tidx, size, "array index out of range")
            var data = LLVMBuildExtractValue(g_builder, arr, uint(JIT_ARRAY DATA), "array.data")
            var ptr_idx = LLVMBuildGEP(g_builder, data, tidx, "array_at")
            if expr.atFlags.r2v
                var at_r2v = LLVMBuildLoad(g_builder, ptr_idx, "array_at_r2v")
                setE(expr, at_r2v)
            else
                setE(expr, ptr_idx)
            //  failed("array: unsupported visitExprAt {describe(tsrc)}[{describe(tidx)}] type {describe(subExprT)}")
        elif subExprT.baseType == Type tTable
            var tab = LLVMBuildLoad(g_builder, tsrc, "tab")
            var tab_ptr = LLVMBuildPointerCast(g_builder, tsrc, LLVMPointerType(LLVMVoidType(),0u), "tab_ptr")
            var at_tab = build_table_at(subExprT.firstType.baseType, tab_ptr, tidx, subExprT.secondType.sizeOf)
            var data = LLVMBuildExtractValue(g_builder, tab, uint(JIT_TABLE DATA), "table.data")
            var ptr_idx = LLVMBuildGEP(g_builder, data, at_tab, "table_at")
            if expr.atFlags.r2v
                var at_r2v = LLVMBuildLoad(g_builder, ptr_idx, "array_at_r2v")
                setE(expr, at_r2v)
            else
                setE(expr, ptr_idx)
        else
            failed("unsupported visitExprAt type {describe(subExprT)}")
        return expr

    def isReadModifyWriteOp2 ( name:das_string )
        return (name=="+=" || name=="-=" || name=="*=" || name=="/=" || name=="%=" ||
            name=="&=" || name=="|=" || name=="^=" || name=="<<=" || name==">>=" ||
            name=="<<<=" || name==">>>=" || name=="&&=" || name=="||=" || name=="^^=")

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
        elif expr.op=="~"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildNot(g_builder, sexpr, ""))
            else
                failed_E(expr, "unsupported ~ type {opType}")
        elif expr.op=="!"
            if opType.isBool
                setE(expr, LLVMBuildNot(g_builder, sexpr, ""))
            else
                failed_E(expr, "unsupported ! type {opType}")
        elif expr.op=="-"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildNeg(g_builder, sexpr, ""))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFNeg(g_builder, sexpr, ""))
            else
                failed_E(expr, "unsupported ~ type {opType}")
        else
            failed_E(expr, "unsupported unary operator {expr.op}")
        return expr

    def check_divide_by_0 ( right:LLVMOpaqueValue?; opType:TypeDeclPtr )
        var check_div_0 = append_basic_block("check_div_0")
        var check_true = append_basic_block("check_true")
        var check_end = append_basic_block("check_end")
        LLVMBuildBr(g_builder, check_div_0)
        position_at_end(check_div_0)
        let zero =  LLVMConstInt(base_type_to_llvm_type(opType.baseType), 0ul, 0)
        let cond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, right, zero, "cmp_is_zero")
        LLVMBuildCondBr(g_builder, cond, check_true, check_end)
        position_at_end(check_true)
        build_exception("division by zero")
        LLVMBuildBr(g_builder, check_end)
        position_at_end(check_end)

    def expand_scalar ( scalar:LLVMOpaqueValue?; vecType:TypeDeclPtr )
        var vec = LLVMGetUndef(type_to_llvm_type(vecType))
        for d in range(vecType.vectorDim)
            var idx = LLVMConstInt(LLVMInt32Type(), uint64(d), 0)
            vec = LLVMBuildInsertElement(g_builder, vec, scalar, idx, "")
        return vec

    def isExprIntNZ ( expr:ExpressionPtr )
        return true     if (expr is ExprConstInt) && (expr as ExprConstInt).value!=0
        return true     if (expr is ExprConstUInt) && (expr as ExprConstUInt).value!=0u
        return true     if (expr is ExprConstInt64) && (expr as ExprConstInt64).value!=0l
        return true     if (expr is ExprConstUInt64) && (expr as ExprConstUInt64).value!=0ul
        return false

    def build_str_cmp ( a,b:LLVMOpaqueValue? ) : LLVMOpaqueValue?
        return LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod,FN_JIT_STR_CMP), [[auto a;b]], "")

    def reduce_bit_vector ( i1v:LLVMOpaqueValue?; bits:int; op:string )
        var result = LLVMBuildExtractElement(g_builder, i1v, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "bit_0")
        for b in range(1,bits)
            var bit = LLVMBuildExtractElement(g_builder, i1v, LLVMConstInt(LLVMInt32Type(), uint64(b), 0), "bit_{b}")
            if op=="=="
                result = LLVMBuildAnd(g_builder, result, bit, "")
            elif op=="!="
                result = LLVMBuildOr(g_builder, result, bit, "")
            else
                error("unsupported bit reduction operator {op}")
        return result

    def visitExprOp2_NonMonade(expr:smart_ptr<ExprOp2>)
        var left = getE(expr.left)
        var right = getE(expr.right)
        assume opType = expr.right._type
        assume shiftType = expr.left._type
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

        // note: we need to handle shift operations
        // the way we do it, is we expand scalar up to vector type
        if expr.op=="<<" || expr.op==">>" || expr.op==">>>" || expr.op=="<<<" || expr.op=="<<=" || expr.op==">>=" || expr.op==">>>=" || expr.op=="<<<="
            right = build_broadcast_vector(shiftType, right)

        if expr.op=="+"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildAdd(g_builder, left, right, ""))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFAdd(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported + type {describe(opType)}")
        elif expr.op=="+="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildAdd(g_builder, r2v_left, right, ""), left))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildFAdd(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported += type {describe(opType)}")
        elif expr.op=="-"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildSub(g_builder, left, right, ""))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFSub(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported - type {describe(opType)}")
        elif expr.op=="-="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildSub(g_builder, r2v_left, right, ""), left))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildFSub(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported -= type {describe(opType)}")
        elif expr.op=="*"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildMul(g_builder, left, right, ""))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildFMul(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported * type {describe(opType)}")
        elif expr.op=="*="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildMul(g_builder, r2v_left, right, ""), left))
            elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildFMul(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported *= type {describe(opType)}")
        elif expr.op=="/" || expr.op=="%" || expr.op=="/=" || expr.op=="%="
            // for integer division we check division by zero
            if opType.isInteger && expr.left._type.isInteger
                if !isExprIntNZ(expr.right)
                    check_divide_by_0(right, opType)
            if expr.op=="/"
                if opType.isSignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildSDiv(g_builder, left, right, ""))
                elif opType.isUnsignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildUDiv(g_builder, left, right, ""))
                elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildFDiv(g_builder, left, right, ""))
                else
                    failed_E(expr, "unsupported /= type {describe(opType)}")
            elif expr.op=="/="
                if opType.isSignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildSDiv(g_builder, r2v_left, right, ""), left))
                elif opType.isUnsignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildUDiv(g_builder, r2v_left, right, ""), left))
                elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildFDiv(g_builder, r2v_left, right, ""), left))
                else
                    failed_E(expr, "unsupported /= type {describe(opType)}")
            elif expr.op=="%"
                if opType.isSignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildSRem(g_builder, left, right, ""))
                elif opType.isUnsignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildURem(g_builder, left, right, ""))
                elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildFRem(g_builder, left, right, ""))
                else
                    failed_E(expr, "unsupported % type {describe(opType)}")
            elif expr.op=="%="
                if opType.isSignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tInt)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildSRem(g_builder, r2v_left, right, ""), left))
                elif opType.isUnsignedInteger || (opType.isVectorType && opType.vectorBaseType==Type tUInt)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildURem(g_builder, r2v_left, right, ""), left))
                elif opType.isFloatOrDouble || (opType.isVectorType && opType.vectorBaseType==Type tFloat)
                    setE(expr, LLVMBuildStore(g_builder, LLVMBuildFRem(g_builder, r2v_left, right, ""), left))
                else
                    failed_E(expr, "unsupported %= type {describe(opType)}")
            else
                failed_E(expr, "unsupported division type operator {expr.op}")
        elif expr.op=="<" || expr.op==">" || expr.op=="<=" || expr.op==">=" || expr.op=="==" || expr.op=="!="
            var opR : LLVMOpaqueValue?
            if expr.left._type.isSignedInteger || (expr.left._type.isVectorType && expr.left._type.vectorBaseType==Type tInt)
                opR = LLVMBuildICmp(g_builder, get_int_compare_op(expr,string(expr.op)), left, right, "")
                if expr.left._type.isVectorType
                    opR = reduce_bit_vector(opR, expr.left._type.vectorDim, string(expr.op))
            elif expr.left._type.isUnsignedInteger || (expr.left._type.isVectorType && expr.left._type.vectorBaseType==Type tUInt)
                opR = LLVMBuildICmp(g_builder, get_uint_compare_op(expr,string(expr.op)), left, right, "")
                if expr.left._type.isVectorType
                    opR = reduce_bit_vector(opR, expr.left._type.vectorDim, string(expr.op))
            elif expr.left._type.isFloatOrDouble || (expr.left._type.isVectorType && expr.left._type.vectorBaseType==Type tFloat)
                opR = LLVMBuildFCmp(g_builder, get_float_compare_op(expr,string(expr.op)), left, right, "")
                if expr.left._type.isVectorType
                    opR = reduce_bit_vector(opR, expr.left._type.vectorDim, string(expr.op))
            elif expr.left._type.baseType == Type tBool
                opR = LLVMBuildICmp(g_builder, get_bool_compare_op(expr,string(expr.op)), left, right, "")
            elif expr.left._type.baseType == Type tPointer
                var lptr = LLVMBuildPointerCast(g_builder, left, LLVMPointerType(LLVMInt8Type(), 0u), "")
                var rptr = LLVMBuildPointerCast(g_builder, right, LLVMPointerType(LLVMInt8Type(), 0u), "")
                opR = LLVMBuildICmp(g_builder, get_uint_compare_op(expr,string(expr.op)), lptr, rptr, "")
            elif expr.left._type.baseType == Type tString
                var cmpR = build_str_cmp(left, right)
                opR = LLVMBuildICmp(g_builder, get_int_compare_op(expr,string(expr.op)), cmpR, LLVMConstInt(LLVMInt32Type(), 0ul, 0), "")
            if opR == null
                opR = LLVMGetUndef(LLVMInt8Type())
                failed_E(expr, "unsupported compare type {describe(expr.left._type)} {expr.op} {describe(expr.right._type)}")
            setE(expr, opR)
        elif expr.op=="<<"
            if shiftType.isInteger || (shiftType.isVectorType && (shiftType.vectorBaseType==Type tInt || shiftType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildShl(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported << type {describe(shiftType)}")
        elif expr.op=="<<="
            if shiftType.isInteger || (shiftType.isVectorType && (shiftType.vectorBaseType==Type tInt || shiftType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildShl(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported <<= type {describe(shiftType)}")
        elif expr.op==">>"
            if shiftType.isSignedInteger || (shiftType.isVectorType && shiftType.vectorBaseType==Type tInt)
                setE(expr, LLVMBuildAShr(g_builder, left, right, ""))
            elif shiftType.isUnsignedInteger || (shiftType.isVectorType && shiftType.vectorBaseType==Type tUInt)
                setE(expr, LLVMBuildLShr(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported >> type {describe(shiftType)}")
        elif expr.op==">>="
            if shiftType.isSignedInteger || (shiftType.isVectorType && shiftType.vectorBaseType==Type tInt)
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildAShr(g_builder, r2v_left, right, ""), left))
            elif shiftType.isUnsignedInteger || (shiftType.isVectorType && shiftType.vectorBaseType==Type tUInt)
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildLShr(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported >>= type {describe(shiftType)}")
        elif expr.op==">>>" || expr.op=="<<<"
            if shiftType.isInteger
                var fshr_name = expr.op==">>>" ? "llvm.fshr.i32" : "llvm.fshl.i32"
                if shiftType.baseType==Type tInt64 || shiftType.baseType==Type tUInt64
                    fshr_name = expr.op==">>>" ? "llvm.fshr.i64" : "llvm.fshl.i64"
                var args <- [{auto left; left; right}]
                var argTypes <- [{auto base_type_to_llvm_type(shiftType.baseType)}]
                var id = LLVMLookupIntrinsicID(fshr_name)
                var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
                if decl==null
                    failed_E(expr, "failed to get intrinsic {fshr_name}")
                else
                    setE(expr, LLVMBuildCall(g_builder, decl, args, string(expr.op)))
            else
                failed_E(expr, "unsupported >>> type {opType}")
        elif expr.op==">>>=" || expr.op=="<<<="
            if shiftType.isInteger
                var fshr_name = expr.op==">>>=" ? "llvm.fshr.i32" : "llvm.fshl.i32"
                if shiftType.baseType==Type tInt64 || shiftType.baseType==Type tUInt64
                    fshr_name = expr.op==">>>=" ? "llvm.fshr.i64" : "llvm.fshl.i64"
                var args <- [{auto r2v_left; r2v_left; right}]
                var argTypes <- [{auto base_type_to_llvm_type(shiftType.baseType)}]
                var id = LLVMLookupIntrinsicID(fshr_name)
                var decl = LLVMGetIntrinsicDeclaration(g_mod, id, argTypes)
                if decl==null
                    failed_E(expr, "failed to get intrinsic {fshr_name}")
                else
                    var cll = LLVMBuildCall(g_builder, decl, args, string(expr.op))
                    setE(expr, LLVMBuildStore(g_builder, cll, left))
            else
                failed_E(expr, "unsupported >>> type {opType}")
        elif expr.op=="&"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildAnd(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported & type {opType}")
        elif expr.op=="&="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildAnd(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported &= type {opType}")
        elif expr.op=="|"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildOr(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported | type {opType}")
        elif expr.op=="|="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildOr(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported |= type {opType}")
        elif expr.op=="^"
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildXor(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported ^ type {opType}")
        elif expr.op=="^="
            if opType.isInteger || (opType.isVectorType && (opType.vectorBaseType==Type tInt || opType.vectorBaseType==Type tUInt))
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildXor(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported ^= type {opType}")
        elif expr.op=="^^"
            if opType.isBool
                setE(expr, LLVMBuildXor(g_builder, left, right, ""))
            else
                failed_E(expr, "unsupported ^^ type {opType}")
        elif expr.op=="^^="
            if opType.isBool
                setE(expr, LLVMBuildStore(g_builder, LLVMBuildXor(g_builder, r2v_left, right, ""), left))
            else
                failed_E(expr, "unsupported ^^= type {opType}")
        else
            failed_E(expr, "unsupported binary operator {expr.op}")

    def override preVisitExprOp2Right(expr:smart_ptr<ExprOp2>;right:ExpressionPtr): void
        if expr.op=="&&" || expr.op=="&&=" || expr.op=="||" || expr.op=="||="
            var monad_true = append_basic_block("monad_true_at_{int(expr.at.line)}")
            var monad_false = append_basic_block("monad_true_at_{int(expr.at.line)}")
            var monad_result, monad_cond : LLVMOpaqueValue?
            if expr.op=="&&" || expr.op=="||"
                at_function_entry <|
                    monad_result = LLVMBuildAlloca(g_builder, LLVMInt1Type(), "{expr.op}_at_{int(expr.at.line)}")
                monad_cond = getE(expr.left)
            else
                monad_result = getE(expr.left)
                monad_cond = LLVMBuildLoad(g_builder, monad_result, "")
            monad2block[get_ptr(ExpressionPtr(expr))] = monad_false
            setE(expr, monad_result)
            if expr.op=="&&" || expr.op=="||"
                LLVMBuildStore(g_builder, monad_cond, monad_result)
            if expr.op=="&&" || expr.op=="&&="
                LLVMBuildCondBr(g_builder, monad_cond, monad_true, monad_false)
            else
                LLVMBuildCondBr(g_builder, monad_cond, monad_false, monad_true)
            position_at_end(monad_true)

    def override visitExprOp2(expr:smart_ptr<ExprOp2>) : ExpressionPtr
        if expr.op=="&&" || expr.op=="&&=" || expr.op=="||" || expr.op=="||="
            var monad_false = monad2block[get_ptr(ExpressionPtr(expr))]
            LLVMBuildStore(g_builder, getE(expr.right), getE(expr))
            LLVMBuildBr(g_builder, monad_false)
            position_at_end(monad_false)
            if expr.op=="&&" || expr.op=="||"
                setE(expr, LLVMBuildLoad(g_builder, getE(expr), ""))
        else
            visitExprOp2_NonMonade(expr)
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
        return LLVMRealPredicate LLVMRealUNE    if op=="!="
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
        position_at_end(before_if)
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
        position_at_end(blk.if_true)
    def override preVisitExprIfThenElseElseBlock(expr:smart_ptr<ExprIfThenElse>;elseBlock:ExpressionPtr) : void
        var blk = ite2blocks[get_expr_ptr(expr)]
        if !blk.if_true_terminates
            LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_true block we jump to if_end
        position_at_end(blk.if_false)
    def override visitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>) : ExpressionPtr
        var blk = ite2blocks[get_expr_ptr(expr)]
        if expr.if_false == null
            if !blk.if_true_terminates
                LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_true block we jump to if_end
            position_at_end(blk.if_false)
        if !blk.if_false_terminates
            LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_false block we jump to if_end
        if blk.if_end != null
            position_at_end(blk.if_end)
        return expr

// op3
    def override preVisitExprOp3(expr:smart_ptr<ExprOp3>): void
        var before_if = append_basic_block("op3_cond_at_{expr.at.line}")
        LLVMBuildBr(g_builder, before_if)
        position_at_end(before_if)
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
        position_at_end(blk.if_true)
        sbstack |> push(blk.if_true)
    def override preVisitExprOp3Right(expr:smart_ptr<ExprOp3>;right:ExpressionPtr): void
        var blk = ite2blocks[get_expr_ptr(expr)]
        LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)   // after the end of if_true block we jump to if_end
        position_at_end(blk.if_false)
        sbstack |> pop()
        sbstack |> push(blk.if_false)
    def override visitExprOp3(expr:smart_ptr<ExprOp3>) : ExpressionPtr
        var blk = ite2blocks[get_expr_ptr(expr)]
        LLVMBuildBr(g_builder, ite2blocks[get_expr_ptr(expr)].if_end)
        position_at_end(blk.if_end)
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

// is variant
    def override visitExprIsVariant(expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
        var field_index = find_argument_index(expr.value._type, string(expr.name))
        if field_index==-1
            failed_E(expr, "variant field {expr.name} not found")
            return expr
        var vptr = getE(expr.value)
        var vint = LLVMBuildLoad2(g_builder, LLVMInt32Type(), vptr, "variant_index")
        let cond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, vint, LLVMConstInt(LLVMInt32Type(), uint64(field_index), 0), "is_variant")
        setE(expr, cond)
        return expr

    def check_variant_index ( tidx:LLVMOpaqueValue?; vidx:int; message:string )
        var cmpIdx = LLVMConstInt(LLVMInt32Type(), uint64(vidx), 0)
        var check_null_ptr = append_basic_block("check_variant_index")
        var check_true = append_basic_block("check_true")
        var check_end = append_basic_block("check_end")
        LLVMBuildBr(g_builder, check_null_ptr)
        position_at_end(check_null_ptr)
        var cond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntNE, tidx, cmpIdx, "index_check")
        LLVMBuildCondBr(g_builder, cond, check_true, check_end)
        position_at_end(check_true)
        build_exception(message)
        LLVMBuildBr(g_builder, check_end)
        position_at_end(check_end)

    def override visitExprAsVariant(expr:smart_ptr<ExprAsVariant>) : ExpressionPtr
        var field_index = find_argument_index(expr.value._type, string(expr.name))
        if field_index==-1
            failed_E(expr, "variant field {expr.name} not found")
            return expr
        var vptr = getE(expr.value)
        var vint = LLVMBuildLoad2(g_builder, LLVMInt32Type(), vptr, "variant_index")
        check_variant_index(vint, field_index, "variant mismatch, expecting {expr.value._type.argNames[field_index]}")
        let field_offset = get_variant_field_offset(expr.value._type,field_index)
        var fptr = LLVMBuildPointerCast(g_builder, vptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
        fptr = LLVMBuildGEP(g_builder, fptr, LLVMConstInt(LLVMInt32Type(), uint64(field_offset), 0), "")
        fptr = LLVMBuildPointerCast(g_builder, fptr, LLVMPointerType(type_to_llvm_type(expr._type), 0u), "variant_field_ptr")
        if expr.fieldFlags.r2v
            setE(expr, LLVMBuildLoad(g_builder, fptr, "variant_field"))
        else
            setE(expr, fptr)
        return expr

// function calls

    def build_array_of_arguments ( arguments:dasvector`smart_ptr`Expression; skip0:bool = false ) : LLVMOpaqueValue?
        var nargs = length(arguments)
        var from_ai = 0
        if skip0
            nargs --
            from_ai --
        var args : LLVMOpaqueValue?
        if nargs != 0
            args = LLVMBuildAlloca(g_builder, LLVMArrayType(LLVMFloat4Type(), uint(nargs)), "args")
            LLVMSetAlignment(args, 16u)
            args = LLVMBuildPointerCast(g_builder, args, LLVMPointerType(LLVMFloat4Type(), 0u), "call_args_ptr")
            for a,ai in arguments,range(from_ai,100500)
                if ai<0
                    continue
                var arg_ptr = LLVMBuildGEP(g_builder, args, LLVMConstInt(LLVMInt32Type(), uint64(ai), 0), "call_arg_{ai}_ptr")
                var arg_val = getE(a)
                var arg_cast_type = type_to_llvm_abi_type(a._type)
                if a._type.isRef
                    arg_val = LLVMBuildPointerCast(g_builder, arg_val, arg_cast_type, "call_arg_{ai}_cast")
                LLVMBuildStore2(g_builder, arg_cast_type, arg_val, arg_ptr)
        else
            args = LLVMConstPointerNull(LLVMPointerType(LLVMFloat4Type(), 0u))
        return args

    def register_extern_function ( expr:smart_ptr<ExprCallFunc> ) : LLVMOpaqueValue?
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

    def register_interop_function ( expr:smart_ptr<ExprCallFunc> ) : LLVMOpaqueValue?
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

    def build_call_params ( expr:smart_ptr<ExprCallFunc>; extern_func : Function? = null )
        var params : array<LLVMOpaqueValue?>
        for a,ai in expr.arguments,range(100500)
            if a._type.baseType==Type fakeContext
                params |> push(get_context_param())
            elif a._type.baseType==Type fakeLineInfo
                params |> push(LLVMConstPointerNull(LLVMVoidType()))
            elif a._type.baseType==Type tPointer
                assume funcArg = expr.func.arguments[ai]
                var ptrType = type_to_llvm_abi_type(funcArg._type)
                var ptrValue = LLVMBuildPointerCast(g_builder, getE(a), ptrType, "arg_{funcArg.name}_ptr")
                params |> push( ptrValue )
            elif (a._type.baseType==Type tArray || a._type.baseType==Type tTable) && (extern_func != null)
                assume funcArg = expr.func.arguments[ai]
                var destType = type_to_llvm_abi_type(funcArg._type)
                var arg = LLVMBuildPointerCast(g_builder, getE(a), destType, "any_array_{funcArg.name}_ptr")
                params |> push(arg)
            else
                params |> push(getE(a))
        return <- params

    def make_call ( expr:smart_ptr<ExprCallFunc>; doesNotNeedSp:bool )
        // lookup intrinsics first
        var intrin_call = lookup_intinsic(expr, [{for x in expr.arguments; getE(x)}])
        if intrin_call!=null
            setE(expr, intrin_call)
            return

        // get CMRES, if its local
        var cmresEval : LLVMOpaqueValue?
        if isCMRES(expr.func)
            if !doesNotNeedSp && expr.stackTop!=0u
                var vtype = type_to_llvm_type(expr._type)
                at_function_entry <|
                    cmresEval = LLVMBuildAlloca(g_builder, vtype, "cmres_eval_{expr.name}")
                    LLVMSetAlignment(cmresEval, 16u)
                    cmresEval = LLVMBuildPointerCast(g_builder, cmresEval, LLVMPointerType(LLVMVoidType(),0u), "cmres_eval_{expr.name}")
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
                    get_context_param();
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
                var params <- build_call_params(expr, expr.func)
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
                params |> push(get_context_param())
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
                params |> push(get_context_param())
                var func = LLVMGetNamedFunction(g_mod, cmresEval!=null ? FN_JIT_CALL_WITH_CMRES : FN_JIT_CALL_OR_FASTCALL)
                var ccall = LLVMBuildCall(g_builder, func, params, "")
                if cmresEval != null
                    setE(expr, cmresEval)
                elif expr.func.result.isVoid
                    setE(expr, ccall)
                else
                    var cres = cast_from_vec4f(expr, expr._type, ccall)
                    setE(expr, cres)

    def override visitExprCall(expr:smart_ptr<ExprCall>) : ExpressionPtr
        make_call(expr, expr.doesNotNeedSp)
        return expr

// invoke
    def build_invoke_params ( expr:smart_ptr<ExprInvoke> )
        assume blockT = expr.arguments[0]._type
        var params : array<LLVMOpaqueValue?>
        for a,ai in expr.arguments,range(100500)
            if ai==0        // skip first argument
                continue
            if a._type.baseType==Type fakeContext
                params |> push(get_context_param())
            elif a._type.baseType==Type fakeLineInfo
                params |> push(LLVMConstPointerNull(LLVMVoidType()))
            elif a._type.baseType==Type tPointer
                assume funcArg = blockT.argTypes[ai-1]
                var ptrType = type_to_llvm_abi_type(funcArg)
                var ptrValue = LLVMBuildPointerCast(g_builder, getE(a), ptrType, "arg_{ai-1}_ptr")
                params |> push( ptrValue )
            elif (a._type.baseType==Type tArray || a._type.baseType==Type tTable)
                assume funcArg = blockT.argTypes[ai-1]
                var destType = type_to_llvm_abi_type(funcArg)
                var arg = LLVMBuildPointerCast(g_builder, getE(a), destType, "any_array_{ai}_ptr")
                params |> push(arg)
            else
                params |> push(getE(a))
        return <- params

    def override visitExprInvoke(expr:smart_ptr<ExprInvoke>) : ExpressionPtr
        assume blockT = expr.arguments[0]._type
        var cmresEval : LLVMOpaqueValue?
        if isCMRESType(expr._type)
            if call2cmres |> key_exists(get_ptr(expr))
                cmresEval = call2cmres[get_ptr(ExpressionPtr(expr))]
            else
                var vtype = type_to_llvm_type(expr._type)
                at_function_entry <|
                    cmresEval = LLVMBuildAlloca(g_builder, vtype, "invoke_cmres_eval_{int(expr.at.line)}_var")
                    LLVMSetAlignment(cmresEval, 16u)
                    cmresEval = LLVMBuildPointerCast(g_builder, cmresEval, LLVMPointerType(LLVMVoidType(),0u), "invoke_cmres_eval_{int(expr.at.line)}_ptr")
        if blockT.isGoodFunctionType
            var args = build_array_of_arguments(expr.arguments, true)
            var FUNC = getE(expr.arguments[0])
            var SIMFUNCTION = LLVMBuildExtractValue(g_builder, FUNC, uint(JIT_FUNCTION SIM_FUNCTION), "SIMFUNCTION")
            SIMFUNCTION = LLVMBuildPointerCast(g_builder, SIMFUNCTION, LLVMPointerType(LLVMVoidType(), 0u), "")
            var params : array<LLVMOpaqueValue?>
            params |> push(SIMFUNCTION)
            params |> push(args)
            if cmresEval!=null
                params |> push(cmresEval)
            params |> push(get_context_param())
            var func = LLVMGetNamedFunction(g_mod, cmresEval!=null ? FN_JIT_CALL_WITH_CMRES : FN_JIT_CALL_OR_FASTCALL)
            var ccall = LLVMBuildCall(g_builder, func, params, "invoke")
            if cmresEval != null
                setE(expr, cmresEval)
            elif blockT.firstType.isVoid
                setE(expr, ccall)
            else
                var cres = cast_from_vec4f(expr, expr._type, ccall)
                setE(expr, cres)
        elif blockT.isGoodLambdaType
            var args = build_array_of_arguments(expr.arguments, false)
            var LAMBDA = getE(expr.arguments[0])
            LAMBDA = LLVMBuildLoad(g_builder, LAMBDA, "LAMBDA")
            var SIMFUNCTION = LLVMBuildExtractValue(g_builder, LAMBDA, uint(JIT_LAMBDA EVAL), "EVAL")
            SIMFUNCTION = LLVMBuildPointerCast(g_builder, SIMFUNCTION, LLVMPointerType(LLVMVoidType(), 0u), "")
            var params : array<LLVMOpaqueValue?>
            params |> push(SIMFUNCTION)
            params |> push(args)
            if cmresEval!=null
                params |> push(cmresEval)
            params |> push(get_context_param())
            var func = LLVMGetNamedFunction(g_mod, cmresEval!=null ? FN_JIT_CALL_WITH_CMRES : FN_JIT_CALL_OR_FASTCALL)
            var ccall = LLVMBuildCall(g_builder, func, params, "invoke")
            if cmresEval != null
                setE(expr, cmresEval)
            elif blockT.firstType.isVoid
                setE(expr, ccall)
            else
                var cres = cast_from_vec4f(expr, expr._type, ccall)
                setE(expr, cres)
        elif blockT.isGoodBlockType
            var args = build_array_of_arguments(expr.arguments, true)
            var blk = getE(expr.arguments[0])
            var params : array<LLVMOpaqueValue?>
            params |> push(blk)
            params |> push(args)
            if cmresEval!=null
                params |> push(cmresEval)
            params |> push(get_context_param())
            var func = LLVMGetNamedFunction(g_mod, cmresEval!=null ? FN_JIT_INVOKE_BLOCK_WITH_CMRES : FN_JIT_INVOKE_BLOCK)
            var ccall = LLVMBuildCall(g_builder, func, params, "invoke")
            if cmresEval != null
                setE(expr, cmresEval)
            elif blockT.firstType.isVoid
                setE(expr, ccall)
            else
                var cres = cast_from_vec4f(expr, expr._type, ccall)
                setE(expr, cres)

        else
            failed_E(expr, "invoke({describe(blockT)},...) is not supported yet")
        return expr

// string builder
    def override visitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>) : ExpressionPtr
        var args = build_array_of_arguments(expr.elements)
        var func = LLVMGetNamedFunction(g_mod, FN_JIT_STRING_BUILDER)
        assert(func!=null)
        let NODE_PTR = make_interop_node(*jit_context,get_ptr(expr))
        if NODE_PTR==null
            failed_E(expr, "missing interop function node pointer for String Builder")
        let NODE_ADDR = intptr(NODE_PTR)
        var params <- [{LLVMOpaqueValue?
            get_context_param();
            LLVMConstIntToPtr(LLVMConstInt(LLVMInt64Type(), NODE_ADDR, 0), LLVMPointerType(LLVMVoidType(), 0u)); // Node *
            args                                                   // args
        }]
        setE(expr, LLVMBuildCall(g_builder, func, params, "string_builder"))
        return expr

// swizzle

    def override visitExprSwizzle(expr:smart_ptr<ExprSwizzle>) : ExpressionPtr
        var val = getE(expr.value)
        if expr._type.flags.ref
            var val_ptr = LLVMBuildBitCast(g_builder, val, get_basetype_pointer(expr.value._type.vectorBaseType), "")
            val_ptr = LLVMBuildGEP(g_builder, val_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.fields[0]), 0), "")
            setE(expr, val_ptr)
        else
            var isSequencial = true
            for i in range(1,length(expr.fields))
                if (int(expr.fields[i-1])+1)!=int(expr.fields[i])
                    isSequencial = false
                    break
            if expr.value._type.flags.ref && isSequencial
                var val_ptr = LLVMBuildBitCast(g_builder, val, get_basetype_pointer(expr.value._type.vectorBaseType), "")
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
        elif expr.value._type.isTuple
            assert(expr.fieldIndex!=-1)
            var val_ptr = getE(expr.value)
            if expr.value._type.isPointer && !expr.derefFlags.unsafeDeref
                check_ptr_zero(val_ptr)
            val_ptr = LLVMBuildPointerCast(g_builder, val_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            let field_offset = get_tuple_field_offset(expr.value._type, expr.fieldIndex)
            var field_ptr = LLVMBuildGEP(g_builder, val_ptr, LLVMConstInt(LLVMInt32Type(), uint64(field_offset), 0), "")
            let argName = tuple_field_name(expr.value._type,expr.fieldIndex)
            field_ptr = LLVMBuildPointerCast(g_builder, field_ptr, get_type_pointer(expr.value._type.argTypes[expr.fieldIndex]), "{describe(expr.value._type,false,false,false)}.{argName}")
            if expr.fieldFlags.r2v
                setE(expr, LLVMBuildLoad(g_builder, field_ptr, ""))
            else
                setE(expr, field_ptr)
        elif expr.value._type.isVariant
            assert(expr.fieldIndex!=-1)
            var val_ptr = getE(expr.value)
            if expr.value._type.isPointer && !expr.derefFlags.unsafeDeref
                check_ptr_zero(val_ptr)
            val_ptr = LLVMBuildPointerCast(g_builder, val_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            let field_offset = get_variant_field_offset(expr.value._type, expr.fieldIndex)
            var field_ptr = LLVMBuildGEP(g_builder, val_ptr, LLVMConstInt(LLVMInt32Type(), uint64(field_offset), 0), "")
            let argName = variant_field_name(expr.value._type,expr.fieldIndex)
            field_ptr = LLVMBuildPointerCast(g_builder, field_ptr, get_type_pointer(expr.value._type.argTypes[expr.fieldIndex]), "{describe(expr.value._type,false,false,false)}.{argName}")
            if expr.fieldFlags.r2v
                setE(expr, LLVMBuildLoad(g_builder, field_ptr, ""))
            else
                setE(expr, field_ptr)
        else
            assert(expr.field!=null)
            var val_ptr = getE(expr.value)
            if expr.value._type.isPointer && !expr.derefFlags.unsafeDeref
                check_ptr_zero(val_ptr)
            val_ptr = LLVMBuildPointerCast(g_builder, val_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            var field_ptr = LLVMBuildGEP(g_builder, val_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.field.offset), 0), "")
            field_ptr = LLVMBuildPointerCast(g_builder, field_ptr, get_type_pointer(expr.field._type), "{describe(expr.value._type,false,false,false)}.{expr.field.name}")
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
        position_at_end(lblk.loop_end)

        for ssrc in expr.sources
            if ssrc._type.isGoodArrayType
                build_array_unlock(getE(ssrc))

        if expr.body is ExprBlock
            let body = expr.body as ExprBlock
            if length(body.finalList) != 0
                visit_finally(body, adapter)
        return expr

    def override preVisitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool) : void
        // add variable to be looked up
        var vtype = type_to_llvm_type(svar._type)
        var vptrtype = get_type_pointer(svar._type)
        if svar._type.isRef
            vtype = vptrtype
            vptrtype = LLVMPointerType(vptrtype,0u)
        var v_ptr : LLVMOpaqueValue?
        at_function_entry <|
            v_ptr = LLVMBuildAlloca(g_builder, vtype, "for_loop_variable_{svar.name}")
            LLVMSetAlignment(v_ptr, 16u)
            v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, vptrtype, "for_loop_variable_{svar.name}")
            setV(svar, v_ptr)
        // initialize its memory with 0 (should we?)
        var vsize = svar._type.isRef ?  typeinfo(sizeof type<void?>) : svar._type.sizeOf
        LLVMBuildMemSet(g_builder, v_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(vsize), 0), 16u)

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
                position_at_end(okay)
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
            elif ssrc._type.isGoodArrayType
                var element_type = type_to_llvm_type(svar._type)
                var arr = LLVMBuildLoad(g_builder, getE(ssrc), "array")
                var size = LLVMBuildExtractValue(g_builder, arr, uint(JIT_ARRAY SIZE), "array.size")
                var cmp_from_to = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntNE, size, LLVMConstInt(LLVMInt32Type(),0ul,0), "")
                var okay = LLVMAppendBasicBlock(ffunc, "for_{svar.name}_not_empty")
                LLVMBuildCondBr(g_builder, cmp_from_to, okay, lblk.loop_end)
                position_at_end(okay)
                build_array_lock(getE(ssrc))
                var data = LLVMBuildExtractValue(g_builder, arr, uint(JIT_ARRAY DATA), "array.data")
                LLVMBuildStore(g_builder, data, getV(svar))
                var sto = LLVMBuildGEP2(g_builder, element_type, data, size, "")
                sto = LLVMBuildPtrToInt(g_builder, sto, LLVMIntPtrType(), "array_end")
                range2[get_ptr(ssrc)] = sto
            else
                failed_E(ssrc, "unsupported loop source {describe(ssrc._type)}")

        var for_iterate = LLVMAppendBasicBlock(ffunc, "for_body_loop")
        LLVMBuildBr(g_builder, for_iterate)
        position_at_end(lblk.loop_start)

        for svar,ssrc in expr.iteratorVariables,expr.sources
            if ssrc._type.isRange           // range()
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                svar_v = LLVMBuildAdd(g_builder, svar_v, LLVMConstInt(LLVMInt32Type(), 1ul, 0), "")
                LLVMBuildStore(g_builder, svar_v, getV(svar))
            elif ssrc._type.dim.length != 0 // []
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                var svar_i = LLVMBuildGEP2(g_builder, type_to_llvm_type(svar._type), svar_v, LLVMConstInt(LLVMInt32Type(),1ul,0), "")
                LLVMBuildStore(g_builder, svar_i, getV(svar))
            elif ssrc._type.isGoodArrayType
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                var svar_i = LLVMBuildGEP2(g_builder, type_to_llvm_type(svar._type), svar_v, LLVMConstInt(LLVMInt32Type(),1ul,0), "")
                LLVMBuildStore(g_builder, svar_i, getV(svar))
            else
                failed_E(ssrc, "unsupported loop source {describe(ssrc._type)}")

        LLVMBuildBr(g_builder, for_iterate)
        position_at_end(for_iterate)

        for svar,ssrc in expr.iteratorVariables,expr.sources
            if ssrc._type.isRange                       // range
                var sto = range2[get_ptr(ssrc)]
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                var rcond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, svar_v, sto, "")
                var okay = LLVMAppendBasicBlock(ffunc, "for_{svar.name}_okay")
                LLVMBuildCondBr(g_builder, rcond, lblk.loop_end, okay)
                position_at_end(okay)
            elif ssrc._type.dim.length != 0             // []
                var sto = range2[get_ptr(ssrc)]
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                var svar_i = LLVMBuildPtrToInt(g_builder, svar_v, LLVMIntPtrType(), "")
                var rcond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, svar_i, sto, "")
                var okay = LLVMAppendBasicBlock(ffunc, "for_{svar.name}_okay")
                LLVMBuildCondBr(g_builder, rcond, lblk.loop_end, okay)
                position_at_end(okay)
            elif ssrc._type.isGoodArrayType
                var sto = range2[get_ptr(ssrc)]
                var svar_v = LLVMBuildLoad(g_builder, getV(svar), "")
                var svar_i = LLVMBuildPtrToInt(g_builder, svar_v, LLVMIntPtrType(), "")
                var rcond = LLVMBuildICmp(g_builder, LLVMIntPredicate LLVMIntEQ, svar_i, sto, "")
                var okay = LLVMAppendBasicBlock(ffunc, "for_{svar.name}_okay")
                LLVMBuildCondBr(g_builder, rcond, lblk.loop_end, okay)
                position_at_end(okay)
            else
                failed_E(ssrc, "unsupported loop source {describe(ssrc._type)}")

        LLVMBuildBr(g_builder, lblk.loop_body)
        position_at_end(lblk.loop_body)

// while loop
    def override preVisitExprWhile(expr:smart_ptr<ExprWhile>) : void
        var lblk = [[LoopBlock
            loop_start = LLVMAppendBasicBlock(ffunc, "while_start"),
            loop_body = LLVMAppendBasicBlock(ffunc, "while_body"),
            loop_end = LLVMAppendBasicBlock(ffunc, "while_end")
        ]]
        loop_stack |> push(lblk)
        LLVMBuildBr(g_builder, lblk.loop_start)
        position_at_end(lblk.loop_start)
    def override preVisitExprWhileBody(expr:smart_ptr<ExprWhile>;right:ExpressionPtr) : void
        var lblk = loop_stack |> back()
        LLVMBuildCondBr(g_builder, getE(expr.cond), lblk.loop_body, lblk.loop_end)
        position_at_end(lblk.loop_body)
    def override visitExprWhile(expr:smart_ptr<ExprWhile>) : ExpressionPtr
        var lblk = loop_stack |> back()
        loop_stack |> pop()
        LLVMBuildBr(g_builder, lblk.loop_start)
        position_at_end(lblk.loop_end)
        if expr.body is ExprBlock
            let body = expr.body as ExprBlock
            if length(body.finalList) != 0
                visit_finally(body, adapter)
        return expr

// make array
    def override preVisitExprMakeArray(expr:smart_ptr<ExprMakeArray>) : void
        var mks_ptr : LLVMOpaqueValue?
        if expr.makeFlags.useCMRES
            mks_ptr = get_cmres_param()
            mks_ptr = LLVMBuildPointerCast(g_builder, mks_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "")
        elif expr.makeFlags.useStackRef
            mks_ptr = getE(expr)
            verify(mks_ptr!=null && LLVMIsUndef(mks_ptr)==0,"USE STACKREF, but no stackref")
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "")
        else
            mks_ptr = tryE(expr) <|
                var vtype = type_to_llvm_type(expr._type)
                var s_ptr : LLVMOpaqueValue?
                at_function_entry <|
                    s_ptr = LLVMBuildAlloca(g_builder, vtype, "mka_local_sp_{expr.stackTop}")
                    LLVMSetAlignment(s_ptr, 16u)
                    s_ptr = LLVMBuildPointerCast(g_builder, s_ptr, LLVMPointerType(LLVMInt8Type(),0u), "mka_local_sp_{expr.stackTop}")
                return s_ptr
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "mka_local_sp_{expr.stackTop}_eo_{expr.extraOffset}")
        let stride = expr.makeType.stride
        if !expr.makeFlags.doesNotNeedInit && !expr.makeFlags.initAllFields && stride!=0
            let total = int(expr.values.length)
            let bytes = max(total,1) * stride
            let align = uint(expr.makeType.alignOf)
            LLVMBuildMemSet(g_builder, mks_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(bytes), 0), align)
        setE(expr, mks_ptr)
    def override preVisitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool) : void
        let stride = expr.makeType.stride
        let offset =  index*stride
        var v_ptr = getE(expr)
        verify(v_ptr!=null && LLVMIsUndef(v_ptr)==0,"make array index, but no pointer")
        let record_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset), 0), "")
        if init |> isMakeLocal
            var val = unsafe(reinterpret<ExprMakeLocal?> get_ptr(init))
            val.extraOffset = uint(offset) + expr.extraOffset
            if expr.makeFlags.useCMRES
                pass
            elif expr.makeFlags.useStackRef
                setE(init, v_ptr)
            else
                setE(init, v_ptr)
        elif make_call_to_cmres ( init, record_ptr )
            pass
    def override visitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
        let stride = expr.makeType.stride
        let offset =  index*stride
        var v_ptr = getE(expr)
        v_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset), 0), "[{index}]")
        v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, get_type_pointer(expr.recordType), "")
        if init |> isMakeLocal
            pass // do nothign. we already taken care of makeLocal in preVisitExprMakeStructField
        elif isCall2CMRES(init)
            pass // do nothing. we already handle this in preVisitExprMakeStructField
        else
            if expr.recordType.canCopy
                build_copy(v_ptr, init)
            else
                failed_E(expr, "makeArray: move is not supported")
        return init

// make tuple
    def override preVisitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>) : void
        var mks_ptr : LLVMOpaqueValue?
        if expr.makeFlags.useCMRES
            mks_ptr = get_cmres_param()
            mks_ptr = LLVMBuildPointerCast(g_builder, mks_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "")
        elif expr.makeFlags.useStackRef
            mks_ptr = getE(expr)
            verify(mks_ptr!=null && LLVMIsUndef(mks_ptr)==0,"USE STACKREF, but no stackref")
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "")
        else
            mks_ptr = tryE(expr) <|
                var vtype = type_to_llvm_type(expr._type)
                var s_ptr : LLVMOpaqueValue?
                at_function_entry <|
                    s_ptr = LLVMBuildAlloca(g_builder, vtype, "mkt_local_sp_{expr.stackTop}")
                    LLVMSetAlignment(s_ptr, 16u)
                    s_ptr = LLVMBuildPointerCast(g_builder, s_ptr, LLVMPointerType(LLVMInt8Type(),0u), "mkt_local_sp_{expr.stackTop}")
                return s_ptr
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "mkt_local_sp_{expr.stackTop}_eo_{expr.extraOffset}")
        let stride = expr.makeType.stride
        if !expr.makeFlags.doesNotNeedInit && !expr.makeFlags.initAllFields && stride!=0
            let total = int(expr.values.length)
            let bytes = max(total,1) * stride
            let align = uint(expr.makeType.alignOf)
            LLVMBuildMemSet(g_builder, mks_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(bytes), 0), align)
        setE(expr, mks_ptr)
    def override preVisitExprMakeTupleIndex(expr:smart_ptr<ExprMakeTuple>;index:int;init:ExpressionPtr;last:bool) : void
        let offset = get_tuple_field_offset(expr.makeType,index)
        var v_ptr = getE(expr)
        verify(v_ptr!=null && LLVMIsUndef(v_ptr)==0,"make struct field, but no pointer")
        let field_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset), 0), "")
        if init |> isMakeLocal
            var val = unsafe(reinterpret<ExprMakeLocal?> get_ptr(init))
            val.extraOffset = uint(offset) + expr.extraOffset
            if expr.makeFlags.useCMRES
                pass
            elif expr.makeFlags.useStackRef
                setE(init, v_ptr)
            else
                setE(init, v_ptr)
        elif make_call_to_cmres ( init, field_ptr )
            pass
    def tuple_field_name ( t:TypeDeclPtr; index:int )
        if !t.isTuple
            failed_T(t, "not a tuple {describe(t)}")
            return "!!not_a_tuple!!"
        return t.argNames.length!=0 ? "{t.argNames[index]}" : "{describe(t.argTypes[index])}"
    def variant_field_name ( t:TypeDeclPtr; index:int )
        if !t.isVariant
            failed_T(t, "not a variant {describe(t)}")
            return "!!not_a_variant!!"
        return t.argNames.length!=0 ? "{t.argNames[index]}" : "{describe(t.argTypes[index])}"
    def override visitExprMakeTupleIndex(expr:smart_ptr<ExprMakeTuple>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
        let offset = get_tuple_field_offset(expr.makeType,index)
        var v_ptr = getE(expr)
        let argName = "_{tuple_field_name(expr.makeType,index)}"
        v_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset), 0), "tuple_{index}{argName}")
        v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, get_type_pointer(expr.makeType.argTypes[index]), "")
        if init |> isMakeLocal
            pass // do nothign. we already taken care of makeLocal in preVisitExprMakeStructField
        elif isCall2CMRES(init)
            pass // do nothing. we already handle this in preVisitExprMakeStructField
        else
            if expr.makeType.canCopy
                build_copy(v_ptr, init)
            else
                failed_E(expr, "makeTuple: move is not supported")
        return init

// make variant
    def override preVisitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>) : void
        var mkv_ptr : LLVMOpaqueValue?
        if expr.makeFlags.useCMRES
            mkv_ptr = get_cmres_param()
            mkv_ptr = LLVMBuildPointerCast(g_builder, mkv_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            if expr.extraOffset != 0u
                mkv_ptr = LLVMBuildGEP(g_builder, mkv_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "")
        elif expr.makeFlags.useStackRef
            mkv_ptr = getE(expr)
            verify(mkv_ptr!=null && LLVMIsUndef(mkv_ptr)==0,"USE STACKREF, but no stackref")
            if expr.extraOffset != 0u
                mkv_ptr = LLVMBuildGEP(g_builder, mkv_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "")
        else
            mkv_ptr = tryE(expr) <|
                var vtype = type_to_llvm_type(expr._type)
                var s_ptr : LLVMOpaqueValue?
                at_function_entry <|
                    s_ptr = LLVMBuildAlloca(g_builder, vtype, "mkv_local_sp_{expr.stackTop}")
                    LLVMSetAlignment(s_ptr, 16u)
                    s_ptr = LLVMBuildPointerCast(g_builder, s_ptr, LLVMPointerType(LLVMInt8Type(),0u), "mkv_local_sp_{expr.stackTop}")
                return s_ptr
            if expr.extraOffset != 0u
                mkv_ptr = LLVMBuildGEP(g_builder, mkv_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "mkv_local_sp_{expr.stackTop}_eo_{expr.extraOffset}")
        let stride = expr.makeType.stride
        if stride!=0 && length(expr.variants)==0
            let total = int(expr.variants.length)
            let bytes = max(total,1) * stride
            let align = uint(expr.makeType.alignOf)
            LLVMBuildMemSet(g_builder, mkv_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(bytes), 0), align)
        setE(expr, mkv_ptr)
    def override preVisitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool) : void
        var field_index = find_argument_index(expr.makeType, string(decl.name))
        if field_index==-1
            failed_E(expr, "variant field {decl.name} not found")
            return
        let field_offset = get_variant_field_offset(expr.makeType,field_index)
        let stride = expr.makeType.stride
        let offset =  index*stride
        var v_ptr = getE(expr)
        verify(v_ptr!=null && LLVMIsUndef(v_ptr)==0,"make variant field, but no pointer")
        let index_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset), 0), "")
        LLVMBuildStore2(g_builder, LLVMInt32Type(), LLVMConstInt(LLVMInt32Type(), uint64(field_index), 0), index_ptr)
        let field_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset + field_offset), 0), "")
        if decl.value |> isMakeLocal
            var val = unsafe(reinterpret<ExprMakeLocal?> get_ptr(decl.value))
            val.extraOffset = uint(offset + field_offset) + expr.extraOffset
            if expr.makeFlags.useCMRES
                pass
            elif expr.makeFlags.useStackRef
                setE(decl.value, v_ptr)
            else
                setE(decl.value, v_ptr)
        elif make_call_to_cmres ( decl.value, field_ptr )
            pass
    def override visitExprMakeVariantField(expr:smart_ptr<ExprMakeVariant>;index:int;decl:MakeFieldDeclPtr;last:bool) : MakeFieldDeclPtr
        var field_index = find_argument_index(expr.makeType, string(decl.name))
        if field_index==-1
            failed_E(expr, "variant field {decl.name} not found")
            return decl
        let field_offset = get_variant_field_offset(expr.makeType,field_index)
        let stride = expr.makeType.stride
        let offset =  index*stride
        var v_ptr = getE(expr)
        v_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset+field_offset), 0), "[{index}].{decl.name}")
        v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, get_type_pointer(expr.makeType.argTypes[field_index]), "")
        if decl.value |> isMakeLocal
            pass // do nothign. we already taken care of makeLocal in preVisitExprMakeStructField
        elif isCall2CMRES(decl.value)
            pass // do nothing. we already handle this in preVisitExprMakeStructField
        else
            if decl.flags.moveSemantics
                failed_E(expr, "makeStruct: move is not supported")
            else
                build_copy(v_ptr, decl.value)
        return decl

// make structure
    def override preVisitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>) : void
        var mks_ptr : LLVMOpaqueValue?
        if expr.makeFlags.useCMRES
            mks_ptr = get_cmres_param()
            mks_ptr = LLVMBuildPointerCast(g_builder, mks_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "")
        elif expr.makeFlags.useStackRef
            mks_ptr = getE(expr)
            verify(mks_ptr!=null && LLVMIsUndef(mks_ptr)==0,"USE STACKREF, but no stackref")
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "")
        else
            mks_ptr = tryE(expr) <|
                var vtype = type_to_llvm_type(expr._type)
                var s_ptr : LLVMOpaqueValue?
                at_function_entry <|
                    s_ptr = LLVMBuildAlloca(g_builder, vtype, "mks_local_sp_{expr.stackTop}")
                    LLVMSetAlignment(s_ptr, 16u)
                    s_ptr = LLVMBuildPointerCast(g_builder, s_ptr, LLVMPointerType(LLVMInt8Type(),0u), "mks_local_sp_{expr.stackTop}")
                return s_ptr
            if expr.extraOffset != 0u
                mks_ptr = LLVMBuildGEP(g_builder, mks_ptr, LLVMConstInt(LLVMInt32Type(), uint64(expr.extraOffset), 0), "mks_local_sp_{expr.stackTop}_eo_{expr.extraOffset}")
        let stride = expr.makeType.stride
        if !expr.makeFlags.doesNotNeedInit && !expr.makeFlags.initAllFields && stride!=0
            let total = int(expr.structs.length)
            let bytes = max(total,1) * stride
            let align = uint(expr.makeType.alignOf)
            LLVMBuildMemSet(g_builder, mks_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(bytes), 0), align)
        setE(expr, mks_ptr)
    def override preVisitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool) : void
        var field = find_structure_field(expr.makeType.structType, string(decl.name))
        if field==null
            failed_E(expr, "field {decl.name} not found")
            return
        let stride = expr.makeType.stride
        let offset =  index*stride + field.offset
        var v_ptr = getE(expr)
        verify(v_ptr!=null && LLVMIsUndef(v_ptr)==0,"make struct field, but no pointer")
        let field_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset), 0), "")
        if decl.value |> isMakeLocal
            var val = unsafe(reinterpret<ExprMakeLocal?> get_ptr(decl.value))
            val.extraOffset = uint(offset) + expr.extraOffset
            if expr.makeFlags.useCMRES
                pass
            elif expr.makeFlags.useStackRef
                setE(decl.value, v_ptr)
            else
                setE(decl.value, v_ptr)
        elif make_call_to_cmres ( decl.value, field_ptr )
            pass
    def override visitExprMakeStructField(expr:smart_ptr<ExprMakeStruct>;index:int;decl:MakeFieldDeclPtr;last:bool) : MakeFieldDeclPtr
        var field = find_structure_field(expr.makeType.structType, string(decl.name))
        if field==null
            failed_E(expr, "field {decl.name} not found")
            return decl
        let stride = expr.makeType.stride
        let offset =  index*stride + field.offset
        var v_ptr = getE(expr)
        v_ptr = LLVMBuildGEP(g_builder, v_ptr, LLVMConstInt(LLVMInt32Type(), uint64(offset), 0), "[{index}].{decl.name}")
        v_ptr = LLVMBuildPointerCast(g_builder, v_ptr, get_type_pointer(field._type), "")
        if decl.value |> isMakeLocal
            pass // do nothign. we already taken care of makeLocal in preVisitExprMakeStructField
        elif isCall2CMRES(decl.value)
            pass // do nothing. we already handle this in preVisitExprMakeStructField
        else
            if decl.flags.moveSemantics
                failed_E(expr, "makeStruct: move is not supported")
            else
                build_copy(v_ptr, decl.value)
        return decl

// new
    def build_alloc ( bytes:int; persistent:bool )
        var params = [[auto
            LLVMConstInt(LLVMInt32Type(), uint64(bytes), 0);    // bytes
            get_context_param()
        ]]
        return LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, persistent ? FN_JIT_ALLOC_PERSISTENT : FN_JIT_ALLOC_HEAP), params, "")
    def override preVisitExprNew(expr:smart_ptr<ExprNew>): void
        var new_ptr : LLVMOpaqueValue?
        if expr.typeexpr.baseType==Type tHandle
            failed_E(expr, "new handle is not supported yet")
            return
        else
            var persistent = false
            if expr.typeexpr.baseType == Type tStructure
                persistent = expr.typeexpr.structType.flags.persistent
            let bytes = expr._type.firstType.sizeOf
            new_ptr = build_alloc(bytes, persistent)
            check_ptr_zero(new_ptr, "new returned null")
            if expr.initializer
                call2cmres[get_ptr(ExpressionPtr(expr))] = new_ptr
                make_call(expr, false)
            else
                LLVMBuildMemSet(g_builder, new_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(bytes), 0), 16u)
        new_ptr = LLVMBuildPointerCast(g_builder, new_ptr, type_to_llvm_type(expr._type), "new")
        if expr._type.dim.length != 0
            failed_E(expr, "new [] is not supported yet")
        setE(expr, new_ptr)

// ascend
    def override preVisitExprAscend(expr:smart_ptr<ExprAscend>) : void
        if expr.subexpr._type.baseType == Type tHandle
            failed_E(expr, "ascend of handle is not supported")
            return
        let bytes = expr.subexpr._type.sizeOf
        var mks_ptr = build_alloc(bytes, false)
        check_ptr_zero(mks_ptr, "new [[...]] returned null")
        LLVMBuildMemSet(g_builder, mks_ptr, LLVMConstInt(LLVMInt8Type(), 0ul, 0), LLVMConstInt(LLVMInt32Type(), uint64(bytes), 0), 16u)
        if expr.ascendFlags.useStackRef
            let stackref_ptr = LLVMBuildPointerCast(g_builder, mks_ptr, LLVMPointerType(LLVMInt8Type(), 0u), "")
            setE(expr.subexpr, stackref_ptr)
        mks_ptr = LLVMBuildPointerCast(g_builder, mks_ptr, type_to_llvm_type(expr._type), "")
        setE(expr,mks_ptr)

// cast
    def override visitExprCast(expr:smart_ptr<ExprCast>) : ExpressionPtr
        var res : LLVMOpaqueValue?
        if expr.castType.isPointer && expr.subexpr._type.isPointer
            res = LLVMBuildPointerCast(g_builder, getE(expr.subexpr), type_to_llvm_type(expr.castType), "")
        else
            res = LLVMGetUndef(type_to_llvm_type(expr.castType))
            failed_E(expr, "cast<{describe(expr.castType)} is not supported yet")
        setE(expr, res)
        return expr

// make block
    def make_block_funcion(expr:smart_ptr<ExprMakeBlock>)
        var astVisitor = new LlvmJitVisitor(jit_context)
        unsafe
            astVisitor.adapter <- make_visitor(*astVisitor)
        visit(expr._block, astVisitor.adapter)
        astVisitor.adapter := null
        unsafe
            delete astVisitor

    def override canVisitMakeBlockBody(expr:smart_ptr<ExprMakeBlock>) : bool
        return false
    def override preVisitExprMakeBlock(expr:smart_ptr<ExprMakeBlock>) : void
        var capture <- capture_block(expr._block)
        // build block type
        var blk_fields <- build_block_type()
        blk_fields |> push(LLVMArrayType(LLVMFloat4Type(), 3u))
        var first_field_index = length(blk_fields)
        for c in capture
            blk_fields |> push(LLVMPointerType(type_to_llvm_type(c.variable._type), 0u))
        var this_block_type = LLVMStructType(blk_fields)
        // allocate block, set capture fields
        var blk = LLVMBuildAlloca(g_builder, this_block_type, "block_at_{int(expr.at.line)}")
        for c,idx in capture,range(100500)
            var v_ptr = getV_ptr(c.variable)
            if v_ptr == null
                v_ptr = getE_ptr(c.expression)
            if v_ptr == null
                failed_E(expr, "capture {c.variable.name} not found")
                return
            blk = LLVMBuildInsertValue(g_builder, blk, v_ptr, uint(idx + first_field_index), "capture_{c.variable.name}")
        // call jit_make_block
        var blk_ptr = LLVMBuildPointerCast(g_builder, blk, LLVMPointerType(g_t_block, 0u), "")
        let argStackTop = (expr._block as ExprBlock).stackTop
        let null_ptr = LLVMConstPointerNull(LLVMPointerType(LLVMVoidType(),0u))
        var params = [[auto
            blk_ptr;
            LLVMConstInt(LLVMInt32Type(), uint64(argStackTop), 0);
            null_ptr;   // TODO: function pointer
            null_ptr;   // TODO: function inf
            get_context_param()
        ]]
        LLVMBuildCall(g_builder, LLVMGetNamedFunction(g_mod, FN_JIT_MAKE_BLOCK), params, "")
        setE(expr, blk_ptr)

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
    if length(g_errors) > 0
        let errors = g_errors |> join("\n")
        delete g_errors
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
    LLVMPassManagerBuilderSetSizeLevel(pmb, LLVM_SIZE_LEVEL)
    LLVMPassManagerBuilderUseInlinerWithThreshold(pmb, LLVM_INLINE_THRESHOLD)
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
        if !LLVM_JIT_ENABLED
            return true
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

