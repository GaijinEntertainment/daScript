options gen2
options no_global_variables = false
require dastest/testing_boost public
require llvm/daslib/llvm_boost
require llvm/daslib/llvm_jit_common
require llvm/daslib/ir_dsl

def private run_i32(fn : LLVMOpaqueValue?; n : int) : int {
    var engine : LLVMExecutionEngineRef
    var err : string
    unsafe {
        LLVMCreateInterpreterForModule(addr(engine), g_mod, addr(err))
    }
    var args = fixed_array(LLVMCreateGenericValueOfInt(g_prim_t.t_int32, uint64(n), 1))
    var res : LLVMOpaqueGenericValue? = LLVMRunFunction(engine, fn, 1u, unsafe(addr(args[0])))
    let r = int(LLVMGenericValueToInt(res, 1))
    LLVMDisposeGenericValue(args[0])
    LLVMDisposeGenericValue(res)
    var outMod : LLVMOpaqueModule?
    unsafe {
        LLVMRemoveModule(engine, g_mod, addr(outMod), addr(err))
    }
    LLVMDisposeExecutionEngine(engine)
    return r
}

def private with_module(blk : block<void>) {
    LLVMLinkInInterpreter()
    g_ctx = LLVMContextCreate()
    g_prim_t = new PrimitiveTypes(g_ctx)
    g_mod = LLVMModuleCreateWithNameInContext("ir_dsl_loop", g_ctx)
    invoke(blk)
    LLVMDisposeModule(g_mod)
    LLVMContextDispose(g_ctx)
    g_mod = null
    g_ctx = null
}

def private sum_type : LLVMTypeRef {
    var params = fixed_array(g_prim_t.t_int32)
    return LLVMFunctionType(g_prim_t.t_int32, params, false)
}

[test(description = "ir_loop runs its body count times with i from 0, and not at all for a zero count")]
def test_ir_loop_sums_indices(t : T?) {
    with_module() {
        let fn = ir_function("sum_below", sum_type(), -1, -1) {
            let acc = ir(LLVMBuildAlloca(ir_builder(), g_prim_t.t_int32, "acc"))
            store(acc, ir(0u))
            ir_loop(arg(0)) $(i) {
                store(acc, load32(acc) + i)
            }
            ret(load32(acc))
        }
        t |> equal(run_i32(fn, 5), 10)
        t |> equal(run_i32(fn, 1), 0)
        t |> equal(run_i32(fn, 0), 0)
    }
}

[test(description = "ir_at builds at an existing builder in its function, and an ir_function built inside it leaves the outer state intact")]
def test_ir_at_builds_into_an_existing_function(t : T?) {
    with_module() {
        let fn = LLVMAddFunction(g_mod, "count_twice", sum_type())
        let b = LLVMCreateBuilderInContext(g_ctx)
        LLVMPositionBuilderAtEnd(b, LLVMAppendBasicBlockInContext(g_ctx, fn, "entry"))
        let acc = LLVMBuildAlloca(b, g_prim_t.t_int32, "acc")
        LLVMBuildStore(b, LLVMConstInt(g_prim_t.t_int32, 0ul, 0), acc)
        ir_at(b, null, null) {
            ir_function("unused_helper", sum_type(), -1, -1) {
                ret(arg(0))
            }
            ir_loop(arg(0)) $(_i) {
                store(ir(acc), load32(ir(acc)) + ir(2u))
            }
        }
        LLVMBuildRet(b, LLVMBuildLoad2(b, g_prim_t.t_int32, acc, ""))
        LLVMDisposeBuilder(b)
        t |> equal(run_i32(fn, 4), 8)
        t |> equal(run_i32(fn, 0), 0)
    }
}

[test(description = "ir_function into a given module, call, multiply, not, return_if with a value and if_value compute what they say")]
def test_ir_dsl_helpers(t : T?) {
    with_module() {
        let square = ir_function(g_mod, "square", sum_type(), -1, -1) {
            ret(arg(0) * arg(0))
        }
        let fn = ir_function("pick", sum_type(), -1, -1) {
            return_if(arg(0) == ir(0u), ir(100u))
            var args <- [arg(0)]
            let v = if_value(below(arg(0), ir(3u)), $ {
                return call(square, args)
            }, $ {
                return !arg(0)
            })
            ret(v)
        }
        t |> equal(run_i32(fn, 0), 100)
        t |> equal(run_i32(fn, 2), 4)
        t |> equal(run_i32(fn, 5), -6)
    }
}
