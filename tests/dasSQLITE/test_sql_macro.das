options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq
require daslib/linq
require daslib/linq_boost
require _sql_composability_macros
require daslib/result
require daslib/option

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
// _sql — bare select_from
// ──────────────────────────────────────────────────────────────────────────

[test(description = "a bare select_from inside _sql on an empty table returns an empty array")]
def test_sql_bare_select_from_empty_table(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let cars <- _sql(db |> select_from(type<Car>))
        t |> equal(length(cars), 0, "empty table -> empty array")
    }
}

[test(description = "a bare select_from inside _sql returns every row with every column filled")]
def test_sql_bare_select_from_returns_all_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>))
        t |> equal(length(cars), 5, "5 rows expected")
        t |> equal(cars[0].Id,    1,        "row 0 Id")
        t |> equal(cars[0].Name,  "BMW",    "row 0 Name")
        t |> equal(cars[0].Price, 100,      "row 0 Price")
        t |> equal(cars[4].Id,    5,        "row 4 Id")
        t |> equal(cars[4].Name,  "Lambo",  "row 4 Name")
        t |> equal(cars[4].Price, 999,      "row 4 Price")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _to_array — explicit terminal (no-op semantically)
// ──────────────────────────────────────────────────────────────────────────

[test(description = "an explicit _to_array terminal returns the same rows as the bare chain")]
def test_sql_to_array_explicit_terminal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _to_array())
        t |> equal(length(cars), 5, "_to_array() yields the same array")
        t |> equal(cars[0].Name, "BMW", "row 0 Name preserved")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _select — single-column projection
// ──────────────────────────────────────────────────────────────────────────

[test(description = "_select of a string column returns one string per row")]
def test_sql_select_string_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- _sql(db |> select_from(type<Car>) |> _select(_.Name))
        t |> equal(length(names), 5, "5 names expected")
        t |> equal(names[0], "BMW",   "name 0")
        t |> equal(names[2], "Tesla", "name 2")
        t |> equal(names[4], "Lambo", "name 4")
    }
}

[test(description = "_select of an int column returns one int per row")]
def test_sql_select_int_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let prices <- _sql(db |> select_from(type<Car>) |> _select(_.Price))
        t |> equal(length(prices), 5, "5 prices expected")
        t |> equal(prices[0], 100, "price 0")
        t |> equal(prices[3], 50,  "price 3")
        t |> equal(prices[4], 999, "price 4")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _where — predicate filter with bind capture
// ──────────────────────────────────────────────────────────────────────────

[test(description = "_where with an integer literal equality returns only the matching row")]
def test_sql_where_eq_literal_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == 3))
        t |> equal(length(cars), 1, "one row matches Id=3")
        t |> equal(cars[0].Name, "Tesla", "matched Tesla")
    }
}

[test(description = "_where with a string literal equality returns only the matching row")]
def test_sql_where_eq_literal_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Name == "Audi"))
        t |> equal(length(cars), 1, "one row matches Name=Audi")
        t |> equal(cars[0].Id, 2, "matched Id 2")
    }
}

[test(description = "_where compares a column against a captured local variable")]
def test_sql_where_captured_var(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let target = 4
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == target))
        t |> equal(length(cars), 1, "one row matches captured target")
        t |> equal(cars[0].Name, "Lada", "matched Lada")
    }
}

struct private Cfg {
    target : int
}

def private compute_target() : int => 4

[test(description = "_where compares a column against a struct field read at runtime")]
def test_sql_where_struct_field_bind(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cfg = Cfg(target = 4)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == cfg.target))
        t |> equal(length(cars), 1, "one row matches struct-field target")
        t |> equal(cars[0].Name, "Lada", "matched Lada")
    }
}

[test(description = "_where compares a column against the result of a function call")]
def test_sql_where_fn_call_bind(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == compute_target()))
        t |> equal(length(cars), 1, "one row matches fn-call target")
        t |> equal(cars[0].Name, "Lada", "matched Lada")
    }
}

