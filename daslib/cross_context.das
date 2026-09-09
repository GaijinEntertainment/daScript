options gen2
options indenting = 4
options strict_smart_pointers

module cross_context shared public

//! A module required after the walk, and calls into its macro context by a name held once there.

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
    //! Compiles `name` - a `shared` module - and its prerequisites now, under the compiling
    //! program's policies and file access, or answers the one already in the process; a panic
    //! outside a compile (a macro, a simulate macro, an `[init]`). Null, logged, on failure.
    if (!is_compiling()) {
        panic("require_module_now({name}): no program is compiling - a require after the walk is issued from a macro, a simulate macro or an [init], not from a running script")
    }
    var found : Module?
    var inscope no_access : smart_ptr<FileAccess>
    require_module_now(name, no_access, compiling_program().policies) $(var mod : Module?; issues : das_string) {
        found = report_late_require(name, mod, issues)
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
    //! Calls `fn` in `ctx` with a pointer to `result` as its only argument; the target must not
    //! call back into this context - its answer comes back through `result`.
    unsafe {
        invoke_in_context(ctx, fn, addr(result))
    }
}

def call_in_context(ctx : Context?; fn : string; a0 : auto(A0); var result : auto(T)&) {
    //! Calls `fn` in `ctx` with `a0` and a pointer to `result`; the target must not call back
    //! into this context - its answer comes back through `result`.
    unsafe {
        invoke_in_context(ctx, fn, a0, addr(result))
    }
}

def call_in_context(ctx : Context?; fn : string; a0 : auto(A0); a1 : auto(A1); var result : auto(T)&) {
    //! Calls `fn` in `ctx` with `a0`, `a1` and a pointer to `result`; the target must not call
    //! back into this context - its answer comes back through `result`.
    unsafe {
        invoke_in_context(ctx, fn, a0, a1, addr(result))
    }
}

def call_in_context(ctx : Context?; fn : string; a0 : auto(A0); a1 : auto(A1); a2 : auto(A2); var result : auto(T)&) {
    //! Calls `fn` in `ctx` with `a0`, `a1`, `a2` and a pointer to `result`; the target must not
    //! call back into this context - its answer comes back through `result`.
    unsafe {
        invoke_in_context(ctx, fn, a0, a1, a2, addr(result))
    }
}
