options gen2

// [sql_function] rejects:
//   1. Struct argument type
//   2. Pointer argument type
//   3. > 4 arguments
//   4. Unsupported return type
//   5. Empty name=
//   6. name= that isn't a valid SQLite identifier
//   7. name= with non-string value (silently coerced default would mask the typo)
//   8. deterministic= with non-bool value
//   9. directonly= with non-bool value
expect 20800:9

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

struct Foo {
    x : int
}

[sql_function]
def fn_struct_arg(f : Foo) : int {
    return f.x
}

[sql_function]
def fn_ptr_arg(p : void?) : int {
    return p == null ? 0 : 1
}

[sql_function]
def fn_five_args(a : int; b : int; c : int; d : int; e : int) : int {
    return a + b + c + d + e
}

[sql_function]
def fn_struct_ret(x : int) : Foo {
    return Foo(x = x)
}

[sql_function(name="")]
def fn_empty_name(x : int) : int {
    return x
}

[sql_function(name="bad name")]
def fn_invalid_name(x : int) : int {
    return x
}

[sql_function(name=42)]
def fn_name_not_string(x : int) : int {
    return x
}

[sql_function(deterministic="true")]
def fn_det_not_bool(x : int) : int {
    return x
}

[sql_function(directonly=1)]
def fn_don_not_bool(x : int) : int {
    return x
}

[export]
def main {
}