[test(description = "_where with a greater-than literal returns the rows above the bound")]
def test_sql_where_gt_literal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Price > 200))
        t |> equal(length(cars), 2, "two rows priced > 200 (Tesla, Lambo)")
        t |> equal(cars[0].Name, "Tesla", "row 0 Tesla")
        t |> equal(cars[1].Name, "Lambo", "row 1 Lambo")
    }
}

[test(description = "_where with a less-than literal returns the rows below the bound")]
def test_sql_where_lt_literal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Price < 100))
        t |> equal(length(cars), 1, "one row priced < 100 (Lada)")
        t |> equal(cars[0].Name, "Lada", "matched Lada")
    }
}

[test(description = "_where with not-equal excludes exactly the matching row")]
def test_sql_where_neq(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id != 3))
        t |> equal(length(cars), 4, "four rows have Id != 3")
    }
}

[test(description = "_where joins two captured bounds with and")]
def test_sql_where_and_with_two_captures(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let lo = 100
        let hi = 300
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Price >= lo && _.Price <= hi))
        t |> equal(length(cars), 3, "BMW, Audi, Tesla in [100, 300]")
        t |> equal(cars[0].Name, "BMW",   "row 0")
        t |> equal(cars[1].Name, "Audi",  "row 1")
        t |> equal(cars[2].Name, "Tesla", "row 2")
    }
}

[test(description = "_where joins two equalities with or")]
def test_sql_where_or(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == 1 || _.Id == 5))
        t |> equal(length(cars), 2, "two rows: Id=1 OR Id=5")
        t |> equal(cars[0].Name, "BMW",   "row 0")
        t |> equal(cars[1].Name, "Lambo", "row 1")
    }
}

[test(description = "_where negates a predicate with not")]
def test_sql_where_not(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(!(_.Id == 3)))
        t |> equal(length(cars), 4, "four rows have NOT Id=3")
    }
}

[test(description = "_where followed by _select filters first and projects the survivors")]
def test_sql_where_then_select_compound(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- _sql(db |> select_from(type<Car>) |> _where(_.Price >= 200) |> _select(_.Name))
        t |> equal(length(names), 3, "three names priced >= 200")
        t |> equal(names[0], "Audi",  "row 0")
        t |> equal(names[1], "Tesla", "row 1")
        t |> equal(names[2], "Lambo", "row 2")
    }
}

[test(description = "two chained _where calls combine as an and")]
def test_sql_two_where_calls_compose_as_and(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Price >= 100) |> _where(_.Price <= 300))
        t |> equal(length(cars), 3, "two _where calls compose AND-wise: BMW, Audi, Tesla")
    }
}

[test(description = "_to_array as the terminal of a filtered chain returns the filtered rows")]
def test_sql_with_to_array_terminal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> _where(_.Id == 2) |> _to_array())
        t |> equal(length(cars), 1, "_to_array as terminal yields a row")
        t |> equal(cars[0].Name, "Audi", "matched Audi")
    }
}

def private fixture_05a(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// Compatibility mode: select_from used WITHOUT the _sql macro wrapper.
//
// `select_from(db, type<T>) : array<T>` is a regular runtime function. It
// prepares `SELECT col1, col2, ... FROM Table`, steps the cursor, and
// materializes every row via the chunk-1 `[sql_table]`-emitted
// `_sql_read_row` helper + `read_via_adapter` dispatch rail. Anything chained on
// top in compat mode is plain daslib/linq running client-side over the
// already-materialized array — no SQL push-down. `_sql` is the optimization
// path; `select_from` standalone is the always-works baseline.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "select_from without the _sql macro materializes every row into an array")]
def test_compat_select_from_bare_returns_array(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_05a(db)
        let cars <- db |> select_from(type<Car>)
        t |> equal(length(cars), 5,        "5 rows materialized")
        t |> equal(cars[0].Name, "BMW",    "row 0 Name")
        t |> equal(cars[2].Name, "Tesla",  "row 2 Name")
        t |> equal(cars[4].Price, 999,     "row 4 Price")
    }
}

