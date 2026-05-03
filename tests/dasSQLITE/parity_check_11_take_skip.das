options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 11 — `take(n)` / `skip(n)` pagination.
//
// IMPORTANT: SQLite's LIMIT/OFFSET without ORDER BY is not deterministic
// (the tutorial explicitly warns about this), and a plain in-memory `take`
// over an array IS positional. Bare `take(2)` therefore has no parity
// claim — Mode 1's row order is provider-specific, Mode 3's is fixed.
// All parity tests below pair take/skip with `_order_by` so the order is
// well-defined in every mode.
//
// Shapes:
//   - `_order_by |> take(n)`
//   - `_order_by |> skip(n)`
//   - `_order_by |> take(n) |> skip(m)` (windowed)
//   - `_where |> _order_by |> take(n)` (composed)
//   - `_order_by |> take(n) |> _first()` (terminal override)

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array() : array<Car> {
    var arr : array<Car>
    arr |> emplace(Car(Id = 1, Name = "BMW",   Price = 100))
    arr |> emplace(Car(Id = 2, Name = "Audi",  Price = 200))
    arr |> emplace(Car(Id = 3, Name = "Tesla", Price = 300))
    arr |> emplace(Car(Id = 4, Name = "Lada",  Price = 50))
    arr |> emplace(Car(Id = 5, Name = "Lambo", Price = 999))
    return <- arr
}

def cars_equal(a : array<Car>; b : array<Car>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i].Id != b[i].Id || a[i].Name != b[i].Name || a[i].Price != b[i].Price) {
            return false
        }
    }
    return true
}

[test]
def parity_order_take(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(2))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(2))
        let m3 <- (arr |> _order_by(_.Price) |> take(2))

        t |> success(cars_equal(m1, m2), "order|take: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "order|take: M2 ≡ M3")
    }
}

[test]
def parity_order_skip(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> skip(3))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> skip(3))
        let m3 <- (arr |> _order_by(_.Price) |> skip(3))

        t |> success(cars_equal(m1, m2), "order|skip: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "order|skip: M2 ≡ M3")
    }
}

[test]
def parity_order_take_skip(t : T?) {
    // F1 RESOLVED via multi-Q lowering. `take(2) |> skip(1)` (linq positional:
    // take 2, then drop 1) lowers to a nested SELECT under `_sql`:
    //
    //   SELECT … FROM (SELECT … LIMIT 2) AS "t0" LIMIT -1 OFFSET 1
    //
    // All three modes now agree. For the canonical `LIMIT n OFFSET m` shape
    // (`skip(1) |> take(2)`), see `parity_order_skip_then_take` below — that
    // stays a flat SELECT.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(2) |> skip(1))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(2) |> skip(1))
        let m3 <- (arr |> _order_by(_.Price) |> take(2) |> skip(1))

        t |> success(cars_equal(m1, m2), "order|take|skip: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "order|take|skip: M2 ≡ M3")
    }
}

[test]
def parity_order_take_skip_n_gt_m(t : T?) {
    // n > m: take(4) → 4 rows, skip(2) drops 2 → 2 rows. All three modes agree.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(4) |> skip(2))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(4) |> skip(2))
        let m3 <- (arr |> _order_by(_.Price) |> take(4) |> skip(2))

        t |> success(cars_equal(m1, m2), "order|take(4)|skip(2): M1 ≡ M2 (2 rows)")
        t |> success(cars_equal(m2, m3), "order|take(4)|skip(2): M2 ≡ M3")
    }
}

[test]
def parity_order_take_skip_degenerate(t : T?) {
    // m > n: take(2), skip(5) drops everything → 0 rows. All three modes agree.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(2) |> skip(5))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(2) |> skip(5))
        let m3 <- (arr |> _order_by(_.Price) |> take(2) |> skip(5))

        t |> equal(length(m1), 0, "order|take(2)|skip(5): M1 → 0 rows")
        t |> equal(length(m2), 0, "order|take(2)|skip(5): M2 → 0 rows")
        t |> equal(length(m3), 0, "order|take(2)|skip(5): M3 → 0 rows")
    }
}

[test]
def parity_order_skip_then_take(t : T?) {
    // Workaround for the divergence: writing `skip(m) |> take(n)` instead of
    // `take(n) |> skip(m)` makes the chain order match SQL's LIMIT/OFFSET
    // semantics (skip first, then take). All three modes agree.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> skip(1) |> take(2))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> skip(1) |> take(2))
        let m3 <- (arr |> _order_by(_.Price) |> skip(1) |> take(2))

        t |> success(cars_equal(m1, m2), "skip|take: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "skip|take: M2 ≡ M3")
    }
}

[test]
def parity_where_order_take(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()
        let threshold = 100

        let m1 <- _sql(db |> select_from(type<Car>)
                         |> _where(_.Price > threshold)
                         |> _order_by(_.Price)
                         |> take(2))
        let m2 <- (db |> select_from(type<Car>)
                     |> _where(_.Price > threshold)
                     |> _order_by(_.Price)
                     |> take(2))
        let m3 <- (arr |> _where(_.Price > threshold)
                       |> _order_by(_.Price)
                       |> take(2))

        t |> success(cars_equal(m1, m2), "where|order|take: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "where|order|take: M2 ≡ M3")
    }
}

[test]
def parity_take_first_terminal(t : T?) {
    // `_first()` overrides take in `_sql` (emits LIMIT 1). In linq,
    // take(5) |> _first() also returns the first element. Result should
    // be the same Car in all three modes.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> take(5) |> _first())
        let m2 = (db |> select_from(type<Car>) |> _order_by(_.Price) |> take(5) |> _first())
        let m3 = (arr |> _order_by(_.Price) |> take(5) |> _first())

        t |> equal(m1.Id,    m2.Id,    "take|_first: M1.Id ≡ M2.Id")
        t |> equal(m1.Name,  m2.Name,  "take|_first: M1.Name ≡ M2.Name")
        t |> equal(m1.Price, m2.Price, "take|_first: M1.Price ≡ M2.Price")
        t |> equal(m2.Id,    m3.Id,    "take|_first: M2.Id ≡ M3.Id")
        t |> equal(m2.Name,  m3.Name,  "take|_first: M2.Name ≡ M3.Name")
        t |> equal(m2.Price, m3.Price, "take|_first: M2.Price ≡ M3.Price")
    }
}
