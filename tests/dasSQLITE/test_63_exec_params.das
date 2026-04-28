options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

// Item A — exec/try_exec parameterized binds (chunk-6 deferred).
//
// `query_one` shipped 0/1/2/3-arg variadic overloads in chunk 3; `exec`
// got the same fan-out in chunk 8 so raw DDL/DML can take captured
// daslang values without inlining them into the SQL string.

[sql_table(name = "T")]
struct Row {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

[test]
def test_exec_one_arg(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Row>)
        let id = 7
        db |> exec("INSERT INTO T(Id, Name, Score) VALUES (?, 'a', 0)", id)
        let row = db |> query_one("SELECT * FROM T WHERE Id = ?", type<Row>, 7)
        t |> equal(row.Id, 7, "1-arg exec bound id")
    }
}

[test]
def test_exec_two_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Row>)
        db |> exec("INSERT INTO T(Id, Name, Score) VALUES (?, ?, 0)", 1, "alpha")
        let row = db |> query_one("SELECT * FROM T WHERE Id = ?", type<Row>, 1)
        t |> equal(row.Name, "alpha", "2-arg exec bound id + name")
    }
}

[test]
def test_exec_three_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Row>)
        db |> exec("INSERT INTO T(Id, Name, Score) VALUES (?, ?, ?)", 1, "alpha", 100)
        let row = db |> query_one("SELECT * FROM T WHERE Id = ?", type<Row>, 1)
        t |> equal(row.Score, 100, "3-arg exec bound all three columns")
    }
}

[test]
def test_try_exec_returns_error(t : T?) {
    with_sqlite(":memory:") $(db) {
        // bind to a non-existent table — try_exec returns some(errmsg)
        let err = db |> try_exec("INSERT INTO NoTable VALUES (?)", 1)
        t |> success(err |> is_some(), "missing table → some(err)")
    }
}

[test]
def test_try_exec_success_returns_none(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Row>)
        let err = db |> try_exec("INSERT INTO T(Id, Name, Score) VALUES (?, 'x', 0)", 42)
        t |> success(err |> is_none(), "successful exec → none")
        let count = db |> query_scalar("SELECT COUNT(*) FROM T", type<int>)
        t |> equal(count, 1, "row visible after try_exec success")
    }
}

[test]
def test_exec_no_args_still_works(t : T?) {
    //! Regression: the chunk-8 variadic overloads must not break the
    //! existing 0-arg ``exec(sql)`` path used everywhere in dasSQLITE
    //! for DDL.
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE X(a INT)")
        let count = db |> query_scalar("SELECT COUNT(*) FROM X", type<int>)
        t |> equal(count, 0, "0-arg exec still works")
    }
}
