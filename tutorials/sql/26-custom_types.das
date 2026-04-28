options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 26 — custom column types via the sql_bind / sql_extract pair.
//
// Every SQL-addressable type T has a bidirectional pair of named functions:
//
//     def sql_bind    (v : T) : P                 // T → primitive
//     def sql_extract (v : P; t : type<T>) : T    // primitive → T
//
// where P is one of {int64, double, string, array<uint8>} — the four
// SQLite storage classes. The RETURN TYPE of sql_bind IS the storage
// type; the [sql_table] macro reads it and emits the matching
// sqlite3_bind_* / sqlite3_column_* call. NULL is handled orthogonally
// by tut 25's Option<T>, so this rail covers four storage forms, not five.
//
// Convention dispatch — no register_sql_type() call, no runtime lookup
// table. Defining the pair next to the type IS the registration. The
// [sql_table] macro emits `_::sql_bind(...)` so the user's-module
// overloads are visible at expansion (same mechanism as `_::clone` /
// `_::finalize`).
//
// Standard daslang types (every int*/uint*, float, double, bool, string,
// array<uint8>) ship with adapter pairs in dasSQLITE itself. Enums round-
// trip through INTEGER via a generic overload. User code only writes
// adapters for domain types.

// --- DateTime as INTEGER (unix seconds) ----------------------------------
// Default storage choice for DateTime: integer is indexable, compact, and
// math-friendly. The user provides the pair next to the type definition.

struct DateTime {
    unix_seconds : int64
}

def sql_bind(dt : DateTime) : int64 {
    return dt.unix_seconds
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<DateTime>) : DateTime {
    return DateTime(unix_seconds = v)
}

// --- Guid as BLOB (16 raw bytes) -----------------------------------------
// Same pattern, different primitive: the return type ``array<uint8>`` picks
// BLOB storage automatically. Anything that round-trips through bytes —
// asset hashes, network IDs, fingerprints — fits this shape.

struct Guid {
    @safe_when_uninitialized bytes : array<uint8>
}

def sql_bind(g : Guid) : array<uint8> {
    //! Clones bytes into a fresh array so the returned value is owned. The
    //! BLOB is then copied again by ``sqlite3_bind_blob`` (SQLITE_TRANSIENT).
    //! For multi-MB asset blobs, prefer the ``sqlite3_blob_open`` streaming
    //! API instead of round-tripping through a copy-on-bind adapter.
    var copy : array<uint8>
    copy := g.bytes
    return <- copy
}

[unused_argument(t)]
def sql_extract(var v : array<uint8>; t : type<Guid>) : Guid {
    return Guid(bytes <- v)
}

// --- Domain enum (auto-handled by dasSQLITE's generic overload) ----------
// dasSQLITE ships ``def sql_bind $T (e : T) : int64 where T : enum`` and
// the matching extract, so any enum round-trips through INTEGER without
// per-enum boilerplate. Tradeoff: reordering enum values silently corrupts
// existing data (value 2 means something different in the new schema). For
// rename-safe storage, the deferred `@sql_enum_as_string` annotation will
// flip the rail to TEXT-backed; ship int-backed by default.

enum OrderStatus {
    Pending
    Paid
    Shipped
    Cancelled
}

// --- Schema using all three custom types + a stdlib type -----------------

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    ExternalId : Guid           // BLOB column — Guid pair
    PlacedAt   : DateTime       // INTEGER column — DateTime pair
    Status     : OrderStatus    // INTEGER column — enum generic
    Total      : float          // REAL column — stdlib float widening
}

// Emitted DDL (formatted for readability):
//   CREATE TABLE "Orders"(
//       "Id"         INTEGER PRIMARY KEY,
//       "ExternalId" BLOB    NOT NULL,
//       "PlacedAt"   INTEGER NOT NULL,
//       "Status"     INTEGER NOT NULL,
//       "Total"      REAL    NOT NULL
//   )

// --- Option<T> composes automatically ------------------------------------
// Tut 25 already handles nullability: ``Option<DateTime>`` works iff
// DateTime has the adapter pair. The [sql_table] macro unwraps Option<T>
// at runtime and emits NULL-aware bind/read code. Zero extra adapters
// needed.

