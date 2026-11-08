options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate
require daslib/option public

[sql_table(name = "users", legacy = true)]
struct UserV1 {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

[struct_convert]
def my_v1_to_v2(old : UserV1; var dst : User) {
    pass
}

[test(description = "struct_convert between identical structs copies every row and field verbatim")]
def test_trivial_round_trip(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Score INTEGER NOT NULL DEFAULT 0)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (1, 'alice', 42), (2, 'bob', 7)")
    db |> convert_and_rename(type<UserV1>, type<User>)
    let n = db |> query_scalar("SELECT COUNT(*) FROM users", type<int>)
    t |> equal(n, 2, "row count preserved")
    let alice_score = db |> query_scalar("SELECT Score FROM users WHERE Name='alice'", type<int>)
    t |> equal(alice_score, 42, "alice's Score copied verbatim")
    let bob_score = db |> query_scalar("SELECT Score FROM users WHERE Name='bob'", type<int>)
    t |> equal(bob_score, 7, "bob's Score copied verbatim")
}

// T → Option<T>: source has bare Score, target has Option<Score>. Auto-rule wraps with some().
[sql_table(name = "users", legacy = true)]
struct UserV1_81 {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

[sql_table(name = "users")]
struct User_81 {
    @sql_primary_key Id : int
    Name : string
    Score : Option<int>
}

[struct_convert]
def my_v1_to_v2_81(old : UserV1_81; var dst : User_81) {
    pass
}

[test(description = "struct_convert wraps a plain value into some when the target field is an option")]
def test_t_to_option_wraps_with_some(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Score INTEGER NOT NULL DEFAULT 0)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (1, 'alice', 42)")
    db |> convert_and_rename(type<UserV1_81>, type<User_81>)
    var rows : array<User_81>
    rows |> reserve(2)
    for (row in db |> select_from(type<User_81>)) {
        rows |> emplace(row)
    }
    t |> equal(length(rows), 1, "row preserved")
    t |> success(rows[0].Score |> is_some, "Option<int> wraps to some(...)")
    t |> equal(rows[0].Score |> unwrap, 42, "wrapped value preserved")
}

// [struct_convert] dispatches to overloaded ``struct_convert_field``. This test exercises the
// numeric/string conversion overloads — int↔int64, float↔double, string→numeric, numeric→string.

[sql_table(name = "items", legacy = true)]
struct ItemV1 {
    @sql_primary_key Id : int
    Count : int          // → int64 (widening)
    Score : int64        // → int   (narrowing — silent, user opted in)
    Ratio : float        // → double (widening)
    Avg : double         // → float (narrowing)
    LabelInt : int       // → string (numeric → string)
    PriceStr : string    // → double (string → double parse)
}

[sql_table(name = "items")]
struct Item {
    @sql_primary_key Id : int
    Count : int64
    Score : int
    Ratio : double
    Avg : float
    LabelInt : string
    PriceStr : double
}

[struct_convert]
def convert_item_v1_to_v2(old : ItemV1; var dst : Item) {}

[test(description = "struct_convert casts between int, int64, float, double and string fields")]
def test_primitive_casts_through_dispatch(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE items (Id INTEGER PRIMARY KEY, Count INTEGER NOT NULL, Score INTEGER NOT NULL, Ratio REAL NOT NULL, Avg REAL NOT NULL, LabelInt INTEGER NOT NULL, PriceStr TEXT NOT NULL)")
    db |> exec("INSERT INTO items (Id, Count, Score, Ratio, Avg, LabelInt, PriceStr) VALUES (1, 42, 9999, 1.5, 2.75, 7, '3.14')")
    db |> convert_and_rename(type<ItemV1>, type<Item>)
    let count_int64 = db |> query_scalar("SELECT Count FROM items WHERE Id=1", type<int64>)
    t |> equal(count_int64, 42l, "int → int64 widening preserved")
    let score_int = db |> query_scalar("SELECT Score FROM items WHERE Id=1", type<int>)
    t |> equal(score_int, 9999, "int64 → int narrowing preserved (within range)")
    let ratio_double = db |> query_scalar("SELECT Ratio FROM items WHERE Id=1", type<double>)
    t |> equal(ratio_double, 1.5lf, "float → double widening preserved")
    let avg_float = db |> query_scalar("SELECT Avg FROM items WHERE Id=1", type<float>)
    t |> equal(avg_float, 2.75f, "double → float narrowing preserved")
    let label_str = db |> query_scalar("SELECT LabelInt FROM items WHERE Id=1", type<string>)
    t |> equal(label_str, "7", "int → string interp")
    let price_double = db |> query_scalar("SELECT PriceStr FROM items WHERE Id=1", type<double>)
    t |> equal(price_double, 3.14lf, "string → double parse")
}

