options gen2
options indenting = 4
options no_aot = true

module tune shared public

require daslib/ast
require daslib/ast_boost

require ?llvm llvm/daslib/llvm_code public
require ?llvm llvm/daslib/llvm_tune public

//! The six kernel-tuning annotations - `[tune_perm]`, `[tune_companion]`, `[tune]`,
//! `[tune_scope]`, `[tune_policy]` and `[llvm_code]` - plus the default-policy infer pass, as
//! shells every program can spell. The framework that implements them lives in
//! `llvm/daslib/llvm_tune` and `llvm/daslib/llvm_code`, reached through the `?llvm` guards
//! above and re-exported, so `require daslib/tune` gives exactly what `require llvm/daslib/llvm_tune`
//! gave. The guard is the `llvm` C++ witness module, compiled in exactly when the build is
//! configured with dasLLVM - the das files being on disk never counts; the guarded require is
//! then what makes `module_exists` below answer for this compile (`module_exists`, not
//! `builtin_module_exists`: visible from this module, the same answer inside a tool's nested
//! compile). Every tune annotation registers HERE and nowhere else, because a
//! module's macro state is per-module: the `[tune_scope]` bank and the pass that reads it have
//! to run in one macro context. Without dasLLVM every apply is a no-op - no sidecar read, no
//! stamp, no `_variants()` registry, no tuner spawn - and the reference body IS the function.
//! Framework semantics, arguments and the sidecar contract: `skills/tune.md`.

[function_macro(name = "tune_perm")]
class private TunePermShell : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        static_if (typeinfo module_exists(llvm_tune)) {
            return tune_perm_apply(func, group, args, errors)
        } else {
            return true
        }
    }
}

[function_macro(name = "tune_companion")]
class private TuneCompanionShell : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        static_if (typeinfo module_exists(llvm_tune)) {
            return tune_companion_apply(func, group, args, errors)
        } else {
            return true
        }
    }
}

[function_macro(name = "tune")]
class private TuneShell : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        static_if (typeinfo module_exists(llvm_tune)) {
            return tune_apply(func, group, args, errors)
        } else {
            return true
        }
    }
}

[structure_macro(name = "tune_scope")]
class private TuneScopeShell : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        static_if (typeinfo module_exists(llvm_tune)) {
            return tune_scope_apply(st, group, args, errors)
        } else {
            return true
        }
    }
}

[function_macro(name = "tune_policy")]
class private TunePolicyShell : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        static_if (typeinfo module_exists(llvm_tune)) {
            return tune_policy_apply(func, group, args, errors)
        } else {
            return true
        }
    }
}

[function_macro(name = "llvm_code")]
class private LlvmCodeShell : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        static_if (typeinfo module_exists(llvm_code)) {
            return llvm_code_apply(func, group, args, errors)
        } else {
            return true
        }
    }
}

[infer_macro]
class private TuneDefaultPolicyShell : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        static_if (typeinfo module_exists(llvm_tune)) {
            return tune_default_policy_apply(prog, mod)
        } else {
            return false
        }
    }
}

//! Log the tune status table at `LOG_INFO`, prefixed by `appName`: `<n>/<total> kernels tuned for
//! this box`, one line per function, a `--tune` hint when any kernel sits on a fallback tier.
//! Silent when the table is empty, and in a build without the framework - nothing was stamped.
[arch(at = "ARCHITECTURE.md#tune-shells-module-exists")]
def log_tune_status(appName : string) {
    var inscope rows <- tune_status_rows()
    if (empty(rows)) {
        return
    }
    var tuned = 0
    for (r in rows) {
        tuned += (r.source == "manifest" || r.source == "profile") ? 1 : 0
    }
    to_log(LOG_INFO, "{appName}: tune status ({tuned}/{length(rows)} kernels tuned for this box):\n")
    for (r in rows) {
        to_log(LOG_INFO, "  {r.fname}: {empty(r.suffix) ? "reference" : r.suffix} ({r.source})\n")
    }
    if (tuned < length(rows)) {
        to_log(LOG_INFO, "{appName}: run with --tune to tune the remaining kernels for this box\n")
    }
}

//! The status table as plain strings - the framework's row type never crosses the guard, so the
//! caller above is one function in every build; empty without the framework.
def private tune_status_rows() : array<tuple<fname : string; suffix : string; source : string>> {
    var rows : array<tuple<fname : string; suffix : string; source : string>>
    static_if (typeinfo module_exists(llvm_tune)) {
        var inscope st <- tune_status()
        rows |> reserve(length(st))
        for (s in st) {
            rows |> push((fname = s.fname, suffix = s.suffix, source = s.source))
        }
    }
    return <- rows
}
