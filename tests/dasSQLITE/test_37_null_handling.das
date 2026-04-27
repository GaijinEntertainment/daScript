options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/option
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Type-driven nullability — `T` columns are NOT NULL, `Option<T>` columns
// allow SQL NULL. The same `User` struct doubles as the row shape and the
// schema; no `@sql_nullable` annotation, no second source of truth.
[sql_table(name="Users")]
struct User {
    @sql_primary_key Id : int
    Name      : string
    @safe_when_uninitialized Age       : Option<int>
    @safe_when_uninitialized Nick      : Option<string>
    @safe_when_uninitialized DeletedAt : Option<int64>
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice", Age = some(30),     Nick = none(type<string>), DeletedAt = none(type<int64>)))
    db |> insert(User(Id = 2, Name = "bob",   Age = none(type<int>), Nick = some("bobby"),    DeletedAt = none(type<int64>)))
    db |> insert(User(Id = 3, Name = "carol", Age = some(40),     Nick = some("c"),         DeletedAt = some(1000l)))
}

// ──────────────────────────────────────────────────────────────────────────
// DDL — Option<T> drops NOT NULL; T keeps it.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_create_table_drops_not_null_for_option(t : T?) {
    t |> equal(_::_sql_create_table_sql(type<User>),
        "CREATE TABLE \"Users\"(\"Id\" INTEGER PRIMARY KEY, \"Name\" TEXT NOT NULL, \"Age\" INTEGER, \"Nick\" TEXT, \"DeletedAt\" INTEGER)",
        "Option<T> columns omit NOT NULL; required T columns keep it")
}

// ──────────────────────────────────────────────────────────────────────────
// INSERT — some(v) binds the underlying value; none() binds SQL NULL.
// SELECT (full row) — readers wrap NULL → none, value → some.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_round_trip_option_columns(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- db |> select_from(type<User>)
        t |> equal(length(rows), 3, "three users")

        // alice — Age=some(30), Nick=none, DeletedAt=none
        t |> equal(rows[0].Name, "alice", "alice row")
        t |> success(rows[0].Age |> is_some, "alice age present")
        t |> equal(rows[0].Age |> unwrap, 30, "alice age value")
        t |> success(rows[0].Nick |> is_none, "alice nick absent")
        t |> success(rows[0].DeletedAt |> is_none, "alice not soft-deleted")

        // bob — Age=none, Nick=some("bobby")
        t |> equal(rows[1].Name, "bob", "bob row")
        t |> success(rows[1].Age |> is_none, "bob age absent")
        t |> success(rows[1].Nick |> is_some, "bob nick present")
        t |> equal(rows[1].Nick |> unwrap, "bobby", "bob nick value")

        // carol — all some
        t |> equal(rows[2].Name, "carol", "carol row")
        t |> equal(rows[2].Age |> unwrap, 40, "carol age")
        t |> equal(rows[2].Nick |> unwrap, "c", "carol nick")
        t |> equal(rows[2].DeletedAt |> unwrap, 1000l, "carol deleted_at")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _where — is_some / is_none translate to IS NOT NULL / IS NULL.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_where_is_some_emits_is_not_null(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Age |> is_some))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Age\", \"Nick\", \"DeletedAt\" FROM \"Users\" WHERE \"Age\" IS NOT NULL",
        "_.Col |> is_some emits Col IS NOT NULL")
}

[test]
def test_where_is_none_emits_is_null(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.DeletedAt |> is_none))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Age\", \"Nick\", \"DeletedAt\" FROM \"Users\" WHERE \"DeletedAt\" IS NULL",
        "_.Col |> is_none emits Col IS NULL")
}

[test]
def test_where_is_some_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where(_.Age |> is_some)
                           |> _order_by(_.Id))
        t |> equal(length(rows), 2, "alice and carol have ages")
        t |> equal(rows[0].Name, "alice", "alice first")
        t |> equal(rows[1].Name, "carol", "carol second")
    }
}

[test]
def test_where_is_none_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where(_.Age |> is_none))
        t |> equal(length(rows), 1, "only bob has no age")
        t |> equal(rows[0].Name, "bob", "bob has no age")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _where — unwrap_or translates to COALESCE(col, ?).
// The default value goes through pred_to_sql, so a captured local becomes
// a bind parameter (`?`), and the surrounding comparison adds another.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_where_unwrap_or_emits_coalesce(t : T?) {
    var _db = SqlRunner()
    let cutoff = 18
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Age |> unwrap_or(0) >= cutoff))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Age\", \"Nick\", \"DeletedAt\" FROM \"Users\" WHERE COALESCE(\"Age\", ?) >= ?",
        "_.Col |> unwrap_or(d) >= x emits COALESCE + comparison")
}

[test]
def test_where_unwrap_or_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // bob has Age=none → COALESCE returns 0 → 0 >= 18 false, bob filtered out.
        let cutoff = 18
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where(_.Age |> unwrap_or(0) >= cutoff)
                           |> _order_by(_.Id))
        t |> equal(length(rows), 2, "alice + carol pass; bob's none → 0 < 18 fails")
        t |> equal(rows[0].Name, "alice", "alice in")
        t |> equal(rows[1].Name, "carol", "carol in")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Projection — Option<T> field passes through into the result tuple.
// The result element type is the table struct (FullRow), which preserves
// the field's `Option<int>` shape. No implicit unwrapping at projection.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_full_row_preserves_option_in_result(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let alive <- _sql(db |> select_from(type<User>)
                            |> _where(_.DeletedAt |> is_none)
                            |> _order_by(_.Id))
        t |> equal(length(alive), 2, "alice + bob alive; carol soft-deleted")
        t |> success(alive[0].DeletedAt |> is_none, "alice DeletedAt none")
        t |> success(alive[1].DeletedAt |> is_none, "bob DeletedAt none")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _try_sql variant — Option methods compose with the non-panicking form.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_try_sql_with_is_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let attempt <- _try_sql(db |> select_from(type<User>)
                                  |> _where(_.Age |> is_some))
        t |> success(attempt |> is_ok, "_try_sql succeeded")
        let rows <- attempt |> unwrap
        t |> equal(length(rows), 2, "two users have known ages")
    }
}