[test(description = "select_from without the _sql macro returns an empty array for an empty table")]
def test_compat_select_from_empty_table(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let cars <- db |> select_from(type<Car>)
        t |> equal(length(cars), 0, "empty table -> empty array")
    }
}

[test(description = "select_from without the _sql macro can be iterated with a for loop over all rows")]
def test_compat_select_from_for_loop(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_05a(db)
        var seen_ids : array<int>
        seen_ids |> reserve(5)
        for (car in db |> select_from(type<Car>)) {
            seen_ids |> push(car.Id)
        }
        t |> equal(length(seen_ids), 5, "for loop iterates all rows")
        t |> equal(seen_ids[0], 1, "first Id")
        t |> equal(seen_ids[4], 5, "last Id")
    }
}

[test(description = "a linq where after a plain select_from filters client-side over the materialized array")]
def test_compat_select_from_then_linq_where(t : T?) {
    // No _sql wrapper: select_from materializes the whole table, then
    // daslib/linq_boost's _where filters client-side over the array via
    // each() iterator wrap.
    with_sqlite(":memory:") $(db) {
        fixture_05a(db)
        let cars <- db |> select_from(type<Car>)
        let pricey <- unsafe(each(cars)) |> _where_to_array(_.Price > 200)
        t |> equal(length(pricey), 2, "linq _where over materialized array")
        t |> equal(pricey[0].Name, "Tesla", "row 0")
        t |> equal(pricey[1].Name, "Lambo", "row 1")
    }
}

[test(description = "a linq select after a plain select_from projects client-side over the materialized array")]
def test_compat_select_from_then_linq_select(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_05a(db)
        let cars <- db |> select_from(type<Car>)
        let names <- unsafe(each(cars)) |> _select_to_array(_.Name)
        t |> equal(length(names), 5, "linq _select over materialized array")
        t |> equal(names[0], "BMW",   "name 0")
        t |> equal(names[4], "Lambo", "name 4")
    }
}

[test(description = "_to_array after a plain select_from is a passthrough that keeps the rows")]
def test_compat_select_from_chain_with_to_array_terminal(t : T?) {
    // sql_linq's _to_array is a regular passthrough function in compat
    // mode — it just returns the array.
    with_sqlite(":memory:") $(db) {
        fixture_05a(db)
        let cars <- db |> select_from(type<Car>) |> _to_array()
        t |> equal(length(cars), 5, "_to_array() in compat mode is a passthrough")
        t |> equal(cars[2].Name, "Tesla", "row 2 preserved")
    }
}

[test(description = "try_select_from returns Ok with the rows of an existing table")]
def test_compat_try_select_from_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_05a(db)
        let r <- db |> try_select_from(type<Car>)
        t |> success(r |> is_ok, "try_select_from returns Ok on a valid table")
        let cars <- r |> unwrap
        t |> equal(length(cars), 5, "5 rows via try_ form")
    }
}

[test(description = "try_select_from on a missing table returns the SQLite error instead of panicking")]
def test_compat_try_select_from_propagates_error(t : T?) {
    // Calling select_from on a Car table that doesn't exist surfaces the
    // libsqlite3 errmsg through Result rather than panicking.
    with_sqlite(":memory:") $(db) {
        // Note: we did NOT create the Cars table, so the SELECT must fail.
        let r <- db |> try_select_from(type<Car>)
        t |> success(r |> is_err, "try_select_from returns Err on missing table")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// _sql_text — compile-time SQL emission. The chain expression is never run;
// the macro inspects the AST and returns the SQL string that _sql() would
// have emitted. Whitespace is canonicalized (single spaces, no trailing).
//
// The dummy `var _db = SqlRunner()` is unused at runtime — _sql_text never
// executes the chain — so each test's binding-targets (`_db`, `_target`,
// `_lo`, `_hi`) carry an `_` prefix to suppress the LINT002 unused-var
// warning. The captured-var tests still exercise the bind-emission path
// because the macro analyzer reads the AST regardless of runtime use.
// ──────────────────────────────────────────────────────────────────────────

// ── bare select_from ──────────────────────────────────────────────────────

[test(description = "_sql_text of a bare select_from emits SELECT with every column and the table name")]
def test_sql_text_bare_select_from(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>))
    t |> equal(sql, "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\"", "bare select_from emits SELECT * with all columns and table name")
}

