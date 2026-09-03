options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = false
options relaxed_pointer_const
options unsafe_table_lookup = false
options no_global_variables = false
options stack = 4_194_304

module llvm_lib shared private

require llvm/daslib/llvm_boost
require llvm/daslib/llvm_jit
require llvm/daslib/llvm_jit_common
require llvm/daslib/llvm_dll_utils
require llvm/daslib/llvm_exe
require daslib/ast_boost
require daslib/c_api_header
require daslib/defer
require daslib/rtti

//! The `daslang -lib` entry emitter: turns the jitted program into a C-ABI library.
//!
//! One `extern "C"` thunk per `CExport` (`daslib/c_api_header` decides the set and the C spelling)
//! plus the four fixed entry points. Codegen stays in `LlvmJitMode.EXE` - a library wants exactly
//! the exe's symbol picture, private wrappers and load-resolved globals - so only the entry shape
//! and the artifact differ, and both halves of the exe startup are shared with `inject_main`
//! through the `llvm_exe` helpers.
//!
//! A das panic must never unwind through the C boundary: every thunk hands its body to
//! `jit_lib_invoke_guarded`, which runs it under the context's catch boundary and answers with a
//! code. `<P>_last_error` then reports the text.

let private LIB_GUARD_GLOBAL = "__das_lib_init_guard"
let private LIB_RUNTIME_INIT = "__das_lib_runtime_init"
let private LIB_CTX_INIT = "__das_lib_ctx_init"

var public g_lib_exports : array<CExport>


def public lib_symbol_prefix(prog : Program?) : string {
    //! The library's C symbol prefix. The entry module's file name is the one source, so the header
    //! and the emitted symbols cannot disagree.
    let file = string(prog.getThisModule.fileName)
    let prefix = lib_prefix_from_path(file)
    return prefix |> empty() ? "daslib" : prefix
}


def public lib_link_note(static_lib : bool; link_whole_lib : bool) : string {
    //! What a host has to link against this artifact, for the header's own record. Names the files
    //! as this platform spells them, because that is the one thing a reader cannot guess.
    let windows = get_platform_name() == "windows"
    let rt_static = windows ? "libDaScript_runtime.lib" : "liblibDaScript_runtime.a"
    let cc_static = windows ? "libDaScript.lib" : "liblibDaScript.a"
    let rt_shared = windows ? "libDaScriptDyn_runtime.dll" : (get_platform_name() == "darwin" ? "liblibDaScriptDyn_runtime.dylib" : "liblibDaScriptDyn_runtime.so")
    if (static_lib) {
        let whole = link_whole_lib ? " Add <das_root>/lib/{cc_static} too - this library\nregisters a module that lives in the compiler library." : ""
        let sys = windows ? "" : " Also link the platform libraries the runtime needs:\n-lpthread -ldl -lm -lstdc++."
        return "Link this archive plus <das_root>/lib/{rt_static}.{whole}{sys}"
    }
    let whole = link_whole_lib ? " It also needs the compiler library beside it - this library\nregisters a module that lives there." : ""
    return "Link this shared library. At load it needs {rt_shared}, which it looks for beside itself,\nin ../lib, and in <das_root>/lib; otherwise put it on the loader path.{whole}"
}


def private declare_shim(name : string; ret : LLVMTypeRef; var args : array<LLVMTypeRef>) : LLVMOpaqueValue? {
    var fn = LLVMGetNamedFunction(g_mod, name)
    if (fn == null) {
        fn = LLVMAddFunctionWithType(g_mod, name, LLVMFunctionType(ret, args))
    }
    return fn
}


def private new_private_function(ctx : LLVMContextRef; name : string; typ : LLVMTypeRef) : tuple<fn : LLVMOpaqueValue?; builder : LLVMBuilderRef> {
    let fn = LLVMAddFunctionWithType(g_mod, name, typ)
    set_private_linkage(fn)
    let b = LLVMCreateBuilderInContext(ctx)
    LLVMPositionBuilderAtEnd(b, LLVMAppendBasicBlockInContext(ctx, fn, "entry"))
    return (fn = fn, builder = b)
}


