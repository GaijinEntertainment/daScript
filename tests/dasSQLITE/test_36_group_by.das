options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    City : string
    Age : int
    Salary : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "Alice",   City = "NYC",     Age = 30, Salary = 100))
    db |> insert(User(Id = 2, Name = "Bob",     City = "NYC",     Age = 25, Salary = 80))
    db |> insert(User(Id = 3, Name = "Carol",   City = "Chicago", Age = 40, Salary = 120))
    db |> insert(User(Id = 4, Name = "Dave",    City = "Chicago", Age = 35, Salary = 90))
    db |> insert(User(Id = 5, Name = "Eve",     City = "Chicago", Age = 50, Salary = 200))
    db |> insert(User(Id = 6, Name = "Frank",   City = "Boston",  Age = 28, Salary = 70))
}

// ──────────────────────────────────────────────────────────────────────────
// _group_by + _select using IGrouping shape:
//   _._0                                   → group key column (single-key)
//   _._0._N                                → multi-key Nth column
//   _._1 |> length / |> count              → COUNT(*)
//   _._1 |> select($(u:T) => u.X) |> sum   → SUM("X")
//   _._1 |> select($(u:T) => u.X) |> average → AVG("X")  (result type double)
//   _._1 |> select($(u:T) => u.X) |> min   → MIN("X")
//   _._1 |> select($(u:T) => u.X) |> max   → MAX("X")
//
// Aligns with EF-Core / linq.das `g.Key` / `g.Count()` /
// `g.Average(u => u.Age)` shape — `_._0` corresponds to ``g.Key``,
// ``_._1 |> length`` to ``g.Count()``, ``_._1 |> select(u => u.Age) |> average``
// to ``g.Average(u => u.Age)``.
//
// The inner ``select`` uses a NAMED lambda parameter (``$(u : User)``)
// rather than ``_select(_.Age)`` because the outer ``_select`` lambda
// already binds ``_`` to the group tuple — daslang reports a shadowing
// error when an inner ``_`` lambda has a different type. Naming the inner
// parameter (``u``) sidesteps the conflict.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_group_by_count_emits_group_by(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _group_by(_.City)
                          |> _select((City = _._0, N = _._1 |> length)))
    t |> equal(sql,
        "SELECT \"City\", COUNT(*) FROM \"Users\" GROUP BY \"City\"",
        "_group_by + (_._0, _._1 |> length) emits GROUP BY + COUNT(*)")
}

[test]
def test_group_by_avg_emits_avg_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _group_by(_.City)
                          |> _select((City = _._0, AvgAge = _._1 |> select($(u : User) => u.Age) |> average)))
    t |> equal(sql,
        "SELECT \"City\", AVG(\"Age\") FROM \"Users\" GROUP BY \"City\"",
        "_._1 |> select(u => u.X) |> average emits AVG(col)")
}

[test]
def test_group_by_multi_aggregates(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _group_by(_.City)
                          |> _select((City   = _._0,
                                      N      = _._1 |> length,
                                      MinAge = _._1 |> select($(u : User) => u.Age)    |> min,
                                      MaxAge = _._1 |> select($(u : User) => u.Age)    |> max,
                                      Total  = _._1 |> select($(u : User) => u.Salary) |> sum)))
    t |> equal(sql,
        "SELECT \"City\", COUNT(*), MIN(\"Age\"), MAX(\"Age\"), SUM(\"Salary\") FROM \"Users\" GROUP BY \"City\"",
        "multi-aggregate projection emits all SQL aggregates")
}

[test]
def test_group_by_count_via_count_fn(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _group_by(_.City)
                          |> _select((City = _._0, N = _._1 |> count)))
    t |> equal(sql,
        "SELECT \"City\", COUNT(*) FROM \"Users\" GROUP BY \"City\"",
        "_._1 |> count emits COUNT(*) (alias of |> length)")
}

[test]
def test_group_by_multi_key(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _group_by((_.City, _.Age))
                          |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))
    t |> equal(sql,
        "SELECT \"City\", \"Age\", COUNT(*) FROM \"Users\" GROUP BY \"City\", \"Age\"",
        "multi-column _group_by emits GROUP BY col1, col2; projection uses _._0._N")
}

[test]
def test_group_by_with_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Age > 25)
                          |> _group_by(_.City)
                          |> _select((City = _._0, N = _._1 |> length)))
    t |> equal(sql,
        "SELECT \"City\", COUNT(*) FROM \"Users\" WHERE \"Age\" > ? GROUP BY \"City\"",
        "_where before _group_by emits WHERE before GROUP BY")
}

// ──────────────────────────────────────────────────────────────────────────
// _having — post-aggregate filter. Emits HAVING clause; bind exprs go to
// havingBindExprs and slot in between WHERE and LIMIT in the placeholder
// index sequence.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_having_with_count_threshold(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _group_by(_.City)
                          |> _having(_._1 |> length > 1)
                          |> _select((City = _._0, N = _._1 |> length)))
    t |> equal(sql,
        "SELECT \"City\", COUNT(*) FROM \"Users\" GROUP BY \"City\" HAVING COUNT(*) > ?",
        "_having with COUNT(*) > N emits HAVING")
}

