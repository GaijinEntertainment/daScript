options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// @sql_json on a tuple — round-trip via daslib/json (TEXT storage, JV/from_JV).
[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    @sql_json Prefs : tuple<theme : string; lang : string; notify : bool>
}

[test]
def test_sql_json_tuple_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        db |> insert(User(Id = 1, Name = "alice",
            Prefs = (theme = "dark", lang = "en", notify = true)))
        db |> insert(User(Id = 2, Name = "bob",
            Prefs = (theme = "light", lang = "ru", notify = false)))

        let rows <- db |> select_from(type<User>)
        t |> equal(length(rows), 2, "two rows")
        t |> equal(rows[0].Prefs.theme, "dark", "alice theme round-trips")
        t |> equal(rows[0].Prefs.lang, "en", "alice lang round-trips")
        t |> equal(rows[0].Prefs.notify, true, "alice notify round-trips")
        t |> equal(rows[1].Prefs.theme, "light", "bob theme round-trips")
        t |> equal(rows[1].Prefs.notify, false, "bob notify round-trips")
    }
}

[test]
def test_sql_json_ddl_uses_text_storage(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        // PRAGMA reports column types verbatim from CREATE TABLE — confirms
        // the @sql_json DDL override emits TEXT, not whatever sql_storage_type_for
        // would have tried for tuple<...>.
        let prefs_col_type = db |> query_scalar(
            "SELECT type FROM pragma_table_info('Users') WHERE name='Prefs'",
            type<string>)
        t |> equal(prefs_col_type, "TEXT", "@sql_json column DDL is TEXT")
    }
}

// @sql_blob on a struct — round-trip via daslib/archive (BLOB storage,
// mem_archive_save / mem_archive_load).
struct SessionSnapshot {
    last_scene : string
    unlocks : array<int>
    last_tick_ms : int64
}

[sql_table(name = "Sessions")]
struct SessionRow {
    @sql_primary_key UserId : int
    StartedAt : int64
    @sql_blob State : SessionSnapshot
}

[test]
def test_sql_blob_struct_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<SessionRow>)

        var snap = SessionSnapshot(
            last_scene = "intro",
            unlocks <- [1, 2, 3, 5],
            last_tick_ms = 1234567l)

        db |> insert(SessionRow(UserId = 42, StartedAt = 1000l, State <- snap))

        let rows <- db |> select_from(type<SessionRow>)
        t |> equal(length(rows), 1, "one session row")
        t |> equal(rows[0].State.last_scene, "intro", "scene round-trips through blob")
        t |> equal(length(rows[0].State.unlocks), 4, "unlocks count round-trips")
        t |> equal(rows[0].State.unlocks[3], 5, "unlocks values round-trip")
        t |> equal(rows[0].State.last_tick_ms, 1234567l, "tick ms round-trips")
    }
}

[test]
def test_sql_blob_ddl_uses_blob_storage(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<SessionRow>)
        let state_col_type = db |> query_scalar(
            "SELECT type FROM pragma_table_info('Sessions') WHERE name='State'",
            type<string>)
        t |> equal(state_col_type, "BLOB", "@sql_blob column DDL is BLOB")
    }
}

// Two `[sql_table]`s sharing the same JSON-typed field — adapter dedup
// must not error on duplicate sql_bind / sql_extract definitions.
[sql_table(name = "OtherJson")]
struct OtherJsonHolder {
    @sql_primary_key Id : int
    @sql_json Prefs : tuple<theme : string; lang : string; notify : bool>
}