[sql_table(name = "Events")]
struct Event {
    @sql_primary_key Id : int
    At       : DateTime                                    // NOT NULL INTEGER
    @safe_when_uninitialized StartsAt : Option<DateTime>   // NULL INTEGER — same adapter under the Option
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Order>)
        db |> create_table(type<Event>)

        // The macro emits, per column:
        //     sqlite3_bind_blob (..., _::sql_bind(o.ExternalId))   // array<uint8>
        //     sqlite3_bind_int64(..., _::sql_bind(o.PlacedAt))     // int64
        //     sqlite3_bind_int64(..., _::sql_bind(o.Status))       // enum → int64
        //     sqlite3_bind_double(..., _::sql_bind(o.Total))       // float → double widening
        var bytes : array<uint8>
        for (_ in range(16)) {
            bytes |> push(uint8(0xAB))
        }
        db |> insert(Order(
            Id = 1,
            ExternalId = Guid(bytes <- bytes),
            PlacedAt = DateTime(unix_seconds = 1713000000l),
            Status = OrderStatus.Paid,
            Total = 99.5
        ))

        // SELECT round-trips the adapters in reverse:
        //     _::sql_extract(sqlite3_column_blob (...), type<Guid>)
        //     _::sql_extract(sqlite3_column_int64(...), type<DateTime>)
        //     _::sql_extract(sqlite3_column_int64(...), type<OrderStatus>)
        let paid <- _sql(db |> select_from(type<Order>) |> _where(_.Status == OrderStatus.Paid))
        to_log(LOG_INFO, "paid orders: {length(paid)}\n")
        if (length(paid) > 0) {
            to_log(LOG_INFO, "first paid: PlacedAt unix_seconds={paid[0].PlacedAt.unix_seconds}\n")
        }

        // Option<DateTime> reads as Option — NULL column → none.
        db |> insert(Event(Id = 1, At = DateTime(unix_seconds = 0l), StartsAt = none(type<DateTime>)))
        db |> insert(Event(Id = 2, At = DateTime(unix_seconds = 0l), StartsAt = some(DateTime(unix_seconds = 1713000000l))))
        let events <- _sql(db |> select_from(type<Event>))
        for (e in events) {
            if (e.StartsAt |> is_none()) {
                to_log(LOG_INFO, "event {e.Id} not scheduled\n")
            } else {
                to_log(LOG_INFO, "event {e.Id} starts at {e.StartsAt._value.unix_seconds}\n")
            }
        }
    }
}

// --- DateTime as TEXT (alternate storage form) ---------------------------
// If ISO8601 text is preferred — for human-readable sqlite3 shell output
// or compatibility with a schema that already stores dates as strings —
// the user writes a pair whose ``sql_bind`` RETURNS string instead of
// int64. Schema picks one storage form per type at module level (no
// per-field override yet — see DEFERRED below).
//
//   def sql_bind(dt : DateTime) : string {
//       return to_iso8601(dt.unix_seconds)
//   }
//   [unused_argument(t)]
//   def sql_extract(s : string; t : type<DateTime>) : DateTime {
//       return DateTime(unix_seconds = parse_iso8601(s))
//   }

// --- Missing-adapter compile error ---------------------------------------
// If a [sql_table] struct has a field whose type has no ``sql_bind`` /
// ``sql_extract`` pair in scope, the macro emits a literal call to
// ``_::sql_bind(obj.field)`` and overload resolution falls through to the
// catch-all generic, which tells the user to define the pair:
//
//     struct Color { r, g, b : float }
//     [sql_table(name = "styles")]
//     struct Style {
//         @sql_primary_key Id : int
//         Bg : Color           // no sql_bind(Color) — compile error
//     }
//
// Error message names the offending struct + field type and gives the
// signature template for the missing adapter. No runtime "type not
// registered" error — this is all compile-time.

// --- DEFERRED ------------------------------------------------------------
//
// @sql_as(type<P>) per-field override. Picks a specific primitive-returning
// overload for ONE column when multiple adapter shapes exist for the same
// type. Lock 8 in API_REWORK §29 specifies the shape; ships when a user
// files an issue (the 95% case is one adapter per type).
//
//     [sql_table(name = "Meetings")]
//     struct Meeting {
//         @sql_primary_key Id : int
//         At      : DateTime                  // default INTEGER adapter
//         @sql_as(type<string>)               // FUTURE — string-returning overload
//         HumanAt : DateTime                  // would emit TEXT column with ISO8601
//     }
//
// Other deferred:
//   - @sql_enum_as_string (rename-safe text storage for enums).
//   - Fully-generic user adapters (`$T` templated on traits other than
//     `enum`). Enum is the one concession; further genericity ships on
//     concrete demand.
//   - Adapter validators (`def sql_validate(v : T) : bool` for CHECK
//     constraints). Column-level CHECK stays in tut 25's string form.
//   - JSON / BLOB struct columns — tut 28 territory. Builds on this rail.
