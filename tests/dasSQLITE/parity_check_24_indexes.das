options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 24 — indexes.
//
// Indexes are transparent to query semantics — the SQLite planner uses
// them automatically; no chain shape changes. The tutorial's first
// chain (`_where(_.Email == s) |> _first()`) is covered by tut 05.
// The new shape here is `_where + _order_by_descending + _to_array()`
// over an int64 column. The DDL side (`[sql_index]` annotation,
// `_sql_create_indexes_sql`) and unique-constraint write-path are
// out of scope.

[sql_table(name = "Users"),
 sql_index(fields = "Email", unique = true),
 sql_index(fields = ("City", "LastSeen")),
 sql_index(fields = "LastSeen", name = "ix_users_lastseen")]
struct User {
    @sql_primary_key Id : int
    Email    : string
    Name     : string
    City     : string
    LastSeen : int64
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Email = "alice@x.com", Name = "alice", City = "SF",  LastSeen = 100l))
    db |> insert(User(Id = 2, Email = "bob@x.com",   Name = "bob",   City = "SF",  LastSeen = 200l))
    db |> insert(User(Id = 3, Email = "carol@x.com", Name = "carol", City = "NYC", LastSeen = 300l))
}

def fixture_array() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Email = "alice@x.com", Name = "alice", City = "SF",  LastSeen = 100l))
    arr |> emplace(User(Id = 2, Email = "bob@x.com",   Name = "bob",   City = "SF",  LastSeen = 200l))
    arr |> emplace(User(Id = 3, Email = "carol@x.com", Name = "carol", City = "NYC", LastSeen = 300l))
    return <- arr
}

def user_eq(a : User; b : User) : bool {
    return (a.Id == b.Id
        && a.Email == b.Email
        && a.Name == b.Name
        && a.City == b.City
        && a.LastSeen == b.LastSeen)
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
def parity_where_order_desc_to_array(t : T?) {
    // _where(_.City == "SF") narrows to alice + bob; _order_by_descending(_.LastSeen)
    // sorts so bob (200) precedes alice (100). int64 column.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where(_.City == "SF")
                         |> _order_by_descending(_.LastSeen)
                         |> _to_array())
        let m2 <- (db |> select_from(type<User>)
                     |> _where(_.City == "SF")
                     |> _order_by_descending(_.LastSeen)
                     |> _to_array())
        let m3 <- (arr |> _where(_.City == "SF")
                       |> _order_by_descending(_.LastSeen)
                       |> _to_array())

        t |> equal(length(m1), 2, "where|order_desc|to_array: 2 SF users")
        t |> success(users_eq(m1, m2), "where|order_desc|to_array: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "where|order_desc|to_array: M2 ≡ M3")
        // Sanity on order: bob (LastSeen=200) should precede alice (100).
        t |> equal(m1[0].Name, "bob",   "first row: bob (LastSeen=200)")
        t |> equal(m1[1].Name, "alice", "second row: alice (LastSeen=100)")
    }
}
