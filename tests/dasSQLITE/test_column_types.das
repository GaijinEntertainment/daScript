options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

// Tutorial 26 — sql_bind / sql_extract adapter rail. Tests cover:
//   - DateTime via INTEGER (default storage choice — 64-bit unix seconds)
//   - Guid via BLOB (16 raw bytes)
//   - Domain enum via INTEGER (auto-handled by dasSQLITE's enum generic)
//   - Standard-type widenings (int → int64, float → double, bool → int64)
//   - Option<T> over a custom adapter — NULL on the bind/read sides
//   - DDL column types come from sql_bind's RETURN type
//   - _sql(... where _.EnumCol == EnumValue) translates to bind param

struct DateTime {
    unix_seconds : int64
}

def sql_bind(dt : DateTime) : int64 => dt.unix_seconds

[unused_argument(t)]
def sql_extract(v : int64; t : type<DateTime>) : DateTime => DateTime(unix_seconds = v)

struct Guid {
    @safe_when_uninitialized bytes : array<uint8>
}

def sql_bind(g : Guid) : array<uint8> {
    var copy : array<uint8>
    copy := g.bytes
    return <- copy
}

[unused_argument(t)]
def sql_extract(var v : array<uint8>; t : type<Guid>) : Guid => Guid(bytes <- v)

enum OrderStatus {
    Pending
    Paid
    Shipped
    Cancelled
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    ExternalId : Guid           // BLOB
    PlacedAt   : DateTime       // INTEGER
    Status     : OrderStatus    // INTEGER (enum generic)
    Total      : float          // REAL (float → double widening)
}

[sql_table(name = "Events")]
struct Event {
    @sql_primary_key Id : int
    At                                : DateTime
    @safe_when_uninitialized StartsAt : Option<DateTime>
}

def make_guid_filled(b : uint8) : Guid {
    var bytes : array<uint8>
    bytes |> resize(16)
    for (i in range(16)) {
        bytes[i] = b
    }
    return Guid(bytes <- bytes)
}

[test(description = "the DDL column type follows what each type sql_bind adapter returns")]
def test_ddl_picks_storage_type_per_adapter(t : T?) {
    let ddl = _sql_create_table_sql(type<Order>)
    t |> success(ddl |> find("\"ExternalId\" BLOB") >= 0, "Guid (sql_bind→array<uint8>) → BLOB column")
    t |> success(ddl |> find("\"PlacedAt\" INTEGER") >= 0, "DateTime (sql_bind→int64) → INTEGER column")
    t |> success(ddl |> find("\"Status\" INTEGER") >= 0, "enum (generic sql_bind→int64) → INTEGER column")
    t |> success(ddl |> find("\"Total\" REAL") >= 0, "float (widening sql_bind→double) → REAL column")
    t |> success(ddl |> find("\"Id\" INTEGER PRIMARY KEY") >= 0, "int PK (widening sql_bind→int64) → INTEGER PRIMARY KEY")
}

[test(description = "a custom DateTime type is stored as int64 seconds and read back through its sql_extract")]
def test_datetime_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Order>)
        db |> insert(Order(
            Id = 1,
            ExternalId = make_guid_filled(uint8(0)),
            PlacedAt = DateTime(unix_seconds = 1713000000l),
            Status = OrderStatus.Pending,
            Total = 0.0
        ))
        let stored = db |> query_scalar("SELECT PlacedAt FROM Orders WHERE Id = 1", type<int64>)
        t |> equal(stored, 1713000000l, "DateTime stored as int64 unix seconds")
        let rows <- db |> select_from(type<Order>)
        t |> equal(rows[0].PlacedAt.unix_seconds, 1713000000l, "DateTime round-trips through sql_extract")
    }
}

