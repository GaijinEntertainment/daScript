options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

[sql_view(name="ExpensiveCars")]
struct ExpensiveCar {
    Id    : int
    Name  : string
    Price : int
}

[sql_view(name="CarPrices")]
struct CarPrice {
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 800))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

[test]
def test_view_create_and_select_full_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> _create_view(type<ExpensiveCar>,
            db |> select_from(type<Car>) |> _where(_.Price > 200))
        let rows <- db |> select_from(type<ExpensiveCar>)
        t |> equal(length(rows), 2, "Tesla + Lambo over 200")
        t |> equal(rows[0].Name, "Tesla", "row 0 Tesla")
        t |> equal(rows[1].Name, "Lambo", "row 1 Lambo")
    }
}

[test]
def test_view_named_tuple_projection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> _create_view(type<CarPrice>,
            db |> select_from(type<Car>) |> _select((Name = _.Name, Price = _.Price)))
        let rows <- db |> select_from(type<CarPrice>)
        t |> equal(length(rows), 5, "5 rows expected")
        t |> equal(rows[0].Name, "BMW", "row 0 Name")
        t |> equal(rows[0].Price, 100, "row 0 Price")
    }
}

[test]
def test_view_select_via_sql_macro(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> _create_view(type<ExpensiveCar>,
            db |> select_from(type<Car>) |> _where(_.Price >= 200))
        let names <- _sql(db |> select_from(type<ExpensiveCar>) |> _select(_.Name))
        t |> equal(length(names), 3, "Audi + Tesla + Lambo")
        t |> equal(names[0], "Audi", "names[0]")
    }
}

[test]
def test_drop_view_if_exists(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> _create_view(type<ExpensiveCar>,
            db |> select_from(type<Car>) |> _where(_.Price > 200))
        let before <- db |> select_from(type<ExpensiveCar>)
        t |> equal(length(before), 2, "view exists before drop")
        db |> drop_view_if_exists(type<ExpensiveCar>)
        // re-creating with a different filter validates that drop succeeded
        db |> _create_view(type<ExpensiveCar>,
            db |> select_from(type<Car>) |> _where(_.Price > 100))
        let after <- db |> select_from(type<ExpensiveCar>)
        t |> equal(length(after), 3, "view re-created with looser filter")
    }
}

[test]
def test_try_drop_view_if_exists_idempotent(t : T?) {
    with_sqlite(":memory:") $(db) {
        // dropping a non-existent view is a no-op (IF EXISTS)
        let err = db |> try_drop_view_if_exists(type<ExpensiveCar>)
        t |> equal(err |> is_some, false, "no error dropping a non-existent view")
    }
}

// --- Enum + bitfield literals in `_create_view` predicates -------------
// `is_const_or_captured_var` (collection side) recognized
// ExprConstEnumeration / ExprConstBitfield as bindable constants, but
// `is_const_literal` (CREATE VIEW inlining) didn't — so a chain like
// `_where(_.Status == Status.Shipped)` failed with "cannot reference local
// variables" even though the enum value is a compile-time constant.
// These tests exercise both shapes; before the fix, both failed at
// _create_view's compile-time inlining pass.

enum Status {
    Pending = 1
    Shipped = 2
    Delivered = 3
}

bitfield Permissions : uint8 {
    Read
    Write
    Execute
}

[sql_table(name="Orders")]
struct OrderRec {
    @sql_primary_key Id : int
    StatusVal : int
    PermBits  : uint8
}

[sql_view(name="ShippedOrders")]
struct ShippedOrder {
    Id        : int
    StatusVal : int
    PermBits  : uint8
}

[sql_view(name="WriteableOrders")]
struct WriteableOrder {
    Id        : int
    StatusVal : int
    PermBits  : uint8
}

[test]
def test_view_predicate_with_enum_literal(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<OrderRec>)
        db |> insert(OrderRec(Id = 1, StatusVal = int(Status.Pending),   PermBits = uint8(0)))
        db |> insert(OrderRec(Id = 2, StatusVal = int(Status.Shipped),   PermBits = uint8(0)))
        db |> insert(OrderRec(Id = 3, StatusVal = int(Status.Delivered), PermBits = uint8(0)))
        // Status.Shipped == 2 — must inline as the integer 2 inside the
        // CREATE VIEW body since SQLite rejects ? placeholders there.
        db |> _create_view(type<ShippedOrder>,
            db |> select_from(type<OrderRec>) |> _where(_.StatusVal == int(Status.Shipped)))
        let rows <- db |> select_from(type<ShippedOrder>)
        t |> equal(length(rows), 1, "exactly one shipped order")
        t |> equal(rows[0].Id, 2, "Id 2 is the shipped one")
    }
}

[test]
def test_view_predicate_with_bitfield_literal(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<OrderRec>)
        let write_only = Permissions.Write
        db |> insert(OrderRec(Id = 1, StatusVal = 0, PermBits = uint8(0)))
        db |> insert(OrderRec(Id = 2, StatusVal = 0, PermBits = uint8(write_only)))
        db |> insert(OrderRec(Id = 3, StatusVal = 0, PermBits = uint8(Permissions.Read | Permissions.Write)))
        // Bitfield literal must inline as its underlying integer value.
        db |> _create_view(type<WriteableOrder>,
            db |> select_from(type<OrderRec>) |> _where(_.PermBits == uint8(Permissions.Write)))
        let rows <- db |> select_from(type<WriteableOrder>)
        t |> equal(length(rows), 1, "exactly one row with permbits == Write")
        t |> equal(rows[0].Id, 2, "Id 2 has just Write")
    }
}
