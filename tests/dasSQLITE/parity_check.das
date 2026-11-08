options gen2

require dastest/testing_boost public
require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require daslib/sql_linq
require daslib/option
require daslib/result
require daslib/strings_boost
require math
require daslib/cmp
require daslib/linq

// Parity check for tutorial 04 — `_sql` chain ≡ no-`_sql` chain (over
// the same SQL source) ≡ same chain over a plain `array<T>`. Three
// modes must produce identical results for every chain shape the
// tutorial uses.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
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
    return <- [
        Car(Id = 1, Name = "BMW",   Price = 100),
        Car(Id = 2, Name = "Audi",  Price = 200),
        Car(Id = 3, Name = "Tesla", Price = 300),
        Car(Id = 4, Name = "Lada",  Price = 50),
        Car(Id = 5, Name = "Lambo", Price = 999)
    ]
}

def cars_equal(a : array<Car>; b : array<Car>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Price != eb.Price) {
            return false
        }
    }
    return true
}

[test(description = "selecting all rows gives identical results with _sql, a plain chain over the SQL source, and a plain array")]
def parity_select_all(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>))
        let m2 <- (db |> select_from(type<Car>))

        t |> success(cars_equal(m1, m2),
                     "select_all: Mode 1 (_sql) ≡ Mode 2 (no _sql, SQL source)")
        t |> success(cars_equal(m2, arr),
                     "select_all: Mode 2 (SQL source) ≡ Mode 3 (plain array)")
    }
}

[test(description = "a where filter gives identical results in all three modes")]
def parity_where_filter(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()
        let threshold = 200

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Price > threshold))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Price > threshold))
        let m3 <- (arr |> _where(_.Price > threshold))

        t |> success(cars_equal(m1, m2), "where: Mode 1 (_sql) ≡ Mode 2 (no _sql, SQL source)")
        t |> success(cars_equal(m2, m3), "where: Mode 2 (SQL source) ≡ Mode 3 (plain array)")
    }
}

[test(description = "a single-column projection gives identical results in all three modes")]
def parity_project_name(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _select(_.Name))
        let m2 <- (db |> select_from(type<Car>) |> _select(_.Name))
        let m3 <- (arr |> _select(_.Name))

        t |> equal(length(m1), length(m2), "project: Mode 1 length ≡ Mode 2 length")
        t |> equal(length(m2), length(m3), "project: Mode 2 length ≡ Mode 3 length")
        for (i in range(length(m1))) {
            t |> equal(m1[i], m2[i], "project[{i}]: Mode 1 ≡ Mode 2")
            t |> equal(m2[i], m3[i], "project[{i}]: Mode 2 ≡ Mode 3")
        }
    }
}

// Parity check for tutorial 05 — `_where(...) |> _first()` and
// `_where(...) |> _first_opt()`. Chain semantics across the three
// modes.
//
// The raw-SQL escape hatch (`query_one(sql, type<T>, args...)`) is
// NOT covered here — it is a non-chain code path with no Mode-3
// analog.

def fixture_db_05(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
}

def fixture_array_05() : array<Car> {
    return <- [
        Car(Id = 1, Name = "BMW",   Price = 100),
        Car(Id = 2, Name = "Audi",  Price = 200),
        Car(Id = 3, Name = "Tesla", Price = 300)
    ]
}

def car_eq(a : Car; b : Car) : bool => a.Id == b.Id && a.Name == b.Name && a.Price == b.Price

[test(description = "where followed by first returns the same row in all three modes")]
def parity_first_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_05(db)
        var arr <- fixture_array_05()
        let target = 3

        let m1 = _sql(db |> select_from(type<Car>) |> _where(_.Id == target) |> _first())
        let m2 = db |> select_from(type<Car>) |> _where(_.Id == target) |> _first()
        let m3 = arr |> _where(_.Id == target) |> _first()

        t |> success(car_eq(m1, m2), "first: Mode 1 (_sql) ≡ Mode 2 (no _sql, SQL source)")
        t |> success(car_eq(m2, m3), "first: Mode 2 (SQL source) ≡ Mode 3 (plain array)")
    }
}

[test(description = "first_opt with no match is none in all three modes")]
def parity_first_opt_miss(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_05(db)
        var arr <- fixture_array_05()

        let m1 = _sql(db |> select_from(type<Car>) |> _where(_.Id == 999) |> _first_opt())
        let m2 = db |> select_from(type<Car>) |> _where(_.Id == 999) |> _first_opt()
        let m3 = arr |> _where(_.Id == 999) |> _first_opt()

        t |> success(m1 |> is_none, "first_opt(miss): Mode 1 produces none")
        t |> success(m2 |> is_none, "first_opt(miss): Mode 2 produces none")
        t |> success(m3 |> is_none, "first_opt(miss): Mode 3 produces none")
    }
}

[test(description = "first_opt with a match returns the same row in all three modes")]
def parity_first_opt_hit(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_05(db)
        var arr <- fixture_array_05()
        let target = 2

        let m1 = _sql(db |> select_from(type<Car>) |> _where(_.Id == target) |> _first_opt())
        let m2 = db |> select_from(type<Car>) |> _where(_.Id == target) |> _first_opt()
        let m3 = arr |> _where(_.Id == target) |> _first_opt()

        t |> success(m1 |> is_some, "first_opt(hit): Mode 1 produces some")
        t |> success(m2 |> is_some, "first_opt(hit): Mode 2 produces some")
        t |> success(m3 |> is_some, "first_opt(hit): Mode 3 produces some")
        t |> success(car_eq(m1 |> unwrap, m2 |> unwrap), "first_opt(hit): Mode 1 ≡ Mode 2 value")
        t |> success(car_eq(m2 |> unwrap, m3 |> unwrap), "first_opt(hit): Mode 2 ≡ Mode 3 value")
    }
}

// Parity check for tutorial 06 — `_try_sql(chain)` is `_sql(chain)`
// wrapped in `Result<T, string>` for SQL-side prepare/step failures.
// The INNER chain (`_first()`, `_first_opt()`) is the same linq we
// verified in tut 05; the new piece is that `_try_sql(...).unwrap()`
// on the ok path must agree with the Mode-2 / Mode-3 results.
//
// On the err path (e.g. `_first()` on an empty table), Modes 2 and 3
// PANIC — no parity assertion is possible there; the file only
// confirms Mode 1 returns Err as a sanity check.
//
// Raw-SQL helpers (`try_open_sqlite`, `try_insert`,
// `query_scalar_opt`, `try_exec`) are out of scope per API_CHECKED.md
// — non-chain code paths.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

def fixture_db_06(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice"))
    db |> insert(User(Id = 2, Name = "bob"))
}

def fixture_array_06() : array<User> {
    return <- [
        User(Id = 1, Name = "alice"),
        User(Id = 2, Name = "bob")
    ]
}

def user_eq(a : User; b : User) : bool => a.Id == b.Id && a.Name == b.Name

[test(description = "_try_sql with first returns Ok on a populated table with the row all modes agree on")]
def parity_try_sql_first_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_06(db)
        let arr <- fixture_array_06()

        // Mode 1: _try_sql(...) returns Result<User, string>; ok-path here.
        let r1 = _try_sql(db |> select_from(type<User>) |> _first())
        // Mode 2 / Mode 3: same inner chain, no error wrapping.
        let m2 = db |> select_from(type<User>) |> _first()
        let m3 = arr |> _first()

        t |> success(r1 |> is_ok, "try_sql(_first): ok on populated table")
        t |> success(user_eq(r1 |> unwrap, m2), "try_sql(_first).unwrap() ≡ Mode 2")
        t |> success(user_eq(m2, m3),            "Mode 2 ≡ Mode 3")
    }
}

[test(description = "_try_sql with first_opt returns Ok wrapping the same some value as the other modes")]
def parity_try_sql_first_opt_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_06(db)
        let arr <- fixture_array_06()

        // Mode 1: _try_sql(... |> _first_opt()) → Result<Option<User>,
        // string>.
        let r1 = _try_sql(db |> select_from(type<User>) |> _first_opt())
        let m2 = db |> select_from(type<User>) |> _first_opt()
        let m3 = arr |> _first_opt()

        t |> success(r1 |> is_ok, "try_sql(_first_opt): ok on populated table")
        let o1 = r1 |> unwrap
        t |> success(o1 |> is_some && m2 |> is_some && m3 |> is_some,
            "all three: some on populated table")
        t |> success(user_eq(o1 |> unwrap, m2 |> unwrap),
                     "try_sql(_first_opt).unwrap().unwrap() ≡ Mode 2")
        t |> success(user_eq(m2 |> unwrap, m3 |> unwrap), "Mode 2 ≡ Mode 3")
    }
}

[test(description = "_try_sql with first_opt on an empty table returns Ok wrapping none like the other modes")]
def parity_try_sql_first_opt_none(t : T?) {
    with_sqlite(":memory:") $(db) {
        // Empty table — _first_opt returns none across all three modes.
        db |> create_table(type<User>)
        let arr : array<User>

        let r1 = _try_sql(db |> select_from(type<User>) |> _first_opt())
        let m2 = db |> select_from(type<User>) |> _first_opt()
        let m3 = arr |> _first_opt()

        t |> success(r1 |> is_ok, "try_sql(_first_opt): ok on empty (0 rows is not an error)")
        t |> success((r1 |> unwrap) |> is_none, "Mode 1: none on empty")
        t |> success(m2 |> is_none,             "Mode 2: none on empty")
        t |> success(m3 |> is_none,             "Mode 3: none on empty")
    }
}

[test(description = "_try_sql with first on an empty table returns Err")]
def sanity_try_sql_first_empty_errs(t : T?) {
    // Sanity check (Mode 1 only): _try_sql(... |> _first()) on empty
    // table returns Err. Modes 2 and 3 PANIC on empty `_first()` — no
    // parity assertion is possible (a panic would terminate the test
    // process).
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        let r1 = _try_sql(db |> select_from(type<User>) |> _first())
        t |> success(r1 |> is_err,
                     "try_sql(_first) on empty table → Err (Mode 1 only; M2/M3 would panic)")
    }
}

// Parity check for tutorial 07 — `_where` mechanics introduced beyond
// tut 04: symmetric column-vs-bind sides (`_.Price > cutoff` ≡
// `cutoff < _.Price`), chained `_where` (composes with AND), and the
// `starts_with` string predicate inside `_where`.
//
// `_sql_text` (debug emitter) and `query_scalar` (raw SQL) are out of
// scope per API_CHECKED.md.

[sql_table(name = "Cars")]
struct Car_07 {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db_07(db : SqlRunner) {
    db |> create_table(type<Car_07>)
    db |> insert(Car_07(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car_07(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car_07(Id = 3, Name = "Tesla", Price = 300))
}

def fixture_array_07() : array<Car_07> {
    return <- [
        Car_07(Id = 1, Name = "BMW",   Price = 100),
        Car_07(Id = 2, Name = "Audi",  Price = 200),
        Car_07(Id = 3, Name = "Tesla", Price = 300)
    ]
}

def cars_equal_07(a : array<Car_07>; b : array<Car_07>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Price != eb.Price) {
            return false
        }
    }
    return true
}

[test(description = "a where gives the same rows whether the column is on the left or the right of the comparison, in all three modes")]
def parity_where_symmetric(t : T?) {
    // The translation should be order-independent: `_.Price > cutoff` and
    // `cutoff < _.Price` must produce the same row set across all three
    // modes.
    with_sqlite(":memory:") $(db) {
        fixture_db_07(db)
        var arr <- fixture_array_07()
        let cutoff = 150

        let m1_a <- _sql(db |> select_from(type<Car_07>) |> _where(_.Price > cutoff))
        let m1_b <- _sql(db |> select_from(type<Car_07>) |> _where(cutoff < _.Price))
        let m2_a <- (db |> select_from(type<Car_07>) |> _where(_.Price > cutoff))
        let m2_b <- (db |> select_from(type<Car_07>) |> _where(cutoff < _.Price))
        let m3_a <- (arr |> _where(_.Price > cutoff))
        let m3_b <- (arr |> _where(cutoff < _.Price))

        t |> success(cars_equal_07(m1_a, m1_b), "Mode 1: column-left ≡ column-right")
        t |> success(cars_equal_07(m2_a, m2_b), "Mode 2: column-left ≡ column-right")
        t |> success(cars_equal_07(m3_a, m3_b), "Mode 3: column-left ≡ column-right")
        t |> success(cars_equal_07(m1_a, m2_a), "Mode 1 ≡ Mode 2 (column-left form)")
        t |> success(cars_equal_07(m2_a, m3_a), "Mode 2 ≡ Mode 3 (column-left form)")
    }
}

[test(description = "chained where calls combine as AND identically in all three modes")]
def parity_where_chained_and(t : T?) {
    // Two `_where` calls compose with AND in `_sql` (and trivially in
    // linq — each filter narrows the array further).
    with_sqlite(":memory:") $(db) {
        fixture_db_07(db)
        var arr <- fixture_array_07()

        let m1 <- _sql(db |> select_from(type<Car_07>)
                         |> _where(_.Price > 100)
                         |> _where(_.Name |> starts_with("T")))
        let m2 <- (db |> select_from(type<Car_07>)
                     |> _where(_.Price > 100)
                     |> _where(_.Name |> starts_with("T")))
        let m3 <- (arr |> _where(_.Price > 100)
                       |> _where(_.Name |> starts_with("T")))

        t |> success(cars_equal_07(m1, m2), "chained-where: Mode 1 ≡ Mode 2")
        t |> success(cars_equal_07(m2, m3), "chained-where: Mode 2 ≡ Mode 3")
    }
}

[test(description = "a starts_with predicate gives identical results in all three modes")]
def parity_where_starts_with(t : T?) {
    // `starts_with` as a string predicate inside `_where`. SQL emits
    // `LIKE ? ESCAPE '\'` with the appropriate pattern; the in-memory
    // linq path calls `strings_boost::starts_with`. Results must agree.
    with_sqlite(":memory:") $(db) {
        fixture_db_07(db)
        var arr <- fixture_array_07()

        let m1 <- _sql(db |> select_from(type<Car_07>) |> _where(_.Name |> starts_with("T")))
        let m2 <- (db |> select_from(type<Car_07>) |> _where(_.Name |> starts_with("T")))
        let m3 <- (arr |> _where(_.Name |> starts_with("T")))

        t |> success(cars_equal_07(m1, m2), "starts_with: Mode 1 ≡ Mode 2")
        t |> success(cars_equal_07(m2, m3), "starts_with: Mode 2 ≡ Mode 3")
    }
}

// Parity check for tutorial 08 — the full `_where` predicate surface.
// Each shape recognized by the `_sql` predicate analyzer must produce
// the same row set in Mode 1, Mode 2, and Mode 3.
//
// Shapes covered (NEW vs tut 04/05/07):
//   - logic op `||` mixed with string predicate
//   - `ends_with(s)`
//   - `contains(s)`
//   - case folding (`to_lower() == ...`)
//   - free-function `length(_.Field)`
//   - numeric `abs(_.Field)`
//
// `_sql_text` (debug emitter) is out of scope per API_CHECKED.md.

def fixture_db_08(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "Ford",        Price = 100))
    db |> insert(Car(Id = 2, Name = "Ferrari",     Price = 9999))
    db |> insert(Car(Id = 3, Name = "Honda",       Price = 200))
    db |> insert(Car(Id = 4, Name = "Lada",        Price = 50))
    db |> insert(Car(Id = 5, Name = "Lamborghini", Price = 88888))
}

def fixture_array_08() : array<Car> {
    return <- [
        Car(Id = 1, Name = "Ford",        Price = 100),
        Car(Id = 2, Name = "Ferrari",     Price = 9999),
        Car(Id = 3, Name = "Honda",       Price = 200),
        Car(Id = 4, Name = "Lada",        Price = 50),
        Car(Id = 5, Name = "Lamborghini", Price = 88888)
    ]
}

[test(description = "an or combined with a string predicate gives identical results in all three modes")]
def parity_where_or_with_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_08(db)
        var arr <- fixture_array_08()

        let m1 <- _sql(db |> select_from(type<Car>)
                         |> _where(_.Price < 100 || (_.Name |> starts_with("F"))))
        let m2 <- (db |> select_from(type<Car>)
                     |> _where(_.Price < 100 || (_.Name |> starts_with("F"))))
        let m3 <- (arr |> _where(_.Price < 100 || (_.Name |> starts_with("F"))))

        t |> success(cars_equal(m1, m2), "OR + starts_with: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "OR + starts_with: Mode 2 ≡ Mode 3")
    }
}

[test(description = "an ends_with predicate gives identical results in all three modes")]
def parity_where_ends_with(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_08(db)
        var arr <- fixture_array_08()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Name |> ends_with("da")))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Name |> ends_with("da")))
        let m3 <- (arr |> _where(_.Name |> ends_with("da")))

        t |> success(cars_equal(m1, m2), "ends_with: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "ends_with: Mode 2 ≡ Mode 3")
    }
}

[test(description = "a contains predicate gives identical results in all three modes")]
def parity_where_contains(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_08(db)
        var arr <- fixture_array_08()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Name |> contains("o")))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Name |> contains("o")))
        let m3 <- (arr |> _where(_.Name |> contains("o")))

        t |> success(cars_equal(m1, m2), "contains: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "contains: Mode 2 ≡ Mode 3")
    }
}

[test(description = "a to_lower comparison gives identical results in all three modes")]
def parity_where_to_lower_eq(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_08(db)
        var arr <- fixture_array_08()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(_.Name |> to_lower() == "ford"))
        let m2 <- (db |> select_from(type<Car>) |> _where(_.Name |> to_lower() == "ford"))
        let m3 <- (arr |> _where(_.Name |> to_lower() == "ford"))

        t |> success(cars_equal(m1, m2), "to_lower==: Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "to_lower==: Mode 2 ≡ Mode 3")
    }
}

[test(description = "a string length predicate gives identical results in all three modes")]
def parity_where_string_length(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_08(db)
        var arr <- fixture_array_08()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where(length(_.Name) > 5))
        let m2 <- (db |> select_from(type<Car>) |> _where(length(_.Name) > 5))
        let m3 <- (arr |> _where(length(_.Name) > 5))

        t |> success(cars_equal(m1, m2), "length(string): Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "length(string): Mode 2 ≡ Mode 3")
    }
}

