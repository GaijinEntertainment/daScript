options gen2
options indenting = 4
options strict_smart_pointers = false
options stack = 4_194_304

require llvm/daslib/llvm_boost
require llvm/daslib/llvm_dll_utils
require llvm/daslib/llvm_exe
require llvm/daslib/llvm_jit
require llvm/daslib/llvm_jit_common
require llvm/daslib/llvm_jit_intrin
require llvm/daslib/llvm_jit_cli

require daslib/ast_boost
require daslib/strings_boost
require daslib/defer

require daslib/fio
require daslib/option
require daslib/result
require jit

module llvm_jit_run shared private

//! JIT codegen driver shared by the `jit_llvm` simulate-macro (llvm_macro) and
//! the `utils/jit` tool. `run_jit` performs the whole compile->IR->optimize->
//! emit(.dll/.exe/.wasm) sequence for an already-simulated program+context, so
//! callers do not need to inject the `just_in_time` module at runtime.

var LINK_WHOLE_LIB = false // when true, standalone exe links against the whole library; when false, only against runtime

// Bump this constant whenever LLVM codegen / linking logic changes in a way that
// invalidates cached DLLs (e.g. edits to llvm_jit.das, llvm_macro.das, llvm_jit_common.das,
// runtime helper ABI, default target triple). Cache filenames fold this in, so a bump
// makes every previously written DLL miss the cache on the next run and get GC'd.
let LLVM_JIT_CODEGEN_VERSION : uint64 = 0x2ful   // uwtable on all emitted functions (EH through JIT frames)

let JIT_FNV_PRIME : uint64 = 1099511628211ul

// `options _jit_fast_math = true` stamps every FP instruction with all fast-math flags before opt — lets
// reductions reassociate/vectorize and FP contract everywhere. NON-bit-exact, but this is the same FP
// laxity llama.cpp/ggml already runs with (hand-coded FMA + ~2ulp poly exp + reassociated SIMD reductions;
// ggml CPU is just -O3, no -ffast-math flag — it hand-writes the equivalent). Measured greedy-STABLE on
// Llama-3.2-1B prefill (identical next token @512/1024/2048, ~1% logit drift) for +8-10% end-to-end.
// Default off (bit-exact). The magic-number exp survives reassoc — LLVM keeps `z` (its bits feed the
// exponent bitcast), so it does NOT fold `(x*log2e + M) - M` away.
var g_jit_fast_math = false

def private apply_fast_math_to_module(m : LLVMOpaqueModule?) {
    var fn = LLVMGetFirstFunction(m)
    while (fn != null) {
        var bb = LLVMGetFirstBasicBlock(fn)
        while (bb != null) {
            var inst = LLVMGetFirstInstruction(bb)
            while (inst != null) {
                if (LLVMCanValueUseFastMathFlags(inst) != 0) {
                    LLVMSetFastMathFlags(inst, 0x7fu)   // reassoc|nnan|ninf|nsz|arcp|contract|afn
                }
                inst = LLVMGetNextInstruction(inst)
            }
            bb = LLVMGetNextBasicBlock(bb)
        }
        fn = LLVMGetNextFunction(fn)
    }
}