[test]
def test_sql_json_adapter_is_deduped_across_tables(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        db |> create_table(type<OtherJsonHolder>)
        db |> insert(OtherJsonHolder(Id = 7,
            Prefs = (theme = "blue", lang = "fr", notify = false)))
        let rows <- db |> select_from(type<OtherJsonHolder>)
        t |> equal(length(rows), 1, "second table also round-trips")
        t |> equal(rows[0].Prefs.theme, "blue", "shared adapter still works")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// `_sql` walker — JSON-path descent (B4).
//
// `_.<json_col>.<path…>` on an `@sql_json` column descends into the stored
// JSON via SQLite's `json_extract`. Nested struct paths concatenate dotted.
// `@sql_blob` columns are opaque — descent into them is a compile error.
// ──────────────────────────────────────────────────────────────────────────

// Nested-struct schema used by the multi-level descent tests.
struct InnerAddress {
    City : string
    Zip : string
}
struct InnerProfile {
    Addr : InnerAddress
    Bio : string
}
[sql_table(name = "Accounts")]
struct Account {
    @sql_primary_key Id : int
    @sql_json Profile : InnerProfile
}

[test]
def test_sql_text_json_descent_tuple_field(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>) |> _where(_.Prefs.theme == "dark"))
    t |> success(sql |> find("json_extract(\"Prefs\", '$.theme') = ?") >= 0,
        "WHERE on @sql_json tuple field emits json_extract with $.theme")
}

[test]
def test_sql_text_json_descent_bool_subfield(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>) |> _where(_.Prefs.notify == true))
    t |> success(sql |> find("json_extract(\"Prefs\", '$.notify') = ?") >= 0,
        "WHERE on @sql_json bool subfield emits json_extract with $.notify")
}

[test]
def test_sql_text_json_descent_nested_struct_path(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Account>) |> _where(_.Profile.Addr.City == "NY"))
    t |> success(sql |> find("json_extract(\"Profile\", '$.Addr.City') = ?") >= 0,
        "WHERE on nested @sql_json struct path emits dotted json_extract path")
}

// End-to-end — round-trip the data, query through the JSON path, confirm SQLite
// actually evaluates the json_extract emission.
[test]
def test_sql_json_descent_query_filters_on_subfield(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        db |> insert(User(Id = 1, Name = "alice",
            Prefs = (theme = "dark", lang = "en", notify = true)))
        db |> insert(User(Id = 2, Name = "bob",
            Prefs = (theme = "light", lang = "ru", notify = false)))
        db |> insert(User(Id = 3, Name = "carol",
            Prefs = (theme = "dark", lang = "fr", notify = false)))

        let dark_users <- _sql(db |> select_from(type<User>)
            |> _where(_.Prefs.theme == "dark"))
        t |> equal(length(dark_users), 2, "two users with theme=dark")
        t |> success(dark_users[0].Name == "alice" || dark_users[0].Name == "carol",
            "first dark-theme row is alice or carol")
    }
}

[test]
def test_sql_json_descent_query_nested_path(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Account>)
        var p1 : InnerProfile
        p1.Addr.City := "NY"
        p1.Addr.Zip := "10001"
        p1.Bio := "alice in NY"
        var p2 : InnerProfile
        p2.Addr.City := "SF"
        p2.Addr.Zip := "94102"
        p2.Bio := "bob in SF"
        db |> insert(Account(Id = 1, Profile <- p1))
        db |> insert(Account(Id = 2, Profile <- p2))

        let ny <- _sql(db |> select_from(type<Account>)
            |> _where(_.Profile.Addr.City == "NY"))
        t |> equal(length(ny), 1, "one account in NY via nested JSON path")
        t |> equal(ny[0].Id, 1, "alice's account id")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Projection-side JSON-path descent (B4-followup).
//
// `_select(_.<json_col>.<path…>)` lowers to `SELECT json_extract(...)` —
// mirror of the predicate-side recognizer. Works for single-column,
// nested-struct paths, named-tuple projections, and mixed
// (plain-column + JSON-path) named tuples. Result row type is resolved
// from the daslang struct/tuple definition at the leaf.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_sql_text_projection_descent_tuple_field(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>) |> _select(_.Prefs.theme))
    t |> equal(sql,
        "SELECT json_extract(\"Prefs\", '$.theme') FROM \"Users\"",
        "_select on @sql_json tuple field emits json_extract")
}

[test]
def test_sql_text_projection_descent_nested_struct_path(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Account>) |> _select(_.Profile.Addr.City))
    t |> equal(sql,
        "SELECT json_extract(\"Profile\", '$.Addr.City') FROM \"Accounts\"",
        "_select on nested @sql_json struct path emits dotted json_extract")
}