def private new_public_function(ctx : LLVMContextRef; name : string; typ : LLVMTypeRef) : tuple<fn : LLVMOpaqueValue?; builder : LLVMBuilderRef> {
    let fn = LLVMAddFunctionWithType(g_mod, name, typ)
    set_public_linkage(fn)
    let b = LLVMCreateBuilderInContext(ctx)
    LLVMPositionBuilderAtEnd(b, LLVMAppendBasicBlockInContext(ctx, fn, "entry"))
    return (fn = fn, builder = b)
}


struct private ThunkAbi {
    //! The impl-side shape of one export: what the C parameters turn into, and where the result
    //! lands. `slot_type` is the frame slot the trampoline writes a non-cmres result into.
    arg_types : array<LLVMTypeRef>
    arg_loads : array<bool>
    slot_type : LLVMOpaqueType?
    has_slot : bool
}


def private thunk_abi(var e : CExport; var types : PrimitiveTypes?) : ThunkAbi {
    var abi : ThunkAbi
    for (arg, p in e.fn.arguments, e.params) {
        abi.arg_types |> push(type_to_llvm_abi_type(arg._type))
        // A das ABI pointer takes the C pointer unchanged; a by-value type is loaded through it.
        abi.arg_loads |> push(p.by_pointer && !p.pointer_in_impl)
    }
    if (!e.fn.result.isVoid) {
        abi.has_slot = true
        // The frame carries a POINTER to the result: an in-struct [N x i8] slot would align to 1.
        abi.slot_type = (e.result.cmres ? types.LLVMVoidPtrType() : type_to_llvm_abi_type(e.fn.result))
    }
    return <- abi
}


def private frame_type(var abi : ThunkAbi; ctx : LLVMContextRef) : LLVMOpaqueType? {
    var fields <- [for (t in abi.arg_types); t]
    if (abi.has_slot) {
        fields |> push(abi.slot_type)
    }
    if (fields |> empty()) {
        fields |> push(g_prim_t.t_int32)   // LLVM has no empty struct to alloca
    }
    return LLVMStructTypeInContext(ctx, array_data_ptr(fields), uint(length(fields)), 0)
}


def private emit_trampoline(ctx : LLVMContextRef; var e : CExport; var uids : UidNodes?;
                            var types : PrimitiveTypes?; var abi : ThunkAbi; ft : LLVMOpaqueType?) : LLVMOpaqueValue? {
    //! void tramp(Context*, void* frame): unpacks the frame, calls the impl, stores the result back.
    //! It runs under `jit_lib_invoke_guarded`, so a panic inside it never reaches the C caller.
    let impl_name = uids.get_dll_fn_name_ptr(e.fn).impl()
    var impl = LLVMGetNamedFunction(g_mod, impl_name)
    if (impl == null) {
        return null
    }
    let tramp_type = LLVMFunctionType(types.t_void, array(types.LLVMVoidPtrType(), types.LLVMVoidPtrType()))
    let made = new_private_function(ctx, "__das_lib_tramp_{e.c_name}", tramp_type)
    let b = made.builder
    defer() {
        LLVMDisposeBuilder(b)
    }
    var ctx_arg = LLVMGetParam(made.fn, 0u)
    var frame = LLVMBuildPointerCast(b, LLVMGetParam(made.fn, 1u), LLVMPointerType(ft, 0u), "frame")
    var args : array<LLVMOpaqueValue?>
    args |> reserve(length(abi.arg_types) + 2)
    for (at, i in abi.arg_types, count()) {
        let slot = LLVMBuildStructGEP2(b, ft, frame, uint(i), "arg_{i}")
        args |> push(LLVMBuildLoad2(b, at, slot, "argv_{i}"))
    }
    args |> push(ctx_arg)
    let slot_index = uint(length(abi.arg_types))
    if (e.result.cmres) {
        var slot_ptr = LLVMBuildStructGEP2(b, ft, frame, slot_index, "res_ptr")
        args |> push(LLVMBuildLoad2(b, types.LLVMVoidPtrType(), slot_ptr, "res"))
    }
    var ret = LLVMBuildCall2(b, g_fn_types[impl_name], impl, args, "")
    if (abi.has_slot && !e.result.cmres) {
        LLVMBuildStore(b, ret, LLVMBuildStructGEP2(b, ft, frame, slot_index, "res"))
    }
    LLVMBuildRetVoid(b)
    return made.fn
}