[test(description = "_sql_text takes the FROM table from the sql_table name, not the struct name")]
def test_sql_text_uses_sql_table_name_override(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>))
    t |> success(sql |> find("FROM \"Cars\"") >= 0, "FROM uses [sql_table(name=)] override, not struct name")
}

// ── _to_array — no-op terminal ────────────────────────────────────────────

[test(description = "_to_array does not change the SQL _sql_text emits")]
def test_sql_text_to_array_is_noop(t : T?) {
    var _db = SqlRunner()
    let with_to    = _sql_text(_db |> select_from(type<Car>) |> _to_array())
    let without_to = _sql_text(_db |> select_from(type<Car>))
    t |> equal(with_to, without_to, "_to_array() does not change emitted SQL")
}

// ── _select — single-column projection ───────────────────────────────────

[test(description = "_select of one field emits a single-column SELECT")]
def test_sql_text_select_single_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Name))
    t |> equal(sql, "SELECT \"Name\" FROM \"Cars\"", "_select(_.Field) projects single column")
}

[test(description = "_select of an int field emits a single-column SELECT")]
def test_sql_text_select_int_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price))
    t |> equal(sql, "SELECT \"Price\" FROM \"Cars\"", "_select(_.IntField) projects int column")
}

// ── _where — filter clause ───────────────────────────────────────────────

[test(description = "an equality in _where emits WHERE with a placeholder")]
def test_sql_text_where_eq_literal(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 5))
    t |> equal(sql, "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Id\" = ?", "WHERE eq literal: ?")
}

[test(description = "not-equal in _where emits the SQL not-equal operator")]
def test_sql_text_where_neq(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id != 5))
    t |> success(sql |> find("WHERE \"Id\" <> ?") >= 0, "WHERE != becomes <>")
}

[test(description = "less-than in _where emits the SQL less-than operator")]
def test_sql_text_where_lt(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price < 200))
    t |> success(sql |> find("WHERE \"Price\" < ?") >= 0, "WHERE < emits <")
}

[test(description = "less-or-equal in _where emits the SQL less-or-equal operator")]
def test_sql_text_where_le(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price <= 200))
    t |> success(sql |> find("WHERE \"Price\" <= ?") >= 0, "WHERE <= emits <=")
}

[test(description = "greater-than in _where emits the SQL greater-than operator")]
def test_sql_text_where_gt(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price > 200))
    t |> success(sql |> find("WHERE \"Price\" > ?") >= 0, "WHERE > emits >")
}

[test(description = "greater-or-equal in _where emits the SQL greater-or-equal operator")]
def test_sql_text_where_ge(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price >= 200))
    t |> success(sql |> find("WHERE \"Price\" >= ?") >= 0, "WHERE >= emits >=")
}

[test(description = "a string literal in _where is emitted as a placeholder like any other value")]
def test_sql_text_where_string_literal(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Name == "BMW"))
    t |> success(sql |> find("WHERE \"Name\" = ?") >= 0, "WHERE eq string literal also gets ? placeholder")
}

[test(description = "a captured variable in _where is emitted as a placeholder")]
def test_sql_text_where_captured_var(t : T?) {
    var _db = SqlRunner()
    let _target = 3
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == _target))
    t |> success(sql |> find("WHERE \"Id\" = ?") >= 0, "captured var also yields ? placeholder")
}

