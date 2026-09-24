options gen2

options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options relaxed_pointer_const

module llvm_jit_runtime shared private

require llvm/daslib/llvm_boost
require llvm/daslib/llvm_dsl
require llvm/daslib/llvm_jit_common
require llvm/daslib/ir_dsl
require jit

//! The runtime entry points jitted code calls that are plain IR, the twins of src/builtin/jit_runtime.cpp:
//! each keeps its old C++ name and signature and is an internal function of every module that calls it.

//! the IR runtime helpers; each value's name is the helper's symbol
enum public JitRuntime {
    jit_str_cmp
    jit_array_lock
    jit_array_unlock
    jit_table_lock
    jit_table_unlock
}

//! calls runtime helper `fn`, defining its body into g_mod on the module's first call
def public build_jit_runtime_call(b : LLVMBuilderRef; fn : JitRuntime; var args : LLVMOpaqueValue? []) : LLVMOpaqueValue? {
    var f = LLVMGetNamedFunction(g_mod, "{fn}")
    if (f == null) {
        f = define_jit_runtime_helper(fn)
    }
    return LLVMBuildCall2(b, LLVMGlobalGetValueType(f), f, args, "")
}

def private define_jit_runtime_helper(fn : JitRuntime) : LLVMOpaqueValue? {
    let name = "{fn}"
    if (fn == JitRuntime.jit_str_cmp) return define_str_cmp(name)
    if (fn == JitRuntime.jit_array_lock) return define_lock(name, "array")
    if (fn == JitRuntime.jit_table_lock) return define_lock(name, "table")
    if (fn == JitRuntime.jit_array_unlock) return define_unlock(name, "array")
    return define_unlock(name, "table")
}

//! strcmp where a null string reads as ""
def private define_str_cmp(name : string) : LLVMOpaqueValue? {
    let strcmp_type = jit_fn_type($(sA, sB : void?) : int {})
    return ir_function(name, strcmp_type, -1, -1) {
        let a = select(is_null(arg(0)), text(""), arg(0))
        let b = select(is_null(arg(1)), text(""), arg(1))
        ret(ir(LLVMBuildCall2(ir_builder(), strcmp_type, declare_extern_fn("strcmp", strcmp_type), fixed_array(a.v, b.v), "")))
    }
}

//! a field of the lock header Array and Table share, on the helper's first argument
def private header(f : JIT_ARRAY) : Ir {
    return field(arg(0), jit_struct_type(type<tuple<data : void?; size : int64; capacity : int64; magic : int; lock : int; flags : int>>), int(f))
}

//! array_lock / table_lock (runtime_array.cpp, runtime_table.cpp)
def private define_lock(name, what : string) : LLVMOpaqueValue? {
    return ir_function(name, jit_fn_type($(arr, ctx, at : void?) : void {}), 1, 2) {
        return_if((load32(header(JIT_ARRAY.FLAGS)) & ir(ARRAY_LOCK_EXEMPT_FLAGS)) != ir(0u))
        let lock = load32(header(JIT_ARRAY.LOCK))
        let first = lock == ir(0u)
        throw_if(load32(header(JIT_ARRAY.MAGIC)) != select(first, ir(0u), ir(ARRAY_MAGIC)), select(first,
            text("{what} magic mismatch on first lock, was it moved or overwritten?"),
            text("{what} magic mismatch on lock, was it moved or overwritten?")))
        store(header(JIT_ARRAY.LOCK), lock + ir(1u))
        store(header(JIT_ARRAY.MAGIC), ir(ARRAY_MAGIC))
    }
}

//! array_unlock / table_unlock: the last unlock clears the magic
def private define_unlock(name, what : string) : LLVMOpaqueValue? {
    return ir_function(name, jit_fn_type($(arr, ctx, at : void?) : void {}), 1, 2) {
        return_if((load32(header(JIT_ARRAY.FLAGS)) & ir(ARRAY_LOCK_EXEMPT_FLAGS)) != ir(0u))
        throw_if(load32(header(JIT_ARRAY.MAGIC)) != ir(ARRAY_MAGIC), text("{what} magic mismatch on unlock, was it moved or overwritten?"))
        let lock = load32(header(JIT_ARRAY.LOCK))
        throw_if(lock == ir(0u), text("{what} lock underflow, was it moved or overwritten?"))
        let left = lock - ir(1u)
        store(header(JIT_ARRAY.LOCK), left)
        store(header(JIT_ARRAY.MAGIC), select(left == ir(0u), ir(0u), ir(ARRAY_MAGIC)))
    }
}