// `Option<T> → T` dispatches through the shipped ``struct_convert_field`` overload —
// NULL collapses to ``default<T>`` (0 / "" / 0.0). Override in your module for different
// NULL-handling semantics.

[sql_table(name = "users", legacy = true)]
struct UserV1_83 {
    @sql_primary_key Id : int
    Name : Option<string>
    Score : Option<int>
}

[struct_convert]
def m_v1_to_v2(old : UserV1_83; var dst : User) {}

[test(description = "struct_convert unwraps an option to its value and uses the type default for NULL")]
def test_option_unwrap_default_on_null(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT, Score INTEGER)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (1, 'alice', 42)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (2, NULL, NULL)")
    db |> convert_and_rename(type<UserV1_83>, type<User>)
    let alice_name = db |> query_scalar("SELECT Name FROM users WHERE Id=1", type<string>)
    let alice_score = db |> query_scalar("SELECT Score FROM users WHERE Id=1", type<int>)
    t |> equal(alice_name, "alice", "Some(string) → string preserved")
    t |> equal(alice_score, 42, "Some(int) → int preserved")
    let bob_name = db |> query_scalar("SELECT Name FROM users WHERE Id=2", type<string>)
    let bob_score = db |> query_scalar("SELECT Score FROM users WHERE Id=2", type<int>)
    t |> equal(bob_name, "", "NULL string → default<string>=\"\"")
    t |> equal(bob_score, 0, "NULL int → default<int>=0")
}

// ``Option<S> → Option<T>`` cross-payload — the macro emits ``struct_convert_field(dst, src)``
// and the cross-payload Option overload unwraps src, dispatches S→T inner, re-wraps.

[sql_table(name = "users", legacy = true)]
struct UserV1_84 {
    @sql_primary_key Id : int
    Score : Option<int>
}

[sql_table(name = "users")]
struct User_84 {
    @sql_primary_key Id : int
    Score : Option<string>
}

[struct_convert]
def m_v1_to_v2_84(old : UserV1_84; var dst : User_84) {}

[test(description = "struct_convert converts the payload of an option and keeps NULL as NULL")]
def test_option_cross_payload(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Score INTEGER)")
    db |> exec("INSERT INTO users (Id, Score) VALUES (1, 42), (2, NULL)")
    db |> convert_and_rename(type<UserV1_84>, type<User_84>)
    let alice = db |> query_scalar("SELECT Score FROM users WHERE Id=1", type<string>)
    t |> equal(alice, "42", "Option<int>(some) → Option<string>(some) via int→string inner")
    // SQLite returns "" for NULL when target is non-nullable string column; with Option<string>
    // target the rebuild stores actual NULL, but query_scalar collapses to default<string>.
    let n_null = db |> query_scalar(
        "SELECT COUNT(*) FROM users WHERE Id=2 AND Score IS NULL",
        type<int>)
    t |> equal(n_null, 1, "Option<int>(none) → Option<string>(none) preserved as NULL")
}

// Read-only mentions of `dst.X` (e.g. `if (dst.X == "")`) must NOT suppress the
// auto-derived `dst.X = old.X`. A previous broader visitor catching ANY ExprField
// under `dst` would silently leave dst.X at default — copying produced empty rows
// instead of the source data.