[test]
def test_sql_text_projection_named_tuple_two_json_paths(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
        |> _select((Theme = _.Prefs.theme, Lang = _.Prefs.lang)))
    t |> equal(sql,
        "SELECT json_extract(\"Prefs\", '$.theme'), json_extract(\"Prefs\", '$.lang') FROM \"Users\"",
        "named-tuple projection with two JSON paths emits two json_extracts")
}

[test]
def test_sql_text_projection_mixed_plain_and_json_path(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
        |> _select((N = _.Name, T = _.Prefs.theme)))
    t |> equal(sql,
        "SELECT \"Name\", json_extract(\"Prefs\", '$.theme') FROM \"Users\"",
        "named-tuple mixing plain column and JSON path emits both shapes")
}

[test]
def test_projection_descent_returns_string_array(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        db |> insert(User(Id = 1, Name = "alice",
            Prefs = (theme = "dark", lang = "en", notify = true)))
        db |> insert(User(Id = 2, Name = "bob",
            Prefs = (theme = "light", lang = "ru", notify = false)))
        db |> insert(User(Id = 3, Name = "carol",
            Prefs = (theme = "dark", lang = "fr", notify = false)))

        let themes <- _sql(db |> select_from(type<User>) |> _select(_.Prefs.theme))
        t |> equal(length(themes), 3, "three theme values")
        t |> equal(themes[0], "dark", "first user's theme via JSON projection")
        t |> equal(themes[1], "light", "second user's theme via JSON projection")
        t |> equal(themes[2], "dark", "third user's theme via JSON projection")
    }
}

[test]
def test_projection_descent_returns_bool_array(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        db |> insert(User(Id = 1, Name = "alice",
            Prefs = (theme = "dark", lang = "en", notify = true)))
        db |> insert(User(Id = 2, Name = "bob",
            Prefs = (theme = "light", lang = "ru", notify = false)))

        let notify_flags <- _sql(db |> select_from(type<User>) |> _select(_.Prefs.notify))
        t |> equal(length(notify_flags), 2, "two notify flags")
        t |> equal(notify_flags[0], true, "alice notify=true via JSON projection")
        t |> equal(notify_flags[1], false, "bob notify=false via JSON projection")
    }
}

[test]
def test_projection_descent_nested_struct_path_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Account>)
        var p1 : InnerProfile
        p1.Addr.City := "NY"
        p1.Addr.Zip := "10001"
        p1.Bio := "alice in NY"
        var p2 : InnerProfile
        p2.Addr.City := "SF"
        p2.Addr.Zip := "94102"
        p2.Bio := "bob in SF"
        db |> insert(Account(Id = 1, Profile <- p1))
        db |> insert(Account(Id = 2, Profile <- p2))

        let cities <- _sql(db |> select_from(type<Account>) |> _select(_.Profile.Addr.City))
        t |> equal(length(cities), 2, "two cities")
        t |> equal(cities[0], "NY", "alice's city via nested JSON path projection")
        t |> equal(cities[1], "SF", "bob's city via nested JSON path projection")
    }
}

[test]
def test_projection_named_tuple_mixed_plain_and_json_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        db |> insert(User(Id = 1, Name = "alice",
            Prefs = (theme = "dark", lang = "en", notify = true)))
        db |> insert(User(Id = 2, Name = "bob",
            Prefs = (theme = "light", lang = "ru", notify = false)))

        let rows <- _sql(db |> select_from(type<User>)
            |> _select((N = _.Name, T = _.Prefs.theme)))
        t |> equal(length(rows), 2, "two rows from mixed projection")
        t |> equal(rows[0].N, "alice", "row 0 plain Name")
        t |> equal(rows[0].T, "dark", "row 0 JSON-extracted theme")
        t |> equal(rows[1].N, "bob", "row 1 plain Name")
        t |> equal(rows[1].T, "light", "row 1 JSON-extracted theme")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Malformed JSON in a stored @sql_json column panics on extract — symmetric
