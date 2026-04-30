options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require daslib/fio

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Active : bool
}

[sql_table(name = "Items"), sql_index(fields = "Name", unique = true, name = "ux_Items_Name")]
struct Item {
    @sql_primary_key Id : int
    Name : string
}

def private build_archive_db(path : string) {
    var inscope arch = open_sqlite(path)
    arch |> create_table(type<User>)
    arch |> insert(User(Id = 1, Name = "alice", Active = true))
    arch |> insert(User(Id = 2, Name = "bob", Active = false))
    arch |> insert(User(Id = 3, Name = "carol", Active = true))
}

def private temp_archive_path(name : string) : string {
    let r = create_temp_file_result("test_93_{name}", ".db")
    let path = unsafe(r.value)
    remove(path)
    return path
}

[test]
def test_with_attached_finally_on_panic(t : T?) {
    with_sqlite(":memory:") $(db) {
        var panicked = false
        try {
            db |> with_attached(":memory:", "panic_arch") $(arch) {
                panic("body panic")
            }
        } recover {
            panicked = true
        }
        t |> success(panicked, "the inner panic must propagate")
    }
}

[test]
def test_with_schema_pure_copy(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> with_schema("archive") $(arch) {
            t |> equal(arch.schema_name, "archive")
            t |> equal(db.schema_name, "")
            t |> success(arch.sqlite_handle == db.sqlite_handle, "with_schema must alias the same handle")
        }
    }
}

[test]
def test_attach_detach_round_trip(t : T?) {
    let archive_path = temp_archive_path("attach_round_trip")
    build_archive_db(archive_path)
    with_sqlite(":memory:") $(db) {
        db |> attach(archive_path, "archive")
        db |> with_schema("archive") $(arch) {
            let names <- _sql(arch |> select_from(type<User>) |> _order_by(_.Id) |> _select(_.Name))
            t |> equal(length(names), 3)
            t |> equal(names[0], "alice")
            t |> equal(names[1], "bob")
            t |> equal(names[2], "carol")
        }
        db |> detach("archive")
    }
    remove(archive_path)
}

[test]
def test_with_attached_block(t : T?) {
    let archive_path = temp_archive_path("with_attached")
    build_archive_db(archive_path)
    with_sqlite(":memory:") $(db) {
        var seen = 0
        db |> with_attached(archive_path, "archive") $(arch) {
            for (u in _sql(arch |> select_from(type<User>) |> _where(_.Active) |> _order_by(_.Id))) {
                seen++
                t |> success(u.Active, "filtered row must be Active")
            }
        }
        t |> equal(seen, 2, "alice + carol are Active")
        // After block exit, the schema is detached — reattaching a fresh handle works.
        db |> attach(archive_path, "archive")
        db |> detach("archive")
    }
    remove(archive_path)
}

[test]
def test_try_attach_bad_path_returns_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> try_attach("/no/such/dir/missing.db", "ghost")
        t |> success(err |> is_some, "try_attach must surface SQLITE_CANTOPEN as some(errmsg)")
    }
}

[test]
def test_attached_create_table_with_sql_index(t : T?) {
    // Regression: `_qualify_schema` must rewrite `CREATE INDEX "..."` (no IF NOT EXISTS)
    // emitted by [sql_index] so the index lands in the attached schema, not main.
    let archive_path = temp_archive_path("attach_index")
    remove(archive_path)
    with_sqlite(":memory:") $(db) {
        db |> with_attached(archive_path, "ix_archive") $(arch) {
            arch |> create_table(type<Item>)
            arch |> insert(Item(Id = 1, Name = "a"))
            // Inserting a duplicate Name must hit the UNIQUE INDEX in the ATTACHED schema.
            let dup = arch |> try_insert(Item(Id = 2, Name = "a"))
            t |> success(dup |> is_err, "UNIQUE INDEX must reject duplicate Name in attached schema")
            // Verify the index actually lives in ix_archive (not main).
            let where_idx = db |> query_scalar_opt(
                "SELECT 1 FROM ix_archive.sqlite_master WHERE type='index' AND name='ux_Items_Name'",
                type<int>)
            t |> success(where_idx |> is_some, "index must exist in attached schema")
            let in_main = db |> query_scalar_opt(
                "SELECT 1 FROM main.sqlite_master WHERE type='index' AND name='ux_Items_Name'",
                type<int>)
            t |> success(in_main |> is_none, "index must NOT exist in main schema")
        }
    }
    remove(archive_path)
}

[test]
def test_schema_qualified_insert_update_delete(t : T?) {
    let archive_path = temp_archive_path("schema_dml")
    build_archive_db(archive_path)
    with_sqlite(":memory:") $(db) {
        db |> with_attached(archive_path, "archive") $(arch) {
            arch |> insert(User(Id = 4, Name = "dave", Active = true))
            // _sql_update via the qualified runner — schema rewrite must pick up UPDATE.
            arch |> _sql_update(type<User>, _.Id == 2, (Active = true))
            // _sql_delete via the qualified runner — DELETE FROM rewrite.
            arch |> _sql_delete(type<User>, _.Id == 1)
            let n = _sql(arch |> select_from(type<User>) |> count())
            // start 3 (alice, bob, carol) + insert dave = 4, delete alice = 3
            t |> equal(n, 3)
        }
    }
    remove(archive_path)
}