[sql_table(name = "users", legacy = true)]
struct OldUser {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[sql_table(name = "users")]
struct NewUser {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[struct_convert]
def convert_readonly_mention(old : OldUser; var dst : NewUser) {
    // Read-only mention of dst.Name — auto-fill MUST still emit `dst.Name = old.Name`.
    if (dst.Name == "") {
        // Path is unreachable in practice; the conditional exists to anchor the read mention.
    }
    // Read-only mention of dst.Email used as RHS — same: auto-fill MUST still copy.
    let _peek = dst.Email
}

[test(description = "reading a field in the convert body does not stop its automatic copy")]
def test_readonly_mentions_preserve_auto_fill(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Email TEXT NOT NULL)")
    db |> exec("INSERT INTO users (Id, Name, Email) VALUES (1, 'alice', 'alice@x')")
    db |> convert_and_rename(type<OldUser>, type<NewUser>)
    let landed_name = db |> query_scalar("SELECT Name FROM users WHERE Id=1", type<string>)
    let landed_email = db |> query_scalar("SELECT Email FROM users WHERE Id=1", type<string>)
    t |> equal(landed_name, "alice", "Name preserved (read-only mention did NOT suppress auto-copy)")
    t |> equal(landed_email, "alice@x", "Email preserved (read-only RHS mention did NOT suppress auto-copy)")
}

// Override-detection records `dst.X` ONLY when it appears on the LHS of an
// assignment (`=`, `<-`, `:=`). The visitor sees `dst.Score = …` and suppresses
// the auto-derive that would otherwise macro_error on `Option<int> → int`
// (existing rows may be NULL).

[sql_table(name = "users", legacy = true)]
struct UserV1_87 {
    @sql_primary_key Id : int
    Name : string
    Score : Option<int>
}

[struct_convert]
def my_v1_to_v2_87(old : UserV1_87; var dst : User) {
    // LHS write recorded; auto-derive suppressed; user expression resolves the Option.
    dst.Score = old.Score ?? 0
}

[test(description = "assigning a target field in the convert body replaces its automatic derivation")]
def test_lhs_write_suppresses_auto_derive(t : T?) {
    // The compile passing IS the test — without LHS-detection [struct_convert]
    // would macro_error on Option<T> → T. With it, the user's body resolves the
    // Option and the macro stays out of the way.
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Score INTEGER)")
    db |> exec("INSERT INTO users (Id, Name, Score) VALUES (1, 'alice', 42)")
    db |> convert_and_rename(type<UserV1_87>, type<User>)
    let alice_score = db |> query_scalar("SELECT Score FROM users WHERE Name='alice'", type<int>)
    t |> equal(alice_score, 42, "alice's some(42) unwrapped via the body's resolution")
}

[sql_table(name = "users"), sql_index(fields = "Name")]
struct User_88 {
    @sql_primary_key Id : int
    Name : string
}

[test(description = "create_table with a name override creates only the alternate table")]
def test_create_table_emits_with_alternate_name(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<User_88>, "users_new")
    let count = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users_new'", type<int>)
    t |> equal(count, 1, "users_new table created via name override")
    let original_count = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users'", type<int>)
    t |> equal(original_count, 0, "original 'users' table not auto-created")
}

[test(description = "create_table with a name override emits the full schema including primary key and NOT NULL")]
def test_create_table_emits_full_schema(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<User_88>, "users_new")
    let pk = db |> query_scalar(
        "SELECT pk FROM pragma_table_info('users_new') WHERE name='Id'", type<int>)
    t |> equal(pk, 1, "Id column is PRIMARY KEY in users_new")
    let notnull = db |> query_scalar(
        "SELECT \"notnull\" FROM pragma_table_info('users_new') WHERE name='Name'", type<int>)
    t |> equal(notnull, 1, "Name column is NOT NULL")
}

[test(description = "create_table with a name override creates its indexes on the alternate table")]
def test_create_table_emits_indexes_with_runtime_name(t : T?) {
    // [sql_index] auto-name uses the runtime table name, so indexes for the staging table
    // are named `idx_users_new_<col>` — no collision with original table's `idx_users_<col>`.
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<User_88>, "users_new")
    let idx = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND tbl_name='users_new' AND name='idx_users_new_Name'", type<int>)
    t |> equal(idx, 1, "auto-named index targets the staging table")
}