[test(description = "a custom Guid type is stored as a 16-byte BLOB and read back intact")]
def test_guid_blob_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Order>)
        db |> insert(Order(
            Id = 1,
            ExternalId = make_guid_filled(uint8(0xAB)),
            PlacedAt = DateTime(unix_seconds = 0l),
            Status = OrderStatus.Pending,
            Total = 0.0
        ))
        let rows <- db |> select_from(type<Order>)
        t |> equal(length(rows), 1, "one row inserted + read back")
        t |> equal(length(rows[0].ExternalId.bytes), 16, "Guid round-tripped 16 bytes")
        t |> equal(int(rows[0].ExternalId.bytes[0]), int(0xAB), "first byte preserved")
        t |> equal(int(rows[0].ExternalId.bytes[15]), int(0xAB), "last byte preserved")
    }
}

[test(description = "an enum is stored as its integer ordinal and read back as the enum")]
def test_enum_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Order>)
        db |> insert(Order(
            Id = 1, ExternalId = make_guid_filled(uint8(0)),
            PlacedAt = DateTime(unix_seconds = 0l),
            Status = OrderStatus.Shipped, Total = 0.0
        ))
        let rows <- db |> select_from(type<Order>)
        t |> equal(int(rows[0].Status), int(OrderStatus.Shipped), "enum round-trips through INTEGER")
        let stored = db |> query_scalar("SELECT Status FROM Orders WHERE Id = 1", type<int64>)
        t |> equal(stored, int64(OrderStatus.Shipped), "enum stored as int64 ordinal")
    }
}

[test(description = "an enum value compares correctly in a where predicate")]
def test_enum_in_where_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Order>)
        db |> insert(Order(
            Id = 1, ExternalId = make_guid_filled(uint8(0)),
            PlacedAt = DateTime(unix_seconds = 0l),
            Status = OrderStatus.Paid, Total = 0.0
        ))
        db |> insert(Order(
            Id = 2, ExternalId = make_guid_filled(uint8(0)),
            PlacedAt = DateTime(unix_seconds = 0l),
            Status = OrderStatus.Cancelled, Total = 0.0
        ))
        let paid <- _sql(db |> select_from(type<Order>) |> _where(_.Status == OrderStatus.Paid))
        t |> equal(length(paid), 1, "WHERE Status = OrderStatus.Paid matches one row")
        t |> equal(paid[0].Id, 1, "matched the right row")
    }
}

[test(description = "an option of a custom adapter type binds none as NULL and some as the adapted value")]
def test_option_over_custom_adapter(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Event>)
        db |> insert(Event(Id = 1, At = DateTime(unix_seconds = 100l), StartsAt = none(type<DateTime>)))
        db |> insert(Event(Id = 2, At = DateTime(unix_seconds = 200l), StartsAt = some(DateTime(unix_seconds = 300l))))

        let r1 = db |> query_one("SELECT * FROM Events WHERE Id = 1", type<Event>)
        t |> success(r1.StartsAt |> is_none(), "none() Option<DateTime> binds as NULL, reads as none")

        let r2 = db |> query_one("SELECT * FROM Events WHERE Id = 2", type<Event>)
        t |> success(r2.StartsAt |> is_some(), "some(...) Option<DateTime> binds and reads back as some")
        t |> equal(unwrap(r2.StartsAt).unix_seconds, 300l, "Option<DateTime> inner value preserved")
    }
}

[test(description = "query_scalar extracts custom and enum types through their adapters")]
def test_query_scalar_through_adapter(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Order>)
        db |> insert(Order(
            Id = 42, ExternalId = make_guid_filled(uint8(0)),
            PlacedAt = DateTime(unix_seconds = 1234567890l),
            Status = OrderStatus.Paid, Total = 99.5
        ))
        // query_scalar routes through read_via_adapter, so user adapters
        // work transparently in raw-SQL paths too.
        let dt = db |> query_scalar("SELECT PlacedAt FROM Orders WHERE Id = 42", type<DateTime>)
        t |> equal(dt.unix_seconds, 1234567890l, "DateTime extracted via custom adapter from raw SQL")
        let st = db |> query_scalar("SELECT Status FROM Orders WHERE Id = 42", type<OrderStatus>)
        t |> equal(int(st), int(OrderStatus.Paid), "enum extracted via generic adapter from raw SQL")
    }
}