[test(description = "an abs predicate gives identical results in all three modes")]
def parity_where_abs(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_08(db)
        var arr <- fixture_array_08()

        let m1 <- _sql(db |> select_from(type<Car>) |> _where((_.Price |> abs()) > 5000))
        let m2 <- (db |> select_from(type<Car>) |> _where((_.Price |> abs()) > 5000))
        let m3 <- (arr |> _where((_.Price |> abs()) > 5000))

        t |> success(cars_equal(m1, m2), "abs(int): Mode 1 ≡ Mode 2")
        t |> success(cars_equal(m2, m3), "abs(int): Mode 2 ≡ Mode 3")
    }
}

// Parity check for tutorial 09 — `_select` projections.
//
// Default (full row) and single-column `_select(_.Field)` were
// covered in tut 04. New here: - named-tuple projection
// `_select((Name = _.Name, Price = _.Price))` - renaming via named
// tuple - named-tuple + terminal: `_select((...)) |> _first()`
//
// `_sql_text` is out of scope per API_CHECKED.md.

def fixture_db_09(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
}

def fixture_array_09() : array<Car> {
    return <- [
        Car(Id = 1, Name = "BMW",   Price = 100),
        Car(Id = 2, Name = "Audi",  Price = 200),
        Car(Id = 3, Name = "Tesla", Price = 300)
    ]
}

[test(description = "a named-tuple select gives identical rows in all three modes")]
def parity_select_named_tuple(t : T?) {
    // Named-tuple projection: result element is
    // tuple<Name:string;Price:int> in all three modes; recordNames
    // preserved so .Name / .Price access works.
    with_sqlite(":memory:") $(db) {
        fixture_db_09(db)
        var arr <- fixture_array_09()

        let m1 <- _sql(db |> select_from(type<Car>) |> _select((Name = _.Name, Price = _.Price)))
        let m2 <- (db |> select_from(type<Car>) |> _select((Name = _.Name, Price = _.Price)))
        let m3 <- (arr |> _select((Name = _.Name, Price = _.Price)))

        t |> equal(length(m1), length(m2), "named-tuple length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "named-tuple length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Name,  m2[i].Name,  "row[{i}].Name: M1 ≡ M2")
            t |> equal(m1[i].Price, m2[i].Price, "row[{i}].Price: M1 ≡ M2")
            t |> equal(m2[i].Name,  m3[i].Name,  "row[{i}].Name: M2 ≡ M3")
            t |> equal(m2[i].Price, m3[i].Price, "row[{i}].Price: M2 ≡ M3")
        }
    }
}

[test(description = "a named-tuple select that renames fields gives identical rows in all three modes")]
def parity_select_renamed(t : T?) {
    // Renaming: result fields differ from source columns. The tuple shape
    // (Identifier:int; Label:string) must agree across all three modes.
    with_sqlite(":memory:") $(db) {
        fixture_db_09(db)
        var arr <- fixture_array_09()

        let m1 <- _sql(db |> select_from(type<Car>) |> _select((Identifier = _.Id, Label = _.Name)))
        let m2 <- (db |> select_from(type<Car>) |> _select((Identifier = _.Id, Label = _.Name)))
        let m3 <- (arr |> _select((Identifier = _.Id, Label = _.Name)))

        t |> equal(length(m1), length(m2), "renamed length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "renamed length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Identifier, m2[i].Identifier, "row[{i}].Identifier: M1 ≡ M2")
            t |> equal(m1[i].Label,      m2[i].Label,      "row[{i}].Label: M1 ≡ M2")
            t |> equal(m2[i].Identifier, m3[i].Identifier, "row[{i}].Identifier: M2 ≡ M3")
            t |> equal(m2[i].Label,      m3[i].Label,      "row[{i}].Label: M2 ≡ M3")
        }
    }
}

[test(description = "a named-tuple select followed by first gives the same head in all three modes")]
def parity_select_named_tuple_first(t : T?) {
    // Named-tuple projection + `_first()` terminal: result is the tuple
    // itself.
    with_sqlite(":memory:") $(db) {
        fixture_db_09(db)
        var arr <- fixture_array_09()

        let m1 = _sql(db |> select_from(type<Car>)
                        |> _select((Name = _.Name, Price = _.Price))
                        |> _first())
        let m2 = (db |> select_from(type<Car>)
                    |> _select((Name = _.Name, Price = _.Price))
                    |> _first())
        let m3 = arr |> _select((Name = _.Name, Price = _.Price)) |> _first()

        t |> equal(m1.Name,  m2.Name,  "head.Name: M1 ≡ M2")
        t |> equal(m1.Price, m2.Price, "head.Price: M1 ≡ M2")
        t |> equal(m2.Name,  m3.Name,  "head.Name: M2 ≡ M3")
        t |> equal(m2.Price, m3.Price, "head.Price: M2 ≡ M3")
    }
}

// Parity check for tutorial 10 — `_order_by` and
// `_order_by_descending`.
//
// Shapes:
//   - `_order_by(_.Price)`            → ORDER BY single column ASC
//   - `_order_by_descending(_.Price)` → DESC variant
// - `_order_by((_.Price, _.Name))`  → multi-column tuple key
// (tie-break) - composed: `_where |> _order_by |> take`
//
// Note: the tutorial fixture has Price ties (BMW=100, Tesla=100). For
// the single-column `_order_by(_.Price)` test, sort stability is
// *not* part of the parity claim — we only assert the resulting Price
// sequence matches across modes, ignoring the relative order of tied
// rows. The tuple-key test eliminates ties by design.

