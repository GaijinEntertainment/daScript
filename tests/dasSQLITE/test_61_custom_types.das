options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

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

def sql_bind(dt : DateTime) : int64 {
    return dt.unix_seconds
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<DateTime>) : DateTime {
    return DateTime(unix_seconds = v)
}

struct Guid {
    @safe_when_uninitialized bytes : array<uint8>
}

def sql_bind(g : Guid) : array<uint8> {
    var copy : array<uint8>
    copy := g.bytes
    return <- copy
}

[unused_argument(t)]
def sql_extract(var v : array<uint8>; t : type<Guid>) : Guid {
    return Guid(bytes <- v)
}

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

[test]
def test_ddl_picks_storage_type_per_adapter(t : T?) {
    let ddl = _sql_create_table_sql(type<Order>)
    t |> success(ddl |> find("\"ExternalId\" BLOB") >= 0, "Guid (sql_bind→array<uint8>) → BLOB column")
    t |> success(ddl |> find("\"PlacedAt\" INTEGER") >= 0, "DateTime (sql_bind→int64) → INTEGER column")
    t |> success(ddl |> find("\"Status\" INTEGER") >= 0, "enum (generic sql_bind→int64) → INTEGER column")
    t |> success(ddl |> find("\"Total\" REAL") >= 0, "float (widening sql_bind→double) → REAL column")
    t |> success(ddl |> find("\"Id\" INTEGER PRIMARY KEY") >= 0, "int PK (widening sql_bind→int64) → INTEGER PRIMARY KEY")
}

[test]
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

[test]
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

[test]
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

[test]
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

[test]
def test_option_over_custom_adapter(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Event>)
        db |> insert(Event(Id = 1, At = DateTime(unix_seconds = 100l), StartsAt = none(type<DateTime>)))
        db |> insert(Event(Id = 2, At = DateTime(unix_seconds = 200l), StartsAt = some(DateTime(unix_seconds = 300l))))

        let r1 = db |> query_one("SELECT * FROM Events WHERE Id = 1", type<Event>)
        t |> success(r1.StartsAt |> is_none(), "none() Option<DateTime> binds as NULL, reads as none")

        let r2 = db |> query_one("SELECT * FROM Events WHERE Id = 2", type<Event>)
        t |> success(r2.StartsAt |> is_some(), "some(...) Option<DateTime> binds and reads back as some")
        t |> equal(r2.StartsAt._value.unix_seconds, 300l, "Option<DateTime> inner value preserved")
    }
}

[test]
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
