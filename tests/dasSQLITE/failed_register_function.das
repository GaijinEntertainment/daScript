options gen2

// register_function rejects unsupported argument and return types at compile
// time with a per-position diagnostic naming the offender.
//
//   1. struct argument
//   2. struct return
//   3. pointer argument
//   4. lambda passed instead of @@fn (not a function pointer)
//   5. > 4 arguments
expect 50503:5

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

struct Foo {
    x : int
}

def fn_struct_arg(f : Foo) : int {
    return f.x
}

def fn_struct_return(x : int) : Foo {
    return Foo(x = x)
}

def fn_ptr_arg(p : void?) : int {
    return p == null ? 0 : 1
}

def fn_ok_two(a : int; b : int) : int {
    return a + b
}

def fn_five_args(a : int; b : int; c : int; d : int; e : int) : int {
    return a + b + c + d + e
}

[export]
def main {
    var inscope db = open_sqlite(":memory:")
    db |> register_function("a", @@fn_struct_arg)
    db |> register_function("b", @@fn_struct_return)
    db |> register_function("c", @@fn_ptr_arg)
    let lam = @(x : int) => x + 1
    db |> register_function("d", lam)
    db |> register_function("e", @@fn_five_args)
}