[sql_table(name = "Cars")]
struct Car_10 {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db_10(db : SqlRunner) {
    db |> create_table(type<Car_10>)
    db |> insert(Car_10(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car_10(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car_10(Id = 3, Name = "Tesla", Price = 100))
    db |> insert(Car_10(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car_10(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array_10() : array<Car_10> {
    return <- [
        Car_10(Id = 1, Name = "BMW",   Price = 100),
        Car_10(Id = 2, Name = "Audi",  Price = 200),
        Car_10(Id = 3, Name = "Tesla", Price = 100),
        Car_10(Id = 4, Name = "Lada",  Price = 50),
        Car_10(Id = 5, Name = "Lambo", Price = 999)
    ]
}

def cars_equal_10(a : array<Car_10>; b : array<Car_10>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Price != eb.Price) {
            return false
        }
    }
    return true
}

def prices_of(cars : array<Car_10>) : array<int> => <- [for (c in cars); c.Price]

def int_array_eq(a : array<int>; b : array<int>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea != eb) {
            return false
        }
    }
    return true
}

[test(description = "order_by ascending gives the same price sequence in all three modes")]
def parity_order_by_single_asc(t : T?) {
    // Sort-stability is not part of the parity claim — compare Price
    // sequence.
    with_sqlite(":memory:") $(db) {
        fixture_db_10(db)
        let arr <- fixture_array_10()

        let m1 <- _sql(db |> select_from(type<Car_10>) |> _order_by(_.Price))
        let m2 <- (db |> select_from(type<Car_10>) |> _order_by(_.Price))
        let m3 <- (arr |> _order_by(_.Price))

        let p1 <- prices_of(m1)
        let p2 <- prices_of(m2)
        let p3 <- prices_of(m3)

        t |> success(int_array_eq(p1, p2), "ASC: Price sequence M1 ≡ M2")
        t |> success(int_array_eq(p2, p3), "ASC: Price sequence M2 ≡ M3")
    }
}

[test(description = "order_by descending gives the same price sequence in all three modes")]
def parity_order_by_single_desc(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_10(db)
        let arr <- fixture_array_10()

        let m1 <- _sql(db |> select_from(type<Car_10>) |> _order_by_descending(_.Price))
        let m2 <- (db |> select_from(type<Car_10>) |> _order_by_descending(_.Price))
        let m3 <- (arr |> _order_by_descending(_.Price))

        let p1 <- prices_of(m1)
        let p2 <- prices_of(m2)
        let p3 <- prices_of(m3)

        t |> success(int_array_eq(p1, p2), "DESC: Price sequence M1 ≡ M2")
        t |> success(int_array_eq(p2, p3), "DESC: Price sequence M2 ≡ M3")
    }
}

[test(description = "a tuple order key gives identical rows in all three modes")]
def parity_order_by_tuple_key(t : T?) {
    // Tuple-key (_.Price, _.Name) eliminates ties — exact full-row order
    // must agree across all three modes.
    with_sqlite(":memory:") $(db) {
        fixture_db_10(db)
        let arr <- fixture_array_10()

        let m1 <- _sql(db |> select_from(type<Car_10>) |> _order_by((_.Price, _.Name)))
        let m2 <- (db |> select_from(type<Car_10>) |> _order_by((_.Price, _.Name)))
        let m3 <- (arr |> _order_by((_.Price, _.Name)))

        t |> success(cars_equal_10(m1, m2), "tuple-key: M1 ≡ M2")
        t |> success(cars_equal_10(m2, m3), "tuple-key: M2 ≡ M3")
    }
}

[test(description = "where, order_by and take together give identical rows in all three modes")]
def parity_where_orderby_take(t : T?) {
    // Composed chain: filter, sort, limit. The tuple key avoids tie
    // ambiguity in the sort step.
    with_sqlite(":memory:") $(db) {
        fixture_db_10(db)
        var arr <- fixture_array_10()

        let m1 <- _sql(db |> select_from(type<Car_10>)
                         |> _where(_.Price > 50)
                         |> _order_by((_.Price, _.Name))
                         |> take(3))
        let m2 <- (db |> select_from(type<Car_10>)
                     |> _where(_.Price > 50)
                     |> _order_by((_.Price, _.Name))
                     |> take(3))
        let m3 <- (arr |> _where(_.Price > 50)
                       |> _order_by((_.Price, _.Name))
                       |> take(3))

        t |> success(cars_equal_10(m1, m2), "where|order_by|take: M1 ≡ M2")
        t |> success(cars_equal_10(m2, m3), "where|order_by|take: M2 ≡ M3")
    }
}

// Parity check for tutorial 11 — `take(n)` / `skip(n)` pagination.
//
// IMPORTANT: SQLite's LIMIT/OFFSET without ORDER BY is not
// deterministic (the tutorial explicitly warns about this), and a
// plain in-memory `take` over an array IS positional. Bare `take(2)`
// therefore has no parity claim — Mode 1's row order is
// provider-specific, Mode 3's is fixed. All parity tests below pair
// take/skip with `_order_by` so the order is well-defined in every
// mode.
//
// Shapes:
//   - `_order_by |> take(n)`
//   - `_order_by |> skip(n)`
//   - `_order_by |> take(n) |> skip(m)` (windowed)
//   - `_where |> _order_by |> take(n)` (composed)
//   - `_order_by |> take(n) |> _first()` (terminal override)

[sql_table(name = "Cars")]
struct Car_11 {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db_11(db : SqlRunner) {
    db |> create_table(type<Car_11>)
    db |> insert(Car_11(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car_11(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car_11(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car_11(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car_11(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array_11() : array<Car_11> {
    return <- [
        Car_11(Id = 1, Name = "BMW",   Price = 100),
        Car_11(Id = 2, Name = "Audi",  Price = 200),
        Car_11(Id = 3, Name = "Tesla", Price = 300),
        Car_11(Id = 4, Name = "Lada",  Price = 50),
        Car_11(Id = 5, Name = "Lambo", Price = 999)
    ]
}

def cars_equal_11(a : array<Car_11>; b : array<Car_11>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Price != eb.Price) {
            return false
        }
    }
    return true
}

[test(description = "order_by then take gives identical rows in all three modes")]
def parity_order_take(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_11(db)
        let arr <- fixture_array_11()

        let m1 <- _sql(db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(2))
        let m2 <- (db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(2))
        let m3 <- (arr |> _order_by(_.Price) |> take(2))

        t |> success(cars_equal_11(m1, m2), "order|take: M1 ≡ M2")
        t |> success(cars_equal_11(m2, m3), "order|take: M2 ≡ M3")
    }
}

[test(description = "order_by then skip gives identical rows in all three modes")]
def parity_order_skip(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_11(db)
        let arr <- fixture_array_11()

        let m1 <- _sql(db |> select_from(type<Car_11>) |> _order_by(_.Price) |> skip(3))
        let m2 <- (db |> select_from(type<Car_11>) |> _order_by(_.Price) |> skip(3))
        let m3 <- (arr |> _order_by(_.Price) |> skip(3))

        t |> success(cars_equal_11(m1, m2), "order|skip: M1 ≡ M2")
        t |> success(cars_equal_11(m2, m3), "order|skip: M2 ≡ M3")
    }
}

[test(description = "order_by, take then skip give identical rows in all three modes")]
def parity_order_take_skip(t : T?) {
    // F1 RESOLVED via multi-Q lowering. `take(2) |> skip(1)` (linq
    // positional: take 2, then drop 1) lowers to a nested SELECT under
    // `_sql`:
    //
    //   SELECT … FROM (SELECT … LIMIT 2) AS "t0" LIMIT -1 OFFSET 1
    //
    // All three modes now agree. For the canonical `LIMIT n OFFSET m`
    // shape (`skip(1) |> take(2)`), see `parity_order_skip_then_take`
    // below — that stays a flat SELECT.
    with_sqlite(":memory:") $(db) {
        fixture_db_11(db)
        let arr <- fixture_array_11()

        let m1 <- _sql(db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(2) |> skip(1))
        let m2 <- (db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(2) |> skip(1))
        let m3 <- (arr |> _order_by(_.Price) |> take(2) |> skip(1))

        t |> success(cars_equal_11(m1, m2), "order|take|skip: M1 ≡ M2")
        t |> success(cars_equal_11(m2, m3), "order|take|skip: M2 ≡ M3")
    }
}

[test(description = "take of more rows than the following skip drops gives identical rows in all three modes")]
def parity_order_take_skip_n_gt_m(t : T?) {
    // n > m: take(4) → 4 rows, skip(2) drops 2 → 2 rows. All three modes
    // agree.
    with_sqlite(":memory:") $(db) {
        fixture_db_11(db)
        let arr <- fixture_array_11()

        let m1 <- _sql(db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(4) |> skip(2))
        let m2 <- (db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(4) |> skip(2))
        let m3 <- (arr |> _order_by(_.Price) |> take(4) |> skip(2))

        t |> success(cars_equal_11(m1, m2), "order|take(4)|skip(2): M1 ≡ M2 (2 rows)")
        t |> success(cars_equal_11(m2, m3), "order|take(4)|skip(2): M2 ≡ M3")
    }
}

[test(description = "a skip that drops every taken row gives the same empty result in all three modes")]
def parity_order_take_skip_degenerate(t : T?) {
    // m > n: take(2), skip(5) drops everything → 0 rows. All three modes
    // agree.
    with_sqlite(":memory:") $(db) {
        fixture_db_11(db)
        let arr <- fixture_array_11()

        let m1 <- _sql(db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(2) |> skip(5))
        let m2 <- (db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(2) |> skip(5))
        let m3 <- (arr |> _order_by(_.Price) |> take(2) |> skip(5))

        t |> equal(length(m1), 0, "order|take(2)|skip(5): M1 → 0 rows")
        t |> equal(length(m2), 0, "order|take(2)|skip(5): M2 → 0 rows")
        t |> equal(length(m3), 0, "order|take(2)|skip(5): M3 → 0 rows")
    }
}

[test(description = "order_by, skip then take give identical rows in all three modes")]
def parity_order_skip_then_take(t : T?) {
    // Workaround for the divergence: writing `skip(m) |> take(n)` instead
    // of `take(n) |> skip(m)` makes the chain order match SQL's
    // LIMIT/OFFSET semantics (skip first, then take). All three modes
    // agree.
    with_sqlite(":memory:") $(db) {
        fixture_db_11(db)
        let arr <- fixture_array_11()

        let m1 <- _sql(db |> select_from(type<Car_11>) |> _order_by(_.Price) |> skip(1) |> take(2))
        let m2 <- (db |> select_from(type<Car_11>) |> _order_by(_.Price) |> skip(1) |> take(2))
        let m3 <- (arr |> _order_by(_.Price) |> skip(1) |> take(2))

        t |> success(cars_equal_11(m1, m2), "skip|take: M1 ≡ M2")
        t |> success(cars_equal_11(m2, m3), "skip|take: M2 ≡ M3")
    }
}

[test(description = "where, order_by and take give identical rows in all three modes")]
def parity_where_order_take(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_11(db)
        var arr <- fixture_array_11()
        let threshold = 100

        let m1 <- _sql(db |> select_from(type<Car_11>)
                         |> _where(_.Price > threshold)
                         |> _order_by(_.Price)
                         |> take(2))
        let m2 <- (db |> select_from(type<Car_11>)
                     |> _where(_.Price > threshold)
                     |> _order_by(_.Price)
                     |> take(2))
        let m3 <- (arr |> _where(_.Price > threshold)
                       |> _order_by(_.Price)
                       |> take(2))

        t |> success(cars_equal_11(m1, m2), "where|order|take: M1 ≡ M2")
        t |> success(cars_equal_11(m2, m3), "where|order|take: M2 ≡ M3")
    }
}

[test(description = "take followed by first returns the same row in all three modes")]
def parity_take_first_terminal(t : T?) {
    // `_first()` overrides take in `_sql` (emits LIMIT 1). In linq,
    // take(5) |> _first() also returns the first element. Result should
    // be the same Car_11 in all three modes.
    with_sqlite(":memory:") $(db) {
        fixture_db_11(db)
        let arr <- fixture_array_11()

        let m1 = _sql(db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(5) |> _first())
        let m2 = (db |> select_from(type<Car_11>) |> _order_by(_.Price) |> take(5) |> _first())
        let m3 = (arr |> _order_by(_.Price) |> take(5) |> _first())

        t |> equal(m1.Id,    m2.Id,    "take|_first: M1.Id ≡ M2.Id")
        t |> equal(m1.Name,  m2.Name,  "take|_first: M1.Name ≡ M2.Name")
        t |> equal(m1.Price, m2.Price, "take|_first: M1.Price ≡ M2.Price")
        t |> equal(m2.Id,    m3.Id,    "take|_first: M2.Id ≡ M3.Id")
        t |> equal(m2.Name,  m3.Name,  "take|_first: M2.Name ≡ M3.Name")
        t |> equal(m2.Price, m3.Price, "take|_first: M2.Price ≡ M3.Price")
    }
}

// Multi-Q lowering parity check: outer-Q `_where` over inner-Q
// `take`.
//
//   `... |> _order_by(_.Price) |> take(3) |> _where(_.Price > 100)`
//
// `_sql` lowers the divergent shape to:
// SELECT … FROM (SELECT … ORDER BY "Price" ASC LIMIT 3) AS "t0" WHERE
// "Price" > ?
//
// Linq positional semantics: order by price, take first 3, then
// filter those 3 rows by `Price > 100`. All three modes must agree.

[sql_table(name = "Cars")]
struct Car_11b {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db_11b(db : SqlRunner) {
    db |> create_table(type<Car_11b>)
    db |> insert(Car_11b(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car_11b(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car_11b(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car_11b(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car_11b(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array_11b() : array<Car_11b> {
    return <- [
        Car_11b(Id = 1, Name = "BMW",   Price = 100),
        Car_11b(Id = 2, Name = "Audi",  Price = 200),
        Car_11b(Id = 3, Name = "Tesla", Price = 300),
        Car_11b(Id = 4, Name = "Lada",  Price = 50),
        Car_11b(Id = 5, Name = "Lambo", Price = 999)
    ]
}

def cars_equal_11b(a : array<Car_11b>; b : array<Car_11b>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Price != eb.Price) {
            return false
        }
    }
    return true
}

[test(description = "a where after a take filters the taken rows identically in all three modes")]
def parity_take_then_where(t : T?) {
    // Order by price (Lada=50, BMW=100, Audi=200), take first 3, filter
    // Price > 100. Result: only Audi (Lada and BMW dropped by the filter;
    // Tesla and Lambo never reached).
    with_sqlite(":memory:") $(db) {
        fixture_db_11b(db)
        let arr <- fixture_array_11b()

        let m1 <- _sql(db |> select_from(type<Car_11b>) |> _order_by(_.Price)
            |> take(3) |> _where(_.Price > 100))
        let m2 <- (db |> select_from(type<Car_11b>) |> _order_by(_.Price)
            |> take(3) |> _where(_.Price > 100))
        let m3 <- (arr |> _order_by(_.Price) |> take(3) |> _where(_.Price > 100))

        t |> success(cars_equal_11b(m1, m2), "take|where: M1 ≡ M2")
        t |> success(cars_equal_11b(m2, m3), "take|where: M2 ≡ M3")
    }
}

[test(description = "two wheres after a take filter the taken rows identically in all three modes")]
def parity_take_then_two_wheres(t : T?) {
    // Two outer wheres AND-fold in the same outer Q (same phase, no extra
    // divert).
    with_sqlite(":memory:") $(db) {
        fixture_db_11b(db)
        let arr <- fixture_array_11b()

        let m1 <- _sql(db |> select_from(type<Car_11b>) |> _order_by(_.Price) |> take(4)
                         |> _where(_.Price > 100)
                         |> _where(_.Name != "BMW"))
        let m2 <- (db |> select_from(type<Car_11b>) |> _order_by(_.Price) |> take(4)
                     |> _where(_.Price > 100)
                     |> _where(_.Name != "BMW"))
        let m3 <- (arr |> _order_by(_.Price) |> take(4)
                       |> _where(_.Price > 100)
                       |> _where(_.Name != "BMW"))

        t |> success(cars_equal_11b(m1, m2), "take|where|where: M1 ≡ M2")
        t |> success(cars_equal_11b(m2, m3), "take|where|where: M2 ≡ M3")
    }
}

[test(description = "a where before and after a take gives identical rows in all three modes")]
def parity_inner_where_take_outer_where(t : T?) {
    // Inner Q has its own `_where` (Price > 50 keeps 4 rows), then
    // `_order_by`, then `take(3)`. Outer Q filters those 3 by `Name !=
    // "Lambo"`. Both inner and outer wheres co-exist without conflict.
    with_sqlite(":memory:") $(db) {
        fixture_db_11b(db)
        var arr <- fixture_array_11b()

        let m1 <- _sql(db |> select_from(type<Car_11b>)
                         |> _where(_.Price > 50)
                         |> _order_by(_.Price)
                         |> take(3)
                         |> _where(_.Name != "Lambo"))
        let m2 <- (db |> select_from(type<Car_11b>)
                     |> _where(_.Price > 50)
                     |> _order_by(_.Price)
                     |> take(3)
                     |> _where(_.Name != "Lambo"))
        let m3 <- (arr |> _where(_.Price > 50)
                       |> _order_by(_.Price)
                       |> take(3)
                       |> _where(_.Name != "Lambo"))

        t |> success(cars_equal_11b(m1, m2), "where|order|take|where: M1 ≡ M2")
        t |> success(cars_equal_11b(m2, m3), "where|order|take|where: M2 ≡ M3")
    }
}

[test(description = "take, where and to_array give identical rows in all three modes")]
def parity_take_then_where_to_array(t : T?) {
    // Terminal `_to_array()` in outer Q.
    with_sqlite(":memory:") $(db) {
        fixture_db_11b(db)
        let arr <- fixture_array_11b()

        let m1 <- _sql(db |> select_from(type<Car_11b>) |> _order_by(_.Price) |> take(3)
                         |> _where(_.Price > 100) |> _to_array())
        let m2 <- (db |> select_from(type<Car_11b>) |> _order_by(_.Price) |> take(3)
                     |> _where(_.Price > 100) |> _to_array())
        let m3 <- (arr |> _order_by(_.Price) |> take(3)
                       |> _where(_.Price > 100) |> _to_array())

        t |> success(cars_equal_11b(m1, m2), "take|where|to_array: M1 ≡ M2")
        t |> success(cars_equal_11b(m2, m3), "take|where|to_array: M2 ≡ M3")
    }
}

[test(description = "take, where and first_opt find the same row in all three modes")]
def parity_take_then_where_first_opt(t : T?) {
    // Outer single-row terminal (`_first_opt`) wraps the take|where
    // subquery. Outer LIMIT 1 must NOT collapse the inner LIMIT 3.
    with_sqlite(":memory:") $(db) {
        fixture_db_11b(db)
        let arr <- fixture_array_11b()

        let m1 = _sql(db |> select_from(type<Car_11b>) |> _order_by(_.Price) |> take(3)
                        |> _where(_.Price > 100) |> _first_opt())
        let m2 = (db |> select_from(type<Car_11b>) |> _order_by(_.Price) |> take(3)
                    |> _where(_.Price > 100) |> _first_opt())
        let m3 = (arr |> _order_by(_.Price) |> take(3)
                      |> _where(_.Price > 100) |> _first_opt())

        t |> success(is_some(m1), "take|where|first_opt: M1 has value (Audi)")
        t |> success(is_some(m2), "take|where|first_opt: M2 has value")
        t |> success(is_some(m3), "take|where|first_opt: M3 has value")
        t |> equal((m1 |> unwrap_or(Car_11b(Id = 0, Name = "?", Price = 0))).Name, "Audi",
                   "M1 first match is Audi")
        t |> equal((m2 |> unwrap_or(Car_11b(Id = 0, Name = "?", Price = 0))).Name, "Audi",
                   "M2 first match is Audi")
        t |> equal((m3 |> unwrap_or(Car_11b(Id = 0, Name = "?", Price = 0))).Name, "Audi",
                   "M3 first match is Audi")
    }
}

// Multi-Q lowering parity check: outer-Q `_order_by` over inner-Q
// `take`.
//
//   `... |> _order_by(_.Id) |> take(3) |> _order_by(_.Price)`
//
// Lowers under `_sql` to:
// SELECT … FROM (SELECT … ORDER BY "Id" ASC LIMIT 3) AS "t0" ORDER BY
// "Price" ASC
//
// Linq positional semantics: order by Id (deterministic), take first
// 3, then re-sort those 3 by Price.

[sql_table(name = "Cars")]
struct Car_11c {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db_11c(db : SqlRunner) {
    db |> create_table(type<Car_11c>)
    db |> insert(Car_11c(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car_11c(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car_11c(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car_11c(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car_11c(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array_11c() : array<Car_11c> {
    return <- [
        Car_11c(Id = 1, Name = "BMW",   Price = 100),
        Car_11c(Id = 2, Name = "Audi",  Price = 200),
        Car_11c(Id = 3, Name = "Tesla", Price = 300),
        Car_11c(Id = 4, Name = "Lada",  Price = 50),
        Car_11c(Id = 5, Name = "Lambo", Price = 999)
    ]
}

def cars_equal_11c(a : array<Car_11c>; b : array<Car_11c>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Price != eb.Price) {
            return false
        }
    }
    return true
}

[test(description = "order_by after a take re-sorts the taken rows identically in all three modes")]
def parity_take_then_order(t : T?) {
    // Order by Id, take 3 (BMW=100, Audi=200, Tesla=300), re-sort by
    // Price ascending (BMW=100 < Audi=200 < Tesla=300 — same order; just
    // covers the divert path).
    with_sqlite(":memory:") $(db) {
        fixture_db_11c(db)
        let arr <- fixture_array_11c()

        let m1 <- _sql(db |> select_from(type<Car_11c>) |> _order_by(_.Id)
            |> take(3) |> _order_by(_.Price))
        let m2 <- (db |> select_from(type<Car_11c>) |> _order_by(_.Id)
            |> take(3) |> _order_by(_.Price))
        let m3 <- (arr |> _order_by(_.Id) |> take(3) |> _order_by(_.Price))

        t |> success(cars_equal_11c(m1, m2), "order|take|order: M1 ≡ M2")
        t |> success(cars_equal_11c(m2, m3), "order|take|order: M2 ≡ M3")
    }
}

[test(description = "order_by descending after a take re-sorts the taken rows identically in all three modes")]
def parity_take_then_order_desc(t : T?) {
    // Inner: order by Id, take 3 (BMW, Audi, Tesla). Outer: order by
    // Price DESC (Tesla=300, Audi=200, BMW=100).
    with_sqlite(":memory:") $(db) {
        fixture_db_11c(db)
        let arr <- fixture_array_11c()

        let m1 <- _sql(db |> select_from(type<Car_11c>) |> _order_by(_.Id) |> take(3)
            |> _order_by_descending(_.Price))
        let m2 <- (db |> select_from(type<Car_11c>) |> _order_by(_.Id) |> take(3)
            |> _order_by_descending(_.Price))
        let m3 <- (arr |> _order_by(_.Id) |> take(3) |> _order_by_descending(_.Price))

        t |> success(cars_equal_11c(m1, m2), "order|take|order_desc: M1 ≡ M2")
        t |> success(cars_equal_11c(m2, m3), "order|take|order_desc: M2 ≡ M3")
        t |> equal(length(m1), 3, "result has 3 rows")
        t |> equal(m1[0].Name, "Tesla", "highest price first")
    }
}

[test(description = "take, order_by and take nested three levels deep give identical rows in all three modes")]
def parity_three_level_nesting(t : T?) {
    // Inner Q: order by Id, take 4 (BMW, Audi, Tesla, Lada). Mid Q: order
    // by Price ASC (Lada=50, BMW=100, Audi=200, Tesla=300). Outer Q: take
    // 2 (Lada, BMW).
    //
    // SQL:
    //   SELECT … FROM (
    //       SELECT … FROM (SELECT … ORDER BY "Id" ASC LIMIT 4) AS "t0"
    //       ORDER BY "Price" ASC
    //   ) AS "t0" LIMIT 2
    with_sqlite(":memory:") $(db) {
        fixture_db_11c(db)
        let arr <- fixture_array_11c()

        let m1 <- _sql(db |> select_from(type<Car_11c>) |> _order_by(_.Id) |> take(4)
            |> _order_by(_.Price) |> take(2))
        let m2 <- (db |> select_from(type<Car_11c>) |> _order_by(_.Id) |> take(4)
            |> _order_by(_.Price) |> take(2))
        let m3 <- (arr |> _order_by(_.Id) |> take(4) |> _order_by(_.Price) |> take(2))

        t |> success(cars_equal_11c(m1, m2), "three-level: M1 ≡ M2")
        t |> success(cars_equal_11c(m2, m3), "three-level: M2 ≡ M3")
        t |> equal(length(m1), 2, "result has 2 rows")
        t |> equal(m1[0].Name, "Lada", "lowest of inner-4 first")
        t |> equal(m1[1].Name, "BMW", "second-lowest second")
    }
}

// Multi-Q lowering parity check: outer-Q `_where` over inner-Q
// `skip`.
//
//   `... |> _order_by(_.Price) |> skip(2) |> _where(_.Price > 100)`
//
// Lowers under `_sql` to:
// SELECT … FROM (SELECT … ORDER BY "Price" ASC LIMIT -1 OFFSET 2) AS
// "t0" WHERE "Price" > ?
//
// Linq positional semantics: order by price, skip first 2, then
// filter the rest.

[sql_table(name = "Cars")]
struct Car_11d {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db_11d(db : SqlRunner) {
    db |> create_table(type<Car_11d>)
    db |> insert(Car_11d(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car_11d(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car_11d(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car_11d(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car_11d(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array_11d() : array<Car_11d> {
    return <- [
        Car_11d(Id = 1, Name = "BMW",   Price = 100),
        Car_11d(Id = 2, Name = "Audi",  Price = 200),
        Car_11d(Id = 3, Name = "Tesla", Price = 300),
        Car_11d(Id = 4, Name = "Lada",  Price = 50),
        Car_11d(Id = 5, Name = "Lambo", Price = 999)
    ]
}

def cars_equal_11d(a : array<Car_11d>; b : array<Car_11d>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Price != eb.Price) {
            return false
        }
    }
    return true
}

[test(description = "a where after a skip filters the remaining rows identically in all three modes")]
def parity_skip_then_where(t : T?) {
    // Order by price (Lada=50, BMW=100, Audi=200, Tesla=300, Lambo=999),
    // skip 2 (drops Lada, BMW), then filter Price > 100 (keeps all 3
    // remaining since Audi=200, Tesla=300, Lambo=999 all > 100).
    with_sqlite(":memory:") $(db) {
        fixture_db_11d(db)
        let arr <- fixture_array_11d()

        let m1 <- _sql(db |> select_from(type<Car_11d>) |> _order_by(_.Price)
            |> skip(2) |> _where(_.Price > 100))
        let m2 <- (db |> select_from(type<Car_11d>) |> _order_by(_.Price)
            |> skip(2) |> _where(_.Price > 100))
        let m3 <- (arr |> _order_by(_.Price) |> skip(2) |> _where(_.Price > 100))

        t |> success(cars_equal_11d(m1, m2), "order|skip|where: M1 ≡ M2")
        t |> success(cars_equal_11d(m2, m3), "order|skip|where: M2 ≡ M3")
        t |> equal(length(m1), 3, "3 rows after skip+filter")
    }
}

[test(description = "skip, where and a descending order_by give identical rows in all three modes")]
def parity_skip_then_where_then_order_desc(t : T?) {
    // Inner: order ASC + skip(1) → drops Lada. Outer: where(Price>50)
    // keeps the rest, then order_descending(Price). All in outer Q after
    // the skip's divert.
    with_sqlite(":memory:") $(db) {
        fixture_db_11d(db)
        let arr <- fixture_array_11d()

        let m1 <- _sql(db |> select_from(type<Car_11d>) |> _order_by(_.Price) |> skip(1)
                         |> _where(_.Price > 50)
                         |> _order_by_descending(_.Price))
        let m2 <- (db |> select_from(type<Car_11d>) |> _order_by(_.Price) |> skip(1)
                     |> _where(_.Price > 50)
                     |> _order_by_descending(_.Price))
        let m3 <- (arr |> _order_by(_.Price) |> skip(1)
                       |> _where(_.Price > 50)
                       |> _order_by_descending(_.Price))

        t |> success(cars_equal_11d(m1, m2), "order|skip|where|order_desc: M1 ≡ M2")
        t |> success(cars_equal_11d(m2, m3), "order|skip|where|order_desc: M2 ≡ M3")
        t |> equal(m1[0].Name, "Lambo", "highest price first")
    }
}

// Parity check for tutorial 12 — `distinct()` row dedupe.
//
// Shapes:
// - `select_from |> distinct()`                    full-row DISTINCT
// - `_select(_.Name) |> distinct()`                single-column
// DISTINCT - `_where |> _select |> distinct()`              composed
//
// Result-order note: SQL DISTINCT does not guarantee any order, while
// linq's `distinct` preserves first-occurrence order. To keep parity
// assertions order-independent, all tests below compare *sorted*
// results — the multiset of distinct values must agree across modes.

[sql_table(name = "Cars")]
struct Car_12 {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db_12(db : SqlRunner) {
    db |> create_table(type<Car_12>)
    db |> insert(Car_12(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car_12(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car_12(Id = 3, Name = "BMW",   Price = 150))
    db |> insert(Car_12(Id = 4, Name = "Tesla", Price = 300))
    db |> insert(Car_12(Id = 5, Name = "Audi",  Price = 200))
}

def fixture_array_12() : array<Car_12> {
    return <- [
        Car_12(Id = 1, Name = "BMW",   Price = 100),
        Car_12(Id = 2, Name = "Audi",  Price = 200),
        Car_12(Id = 3, Name = "BMW",   Price = 150),
        Car_12(Id = 4, Name = "Tesla", Price = 300),
        Car_12(Id = 5, Name = "Audi",  Price = 200)
    ]
}

def sorted_strings(var a : array<string>) : array<string> {
    a |> sort()
    return <- a
}

def sorted_int_pairs(cars : array<Car_12>) : array<tuple<int; int>> {
    var pairs <- [for (c in cars); (c.Id, c.Price)]
    pairs |> sort() <| cmp_by(_._0, _._1)
    return <- pairs
}

def string_arrays_eq(a : array<string>; b : array<string>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea != eb) {
            return false
        }
    }
    return true
}

def pair_arrays_eq(a : array<tuple<int; int>>; b : array<tuple<int; int>>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea._0 != eb._0 || ea._1 != eb._1) {
            return false
        }
    }
    return true
}

[test(description = "full-row distinct gives the same row set in all three modes")]
def parity_full_row_distinct(t : T?) {
    // All PKs are unique, so DISTINCT survives every row. The point of
    // the test: full-row DISTINCT must produce the same multiset across
    // all three modes. Compare via sorted (Id, Price) pairs to ignore
    // result ordering.
    with_sqlite(":memory:") $(db) {
        fixture_db_12(db)
        var arr <- fixture_array_12()

        let m1 <- _sql(db |> select_from(type<Car_12>) |> distinct())
        let m2 <- (db |> select_from(type<Car_12>) |> distinct())
        let m3 <- (arr |> distinct())

        let s1 <- sorted_int_pairs(m1)
        let s2 <- sorted_int_pairs(m2)
        let s3 <- sorted_int_pairs(m3)

        t |> success(pair_arrays_eq(s1, s2), "full-row distinct multiset: M1 ≡ M2")
        t |> success(pair_arrays_eq(s2, s3), "full-row distinct multiset: M2 ≡ M3")
    }
}

[test(description = "single-column distinct gives the same value set in all three modes")]
def parity_single_col_distinct(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_12(db)
        var arr <- fixture_array_12()

        var m1 <- _sql(db |> select_from(type<Car_12>) |> _select(_.Name) |> distinct())
        var m2 <- (db |> select_from(type<Car_12>) |> _select(_.Name) |> distinct())
        var m3 <- (arr |> _select(_.Name) |> distinct())

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 3, "Mode 1: 3 unique names")
        t |> equal(length(m2), 3, "Mode 2: 3 unique names")
        t |> equal(length(m3), 3, "Mode 3: 3 unique names")
        t |> success(string_arrays_eq(m1, m2), "single-col distinct multiset: M1 ≡ M2")
        t |> success(string_arrays_eq(m2, m3), "single-col distinct multiset: M2 ≡ M3")
    }
}

[test(description = "where, select and distinct give the same value set in all three modes")]
def parity_where_select_distinct(t : T?) {
    // Filter, project, dedupe. After Price > 100 we have rows 2,3,4,5
    // with names {Audi, BMW, Tesla, Audi} → distinct {Audi, BMW, Tesla}.
    with_sqlite(":memory:") $(db) {
        fixture_db_12(db)
        var arr <- fixture_array_12()

        var m1 <- _sql(db |> select_from(type<Car_12>) |> _where(_.Price > 100)
            |> _select(_.Name) |> distinct())
        var m2 <- (db |> select_from(type<Car_12>) |> _where(_.Price > 100)
            |> _select(_.Name) |> distinct())
        var m3 <- (arr |> _where(_.Price > 100) |> _select(_.Name) |> distinct())

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> success(string_arrays_eq(m1, m2), "where|select|distinct: M1 ≡ M2")
        t |> success(string_arrays_eq(m2, m3), "where|select|distinct: M2 ≡ M3")
    }
}

// Parity check for tutorial 12b — UNION / INTERSECT / EXCEPT.
//
// Both sides must project the same shape (`_select(_.Col)`). The
// `_sql` macro lowers `lhs |> union(rhs)` etc. to SQL set operations;
// `daslib/linq` already implements `union`/`intersect`/`except` over
// arrays. Set-op output order is unspecified — assertions compare
// sorted multisets.

[sql_table(name = "Customers")]
struct Customer {
    @sql_primary_key Id : int
    Tier : int
}

[sql_table(name = "Prospects")]
struct Prospect {
    @sql_primary_key Id : int
    Tier : int
}

def fixture_db_12b(db : SqlRunner) {
    db |> create_table(type<Customer>)
    db |> create_table(type<Prospect>)
    db |> insert(Customer(Id = 1, Tier = 10))
    db |> insert(Customer(Id = 2, Tier = 20))
    db |> insert(Customer(Id = 3, Tier = 30))
    db |> insert(Prospect(Id = 100, Tier = 20))
    db |> insert(Prospect(Id = 200, Tier = 30))
    db |> insert(Prospect(Id = 300, Tier = 40))
}

def fixture_customers() : array<Customer> {
    return <- [
        Customer(Id = 1, Tier = 10),
        Customer(Id = 2, Tier = 20),
        Customer(Id = 3, Tier = 30)
    ]
}

def fixture_prospects() : array<Prospect> {
    return <- [
        Prospect(Id = 100, Tier = 20),
        Prospect(Id = 200, Tier = 30),
        Prospect(Id = 300, Tier = 40)
    ]
}

def int_arrays_eq(a : array<int>; b : array<int>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea != eb) {
            return false
        }
    }
    return true
}

[test(description = "union gives the same value set in all three modes")]
def parity_union(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_12b(db)
        var customers <- fixture_customers()
        var prospects <- fixture_prospects()

        var m1 <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                       |> union(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m2 <- ((db |> select_from(type<Customer>) |> _select(_.Tier))
                   |> union(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m3 <- ((customers |> _select(_.Tier)) |> union(prospects |> _select(_.Tier)))

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 4, "UNION distinct count: 4 (10, 20, 30, 40)")
        t |> success(int_arrays_eq(m1, m2), "union: M1 ≡ M2 (sorted multiset)")
        t |> success(int_arrays_eq(m2, m3), "union: M2 ≡ M3 (sorted multiset)")
    }
}

[test(description = "intersect gives the same value set in all three modes")]
def parity_intersect(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_12b(db)
        var customers <- fixture_customers()
        var prospects <- fixture_prospects()

        var m1 <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                       |> intersect(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m2 <- ((db |> select_from(type<Customer>) |> _select(_.Tier))
                   |> intersect(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m3 <- ((customers |> _select(_.Tier)) |> intersect(prospects |> _select(_.Tier)))

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 2, "INTERSECT count: 2 (20, 30)")
        t |> success(int_arrays_eq(m1, m2), "intersect: M1 ≡ M2 (sorted multiset)")
        t |> success(int_arrays_eq(m2, m3), "intersect: M2 ≡ M3 (sorted multiset)")
    }
}

[test(description = "except gives the same value set in all three modes")]
def parity_except(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_12b(db)
        var customers <- fixture_customers()
        var prospects <- fixture_prospects()

        var m1 <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                       |> except(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m2 <- ((db |> select_from(type<Customer>) |> _select(_.Tier))
                   |> except(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m3 <- ((customers |> _select(_.Tier)) |> except(prospects |> _select(_.Tier)))

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 1, "EXCEPT count: 1 (just 10)")
        t |> success(int_arrays_eq(m1, m2), "except: M1 ≡ M2 (sorted multiset)")
        t |> success(int_arrays_eq(m2, m3), "except: M2 ≡ M3 (sorted multiset)")
    }
}

// Parity check for tutorial 13 — column aggregates: count, sum,
// average, min, max. Each is a chain terminal returning a scalar.
//
// Result-type expectation per the tutorial header:
//   - sum / min / max: column's daslang type (int for Price)
//   - average:         double (always promoted)
//   - count:           int (matches `length(arr)`)

[sql_table(name = "Cars")]
struct Car_13 {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db_13(db : SqlRunner) {
    db |> create_table(type<Car_13>)
    db |> insert(Car_13(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car_13(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car_13(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car_13(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car_13(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array_13() : array<Car_13> {
    return <- [
        Car_13(Id = 1, Name = "BMW",   Price = 100),
        Car_13(Id = 2, Name = "Audi",  Price = 200),
        Car_13(Id = 3, Name = "Tesla", Price = 300),
        Car_13(Id = 4, Name = "Lada",  Price = 50),
        Car_13(Id = 5, Name = "Lambo", Price = 999)
    ]
}

[test(description = "count gives the same number in all three modes")]
def parity_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_13(db)
        let arr <- fixture_array_13()

        let m1 = _sql(db |> select_from(type<Car_13>) |> count())
        let m2 = db |> select_from(type<Car_13>) |> count()
        let m3 = arr |> count()

        t |> equal(m1, m2, "count: M1 ≡ M2")
        t |> equal(m2, m3, "count: M2 ≡ M3")
        t |> equal(m1, 5,  "count: 5 rows")
    }
}

[test(description = "sum gives the same total in all three modes")]
def parity_sum(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_13(db)
        var arr <- fixture_array_13()

        let m1 = _sql(db |> select_from(type<Car_13>) |> _select(_.Price) |> sum())
        let m2 = db |> select_from(type<Car_13>) |> _select(_.Price) |> sum()
        let m3 = arr |> _select(_.Price) |> sum()

        t |> equal(m1, m2, "sum: M1 ≡ M2")
        t |> equal(m2, m3, "sum: M2 ≡ M3")
        t |> equal(m1, 100 + 200 + 300 + 50 + 999, "sum: 1649")
    }
}

[test(description = "average gives the same double in all three modes")]
def parity_average(t : T?) {
    // F2 resolved: linq `average` always returns `double` (matching SQL
    // `AVG` and C# LINQ `Average`). All three modes agree on type and
    // value.
    with_sqlite(":memory:") $(db) {
        fixture_db_13(db)
        var arr <- fixture_array_13()

        let m1 = _sql(db |> select_from(type<Car_13>) |> _select(_.Price) |> average())
        let m2 = db |> select_from(type<Car_13>) |> _select(_.Price) |> average()
        let m3 = arr |> _select(_.Price) |> average()

        t |> equal(m1, m2, "average: M1 ≡ M2")
        t |> equal(m2, m3, "average: M2 ≡ M3")
        t |> equal(m1, 329.8lf, "average: 1649 / 5 = 329.8")
    }
}

[test(description = "min gives the same value in all three modes")]
def parity_min(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_13(db)
        var arr <- fixture_array_13()

        let m1 = _sql(db |> select_from(type<Car_13>) |> _select(_.Price) |> min())
        let m2 = db |> select_from(type<Car_13>) |> _select(_.Price) |> min()
        let m3 = arr |> _select(_.Price) |> min()

        t |> equal(m1, m2, "min: M1 ≡ M2")
        t |> equal(m2, m3, "min: M2 ≡ M3")
        t |> equal(m1, 50, "min: 50")
    }
}

[test(description = "max gives the same value in all three modes")]
def parity_max(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_13(db)
        var arr <- fixture_array_13()

        let m1 = _sql(db |> select_from(type<Car_13>) |> _select(_.Price) |> max())
        let m2 = db |> select_from(type<Car_13>) |> _select(_.Price) |> max()
        let m3 = arr |> _select(_.Price) |> max()

        t |> equal(m1, m2, "max: M1 ≡ M2")
        t |> equal(m2, m3, "max: M2 ≡ M3")
        t |> equal(m1, 999, "max: 999")
    }
}

[test(description = "sum after a where gives the same total in all three modes")]
def parity_where_sum(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_13(db)
        var arr <- fixture_array_13()
        let cutoff = 100

        let m1 = _sql(db |> select_from(type<Car_13>)
            |> _where(_.Price > cutoff) |> _select(_.Price) |> sum())
        let m2 = (db |> select_from(type<Car_13>)
            |> _where(_.Price > cutoff) |> _select(_.Price) |> sum())
        let m3 = arr |> _where(_.Price > cutoff) |> _select(_.Price) |> sum()

        t |> equal(m1, m2, "where|sum: M1 ≡ M2")
        t |> equal(m2, m3, "where|sum: M2 ≡ M3")
        t |> equal(m1, 200 + 300 + 999, "where|sum: 1499")
    }
}

// Parity check for tutorial 14 — `_group_by` and `_having`.
[sql_table(name = "Users")]
struct User_14 {
    @sql_primary_key Id : int
    Name   : string
    City   : string
    Age    : int
    Salary : int
}

def fixture_db_14(db : SqlRunner) {
    db |> create_table(type<User_14>)
    db |> insert(User_14(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100))
    db |> insert(User_14(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80))
    db |> insert(User_14(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120))
    db |> insert(User_14(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90))
    db |> insert(User_14(Id = 5, Name = "Eve",   City = "Chicago", Age = 50, Salary = 200))
    db |> insert(User_14(Id = 6, Name = "Frank", City = "Boston",  Age = 28, Salary = 70))
    db |> insert(User_14(Id = 7, Name = "Adam",  City = "NYC",     Age = 30, Salary = 110))
}

def fixture_array_14() : array<User_14> {
    return <- [
        User_14(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100),
        User_14(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80),
        User_14(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120),
        User_14(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90),
        User_14(Id = 5, Name = "Eve",   City = "Chicago", Age = 50, Salary = 200),
        User_14(Id = 6, Name = "Frank", City = "Boston",  Age = 28, Salary = 70),
        User_14(Id = 7, Name = "Adam",  City = "NYC",     Age = 30, Salary = 110)
    ]
}

def sort_by_city_age(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.City, _.Age)
}

[test(description = "group_by with a count gives identical groups in all three modes")]
def parity_group_by_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_14(db)
        let arr <- fixture_array_14()

        let m1 <- _sql(db |> select_from(type<User_14>)
                         |> _group_by(_.City)
                         |> _order_by(_._0)
                         |> _select((City = _._0, N = _._1 |> length)))
        let m2 <- (db |> select_from(type<User_14>)
                     |> _group_by(_.City)
                     |> _order_by(_._0)
                     |> _select((City = _._0, N = _._1 |> length)))
        let m3 <- (arr |> _group_by(_.City)
                       |> _order_by(_._0)
                       |> _select((City = _._0, N = _._1 |> length)))

        t |> equal(length(m1), length(m2), "group+count: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "group+count: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test(description = "group_by with several aggregates gives identical groups in all three modes")]
def parity_group_by_multi_aggregate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_14(db)
        let arr <- fixture_array_14()

        let m1 <- _sql(db |> select_from(type<User_14>)
                         |> _group_by(_.City)
                         |> _order_by(_._0)
                         |> _select((
                             City   = _._0,
                             N      = _._1 |> length,
                             MinAge = _._1 |> select($(u : User_14) => u.Age)    |> min,
                             MaxAge = _._1 |> select($(u : User_14) => u.Age)    |> max,
                             Total  = _._1 |> select($(u : User_14) => u.Salary) |> sum)))
        let m2 <- (db |> select_from(type<User_14>)
                     |> _group_by(_.City)
                     |> _order_by(_._0)
                     |> _select((
                         City   = _._0,
                         N      = _._1 |> length,
                         MinAge = _._1 |> select($(u : User_14) => u.Age)    |> min,
                         MaxAge = _._1 |> select($(u : User_14) => u.Age)    |> max,
                         Total  = _._1 |> select($(u : User_14) => u.Salary) |> sum)))
        let m3 <- (arr |> _group_by(_.City)
                       |> _order_by(_._0)
                       |> _select((
                           City   = _._0,
                           N      = _._1 |> length,
                           MinAge = _._1 |> select($(u : User_14) => u.Age)    |> min,
                           MaxAge = _._1 |> select($(u : User_14) => u.Age)    |> max,
                           Total  = _._1 |> select($(u : User_14) => u.Salary) |> sum)))

        t |> equal(length(m1), length(m2), "multi-agg length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "multi-agg length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City,   m2[i].City,   "row[{i}].City:   M1 ≡ M2")
            t |> equal(m1[i].N,      m2[i].N,      "row[{i}].N:      M1 ≡ M2")
            t |> equal(m1[i].MinAge, m2[i].MinAge, "row[{i}].MinAge: M1 ≡ M2")
            t |> equal(m1[i].MaxAge, m2[i].MaxAge, "row[{i}].MaxAge: M1 ≡ M2")
            t |> equal(m1[i].Total,  m2[i].Total,  "row[{i}].Total:  M1 ≡ M2")
            t |> equal(m2[i].City,   m3[i].City,   "row[{i}].City:   M2 ≡ M3")
            t |> equal(m2[i].N,      m3[i].N,      "row[{i}].N:      M2 ≡ M3")
            t |> equal(m2[i].MinAge, m3[i].MinAge, "row[{i}].MinAge: M2 ≡ M3")
            t |> equal(m2[i].MaxAge, m3[i].MaxAge, "row[{i}].MaxAge: M2 ≡ M3")
            t |> equal(m2[i].Total,  m3[i].Total,  "row[{i}].Total:  M2 ≡ M3")
        }
    }
}

[test(description = "where then group_by with a count gives identical groups in all three modes")]
def parity_where_group_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_14(db)
        let arr <- fixture_array_14()

        let m1 <- _sql(db |> select_from(type<User_14>)
                         |> _where(_.Age >= 30)
                         |> _group_by(_.City)
                         |> _order_by(_._0)
                         |> _select((City = _._0, N = _._1 |> length)))
        let m2 <- (db |> select_from(type<User_14>)
                     |> _where(_.Age >= 30)
                     |> _group_by(_.City)
                     |> _order_by(_._0)
                     |> _select((City = _._0, N = _._1 |> length)))
        let m3 <- (arr |> _where(_.Age >= 30)
                       |> _group_by(_.City)
                       |> _order_by(_._0)
                       |> _select((City = _._0, N = _._1 |> length)))

        t |> equal(length(m1), length(m2), "where|group|count: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "where|group|count: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test(description = "group_by with having gives identical groups in all three modes")]
def parity_group_having(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_14(db)
        let arr <- fixture_array_14()

        let m1 <- _sql(db |> select_from(type<User_14>)
                         |> _group_by(_.City)
                         |> _having(_._1 |> length > 1)
                         |> _order_by(_._0)
                         |> _select((City = _._0, N = _._1 |> length)))
        let m2 <- (db |> select_from(type<User_14>)
                     |> _group_by(_.City)
                     |> _having(_._1 |> length > 1)
                     |> _order_by(_._0)
                     |> _select((City = _._0, N = _._1 |> length)))
        let m3 <- (arr |> _group_by(_.City)
                       |> _having(_._1 |> length > 1)
                       |> _order_by(_._0)
                       |> _select((City = _._0, N = _._1 |> length)))

        t |> equal(length(m1), length(m2), "group|having: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "group|having: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test(description = "group_by on a tuple key gives identical groups in all three modes")]
def parity_multi_key_group(t : T?) {
    // No inline `_order_by` here: the `_sql` analyzer's tuple-key
    // validator accepts only `_.Field` / string expressions, not nested
    // walks like `_._0._N`, so multi-component sorts after `_group_by`
    // can't be expressed as a single `_order_by` clause across all
    // three modes. Post-sort by (City, Age) instead.
    with_sqlite(":memory:") $(db) {
        fixture_db_14(db)
        let arr <- fixture_array_14()

        var m1 <- _sql(db |> select_from(type<User_14>)
                         |> _group_by((_.City, _.Age))
                         |> _having(_._1 |> length > 1)
                         |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))
        var m2 <- (db |> select_from(type<User_14>)
                     |> _group_by((_.City, _.Age))
                     |> _having(_._1 |> length > 1)
                     |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))
        var m3 <- (arr |> _group_by((_.City, _.Age))
                       |> _having(_._1 |> length > 1)
                       |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))

        sort_by_city_age(m1)
        sort_by_city_age(m2)
        sort_by_city_age(m3)

        t |> equal(length(m1), length(m2), "multi-key length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "multi-key length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].Age,  m2[i].Age,  "row[{i}].Age:  M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].Age,  m3[i].Age,  "row[{i}].Age:  M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test(description = "a full where, group_by, having, order_by and take chain gives identical groups in all three modes")]
def parity_full_reporting_chain(t : T?) {
    // Full reporting: WHERE → GROUP BY → HAVING → ORDER BY → SELECT →
    // take.
    with_sqlite(":memory:") $(db) {
        fixture_db_14(db)
        let arr <- fixture_array_14()
        let min_age = 25
        let min_count = 1

        let m1 <- _sql(db |> select_from(type<User_14>)
                         |> _where(_.Age >= min_age)
                         |> _group_by(_.City)
                         |> _having(_._1 |> length >= min_count)
                         |> _order_by(_._0)
                         |> _select((
                             City      = _._0,
                             N         = _._1 |> length,
                             TotSalary = _._1 |> select($(u : User_14) => u.Salary) |> sum,
                             OldestAge = _._1 |> select($(u : User_14) => u.Age)    |> max))
                         |> take(10))
        let m2 <- (db |> select_from(type<User_14>)
                     |> _where(_.Age >= min_age)
                     |> _group_by(_.City)
                     |> _having(_._1 |> length >= min_count)
                     |> _order_by(_._0)
                     |> _select((
                         City      = _._0,
                         N         = _._1 |> length,
                         TotSalary = _._1 |> select($(u : User_14) => u.Salary) |> sum,
                         OldestAge = _._1 |> select($(u : User_14) => u.Age)    |> max))
                     |> take(10))
        let m3 <- (arr |> _where(_.Age >= min_age)
                       |> _group_by(_.City)
                       |> _having(_._1 |> length >= min_count)
                       |> _order_by(_._0)
                       |> _select((
                           City      = _._0,
                           N         = _._1 |> length,
                           TotSalary = _._1 |> select($(u : User_14) => u.Salary) |> sum,
                           OldestAge = _._1 |> select($(u : User_14) => u.Age)    |> max))
                       |> take(10))

        t |> equal(length(m1), length(m2), "full chain length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "full chain length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City,      m2[i].City,      "row[{i}].City:      M1 ≡ M2")
            t |> equal(m1[i].N,         m2[i].N,         "row[{i}].N:         M1 ≡ M2")
            t |> equal(m1[i].TotSalary, m2[i].TotSalary, "row[{i}].TotSalary: M1 ≡ M2")
            t |> equal(m1[i].OldestAge, m2[i].OldestAge, "row[{i}].OldestAge: M1 ≡ M2")
            t |> equal(m2[i].City,      m3[i].City,      "row[{i}].City:      M2 ≡ M3")
            t |> equal(m2[i].N,         m3[i].N,         "row[{i}].N:         M2 ≡ M3")
            t |> equal(m2[i].TotSalary, m3[i].TotSalary, "row[{i}].TotSalary: M2 ≡ M3")
            t |> equal(m2[i].OldestAge, m3[i].OldestAge, "row[{i}].OldestAge: M2 ≡ M3")
        }
    }
}

// Coverage for `_having |> _order_by` after `_group_by` across all
// three modes — was F3 in API_CHECKED.md (resolved by PR #2563,
// 2026-05-03 synthesized tuple/variant clone-constness fix).

options gen2

require dastest/testing_boost public

require daslib/sql

require daslib/linq

require daslib/linq_boost

require sqlite/sqlite_boost

require daslib/sql_linq

[sql_table(name = "Users")]
struct User_14_14 {
    @sql_primary_key Id : int
    Name   : string
    City   : string
    Age    : int
    Salary : int
}

def fixture_db_14_14(db : SqlRunner) {
    db |> create_table(type<User_14_14>)
    db |> insert(User_14_14(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100))
    db |> insert(User_14_14(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80))
    db |> insert(User_14_14(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120))
    db |> insert(User_14_14(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90))
    db |> insert(User_14_14(Id = 5, Name = "Eve",   City = "Boston",  Age = 50, Salary = 200))
}

def fixture_array_14_14() : array<User_14_14> {
    return <- [
        User_14_14(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100),
        User_14_14(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80),
        User_14_14(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120),
        User_14_14(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90),
        User_14_14(Id = 5, Name = "Eve",   City = "Boston",  Age = 50, Salary = 200)
    ]
}

[test(description = "order_by after group_by and having gives identical groups in all three modes")]
def parity_having_then_order_by(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db_14_14(db)
        let arr <- fixture_array_14_14()

        let m1 <- _sql(db |> select_from(type<User_14_14>)
                         |> _group_by(_.City)
                         |> _having(_._1 |> length > 1)
                         |> _order_by(_._0)
                         |> _select((City = _._0, N = _._1 |> length)))
        let m2 <- (db |> select_from(type<User_14_14>)
                     |> _group_by(_.City)
                     |> _having(_._1 |> length > 1)
                     |> _order_by(_._0)
                     |> _select((City = _._0, N = _._1 |> length)))
        let m3 <- (arr |> _group_by(_.City)
                       |> _having(_._1 |> length > 1)
                       |> _order_by(_._0)
                       |> _select((City = _._0, N = _._1 |> length)))

        // Fixture has NYC=2, Chicago=2, Boston=1; HAVING > 1 drops Boston.
        t |> equal(length(m1), 2,           "having|order_by: HAVING > 1 keeps 2 groups")
        t |> equal(length(m1), length(m2), "having|order_by: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "having|order_by: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

// Parity check for v2 multi-Q lowering — aggregate-then-filter:
//   `_group_by |> _select |> _where(_.alias)` and friends.
//
// In `_sql` mode, `_select` (eval phase 5) appearing inside `_where`
// / `take` / etc. (lower phases) triggers a multi-Q lowering: the
// `_select` (with its `_group_by`) becomes a nested SELECT exposed
// via `AS "alias"` columns; the outer chain references those aliases
// through `_.alias`.
//
// Plain linq runs the same chain positionally — `_select` projects to
// a named tuple, then `_where` filters that tuple by `_.alias`. M1 /
// M2 / M3 must agree.
//
// Tuple structural-uniqueness note: parity_check_14_group_by.das
// already installs `tuple<string;int>` with names (City, N) and other
// shapes; case 2 below uses `tuple<string;double>` (City, AvgSalary)
// to avoid arg-type-list collisions. See
// feedback_daslang_tuple_recordnames_collide.md.

[sql_table(name = "Users")]
struct User_14b {
    @sql_primary_key Id : int
    Name   : string
    City   : string
    Age    : int
    Salary : int
}

def fixture_db_14b(db : SqlRunner) {
    db |> create_table(type<User_14b>)
    db |> insert(User_14b(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100))
    db |> insert(User_14b(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80))
    db |> insert(User_14b(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120))
    db |> insert(User_14b(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90))
    db |> insert(User_14b(Id = 5, Name = "Eve",   City = "Chicago", Age = 50, Salary = 200))
    db |> insert(User_14b(Id = 6, Name = "Frank", City = "Boston",  Age = 28, Salary = 70))
    db |> insert(User_14b(Id = 7, Name = "Adam",  City = "NYC",     Age = 30, Salary = 110))
}

def fixture_array_14b() : array<User_14b> {
    return <- [
        User_14b(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100),
        User_14b(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80),
        User_14b(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120),
        User_14b(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90),
        User_14b(Id = 5, Name = "Eve",   City = "Chicago", Age = 50, Salary = 200),
        User_14b(Id = 6, Name = "Frank", City = "Boston",  Age = 28, Salary = 70),
        User_14b(Id = 7, Name = "Adam",  City = "NYC",     Age = 30, Salary = 110)
    ]
}

def sort_by_city(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.City)
}

[test(description = "a where on the projected group aggregate gives identical rows in all three modes")]
def parity_group_select_where(t : T?) {
    // Case 1: `_group_by |> _select((City=_._0, N=_._1 |> length)) |>
    // _where(_.N > 1)`. Outer WHERE references the aggregate alias `N`.
    // Expect cities with >1 user (NYC=3, Chicago=3).
    with_sqlite(":memory:") $(db) {
        fixture_db_14b(db)
        var arr <- fixture_array_14b()

        var m1 <- _sql(db |> select_from(type<User_14b>)
                         |> _group_by(_.City)
                         |> _select((City = _._0, N = _._1 |> length))
                         |> _where(_.N > 1))
        var m2 <- (db |> select_from(type<User_14b>)
                     |> _group_by(_.City)
                     |> _select((City = _._0, N = _._1 |> length))
                     |> _where(_.N > 1))
        var m3 <- (arr |> _group_by(_.City)
                       |> _select((City = _._0, N = _._1 |> length))
                       |> _where(_.N > 1))

        sort_by_city(m1)
        sort_by_city(m2)
        sort_by_city(m3)

        t |> equal(length(m1), length(m2), "select|where: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "select|where: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test(description = "a where on a projected group average gives identical rows in all three modes")]
def parity_group_select_avg_where(t : T?) {
    // Case 2: aggregate sub-select + outer WHERE on the float aggregate
    // alias. Tuple shape `tuple<string;double>` with (City, AvgSalary) —
    // structurally distinct from tuples installed by
    // parity_check_14_group_by to avoid recordNames collision.
    with_sqlite(":memory:") $(db) {
        fixture_db_14b(db)
        var arr <- fixture_array_14b()

        var m1 <- _sql(db |> select_from(type<User_14b>)
                         |> _group_by(_.City)
                         |> _select((City = _._0,
                                     AvgSalary = _._1 |> select($(u : User_14b) =>
                                         u.Salary) |> average))
                         |> _where(_.AvgSalary > 95.0lf))
        var m2 <- (db |> select_from(type<User_14b>)
                     |> _group_by(_.City)
                     |> _select((City = _._0,
                                 AvgSalary = _._1 |> select($(u : User_14b) =>
                                     u.Salary) |> average))
                     |> _where(_.AvgSalary > 95.0lf))
        var m3 <- (arr |> _group_by(_.City)
                       |> _select((City = _._0,
                                   AvgSalary = _._1 |> select($(u : User_14b) =>
                                       u.Salary) |> average))
                       |> _where(_.AvgSalary > 95.0lf))

        sort_by_city(m1)
        sort_by_city(m2)
        sort_by_city(m3)

        t |> equal(length(m1), length(m2), "select|where(avg): M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "select|where(avg): M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City,      m2[i].City,      "row[{i}].City:      M1 ≡ M2")
            t |> equal(m1[i].AvgSalary, m2[i].AvgSalary, "row[{i}].AvgSalary: M1 ≡ M2")
            t |> equal(m2[i].City,      m3[i].City,      "row[{i}].City:      M2 ≡ M3")
            t |> equal(m2[i].AvgSalary, m3[i].AvgSalary, "row[{i}].AvgSalary: M2 ≡ M3")
        }
    }
}

[test(description = "having, select and a where on the projection give identical rows in all three modes")]
def parity_group_having_select_where(t : T?) {
    // Case 3: HAVING (inner-Q filter on group) plus outer WHERE on the
    // projected alias. Inner Q has GROUP BY + HAVING. Outer Q is WHERE
    // over the projected (City, N).
    with_sqlite(":memory:") $(db) {
        fixture_db_14b(db)
        var arr <- fixture_array_14b()

        var m1 <- _sql(db |> select_from(type<User_14b>)
                         |> _group_by(_.City)
                         |> _having(_._1 |> length > 1)
                         |> _select((City = _._0, N = _._1 |> length))
                         |> _where(_.N > 2))
        var m2 <- (db |> select_from(type<User_14b>)
                     |> _group_by(_.City)
                     |> _having(_._1 |> length > 1)
                     |> _select((City = _._0, N = _._1 |> length))
                     |> _where(_.N > 2))
        var m3 <- (arr |> _group_by(_.City)
                       |> _having(_._1 |> length > 1)
                       |> _select((City = _._0, N = _._1 |> length))
                       |> _where(_.N > 2))

        sort_by_city(m1)
        sort_by_city(m2)
        sort_by_city(m3)

        t |> equal(length(m1), length(m2), "having|select|where: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "having|select|where: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test(description = "group_by, select, where, order_by and take give identical rows in all three modes")]
def parity_group_select_where_order_take(t : T?) {
    // Case 4: full chain — outer WHERE + ORDER + TAKE all reference inner
    // projection. Stresses bind ordering: inner GROUP BY binds (none
    // here) → outer WHERE binds (none) → outer LIMIT bind (3).
    with_sqlite(":memory:") $(db) {
        fixture_db_14b(db)
        var arr <- fixture_array_14b()

        let m1 <- _sql(db |> select_from(type<User_14b>)
                          |> _group_by(_.City)
                          |> _select((City = _._0, N = _._1 |> length))
                          |> _where(_.N > 1)
                          |> _order_by(_.N)
                          |> take(3))
        let m2 <- (db |> select_from(type<User_14b>)
                      |> _group_by(_.City)
                      |> _select((City = _._0, N = _._1 |> length))
                      |> _where(_.N > 1)
                      |> _order_by(_.N)
                      |> take(3))
        let m3 <- (arr |> _group_by(_.City)
                       |> _select((City = _._0, N = _._1 |> length))
                       |> _where(_.N > 1)
                       |> _order_by(_.N)
                       |> take(3))

        // Order is by N (ASC), then ties — keep order from query, do NOT
        // post-sort.
        t |> equal(length(m1), length(m2), "select|where|order|take: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "select|where|order|take: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].N, m2[i].N, "row[{i}].N: M1 ≡ M2 (order pin)")
            t |> equal(m2[i].N, m3[i].N, "row[{i}].N: M2 ≡ M3 (order pin)")
        }
    }
}

// Parity check for tutorial 15 — `_join`: inner equi-join.
//
// Shapes:
//   1. plain inner equi-join + named-tuple projection
//   2. pre-join `_where` (filters left source before join)
// 3. right-side `_where` inside the joined chain (filter via JOIN ON)
//
// Each test uses a structurally distinct projection tuple to avoid
// the generic-instantiation recordNames collision: any two
// `tuple<string;int>` literals flowing through the same linq generic
// in this compilation unit would share the cached instantiation and
// lose the second's recordNames. (See feedback memory.)

[sql_table(name = "Users")]
struct User_15 {
    @sql_primary_key Id : int
    Name   : string
    Active : bool
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

def fixture_db_15(db : SqlRunner) {
    db |> create_table(type<User_15>)
    db |> create_table(type<Order>)
    db |> insert(User_15(Id = 1, Name = "Alice", Active = true))
    db |> insert(User_15(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User_15(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total =  50))
}

def fixture_users() : array<User_15> {
    return <- [
        User_15(Id = 1, Name = "Alice", Active = true),
        User_15(Id = 2, Name = "Bob",   Active = true),
        User_15(Id = 3, Name = "Carol", Active = false)
    ]
}

def fixture_orders() : array<Order> {
    return <- [
        Order(Id = 10, UserId = 1, Total = 100),
        Order(Id = 11, UserId = 1, Total = 250),
        Order(Id = 12, UserId = 2, Total =  50)
    ]
}

def sort_by_pair(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.UserName, _.OrderTotal)
}

[test(description = "an inner join gives identical rows in all three modes")]
def parity_inner_join_basic(t : T?) {
    // Tuple shape: (UserName : string, OrderTotal : int) →
    // tuple<string;int>
    with_sqlite(":memory:") $(db) {
        fixture_db_15(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User_15>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User_15, o : Order) => u.Id == o.UserId,
                                  $(u : User_15, o : Order) =>
                                      (UserName = u.Name, OrderTotal = o.Total)))
        var m2 <- (db |> select_from(type<User_15>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User_15, o : Order) => u.Id == o.UserId,
                              $(u : User_15, o : Order) =>
                                  (UserName = u.Name, OrderTotal = o.Total)))
        var m3 <- (users |> _join(orders,
                                  $(u : User_15, o : Order) => u.Id == o.UserId,
                                  $(u : User_15, o : Order) =>
                                      (UserName = u.Name, OrderTotal = o.Total)))

        sort_by_pair(m1)
        sort_by_pair(m2)
        sort_by_pair(m3)

        t |> equal(length(m1), 3, "inner join row count: 3 (Carol absent)")
        t |> equal(length(m1), length(m2), "join: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "join: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
        }
    }
}

def sort_by_pair_with_active(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.UserName, _.OrderTotal)
}

[test(description = "a join with a filter on the left source gives identical rows in all three modes")]
def parity_left_filter_join(t : T?) {
    // Tuple shape: (UserName, OrderTotal, IsActive) →
    // tuple<string;int;bool> — structurally distinct from
    // parity_inner_join_basic.
    with_sqlite(":memory:") $(db) {
        fixture_db_15(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User_15>)
                         |> _where(_.Active)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User_15, o : Order) => u.Id == o.UserId,
                                  $(u : User_15, o : Order) =>
                                      (UserName = u.Name, OrderTotal = o.Total,
                                       IsActive = u.Active)))
        var m2 <- (db |> select_from(type<User_15>)
                     |> _where(_.Active)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User_15, o : Order) => u.Id == o.UserId,
                              $(u : User_15, o : Order) =>
                                  (UserName = u.Name, OrderTotal = o.Total, IsActive = u.Active)))
        var m3 <- (users |> _where(_.Active)
                         |> _join(orders,
                                  $(u : User_15, o : Order) => u.Id == o.UserId,
                                  $(u : User_15, o : Order) =>
                                      (UserName = u.Name, OrderTotal = o.Total,
                                       IsActive = u.Active)))

        sort_by_pair_with_active(m1)
        sort_by_pair_with_active(m2)
        sort_by_pair_with_active(m3)

        t |> equal(length(m1), length(m2), "where|join: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "where|join: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].IsActive,   m2[i].IsActive,   "row[{i}].IsActive:   M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].IsActive,   m3[i].IsActive,   "row[{i}].IsActive:   M2 ≡ M3")
        }
    }
}

def sort_by_pair_with_orderid(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.UserName, _.OrderId)
}

def sort_by_buyer_orderid(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.Buyer, _.OrderId)
}

[test(description = "a join followed by an outer where and first returns the same row in all three modes")]
def parity_join_with_outer_where_then_first(t : T?) {
    // v2.1 regression: `_join(...) |> _where(_.alias) |>
    // _order_by(_.alias) |> _first()` previously failed at compile time
    // with the misleading "_join LHS chain cannot end in a terminal"
    // error — the snapshot helper conflated the OUTER terminal flag with
    // an INNER terminal it was meant to reject. Hoisted check now lives
    // in process_join_call's srca recursion. Explicit `_order_by` on the
    // projected alias pins the chosen row deterministically across all
    // three modes (M1=_sql, M2=linq via select_from, M3=plain linq over
    // array). Tuple shape: (RefId:int, Label:string, Amount:int) →
    // tuple<int;string;int> — leading int makes the arg-type list
    // structurally distinct from every other test in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db_15(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let m1 = _sql(db |> select_from(type<User_15>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User_15, o : Order) => u.Id == o.UserId,
                                 $(u : User_15, o : Order) =>
                                     (RefId = o.Id, Label = u.Name, Amount = o.Total))
                        |> _where(_.Amount > 75)
                        |> _order_by(_.Amount)
                        |> _first())
        let m2 = (db |> select_from(type<User_15>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User_15, o : Order) => u.Id == o.UserId,
                             $(u : User_15, o : Order) =>
                                 (RefId = o.Id, Label = u.Name, Amount = o.Total))
                    |> _where(_.Amount > 75)
                    |> _order_by(_.Amount)
                    |> _first())
        let m3 = (users |> _join(orders,
                                 $(u : User_15, o : Order) => u.Id == o.UserId,
                                 $(u : User_15, o : Order) =>
                                     (RefId = o.Id, Label = u.Name, Amount = o.Total))
                        |> _where(_.Amount > 75)
                        |> _order_by(_.Amount)
                        |> _first())

        // Alice's orders 100 / 250 both pass `> 75`; ORDER BY Amount ASC
        // picks 100 (Order Id 10 = "Alice" / 100). Same row in all three
        // modes.
        t |> equal(m1.Amount, 100,    "M1 _first picks the ASC-min passing row")
        t |> equal(m1.Label,  "Alice", "M1 row is Alice's")
        t |> equal(m1.Amount, m2.Amount, "M1 ≡ M2 Amount")
        t |> equal(m1.Label,  m2.Label,  "M1 ≡ M2 Label")
        t |> equal(m1.RefId,  m2.RefId,  "M1 ≡ M2 RefId")
        t |> equal(m2.Amount, m3.Amount, "M2 ≡ M3 Amount")
        t |> equal(m2.Label,  m3.Label,  "M2 ≡ M3 Label")
        t |> equal(m2.RefId,  m3.RefId,  "M2 ≡ M3 RefId")
    }
}

[test(description = "an outer where with first after a join does not leak a limit into the inner source, in all three modes")]
def parity_join_outer_where_first_inner_limit_leak(t : T?) {
    // v2.1 review-2 regression for the inner-LIMIT leak: pre-fix,
    // snapshot's `build_sql_string(q, true)` ran with q.matr=One pre-set
    // by outer `_first` → inner subquery emitted `LIMIT 1` BEFORE the
    // outer WHERE filtered. With the fixture's natural JOIN order (Alice
    // 100, Alice 250, Bob 50), inner LIMIT 1 picked Alice 100; outer
    // WHERE OrderAmount == 50 filtered it → 0 rows → `_first()`
    // (Materializer.One) panicked on empty result.
    //
    // After the fix, snapshot save→clears outer-terminal state
    // pre-build_sql_string and restores it post-reset; inner emits no
    // LIMIT, outer is `WHERE OrderAmount == 50 LIMIT 1` → picks Bob 50
    // correctly.
    //
    // M2/M3 (linq over `select_from`'s materialized array / plain
    // in-memory array) don't go through the SQL builder at all — they
    // serve as oracles confirming the post-fix M1 picks the same row the
    // in-memory references do. The bug only ever existed in M1.
    //
    // Tuple shape (IsActive:bool, BuyerName:string, OrderAmount:int) →
    // tuple<bool;string;int> — structurally distinct from every other
    // test in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db_15(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let m1 = _sql(db |> select_from(type<User_15>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User_15, o : Order) => u.Id == o.UserId,
                                 $(u : User_15, o : Order) =>
                                     (IsActive = u.Active, BuyerName = u.Name,
                                      OrderAmount = o.Total))
                        |> _where(_.OrderAmount == 50)
                        |> _first())
        let m2 = (db |> select_from(type<User_15>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User_15, o : Order) => u.Id == o.UserId,
                             $(u : User_15, o : Order) =>
                                 (IsActive = u.Active, BuyerName = u.Name, OrderAmount = o.Total))
                    |> _where(_.OrderAmount == 50)
                    |> _first())
        let m3 = (users |> _join(orders,
                                 $(u : User_15, o : Order) => u.Id == o.UserId,
                                 $(u : User_15, o : Order) =>
                                     (IsActive = u.Active, BuyerName = u.Name,
                                      OrderAmount = o.Total))
                        |> _where(_.OrderAmount == 50)
                        |> _first())

