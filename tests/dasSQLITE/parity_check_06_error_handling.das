options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require daslib/option
require daslib/result
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 06 — `_try_sql(chain)` is `_sql(chain)` wrapped
// in `Result<T, string>` for SQL-side prepare/step failures. The INNER chain
// (`_first()`, `_first_opt()`) is the same linq we verified in tut 05; the
// new piece is that `_try_sql(...).unwrap()` on the ok path must agree with
// the Mode-2 / Mode-3 results.
//
// On the err path (e.g. `_first()` on an empty table), Modes 2 and 3 PANIC —
// no parity assertion is possible there; the file only confirms Mode 1
// returns Err as a sanity check.
//
// Raw-SQL helpers (`try_open_sqlite`, `try_insert`, `query_scalar_opt`,
// `try_exec`) are out of scope per API_CHECKED.md — non-chain code paths.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice"))
    db |> insert(User(Id = 2, Name = "bob"))
}

def fixture_array() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "alice"))
    arr |> emplace(User(Id = 2, Name = "bob"))
    return <- arr
}

def user_eq(a : User; b : User) : bool {
    return a.Id == b.Id && a.Name == b.Name
}

[test]
def parity_try_sql_first_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        // Mode 1: _try_sql(...) returns Result<User, string>; ok-path here.
        let r1 = _try_sql(db |> select_from(type<User>) |> _first())
        // Mode 2 / Mode 3: same inner chain, no error wrapping.
        let m2 = db |> select_from(type<User>) |> _first()
        let m3 = arr |> _first()

        t |> success(r1 |> is_ok, "try_sql(_first): ok on populated table")
        t |> success(user_eq(r1 |> unwrap, m2), "try_sql(_first).unwrap() ≡ Mode 2")
        t |> success(user_eq(m2, m3),            "Mode 2 ≡ Mode 3")
    }
}

[test]
def parity_try_sql_first_opt_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        // Mode 1: _try_sql(... |> _first_opt()) → Result<Option<User>, string>.
        let r1 = _try_sql(db |> select_from(type<User>) |> _first_opt())
        let m2 = db |> select_from(type<User>) |> _first_opt()
        let m3 = arr |> _first_opt()

        t |> success(r1 |> is_ok, "try_sql(_first_opt): ok on populated table")
        let o1 = r1 |> unwrap
        t |> success(o1 |> is_some && m2 |> is_some && m3 |> is_some,
            "all three: some on populated table")
        t |> success(user_eq(o1 |> unwrap, m2 |> unwrap), "try_sql(_first_opt).unwrap().unwrap() ≡ Mode 2")
        t |> success(user_eq(m2 |> unwrap, m3 |> unwrap), "Mode 2 ≡ Mode 3")
    }
}

[test]
def parity_try_sql_first_opt_none(t : T?) {
    with_sqlite(":memory:") $(db) {
        // Empty table — _first_opt returns none across all three modes.
        db |> create_table(type<User>)
        var arr : array<User>

        let r1 = _try_sql(db |> select_from(type<User>) |> _first_opt())
        let m2 = db |> select_from(type<User>) |> _first_opt()
        let m3 = arr |> _first_opt()

        t |> success(r1 |> is_ok, "try_sql(_first_opt): ok on empty (0 rows is not an error)")
        t |> success((r1 |> unwrap) |> is_none, "Mode 1: none on empty")
        t |> success(m2 |> is_none,             "Mode 2: none on empty")
        t |> success(m3 |> is_none,             "Mode 3: none on empty")
    }
}

[test]
def sanity_try_sql_first_empty_errs(t : T?) {
    // Sanity check (Mode 1 only): _try_sql(... |> _first()) on empty table
    // returns Err. Modes 2 and 3 PANIC on empty `_first()` — no parity
    // assertion is possible (a panic would terminate the test process).
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        let r1 = _try_sql(db |> select_from(type<User>) |> _first())
        t |> success(r1 |> is_err, "try_sql(_first) on empty table → Err (Mode 1 only; M2/M3 would panic)")
    }
}
