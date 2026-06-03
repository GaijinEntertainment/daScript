options gen2

// Regression (Copilot R2, #2979): a pre-join `_where` with a NAMED receiver (`c`) over an UNTRANSLATABLE
// column expression must reject cleanly via the source-arg guard — NOT bind `?` and splice the lambda-local
// `c` into the runtime bind list, which fails to compile with `error[30838]: can't locate variable 'c'`.
//
// Multi-source (join) chains skip arg-name normalization (joins need distinct `c`/`b` names), so the LHS
// `_where` keeps its named param. It runs in the pre-join window (rootAlias="t0", !seenJoin) where the
// join's `into` has not populated argNames yet. The column-ref resolver already maps any field receiver to
// t0 there (the three sites fixed in test_prejoin_where_named.das); `mentions_source_arg` — the fourth site,
// added by the pred_to_sql audit — was missing the same fallback, so the bind guard never fired for `c`.
// The single-source form (`_`) already rejected cleanly; this is the join-LHS analogue.
expect 50503

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

[sql_table(name = "Owners")]
struct Owner {
    @sql_primary_key Id : int
    CarId : int
    Name  : string
}

// Unrecognized by fn_call_to_sql → reaches pred_to_sql's bind fallthrough, where the source-arg guard must
// catch the column reference and reject (instead of binding the whole call as `?`).
def private untranslatable_score(x : int) : int {
    return x * 3 + 1
}

[export]
def main() {
    var db = SqlRunner()
    let bad <- _sql(db |> select_from(type<Car>)
                      |> _where($(c : Car) => untranslatable_score(c.Price) > 100)
                      |> _join(db |> select_from(type<Owner>),
                               $(c : Car, o : Owner) => c.Id == o.CarId,
                               $(c : Car, o : Owner) => (Name = c.Name, Owner = o.Name)))
}