// Loop tune hints (vectorize / vectorize_width / unroll / unroll_count / interleave_count)
// live on ExprFor / ExprWhile annotations and reach ONLY the JIT — attach_loop_metadata
// turns them into LLVM !llvm.loop metadata. They never enter the interpreter SimNode tree
// that get_function_aot_hash walks, so the semantic hash is blind to them: a re-tuned
// kernel ([tuned] restamping a different perm from a changed box_profile.json) has an
// IDENTICAL aot hash and would silently reuse a stale DLL. Fold them into the JIT-only
// cache key here, alongside LLVM_JIT_CODEGEN_VERSION / g_jit_fast_math.
class private LoopHintHashVisitor : AstVisitor {
    h : uint64
    def fold_args(args : AnnotationArgumentList) {
        // Fold the ONE value field the arg's type says is live. bValue/iValue/fValue share a C++
        // union — reading a non-live member is UB (a bool load of vectorize_width=8 aborts ubsan
        // and lets clang -O2 miscompile: chunk workers died with SIGSEGV on linux Release CI).
        // Type coverage stays total: any basicType re-keys the DLL via its own live field.
        for (a in args) {
            h = (h ^ hash(a.name)) * JIT_FNV_PRIME
            h = (h ^ uint64(int(a.basicType))) * JIT_FNV_PRIME
            if (a.basicType == Type.tBool) {
                h = (h ^ (a.bValue ? 1ul : 0ul)) * JIT_FNV_PRIME
            } elif (a.basicType == Type.tInt) {
                h = (h ^ uint64(a.iValue)) * JIT_FNV_PRIME
            } elif (a.basicType == Type.tFloat) {
                h = (h ^ hash(a.fValue)) * JIT_FNV_PRIME
            } elif (a.basicType == Type.tString) {
                h = (h ^ hash(a.sValue)) * JIT_FNV_PRIME
            }
        }
    }
    def override preVisitExprFor(expr : ExprFor?) : void {
        fold_args(expr.annotations)
    }
    def override preVisitExprWhile(expr : ExprWhile?) : void {
        fold_args(expr.annotations)
    }
}

// Complete under jit_all_functions (the default): every used function is a top-level entry
// in `funcs`, so folding each function's own body covers the whole compiled program.
def private fold_loop_hints(funcs : array<FunctionPtr>; seed : uint64) : uint64 {
    var vis = new LoopHintHashVisitor()
    vis.h = seed
    make_visitor(*vis) $(adapter) {
        for (fn in funcs) {
            visit(fn.body, adapter)
        }
    }
    let r = vis.h
    unsafe { delete vis }
    return r
}

def jit_dll_basename(funcs : array<FunctionPtr>; opt_level : int; size_level : int;
                     emit_prologue : bool; debug_info : bool) : string {
    var h = LLVM_JIT_CODEGEN_VERSION
    for (fn in funcs) {
        h = (h ^ get_function_aot_hash(fn)) * JIT_FNV_PRIME
    }
    h = fold_loop_hints(funcs, h)   // JIT-only loop tune hints (see LoopHintHashVisitor)
    h = (h ^ uint64(opt_level)) * JIT_FNV_PRIME
    h = (h ^ uint64(size_level)) * JIT_FNV_PRIME
    h = (h ^ (emit_prologue ? 1ul : 0ul)) * JIT_FNV_PRIME
    h = (h ^ (debug_info ? 1ul : 0ul)) * JIT_FNV_PRIME
    h = (h ^ (g_jit_fast_math ? 1ul : 0ul)) * JIT_FNV_PRIME   // fast-math changes codegen → distinct cache
    // EH hosts get uwtable on every function, longjmp hosts don't — a table-less DLL
    // cached by a longjmp host must never cache-hit on an EH host (panic would crash).
    h = (h ^ (das_is_exceptions_enabled() ? 1ul : 0ul)) * JIT_FNV_PRIME
    return "{h}"
}

def jit_gc_stale_dlls(subdir : string; keep_basename : string) {
    // Delete any file in subdir whose basename does not match the current hash.
    // Covers stale `.dll` plus linker artifacts (`.o`, `.lib`, `.exp`, `.pdb`).
    let keep_prefix = "{keep_basename}."
    dir(subdir) $(fname) {
        if (fname == "." || fname == ".." || fname |> starts_with(keep_prefix)) return
        let full_path = "{subdir}/{fname}"
        var err : string
        let ok = remove(full_path, err)
        if (ok && empty(err)) {
            to_log(LOG_INFO, "LLVM JIT: GC stale artifact {full_path}\n")
        } else {
            to_log(LOG_WARNING, "LLVM JIT: failed to GC stale artifact {full_path}: {err}\n")
        }
    }
}

struct JitState {
    dll : void?
    ee : void?
    ctx : void?
}