        // Without the fix: m1 would be garbage (0 rows from outer WHERE →
        // _first panic or zeroed). With the fix: deterministically Bob's
        // order 12 (UserId 2, Total 50).
        t |> equal(m1.BuyerName,   "Bob", "WHERE OrderAmount==50 picks Bob (only matching row)")
        t |> equal(m1.OrderAmount, 50,    "Picked-row Total is 50")
        t |> equal(m1.IsActive,    true,  "Bob is active")
        // M2/M3 oracles: in-memory references confirm M1 picks the same row.
        t |> equal(m1.BuyerName,   m2.BuyerName,   "M1 ≡ M2 BuyerName")
        t |> equal(m1.OrderAmount, m2.OrderAmount, "M1 ≡ M2 OrderAmount")
        t |> equal(m1.IsActive,    m2.IsActive,    "M1 ≡ M2 IsActive")
        t |> equal(m2.BuyerName,   m3.BuyerName,   "M2 ≡ M3 BuyerName")
        t |> equal(m2.OrderAmount, m3.OrderAmount, "M2 ≡ M3 OrderAmount")
        t |> equal(m2.IsActive,    m3.IsActive,    "M2 ≡ M3 IsActive")
    }
}

[test(description = "a join followed by an outer where gives identical rows in all three modes")]
def parity_join_with_outer_where(t : T?) {
    // v2.1: outer WHERE on a single _join's projected alias must filter
    // PROJECTED rows. The WHERE peel snapshots the analyzed q in place;
    // SQL renders as `... FROM (joinSql) AS "t0" WHERE "alias" = ?` with
    // the alias resolved through q.fromRowType. Tuple shape:
    // (Buyer:string, Spend:int, OrderId:int, Win:bool) →
    // tuple<string;int;int;bool> — structurally distinct from the other
    // tests in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db_15(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User_15>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User_15, o : Order) => u.Id == o.UserId,
                                  $(u : User_15, o : Order) =>
                                      (Buyer = u.Name, Spend = o.Total, OrderId = o.Id,
                                       Win = u.Active))
                         |> _where(_.Spend > 75))
        var m2 <- (db |> select_from(type<User_15>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User_15, o : Order) => u.Id == o.UserId,
                              $(u : User_15, o : Order) =>
                                  (Buyer = u.Name, Spend = o.Total, OrderId = o.Id, Win = u.Active))
                     |> _where(_.Spend > 75))
        var m3 <- (users |> _join(orders,
                                  $(u : User_15, o : Order) => u.Id == o.UserId,
                                  $(u : User_15, o : Order) =>
                                      (Buyer = u.Name, Spend = o.Total, OrderId = o.Id,
                                       Win = u.Active))
                         |> _where(_.Spend > 75))

        sort_by_buyer_orderid(m1)
        sort_by_buyer_orderid(m2)
        sort_by_buyer_orderid(m3)

        // Alice: orders 100, 250 → both > 75. Bob: 50 → dropped. Expect 2
        // rows.
        t |> equal(length(m1), 2,
                   "join + outer WHERE Spend>75: 2 rows (Alice ×2, Bob's 50 dropped)")
        t |> equal(length(m1), length(m2), "M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Buyer,   m2[i].Buyer,   "row[{i}].Buyer:   M1 ≡ M2")
            t |> equal(m1[i].Spend,   m2[i].Spend,   "row[{i}].Spend:   M1 ≡ M2")
            t |> equal(m1[i].OrderId, m2[i].OrderId, "row[{i}].OrderId: M1 ≡ M2")
            t |> equal(m2[i].Buyer,   m3[i].Buyer,   "row[{i}].Buyer:   M2 ≡ M3")
            t |> equal(m2[i].Spend,   m3[i].Spend,   "row[{i}].Spend:   M2 ≡ M3")
            t |> equal(m2[i].OrderId, m3[i].OrderId, "row[{i}].OrderId: M2 ≡ M3")
        }
    }
}

