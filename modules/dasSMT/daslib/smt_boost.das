options gen2

module smt_boost shared

require smt/bindings/z3_func public
require smt/bindings/z3_struct public
require smt/bindings/z3_enum public

require daslib/safe_addr

//! Panics with the Z3 error message when the context carries a pending error.
def public z3_check_error(ctx : Z3_context) {
    let ec = Z3_get_error_code(ctx)
    if (ec != Z3_error_code.Z3_OK) {
        panic("Z3 error {int(ec)}: {Z3_get_error_msg(ctx, ec)}")
    }
}

//! Creates a context in error-code mode. The Z3 default error handler aborts
//! the process; with a null handler errors are polled via `z3_check_error`.
def public z3_create_context : Z3_context {
    var cfg = Z3_mk_config()
    var ctx = Z3_mk_context(cfg)
    Z3_del_config(cfg)
    Z3_set_error_handler(ctx, null)
    return ctx
}

//! Scoped context: creates, invokes the block, deletes.
def public with_context(blk : block<(ctx : Z3_context) : void>) {
    var ctx = z3_create_context()
    invoke(blk, ctx)
    Z3_del_context(ctx)
}

//! Scoped solver with paired inc_ref/dec_ref.
def public with_solver(ctx : Z3_context; blk : block<(s : Z3_solver) : void>) {
    var s = Z3_mk_solver(ctx)
    Z3_solver_inc_ref(ctx, s)
    invoke(blk, s)
    Z3_solver_dec_ref(ctx, s)
}

//! Scoped model of the last `Z3_solver_check` with paired inc_ref/dec_ref.
def public with_model(ctx : Z3_context; s : Z3_solver; blk : block<(m : Z3_model) : void>) {
    var m = Z3_solver_get_model(ctx, s)
    Z3_model_inc_ref(ctx, m)
    invoke(blk, m)
    Z3_model_dec_ref(ctx, m)
}

def public array_data_ptr(var arr : auto(T)[]) : T? => unsafe(addr(arr[0]))

def public array_data_ptr(var arr : array<auto(T)>) : T? {
    if (arr |> empty()) {
        return null
    } else {
        return unsafe(addr(arr[0]))
    }
}

//! `array<Z3_ast>` overloads for the `(unsigned num_args, Z3_ast const args[])` builders.
def public Z3_mk_and(ctx : Z3_context; var args : array<Z3_ast>) =>
    Z3_mk_and(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_and(ctx : Z3_context; var args : Z3_ast[]) =>
    Z3_mk_and(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_or(ctx : Z3_context; var args : array<Z3_ast>) =>
    Z3_mk_or(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_or(ctx : Z3_context; var args : Z3_ast[]) =>
    Z3_mk_or(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_add(ctx : Z3_context; var args : array<Z3_ast>) =>
    Z3_mk_add(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_add(ctx : Z3_context; var args : Z3_ast[]) =>
    Z3_mk_add(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_mul(ctx : Z3_context; var args : array<Z3_ast>) =>
    Z3_mk_mul(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_mul(ctx : Z3_context; var args : Z3_ast[]) =>
    Z3_mk_mul(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_sub(ctx : Z3_context; var args : array<Z3_ast>) =>
    Z3_mk_sub(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_sub(ctx : Z3_context; var args : Z3_ast[]) =>
    Z3_mk_sub(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_distinct(ctx : Z3_context; var args : array<Z3_ast>) =>
    Z3_mk_distinct(ctx, uint(length(args)), array_data_ptr(args))

def public Z3_mk_distinct(ctx : Z3_context; var args : Z3_ast[]) =>
    Z3_mk_distinct(ctx, uint(length(args)), array_data_ptr(args))

//! Integer constant (unknown) named `name`.
def public z3_int_const(ctx : Z3_context; name : string) =>
    Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_int_sort(ctx))

//! Boolean constant (unknown) named `name`.
def public z3_bool_const(ctx : Z3_context; name : string) =>
    Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, name), Z3_mk_bool_sort(ctx))

//! Integer numeral.
def public z3_int(ctx : Z3_context; v : int) => Z3_mk_int(ctx, v, Z3_mk_int_sort(ctx))

//! Version of the loaded libz3.
def public z3_version : string {
    var major, minor, build, rev : uint
    Z3_get_version(safe_addr(major), safe_addr(minor), safe_addr(build), safe_addr(rev))
    return "{int(major)}.{int(minor)}.{int(build)}"
}