// Tutorial 27 — BLOB column round-trip via array<uint8>.
//
// `array<uint8>` is one of the four storage primitives, so it works as
// a [sql_table] field type without user adapter code. Tests cover
// DDL emission, byte-level round-trip, large blob (SQLITE_TRANSIENT
// correctness across step), Option<array<uint8>> nullability, and
// inline byte-array predicates.

[sql_table(name = "Images")]
struct Image {
    @sql_primary_key Id : int
    Name : string
    @safe_when_uninitialized Data : array<uint8>
}

[sql_table(name = "Files")]
struct FileRow {
    @sql_primary_key Id : int
    @safe_when_uninitialized Body : Option<array<uint8>>
}

def make_bytes(n : int; pattern : uint8) : array<uint8> {
    var bytes : array<uint8>
    bytes |> resize(n)
    for (i in range(n)) {
        bytes[i] = uint8((int(pattern) + i) & int(0xff))
    }
    return <- bytes
}

[test(description = "an array of uint8 field declares a BLOB NOT NULL column")]
def test_blob_ddl_is_BLOB_NOT_NULL(t : T?) {
    let ddl = _sql_create_table_sql(type<Image>)
    t |> success(ddl |> find("\"Data\" BLOB NOT NULL") >= 0, "array<uint8> field DDL is BLOB NOT NULL")
}

[test(description = "a byte array round-trips through a BLOB column with every byte preserved")]
def test_blob_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Image>)
        var payload = make_bytes(64, uint8(0x10))
        let payload_len = length(payload)
        let payload_first = int(payload[0])
        let payload_last = int(payload[63])
        db |> insert(Image(Id = 1, Name = "thumb", Data <- payload))

        let rows <- db |> select_from(type<Image>)
        t |> equal(length(rows), 1, "one row")
        t |> equal(length(rows[0].Data), payload_len, "all bytes round-tripped")
        t |> equal(int(rows[0].Data[0]), payload_first, "first byte preserved")
        t |> equal(int(rows[0].Data[63]), payload_last, "last byte preserved")
    }
}

[test(description = "a 256 KB blob survives insert and select with its first, middle and last bytes intact")]
def test_large_blob_survives_step(t : T?) {
    //! 256 KB blob. Verifies SQLITE_TRANSIENT in sqlite3_bind_blob and the
    //! per-row copy in sqlite3_column_blob_ both behave correctly when the
    //! daslang heap allocations don't outlive the C-side buffer pointers.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Image>)
        let n = 256 * 1024
        var bytes = make_bytes(n, uint8(0x55))
        let expected_first = int(bytes[0])
        let expected_mid = int(bytes[n / 2])
        let expected_last = int(bytes[n - 1])
        db |> insert(Image(Id = 1, Name = "big", Data <- bytes))

        let rows <- db |> select_from(type<Image>)
        t |> equal(length(rows[0].Data), n, "256 KB blob length preserved")
        t |> equal(int(rows[0].Data[0]), expected_first, "byte 0 preserved")
        t |> equal(int(rows[0].Data[n / 2]), expected_mid, "midpoint byte preserved")
        t |> equal(int(rows[0].Data[n - 1]), expected_last, "last byte preserved")
    }
}

[test(description = "query_scalar reads a BLOB result as a byte array")]
def test_query_scalar_blob(t : T?) {
    //! Raw-SQL path goes through read_via_adapter just like [sql_table];
    //! query_scalar with type<array<uint8>> reads the BLOB column directly.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Image>)
        var bytes = make_bytes(8, uint8(0xA0))
        db |> insert(Image(Id = 1, Name = "scalar", Data <- bytes))

        let blob = db |> query_scalar("SELECT Data FROM Images WHERE Id = 1", type<array<uint8>>)
        t |> equal(length(blob), 8, "scalar BLOB read length")
        t |> equal(int(blob[0]), int(0xA0), "byte 0 via query_scalar")
    }
}