[test(description = "a captured variable and an inline literal emit identical SQL")]
def test_sql_text_where_captured_and_literal_match(t : T?) {
    // Captured-var literal vs. inline literal produce identical SQL — proves the
    // macro emits placeholders, not the literal value.
    var _db = SqlRunner()
    let _target = 5
    let with_var     = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == _target))
    let with_literal = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 5))
    t |> equal(with_var, with_literal, "captured var and inline literal produce identical SQL (both ?)")
}

[test(description = "and in _where emits an AND of both conjuncts")]
def test_sql_text_where_and(t : T?) {
    var _db = SqlRunner()
    let _lo = 100
    let _hi = 500
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price >= _lo && _.Price <= _hi))
    t |> success(sql |> find("AND") >= 0, "AND appears in compound WHERE")
    t |> success(sql |> find("\"Price\" >= ?") >= 0, "first conjunct present")
    t |> success(sql |> find("\"Price\" <= ?") >= 0, "second conjunct present")
}

[test(description = "or in _where emits an OR")]
def test_sql_text_where_or(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 1 || _.Id == 2))
    t |> success(sql |> find("OR") >= 0, "OR appears in compound WHERE")
}

[test(description = "not in _where emits NOT")]
def test_sql_text_where_not(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(!(_.Id == 5)))
    t |> success(sql |> find("NOT") >= 0, "NOT appears for ! op")
}

// ── compound: _where + _select ────────────────────────────────────────────

[test(description = "_where and _select compose into one SELECT with a WHERE")]
def test_sql_text_where_then_select(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price > 100) |> _select(_.Name))
    t |> equal(sql, "SELECT \"Name\" FROM \"Cars\" WHERE \"Price\" > ?", "_where + _select compose")
}

[test(description = "_to_array stays a no-op when chained after _where")]
def test_sql_text_where_with_to_array(t : T?) {
    var _db = SqlRunner()
    let with_to    = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 1) |> _to_array())
    let without_to = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 1))
    t |> equal(with_to, without_to, "_to_array still no-op when chained after _where")
}

// ──────────────────────────────────────────────────────────────────────────
// Composability tests for `_sql`.
//
// Mode 2's promise: user-defined call_macros that expand to `_where` /
// `_select` compose transparently inside `_sql(...)`. The macro pass
// expands inner call_macros bottom-up before `_sql.visit` fires, so by the
// time the analyzer walks the chain, every wrapper has already cascaded
// into the canonical `where_(...,$(_:T) => ...)` / `select(...)` shape.
//
// `_sql_composability_macros.das` defines two wrappers:
//   * when_price_lt(it, val)         → _where(it, _.Price < val)
//   * when_lt(it, fieldExpr, val)    → _where(it, fieldExpr < val)
// ──────────────────────────────────────────────────────────────────────────

def private fixture_07(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// Baked-field wrapper — when_price_lt
// ──────────────────────────────────────────────────────────────────────────

[test(description = "a user wrapper macro expanding to _where emits the same SQL as writing _where directly")]
def test_when_price_lt_sql_text(t : T?) {
    var _db = SqlRunner()
    let with_wrapper = _sql_text(_db |> select_from(type<Car>) |> when_price_lt(200))
    let direct       = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price < 200))
    t |> equal(with_wrapper, direct, "when_price_lt and direct _where emit identical SQL")
    t |> equal(with_wrapper, "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Price\" < ?",
        "wrapper round-trips through _where → where_ → analyzer with the expected SQL")
}

[test(description = "a user wrapper macro inside _sql filters rows at runtime")]
def test_when_price_lt_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_07(db)
        let cars <- _sql(db |> select_from(type<Car>) |> when_price_lt(200))
        t |> equal(length(cars), 2, "two rows priced < 200")
        t |> equal(cars[0].Name, "BMW",  "row 0")
        t |> equal(cars[1].Name, "Lada", "row 1")
    }
}