def private c_param_types(var e : CExport; var types : PrimitiveTypes?) : array<LLVMTypeRef> {
    //! The thunk's own signature: the context, one parameter per C parameter, and the out pointer
    //! a by-pointer result travels through. das `bool` meets C as a byte.
    var out : array<LLVMTypeRef>
    out |> push(types.LLVMVoidPtrType())
    for (arg, p in e.fn.arguments, e.params) {
        if (p.by_pointer) {
            out |> push(types.LLVMVoidPtrType())
        } elif (arg._type.isBool) {
            out |> push(types.t_int8)
        } else {
            out |> push(type_to_llvm_abi_type(arg._type))
        }
    }
    if (e.result.via_out) {
        out |> push(types.LLVMVoidPtrType())
    }
    return <- out
}


def private c_return_type(var e : CExport; var types : PrimitiveTypes?) : LLVMTypeRef {
    if (e.result.via_out || e.fn.result.isVoid) {
        return types.t_void
    }
    return e.fn.result.isBool ? types.t_int8 : type_to_llvm_abi_type(e.fn.result)
}


def private store_thunk_args(b : LLVMBuilderRef; var e : CExport; var abi : ThunkAbi;
                             fn : LLVMOpaqueValue?; ft : LLVMOpaqueType?; var types : PrimitiveTypes?; frame : LLVMOpaqueValue?) {
    for (arg, p, i in e.fn.arguments, e.params, count()) {
        var incoming = LLVMGetParam(fn, uint(i + 1))
        var slot = LLVMBuildStructGEP2(b, ft, frame, uint(i), "in_{i}")
        if (abi.arg_loads[i]) {
            var typed = LLVMBuildPointerCast(b, incoming, LLVMPointerType(abi.arg_types[i], 0u), "")
            incoming = LLVMBuildLoadData2Aligned(b, abi.arg_types[i], typed, arg._type.alignOf, "val_{i}")
        } elif (p.by_pointer) {
            incoming = LLVMBuildPointerCast(b, incoming, abi.arg_types[i], "")
        } elif (arg._type.isBool) {
            // != 0, not the low bit: a non-C caller of the shipped library can pass any byte
            incoming = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntNE, incoming, types.ConstI8(int8(0)), "b_{i}")
        }
        LLVMBuildStore(b, incoming, slot)
    }
}


def private emit_thunk(ctx : LLVMContextRef; var e : CExport; var uids : UidNodes?; var types : PrimitiveTypes?) : bool {
    //! One `extern "C"` entry point. It packs its arguments into a frame, hands the frame and the
    //! trampoline to the guard, and answers zero (leaving `out` untouched) when the body raised.
    var abi <- thunk_abi(e, types)
    let ft = frame_type(abi, ctx)
    let tramp = emit_trampoline(ctx, e, uids, types, abi, ft)
    if (tramp == null) {
        to_log(LOG_ERROR, "LLVM LIB: no jitted body for `{e.fn.name}` - it cannot be exported to C\n")
        return false
    }
    if (LLVMGetNamedFunction(g_mod, e.c_name) != null) {
        to_log(LOG_ERROR, "LLVM LIB: C symbol `{e.c_name}` is already emitted; rename one with [export_c(name = \"...\")]\n")
        return false
    }
    var params <- c_param_types(e, types)
    let made = new_public_function(ctx, e.c_name, LLVMFunctionType(c_return_type(e, types), params))
    let b = made.builder
    defer() {
        LLVMDisposeBuilder(b)
    }
    var frame = LLVMBuildAlloca(b, ft, "frame")
    var cmres_slot : LLVMOpaqueValue?
    if (e.result.cmres) {
        cmres_slot = LLVMBuildAlloca(b, LLVMArrayType(types.t_int8, uint(e.fn.result.sizeOf)), "result")
        LLVMSetAlignment(cmres_slot, uint(e.fn.result.alignOf))
        LLVMBuildStore(b, LLVMBuildPointerCast(b, cmres_slot, types.LLVMVoidPtrType(), ""),
            LLVMBuildStructGEP2(b, ft, frame, uint(length(abi.arg_types)), "res_ptr"))
    }
    store_thunk_args(b, e, abi, made.fn, ft, types, frame)
    let guard_type = LLVMFunctionType(types.t_int32, array(types.LLVMVoidPtrType(),
        types.LLVMVoidPtrType(), types.LLVMVoidPtrType()))
    var guard = declare_shim("jit_lib_invoke_guarded", types.t_int32,
        array(types.LLVMVoidPtrType(), types.LLVMVoidPtrType(), types.LLVMVoidPtrType()))
    var guard_args = array(LLVMGetParam(made.fn, 0u),
        LLVMBuildPointerCast(b, tramp, types.LLVMVoidPtrType(), ""),
        LLVMBuildPointerCast(b, frame, types.LLVMVoidPtrType(), ""))
    var ok = LLVMBuildCall2(b, guard_type, guard, guard_args, "ok")
    emit_thunk_tail(b, ctx, e, abi, made.fn, ft, types, frame, cmres_slot, ok)
    return true
}