[test(description = "an optional BLOB column reads NULL as none and a value as some")]
def test_optional_blob_null_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<FileRow>)
        db |> insert(FileRow(Id = 1, Body = default<Option<array<uint8>>>))

        var bytes = make_bytes(4, uint8(0xCD))
        db |> insert(FileRow(Id = 2, Body = move_some(bytes)))

        let rows <- db |> select_from(type<FileRow>)
        t |> equal(length(rows), 2, "two rows")

        // Find by Id; iteration order is rowid here.
        for (r in rows) {
            if (r.Id == 1) {
                t |> success(r.Body |> is_none(), "NULL BLOB → none")
            } elif (r.Id == 2) {
                t |> success(r.Body |> is_some(), "non-NULL BLOB → some")
                t |> equal(length(unwrap(r.Body)), 4, "non-null blob length")
            }
        }
    }
}

[test(description = "an empty blob reads back as some of zero bytes and is distinct from NULL")]
def test_empty_blob_distinct_from_null(t : T?) {
    //! SQLite's BLOB literal vs NULL: a zero-length BLOB and NULL are
    //! distinct values. Verifies the bind/read path preserves the
    //! distinction (some(empty array) ≠ none).
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<FileRow>)
        var empty_bytes : array<uint8>
        db |> insert(FileRow(Id = 1, Body = move_some(empty_bytes)))
        db |> insert(FileRow(Id = 2, Body = default<Option<array<uint8>>>))

        let r1 = db |> query_one("SELECT * FROM Files WHERE Id = 1", type<FileRow>)
        t |> success(r1.Body |> is_some(), "empty blob is some, not none")
        t |> equal(length(unwrap(r1.Body)), 0, "empty blob has zero bytes")

        let r2 = db |> query_one("SELECT * FROM Files WHERE Id = 2", type<FileRow>)
        t |> success(r2.Body |> is_none(), "NULL blob is none")
    }
}

// @sql_json on a tuple — round-trip via daslib/json (TEXT storage, JV/from_JV).
[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    @sql_json Prefs : tuple<theme : string; lang : string; notify : bool>
}

[test(description = "a sql_json tuple column round-trips every field")]
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

[test(description = "a sql_json column is declared TEXT")]
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

[test(description = "a sql_blob struct column round-trips its strings, arrays and numbers")]
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

[test(description = "a sql_blob column is declared BLOB")]
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

[test(description = "one sql_json type used in two tables shares its adapter and round-trips in both")]
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

[test(description = "a where on a sql_json tuple field emits json_extract with the field path")]
def test_sql_text_json_descent_tuple_field(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>) |> _where(_.Prefs.theme == "dark"))
    t |> success(sql |> find("json_extract(\"Prefs\", '$.theme') = ?") >= 0,
        "WHERE on @sql_json tuple field emits json_extract with $.theme")
}

[test(description = "a where on a sql_json bool sub-field emits json_extract with the field path")]
def test_sql_text_json_descent_bool_subfield(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>) |> _where(_.Prefs.notify == true))
    t |> success(sql |> find("json_extract(\"Prefs\", '$.notify') = ?") >= 0,
        "WHERE on @sql_json bool subfield emits json_extract with $.notify")
}

[test(description = "a where on a nested sql_json struct path emits json_extract with the dotted path")]
def test_sql_text_json_descent_nested_struct_path(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Account>) |> _where(_.Profile.Addr.City == "NY"))
    t |> success(sql |> find("json_extract(\"Profile\", '$.Addr.City') = ?") >= 0,
        "WHERE on nested @sql_json struct path emits dotted json_extract path")
}

// End-to-end — round-trip the data, query through the JSON path, confirm SQLite
// actually evaluates the json_extract emission.
[test(description = "a where on a sql_json sub-field filters rows at runtime")]
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

