options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

[sql_table(name="Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Active : bool
}

[sql_table(name="Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total = 50))
    // Carol (id 3) has no orders.
}

// ──────────────────────────────────────────────────────────────────────────
// _join — inner equi-join. Emits `INNER JOIN ... ON ...`.
// Multi-source mode forces table-aliased column refs (`"t0"."Col"`).
// ──────────────────────────────────────────────────────────────────────────

[test(description = "join emits INNER JOIN with the key equality as the ON condition and aliased columns")]
def test_inner_join_emits_inner_join_on(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _join(_db |> select_from(type<Order>),
                                   $(u : User, o : Order) => u.Id == o.UserId,
                                   $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
    t |> equal(sql,
        "SELECT \"t0\".\"Name\" AS \"UserName\", \"t1\".\"Total\" FROM \"Users\" AS \"t0\" INNER JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\"",
        "_join emits SELECT aliased cols + INNER JOIN ON aliased keys")
}

[test(description = "a where after a join is emitted as a WHERE over the aliased columns")]
def test_inner_join_with_outer_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Active)
                          |> _join(_db |> select_from(type<Order>),
                                   $(u : User, o : Order) => u.Id == o.UserId,
                                   $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
    t |> equal(sql,
        "SELECT \"t0\".\"Name\" AS \"UserName\", \"t1\".\"Total\" FROM \"Users\" AS \"t0\" INNER JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\" WHERE (\"t0\".\"Active\" <> 0)",
        "_where on left side qualifies as t0.Col in multi-source mode")
}

[test(description = "a where on the joined side is folded into the ON condition")]
def test_inner_join_with_inner_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _join(_db |> select_from(type<Order>) |> _where(_.Total > 75),
                                   $(u : User, o : Order) => u.Id == o.UserId,
                                   $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
    t |> equal(sql,
        "SELECT \"t0\".\"Name\" AS \"UserName\", \"t1\".\"Total\" FROM \"Users\" AS \"t0\" INNER JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\" AND (\"t1\".\"Total\" > ?)",
        "_where inside the right-side chain emits in JOIN's ON clause (qualified t1.Col, NOT in outer WHERE) — required for LEFT JOIN, used uniformly for INNER too")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime
// ──────────────────────────────────────────────────────────────────────────

[test(description = "an inner join returns only the rows that have a match on both sides")]
def test_inner_join_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>)
                           |> _join(db |> select_from(type<Order>),
                                    $(u : User, o : Order) => u.Id == o.UserId,
                                    $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
        // Alice has 2 orders, Bob has 1, Carol 0. Inner join → 3 rows.
        t |> equal(length(rows), 3, "inner join yields 3 rows (Alice×2 + Bob×1, Carol absent)")
        // Total across all join rows: 100 + 250 + 50 = 400
        var sum_total = 0
        for (r in rows) {
            sum_total += r.Total
        }
        t |> equal(sum_total, 400, "Total across joined rows sums correctly")
    }
}

[sql_table(name="Users")]
struct User_39 {
    @sql_primary_key Id : int
    Name : string
}

def private fixture_39(db : SqlRunner) {
    db |> create_table(type<User_39>)
    db |> create_table(type<Order>)
    db |> insert(User_39(Id = 1, Name = "Alice"))
    db |> insert(User_39(Id = 2, Name = "Bob"))
    db |> insert(User_39(Id = 3, Name = "Carol"))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total = 50))
    // Carol (id 3) has NO orders — LEFT JOIN keeps her with right side
    // null.
}

// ──────────────────────────────────────────────────────────────────────────
// _left_join — LEFT OUTER JOIN. Right param of `into` is
// `Option<TB>`; `o |> is_some` / `o |> is_none` probe the join key
// for NULL-ness.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "left_join emits LEFT JOIN with the key equality as the ON condition")]
def test_left_join_emits_left_join_on(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User_39>)
                          |> _left_join(_db |> select_from(type<Order>),
                                        $(u : User_39, o : Order) => u.Id == o.UserId,
                                        $(u : User_39, o : Option<Order>) =>
                                            (Name = u.Name, HasOrder = o |> is_some)))
    t |> equal(sql,
        "SELECT \"t0\".\"Name\", \"t1\".\"UserId\" IS NOT NULL AS \"HasOrder\" FROM \"Users\" AS \"t0\" LEFT JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\"",
        "_left_join + o|>is_some emits LEFT JOIN + IS NOT NULL probe on right key")
}

[test(description = "is_none on a left-joined column emits IS NULL in the select list")]
def test_left_join_is_none_in_select(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User_39>)
                          |> _left_join(_db |> select_from(type<Order>),
                                        $(u : User_39, o : Order) => u.Id == o.UserId,
                                        $(u : User_39, o : Option<Order>) =>
                                            (Name = u.Name, NoOrder = o |> is_none)))
    t |> equal(sql,
        "SELECT \"t0\".\"Name\", \"t1\".\"UserId\" IS NULL AS \"NoOrder\" FROM \"Users\" AS \"t0\" LEFT JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\"",
        "_left_join + o|>is_none emits LEFT JOIN + IS NULL probe")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime — verify that unmatched left rows surface and that