[macro_function]
def finalize_jit(use_dll : bool; gen_exe : bool; lib_handle : DLLHandle?) : JitState? {
    verify(!(is_in_completion() || is_compiling_macros()))
    verify(!(g_mod == null))    // prevent double finalize
    if (LLVM_LOG_RESULT) {
        LLVMDumpModule(g_mod)
        fflush(fstderr())
    }
    var result : JitState? = null;
    if (use_dll || gen_exe) {
        // Note: ExecutionEngine is owner of the g_mod
        LLVMDisposeExecutionEngine(g_engine);
        LLVMContextDispose(g_ctx);

        // JitState is dll only.
        if (use_dll) {
            result = new JitState(dll = lib_handle.handle, ee = null, ctx = null)
        } else {
            // Nothing to finalize.
            result = new JitState(dll = null, ee = null, ctx = null)

        }
    } else {
        g_engine |> LLVMRunStaticConstructors()

        // JitState is engine + ctx.
        result = new JitState(dll = null, ee = g_engine, ctx = g_ctx)
    }
    g_engine = null
    g_ctx = null
    g_mod = null
    unsafe {
        delete g_str2v
    }
    return result
}

// After a codegen failure the global LLVM state (g_mod/g_ctx/g_engine) is left
// built but unusable. init_jit bails while g_mod != null, so the NEXT run_jit in
// the same process would inherit it and cascade-fail. Dispose + null it (mirrors
// finalize_jit's teardown) so each file starts clean — this is what lets a
// one-process bulk run survive an unjittable file (e.g. one using quote/qmacro)
// instead of poisoning every file after it.
[macro_function]
def reset_jit_globals_after_failure() {
    if (g_engine != null) {
        LLVMDisposeExecutionEngine(g_engine)    // engine owns g_mod, frees it too
    } elif (g_ctx != null) {
        LLVMContextDispose(g_ctx)
    }
    g_engine = null
    g_ctx = null
    g_mod = null
    g_failed = false
    unsafe {
        delete g_str2v
    }
    // Drain the per-run codegen accumulators the skipped generate_global_ctors_dtors
    // would have cleared — otherwise stale fileinfo entries crash the next file.
    reset_codegen_accumulators()
}