[sql_table(name = "users")]
struct User_89 {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[test(description = "insert with a name override writes into the alternate table")]
def test_insert_lands_in_named_table(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<User_89>, "users_new")
    let u = User_89(Name = "alice", Email = "alice@x")
    let id = db |> insert(u, "users_new")
    t |> success(id > 0l, "insert returned a row id")
    let n = db |> query_scalar("SELECT COUNT(*) FROM users_new", type<int>)
    t |> equal(n, 1, "row landed in users_new, not users")
    let landed_email = db |> query_scalar(
        "SELECT Email FROM users_new WHERE Name='alice'", type<string>)
    t |> equal(landed_email, "alice@x", "row content preserved")
}

[sql_table(name = "users", legacy = true)]
struct UserV1_90 {
    @sql_primary_key Id : int
    Name : string
    LegacyEmail : string
}

[sql_table(name = "users")]
struct User_90 {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[test(description = "a legacy-marked struct can be read with select_from")]
def test_legacy_struct_select_from_works(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, LegacyEmail TEXT NOT NULL DEFAULT '')")
    db |> exec("INSERT INTO users (Id, Name, LegacyEmail) VALUES (1, 'alice', 'alice@x')")
    var rows : array<UserV1_90>
    rows |> reserve(2)
    for (row in db |> select_from(type<UserV1_90>)) {
        rows |> emplace(row)
    }
    t |> equal(length(rows), 1, "select_from on legacy struct returns rows")
    t |> equal(rows[0].LegacyEmail, "alice@x", "legacy field readable")
}

[test(description = "drop_table_if_exists works on a legacy-marked struct")]
def test_legacy_struct_drop_works(t : T?) {
    // DROP TABLE IF EXISTS is read-side-enough — devs may legitimately drop a legacy table
    // during migration cleanup. Tests that the helper is emitted for legacy structs.
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, LegacyEmail TEXT NOT NULL DEFAULT '')")
    db |> drop_table_if_exists(type<UserV1_90>)
    let n = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users'", type<int>)
    t |> equal(n, 0, "drop_table_if_exists works on legacy struct")
}

// Identifier injection guard: every runtime ``name : string`` overload
// (create_table / insert / select_from / drop / update / delete + the
// migrate path) must route through sql_quote_id so embedded `"` is
// doubled. Without escaping, a `"` in name would close the identifier
// and execute attacker-controlled SQL through sqlite3_exec.

[sql_table(name = "users")]
struct User_92 {
    @sql_primary_key Id : int
    Name : string
}

[test(description = "a hostile table name is quoted as an identifier and injects nothing")]
def test_hostile_name_does_not_inject(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let hostile = "evil\" (Injected INTEGER); CREATE TABLE hacked(z TEXT); --"
    db |> create_table(type<User_92>, hostile)
    let landed = User_92(Name = "alice")
    let id = db |> insert(landed, hostile)
    t |> success(id > 0l, "insert into hostile-named table succeeded")
    let n_landed = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name={sql_quote_lit(hostile)}",
        type<int>)
    t |> equal(n_landed, 1, "hostile name landed verbatim as a table identifier")
    let n_hacked = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='hacked'",
        type<int>)
    t |> equal(n_hacked, 0, "no injected `hacked` table was created")
}

// _qualify_schema must rewrite ALTER TABLE so that convert_and_rename
// rebuilds run inside an attached schema, not silently against `main`.

[sql_table(name = "users", legacy = true)]
struct UserV1_93 {
    @sql_primary_key Id : int
    Name : string
    LegacyEmail : string
}