def private emit_thunk_tail(b : LLVMBuilderRef; ctx : LLVMContextRef; var e : CExport; abi : ThunkAbi;
                            fn : LLVMOpaqueValue?; ft : LLVMOpaqueType?; var types : PrimitiveTypes?;
                            frame, cmres_slot : LLVMOpaqueValue?; ok : LLVMOpaqueValue?) {
    //! On success the result leaves the frame; on failure the C caller gets zero and `out` is left
    //! exactly as it came in - which is why a cmres result is built in the frame, not in `out`.
    let ret_type = c_return_type(e, types)
    if (!abi.has_slot) {
        LLVMBuildRetVoid(b)
        return
    }
    let ok_bb = LLVMAppendBasicBlockInContext(ctx, fn, "ok")
    let fail_bb = LLVMAppendBasicBlockInContext(ctx, fn, "raised")
    var cond = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntNE, ok, types.ConstI32(0ul), "ran")
    LLVMBuildCondBr(b, cond, ok_bb, fail_bb)
    let slot_index = uint(length(abi.arg_types))

    LLVMPositionBuilderAtEnd(b, ok_bb)
    if (e.result.via_out) {
        // The C caller owns `out` and its alignment is unknown here, hence a byte-wise copy in.
        var out_ptr = LLVMGetParam(fn, uint(length(e.params) + 1))
        var source = e.result.cmres ? cmres_slot : LLVMBuildStructGEP2(b, ft, frame, slot_index, "res")
        LLVMBuildMemCpy(b, out_ptr, 1u, source, uint(e.fn.result.alignOf), types.ConstI64(uint64(e.fn.result.sizeOf)))
        LLVMBuildRetVoid(b)
    } else {
        var slot = LLVMBuildStructGEP2(b, ft, frame, slot_index, "res")
        var value = LLVMBuildLoad2(b, abi.slot_type, slot, "value")
        if (e.fn.result.isBool) {
            value = LLVMBuildZExt(b, value, types.t_int8, "b")
        }
        LLVMBuildRet(b, value)
    }

    LLVMPositionBuilderAtEnd(b, fail_bb)
    if (e.result.via_out || e.fn.result.isVoid) {
        LLVMBuildRetVoid(b)
    } else {
        LLVMBuildRet(b, LLVMConstNull(ret_type))
    }
}