[test(description = "a join with a filter on the right source gives identical rows in all three modes")]
def parity_right_filter_join(t : T?) {
    // Tuple shape: (UserName, OrderTotal, OrderId) →
    // tuple<string;int;int>. Right-side `_where` filters the joined
    // source before the equi-join.
    with_sqlite(":memory:") $(db) {
        fixture_db_15(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User_15>)
                         |> _join(db |> select_from(type<Order>) |> _where(_.Total > 75),
                                  $(u : User_15, o : Order) => u.Id == o.UserId,
                                  $(u : User_15, o : Order) =>
                                      (UserName = u.Name, OrderTotal = o.Total, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User_15>)
                     |> _join(db |> select_from(type<Order>) |> _where(_.Total > 75),
                              $(u : User_15, o : Order) => u.Id == o.UserId,
                              $(u : User_15, o : Order) =>
                                  (UserName = u.Name, OrderTotal = o.Total, OrderId = o.Id)))
        var m3 <- (users |> _join(orders |> _where(_.Total > 75),
                                  $(u : User_15, o : Order) => u.Id == o.UserId,
                                  $(u : User_15, o : Order) =>
                                      (UserName = u.Name, OrderTotal = o.Total, OrderId = o.Id)))

        sort_by_pair_with_orderid(m1)
        sort_by_pair_with_orderid(m2)
        sort_by_pair_with_orderid(m3)

        t |> equal(length(m1), 2, "right-filter join row count: 2 (Bob's 50 dropped)")
        t |> equal(length(m1), length(m2), "right-filter: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "right-filter: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].OrderId,    m2[i].OrderId,    "row[{i}].OrderId:    M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].OrderId,    m3[i].OrderId,    "row[{i}].OrderId:    M2 ≡ M3")
        }
    }
}

