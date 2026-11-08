options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq
require daslib/option
require daslib/result

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla",   Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",    Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo",   Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// _first — single row terminal, panics on empty (LIMIT 1)
// ──────────────────────────────────────────────────────────────────────────

[test(description = "the first terminal returns the first row of the table with every column filled")]
def test_first_returns_first_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let car = _sql(db |> select_from(type<Car>) |> _first())
        t |> equal(car.Id,    1,     "row 0 Id")
        t |> equal(car.Name,  "BMW", "row 0 Name")
        t |> equal(car.Price, 100,   "row 0 Price")
    }
}

[test(description = "the first terminal after a where returns the first matching row")]
def test_first_with_where_filters(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let target = 3
        let car = _sql(db |> select_from(type<Car>)
                         |> _where(_.Id == target)
                         |> _first())
        t |> equal(car.Id,    3,        "filtered row Id")
        t |> equal(car.Name,  "Tesla",  "filtered row Name")
    }
}

[test(description = "the first terminal compiles to a SELECT with LIMIT 1")]
def test_first_emits_limit_1(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _first())
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT 1",
        "_first() emits ` LIMIT 1` suffix")
}

[test(description = "the first terminal after a where compiles to WHERE followed by LIMIT 1")]
def test_first_with_where_emits_limit_1(t : T?) {
    var _db = SqlRunner()
    let _target = 3
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Id == _target)
                          |> _first())
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Id\" = ? LIMIT 1",
        "_where + _first emit WHERE then ` LIMIT 1`")
}

[test(description = "a select projection followed by first returns just the first projected value")]
def test_first_with_select_and_first(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let name = _sql(db |> select_from(type<Car>)
                          |> _select(_.Name)
                          |> _first())
        t |> equal(name, "BMW", "first projected name")
    }
}

def private fixture_09(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
}

// ──────────────────────────────────────────────────────────────────────────
// _first_opt — Option<T>, none on empty (LIMIT 1)
// ──────────────────────────────────────────────────────────────────────────

[test(description = "first_opt returns some of the first row when the table has rows")]
def test_first_opt_returns_some_when_present(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_09(db)
        let res = _sql(db |> select_from(type<Car>) |> _first_opt())
        t |> success(res |> is_some, "non-empty table -> some(...)")
        let car = res |> unwrap
        t |> equal(car.Id, 1, "first row Id")
    }
}

[test(description = "first_opt on an empty table returns none instead of panicking")]
def test_first_opt_returns_none_when_empty(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let res = _sql(db |> select_from(type<Car>) |> _first_opt())
        t |> success(res |> is_none, "empty table -> none")
    }
}

[test(description = "first_opt with a where that matches nothing returns none")]
def test_first_opt_returns_none_with_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_09(db)
        let target = 999
        let res = _sql(db |> select_from(type<Car>)
                         |> _where(_.Id == target)
                         |> _first_opt())
        t |> success(res |> is_none, "no matching row -> none")
    }
}

[test(description = "the first_opt terminal compiles to a SELECT with LIMIT 1")]
def test_first_opt_emits_limit_1(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _first_opt())
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT 1",
        "_first_opt() emits ` LIMIT 1` suffix")
}

def private fixture_10(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla",   Price = 300))
}

// ──────────────────────────────────────────────────────────────────────────
// query_one — single-arg positional bind
// ──────────────────────────────────────────────────────────────────────────

[test(description = "query_one with one positional argument binds it and returns the matching row")]
def test_query_one_single_arg_returns_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_10(db)
        let car = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 2)
        t |> equal(car.Id,    2,      "filtered Id")
        t |> equal(car.Name,  "Audi", "filtered Name")
        t |> equal(car.Price, 200,    "filtered Price")
    }
}

[test(description = "query_one with no arguments returns the first row")]
def test_query_one_zero_arg_returns_first_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_10(db)
        let car = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars LIMIT 1",
            type<Car>)
        t |> equal(car.Id, 1, "first row Id")
    }
}

[test(description = "try_query_one returns Ok carrying the matching row")]
def test_try_query_one_ok_on_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_10(db)
        let res = db |> try_query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 3)
        t |> success(res |> is_ok, "match returns ok")
        let car = res |> unwrap
        t |> equal(car.Name, "Tesla", "ok value")
    }
}

[test(description = "try_query_one returns an error when no row matches")]
def test_try_query_one_err_on_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_10(db)
        let res = db |> try_query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 999)
        t |> success(res |> is_err, "no rows -> err")
    }
}

def private fixture_11(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla",   Price = 300))
    db |> insert(Car(Id = 4, Name = "Tesla",   Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// query_one — 2/3-arg positional bind (mixed types)
// ──────────────────────────────────────────────────────────────────────────

[test(description = "query_one binds two positional arguments of mixed types")]
def test_query_one_two_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_11(db)
        let car = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Name = ? AND Price = ?",
            type<Car>, "Tesla", 999)
        t |> equal(car.Id,    4,       "matched Id")
        t |> equal(car.Name,  "Tesla", "matched Name")
        t |> equal(car.Price, 999,     "matched Price")
    }
}

[test(description = "query_one binds three positional arguments and returns the matching row")]
def test_query_one_three_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_11(db)
        // 3-arg form: id-range + name-prefix-like
        let car = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id >= ? AND Id <= ? AND Name = ?",
            type<Car>, 1, 5, "Audi")
        t |> equal(car.Id,    2,      "matched Id")
        t |> equal(car.Name,  "Audi", "matched Name")
    }
}

[test(description = "try_query_one with two arguments returns an error when no row matches")]
def test_try_query_one_two_args_err(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_11(db)
        let res = db |> try_query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Name = ? AND Price = ?",
            type<Car>, "Tesla", 1)
        t |> success(res |> is_err, "no row matches Tesla+1")
    }
}

def private fixture_13(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",  Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi", Price = 200))
}

// ──────────────────────────────────────────────────────────────────────────
// query_one_opt — 0-row case returns none, no panic
// ──────────────────────────────────────────────────────────────────────────

[test(description = "query_one_opt returns some of the matching row")]
def test_query_one_opt_some_on_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_13(db)
        let res = db |> query_one_opt(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 1)
        t |> success(res |> is_some, "match -> some(...)")
        let car = res |> unwrap
        t |> equal(car.Name, "BMW", "some value")
    }
}

[test(description = "query_one_opt returns none when no row matches")]
def test_query_one_opt_none_on_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_13(db)
        let res = db |> query_one_opt(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, 999)
        t |> success(res |> is_none, "no match -> none")
    }
}

[test(description = "query_one_opt with no arguments returns some of the first row of a non-empty table")]
def test_query_one_opt_zero_arg(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_13(db)
        let res = db |> query_one_opt(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars LIMIT 1",
            type<Car>)
        t |> success(res |> is_some, "non-empty -> some(first)")
    }
}

[test(description = "query_one_opt with no arguments returns none on an empty table")]
def test_query_one_opt_zero_arg_empty(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let res = db |> query_one_opt(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars LIMIT 1",
            type<Car>)
        t |> success(res |> is_none, "empty -> none")
    }
}