//! Run the full JIT backend for an already-compiled & simulated program:
//! collects the functions to JIT (honoring [jit]/[no_jit] and jit_all),
//! content-addresses the DLL, generates and optimizes IR, and emits a
//! .dll / .exe / .wasm per the program's jit policies. Returns true.
[macro_function]
def public run_jit(prog : Program?; var ctx : Context?) : bool {
    var funcs : array<FunctionPtr>
    var disabled : array<FunctionPtr>
    var disableJitVisitor = new DisableJitVisitor()

    let r <- parse_args(type<JitCliOptions>)
    let cli_opts = r |> unwrap_or_default
    assume jit_all_functions = prog.policies.jit_jit_all_functions
    let debug_info = cli_opts.debug_info |> unwrap_or(prog.policies.jit_debug_info)
    let dump_ir = cli_opts.dump_ir |> unwrap_or(false)
    if (prog.policies.jit_dll_mode && !das_is_dll_build()) {
        to_log(LOG_WARNING, "LLVM JIT: dll mode is enabled, however " +
            "daslang build is static, disabling dll mode.\n")
    }
    let gen_exe = prog.policies.jit_exe_mode
    // Drives the $::is_standalone_exe JIT intrinsic in llvm_jit_intrin so daslang
    // code can constant-fold to true when this codegen run produces a standalone exe.
    llvm_jit_intrin::g_jit_exe_mode = gen_exe
    defer() { llvm_jit_intrin::g_jit_exe_mode = false; }
    let exe_main = "main" // prog.policies.jit_entry_point |> empty() ? "main" : prog.policies.jit_entry_point
    let use_dll = prog.policies.jit_dll_mode && das_is_dll_build() && !gen_exe
    let opt_level = cli_opts.opt_level |> unwrap_or(prog.policies.jit_opt_level)
    let size_level = cli_opts.size_level |> unwrap_or(prog.policies.jit_size_level)
    let emit_prologue = prog.policies.emit_prologue
    // If you're sure `dynamic` functions is not reachable set this flag
    // to `true` and fail in runtime on `jit_trap` if they turns
    // out to be reachable.
    let exe_strict = true
    assume config_out_path = string(prog.policies.jit_output_path)
    var output_path = config_out_path |> empty() ? ".jitted_scripts/{prog.thisNamespace}" : config_out_path
    assume path_to_shared_lib = string(prog.policies.jit_path_to_shared_lib)
    // CLI --jit-path-to-linker wins over the script-level policy.
    let path_to_linker = cli_opts.path_to_linker |> unwrap_or(string(prog.policies.jit_path_to_linker))
    // Cross-compile triple: --jit-target=<triple> CLI wins over the
    // script-level `options jit_target = "..."`.
    let opt_target = (prog._options |> find_arg("jit_target")) ?as tString ?? ""
    let target_triple = cli_opts.target |> unwrap_or(opt_target)
    let gen_wasm = gen_exe && (target_triple |> starts_with("wasm"))
    // Optional CLI override for the wasm runtime archive location. Falls
    // back to `options jit_runtime_lib = "..."` then write_wasm's
    // find_runtime_lib() auto-locate under web/output/lib/.
    let runtime_lib_override = cli_opts.runtime_lib |> unwrap_or((prog._options |> find_arg("jit_runtime_lib")) ?as tString ?? "")
    // --jit-emit-object: emit the wasm object and skip the emcc link (the caller
    // links it, e.g. daspkg release wasm). Sidesteps the link entirely, including
    // the Windows popen-quoting failure path in link_wasm.
    let emit_object = cli_opts.emit_object |> unwrap_or(false)
    // --jit-threads: emit a -pthread-compatible wasm object (+atomics,+bulk-memory).
    // Set the codegen global before any IR is built so both the data-layout machine
    // and write_wasm's emission machine pick up the threaded feature set.
    g_target_wasm_threads = cli_opts.threads |> unwrap_or(false)
    // --jit-check-abi: emit the handled-type host-vs-target layout safe-check into
    // initialize_modules(). Set before codegen so llvm_exe's sweep sees the flag.
    g_jit_check_handled_abi = cli_opts.check_abi |> unwrap_or(false)
    // Extra args appended verbatim to the host linker cmd. CLI > script option.
    let linker_string = cli_opts.linker_string |> unwrap_or((prog._options |> find_arg("jit_linker_string")) ?as tString ?? "")

    // Set global options
    LLVM_JIT_ALLOW_UNALIGNED_VECTOR_READ_OUT_OF_BOUNDS = prog._options |> find_arg("jit_vec3_ldu") ?as tBool ?? LLVM_JIT_ALLOW_UNALIGNED_VECTOR_READ_OUT_OF_BOUNDS_DEFAULT
    g_jit_fast_math = prog._options |> find_arg("_jit_fast_math") ?as tBool ?? false

    make_visitor(*disableJitVisitor) $(disableJitVisitorAdapter) {
        prog |> for_each_module() $(mod) {
            mod |> for_each_function("") $(fun) {
                let rqj = fun.moreFlags.requestJit
                fun.moreFlags.requestJit = false
                if (fun.flags.used && (jit_all_functions || rqj)) {
                    if (!fun.moreFlags.requestNoJit) {
                        disableJitVisitor.disable = false
                        fun |> visit(disableJitVisitorAdapter)
                        if (!disableJitVisitor.disable) {
                            fun.moreFlags.requestJit = true
                            funcs |> emplace(fun)
                        } else {
                            to_log(LOG_INFO, "LLVM JIT: disabled {fun.name}\n")
                        }
                    } else {
                        disabled |> emplace(fun)
                    }
                }
            }
        }
    }
    unsafe {
        delete disableJitVisitor
    }
    if (!empty(funcs)) {
        let totalTime = ref_time_ticks()
        // Content-address the DLL so that distinct (AST + codegen version + opt flags)
        // combinations land in distinct files. Same inputs -> same filename -> cache hit.
        // Only applied in dll-mode with the default output path; if the user pinned
        // jit_output_path explicitly or we're producing an exe, keep their path.
        var dll_hash_basename : string
        if (use_dll && config_out_path |> empty()) {
            dll_hash_basename = jit_dll_basename(funcs, opt_level, size_level, emit_prologue, debug_info)
            output_path = ".jitted_scripts/{prog.thisNamespace}/{dll_hash_basename}"
        }
        // Pass target_triple so init_jit can pin the module data layout
            // BEFORE IR construction (wasm32 cross-compile needs 4-byte pointer
            // sizes baked into GEP folding from the very first IR instruction).
            init_jit(opt_level |> uint, target_triple)

        var jit_flags : LlvmJitFlags
        jit_flags.emit_prologue = emit_prologue
        jit_flags.gen_exe = gen_exe
        jit_flags.need_di = debug_info

        let attrs = new Attributes(g_ctx)
        var uids = create_uid_nodes(prog, funcs)
        var fake_visitor = new LlvmJitVisitor(ctx, g_prim_t, uids, jit_flags, null, attrs)
        ast_gc_guard() {
            for (fun in funcs) {
                fake_visitor->add_llvm_functions(fun)
            }
        }
        unsafe {
            delete fake_visitor
        }
        var g_dynamic_lib_handle : DLLHandle? = null;
        var recompile_prog = true
        if (use_dll) {
            g_dynamic_lib_handle = get_dll_by_path(output_path)
            let (missing, disabling) = get_dll_missing(funcs, disabled, g_dynamic_lib_handle, uids)
            let dll_changed = !(missing |> empty() && disabling |> empty())
            recompile_prog = dll_changed
            if (recompile_prog) {
                g_dynamic_lib_handle.reset();
                to_log(LOG_INFO, "LLVM JIT: DLL cache miss, codegen for {output_path}.dll\n")
            } else {
                to_log(LOG_INFO, "LLVM JIT: DLL cache hit {output_path}.dll\n")
            }
        }
        var jit_has_externals = false
        if (recompile_prog) {
            for (fun in funcs) {
                jit_has_externals = generate_llvm(ctx, g_prim_t, uids, attrs, fun, jit_flags) || jit_has_externals
                let fnmna = uids.get_dll_fn_name(fun)
                var fn_impl = LLVMGetNamedFunction(g_mod, fnmna.impl())
                var fn = LLVMGetNamedFunction(g_mod, fnmna.publ())
                assume maybe_mod = empty(prog.thisModuleName) ? "" : "in module {prog.thisModuleName}"
                if (!LLVMVerifyFunction(fn_impl, LLVMVerifierFailureAction.LLVMPrintMessageAction, false)) {
                    // to_log(LOG_ERROR, "LLVM JIT: {LLVMPrintModuleToString(g_mod)}\n");
                    reset_jit_globals_after_failure()
                    panic("Internal jit error. Failed to get IR of '{fun.name} implementation' {maybe_mod}.\n")
                }
                if (!LLVMVerifyFunction(fn, LLVMVerifierFailureAction.LLVMPrintMessageAction, false)) {
                    // to_log(LOG_ERROR, "LLVM JIT: {LLVMPrintModuleToString(g_mod)}\n");
                    reset_jit_globals_after_failure()
                    panic("Internal jit error. Failed to get IR of '{fun.name}' in module {maybe_mod}.\n")
                }
            }
            if (gen_exe) {
                let res = inject_main(ctx, g_ctx, prog, exe_main, g_mod, g_prim_t, uids, exe_strict,
                    cli_opts.register_all_modules |> unwrap_or(false))
                LINK_WHOLE_LIB = res.link_whole_lib
                if (res.fn == null) {
                    finalize_jit(use_dll, gen_exe, g_dynamic_lib_handle)
                    // JIT failed. Couldn't create executable.
                    // Still return true, codegen ran correctly.
                    return true
                }
            }
            if (!LLVMVerifyModule(g_mod, LLVMVerifierFailureAction.LLVMPrintMessageAction, false)) {
                g_failed = true
            }
            if (g_failed) {
                let fn_names = join([ for (fn in funcs); "{fn.name}" ], ",")
                reset_jit_globals_after_failure()
                panic("Internal jit error. Failed to get IR for functions {fn_names}.\n")
            }
            generate_global_ctors_dtors(g_prim_t, !(use_dll || gen_exe))
            if (g_jit_fast_math) {
                apply_fast_math_to_module(g_mod)   // EXPERIMENT: stamp all FP ops fast (non-bit-exact ceiling)
            }
            optimize_llvm_module(opt_level |> uint(), size_level |> uint(), LLVM_INLINE_THRESHOLD, !gen_exe)
            if (dump_ir) {
                // Let's remove unused attributes before we print IR.
                var pmo = LLVMCreatePassBuilderOptions()
                let dce_err = LLVMRunPasses(g_mod, "globaldce", null, pmo)
                if (dce_err != null) {
                    let dce_msg = LLVMGetErrorMessage(dce_err)
                    to_log(LOG_WARNING, "LLVM globaldce failed: {dce_msg}\n")
                    LLVMDisposeErrorMessage(dce_msg)
                }
                LLVMDisposePassBuilderOptions(pmo)
                let module_str = LLVMPrintModuleToString(g_mod)
                let msg = clone_string(module_str)
                LLVMDisposeMessage(module_str)
                to_log(LOG_INFO, "LLVM JIT: {msg}\n");
            }
            if (!LLVMVerifyModule(g_mod, LLVMVerifierFailureAction.LLVMPrintMessageAction, false)) {
                g_failed = true
            }
            if (g_failed) {
                let fn_names = join([ for (fn in funcs); "{fn.name}" ], ",")
                reset_jit_globals_after_failure()
                panic("Internal jit error. Failed to get IR for functions {fn_names}.\n")
            }
            if (gen_wasm) {
                mkdir_rec(dir_name(output_path))
                // path_to_linker is reused as the optional emcc override
                // for the wasm link (mirrors how it's used for clang-cl
                // on the host path).
                write_wasm(g_mod, output_path, target_triple, path_to_linker, jit_has_externals, runtime_lib_override, emit_object)
            } elif (gen_exe) {
                mkdir_rec(dir_name(output_path))
                write_exe(g_mod, output_path, path_to_shared_lib, path_to_linker, linker_string, LINK_WHOLE_LIB)
            } elif (use_dll) {
                mkdir_rec(dir_name(output_path))
                write_dll(g_mod, output_path, path_to_shared_lib, path_to_linker, linker_string, LINK_WHOLE_LIB)

                // Open just stored DLL, so we can keep working.
                g_dynamic_lib_handle.reset()
                g_dynamic_lib_handle = get_dll_by_path(output_path)
                let (missing, disabling) = get_dll_missing(funcs, disabled, g_dynamic_lib_handle, uids)
                assert(missing |> empty() && disabling |> empty())

                // Clean up stale DLLs in the per-script subdirectory.
                if (!(dll_hash_basename |> empty())) {
                    jit_gc_stale_dlls(dir_name(output_path), dll_hash_basename)
                }
            }
        }
        if (use_dll) {
            assert(g_dynamic_lib_handle.handle != null)
            var extern_resolver = new ResolveExternVisitor(g_dynamic_lib_handle, g_prim_t, ctx, clone(uids))
            make_visitor(*extern_resolver) $(adapter_resolve) {
                for (fn in funcs) {
                    visit(fn, adapter_resolve)
                }
            }
        }
        if (!gen_exe) {
            for (fun in funcs) {
                let mangled_name = get_mangled_name(fun)
                let MNH = hash(mangled_name)
                let simfun = get_function_by_mangled_name_hash(MNH, *ctx)
                var gen = generate_llvm_code(g_dynamic_lib_handle, uids.get_dll_fn_name(fun), use_dll)
                if (gen != null) {
                    gen |> unsafe(instrument_jit(simfun, fun.at, *ctx))
                } else {
                    unsafe(remove_jit(simfun))
                }
            }
        }
        var result = finalize_jit(use_dll, gen_exe, g_dynamic_lib_handle)

        set_jit_state(*ctx, result.dll, result.ee, result.ctx)

        if (LLVM_JIT_LOG) {
            let totalDt = double(get_time_usec(totalTime)) / 1000000.0lf
            to_log(LOG_INFO, "LLVM JIT: {length(funcs)} functions in {totalDt} sec\n")
        }
    }
    return true
}
