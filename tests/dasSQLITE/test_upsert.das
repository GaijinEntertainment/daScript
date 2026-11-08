options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Email : string
    Name : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Email = "a@x.com", Name = "alice"))
}

[test(description = "insert_or_ignore inserts a row whose primary key is free and reports one row affected")]
def test_insert_or_ignore_inserts_new_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_ignore(User(Id = 2, Email = "b@x.com", Name = "bob"))
        t |> equal(n, 1, "fresh PK insert returns 1 row affected")
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 2, "row count after fresh insert")
    }
}

[test(description = "insert_or_ignore on a primary key conflict affects 0 rows and leaves the original row untouched")]
def test_insert_or_ignore_silently_ignores_pk_conflict(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_ignore(User(Id = 1, Email = "z@x.com", Name = "zoe"))
        t |> equal(n, 0, "PK conflict returns 0 rows affected")
        let name = db |> query_scalar("SELECT Name FROM Users WHERE Id = 1", type<string>)
        t |> equal(name, "alice", "original row left untouched by IGNORE")
    }
}

[test(description = "a bulk insert_or_ignore inserts the non-conflicting rows and skips the conflicting ones")]
def test_insert_or_ignore_bulk_partitions_by_conflict(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_ignore([
            User(Id = 1, Email = "ignored@x.com", Name = "dup"),
            User(Id = 2, Email = "b@x.com", Name = "bob"),
            User(Id = 3, Email = "c@x.com", Name = "cat")
        ])
        t |> equal(n, 2, "two of three rows actually inserted")
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 3, "row count: original alice + bob + cat")
    }
}

[test(description = "try_insert_or_ignore returns Ok with 0 rows affected on a conflict")]
def test_try_insert_or_ignore_ok_path(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_insert_or_ignore(User(Id = 1, Email = "z@x.com", Name = "zoe"))
        t |> success(r |> is_ok, "try_insert_or_ignore on conflict still returns Ok")
        t |> equal(r |> unwrap, 0, "rows-affected = 0 because IGNORE silenced the conflict")
    }
}

[test(description = "insert_or_replace overwrites the row with the same primary key without creating a duplicate")]
def test_insert_or_replace_overwrites_existing(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_replace(User(Id = 1, Email = "a2@x.com", Name = "alice2"))
        t |> equal(n, 1, "replace returns 1 row affected")
        let name = db |> query_scalar("SELECT Name FROM Users WHERE Id = 1", type<string>)
        t |> equal(name, "alice2", "row was overwritten")
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 1, "no duplicate row created")
    }
}

[test(description = "insert_or_replace inserts a new row when the primary key is free")]
def test_insert_or_replace_inserts_when_pk_free(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_replace(User(Id = 5, Email = "e@x.com", Name = "eve"))
        t |> equal(n, 1, "fresh PK gives 1 row affected")
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 2, "two rows now")
    }
}

[test(description = "a bulk insert_or_replace replaces existing rows and inserts new ones in one call")]
def test_insert_or_replace_bulk(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_replace([
            User(Id = 1, Email = "a3@x.com", Name = "alice3"),
            User(Id = 2, Email = "b@x.com", Name = "bob")
        ])
        t |> equal(n, 2, "both rows applied (one replace, one insert)")
        let alice_name = db |> query_scalar("SELECT Name FROM Users WHERE Id = 1", type<string>)
        t |> equal(alice_name, "alice3", "row 1 replaced")
    }
}

[test(description = "try_insert_or_replace returns Ok with the rows-affected count")]
def test_try_insert_or_replace_returns_result(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_insert_or_replace(User(Id = 1, Email = "ar@x.com", Name = "alice_r"))
        t |> success(r |> is_ok, "ok path")
        t |> equal(r |> unwrap, 1, "1 row affected")
    }
}

[sql_table(name = "WordHits")]
struct WordHit {
    @sql_primary_key Id : int
    Word : string
    Hits : int
    Last : int64
}

def private fixture_54(db : SqlRunner) {
    db |> create_table(type<WordHit>)
    db |> insert(WordHit(Id = 1, Word = "hello", Hits = 1, Last = 100l))
}