[test(description = "a user wrapper macro inside _sql accepts a captured value")]
def test_when_price_lt_with_captured_value(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_07(db)
        let threshold = 300
        let cars <- _sql(db |> select_from(type<Car>) |> when_price_lt(threshold))
        t |> equal(length(cars), 3, "three rows priced < 300 (BMW, Lada, Audi)")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Generic wrapper — when_lt(it, _.Field, val)
// ──────────────────────────────────────────────────────────────────────────

[test(description = "a generic user wrapper taking a field expression emits the same SQL as _where")]
def test_when_lt_sql_text(t : T?) {
    var _db = SqlRunner()
    let with_wrapper = _sql_text(_db |> select_from(type<Car>) |> when_lt(_.Price, 200))
    let direct       = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price < 200))
    t |> equal(with_wrapper, direct, "when_lt and direct _where emit identical SQL")
}

[test(description = "a generic user wrapper routes its field argument to the right column")]
def test_when_lt_different_field(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> when_lt(_.Id, 3))
    t |> equal(sql, "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Id\" < ?",
        "field-ref argument routes through to the right column")
}

[test(description = "a generic user wrapper inside _sql filters rows at runtime")]
def test_when_lt_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_07(db)
        let cars <- _sql(db |> select_from(type<Car>) |> when_lt(_.Id, 3))
        t |> equal(length(cars), 2, "two rows with Id < 3 (BMW=1, Audi=2)")
        t |> equal(cars[0].Name, "BMW",  "row 0")
        t |> equal(cars[1].Name, "Audi", "row 1")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Composition: wrapper + canonical _where / _select in the same chain.
// Verifies the analyzer treats wrapper-emitted `where_` calls identically
// to user-written ones.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "a wrapper macro followed by _where combines both predicates with and")]
def test_wrapper_then_where_compose_as_and(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_07(db)
        let cars <- _sql(db |> select_from(type<Car>) |> when_price_lt(500) |> _where(_.Id != 2))
        t |> equal(length(cars), 3, "Price<500 AND Id!=2 → BMW, Tesla, Lada")
    }
}

[test(description = "a wrapper macro followed by _select filters and projects")]
def test_wrapper_then_select(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_07(db)
        let names <- _sql(db |> select_from(type<Car>) |> when_price_lt(300) |> _select(_.Name))
        t |> equal(length(names), 3, "three names priced < 300 (BMW, Lada, Audi)")
    }
}

[test(description = "a wrapper-emitted WHERE composes with an explicit _select projection in the emitted SQL")]
def test_wrapper_then_select_sql_text(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> when_price_lt(300) |> _select(_.Name))
    t |> equal(sql, "SELECT \"Name\" FROM \"Cars\" WHERE \"Price\" < ?",
        "wrapper-emitted WHERE composes with explicit _select projection")
}

[test(description = "two wrapper macro calls in one chain combine with and")]
def test_two_wrappers_compose_as_and(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_07(db)
        // Two wrapper calls — proves the analyzer's where AND-composition
        // treats wrapper output identically to source-level _where output.
        let cars <- _sql(db |> select_from(type<Car>) |> when_price_lt(500) |> when_lt(_.Id, 4))
        t |> equal(length(cars), 3, "Price<500 AND Id<4 → BMW(1), Audi(2), Tesla(3)")
    }
}

def private fixture_21(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",  Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi", Price = 200))
}

// ──────────────────────────────────────────────────────────────────────────
// _try_sql — non-panicking sibling of _sql. Same analyzer, wraps result
// in Result<T, string>.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "_try_sql on a well-formed query returns Ok with the rows")]
def test_try_sql_array_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_21(db)
        var res <- _try_sql(db |> select_from(type<Car>))
        t |> success(res |> is_ok, "well-formed query -> ok")
        let cars <- move_unwrap(res)
        t |> equal(length(cars), 2, "2 rows")
    }
}

