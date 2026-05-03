// Negative-coverage parity for tutorial 14 — `_having |> _order_by`
// after `_group_by(_.City)` over a struct (User) compiles in `_sql(...)`
// but fails to compile in plain linq with a deep typer error pointing
// at `daslib/builtin.das:999`, far from the user's chain.
//
// Root cause: linq's `order_by` calls `clone_to_move` on its input,
// which requires cloneable elements. Post-`_group_by + _having`, the
// element is `tuple<key, array<User>>`; the nested `array<User>` is not
// generically cloneable, so `_::clone(tuple<...>, ...)` resolution fails.
//
// The bare `_group_by |> _order_by |> _select(...)` chain (without
// `_having`) compiles — linq's expansion must follow a different path
// when there's no post-aggregate filter. The `_having` peeling forces
// a separate sort step that needs cloneable elements.
//
// See API_CHECKED.md F3 for the resolution direction.
expect 30304:1

options gen2

require daslib/linq_boost
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name   : string
    City   : string
    Age    : int
    Salary : int
}

[export]
def main {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "A", City = "NYC", Age = 30, Salary = 100))

    // Mode 3: chain with `_having` + `_order_by` after `_group_by`. The
    // `clone(tuple<string, array<User>>, ...)` resolution fails inside
    // linq's `order_by`. The same shape inside `_sql(...)` lowers to
    // `GROUP BY ... HAVING ... ORDER BY ...` and runs.
    let m3 <- (arr |> _group_by(_.City)
                   |> _having(_._1 |> length >= 1)
                   |> _order_by(_._0)
                   |> _select((City = _._0, N = _._1 |> length)))
}
