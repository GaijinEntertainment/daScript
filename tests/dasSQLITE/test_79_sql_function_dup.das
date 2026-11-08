options gen2

require dastest/testing_boost public
require sqlite/sqlite_boost
require daslib/sql_linq

// Two `[sql_function]`s with the same SQL name and arity. The install loop in
// `_install_sql_function_registry` detects this at `try_open_sqlite` time and
// returns Err — SQLite's silent last-write-wins behavior would otherwise make
// import order load-bearing. Isolated in its own file because the duplicate
// poisons the registry for every subsequent open in the context.

[sql_function(name="dup_collide")]
def dup_a(x : int) : int => x

[sql_function(name="dup_collide")]
def dup_b(x : int) : int => x * 2

[test(description = "two sql_function declarations with one name and arity make try_open_sqlite return an error", standalone = "its duplicate [sql_function] fails every database open in the program")]
def test_duplicate_name_arity_detected(t : T?) {
    let r <- try_open_sqlite(":memory:")
    t |> equal(r |> is_err, true, "duplicate (name, arity) at install time → err")
}