[test(description = "upsert inserts the proposed row when there is no conflict")]
def test_upsert_inserts_when_no_conflict(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_54(db)
        let n = db |> _sql_upsert(
            WordHit(Id = 2, Word = "world", Hits = 1, Last = 200l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        t |> equal(n, 1, "fresh insert returns 1")
        let world_hits = db |> query_scalar("SELECT Hits FROM WordHits WHERE Id = 2", type<int>)
        t |> equal(world_hits, 1, "fresh row used the INSERT values, not DO UPDATE")
    }
}

[test(description = "upsert on a primary key collision runs the DO UPDATE and leaves untouched columns alone")]
def test_upsert_merges_on_pk_collision(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_54(db)
        let n = db |> _sql_upsert(
            WordHit(Id = 1, Word = "ignored", Hits = 99, Last = 300l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        t |> equal(n, 1, "merge counts 1 row affected")
        let h = db |> query_scalar("SELECT Hits FROM WordHits WHERE Id = 1", type<int>)
        t |> equal(h, 2, "Hits incremented from existing 1 to 2")
        let last = db |> query_scalar("SELECT Last FROM WordHits WHERE Id = 1", type<int64>)
        t |> equal(last, 300l, "Last was updated from _excluded.Last (300)")
        let word = db |> query_scalar("SELECT Word FROM WordHits WHERE Id = 1", type<string>)
        t |> equal(word, "hello", "Word column unchanged because do_update doesn't touch it")
    }
}

[test(description = "one DO UPDATE expression can combine the existing row and the proposed excluded row")]
def test_upsert_excluded_and_dot_underscore_compose(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_54(db)
        // Exercises both `_.Hits` (existing) and `_excluded.Hits` (proposed)
        // in the same DO UPDATE expression.
        db |> _sql_upsert(
            WordHit(Id = 1, Word = "x", Hits = 7, Last = 0l),
            _.Id,
            (Hits = _.Hits + _excluded.Hits, Last = _excluded.Last))
        let h = db |> query_scalar("SELECT Hits FROM WordHits WHERE Id = 1", type<int>)
        t |> equal(h, 8, "1 (existing) + 7 (proposed) = 8")
    }
}

// Composite UNIQUE on (Email, Tenant) via [sql_index(unique=true, fields=...)].
[sql_table(name = "UserAccts"),
 sql_index(fields = ("Email", "Tenant"), unique = true, name = "ux_useraccts_email_tenant")]

struct UserAcct {
    @sql_primary_key Id : int
    Email : string
    Name : string
    Tenant : string
}

def private fixture_55(db : SqlRunner) {
    db |> create_table(type<UserAcct>)
    db |> insert(UserAcct(Id = 1, Email = "x@y.com", Name = "alice", Tenant = "acme"))
}

[test(description = "upsert conflicts on a composite unique index and merges into the existing row")]
def test_upsert_composite_conflict_fires(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_55(db)
        // Same (Email, Tenant) → conflict → DO UPDATE
        let n = db |> _sql_upsert(
            UserAcct(Id = 999, Email = "x@y.com", Name = "alice2", Tenant = "acme"),
            tuple(_.Email, _.Tenant),
            (Name = _excluded.Name))
        t |> equal(n, 1, "merge counted 1")
        let name = db |> query_scalar("SELECT Name FROM UserAccts WHERE Email='x@y.com' AND Tenant='acme'", type<string>)
        t |> equal(name, "alice2", "Name updated via _excluded.Name")
        let total = db |> query_scalar("SELECT COUNT(*) FROM UserAccts", type<int>)
        t |> equal(total, 1, "still one row — composite conflict merged into existing")
    }
}

[test(description = "upsert with a different value in one composite key column inserts a new row")]
def test_upsert_composite_no_conflict_inserts(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_55(db)
        // Different Tenant → no conflict → fresh insert
        db |> _sql_upsert(
            UserAcct(Id = 2, Email = "x@y.com", Name = "alice_other", Tenant = "other"),
            tuple(_.Email, _.Tenant),
            (Name = _excluded.Name))
        let total = db |> query_scalar("SELECT COUNT(*) FROM UserAccts", type<int>)
        t |> equal(total, 2, "two rows — different Tenant doesn't conflict")
    }
}

def private fixture_56(db : SqlRunner) {
    db |> create_table(type<WordHit>)
    db |> insert(WordHit(Id = 1, Word = "hello", Hits = 1, Last = 100l))
}

[test(description = "upsert with RETURNING gives back the row as it is after the merge")]
def test_upsert_returning_yields_post_merge_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_56(db)
        let rows <- db |> _sql_upsert_returning(
            WordHit(Id = 1, Word = "x", Hits = 7, Last = 999l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        t |> equal(length(rows), 1, "RETURNING yields one row")
        t |> equal(rows[0].Hits, 2, "post-merge Hits = existing 1 + 1")
        t |> equal(rows[0].Last, 999l, "post-merge Last = _excluded.Last")
        t |> equal(rows[0].Word, "hello", "Word unchanged")
    }
}

[test(description = "upsert with RETURNING on a fresh key gives back the inserted row")]
def test_upsert_returning_for_fresh_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_56(db)
        let rows <- db |> _sql_upsert_returning(
            WordHit(Id = 2, Word = "world", Hits = 5, Last = 0l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        t |> equal(length(rows), 1, "RETURNING yields the just-inserted row")
        t |> equal(rows[0].Hits, 5, "fresh insert kept the proposed Hits=5")
    }
}