[sql_table(name = "users")]
struct User_93 {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[struct_convert]
def convert_v1_to_v2(old : UserV1_93; var dst : User_93) {
    dst.Email = (old.LegacyEmail != "") ? old.LegacyEmail : "unknown@example.com"
}

[test(description = "a table rebuild runs in the attached schema and leaves main untouched")]
def test_rebuild_runs_in_attached_schema(t : T?) {
    var inscope main = open_sqlite(":memory:")
    main |> with_attached(":memory:", "att") $(att) {
        att |> exec("DROP TABLE IF EXISTS \"users\"")
        att |> exec("DROP TABLE IF EXISTS \"users_new\"")
        att |> exec("CREATE TABLE \"users\" (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, LegacyEmail TEXT NOT NULL DEFAULT '')")
        att |> exec("INSERT INTO \"users\" (Id, Name, LegacyEmail) VALUES (1, 'alice', 'alice@x')")
        att |> convert_and_rename(type<UserV1_93>, type<User_93>)
        let n_in_att = main |> query_scalar(
            "SELECT COUNT(*) FROM \"att\".sqlite_master WHERE type='table' AND name='users'",
            type<int>)
        t |> equal(n_in_att, 1, "rebuilt `users` lives in attached schema")
        let main_users = main |> query_scalar(
            "SELECT COUNT(*) FROM main.sqlite_master WHERE type='table' AND name='users'",
            type<int>)
        t |> equal(main_users, 0, "no rebuild side-effect in main")
        let landed = att |> query_scalar(
            "SELECT Email FROM users WHERE Id=1", type<string>)
        t |> equal(landed, "alice@x", "row content preserved through rebuild")
    }
}

// `[sql_index(name = "...")]` (explicit custom name) must survive convert_and_rename.
// Earlier rebuild path created the staging table WITH indexes, which collided on the
// custom name — SQLite index names are schema-global and the original still owned it.
// Fixed by deferring index creation to after the RENAME.

[sql_table(name = "users", legacy = true)]
struct UserV1_94 {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[sql_table(name = "users"),
 sql_index(name = "ux_users_name", fields = "Name", unique)]

struct User_94 {
    @sql_primary_key Id : int
    Name : string
    Email : string
}

[struct_convert]
def convert_v1_to_v2_94(old : UserV1_94; var dst : User_94) {}

[test(description = "a custom-named unique index exists after a table rebuild and no staging table is left")]
def test_custom_named_index_survives_rebuild(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Email TEXT NOT NULL DEFAULT '')")
    db |> exec("CREATE UNIQUE INDEX ux_users_name ON users (Name)")
    db |> exec("INSERT INTO users (Id, Name, Email) VALUES (1, 'alice', 'alice@x')")
    db |> convert_and_rename(type<UserV1_94>, type<User_94>)
    let n_idx = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND name='ux_users_name'",
        type<int>)
    t |> equal(n_idx, 1, "custom-named unique index lives after rebuild")
    let n_tab = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='users'",
        type<int>)
    t |> equal(n_tab, 1, "renamed `users` table is the only one (no `users_new` left)")
    let landed = db |> query_scalar("SELECT Name FROM users WHERE Id=1", type<string>)
    t |> equal(landed, "alice", "row preserved through rebuild")
}

// `convert_and_rename` requires source and target structs to share the same
// ``[sql_table(name=...)]`` (legacy/current convention). Mismatched names would
// silently SELECT from the source table but DROP/RENAME the target — a footgun.
// The runtime guard panics with a clear message pointing at the proper escape
// hatch (``convert(type<S>, type<T>, name=...)``) for cross-table conversions.

[sql_table(name = "items_old", legacy = true)]
struct ItemV1_95 {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "items")]
struct Item_95 {
    @sql_primary_key Id : int
    Name : string
}

[struct_convert]
def m_v1_to_v2_95(old : ItemV1_95; var dst : Item_95) {}

[test(description = "convert_and_rename panics when source and target sql_table names disagree")]
def test_convert_and_rename_panics_on_mismatched_table_names(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE items_old (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
    var panicked = false
    try {
        db |> convert_and_rename(type<ItemV1_95>, type<Item_95>)
    } recover {
        panicked = true
    }
    t |> success(panicked,
        "convert_and_rename must panic when source/target [sql_table(name=...)] disagree")
}
