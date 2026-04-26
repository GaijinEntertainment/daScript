options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require daslib/option
require daslib/result

// Tutorial 06 — Error handling.
//
// Naming convention throughout dasSQLITE:
//   foo(...)      — strict; panics on error (programmer-error default)
//   try_foo(...)  — Result<T, string>; for write-side errors you want to
//                   handle (constraint violations, prepare failures, etc.)
//   foo_opt(...)  — Option<T>; for read-side empty results (0 rows is a
//                   normal outcome, not an error)
//
// On the LINQ side:
//   _sql(chain)      — strict; panics on prepare/step failure
//   _try_sql(chain)  — Result<T, string>; non-panic sibling, same chain
//                      analyzer

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

[export]
def main() {
    // try_open_sqlite: Result<SqlRunner, string> — use when the path is
    // user-supplied or otherwise unreliable. The strict open_sqlite /
    // with_sqlite forms panic on the same failure.
    var open_result <- try_open_sqlite(":memory:")
    if (open_result |> is_err) {
        to_log(LOG_ERROR, "could not open db: {open_result |> unwrap_err}\n")
        return
    }
    // Move the SqlRunner out of the Result. SqlRunner has finalize-on-scope-
    // exit (closes the SQLite handle), so it must be `var inscope` and bound
    // by move (`<-`); plain `unwrap` would fail because the type isn't copyable.
    var inscope db <- open_result._value
    db |> create_table(type<User>)

    // ── try_insert: Result<int64, string> ────────────────────────────────
    let first = db |> try_insert(User(Id = 1, Name = "alice"))
    if (first |> is_ok) {
        to_log(LOG_INFO, "inserted rowid={first |> unwrap}\n")
    }
    // Explicit collision — strict insert() would abort here. try_insert
    // returns Err and execution continues.
    let dup = db |> try_insert(User(Id = 1, Name = "alice-dup"))
    if (dup |> is_err) {
        to_log(LOG_INFO, "duplicate rejected as expected: {dup |> unwrap_err}\n")
    }

    // ── query_scalar_opt: Option<T> for "0 rows is fine" ────────────────
    let lookup = db |> query_scalar_opt(
        "SELECT \"Name\" FROM \"Users\" WHERE \"Id\" = 42", type<string>)
    to_log(LOG_INFO, "user 42 is {lookup ?? "unknown"}\n")

    // ── _try_sql(chain) — non-panicking _sql sibling ─────────────────────
    // Returns the SAME shape as _sql but wrapped in Result<..., string>.
    // _try_sql(... |> _first())     returns  Result<T, string>
    // _try_sql(... |> _first_opt()) returns  Result<Option<T>, string>
    // _try_sql(... |> count())      returns  Result<int64, string>
    let res = _try_sql(db |> select_from(type<User>) |> _first())
    if (res |> is_ok) {
        let u = res |> unwrap
        to_log(LOG_INFO, "first user: {u.Name}\n")
    }

    // _try_sql with _first on empty table -> Err (0 rows is an error
    // for _first; for non-error empty handling, use _first_opt).
    let drop_err = db |> try_exec("DELETE FROM \"Users\"")
    if (drop_err |> is_some) {
        to_log(LOG_ERROR, "delete failed: {drop_err |> unwrap}\n")
    }
    let res2 = _try_sql(db |> select_from(type<User>) |> _first())
    if (res2 |> is_err) {
        to_log(LOG_INFO, "after delete, _first errs as expected: {res2 |> unwrap_err}\n")
    }

    // _first_opt under _try_sql: 0 rows is not an error, returns ok(none).
    let res3 = _try_sql(db |> select_from(type<User>) |> _first_opt())
    if (res3 |> is_ok) {
        let opt = res3 |> unwrap
        to_log(LOG_INFO, "after delete, _first_opt is none: {opt |> is_none}\n")
    }
}
