options gen2
options indenting = 4
options no_aot = true

module tune shared public

require daslib/ast
require daslib/ast_boost

require ?llvm/daslib/llvm_code llvm/daslib/llvm_code public
require ?llvm/daslib/llvm_tune llvm/daslib/llvm_tune public

//! The six kernel-tuning annotations - `[tune_perm]`, `[tune_companion]`, `[tune]`,
//! `[tune_scope]`, `[tune_policy]` and `[llvm_code]` - plus the default-policy infer pass, as
//! shells every program can spell. The framework that implements them lives in
//! `llvm/daslib/llvm_tune` and `llvm/daslib/llvm_code`, reached through the path guards above
//! and re-exported, so `require daslib/tune` gives exactly what `require llvm/daslib/llvm_tune`
//! gave; the guarded require is also the witness that makes `builtin_module_exists` below
//! answer for this compile. Every tune annotation registers HERE and nowhere else, because a
//! module's macro state is per-module: the `[tune_scope]` bank and the pass that reads it have
//! to run in one macro context. Without dasLLVM every apply is a no-op - no sidecar read, no
//! stamp, no `_variants()` registry, no tuner spawn - and the reference body IS the function.
//! Framework semantics, arguments and the sidecar contract: `skills/tune.md`.

[function_macro(name = "tune_perm")]
class private TunePermShell : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        static_if (typeinfo builtin_module_exists(llvm_tune)) {
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
        static_if (typeinfo builtin_module_exists(llvm_tune)) {
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
        static_if (typeinfo builtin_module_exists(llvm_tune)) {
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
        static_if (typeinfo builtin_module_exists(llvm_tune)) {
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
        static_if (typeinfo builtin_module_exists(llvm_tune)) {
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
        static_if (typeinfo builtin_module_exists(llvm_code)) {
            return llvm_code_apply(func, group, args, errors)
        } else {
            return true
        }
    }
}

[infer_macro]
class private TuneDefaultPolicyShell : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        static_if (typeinfo builtin_module_exists(llvm_tune)) {
            return tune_default_policy_apply(prog, mod)
        } else {
            return false
        }
    }
}