[test(description = "_try_sql with _first returns Ok with the first row")]
def test_try_sql_first_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_21(db)
        let res = _try_sql(db |> select_from(type<Car>) |> _first())
        t |> success(res |> is_ok, "non-empty -> ok")
        let car = res |> unwrap
        t |> equal(car.Id, 1, "first row")
    }
}

[test(description = "_try_sql with _first on an empty table returns an error")]
def test_try_sql_first_err_on_empty(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let res = _try_sql(db |> select_from(type<Car>) |> _first())
        t |> success(res |> is_err, "empty + _first -> err")
    }
}

[test(description = "_try_sql with _first_opt returns Ok wrapping some on a non-empty table")]
def test_try_sql_first_opt_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_21(db)
        let res = _try_sql(db |> select_from(type<Car>) |> _first_opt())
        t |> success(res |> is_ok, "ok wraps the option")
        let opt = res |> unwrap
        t |> success(opt |> is_some, "non-empty -> some(...)")
    }
}

[test(description = "_try_sql with _first_opt returns Ok wrapping none on an empty table")]
def test_try_sql_first_opt_none(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let res = _try_sql(db |> select_from(type<Car>) |> _first_opt())
        t |> success(res |> is_ok, "empty -> ok(none) (not err)")
        let opt = res |> unwrap
        t |> success(opt |> is_none, "empty -> none")
    }
}

[test(description = "_try_sql with a count returns Ok with the row count")]
def test_try_sql_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_21(db)
        let res = _try_sql(db |> select_from(type<Car>) |> count())
        t |> success(res |> is_ok, "count is always 1 row -> ok")
        t |> equal(res |> unwrap, 2, "ok value")
    }
}

def private fixture_22(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla",   Price = 300))
}

// ──────────────────────────────────────────────────────────────────────────
// _select with named-tuple projection: (Name=_.Name, Price=_.Price)
// Returns array<tuple<Name:string;Price:int>> with recordNames preserved.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "_select with a two-field named tuple returns rows addressable by field name")]
def test_select_named_tuple_two_fields(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_22(db)
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _select((Name = _.Name, Price = _.Price)))
        t |> equal(length(rows), 3, "3 rows")
        t |> equal(rows[0].Name,  "BMW",  "row 0 Name (by tuple field)")
        t |> equal(rows[0].Price, 100,    "row 0 Price (by tuple field)")
        t |> equal(rows[2].Name,  "Tesla", "row 2 Name")
        t |> equal(rows[2].Price, 300,    "row 2 Price")
    }
}

[test(description = "a named-tuple _select followed by _first returns one named tuple")]
def test_select_named_tuple_with_first(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_22(db)
        let row = _sql(db |> select_from(type<Car>)
                         |> _select((Name = _.Name, Price = _.Price))
                         |> _first())
        t |> equal(row.Name,  "BMW", "first Name (named-tuple + _first)")
        t |> equal(row.Price, 100,   "first Price")
    }
}

[test(description = "a named-tuple _select emits a SELECT of exactly the two projected columns")]
def test_select_named_tuple_emits_select_two_cols(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _select((Name = _.Name, Price = _.Price)))
    t |> equal(sql,
        "SELECT \"Name\", \"Price\" FROM \"Cars\"",
        "named-tuple projection emits SELECT col1, col2")
}

[test(description = "a named-tuple _select can rename fields without changing which columns are read")]
def test_select_named_tuple_three_fields(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_22(db)
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _select((Identifier = _.Id, Label = _.Name, Cost = _.Price)))
        t |> equal(length(rows), 3, "3 rows")
        t |> equal(rows[1].Identifier, 2,      "renamed Id field")
        t |> equal(rows[1].Label,      "Audi", "renamed Name field")
        t |> equal(rows[1].Cost,       200,    "renamed Price field")
    }
}