[test(description = "a where on a nested sql_json path filters rows at runtime")]
def test_sql_json_descent_query_nested_path(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Account>)
        var p1 : InnerProfile
        p1.Addr.City = "NY"
        p1.Addr.Zip = "10001"
        p1.Bio = "alice in NY"
        var p2 : InnerProfile
        p2.Addr.City = "SF"
        p2.Addr.Zip = "94102"
        p2.Bio = "bob in SF"
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

[test(description = "a select of a sql_json tuple field emits json_extract of its path")]
def test_sql_text_projection_descent_tuple_field(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>) |> _select(_.Prefs.theme))
    t |> equal(sql,
        "SELECT json_extract(\"Prefs\", '$.theme') FROM \"Users\"",
        "_select on @sql_json tuple field emits json_extract")
}

[test(description = "a select of a nested sql_json path emits json_extract of the dotted path")]
def test_sql_text_projection_descent_nested_struct_path(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Account>) |> _select(_.Profile.Addr.City))
    t |> equal(sql,
        "SELECT json_extract(\"Profile\", '$.Addr.City') FROM \"Accounts\"",
        "_select on nested @sql_json struct path emits dotted json_extract")
}

[test(description = "a named-tuple select of two json paths emits two aliased json_extract columns")]
def test_sql_text_projection_named_tuple_two_json_paths(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
        |> _select((Theme = _.Prefs.theme, Lang = _.Prefs.lang)))
    t |> equal(sql,
        "SELECT json_extract(\"Prefs\", '$.theme') AS \"Theme\", json_extract(\"Prefs\", '$.lang') AS \"Lang\" FROM \"Users\"",
        "named-tuple projection with two JSON paths emits two json_extracts")
}

[test(description = "a named-tuple select can mix a plain column and a json path")]
def test_sql_text_projection_mixed_plain_and_json_path(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
        |> _select((N = _.Name, T = _.Prefs.theme)))
    t |> equal(sql,
        "SELECT \"Name\" AS \"N\", json_extract(\"Prefs\", '$.theme') AS \"T\" FROM \"Users\"",
        "named-tuple mixing plain column and JSON path emits both shapes")
}

[test(description = "selecting a sql_json string sub-field returns an array of strings")]
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

[test(description = "selecting a sql_json bool sub-field returns an array of bools")]
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

[test(description = "selecting a nested sql_json path returns the nested values")]
def test_projection_descent_nested_struct_path_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Account>)
        var p1 : InnerProfile
        p1.Addr.City = "NY"
        p1.Addr.Zip = "10001"
        p1.Bio = "alice in NY"
        var p2 : InnerProfile
        p2.Addr.City = "SF"
        p2.Addr.Zip = "94102"
        p2.Bio = "bob in SF"
        db |> insert(Account(Id = 1, Profile <- p1))
        db |> insert(Account(Id = 2, Profile <- p2))

        let cities <- _sql(db |> select_from(type<Account>) |> _select(_.Profile.Addr.City))
        t |> equal(length(cities), 2, "two cities")
        t |> equal(cities[0], "NY", "alice's city via nested JSON path projection")
        t |> equal(cities[1], "SF", "bob's city via nested JSON path projection")
    }
}

[test(description = "a named-tuple select mixing a plain column and a json path returns both per row")]
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

[test(description = "reading a row whose json column is malformed panics")]
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

[test(description = "an option of a sql_json struct round-trips some and NULL as none")]
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

[test(description = "an option of a sql_json struct is reported as a nullable TEXT column")]
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

[test(description = "a bare and an optional field of one sql_json type share its adapter")]
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

// ──────────────────────────────────────────────────────────────────────────
// @sql_blob — BLOB-backed columns through daslib/archive.
//
// test_64 covers the canonical SessionSnapshot round-trip + DDL=BLOB +
// adapter dedup. This file exercises the rest of the @sql_blob surface:
//   - multiple @sql_blob fields in one struct
//   - moderately nested archive shapes (nested struct, table, deep array)
//   - schema with mixed plain + @sql_blob + @sql_json fields
//   - opacity from the daslang side: select_from materializes the struct
// ──────────────────────────────────────────────────────────────────────────

