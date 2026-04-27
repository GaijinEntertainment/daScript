options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="A")]
struct TabA {
    @sql_primary_key Id : int
    Tag : int
}

[sql_table(name="B")]
struct TabB {
    @sql_primary_key Id : int
    Tag : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<TabA>)
    db |> create_table(type<TabB>)
    db |> insert(TabA(Id = 1, Tag = 10))
    db |> insert(TabA(Id = 2, Tag = 20))
    db |> insert(TabA(Id = 3, Tag = 30))
    db |> insert(TabB(Id = 100, Tag = 20))
    db |> insert(TabB(Id = 200, Tag = 30))
    db |> insert(TabB(Id = 300, Tag = 40))
}

// ──────────────────────────────────────────────────────────────────────────
// SQL emission — UNION / INTERSECT / EXCEPT wrap the LHS query and append
// the RHS query separated by the set-op keyword.
// Both sides project the same column shape (here `_.Tag`) so the resulting
// schemas align.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_union_emits_union(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text((_db |> select_from(type<TabA>) |> _select(_.Tag))
                        |> union(_db |> select_from(type<TabB>) |> _select(_.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" UNION SELECT \"Tag\" FROM \"B\"",
        "union(a, b) emits `<a-sql> UNION <b-sql>`")
}

[test]
def test_intersect_emits_intersect(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text((_db |> select_from(type<TabA>) |> _select(_.Tag))
                        |> intersect(_db |> select_from(type<TabB>) |> _select(_.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" INTERSECT SELECT \"Tag\" FROM \"B\"",
        "intersect(a, b) emits `<a-sql> INTERSECT <b-sql>`")
}

[test]
def test_except_emits_except(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text((_db |> select_from(type<TabA>) |> _select(_.Tag))
                        |> except(_db |> select_from(type<TabB>) |> _select(_.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" EXCEPT SELECT \"Tag\" FROM \"B\"",
        "except(a, b) emits `<a-sql> EXCEPT <b-sql>`")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime — verify the actual rows match the set-op semantics.
// A.Tag values: {10, 20, 30}; B.Tag values: {20, 30, 40}
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_union_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var rows <- _sql((db |> select_from(type<TabA>) |> _select(_.Tag))
                         |> union(db |> select_from(type<TabB>) |> _select(_.Tag)))
        // UNION dedupes: {10, 20, 30, 40}
        sort(rows)
        t |> equal(length(rows), 4, "UNION = 4 distinct tags")
        t |> equal(rows[0], 10)
        t |> equal(rows[1], 20)
        t |> equal(rows[2], 30)
        t |> equal(rows[3], 40)
    }
}

[test]
def test_intersect_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var rows <- _sql((db |> select_from(type<TabA>) |> _select(_.Tag))
                         |> intersect(db |> select_from(type<TabB>) |> _select(_.Tag)))
        // {20, 30}
        sort(rows)
        t |> equal(length(rows), 2)
        t |> equal(rows[0], 20)
        t |> equal(rows[1], 30)
    }
}

[test]
def test_except_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var rows <- _sql((db |> select_from(type<TabA>) |> _select(_.Tag))
                         |> except(db |> select_from(type<TabB>) |> _select(_.Tag)))
        // A's tags minus B's = {10}
        sort(rows)
        t |> equal(length(rows), 1)
        t |> equal(rows[0], 10)
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Compound-query bind ordering — set-op SQL is `<lhs> {OP} <rhs>` with no
// per-side parens, so placeholders run left-to-right across both sides.
// The runtime bind list must follow SQL clause order:
//   LHS(on → where → having)  then  RHS(on → where → having)
// These tests pin that ordering: a misordered build would produce wrong
// runtime values OR get caught by the SQL emission check (placeholder count
// vs. declared bind count) before runtime.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_set_op_basic_lhs_rhs_where_binds(t : T?) {
    var _db = SqlRunner()
    let lo = 15
    let hi = 35
    let sql = _sql_text(
        (_db |> select_from(type<TabA>) |> _where(_.Tag >= lo) |> _select(_.Tag))
        |> union(_db |> select_from(type<TabB>) |> _where(_.Tag <= hi) |> _select(_.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" WHERE \"Tag\" >= ? UNION SELECT \"Tag\" FROM \"B\" WHERE \"Tag\" <= ?",
        "compound query with WHERE on both sides emits two placeholders, LHS then RHS")
}

[test]
def test_set_op_basic_lhs_rhs_where_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let lo = 15
        let hi = 35
        var rows <- _sql(
            (db |> select_from(type<TabA>) |> _where(_.Tag >= lo) |> _select(_.Tag))
            |> union(db |> select_from(type<TabB>) |> _where(_.Tag <= hi) |> _select(_.Tag)))
        // A.Tag where >= 15 → {20, 30}; B.Tag where <= 35 → {20, 30}; UNION → {20, 30}
        sort(rows)
        t |> equal(length(rows), 2, "compound runtime: bind order must match LHS-WHERE first, RHS-WHERE second")
        t |> equal(rows[0], 20)
        t |> equal(rows[1], 30)
    }
}

