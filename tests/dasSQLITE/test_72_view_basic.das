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
// Both arms cast the enum/bitfield to its underlying integer at the call
// site. The macro emits `_::to_sql_literal(<int-cast-expr>)` which the
// runtime evaluates once at CREATE VIEW time and inlines into the DDL.

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

// --- Captured locals in _create_view bodies --------------------------
// The macro emits `_::to_sql_literal(<bind-expr>)` per bind expression;
// the value is evaluated once at view-creation time and inlined into the
// DDL stored in sqlite_schema. To rebind, drop and re-create the view.

[sql_table(name="Inventory")]
struct Item {
    @sql_primary_key Id : int
    Name   : string
    Price  : int
    Weight : double
    OnSale : bool
}

[sql_view(name="ExpensiveItems")]
struct ExpensiveItem {
    Id    : int
    Name  : string
    Price : int
    Weight : double
    OnSale : bool
}

[sql_view(name="HeavyItems")]
struct HeavyItem {
    Id    : int
    Name  : string
    Price : int
    Weight : double
    OnSale : bool
}

[sql_view(name="OnSaleItems")]
struct OnSaleItem {
    Id    : int
    Name  : string
    Price : int
    Weight : double
    OnSale : bool
}

[sql_view(name="NamedItems")]
struct NamedItem {
    Id    : int
    Name  : string
    Price : int
    Weight : double
    OnSale : bool
}

def private inv_fixture(db : SqlRunner) {
    db |> create_table(type<Item>)
    db |> insert(Item(Id = 1, Name = "Apple",  Price =  50, Weight = 0.2lf, OnSale = false))
    db |> insert(Item(Id = 2, Name = "Bread",  Price = 200, Weight = 0.5lf, OnSale = false))
    db |> insert(Item(Id = 3, Name = "Cheese", Price = 800, Weight = 1.2lf, OnSale = true))
    db |> insert(Item(Id = 4, Name = "Diamond", Price = 9999, Weight = 0.01lf, OnSale = false))
    db |> insert(Item(Id = 5, Name = "Apple",  Price = 100, Weight = 0.2lf, OnSale = true))
}

[test]
def test_view_captured_int_local(t : T?) {
    with_sqlite(":memory:") $(db) {
        inv_fixture(db)
        let cutoff = 200
        db |> _create_view(type<ExpensiveItem>,
            db |> select_from(type<Item>) |> _where(_.Price > cutoff))
        let rows <- db |> select_from(type<ExpensiveItem>)
        t |> equal(length(rows), 2, "Cheese + Diamond over 200")
    }
}

[test]
def test_view_captured_double_local(t : T?) {
    with_sqlite(":memory:") $(db) {
        inv_fixture(db)
        // Use a value that needs precision past float to verify the .17g formatter.
        let weight_min = 0.4999999999999999lf
        db |> _create_view(type<HeavyItem>,
            db |> select_from(type<Item>) |> _where(_.Weight > weight_min))
        let rows <- db |> select_from(type<HeavyItem>)
        t |> equal(length(rows), 2, "Bread (0.5) + Cheese (1.2) above 0.4999…")
    }
}

[test]
def test_view_captured_bool_local(t : T?) {
    with_sqlite(":memory:") $(db) {
        inv_fixture(db)
        let want_on_sale = true
        db |> _create_view(type<OnSaleItem>,
            db |> select_from(type<Item>) |> _where(_.OnSale == want_on_sale))
        let rows <- db |> select_from(type<OnSaleItem>)
        t |> equal(length(rows), 2, "Cheese + Apple(5) on sale")
    }
}

[test]
def test_view_captured_string_local(t : T?) {
    with_sqlite(":memory:") $(db) {
        inv_fixture(db)
        // String with an embedded apostrophe: sql_quote_lit must double it.
        db |> exec("INSERT INTO Inventory (Id, Name, Price, Weight, OnSale) VALUES (6, 'O''Brien', 1, 0.1, 0)")
        let needle = "O'Brien"
        db |> _create_view(type<NamedItem>,
            db |> select_from(type<Item>) |> _where(_.Name == needle))
        let rows <- db |> select_from(type<NamedItem>)
        t |> equal(length(rows), 1, "exactly one O'Brien")
        t |> equal(rows[0].Id, 6, "Id 6 is the O'Brien row")
    }
}

// --- Struct-field and function-call binds in view bodies ----------
// The chain analyzer accepts any expression as a bind (column refs are
// recognized first; everything else falls through to ?-bind via
// _::to_sql_literal at view-creation time). User overloads still win
// when types fall outside the default set.

struct private Threshold {
    min_price : int
    qualifier : string
}

[sql_view(name="StructFieldFiltered")]
struct StructFieldFilteredItem {
    Id    : int
    Name  : string
    Price : int
    Weight : double
    OnSale : bool
}

def private compute_threshold() : int => 100

[test]
def test_view_struct_field_bind(t : T?) {
    with_sqlite(":memory:") $(db) {
        inv_fixture(db)
        let cfg = Threshold(min_price = 200, qualifier = "Cheese")
        db |> _create_view(type<StructFieldFilteredItem>,
            db |> select_from(type<Item>) |> _where(_.Price >= cfg.min_price))
        let rows <- db |> select_from(type<StructFieldFilteredItem>)
        t |> equal(length(rows), 3, "Bread + Cheese + Diamond at or above 200")
    }
}

[sql_view(name="FnCallFiltered")]
struct FnCallFilteredItem {
    Id    : int
    Name  : string
    Price : int
    Weight : double
    OnSale : bool
}

[test]
def test_view_fn_call_bind(t : T?) {
    with_sqlite(":memory:") $(db) {
        inv_fixture(db)
        db |> _create_view(type<FnCallFilteredItem>,
            db |> select_from(type<Item>) |> _where(_.Price >= compute_threshold()))
        let rows <- db |> select_from(type<FnCallFilteredItem>)
        t |> equal(length(rows), 4, "items at or above compute_threshold() == 100")
    }
}

// --- User-defined `to_sql_literal` overload --------------------------
// Demonstrates the extension point: a user type that's NOT in the
// default set can join the `_create_view` body by supplying a one-line
// overload. Resolution at the user's call site picks this up via `_::`.

enum private Tier {
    Bronze = 10
    Silver = 20
    Gold   = 30
}

def to_sql_literal(t : Tier) : string => "{int(t)}"

[sql_table(name="Members")]
struct MemberRow {
    @sql_primary_key Id : int
    TierVal : int
}

[sql_view(name="GoldMembers")]
struct GoldMemberRow {
    Id      : int
    TierVal : int
}

[test]
def test_view_user_enum_overload(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<MemberRow>)
        db |> insert(MemberRow(Id = 1, TierVal = int(Tier.Bronze)))
        db |> insert(MemberRow(Id = 2, TierVal = int(Tier.Silver)))
        db |> insert(MemberRow(Id = 3, TierVal = int(Tier.Gold)))
        let want = Tier.Gold
        db |> _create_view(type<GoldMemberRow>,
            db |> select_from(type<MemberRow>) |> _where(_.TierVal == want))
        let rows <- db |> select_from(type<GoldMemberRow>)
        t |> equal(length(rows), 1, "exactly one Gold member")
        t |> equal(rows[0].Id, 3, "Id 3 is Gold")
    }
}