struct PointSnap {
    x : int
    y : int
}

struct VectorSnap {
    head : PointSnap
    tail : PointSnap
}

[sql_table(name = "Geometry")]
struct GeometryRow {
    @sql_primary_key Id : int
    @sql_blob Origin : PointSnap
    @sql_blob Span : VectorSnap
}

[test(description = "two sql_blob fields in one struct each round-trip their own payload")]
def test_two_blob_fields_in_one_struct(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<GeometryRow>)
        var origin = PointSnap(x = 10, y = 20)
        var span = VectorSnap(
            head = PointSnap(x = 0, y = 0),
            tail = PointSnap(x = 100, y = 50))
        db |> insert(GeometryRow(Id = 1, Origin <- origin, Span <- span))

        let rows <- db |> select_from(type<GeometryRow>)
        t |> equal(length(rows), 1, "one row")
        t |> equal(rows[0].Origin.x, 10, "Origin.x round-trips")
        t |> equal(rows[0].Origin.y, 20, "Origin.y round-trips")
        t |> equal(rows[0].Span.head.x, 0, "Span.head.x round-trips")
        t |> equal(rows[0].Span.tail.x, 100, "Span.tail.x round-trips")
        t |> equal(rows[0].Span.tail.y, 50, "Span.tail.y round-trips")
    }
}

[test(description = "two sql_blob fields are each declared BLOB")]
def test_two_blob_fields_get_distinct_storage(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<GeometryRow>)
        let origin_type = db |> query_scalar(
            "SELECT type FROM pragma_table_info('Geometry') WHERE name='Origin'",
            type<string>)
        let span_type = db |> query_scalar(
            "SELECT type FROM pragma_table_info('Geometry') WHERE name='Span'",
            type<string>)
        t |> equal(origin_type, "BLOB", "Origin DDL is BLOB")
        t |> equal(span_type, "BLOB", "Span DDL is BLOB")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Container payloads — array<int> and table<string;int> through @sql_blob.
// daslib/archive serializes these positionally; the round-trip uses
// mem_archive_save / mem_archive_load.
// ──────────────────────────────────────────────────────────────────────────

struct DeepSnap {
    seq : array<int>
    bag : table<string; int>
    nested : array<PointSnap>
}

[sql_table(name = "Deep")]
struct DeepRow {
    @sql_primary_key Id : int
    @sql_blob State : DeepSnap
}

[test(description = "a sql_blob payload round-trips arrays, tables and nested structs")]
def test_blob_payload_with_array_table_nested(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<DeepRow>)
        var snap = DeepSnap(seq <- [10, 20, 30, 40])
        snap.bag |> insert("alpha", 1)
        snap.bag |> insert("beta", 2)
        snap.nested <- [PointSnap(x = 1, y = 2), PointSnap(x = 5, y = 6)]

        db |> insert(DeepRow(Id = 1, State <- snap))

        let rows <- db |> select_from(type<DeepRow>)
        t |> equal(length(rows), 1, "one deep row")
        t |> equal(length(rows[0].State.seq), 4, "seq length round-trips")
        t |> equal(rows[0].State.seq[2], 30, "seq[2] round-trips")
        t |> equal(length(rows[0].State.bag), 2, "bag size round-trips")
        let bag_alpha = rows[0].State.bag?["alpha"] ?? 0
        let bag_beta = rows[0].State.bag?["beta"] ?? 0
        t |> equal(bag_alpha, 1, "bag['alpha'] round-trips")
        t |> equal(bag_beta, 2, "bag['beta'] round-trips")
        t |> equal(length(rows[0].State.nested), 2, "nested length round-trips")
        t |> equal(rows[0].State.nested[1].x, 5, "nested[1].x round-trips")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Mixed columns — @sql_blob next to @sql_json + plain scalars
// ──────────────────────────────────────────────────────────────────────────

struct PrefBlock {
    theme : string
    lang : string
}