[test(description = "a join followed by count gives the same number in all three modes")]
def parity_join_then_count(t : T?) {
    // v2.1 review-3 regression for E1 (no-snapshot path): bare
    // `_join(...) |> count()` (no _where, no snapshot). Pre-fix:
    // process_join_call's analyze_projection unconditionally wrote
    // q.proj=NamedTuple and clobbered the aggregateExpr=COUNT(*) set by
    // the upstream count() peel; build emitted `SELECT t0."Name",
    // t1."Total" FROM Users JOIN Orders LIMIT 1` and returned a tuple
    // instead of an int. Post-fix: process_join_call saves/restores
    // aggregate state across analyze_projection.
    //
    // Inner `into` tuple: (Pay:int, Holder:string, Premium:bool) →
    // tuple<int;string;bool> — structurally distinct from every other
    // test in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db_15(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let n1 = _sql(db |> select_from(type<User_15>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User_15, o : Order) => u.Id == o.UserId,
                                 $(u : User_15, o : Order) =>
                                     (Pay = o.Total, Holder = u.Name, Premium = u.Active))
                        |> count())
        let n2 = (db |> select_from(type<User_15>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User_15, o : Order) => u.Id == o.UserId,
                             $(u : User_15, o : Order) =>
                                 (Pay = o.Total, Holder = u.Name, Premium = u.Active))
                    |> count())
        let n3 = (users |> _join(orders,
                                 $(u : User_15, o : Order) => u.Id == o.UserId,
                                 $(u : User_15, o : Order) =>
                                     (Pay = o.Total, Holder = u.Name, Premium = u.Active))
                        |> count())

        // 3 INNER-join rows (Alice ×2, Bob ×1; Carol absent).
        t |> equal(n1, 3, "_join | count(): 3 INNER-join rows")
        t |> equal(n1, n2, "M1 ≡ M2 count")
        t |> equal(n2, n3, "M2 ≡ M3 count")
    }
}

[test(description = "a join with an outer where and count gives the same number in all three modes")]
def parity_join_outer_where_then_count(t : T?) {
    // v2.1 review-3 regression for E2 (snapshot path): `_join(...) |>
    // _where(_.alias) |> count()` exercises the WHERE-peel snapshot.
    // Pre-fix: snapshot reset dropped aggregateExpr/aggregateType, OUTER
    // rendered projection columns instead of COUNT(*) and returned a
    // tuple. Post-fix: snapshot saves/clears/restores aggregate state and
    // post-restore overrides q.proj=Aggregate.
    //
    // Inner `into` tuple: (Live:bool, Pay:int, Tag:string) →
    // tuple<bool;int;string> — structurally distinct from every other
    // test in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db_15(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let n1 = _sql(db |> select_from(type<User_15>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User_15, o : Order) => u.Id == o.UserId,
                                 $(u : User_15, o : Order) =>
                                     (Live = u.Active, Pay = o.Total, Tag = u.Name))
                        |> _where(_.Pay > 75)
                        |> count())
        let n2 = (db |> select_from(type<User_15>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User_15, o : Order) => u.Id == o.UserId,
                             $(u : User_15, o : Order) =>
                                 (Live = u.Active, Pay = o.Total, Tag = u.Name))
                    |> _where(_.Pay > 75)
                    |> count())
        let n3 = (users |> _join(orders,
                                 $(u : User_15, o : Order) => u.Id == o.UserId,
                                 $(u : User_15, o : Order) =>
                                     (Live = u.Active, Pay = o.Total, Tag = u.Name))
                        |> _where(_.Pay > 75)
                        |> count())

        // Alice's 100 + 250 pass `Pay > 75`; Bob's 50 dropped → 2 rows.
        t |> equal(n1, 2, "_join | _where(Pay>75) | count(): 2 rows (Alice ×2)")
        t |> equal(n1, n2, "M1 ≡ M2 count")
        t |> equal(n2, n3, "M2 ≡ M3 count")
    }
}

// Parity check for v2 multi-Q lowering — multi-join (3+ tables in a
// single chain).
//
// Inner `_join(A, B, ...)` produces a NamedTuple. The outer
// `_join(<inner>, C, ...)` snapshots the inner Q to an
// `(<fromSubquerySql>) AS "t0"` subquery FROM and joins C against it.
// Outer ON-clause and `into` projection resolve `<lhsArg>.<alias>`
// against the inner's projected named tuple (via `q.fromRowType`).
//
// In plain linq, the inner `_join` returns `iterator/array<tuple>`,
// and the outer `_join` accepts that as `srca`. M1 / M2 / M3 must
// agree.

[sql_table(name = "Users")]
struct User_15b {
    @sql_primary_key Id : int
    Name   : string
    Active : bool
}

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    OrderId : int
    Sku     : string
    Qty     : int
}

// Outer-join lambdas need explicit types on BOTH args — daslang's
// lambda inference can't derive the LHS tuple type from join's
// signature when one arg is typed and the other auto. A typedef alias
// keeps the call sites readable; spelling the tuple inline also
// works.
typedef UserOrder = tuple<UserName : string; OrderId : int; OrderTotal : int>

def fixture_db_15b(db : SqlRunner) {
    db |> create_table(type<User_15b>)
    db |> create_table(type<Order>)
    db |> create_table(type<Item>)
    db |> insert(User_15b(Id = 1, Name = "Alice", Active = true))
    db |> insert(User_15b(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User_15b(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total =  50))
    db |> insert(Item(Id = 100, OrderId = 10, Sku = "BOOK",   Qty = 2))
    db |> insert(Item(Id = 101, OrderId = 11, Sku = "PEN",    Qty = 5))
    db |> insert(Item(Id = 102, OrderId = 11, Sku = "BOOK",   Qty = 1))
    db |> insert(Item(Id = 103, OrderId = 12, Sku = "PAPER",  Qty = 10))
}

def fixture_users_15b() : array<User_15b> {
    return <- [
        User_15b(Id = 1, Name = "Alice", Active = true),
        User_15b(Id = 2, Name = "Bob",   Active = true),
        User_15b(Id = 3, Name = "Carol", Active = false)
    ]
}

def fixture_items() : array<Item> {
    return <- [
        Item(Id = 100, OrderId = 10, Sku = "BOOK",  Qty = 2),
        Item(Id = 101, OrderId = 11, Sku = "PEN",   Qty = 5),
        Item(Id = 102, OrderId = 11, Sku = "BOOK",  Qty = 1),
        Item(Id = 103, OrderId = 12, Sku = "PAPER", Qty = 10)
    ]
}

def sort_by_user_sku(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.UserName, _.OrderTotal, _.Sku)
}

