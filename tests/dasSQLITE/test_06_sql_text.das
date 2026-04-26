options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
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

[test]
def test_sql_text_bare_select_from(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>))
    t |> equal(sql, "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\"", "bare select_from emits SELECT * with all columns and table name")
}

[test]
def test_sql_text_uses_sql_table_name_override(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>))
    t |> success(sql |> find("FROM \"Cars\"") >= 0, "FROM uses [sql_table(name=)] override, not struct name")
}

// ── _to_array — no-op terminal ────────────────────────────────────────────

[test]
def test_sql_text_to_array_is_noop(t : T?) {
    var _db = SqlRunner()
    let with_to    = _sql_text(_db |> select_from(type<Car>) |> _to_array())
    let without_to = _sql_text(_db |> select_from(type<Car>))
    t |> equal(with_to, without_to, "_to_array() does not change emitted SQL")
}

// ── _select — single-column projection ───────────────────────────────────

[test]
def test_sql_text_select_single_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Name))
    t |> equal(sql, "SELECT \"Name\" FROM \"Cars\"", "_select(_.Field) projects single column")
}

[test]
def test_sql_text_select_int_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price))
    t |> equal(sql, "SELECT \"Price\" FROM \"Cars\"", "_select(_.IntField) projects int column")
}

// ── _where — filter clause ───────────────────────────────────────────────

[test]
def test_sql_text_where_eq_literal(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 5))
    t |> equal(sql, "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE Id = ?", "WHERE eq literal: ?")
}

[test]
def test_sql_text_where_neq(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id != 5))
    t |> success(sql |> find("WHERE Id <> ?") >= 0, "WHERE != becomes <>")
}

[test]
def test_sql_text_where_lt(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price < 200))
    t |> success(sql |> find("WHERE Price < ?") >= 0, "WHERE < emits <")
}

[test]
def test_sql_text_where_le(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price <= 200))
    t |> success(sql |> find("WHERE Price <= ?") >= 0, "WHERE <= emits <=")
}

[test]
def test_sql_text_where_gt(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price > 200))
    t |> success(sql |> find("WHERE Price > ?") >= 0, "WHERE > emits >")
}

[test]
def test_sql_text_where_ge(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price >= 200))
    t |> success(sql |> find("WHERE Price >= ?") >= 0, "WHERE >= emits >=")
}

[test]
def test_sql_text_where_string_literal(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Name == "BMW"))
    t |> success(sql |> find("WHERE Name = ?") >= 0, "WHERE eq string literal also gets ? placeholder")
}

[test]
def test_sql_text_where_captured_var(t : T?) {
    var _db = SqlRunner()
    let _target = 3
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == _target))
    t |> success(sql |> find("WHERE Id = ?") >= 0, "captured var also yields ? placeholder")
}

[test]
def test_sql_text_where_captured_and_literal_match(t : T?) {
    // Captured-var literal vs. inline literal produce identical SQL — proves the
    // macro emits placeholders, not the literal value.
    var _db = SqlRunner()
    let _target = 5
    let with_var     = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == _target))
    let with_literal = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 5))
    t |> equal(with_var, with_literal, "captured var and inline literal produce identical SQL (both ?)")
}

[test]
def test_sql_text_where_and(t : T?) {
    var _db = SqlRunner()
    let _lo = 100
    let _hi = 500
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price >= _lo && _.Price <= _hi))
    t |> success(sql |> find("AND") >= 0, "AND appears in compound WHERE")
    t |> success(sql |> find("Price >= ?") >= 0, "first conjunct present")
    t |> success(sql |> find("Price <= ?") >= 0, "second conjunct present")
}

[test]
def test_sql_text_where_or(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 1 || _.Id == 2))
    t |> success(sql |> find("OR") >= 0, "OR appears in compound WHERE")
}

[test]
def test_sql_text_where_not(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(!(_.Id == 5)))
    t |> success(sql |> find("NOT") >= 0, "NOT appears for ! op")
}

// ── compound: _where + _select ────────────────────────────────────────────

[test]
def test_sql_text_where_then_select(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price > 100) |> _select(_.Name))
    t |> equal(sql, "SELECT \"Name\" FROM \"Cars\" WHERE Price > ?", "_where + _select compose")
}

[test]
def test_sql_text_where_with_to_array(t : T?) {
    var _db = SqlRunner()
    let with_to    = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 1) |> _to_array())
    let without_to = _sql_text(_db |> select_from(type<Car>) |> _where(_.Id == 1))
    t |> equal(with_to, without_to, "_to_array still no-op when chained after _where")
}