[test(description = "try_upsert_returning returns Ok with the merged row")]
def test_try_upsert_returning_ok_path(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_56(db)
        let r <- db |> _sql_try_upsert_returning(
            WordHit(Id = 1, Word = "x", Hits = 0, Last = 0l),
            _.Id,
            (Hits = _.Hits + 10, Last = _excluded.Last))
        t |> success(r |> is_ok, "try-returning succeeded")
        let rows <- r |> unwrap
        t |> equal(length(rows), 1, "one row returned")
        t |> equal(rows[0].Hits, 11, "existing 1 + 10 = 11")
    }
}

// `@sql_column` rename across UPSERT (INSERT ... ON CONFLICT ... DO UPDATE SET).
// The rename must surface on (a) the INSERT column list, (b) the ON CONFLICT
// key column, (c) the DO UPDATE SET clause LHS, and (d) the `excluded.<col>`
// reference to the proposed row. The runtime tests below exercise all four
// because the table schema is generated by `[sql_table]` (so the SQL columns
// are `"for"` etc.); any emission site that still names the daslang field
// would fail at SQLite parse time with `no such column: col_for`.

[sql_table(name = "Counters")]
struct Counter {
    @sql_primary_key Id : int
    @sql_column = "for"
    @sql_unique col_for : string  // 'for' is a daslang keyword — user types col_for
    Hits : int
}

[test(description = "a renamed column appears under its SQL name in the table DDL and the unique index")]
def test_create_table_uses_renamed_unique_column(t : T?) {
    let ddl = _::_sql_create_table_sql(type<Counter>)
    t |> success(ddl |> find("\"for\" TEXT NOT NULL") >= 0,
        "renamed column shows up as 'for' in table DDL")
    t |> success(ddl |> find("\"col_for\"") < 0,
        "daslang field 'col_for' must NOT appear in DDL")
    t |> equal(_::_sql_create_indexes_sql(type<Counter>),
        "CREATE UNIQUE INDEX \"uq_Counters_for\" ON \"Counters\" (\"for\")",
        "@sql_unique uses the renamed SQL column in its portable index")
}

[test(description = "upsert on a table with a renamed column inserts and reads back through the daslang field name")]
def test_upsert_insert_path_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Counter>)
        db |> _sql_upsert(Counter(Id = 0, col_for = "home", Hits = 1),
                          _.col_for, (Hits = _.Hits + _excluded.Hits))
        let rows <- db |> select_from(type<Counter>)
        t |> equal(length(rows), 1, "one row")
        t |> equal(rows[0].col_for, "home", "renamed col round-trips")
        t |> equal(rows[0].Hits, 1, "Hits stored")
    }
}

[test(description = "upsert conflict on a renamed key column resolves through DO UPDATE using the SQL column names")]
def test_upsert_conflict_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Counter>)
        // First upsert — INSERT path.
        db |> _sql_upsert(Counter(Id = 0, col_for = "home", Hits = 1),
                          _.col_for, (Hits = _.Hits + _excluded.Hits))
        // Second upsert — same `for`, so DO UPDATE SET runs. The DO UPDATE
        // SET clause must reference renamed "for" on both sides:
        //   - LHS: SET "Hits" (not renamed)
        //   - RHS: _.Hits + _excluded.Hits (Hits not renamed; the renamed
        //          column would be excluded."for" but isn't used here).
        db |> _sql_upsert(Counter(Id = 0, col_for = "home", Hits = 7),
                          _.col_for, (Hits = _.Hits + _excluded.Hits))
        let rows <- db |> select_from(type<Counter>)
        t |> equal(length(rows), 1, "one row after two upserts (conflict resolved)")
        t |> equal(rows[0].Hits, 8, "Hits = 1 + 7 (existing + excluded)")
    }
}

[test(description = "excluded referencing a renamed column reaches SQLite under its SQL name")]
def test_upsert_excluded_renamed_column_runtime(t : T?) {
    // Use `_excluded.col_for` in the SET clause to verify `excluded."for"`
    // (with the SQL identifier) is what reaches SQLite — daslang field
    // `col_for` would fail with `no such column: excluded.col_for`.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Counter>)
        db |> _sql_upsert(Counter(Id = 0, col_for = "home", Hits = 1),
                          _.col_for, (Hits = _.Hits + _excluded.Hits))
        // Update by reassigning col_for to the excluded value (no-op
        // semantically — proves the path compiles and runs).
        db |> _sql_upsert(Counter(Id = 0, col_for = "home", Hits = 99),
                          _.col_for, (col_for = _excluded.col_for))
        let rows <- db |> select_from(type<Counter>)
        t |> equal(length(rows), 1, "still one row")
        t |> equal(rows[0].col_for, "home", "col_for unchanged after self-assign upsert")
    }
}