[test(description = "a three-table join gives identical rows in all three modes")]
def parity_three_table_join(t : T?) {
    // Case 1: User_15b INNER JOIN Order INNER JOIN Item.
    // Inner _join projects (UserName, OrderId, OrderTotal). Outer
    // references those aliases by name — `uo.OrderId`, `uo.UserName`,
    // `uo.OrderTotal`.
    with_sqlite(":memory:") $(db) {
        fixture_db_15b(db)
        var users  <- fixture_users_15b()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        var m1 <- _sql(db |> select_from(type<User_15b>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User_15b, o : Order) => u.Id == o.UserId,
                                  $(u : User_15b, o : Order) =>
                                      (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(db |> select_from(type<Item>),
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) =>
                                      (UserName = uo.UserName, OrderTotal = uo.OrderTotal,
                                       Sku = it.Sku, Qty = it.Qty)))
        var m2 <- (db |> select_from(type<User_15b>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User_15b, o : Order) => u.Id == o.UserId,
                              $(u : User_15b, o : Order) =>
                                  (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                     |> _join(db |> select_from(type<Item>),
                              $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                              $(uo : UserOrder, it : Item) =>
                                  (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku,
                                   Qty = it.Qty)))
        var m3 <- (users |> _join(orders,
                                  $(u : User_15b, o : Order) => u.Id == o.UserId,
                                  $(u : User_15b, o : Order) =>
                                      (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(items,
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) =>
                                      (UserName = uo.UserName, OrderTotal = uo.OrderTotal,
                                       Sku = it.Sku, Qty = it.Qty)))

        sort_by_user_sku(m1)
        sort_by_user_sku(m2)
        sort_by_user_sku(m3)

        t |> equal(length(m1), 4,
                   "3-table inner: 4 rows (Alice's 2 orders × items, Bob's 1 order × 1 item)")
        t |> equal(length(m1), length(m2), "3-table: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "3-table: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].Sku,        m2[i].Sku,        "row[{i}].Sku:        M1 ≡ M2")
            t |> equal(m1[i].Qty,        m2[i].Qty,        "row[{i}].Qty:        M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].Sku,        m3[i].Sku,        "row[{i}].Sku:        M2 ≡ M3")
            t |> equal(m2[i].Qty,        m3[i].Qty,        "row[{i}].Qty:        M2 ≡ M3")
        }
    }
}

[test(description = "a three-table join followed by first returns the same row in all three modes")]
def parity_three_table_join_then_first(t : T?) {
    // V2 latent regression: `_join(_join(A,B,...), C, ...) |> _first()`
    // previously failed at compile time with "_join LHS chain cannot end
    // in a terminal" — the snapshot helper mistook the OUTER `_first` for
    // an INNER terminal. Same fix as the v2.1 single-join+terminal case
    // (hoisted check in process_join_call). Explicit
    // `_order_by(_.OrderTotal)` pins the chosen row so all three modes
    // (M1=_sql, M2=linq via select_from, M3=plain linq over array) agree
    // on the _first() result. Reuses case 1's projection shape
    // (UserName/OrderTotal/Sku/Qty); the named tuple is already cached
    // from `parity_three_table_join`.
    with_sqlite(":memory:") $(db) {
        fixture_db_15b(db)
        var users  <- fixture_users_15b()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        let m1 = _sql(db |> select_from(type<User_15b>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User_15b, o : Order) => u.Id == o.UserId,
                                 $(u : User_15b, o : Order) =>
                                     (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(db |> select_from(type<Item>),
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) =>
                                     (UserName = uo.UserName, OrderTotal = uo.OrderTotal,
                                      Sku = it.Sku, Qty = it.Qty))
                        |> _order_by(_.OrderTotal)
                        |> _first())
        let m2 = (db |> select_from(type<User_15b>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User_15b, o : Order) => u.Id == o.UserId,
                             $(u : User_15b, o : Order) =>
                                 (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                    |> _join(db |> select_from(type<Item>),
                             $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                             $(uo : UserOrder, it : Item) =>
                                 (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku,
                                  Qty = it.Qty))
                    |> _order_by(_.OrderTotal)
                    |> _first())
        let m3 = (users |> _join(orders,
                                 $(u : User_15b, o : Order) => u.Id == o.UserId,
                                 $(u : User_15b, o : Order) =>
                                     (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(items,
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) =>
                                     (UserName = uo.UserName, OrderTotal = uo.OrderTotal,
                                      Sku = it.Sku, Qty = it.Qty))
                        |> _order_by(_.OrderTotal)
                        |> _first())

        // 4-row joined result; ORDER BY OrderTotal ASC: Bob's Order 12 (Total
        // 50, PAPER, Qty 10) wins.
        t |> equal(m1.UserName,   "Bob",   "M1 _first picks Bob's row (lowest OrderTotal)")
        t |> equal(m1.OrderTotal, 50,      "M1 OrderTotal is 50")
        t |> equal(m1.Sku,        "PAPER", "M1 Sku matches Bob's only item")
        t |> equal(m1.UserName,   m2.UserName,   "M1 ≡ M2 UserName")
        t |> equal(m1.OrderTotal, m2.OrderTotal, "M1 ≡ M2 OrderTotal")
        t |> equal(m1.Sku,        m2.Sku,        "M1 ≡ M2 Sku")
        t |> equal(m1.Qty,        m2.Qty,        "M1 ≡ M2 Qty")
        t |> equal(m2.UserName,   m3.UserName,   "M2 ≡ M3 UserName")
        t |> equal(m2.OrderTotal, m3.OrderTotal, "M2 ≡ M3 OrderTotal")
        t |> equal(m2.Sku,        m3.Sku,        "M2 ≡ M3 Sku")
        t |> equal(m2.Qty,        m3.Qty,        "M2 ≡ M3 Qty")
    }
}

[test(description = "a three-table join with an outer where gives identical rows in all three modes")]
def parity_three_table_join_with_outer_where(t : T?) {
    // Case 2 (v2.1): outer WHERE on a multi-join's projected alias. The
    // WHERE peel snapshots the analyzed q in place (in-place to avoid the
    // linq Mode-2/3 typer cascade that re-analysis would tickle) and
    // renders as `... FROM (3-table joinSql) AS "t0" WHERE "alias" = ?`,
    // with `alias` resolved through q.fromRowType set by the snapshot.
    with_sqlite(":memory:") $(db) {
        fixture_db_15b(db)
        var users  <- fixture_users_15b()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        var m1 <- _sql(db |> select_from(type<User_15b>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User_15b, o : Order) => u.Id == o.UserId,
                                  $(u : User_15b, o : Order) =>
                                      (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(db |> select_from(type<Item>),
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) =>
                                      (UserName = uo.UserName, OrderTotal = uo.OrderTotal,
                                       Sku = it.Sku, Qty = it.Qty))
                         |> _where(_.Sku == "BOOK"))
        var m2 <- (db |> select_from(type<User_15b>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User_15b, o : Order) => u.Id == o.UserId,
                              $(u : User_15b, o : Order) =>
                                  (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                     |> _join(db |> select_from(type<Item>),
                              $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                              $(uo : UserOrder, it : Item) =>
                                  (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku,
                                   Qty = it.Qty))
                     |> _where(_.Sku == "BOOK"))
        var m3 <- (users |> _join(orders,
                                  $(u : User_15b, o : Order) => u.Id == o.UserId,
                                  $(u : User_15b, o : Order) =>
                                      (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(items,
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) =>
                                      (UserName = uo.UserName, OrderTotal = uo.OrderTotal,
                                       Sku = it.Sku, Qty = it.Qty))
                         |> _where(_.Sku == "BOOK"))

        sort_by_user_sku(m1)
        sort_by_user_sku(m2)
        sort_by_user_sku(m3)

        // Fixture has 2 BOOK items: Item 100 (Order 10, Alice, total 100) and
        // Item 102 (Order 11, Alice, total 250). Item 101 is PEN, 103 is
        // PAPER. Bob has no BOOK item. Expect 2 rows.
        t |> equal(length(m1), 2, "3-table + WHERE Sku=BOOK: 2 rows")
        t |> equal(length(m1), length(m2), "M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].Sku,        m2[i].Sku,        "row[{i}].Sku:        M1 ≡ M2")
            t |> equal(m1[i].Qty,        m2[i].Qty,        "row[{i}].Qty:        M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].Sku,        m3[i].Sku,        "row[{i}].Sku:        M2 ≡ M3")
            t |> equal(m2[i].Qty,        m3[i].Qty,        "row[{i}].Qty:        M2 ≡ M3")
            t |> equal(m1[i].Sku,        "BOOK",           "row[{i}].Sku == BOOK (predicate)")
        }
    }
}

[test(description = "a three-table join with an outer where on an int column gives identical rows in all three modes")]
def parity_three_table_join_with_outer_where_int(t : T?) {
    // v2.1: outer WHERE on a numeric outer-projection alias (Qty), with
    // multi-arg AND.
    with_sqlite(":memory:") $(db) {
        fixture_db_15b(db)
        var users  <- fixture_users_15b()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        var m1 <- _sql(db |> select_from(type<User_15b>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User_15b, o : Order) => u.Id == o.UserId,
                                  $(u : User_15b, o : Order) =>
                                      (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(db |> select_from(type<Item>),
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) =>
                                      (UserName = uo.UserName, OrderTotal = uo.OrderTotal,
                                       Sku = it.Sku, Qty = it.Qty))
                         |> _where(_.Qty >= 2 && _.UserName == "Alice"))
        var m2 <- (db |> select_from(type<User_15b>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User_15b, o : Order) => u.Id == o.UserId,
                              $(u : User_15b, o : Order) =>
                                  (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                     |> _join(db |> select_from(type<Item>),
                              $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                              $(uo : UserOrder, it : Item) =>
                                  (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku,
                                   Qty = it.Qty))
                     |> _where(_.Qty >= 2 && _.UserName == "Alice"))
        var m3 <- (users |> _join(orders,
                                  $(u : User_15b, o : Order) => u.Id == o.UserId,
                                  $(u : User_15b, o : Order) =>
                                      (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(items,
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) =>
                                      (UserName = uo.UserName, OrderTotal = uo.OrderTotal,
                                       Sku = it.Sku, Qty = it.Qty))
                         |> _where(_.Qty >= 2 && _.UserName == "Alice"))

        sort_by_user_sku(m1)
        sort_by_user_sku(m2)
        sort_by_user_sku(m3)

        // Alice's items with Qty >= 2: Item 100 (BOOK, Qty 2), Item 101 (PEN,
        // Qty 5). Two rows.
        t |> equal(length(m1), 2, "3-table + WHERE Qty>=2 AND UserName=Alice: 2 rows")
        t |> equal(length(m1), length(m2), "M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].Sku,        m2[i].Sku,        "row[{i}].Sku:        M1 ≡ M2")
            t |> equal(m1[i].Qty,        m2[i].Qty,        "row[{i}].Qty:        M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].Sku,        m3[i].Sku,        "row[{i}].Sku:        M2 ≡ M3")
            t |> equal(m2[i].Qty,        m3[i].Qty,        "row[{i}].Qty:        M2 ≡ M3")
        }
    }
}

[test(description = "a three-table join followed by count gives the same number in all three modes")]
def parity_three_table_join_then_count(t : T?) {
    // v2.1 review-3 regression for E1 + multi-join: `_join(_join(A,B), C)
    // |> count()` (no WHERE). Pre-fix: outer process_join_call's
    // analyze_projection clobbered q.proj=NamedTuple after the multi-join
    // snapshot ran, dropping aggregateExpr=COUNT(*). Post-fix:
    // process_join_call's save/restore preserves aggregate state across
    // BOTH inner and outer analyze_projection calls.
    //
    // Outer projection: (PayPlus:int, Stock:int, Code:string,
    // Holder:string) → tuple<int;int;string;string> — structurally
    // distinct from every other test here.
    with_sqlite(":memory:") $(db) {
        fixture_db_15b(db)
        var users  <- fixture_users_15b()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        let n1 = _sql(db |> select_from(type<User_15b>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User_15b, o : Order) => u.Id == o.UserId,
                                 $(u : User_15b, o : Order) =>
                                     (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(db |> select_from(type<Item>),
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) =>
                                     (PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku,
                                      Holder = uo.UserName))
                        |> count())
        let n2 = (db |> select_from(type<User_15b>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User_15b, o : Order) => u.Id == o.UserId,
                             $(u : User_15b, o : Order) =>
                                 (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                    |> _join(db |> select_from(type<Item>),
                             $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                             $(uo : UserOrder, it : Item) =>
                                 (PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku,
                                  Holder = uo.UserName))
                    |> count())
        let n3 = (users |> _join(orders,
                                 $(u : User_15b, o : Order) => u.Id == o.UserId,
                                 $(u : User_15b, o : Order) =>
                                     (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(items,
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) =>
                                     (PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku,
                                      Holder = uo.UserName))
                        |> count())

        // 4 items each join their order's user → 4 rows total.
        t |> equal(n1, 4, "3-table | count(): 4 INNER-join rows")
        t |> equal(n1, n2, "M1 ≡ M2 count")
        t |> equal(n2, n3, "M2 ≡ M3 count")
    }
}

[test(description = "a three-table join with an outer where and count gives the same number in all three modes")]
def parity_three_table_join_outer_where_then_count(t : T?) {
    // v2.1 review-3 regression for stacked snapshots: multi-join (v2
    // snapshot via process_join_call) + WHERE-peel (v2.1 snapshot) +
    // count() — exercises BOTH snapshot paths preserving aggregate state
    // across the OUTER COUNT(*) projection.
    //
    // Outer projection: (Holder:string, PayPlus:int, Stock:int,
    // Code:string) → tuple<string;int;int;string> — structurally distinct
    // from every other test here.
    with_sqlite(":memory:") $(db) {
        fixture_db_15b(db)
        var users  <- fixture_users_15b()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        let n1 = _sql(db |> select_from(type<User_15b>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User_15b, o : Order) => u.Id == o.UserId,
                                 $(u : User_15b, o : Order) =>
                                     (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(db |> select_from(type<Item>),
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) =>
                                     (Holder = uo.UserName, PayPlus = uo.OrderTotal, Stock = it.Qty,
                                      Code = it.Sku))
                        |> _where(_.PayPlus > 75)
                        |> count())
        let n2 = (db |> select_from(type<User_15b>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User_15b, o : Order) => u.Id == o.UserId,
                             $(u : User_15b, o : Order) =>
                                 (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                    |> _join(db |> select_from(type<Item>),
                             $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                             $(uo : UserOrder, it : Item) =>
                                 (Holder = uo.UserName, PayPlus = uo.OrderTotal, Stock = it.Qty,
                                  Code = it.Sku))
                    |> _where(_.PayPlus > 75)
                    |> count())
        let n3 = (users |> _join(orders,
                                 $(u : User_15b, o : Order) => u.Id == o.UserId,
                                 $(u : User_15b, o : Order) =>
                                     (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(items,
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) =>
                                     (Holder = uo.UserName, PayPlus = uo.OrderTotal, Stock = it.Qty,
                                      Code = it.Sku))
                        |> _where(_.PayPlus > 75)
                        |> count())

        // Items joined to Order.Total > 75: order 10 (100, BOOK), order 11
        // (250, PEN+BOOK). 3 rows. Order 12 (Total=50, PAPER) excluded.
        t |> equal(n1, 3, "3-table | WHERE PayPlus>75 | count(): 3 rows")
        t |> equal(n1, n2, "M1 ≡ M2 count")
        t |> equal(n2, n3, "M2 ≡ M3 count")
    }
}

// Parity check for tutorial 16 — every join shape (INNER, LEFT,
// RIGHT, FULL OUTER, CROSS).
//
// Each test runs the same chain in three modes and compares
// row-by-row: M1 — `_sql(...)` lowering through SQLite M2 — same
// chain WITHOUT `_sql` (linq runtime over SQL source) M3 — same chain
// on plain in-memory arrays
//
// Fixture asymmetries: Carol (id=3) has no orders → LEFT-only,
// FULL-only. Order#13 (UserId=99) has no user → RIGHT-only,
// FULL-only. Each test's projection is structurally distinct
// (different named-tuple field names) to avoid the
// recordNames-collision regression (#2557).

def fixture_db_16(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice"))
    db |> insert(User(Id = 2, Name = "Bob"))
    db |> insert(User(Id = 3, Name = "Carol"))
    db |> insert(Order(Id = 10, UserId = 1,  Total = 100))
    db |> insert(Order(Id = 11, UserId = 1,  Total = 250))
    db |> insert(Order(Id = 12, UserId = 2,  Total =  50))
    db |> insert(Order(Id = 13, UserId = 99, Total = 999))   // orphan — no matching user
}

def fixture_users_16() : array<User> {
    return <- [
        User(Id = 1, Name = "Alice"),
        User(Id = 2, Name = "Bob"),
        User(Id = 3, Name = "Carol")
    ]
}

def fixture_orders_16() : array<Order> {
    return <- [
        Order(Id = 10, UserId = 1,  Total = 100),
        Order(Id = 11, UserId = 1,  Total = 250),
        Order(Id = 12, UserId = 2,  Total =  50),
        Order(Id = 13, UserId = 99, Total = 999)
    ]
}

def sort_left(var rows : array<auto(R)>) {
    // Both Alice rows are content-identical ((Name=Alice,
    // HasOrder=true)), so ordering between them doesn't matter — sort by
    // Name alone.
    rows |> sort() <| cmp_by(_.Name)
}

def sort_right(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.OrderId)
}

def sort_full(var rows : array<auto(R)>) {
    // Multi-key — both probes drive deterministic order across
    // (T,T)/(T,F)/(F,T) groups.
    rows |> sort() $(a, b) {
        if (a.HasUser != b.HasUser) {
            return !a.HasUser && b.HasUser
        }
        return !a.HasOrder && b.HasOrder
    }
}

def sort_cross(var rows : array<auto(R)>) {
    rows |> sort() <| cmp_by(_.UserName, _.OrderId)
}

[test(description = "a left join keeps unmatched left rows identically in all three modes")]
def parity_left_join_basic(t : T?) {
    // 4 rows expected: Alice/true × 2, Bob/true, Carol/false. Orphan
    // Order#13 doesn't surface (LEFT).
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _left_join(db |> select_from(type<Order>),
                                       $(u : User, o : Order)         => u.Id == o.UserId,
                                       $(u : User, o : Option<Order>) =>
                                           (Name = u.Name, HasOrder = o |> is_some)))
        var m2 <- (db |> select_from(type<User>)
                     |> _left_join(db |> select_from(type<Order>),
                                   $(u : User, o : Order)         => u.Id == o.UserId,
                                   $(u : User, o : Option<Order>) =>
                                       (Name = u.Name, HasOrder = o |> is_some)))
        var m3 <- (users |> _left_join(orders,
                                       $(u : User, o : Order)         => u.Id == o.UserId,
                                       $(u : User, o : Option<Order>) =>
                                           (Name = u.Name, HasOrder = o |> is_some)))

        sort_left(m1)
        sort_left(m2)
        sort_left(m3)

        t |> equal(length(m1), 4, "left join row count: 4 (Alice ×2, Bob, Carol)")
        t |> equal(length(m1), length(m2), "left join: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "left join: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Name,     m2[i].Name,     "row[{i}].Name:     M1 ≡ M2")
            t |> equal(m1[i].HasOrder, m2[i].HasOrder, "row[{i}].HasOrder: M1 ≡ M2")
            t |> equal(m2[i].Name,     m3[i].Name,     "row[{i}].Name:     M2 ≡ M3")
            t |> equal(m2[i].HasOrder, m3[i].HasOrder, "row[{i}].HasOrder: M2 ≡ M3")
        }
    }
}

[test(description = "a right join keeps unmatched right rows identically in all three modes")]
def parity_right_join_basic(t : T?) {
    // 4 rows expected: orders #10/#11/#12 with HasUser=true, orphan #13
    // with HasUser=false.
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _right_join(db |> select_from(type<Order>),
                                        $(u : User, o : Order)         => u.Id == o.UserId,
                                        $(u : Option<User>, o : Order) =>
                                            (HasUser = u |> is_some, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User>)
                     |> _right_join(db |> select_from(type<Order>),
                                    $(u : User, o : Order)         => u.Id == o.UserId,
                                    $(u : Option<User>, o : Order) =>
                                        (HasUser = u |> is_some, OrderId = o.Id)))
        var m3 <- (users |> _right_join(orders,
                                        $(u : User, o : Order)         => u.Id == o.UserId,
                                        $(u : Option<User>, o : Order) =>
                                            (HasUser = u |> is_some, OrderId = o.Id)))

        sort_right(m1)
        sort_right(m2)
        sort_right(m3)

        t |> equal(length(m1), 4, "right join row count: 4 (orders 10/11/12 with HasUser=true, orphan 13 with HasUser=false)")
        t |> equal(length(m1), length(m2), "right join: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "right join: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].HasUser, m2[i].HasUser, "row[{i}].HasUser: M1 ≡ M2")
            t |> equal(m1[i].OrderId, m2[i].OrderId, "row[{i}].OrderId: M1 ≡ M2")
            t |> equal(m2[i].HasUser, m3[i].HasUser, "row[{i}].HasUser: M2 ≡ M3")
            t |> equal(m2[i].OrderId, m3[i].OrderId, "row[{i}].OrderId: M2 ≡ M3")
        }
    }
}

[test(description = "a full outer join keeps unmatched rows of both sides identically in all three modes")]
def parity_full_outer_join_basic(t : T?) {
    // 5 rows expected: 3 matched (Alice ×2, Bob), Carol with
    // HasOrder=false, orphan with HasUser=false.
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _full_outer_join(db |> select_from(type<Order>),
                                             $(u : User, o : Order)                 =>
                                                 u.Id == o.UserId,
                                             $(u : Option<User>, o : Option<Order>) =>
                                                 (HasUser = u |> is_some, HasOrder = o |> is_some)))
        var m2 <- (db |> select_from(type<User>)
                     |> _full_outer_join(db |> select_from(type<Order>),
                                         $(u : User, o : Order)                 => u.Id == o.UserId,
                                         $(u : Option<User>, o : Option<Order>) =>
                                             (HasUser = u |> is_some, HasOrder = o |> is_some)))
        var m3 <- (users |> _full_outer_join(orders,
                                             $(u : User, o : Order)                 =>
                                                 u.Id == o.UserId,
                                             $(u : Option<User>, o : Option<Order>) =>
                                                 (HasUser = u |> is_some, HasOrder = o |> is_some)))

        sort_full(m1)
        sort_full(m2)
        sort_full(m3)

        t |> equal(length(m1), 5,
                   "full outer row count: 5 (3 matched + 1 LEFT-only + 1 RIGHT-only)")
        t |> equal(length(m1), length(m2), "full outer: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "full outer: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].HasUser,  m2[i].HasUser,  "row[{i}].HasUser:  M1 ≡ M2")
            t |> equal(m1[i].HasOrder, m2[i].HasOrder, "row[{i}].HasOrder: M1 ≡ M2")
            t |> equal(m2[i].HasUser,  m3[i].HasUser,  "row[{i}].HasUser:  M2 ≡ M3")
            t |> equal(m2[i].HasOrder, m3[i].HasOrder, "row[{i}].HasOrder: M2 ≡ M3")
        }
    }
}

[test(description = "a cross join gives the same product in all three modes")]
def parity_cross_join_basic(t : T?) {
    // 12 rows expected: 3 users × 4 orders, every pair.
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _cross_join(db |> select_from(type<Order>),
                                        $(u : User, o : Order) =>
                                            (UserName = u.Name, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User>)
                     |> _cross_join(db |> select_from(type<Order>),
                                    $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id)))
        var m3 <- (users |> _cross_join(orders,
                                        $(u : User, o : Order) =>
                                            (UserName = u.Name, OrderId = o.Id)))

        sort_cross(m1)
        sort_cross(m2)
        sort_cross(m3)

        t |> equal(length(m1), 12, "cross row count: 12 (3 users × 4 orders)")
        t |> equal(length(m1), length(m2), "cross: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "cross: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName, m2[i].UserName, "row[{i}].UserName: M1 ≡ M2")
            t |> equal(m1[i].OrderId,  m2[i].OrderId,  "row[{i}].OrderId:  M1 ≡ M2")
            t |> equal(m2[i].UserName, m3[i].UserName, "row[{i}].UserName: M2 ≡ M3")
            t |> equal(m2[i].OrderId,  m3[i].OrderId,  "row[{i}].OrderId:  M2 ≡ M3")
        }
    }
}