// is_some/is_none flags align with the actual presence of right-side
// rows.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "a left join keeps the left rows that have no match, with their right side absent")]
def test_left_join_runtime_keeps_unmatched(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_39(db)
        let rows <- _sql(db |> select_from(type<User_39>)
                           |> _left_join(db |> select_from(type<Order>),
                                         $(u : User_39, o : Order) => u.Id == o.UserId,
                                         $(u : User_39, o : Option<Order>) =>
                                             (Name = u.Name, HasOrder = o |> is_some)))
        // Alice 2 orders + Bob 1 order + Carol 1 unmatched row = 4
        t |> equal(length(rows), 4, "left join yields 4 rows including unmatched Carol")
        var carol_seen = false
        var carol_has_order = true
        for (r in rows) {
            if (r.Name == "Carol") {
                carol_seen = true
                carol_has_order = r.HasOrder
            }
        }
        t |> success(carol_seen)
        t |> success(!carol_has_order)
    }
}

[test(description = "a where on the right side of a left join is emitted inside the ON condition")]
def test_left_join_with_inner_where_emits_in_on(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User_39>)
                          |> _left_join(_db |> select_from(type<Order>) |> _where(_.Total > 75),
                                        $(u : User_39, o : Order) => u.Id == o.UserId,
                                        $(u : User_39, o : Option<Order>) =>
                                            (Name = u.Name, HasBigOrder = o |> is_some)))
    // Right-side `_where` MUST emit in the ON clause for LEFT JOIN — a
    // row filter in the outer WHERE would silently drop unmatched-NULL
    // rows and turn LEFT JOIN into INNER JOIN.
    t |> equal(sql,
        "SELECT \"t0\".\"Name\", \"t1\".\"UserId\" IS NOT NULL AS \"HasBigOrder\" FROM \"Users\" AS \"t0\" LEFT JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\" AND (\"t1\".\"Total\" > ?)",
        "_left_join + right-side _where emits in JOIN's ON clause (NOT outer WHERE) — required to preserve LEFT JOIN semantics for unmatched left rows")
}

[test(description = "a right-side where on a left join keeps the unmatched left rows instead of dropping them")]
def test_left_join_with_inner_where_preserves_unmatched(t : T?) {
    // The key semantic test: with the right-side `_where` filtering out
    // Bob's only order (Total = 50, doesn't pass `> 75`), Bob's left row
    // must STILL appear in the result with HasBigOrder = false. If the
    // filter were emitted in outer WHERE, Bob would be dropped (the LEFT
    // JOIN's NULL-extended row for Bob would have NULL Total, which fails
    // `Total > 75` and excludes the row — silently turning LEFT into
    // INNER).
    with_sqlite(":memory:") $(db) {
        fixture_39(db)
        // Result tuple shape `<string, bool, int>` is structurally distinct
        // from any other tuple in this file (avoids daslang's typer
        // canonicalizing tuples by arg-type list, which would collide field
        // names with the first matching shape — see
        // feedback_daslang_tuple_recordnames_collide).
        // Filter `> 200` keeps only Alice's Order(Total=250); Bob's
        // Order(Total=50) and Alice's Order(Total=100) are both excluded
        // by the ON-clause predicate.
        let rows <- _sql(db |> select_from(type<User_39>)
                           |> _left_join(db |> select_from(type<Order>) |> _where(_.Total > 200),
                                         $(u : User_39, o : Order) => u.Id == o.UserId,
                                         $(u : User_39, o : Option<Order>) =>
                                             (UName = u.Name, BigOrder = o |> is_some, UId = u.Id)))
        // Alice has 2 orders, only Total=250 passes → 1 matching row.
        // Bob has 1 order (Total=50), 0 pass → kept as 1 NULL-extended row.
        // Carol has 0 orders → kept as 1 NULL-extended row.
        // Total: 3 rows. If the predicate had been emitted in outer WHERE
        // instead of ON, Bob and Carol would have been DROPPED (their
        // NULL-extended Total fails `> 200`), leaving only 1 row — the
        // canonical "LEFT JOIN silently became INNER JOIN" symptom.
        t |> equal(length(rows), 3, "LEFT JOIN + right-side _where keeps unmatched left rows (Bob & Carol both NULL-extended)")
        var bob_seen = false
        var bob_has_big = true
        var carol_seen = false
        for (r in rows) {
            if (r.UName == "Bob") {
                bob_seen = true
                bob_has_big = r.BigOrder
            }
            if (r.UName == "Carol") {
                carol_seen = true
            }
        }
        t |> success(bob_seen)
        t |> success(!bob_has_big)
        t |> success(carol_seen)
    }
}

[test(description = "is_none on a left-joined column is true for the left row with no match")]
def test_left_join_is_none_runtime(t : T?) {
    // Three-column projection so the result tuple shape (string, int,
    // bool) is structurally distinct from the 2-tuple in
    // test_left_join_runtime_keeps_unmatched. daslang's typer
    // canonicalizes structurally-identical tuples (same arg type list)
    // into a single type and keeps the FIRST set of record names —
    // re-using `tuple<string; bool>` here would resolve `r.NoOrder` to
    // the previous test's `HasOrder` field.
    with_sqlite(":memory:") $(db) {
        fixture_39(db)
        let rows <- _sql(db |> select_from(type<User_39>)
                           |> _left_join(db |> select_from(type<Order>),
                                         $(u : User_39, o : Order) => u.Id == o.UserId,
                                         $(u : User_39, o : Option<Order>) =>
                                             (Name = u.Name, UserId = u.Id,
                                              NoOrder = o |> is_none)))
        var carol_no_order = false
        for (r in rows) {
            if (r.Name == "Carol" && r.NoOrder) {
                carol_no_order = true
            }
        }
        t |> success(carol_no_order)
    }
}