// with @sql_blob's `mem_archive_load` panic on corrupt bytes. Without this
// guard a row written by another tool / migration / hand-edited DB would
// silently materialize as `default<T>` and mask data loss.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_sql_json_malformed_panics_on_extract(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        // Stuff a non-JSON byte string into the Prefs column directly,
        // bypassing the [sql_table] insert path.
        db |> exec("INSERT INTO Users (Id, Name, Prefs) VALUES (1, 'alice', 'not json')")
        var caught = false
        try {
            let _rows <- db |> select_from(type<User>)
        } recover {
            caught = true
        }
        t |> success(caught, "select_from on row with malformed JSON column panics")
    }
}

// ===== @sql_json + Option<UserStruct> =====
// Regression for the runtime/contract bug pair on @sql_json over Option<>: the runtime
// `sql_bind_to_stmt(Option<T>)` overload strips the Option BEFORE calling user adapters,
// so `emit_json_blob_adapters_for_struct` must register `sql_bind` for the bare T,
// not for `Option<T>`. Pairs with the generic `sql_bind($Option<auto(TT)>)` fallback so
// the macro witness check classifies as TEXT.

struct OptJsonProfile {
    handle : string
    score : int
}

[sql_table(name = "OptUsers")]
struct OptJsonUser {
    @sql_primary_key Id : int
    @sql_json Profile : Option<OptJsonProfile>
}

[test]
def test_sql_json_option_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<OptJsonUser>)
        db |> insert(OptJsonUser(Id = 1, Profile = some(OptJsonProfile(handle = "alice", score = 11))))
        db |> insert(OptJsonUser(Id = 2, Profile = none(type<OptJsonProfile>)))
        let rows <- db |> select_from(type<OptJsonUser>) |> _sql() |> _order_by(_.Id)
        t |> equal(length(rows), 2, "two rows")
        t |> equal(rows[0].Profile |> is_some, true, "row 0 profile present")
        t |> equal((rows[0].Profile |> unwrap).handle, "alice", "json handle round-trips")
        t |> equal((rows[0].Profile |> unwrap).score, 11, "json score round-trips")
        t |> equal(rows[1].Profile |> is_none, true, "row 1 NULL extracts as none")
    }
}

[test]
def test_sql_json_option_classifies_as_text(t : T?) {
    let cols <- column_info(type<OptJsonUser>)
    t |> equal(length(cols), 2, "Id + Profile")
    t |> equal(cols[1].name, "Profile", "Profile column")
    t |> equal(cols[1].is_nullable, true, "Option<UserStruct> ↔ nullable")
    t |> equal(cols[1].data_type, SqlType.Text, "@sql_json on Option<T> classifies as TEXT")
}

// Adapter dedup edge case: two fields, one bare X and one Option<X>, both @sql_json.
// After Option-unwrap, both key as X — second field hits the dedup-skip path (no
// duplicate sql_bind(X) registration; no kind-collision error).

[sql_table(name = "MixedJsonRows")]
struct MixedJsonRow {
    @sql_primary_key Id : int
    @sql_json Required : OptJsonProfile
    @sql_json Optional : Option<OptJsonProfile>
}

[test]
def test_sql_json_dedup_bare_and_option_same_type(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<MixedJsonRow>)
        db |> insert(MixedJsonRow(Id = 1,
            Required = OptJsonProfile(handle = "r1", score = 1),
            Optional = some(OptJsonProfile(handle = "o1", score = 2))))
        db |> insert(MixedJsonRow(Id = 2,
            Required = OptJsonProfile(handle = "r2", score = 3),
            Optional = none(type<OptJsonProfile>)))
        let rows <- db |> select_from(type<MixedJsonRow>) |> _sql() |> _order_by(_.Id)
        t |> equal(length(rows), 2, "two rows")
        t |> equal(rows[0].Required.handle, "r1", "bare X round-trips")
        t |> equal((rows[0].Optional |> unwrap).handle, "o1", "Option<X> some round-trips")
        t |> equal(rows[1].Optional |> is_none, true, "Option<X> none → NULL → none")
    }
}