def private emit_runtime_init(ctx : LLVMContextRef; prog : Program?; var types : PrimitiveTypes?;
                              register_all_modules : bool; var used_modules : table<string; bool>&;
                              var dynamic_modules : table<string; bool>&) : tuple<fn : LLVMOpaqueValue?; builder : LLVMBuilderRef; ships_dynamic : bool> {
    //! The once-per-process half of startup, in its own function: N contexts of one library share
    //! one module registry, so registering per context would fail on the second one.
    let made = new_private_function(ctx, LIB_RUNTIME_INIT, LLVMFunctionType(types.t_void, array<LLVMTypeRef>()))
    var init_modules = LLVMGetNamedFunction(g_mod, "initialize_modules")
    if (init_modules == null) {
        init_modules = LLVMAddFunctionWithType(g_mod, "initialize_modules",
            LLVMFunctionType(types.t_void, array<LLVMTypeRef>()))
        set_private_linkage(init_modules)
    }
    var force_dynamic_modules : table<string; bool>
    if (register_all_modules) {
        force_dynamic_modules["UnitTest"] = true
    }
    let ships = emit_standalone_runtime_init(made.builder, prog, types, register_all_modules,
        force_dynamic_modules, used_modules, dynamic_modules)
    // Balances the registration above at process exit; a library has no end of main to do it in.
    var arm = declare_shim("jit_lib_arm_shutdown", types.t_void, array<LLVMTypeRef>())
    LLVMBuildCall2(made.builder, LLVMFunctionType(types.t_void, array<LLVMTypeRef>()), arm,
        array<LLVMOpaqueValue?>(), "")
    return (fn = made.fn, builder = made.builder, ships_dynamic = ships)
}


def private emit_create(ctx : LLVMContextRef; program_context : Context?; prog : Program?; mod : LLVMOpaqueModule?;
                        var types : PrimitiveTypes?; var uids : UidNodes?; prefix : string;
                        var sf : StandaloneFunctions; runtime_init : LLVMOpaqueValue?;
                        runtime_builder : LLVMBuilderRef; register_all_modules : bool; ships_dynamic : bool;
                        dynamic_modules : table<string; bool>) : tuple<fn : LLVMOpaqueValue?; link_whole_lib : bool> {
    //! `<P>_create`: run the process-global initializer once, make the context, then run its own
    //! startup under the guard so a raising [init] returns NULL instead of unwinding into C.
    let void_ptr = types.LLVMVoidPtrType()
    var guard_global = LLVMAddGlobal(g_mod, types.t_int32, LIB_GUARD_GLOBAL)
    set_private_linkage(guard_global)
    LLVMSetInitializer(guard_global, types.ConstI32(0ul))

    let ctx_init = new_private_function(ctx, LIB_CTX_INIT,
        LLVMFunctionType(types.t_void, array(void_ptr, void_ptr)))
    let made = new_public_function(ctx, "{prefix}_create", LLVMFunctionType(void_ptr, array<LLVMTypeRef>()))
    let b = made.builder
    defer() {
        LLVMDisposeBuilder(b)
        LLVMDisposeBuilder(ctx_init.builder)
    }
    var run_once = declare_shim("jit_lib_run_once", types.t_int32, array(void_ptr, void_ptr))
    var registered = LLVMBuildCall2(b, LLVMFunctionType(types.t_int32, array(void_ptr, void_ptr)), run_once,
        array(LLVMBuildPointerCast(b, guard_global, void_ptr, ""),
            LLVMBuildPointerCast(b, runtime_init, void_ptr, "")), "registered")
    // A declined registration answers with a null instance, never an abort in the C caller.
    let ready_bb = LLVMAppendBasicBlockInContext(ctx, made.fn, "ready")
    let declined_bb = LLVMAppendBasicBlockInContext(ctx, made.fn, "declined")
    var can_run = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntNE, registered, types.ConstI32(0ul), "can_run")
    LLVMBuildCondBr(b, can_run, ready_bb, declined_bb)
    LLVMPositionBuilderAtEnd(b, declined_bb)
    LLVMBuildRet(b, LLVMConstNull(void_ptr))
    LLVMPositionBuilderAtEnd(b, ready_bb)
    var global_context = emit_create_standalone_ctx(b, program_context, prog, types, sf.any_pinvoke)
    var guard = declare_shim("jit_lib_invoke_guarded", types.t_int32, array(void_ptr, void_ptr, void_ptr))
    var ok = LLVMBuildCall2(b, LLVMFunctionType(types.t_int32, array(void_ptr, void_ptr, void_ptr)), guard,
        array(global_context, LLVMBuildPointerCast(b, ctx_init.fn, void_ptr, ""), LLVMConstNull(void_ptr)), "ok")
    var finish = declare_shim("jit_lib_create_finish", void_ptr, array(void_ptr, types.t_int32))
    LLVMBuildRet(b, LLVMBuildCall2(b, LLVMFunctionType(void_ptr, array(void_ptr, types.t_int32)), finish,
        array(global_context, ok), ""))

    // One emitted body serves every instance: it reads its context off its own parameter.
    var ctx_param = LLVMGetParam(ctx_init.fn, 0u)
    let whole = emit_standalone_context_init(ctx_init.builder, runtime_builder, program_context, ctx,
        prog, mod, types, uids, sf.funcs, ctx_param, register_all_modules, ships_dynamic,
        sf.used_modules, dynamic_modules)
    LLVMBuildRetVoid(ctx_init.builder)
    return (fn = made.fn, link_whole_lib = whole)
}