[test]
def test_having_combined_full_query(t : T?) {
    var _db = SqlRunner()
    let min_age = 25
    let min_count = 1
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Age >= min_age)
                          |> _group_by(_.City)
                          |> _having(_._1 |> length >= min_count)
                          |> _order_by(_._0)
                          |> _select((City = _._0,
                                      N = _._1 |> length,
                                      AvgSalary = _._1 |> select($(u : User) => u.Salary) |> average))
                          |> take(10))
    t |> equal(sql,
        "SELECT \"City\", COUNT(*), AVG(\"Salary\") FROM \"Users\" WHERE \"Age\" >= ? GROUP BY \"City\" HAVING COUNT(*) >= ? ORDER BY \"City\" ASC LIMIT ?",
        "full reporting query: WHERE → GROUP BY → HAVING → ORDER BY → LIMIT")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime — actual rows from grouped queries.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_group_by_count_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>)
                           |> _group_by(_.City)
                           |> _order_by(_._0)
                           |> _select((City = _._0, N = _._1 |> length)))
        t |> equal(length(rows), 3, "three cities")
        t |> equal(rows[0].City, "Boston",  "row 0 city")
        t |> equal(rows[0].N,    1,         "Boston: 1 user")
        t |> equal(rows[1].City, "Chicago", "row 1 city")
        t |> equal(rows[1].N,    3,         "Chicago: 3 users")
        t |> equal(rows[2].City, "NYC",     "row 2 city")
        t |> equal(rows[2].N,    2,         "NYC: 2 users")
    }
}

[test]
def test_group_by_avg_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>)
                           |> _group_by(_.City)
                           |> _order_by(_._0)
                           |> _select((City = _._0, AvgSalary = _._1 |> select($(u : User) => u.Salary) |> average)))
        t |> equal(length(rows), 3, "three cities")
        t |> equal(rows[0].City,      "Boston",                          "Boston row")
        t |> equal(rows[0].AvgSalary, 70.0lf,                            "Boston avg salary")
        t |> equal(rows[1].City,      "Chicago",                         "Chicago row")
        t |> equal(rows[1].AvgSalary, double(120 + 90 + 200) / 3.0lf,    "Chicago avg salary")
        t |> equal(rows[2].City,      "NYC",                             "NYC row")
        t |> equal(rows[2].AvgSalary, double(100 + 80) / 2.0lf,          "NYC avg salary")
    }
}

[test]
def test_group_by_having_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Cities with 2+ users — should drop Boston.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _group_by(_.City)
                           |> _having(_._1 |> length > 1)
                           |> _order_by(_._0)
                           |> _select((City = _._0, N = _._1 |> length)))
        t |> equal(length(rows), 2, "Chicago and NYC remain (Boston filtered out)")
        t |> equal(rows[0].City, "Chicago", "Chicago kept")
        t |> equal(rows[0].N,    3,         "Chicago count")
        t |> equal(rows[1].City, "NYC",     "NYC kept")
        t |> equal(rows[1].N,    2,         "NYC count")
    }
}

[test]
def test_group_by_combined_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let min_age = 25
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where(_.Age >= min_age)
                           |> _group_by(_.City)
                           |> _having(_._1 |> length > 1)
                           |> _order_by(_._0)
                           |> _select((City = _._0,
                                       N = _._1 |> length,
                                       MinAge = _._1 |> select($(u : User) => u.Age) |> min,
                                       MaxAge = _._1 |> select($(u : User) => u.Age) |> max)))
        t |> equal(length(rows), 2, "two qualifying cities (Chicago and NYC)")
        t |> equal(rows[0].City,    "Chicago", "Chicago kept")
        t |> equal(rows[0].N,       3,         "Chicago: 3 users >= 25")
        t |> equal(rows[0].MinAge,  35,        "Chicago min age")
        t |> equal(rows[0].MaxAge,  50,        "Chicago max age")
        t |> equal(rows[1].City,    "NYC",     "NYC kept")
        t |> equal(rows[1].N,       2,         "NYC: 2 users >= 25")
        t |> equal(rows[1].MinAge,  25,        "NYC min age")
        t |> equal(rows[1].MaxAge,  30,        "NYC max age")
    }
}

[test]
def test_group_by_multi_key_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        // Add Adam in NYC age 30 (matches Alice's group key) so we can
        // observe a multi-key bucket having two members.
        fixture(db)
        db |> insert(User(Id = 7, Name = "Adam", City = "NYC", Age = 30, Salary = 110))
        let rows <- _sql(db |> select_from(type<User>)
                           |> _group_by((_.City, _.Age))
                           |> _having(_._1 |> length > 1)
                           |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))
        t |> equal(length(rows), 1, "exactly one (City, Age) bucket has 2+ members")
        t |> equal(rows[0].City, "NYC", "NYC bucket")
        t |> equal(rows[0].Age,  30,    "Age 30 bucket")
        t |> equal(rows[0].N,    2,     "Alice + Adam")
    }
}