// ===== Where-on-preserved-side parity =====
//
// RIGHT/FULL OUTER preserve a side that can have NULL on the other;
// naive SQL emission (outer WHERE on LHS, RHS-WHERE-in-ON)
// drops/keeps the wrong rows. Tests below pin the divergence: M3
// (plain array) is the truth, M1 (`_sql`) must match. Cross has no
// preserved side; only its WHERE-merge precedence (parens around both
// sides) is exercised here.

[test(description = "a right join with a where on the left side gives identical rows in all three modes")]
def parity_right_join_with_lhs_where(t : T?) {
    // LHS filter `u.Id < 3` removes Carol. Orphan Order#13 must STILL
    // surface as (none, 13). Pre-fix: M1 emits outer `WHERE u.Id < 3`;
    // orphan row has NULL u → NULL < 3 → dropped (3 rows). Post-fix: LHS
    // wrapped as `(SELECT * FROM Users WHERE Id<3) AS t0` → 4 rows.
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>) |> _where(_.Id < 3)
                         |> _right_join(db |> select_from(type<Order>),
                                        $(u : User, o : Order)         => u.Id == o.UserId,
                                        $(u : Option<User>, o : Order) =>
                                            (HasUser = u |> is_some, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User>) |> _where(_.Id < 3)
                     |> _right_join(db |> select_from(type<Order>),
                                    $(u : User, o : Order)         => u.Id == o.UserId,
                                    $(u : Option<User>, o : Order) =>
                                        (HasUser = u |> is_some, OrderId = o.Id)))
        var m3 <- (users |> _where(_.Id < 3)
                         |> _right_join(orders,
                                        $(u : User, o : Order)         => u.Id == o.UserId,
                                        $(u : Option<User>, o : Order) =>
                                            (HasUser = u |> is_some, OrderId = o.Id)))

        sort_right(m1)
        sort_right(m2)
        sort_right(m3)

        t |> equal(length(m3), 4, "M3 row count: 4 (10/Alice, 11/Alice, 12/Bob, 13/orphan)")
        t |> equal(length(m1), length(m3), "right + lhs where: M1 length ≡ M3 (linq truth)")
        t |> equal(length(m2), length(m3), "right + lhs where: M2 length ≡ M3")
        for (i in range(length(m3))) {
            t |> equal(m1[i].HasUser, m3[i].HasUser, "row[{i}].HasUser: M1 ≡ M3")
            t |> equal(m1[i].OrderId, m3[i].OrderId, "row[{i}].OrderId: M1 ≡ M3")
            t |> equal(m2[i].HasUser, m3[i].HasUser, "row[{i}].HasUser: M2 ≡ M3")
            t |> equal(m2[i].OrderId, m3[i].OrderId, "row[{i}].OrderId: M2 ≡ M3")
        }
    }
}

[test(description = "a right join with a where on the right side gives identical rows in all three modes")]
def parity_right_join_with_rhs_where(t : T?) {
    // RHS filter `o.Total < 500` removes Order#13 (Total=999). Filtered
    // orders all match users → 3 rows. Pre-fix: M1 puts rightWhere in ON;
    // Order#13 fails ON → emits (none, 13) instead of being dropped (4
    // rows). Post-fix: RHS wrapped as `(SELECT * FROM Orders WHERE
    // Total<500) AS t1` → 3 rows.
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _right_join(db |> select_from(type<Order>) |> _where(_.Total < 500),
                                        $(u : User, o : Order)         => u.Id == o.UserId,
                                        $(u : Option<User>, o : Order) =>
                                            (HasUser = u |> is_some, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User>)
                     |> _right_join(db |> select_from(type<Order>) |> _where(_.Total < 500),
                                    $(u : User, o : Order)         => u.Id == o.UserId,
                                    $(u : Option<User>, o : Order) =>
                                        (HasUser = u |> is_some, OrderId = o.Id)))
        var m3 <- (users |> _right_join(orders |> _where(_.Total < 500),
                                        $(u : User, o : Order)         => u.Id == o.UserId,
                                        $(u : Option<User>, o : Order) =>
                                            (HasUser = u |> is_some, OrderId = o.Id)))

        sort_right(m1)
        sort_right(m2)
        sort_right(m3)

        t |> equal(length(m3), 3, "M3 row count: 3 (orders 10/11/12; orphan 13 filtered)")
        t |> equal(length(m1), length(m3), "right + rhs where: M1 length ≡ M3")
        t |> equal(length(m2), length(m3), "right + rhs where: M2 length ≡ M3")
        for (i in range(length(m3))) {
            t |> equal(m1[i].HasUser, m3[i].HasUser, "row[{i}].HasUser: M1 ≡ M3")
            t |> equal(m1[i].OrderId, m3[i].OrderId, "row[{i}].OrderId: M1 ≡ M3")
            t |> equal(m2[i].HasUser, m3[i].HasUser, "row[{i}].HasUser: M2 ≡ M3")
            t |> equal(m2[i].OrderId, m3[i].OrderId, "row[{i}].OrderId: M2 ≡ M3")
        }
    }
}

[test(description = "a full outer join with a where on the left side gives identical rows in all three modes")]
def parity_full_outer_with_lhs_where(t : T?) {
    // LHS filter `u.Id < 3` removes Carol. M3: 4 rows (3 matched + orphan
    // 13 as (none,some)). Pre-fix: M1 outer WHERE drops rows where u IS
    // NULL → 3 rows. Post-fix: LHS-wrap → 4 rows.
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>) |> _where(_.Id < 3)
                         |> _full_outer_join(db |> select_from(type<Order>),
                                             $(u : User, o : Order)                 =>
                                                 u.Id == o.UserId,
                                             $(u : Option<User>, o : Option<Order>) =>
                                                 (HasUser = u |> is_some, HasOrder = o |> is_some)))
        var m2 <- (db |> select_from(type<User>) |> _where(_.Id < 3)
                     |> _full_outer_join(db |> select_from(type<Order>),
                                         $(u : User, o : Order)                 => u.Id == o.UserId,
                                         $(u : Option<User>, o : Option<Order>) =>
                                             (HasUser = u |> is_some, HasOrder = o |> is_some)))
        var m3 <- (users |> _where(_.Id < 3)
                         |> _full_outer_join(orders,
                                             $(u : User, o : Order)                 =>
                                                 u.Id == o.UserId,
                                             $(u : Option<User>, o : Option<Order>) =>
                                                 (HasUser = u |> is_some, HasOrder = o |> is_some)))

        sort_full(m1)
        sort_full(m2)
        sort_full(m3)

        t |> equal(length(m3), 4, "M3 row count: 4 (3 matched + 1 RIGHT-only orphan)")
        t |> equal(length(m1), length(m3), "full outer + lhs where: M1 length ≡ M3")
        t |> equal(length(m2), length(m3), "full outer + lhs where: M2 length ≡ M3")
        for (i in range(length(m3))) {
            t |> equal(m1[i].HasUser,  m3[i].HasUser,  "row[{i}].HasUser:  M1 ≡ M3")
            t |> equal(m1[i].HasOrder, m3[i].HasOrder, "row[{i}].HasOrder: M1 ≡ M3")
            t |> equal(m2[i].HasUser,  m3[i].HasUser,  "row[{i}].HasUser:  M2 ≡ M3")
            t |> equal(m2[i].HasOrder, m3[i].HasOrder, "row[{i}].HasOrder: M2 ≡ M3")
        }
    }
}

[test(description = "a full outer join with a where on the right side gives identical rows in all three modes")]
def parity_full_outer_with_rhs_where(t : T?) {
    // RHS filter `o.Total < 500` removes Order#13. M3: 4 rows (3 matched
    // + Carol unmatched LHS). Pre-fix: M1 ON keeps Order#13 as
    // (none,some) → 5 rows. Post-fix: RHS-wrap → 4 rows.
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _full_outer_join(db |> select_from(type<Order>) |> _where(_.Total < 500),
                                             $(u : User, o : Order)                 =>
                                                 u.Id == o.UserId,
                                             $(u : Option<User>, o : Option<Order>) =>
                                                 (HasUser = u |> is_some, HasOrder = o |> is_some)))
        var m2 <- (db |> select_from(type<User>)
                     |> _full_outer_join(db |> select_from(type<Order>) |> _where(_.Total < 500),
                                         $(u : User, o : Order)                 => u.Id == o.UserId,
                                         $(u : Option<User>, o : Option<Order>) =>
                                             (HasUser = u |> is_some, HasOrder = o |> is_some)))
        var m3 <- (users |> _full_outer_join(orders |> _where(_.Total < 500),
                                             $(u : User, o : Order)                 =>
                                                 u.Id == o.UserId,
                                             $(u : Option<User>, o : Option<Order>) =>
                                                 (HasUser = u |> is_some, HasOrder = o |> is_some)))

        sort_full(m1)
        sort_full(m2)
        sort_full(m3)

        t |> equal(length(m3), 4, "M3 row count: 4 (3 matched + Carol LEFT-only)")
        t |> equal(length(m1), length(m3), "full outer + rhs where: M1 length ≡ M3")
        t |> equal(length(m2), length(m3), "full outer + rhs where: M2 length ≡ M3")
        for (i in range(length(m3))) {
            t |> equal(m1[i].HasUser,  m3[i].HasUser,  "row[{i}].HasUser:  M1 ≡ M3")
            t |> equal(m1[i].HasOrder, m3[i].HasOrder, "row[{i}].HasOrder: M1 ≡ M3")
            t |> equal(m2[i].HasUser,  m3[i].HasUser,  "row[{i}].HasUser:  M2 ≡ M3")
            t |> equal(m2[i].HasOrder, m3[i].HasOrder, "row[{i}].HasOrder: M2 ≡ M3")
        }
    }
}

[test(description = "a cross join with an or predicate on the left side gives identical rows in all three modes")]
def parity_cross_with_or_lhs_where(t : T?) {
    // LHS `u.Id == 1 || u.Id == 3` keeps Alice + Carol. RHS `o.Total <
    // 500` keeps 10/11/12. M3: 2 × 3 = 6 rows. Pre-fix: cross merge emits
    // `(u.Id=1 OR u.Id=3) AND (Total<500)` ← MISSING outer parens → SQL
    // parses as `u.Id=1 OR (u.Id=3 AND Total<500)`: Alice pairs with all
    // 4 orders + Carol with 3 = 7 rows. Post-fix: outer parens added → 6
    // rows.
    with_sqlite(":memory:") $(db) {
        fixture_db_16(db)
        let users  <- fixture_users_16()
        let orders <- fixture_orders_16()

        var m1 <- _sql(db |> select_from(type<User>) |> _where(_.Id == 1 || _.Id == 3)
                         |> _cross_join(db |> select_from(type<Order>) |> _where(_.Total < 500),
                                        $(u : User, o : Order) =>
                                            (UserName = u.Name, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User>) |> _where(_.Id == 1 || _.Id == 3)
                     |> _cross_join(db |> select_from(type<Order>) |> _where(_.Total < 500),
                                    $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id)))
        var m3 <- (users |> _where(_.Id == 1 || _.Id == 3)
                         |> _cross_join(orders |> _where(_.Total < 500),
                                        $(u : User, o : Order) =>
                                            (UserName = u.Name, OrderId = o.Id)))

        sort_cross(m1)
        sort_cross(m2)
        sort_cross(m3)

        t |> equal(length(m3), 6, "M3 row count: 6 (Alice + Carol × orders 10/11/12)")
        t |> equal(length(m1), length(m3),
                   "cross + or-lhs where: M1 length ≡ M3 (parens precedence)")
        t |> equal(length(m2), length(m3), "cross + or-lhs where: M2 length ≡ M3")
        for (i in range(length(m3))) {
            t |> equal(m1[i].UserName, m3[i].UserName, "row[{i}].UserName: M1 ≡ M3")
            t |> equal(m1[i].OrderId,  m3[i].OrderId,  "row[{i}].OrderId:  M1 ≡ M3")
            t |> equal(m2[i].UserName, m3[i].UserName, "row[{i}].UserName: M2 ≡ M3")
            t |> equal(m2[i].OrderId,  m3[i].OrderId,  "row[{i}].OrderId:  M2 ≡ M3")
        }
    }
}
