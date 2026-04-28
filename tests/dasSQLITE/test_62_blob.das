options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

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

[test]
def test_blob_ddl_is_BLOB_NOT_NULL(t : T?) {
    let ddl = _sql_create_table_sql(type<Image>)
    t |> success(ddl |> find("\"Data\" BLOB NOT NULL") >= 0, "array<uint8> field DDL is BLOB NOT NULL")
}

[test]
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

[test]
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

[test]
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

[test]
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
                t |> equal(length(r.Body._value), 4, "non-null blob length")
            }
        }
    }
}

[test]
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
        t |> equal(length(r1.Body._value), 0, "empty blob has zero bytes")

        let r2 = db |> query_one("SELECT * FROM Files WHERE Id = 2", type<FileRow>)
        t |> success(r2.Body |> is_none(), "NULL blob is none")
    }
}
