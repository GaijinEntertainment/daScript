options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require math

[sql_table(name="Score")]
struct Score {
    @sql_primary_key Id : int
    Delta : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Score>)
    db |> insert(Score(Id = 1, Delta = -150))
    db |> insert(Score(Id = 2, Delta = 80))
    db |> insert(Score(Id = 3, Delta = 200))
    db |> insert(Score(Id = 4, Delta = -50))
}

[test]
def test_where_abs_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let threshold = 100
        let rows <- _sql(db |> select_from(type<Score>)
                           |> _where((_.Delta |> abs()) > threshold))
        t |> equal(length(rows), 2, "|delta|>100: Id 1 (-150) + Id 3 (200)")
    }
}

[test]
def test_where_abs_emits_abs_call(t : T?) {
    var _db = SqlRunner()
    let threshold = 100
    let sql = _sql_text(_db |> select_from(type<Score>)
                          |> _where((_.Delta |> abs()) > threshold))
    t |> equal(sql,
        "SELECT \"Id\", \"Delta\" FROM \"Score\" WHERE ABS(\"Delta\") > ?",
        "abs -> ABS(...)")
}
