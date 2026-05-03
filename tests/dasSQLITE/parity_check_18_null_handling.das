options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/option
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 18 — NULL handling via `Option<T>`.
//
// Shapes:
//   1. Round-trip: full Option<T> fields survive Mode-1/2/3 paths
//   2. `_where(_.Col |> is_some)`     → "Col" IS NOT NULL
//   3. `_where(_.Col |> is_none)`     → "Col" IS NULL
//   4. `_where(_.Col |> unwrap_or(d) >= n)`  → COALESCE("Col", ?) >= ?

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name      : string
    @safe_when_uninitialized Age       : Option<int>
    @safe_when_uninitialized Nick      : Option<string>
    @safe_when_uninitialized DeletedAt : Option<int64>
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice", Age = some(30), Nick = none(type<string>), DeletedAt = none(type<int64>)))
    db |> insert(User(Id = 2, Name = "bob",   Age = none(type<int>), Nick = some("bobby"), DeletedAt = none(type<int64>)))
    db |> insert(User(Id = 3, Name = "carol", Age = some(40), Nick = some("c"), DeletedAt = some(1000l)))
}

def fixture_array() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "alice", Age = some(30), Nick = none(type<string>), DeletedAt = none(type<int64>)))
    arr |> emplace(User(Id = 2, Name = "bob",   Age = none(type<int>), Nick = some("bobby"), DeletedAt = none(type<int64>)))
    arr |> emplace(User(Id = 3, Name = "carol", Age = some(40), Nick = some("c"), DeletedAt = some(1000l)))
    return <- arr
}

def opt_int_eq(a : Option<int>; b : Option<int>) : bool {
    if ((a |> is_some) != (b |> is_some)) {
        return false
    }
    if (a |> is_none) {
        return true
    }
    return a |> unwrap == b |> unwrap
}

def opt_str_eq(a : Option<string>; b : Option<string>) : bool {
    if ((a |> is_some) != (b |> is_some)) {
        return false
    }
    if (a |> is_none) {
        return true
    }
    return a |> unwrap == b |> unwrap
}

def opt_i64_eq(a : Option<int64>; b : Option<int64>) : bool {
    if ((a |> is_some) != (b |> is_some)) {
        return false
    }
    if (a |> is_none) {
        return true
    }
    return a |> unwrap == b |> unwrap
}

def user_eq(a : User; b : User) : bool {
    return (a.Id == b.Id
        && a.Name == b.Name
        && opt_int_eq(a.Age, b.Age)
        && opt_str_eq(a.Nick, b.Nick)
        && opt_i64_eq(a.DeletedAt, b.DeletedAt))
}

def users_eq(a : array<User>; b : array<User>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!user_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

[test]
def parity_round_trip(t : T?) {
    // Full row materialization across modes — Option<T> fields survive
    // SQL prep/step + adapter-rail decode in Mode 1/2; trivially in Mode 3.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User>) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<User>) |> _order_by(_.Id))
        let m3 <- (arr |> _order_by(_.Id))

        t |> success(users_eq(m1, m2), "round-trip: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "round-trip: M2 ≡ M3")
    }
}

[test]
def parity_where_is_some(t : T?) {
    // _where(_.Age |> is_some) — keeps alice and carol; drops bob.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User>) |> _where(_.Age |> is_some) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<User>) |> _where(_.Age |> is_some) |> _order_by(_.Id))
        let m3 <- (arr |> _where(_.Age |> is_some) |> _order_by(_.Id))

        t |> equal(length(m1), 2, "is_some: 2 rows (alice, carol)")
        t |> success(users_eq(m1, m2), "is_some: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "is_some: M2 ≡ M3")
    }
}

[test]
def parity_where_is_none(t : T?) {
    // _where(_.DeletedAt |> is_none) — keeps alive users (alice, bob); drops carol.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User>) |> _where(_.DeletedAt |> is_none) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<User>) |> _where(_.DeletedAt |> is_none) |> _order_by(_.Id))
        let m3 <- (arr |> _where(_.DeletedAt |> is_none) |> _order_by(_.Id))

        t |> equal(length(m1), 2, "is_none: 2 rows (alice, bob)")
        t |> success(users_eq(m1, m2), "is_none: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "is_none: M2 ≡ M3")
    }
}

[test]
def parity_where_unwrap_or(t : T?) {
    // _where(_.Age |> unwrap_or(0) >= 18) — alice (30) and carol (40) pass;
    // bob (none → 0) fails. Mode 1: SQL `COALESCE("Age", ?) >= ?`.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()
        let cutoff = 18

        let m1 <- _sql(db |> select_from(type<User>) |> _where(_.Age |> unwrap_or(0) >= cutoff) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<User>) |> _where(_.Age |> unwrap_or(0) >= cutoff) |> _order_by(_.Id))
        let m3 <- (arr |> _where(_.Age |> unwrap_or(0) >= cutoff) |> _order_by(_.Id))

        t |> equal(length(m1), 2, "unwrap_or: 2 rows (alice, carol)")
        t |> success(users_eq(m1, m2), "unwrap_or: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "unwrap_or: M2 ≡ M3")
    }
}