struct private FixedEntry {
    name : string
    shim : string
    takes_ctx : bool
    returns_void : bool
}


def private emit_fixed_entry(ctx : LLVMContextRef; var types : PrimitiveTypes?; e : FixedEntry) {
    let void_ptr = types.LLVMVoidPtrType()
    let ret = e.returns_void ? types.t_void : void_ptr
    var params <- e.takes_ctx ? array(void_ptr) : array<LLVMTypeRef>()
    var shim_params <- e.takes_ctx ? array(void_ptr) : array<LLVMTypeRef>()
    var type_params <- e.takes_ctx ? array(void_ptr) : array<LLVMTypeRef>()
    let made = new_public_function(ctx, e.name, LLVMFunctionType(ret, params))
    let b = made.builder
    defer() {
        LLVMDisposeBuilder(b)
    }
    var shim = declare_shim(e.shim, ret, shim_params)
    var args <- e.takes_ctx ? array(LLVMGetParam(made.fn, 0u)) : array<LLVMOpaqueValue?>()
    var call = LLVMBuildCall2(b, LLVMFunctionType(ret, type_params), shim, args, "")
    if (e.returns_void) {
        LLVMBuildRetVoid(b)
    } else {
        LLVMBuildRet(b, call)
    }
}


def private emit_shutdown_trampoline(ctx : LLVMContextRef; var types : PrimitiveTypes?; var uids : UidNodes?;
                                     var funcs : array<FunctionPtr>) : LLVMOpaqueValue? {
    //! The program's `[finalize]` / `[shutdown]` functions, in one guarded body `<P>_destroy` runs
    //! before the context goes. A jitted SimFunction carries no debug-info flags, so the runtime's
    //! own runShutdownScript never finds them - this emission IS the shutdown protocol here.
    var shutdown_fns : array<FunctionPtr>
    for (fn in funcs) {
        if (fn.flags.shutdown) {
            shutdown_fns |> emplace(fn)
        }
    }
    if (shutdown_fns |> empty()) {
        return null
    }
    let void_ptr = types.LLVMVoidPtrType()
    let made = new_private_function(ctx, "__das_lib_ctx_shutdown",
        LLVMFunctionType(types.t_void, array(void_ptr, void_ptr)))
    let b = made.builder
    defer() {
        LLVMDisposeBuilder(b)
    }
    var ctx_arg = LLVMGetParam(made.fn, 0u)
    for (fn in shutdown_fns) {
        let impl_name = uids.get_dll_fn_name(fn).impl()
        var impl = LLVMGetNamedFunction(g_mod, impl_name)
        if (impl != null) {
            LLVMBuildCall2(b, g_fn_types[impl_name], impl, array(ctx_arg), "")
        }
    }
    LLVMBuildRetVoid(b)
    return made.fn
}