[sql_table(name = "MixedRow")]
struct MixedRow {
    @sql_primary_key Id : int
    Plain : int
    @sql_json Prefs : PrefBlock
    @sql_blob State : PointSnap
}

[test(description = "plain, sql_json and sql_blob columns coexist in one table")]
def test_mixed_blob_json_plain_columns_coexist(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<MixedRow>)
        var prefs = PrefBlock(theme = "neon", lang = "ru")
        var state = PointSnap(x = 7, y = 11)
        db |> insert(MixedRow(Id = 1, Plain = 42, Prefs <- prefs, State <- state))

        let rows <- db |> select_from(type<MixedRow>)
        t |> equal(length(rows), 1, "one mixed row")
        t |> equal(rows[0].Plain, 42, "plain int unaffected by mixed adapters")
        t |> equal(rows[0].Prefs.theme, "neon", "json prefs round-trips")
        t |> equal(rows[0].State.x, 7, "blob state.x round-trips")
        t |> equal(rows[0].State.y, 11, "blob state.y round-trips")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Two rows worth of distinct blob payloads — confirms each row carries its
// own archive bytes, no aliasing across the materialization loop.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "each row keeps its own sql_blob payload")]
def test_blob_payloads_are_per_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<DeepRow>)
        var a : DeepSnap
        a.seq <- [1, 2, 3]
        var b : DeepSnap
        b.seq <- [100, 200]
        db |> insert(DeepRow(Id = 1, State <- a))
        db |> insert(DeepRow(Id = 2, State <- b))

        let rows <- db |> select_from(type<DeepRow>)
        t |> equal(length(rows), 2, "two rows")
        t |> equal(length(rows[0].State.seq), 3, "row 1 seq size")
        t |> equal(rows[0].State.seq[0], 1, "row 1 seq[0]")
        t |> equal(length(rows[1].State.seq), 2, "row 2 seq size")
        t |> equal(rows[1].State.seq[0], 100, "row 2 seq[0]")
    }
}

// ===== @sql_blob + Option<UserStruct> =====
// Regression for the same Option-unwrap pair as test_64's @sql_json case: codegen must
// register `sql_bind` for bare T (not Option<T>) so the runtime Option-strip in
// sql_bind_to_stmt finds the user-module adapter.

struct OptBlobPayload {
    seq : array<int>
}

[sql_table(name = "OptBlobRows")]
struct OptBlobRow {
    @sql_primary_key Id : int
    @sql_blob Payload : Option<OptBlobPayload>
}

[test(description = "an option of a sql_blob struct round-trips some and NULL as none")]
def test_sql_blob_option_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<OptBlobRow>)
        var p : OptBlobPayload
        p.seq <- [10, 20, 30]
        db |> insert(OptBlobRow(Id = 1, Payload = some(p)))
        db |> insert(OptBlobRow(Id = 2, Payload = none(type<OptBlobPayload>)))
        let rows <- db |> select_from(type<OptBlobRow>)
        t |> equal(length(rows), 2, "two rows")
        t |> equal(rows[0].Payload |> is_some, true, "row 0 payload present")
        t |> equal(length((rows[0].Payload |> unwrap).seq), 3, "blob payload size preserved")
        t |> equal((rows[0].Payload |> unwrap).seq[0], 10, "blob payload first elem")
        t |> equal(rows[1].Payload |> is_none, true, "row 1 NULL extracts as none")
    }
}

[test(description = "an option of a sql_blob struct is reported as a nullable BLOB column")]
def test_sql_blob_option_classifies_as_blob(t : T?) {
    let cols <- column_info(type<OptBlobRow>)
    t |> equal(length(cols), 2, "Id + Payload")
    t |> equal(cols[1].name, "Payload", "Payload column")
    t |> equal(cols[1].is_nullable, true, "Option<UserStruct> ↔ nullable")
    t |> equal(cols[1].data_type, SqlType.Blob, "@sql_blob on Option<T> classifies as BLOB")
}
