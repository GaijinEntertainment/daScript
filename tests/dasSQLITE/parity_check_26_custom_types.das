options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/option
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 26 — custom types via sql_bind / sql_extract.
//
// Chain shapes:
//   - `_where(_.Status == OrderStatus.Paid)`   filter on enum field
//   - full-row read of struct with Option<custom> field
//
// The adapter rail (sql_bind / sql_extract) is invisible to Mode 3 — plain
// arrays don't go through SQL, so the custom types just live as structs.
// Parity should hold because the round-trip preserves data.

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
    ExternalId : Guid
    PlacedAt   : DateTime
    Status     : OrderStatus
    Total      : float
}

[sql_table(name = "Events")]
struct Event {
    @sql_primary_key Id : int
    At : DateTime
    @safe_when_uninitialized StartsAt : Option<DateTime>
}

def sample_bytes() : array<uint8> {
    var bytes : array<uint8>
    for (_ in range(16)) {
        bytes |> push(uint8(0xAB))
    }
    return <- bytes
}

def fixture_orders_db(db : SqlRunner) {
    db |> create_table(type<Order>)
    var b1 <- sample_bytes()
    var b2 <- sample_bytes()
    var b3 <- sample_bytes()
    db |> insert(Order(Id = 1, ExternalId = Guid(bytes <- b1), PlacedAt = DateTime(unix_seconds = 100l), Status = OrderStatus.Pending, Total = 10.0))
    db |> insert(Order(Id = 2, ExternalId = Guid(bytes <- b2), PlacedAt = DateTime(unix_seconds = 200l), Status = OrderStatus.Paid,    Total = 20.0))
    db |> insert(Order(Id = 3, ExternalId = Guid(bytes <- b3), PlacedAt = DateTime(unix_seconds = 300l), Status = OrderStatus.Paid,    Total = 30.0))
}

def fixture_orders_array() : array<Order> {
    var arr : array<Order>
    var b1 <- sample_bytes()
    var b2 <- sample_bytes()
    var b3 <- sample_bytes()
    arr |> emplace(Order(Id = 1, ExternalId = Guid(bytes <- b1), PlacedAt = DateTime(unix_seconds = 100l), Status = OrderStatus.Pending, Total = 10.0))
    arr |> emplace(Order(Id = 2, ExternalId = Guid(bytes <- b2), PlacedAt = DateTime(unix_seconds = 200l), Status = OrderStatus.Paid,    Total = 20.0))
    arr |> emplace(Order(Id = 3, ExternalId = Guid(bytes <- b3), PlacedAt = DateTime(unix_seconds = 300l), Status = OrderStatus.Paid,    Total = 30.0))
    return <- arr
}

def fixture_events_db(db : SqlRunner) {
    db |> create_table(type<Event>)
    db |> insert(Event(Id = 1, At = DateTime(unix_seconds = 0l), StartsAt = none(type<DateTime>)))
    db |> insert(Event(Id = 2, At = DateTime(unix_seconds = 0l), StartsAt = some(DateTime(unix_seconds = 1713000000l))))
}

def fixture_events_array() : array<Event> {
    var arr : array<Event>
    arr |> emplace(Event(Id = 1, At = DateTime(unix_seconds = 0l), StartsAt = none(type<DateTime>)))
    arr |> emplace(Event(Id = 2, At = DateTime(unix_seconds = 0l), StartsAt = some(DateTime(unix_seconds = 1713000000l))))
    return <- arr
}

def bytes_eq(a : array<uint8>; b : array<uint8>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

def order_eq(a : Order; b : Order) : bool {
    return (a.Id == b.Id
        && bytes_eq(a.ExternalId.bytes, b.ExternalId.bytes)
        && a.PlacedAt.unix_seconds == b.PlacedAt.unix_seconds
        && a.Status == b.Status
        && a.Total == b.Total)
}

def orders_eq(a : array<Order>; b : array<Order>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!order_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

def opt_dt_eq(a : Option<DateTime>; b : Option<DateTime>) : bool {
    if ((a |> is_some) != (b |> is_some)) {
        return false
    }
    if (a |> is_none) {
        return true
    }
    return a._value.unix_seconds == b._value.unix_seconds
}

def event_eq(a : Event; b : Event) : bool {
    return (a.Id == b.Id
        && a.At.unix_seconds == b.At.unix_seconds
        && opt_dt_eq(a.StartsAt, b.StartsAt))
}

def events_eq(a : array<Event>; b : array<Event>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!event_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

[test]
def parity_where_enum_filter(t : T?) {
    // Filter by enum field. SQL emits "Status" = ? with the enum lowered to int64.
    with_sqlite(":memory:") $(db) {
        fixture_orders_db(db)
        var arr <- fixture_orders_array()

        let m1 <- _sql(db |> select_from(type<Order>) |> _where(_.Status == OrderStatus.Paid) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<Order>) |> _where(_.Status == OrderStatus.Paid) |> _order_by(_.Id))
        let m3 <- (arr |> _where(_.Status == OrderStatus.Paid) |> _order_by(_.Id))

        t |> equal(length(m1), 2, "enum filter: 2 paid orders")
        t |> success(orders_eq(m1, m2), "enum filter: M1 ≡ M2")
        t |> success(orders_eq(m2, m3), "enum filter: M2 ≡ M3")
    }
}

[test]
def parity_event_option_custom_type(t : T?) {
    // Full-row read of Event whose StartsAt is Option<DateTime> (a custom type).
    with_sqlite(":memory:") $(db) {
        fixture_events_db(db)
        var arr <- fixture_events_array()

        let m1 <- _sql(db |> select_from(type<Event>) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<Event>) |> _order_by(_.Id))
        let m3 <- (arr |> _order_by(_.Id))

        t |> equal(length(m1), 2, "events: 2 rows")
        t |> success(events_eq(m1, m2), "Option<DateTime>: M1 ≡ M2")
        t |> success(events_eq(m2, m3), "Option<DateTime>: M2 ≡ M3")
    }
}