def private emit_destroy(ctx : LLVMContextRef; var types : PrimitiveTypes?; prefix : string;
                         shutdown_tramp : LLVMOpaqueValue?) {
    //! `<P>_destroy`: the program's shutdown functions under the guard, then the context itself.
    let void_ptr = types.LLVMVoidPtrType()
    let made = new_public_function(ctx, "{prefix}_destroy", LLVMFunctionType(types.t_void, array(void_ptr)))
    let b = made.builder
    defer() {
        LLVMDisposeBuilder(b)
    }
    var ctx_arg = LLVMGetParam(made.fn, 0u)
    if (shutdown_tramp != null) {
        var guard = declare_shim("jit_lib_invoke_guarded", types.t_int32, array(void_ptr, void_ptr, void_ptr))
        LLVMBuildCall2(b, LLVMFunctionType(types.t_int32, array(void_ptr, void_ptr, void_ptr)), guard,
            array(ctx_arg, LLVMBuildPointerCast(b, shutdown_tramp, void_ptr, ""), LLVMConstNull(void_ptr)), "")
    }
    var destroy = declare_shim("jit_destroy_standalone_ctx", types.t_void, array(void_ptr))
    LLVMBuildCall2(b, LLVMFunctionType(types.t_void, array(void_ptr)), destroy, array(ctx_arg), "")
    LLVMBuildRetVoid(b)
}


def private emit_fixed_entries(ctx : LLVMContextRef; var types : PrimitiveTypes?; var uids : UidNodes?;
                               var funcs : array<FunctionPtr>; prefix : string) {
    //! `<P>_destroy`, `<P>_last_error` and `<P>_shutdown_runtime`.
    emit_destroy(ctx, types, prefix, emit_shutdown_trampoline(ctx, types, uids, funcs))
    emit_fixed_entry(ctx, types, FixedEntry(name = "{prefix}_last_error", shim = "jit_lib_last_error",
        takes_ctx = true))
    emit_fixed_entry(ctx, types, FixedEntry(name = "{prefix}_shutdown_runtime", shim = "jit_lib_shutdown",
        returns_void = true))
}


def public inject_lib(program_context : Context?; ctx : LLVMContextRef; prog : Program?;
                      mod : LLVMOpaqueModule?; var types : PrimitiveTypes?; var uids : UidNodes?;
                      strict : bool; export_all : bool;
                      register_all_modules : bool = false) : tuple<fn : LLVMOpaqueValue?; link_whole_lib : bool> {
    //! Emits the whole C boundary: the four fixed entry points and one thunk per export. Returns a
    //! null `fn` when nothing could be emitted, which run_jit reports as a failed -lib build.
    g_lib_exports |> clear()
    let prefix = lib_symbol_prefix(prog)
    let names = CNames(prefix = prefix, this_module = prog.getThisModule)
    var selected <- collect_c_exports(prog, names, export_all)
    for (r in selected.errors) {
        failed(r.message)
    }
    if (!(selected.errors |> empty())) {
        return (fn = null, link_whole_lib = false)
    }
    var exports <- selected.exports
    if (exports |> empty()) {
        to_log(LOG_ERROR, "LLVM LIB: nothing to export - annotate functions with [export_c], or pass -lib-export-all\n")
        return (fn = null, link_whole_lib = false)
    }
    var sf <- collect_standalone_functions(prog, strict)
    if (!sf.ok) {
        return (fn = null, link_whole_lib = false)
    }
    var dynamic_modules : table<string; bool>
    let rt = emit_runtime_init(ctx, prog, types, register_all_modules, sf.used_modules, dynamic_modules)
    let created = emit_create(ctx, program_context, prog, mod, types, uids, prefix, sf, rt.fn,
        rt.builder, register_all_modules, rt.ships_dynamic, dynamic_modules)
    LLVMBuildRetVoid(rt.builder)
    LLVMDisposeBuilder(rt.builder)
    emit_fixed_entries(ctx, types, uids, sf.funcs, prefix)
    for (e in exports) {
        if (!emit_thunk(ctx, e, uids, types)) {
            return (fn = null, link_whole_lib = false)
        }
    }
    to_log(LOG_INFO, "LLVM LIB: C entry points generated: {prefix}_create/destroy/last_error plus {length(exports)} exports, {length(sf.funcs)} functions\n")
    g_lib_exports |> clear()
    g_lib_exports |> push_clone_from(exports)
    return (fn = created.fn, link_whole_lib = created.link_whole_lib)
}