[test]
def test_set_op_lhs_having_then_rhs_where_runtime(t : T?) {
    // The discriminating test for the round-3 bind-ordering fix: LHS has
    // both _where (placeholder #1) AND _having (placeholder #2), then UNION,
    // then RHS has _where (placeholder #3). Pre-fix, LHS HAVING binds were
    // pushed to q.havingBindExprs which the finalizer appended LAST — so
    // placeholders would have ended up in order [LHS-WHERE, RHS-WHERE,
    // LHS-HAVING], silently swapping LHS-HAVING and RHS-WHERE values.
    //
    // Both sides project as named-tuple `(Tg : int)` so the UNION column
    // shape matches (a `_select` after `_group_by` must be a named tuple).
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let lhs_min_tag = 15      // LHS WHERE bind value
        let lhs_min_count = 1     // LHS HAVING bind value (each Tag occurs once in A)
        let rhs_max_tag = 25      // RHS WHERE bind value
        var rows <- _sql(
            (db |> select_from(type<TabA>)
                |> _where(_.Tag >= lhs_min_tag)
                |> _group_by(_.Tag)
                |> _having(_._1 |> length >= lhs_min_count)
                |> _select((Tg = _._0)))
            |> union(db |> select_from(type<TabB>) |> _where(_.Tag <= rhs_max_tag) |> _select((Tg = _.Tag))))
        // LHS: A.Tag where >= 15, GROUP BY Tag, HAVING count >= 1 → {20, 30}
        // RHS: B.Tag where <= 25 → {20}
        // UNION: distinct {20, 30}
        // If the misordering were present, swapping placeholders #2 and #3
        // would assign rhs_max_tag=25 to the LHS HAVING (count >= 25 → 0
        // rows) and lhs_min_count=1 to the RHS WHERE (Tag <= 1 → 0 rows),
        // producing an empty result.
        var tags : array<int>
        for (r in rows) {
            tags |> push(r.Tg)
        }
        sort(tags)
        t |> equal(length(tags), 2, "LHS HAVING + RHS WHERE binds must occupy correct placeholder positions")
        t |> equal(tags[0], 20)
        t |> equal(tags[1], 30)
    }
}

[test]
def test_set_op_lhs_having_emits_correct_sql(t : T?) {
    var _db = SqlRunner()
    let lo = 15
    let mc = 1
    let hi = 25
    let sql = _sql_text(
        (_db |> select_from(type<TabA>)
            |> _where(_.Tag >= lo)
            |> _group_by(_.Tag)
            |> _having(_._1 |> length >= mc)
            |> _select((Tg = _._0)))
        |> union(_db |> select_from(type<TabB>) |> _where(_.Tag <= hi) |> _select((Tg = _.Tag))))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" WHERE \"Tag\" >= ? GROUP BY \"Tag\" HAVING COUNT(*) >= ? UNION SELECT \"Tag\" FROM \"B\" WHERE \"Tag\" <= ?",
        "compound SQL preserves clause order: LHS WHERE → GROUP BY → HAVING → UNION → RHS WHERE")
}

[test]
def test_set_op_rhs_join_on_bind_emits_correct_sql(t : T?) {
    // RHS has a join whose right-side _where carries a bind — the bind
    // belongs to the JOIN's ON clause (per chunk-5 ON-emission). The fix
    // also walks subQ.joins to fold their rightOnBindExprs into the
    // compound-query placeholder list.
    var _db = SqlRunner()
    let bnd = 5
    let sql = _sql_text(
        (_db |> select_from(type<TabA>) |> _select(_.Tag))
        |> union(_db |> select_from(type<TabA>)
                    |> _join(_db |> select_from(type<TabB>) |> _where(_.Tag > bnd),
                             $(a : TabA, b : TabB) => a.Id == b.Id,
                             $(a : TabA, b : TabB) => a.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" UNION SELECT \"t0\".\"Tag\" FROM \"A\" AS \"t0\" INNER JOIN \"B\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"Id\" AND (\"t1\".\"Tag\" > ?)",
        "RHS join right-side _where placeholder appears inside compound query's RHS ON clause")
}
