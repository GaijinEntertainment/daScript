options gen2
options indenting = 4
options strict_smart_pointers

module cross_context shared public

//! A module required after the walk, and calls into its macro context.
//!
//! ``require_module_now("llvm/daslib/llvm_macro")`` compiles a ``shared`` module and its
//! prerequisites into the process at the point of the call - from a macro, a simulate
//! macro or a running script - and answers the one already there on every later call.
//! The module is not a dependency of the caller: its symbols stay out of the caller's
//! scope and its macros do not apply to the caller's program. The caller reaches it
//! through its macro context - ``macro_context_of`` - and calls a function there by
//! name with ``call_in_context``, which passes a pointer to the result as the last
//! argument, the way ``[apply_in_context]`` does. The module has a macro context when
//! it declares a macro - a ``[_macro]`` function is the smallest - and a function called
//! by name is ``[export]``ed, since the context keeps only exported, ``[init]`` and
//! macro-init functions. A function called that way runs under the target context's
//! lock, so it must not call back into the caller's context.

require daslib/ast public
require daslib/rtti public
require daslib/debugger public

def private report_late_require(name : string; var mod : Module?; issues : das_string) : Module? {
    if (mod == null) {
        to_log(LOG_ERROR, "require_module_now({name}): {issues}")
    }
    return mod
}

def require_module_now(name : string) : Module? {
    //! Compiles `name` - a `shared` module - and its prerequisites now, or answers the one
    //! already in the process; the compiling program's policies apply when one is bound.
    //! A failure logs the errors and answers null.
    var found : Module?
    var inscope no_access : smart_ptr<FileAccess>
    if (compiling_program() != null) {
        require_module_now(name, no_access, compiling_program().policies) $(var mod : Module?; issues : das_string) {
            found = report_late_require(name, mod, issues)
        }
    } else {
        using() $(var cop : CodeOfPolicies) {
            require_module_now(name, no_access, cop) $(var mod : Module?; issues : das_string) {
                found = report_late_require(name, mod, issues)
            }
        }
    }
    return found
}

def macro_context_of(name : string) : Context? {
    //! The macro context of the shared module `name`, required now when the process lacks
    //! it; null, logged, when the module has no macros.
    var mod = require_module_now(name)
    if (mod == null) {
        return null
    }
    if (!has_macro_context(mod)) {
        to_log(LOG_ERROR, "macro_context_of({name}): the module has no macro context")
        return null
    }
    return unsafe(addr(find_macro_context(mod)))
}

def call_in_context(ctx : Context?; fn : string; var result : auto(T)&) {
    //! Calls `fn` in `ctx` with a pointer to `result` as its only argument.
    unsafe {
        invoke_in_context(ctx, fn, addr(result))
    }
}

def call_in_context(ctx : Context?; fn : string; a0 : auto(A0); var result : auto(T)&) {
    //! Calls `fn` in `ctx` with `a0` and a pointer to `result`.
    unsafe {
        invoke_in_context(ctx, fn, a0, addr(result))
    }
}

def call_in_context(ctx : Context?; fn : string; a0 : auto(A0); a1 : auto(A1); var result : auto(T)&) {
    //! Calls `fn` in `ctx` with `a0`, `a1` and a pointer to `result`.
    unsafe {
        invoke_in_context(ctx, fn, a0, a1, addr(result))
    }
}

def call_in_context(ctx : Context?; fn : string; a0 : auto(A0); a1 : auto(A1); a2 : auto(A2); var result : auto(T)&) {
    //! Calls `fn` in `ctx` with `a0`, `a1`, `a2` and a pointer to `result`.
    unsafe {
        invoke_in_context(ctx, fn, a0, a1, a2, addr(result))
    }
}
