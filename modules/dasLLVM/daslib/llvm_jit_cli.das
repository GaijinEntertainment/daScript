options gen2
options indenting = 4

module llvm_jit_cli shared private

require daslib/clargs

//! Shared JIT command-line option schema.
//!
//! Declared here (rather than inside `llvm_macro`) so both the `jit_llvm`
//! simulate-macro and the `utils/jit` driver parse the exact same flags.

[CommandLineArgs]
struct public JitCliOptions {
    @clarg_name = "jit-opt-level"
    @clarg_doc = "JIT: LLVM optimization level (0..3); 0 disables all passes"
    opt_level : Option<int>

    @clarg_name = "jit-size-level"
    @clarg_doc = "JIT: LLVM size-optimization level (0..2)"
    size_level : Option<int>

    @clarg_short = "g"
    @clarg_name = "jit-debug"
    @clarg_doc = "JIT: emit debug info / symbols"
    debug_info : Option<bool>

    @clarg_short = "d"
    @clarg_name = "jit-dump"
    @clarg_doc = "JIT: dump generated LLVM IR"
    dump_ir : Option<bool>

    @clarg_name = "jit-target"
    @clarg_doc = "JIT: cross-compile target triple (use with -exe; e.g. wasm32-unknown-emscripten)"
    target : Option<string>

    @clarg_name = "jit-runtime-lib"
    @clarg_doc = "JIT: explicit path to libDaScript_runtime.a (wasm cross-compile); overrides auto-locate under web/output/lib/"
    runtime_lib : Option<string>

    @clarg_name = "jit-linker-string"
    @clarg_doc = "JIT: extra args inserted verbatim into the host linker cmd (e.g. -fsanitize=undefined)"
    linker_string : Option<string>

    @clarg_name = "jit-path-to-linker"
    @clarg_doc = "JIT: explicit linker binary (overrides default c++/clang-cl). Match the compiler daslang was built with to avoid sanitizer runtime mismatch."
    path_to_linker : Option<string>

    @clarg_name = "jit-register-all-modules"
    @clarg_doc = "JIT (-exe only): register all builtin native modules (math, fio, dasbind, ...) at exe startup, so a standalone compiler-driver exe can recompile arbitrary daslang at runtime"
    register_all_modules : Option<bool>
}
