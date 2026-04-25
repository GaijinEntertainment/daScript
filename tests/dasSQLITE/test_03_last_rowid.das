options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name="Friends")]
struct Friend {
    @sql_primary_key Id : int
    Name : string
}

[test]
def test_no_pk_insert_assigns_rowid(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let new_id = db |> insert(Friend(Name = "Tom"))
        t |> equal(new_id, int64(1), "first no-PK insert assigns rowid 1")
        let nm = db |> query_scalar("SELECT Name FROM Friends WHERE Id = 1", type<string>)
        t |> equal(nm, "Tom", "row stored under assigned rowid")
    }
}

[test]
def test_explicit_pk_insert_keeps_id(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let new_id = db |> insert(Friend(Id = 42, Name = "Roger"))
        t |> equal(new_id, int64(42), "explicit PK is preserved")
        let nm = db |> query_scalar("SELECT Name FROM Friends WHERE Id = 42", type<string>)
        t |> equal(nm, "Roger", "row landed at explicit Id")
    }
}

[test]
def test_array_insert_returns_last_rowid(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let last_id = db |> insert([
            Friend(Name="Tom"),
            Friend(Name="Rebecca"),
            Friend(Name="Jim"),
            Friend(Name="Roger"),
            Friend(Name="Robert")
        ])
        t |> equal(last_id, int64(5), "array insert returns last rowid")
    }
}

[test]
def test_last_insert_rowid_matches_insert_return(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let returned = db |> insert(Friend(Name = "One"))
        let queried = db |> last_insert_rowid()
        t |> equal(returned, queried, "insert return value equals last_insert_rowid()")
    }
}

[test]
def test_pk_is_unset_predicate(t : T?) {
    let unset = Friend(Name = "X")
    let set_ = Friend(Id = 42, Name = "X")
    t |> success(_sql_pk_is_unset(unset), "Id=0 is unset")
    t |> success(!_sql_pk_is_unset(set_), "Id=42 is set")
}

[test]
def test_drop_then_recreate(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        db |> insert(Friend(Name = "Tom"))
        db |> drop_table_if_exists(type<Friend>)
        // Drop must succeed even though it existed.
        db |> create_table(type<Friend>)
        // After recreate, rowid sequence restarts at 1 for INTEGER PRIMARY KEY.
        let new_id = db |> insert(Friend(Name = "Reset"))
        t |> equal(new_id, int64(1), "rowid restarts at 1 after drop+create")
    }
}
