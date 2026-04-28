options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 27 — BLOB columns: storing and reading raw bytes.
//
// ``array<uint8>`` is one of the four primitives in the tut-26 adapter
// rail, so it works as a [sql_table] field type with no user adapter:
//
//     @sql_primary_key Id : int
//     Data : array<uint8>            -> BLOB column, NOT NULL
//
// The [sql_table] DDL emits the BLOB type, INSERT binds the bytes via
// ``sqlite3_bind_blob`` with ``SQLITE_TRANSIENT`` (SQLite copies into
// its own buffer), and SELECT allocates a fresh ``array<uint8>`` on
// daslang's heap and copies from the column pointer (which is only
// valid until the next ``step``/``reset``).
//
// Three tradeoffs to be aware of:
//
//   - **Two copies on insert.** SQLite's ``SQLITE_TRANSIENT`` makes its
//     own copy after the bind call. For multi-MB asset blobs, prefer
//     SQLite's ``sqlite3_blob_open`` streaming API or chunked storage.
//   - **One copy on read.** Each row materializes a fresh
//     ``array<uint8>`` — the column pointer can't escape the cursor
//     position. Streaming reads need ``sqlite3_blob_read`` directly.
//   - **Nullable BLOB columns** use ``Option<array<uint8>>``, the same
//     pattern as nullable scalars in tut 18.

[sql_table(name = "Images")]
struct Image {
    @sql_primary_key Id : int
    Name : string
    @safe_when_uninitialized Data : array<uint8>
}

def make_demo_image(payload : string) : array<uint8> {
    //! Tiny synthetic byte payload. Real apps load from fopen / fmap,
    //! game asset packs, network sockets, etc.
    var bytes : array<uint8>
    peek_data(payload) $(arr) {
        bytes |> resize(length(arr))
        for (i in range(length(arr))) {
            bytes[i] = arr[i]
        }
    }
    return <- bytes
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Image>)

        // Inspect the emitted DDL — Data column is BLOB NOT NULL because
        // sql_bind(array<uint8>) returns array<uint8> and the field has
        // no Option wrapper.
        let ddl = _sql_create_table_sql(type<Image>)
        to_log(LOG_INFO, "DDL: {ddl}\n")

        // INSERT — the macro emits sqlite3_bind_blob(stmt, idx,
        // _::sql_bind(row.Data)). For ``array<uint8>``, sql_bind is the
        // identity-clone passthrough; SQLite then copies the bytes via
        // SQLITE_TRANSIENT.
        var hello = make_demo_image("hello, blob")
        var world = make_demo_image("blob round-trip works")
        db |> insert(Image(Id = 1, Name = "hello", Data <- hello))
        db |> insert(Image(Id = 2, Name = "world", Data <- world))

        // SELECT — read_via_adapter dispatches on the storage primitive
        // (array<uint8>) and calls sqlite3_column_blob_ to allocate a
        // fresh array; sql_extract is the identity-clone for the
        // primitive case.
        let images <- _sql(db |> select_from(type<Image>))
        for (img in images) {
            to_log(LOG_INFO, "{img.Name} ({length(img.Data)} bytes)\n")
        }

        // Filter by raw byte length using a parameterized scalar query —
        // routes through query_scalar, which goes through the same
        // read_via_adapter path.
        let total = db |> query_scalar("SELECT SUM(LENGTH(Data)) FROM Images", type<int64>)
        to_log(LOG_INFO, "total blob bytes stored: {total}\n")
    }
}

// --- Future work: streaming BLOB I/O ------------------------------------
//
// SQLite offers ``sqlite3_blob_open`` / ``sqlite3_blob_read`` /
// ``sqlite3_blob_write`` for incremental access without materializing
// the whole blob in daslang's heap. Likely shape:
//
//     db |> open_blob(type<Image>, rowid, "Data") <| $(blob) {
//         var buf : array<uint8>
//         buf |> resize(4096)
//         blob |> read(offset, buf)
//     }
//
// EF Core doesn't expose this — game asset and large-binary use cases
// typically live outside the ORM in dedicated APIs. dasSQLITE keeps the
// streaming path on the deferred list until a real workload demands it.
