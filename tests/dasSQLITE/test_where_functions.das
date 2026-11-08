options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq
require daslib/strings_boost

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "Ford",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Ferrari",  Price = 9999))
    db |> insert(Car(Id = 3, Name = "Audi",     Price = 200))
    db |> insert(Car(Id = 4, Name = "Lada",     Price = 50))
}

[test(description = "starts_with in a where clause returns exactly the rows whose Name begins with the prefix")]
def test_where_starts_with_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let prefix = "F"
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Name |> starts_with(prefix)))
        t |> equal(length(cars), 2, "2 cars start with 'F'")
        t |> equal(cars[0].Name, "Ford",    "first match")
        t |> equal(cars[1].Name, "Ferrari", "second match")
    }
}

[test(description = "starts_with in a where clause compiles to a case-sensitive GLOB parameter comparison")]
def test_where_starts_with_emits_like_pattern(t : T?) {
    var _db = SqlRunner()
    let _prefix = "F"
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Name |> starts_with(_prefix)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Name\" GLOB ?",
        "starts_with -> case-sensitive GLOB on SQLite (ci_like provider)")
}

[test(description = "a literal percent sign in a starts_with prefix matches only itself, not as a pattern wildcard")]
def test_where_starts_with_escapes_literal_percent(t : T?) {
    // User typing "50%" should match literal "50%", not "50…anything".
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "50% off",  Price = 1))
        db |> insert(Car(Id = 2, Name = "5000 USD", Price = 2))
        let prefix = "50%"
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Name |> starts_with(prefix)))
        t |> equal(length(cars), 1, "literal '%' in user prefix matches only the '50% off' row")
        t |> equal(cars[0].Id, 1, "matched the literal-percent row")
    }
}

[sql_table(name="Cars")]
struct Car_25_where_end {
    @sql_primary_key Id : int
    Name  : string
}

def private fixture_25_where_end(db : SqlRunner) {
    db |> create_table(type<Car_25_where_end>)
    db |> insert(Car_25_where_end(Id = 1, Name = "Ford"))
    db |> insert(Car_25_where_end(Id = 2, Name = "Honda"))
    db |> insert(Car_25_where_end(Id = 3, Name = "Audi"))
    db |> insert(Car_25_where_end(Id = 4, Name = "Lada"))
}

[test(description = "ends_with in a where clause returns exactly the rows whose Name ends with the suffix")]
def test_where_ends_with_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_25_where_end(db)
        let suffix = "da"
        let cars <- _sql(db |> select_from(type<Car_25_where_end>)
                           |> _where(_.Name |> ends_with(suffix)))
        t |> equal(length(cars), 2, "2 cars end in 'da' (Honda, Lada)")
        t |> equal(cars[0].Name, "Honda", "first match")
        t |> equal(cars[1].Name, "Lada",  "second match")
    }
}

[test(description = "ends_with in a where clause compiles to a case-sensitive GLOB parameter comparison")]
def test_where_ends_with_emits_like_suffix(t : T?) {
    var _db = SqlRunner()
    let _suffix = "X"
    let sql = _sql_text(_db |> select_from(type<Car_25_where_end>)
                          |> _where(_.Name |> ends_with(_suffix)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE \"Name\" GLOB ?",
        "ends_with -> case-sensitive GLOB on SQLite (ci_like provider)")
}

[test(description = "a literal underscore in an ends_with suffix matches only itself, not as a single-character wildcard")]
def test_where_ends_with_escapes_literal_underscore(t : T?) {
    // Literal '_' must not act as the LIKE wildcard.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car_25_where_end>)
        db |> insert(Car_25_where_end(Id = 1, Name = "model_X"))
        db |> insert(Car_25_where_end(Id = 2, Name = "modelAX"))
        let suffix = "_X"
        let cars <- _sql(db |> select_from(type<Car_25_where_end>)
                           |> _where(_.Name |> ends_with(suffix)))
        t |> equal(length(cars), 1, "literal '_' in user suffix matches only 'model_X'")
        t |> equal(cars[0].Id, 1, "matched the literal-underscore row")
    }
}

[sql_table(name="Cars")]
struct Car_26_where_con {
    @sql_primary_key Id : int
    Name  : string
}

def private fixture_26_where_con(db : SqlRunner) {
    db |> create_table(type<Car_26_where_con>)
    db |> insert(Car_26_where_con(Id = 1, Name = "Ford"))
    db |> insert(Car_26_where_con(Id = 2, Name = "Honda"))
    db |> insert(Car_26_where_con(Id = 3, Name = "Toyota"))
    db |> insert(Car_26_where_con(Id = 4, Name = "Lada"))
}

[test(description = "contains in a where clause returns exactly the rows whose Name includes the needle")]
def test_where_contains_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_26_where_con(db)
        let needle = "o"
        let cars <- _sql(db |> select_from(type<Car_26_where_con>)
                           |> _where(_.Name |> contains(needle)))
        t |> equal(length(cars), 3, "3 cars contain 'o' (Ford, Honda, Toyota)")
    }
}

[test(description = "contains in a where clause compiles to a case-sensitive GLOB parameter comparison")]
def test_where_contains_emits_like_wildcards(t : T?) {
    var _db = SqlRunner()
    let _needle = "x"
    let sql = _sql_text(_db |> select_from(type<Car_26_where_con>)
                          |> _where(_.Name |> contains(_needle)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE \"Name\" GLOB ?",
        "contains -> case-sensitive GLOB on SQLite (ci_like provider)")
}

[test(description = "a backslash in a contains needle is matched literally")]
def test_where_contains_escapes_literal_backslash(t : T?) {
    // Literal '\' in the needle must not collide with the ESCAPE char.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car_26_where_con>)
        db |> insert(Car_26_where_con(Id = 1, Name = "C:\\Windows"))
        db |> insert(Car_26_where_con(Id = 2, Name = "Toyota"))
        let needle = "\\Windows"
        let cars <- _sql(db |> select_from(type<Car_26_where_con>)
                           |> _where(_.Name |> contains(needle)))
        t |> equal(length(cars), 1, "backslash needle matches the path row")
        t |> equal(cars[0].Id, 1, "matched the literal-backslash row")
    }
}

[sql_table(name="Cars")]
struct Car_27_where_to_ {
    @sql_primary_key Id : int
    Name  : string
}

def private fixture_27_where_to_(db : SqlRunner) {
    db |> create_table(type<Car_27_where_to_>)
    db |> insert(Car_27_where_to_(Id = 1, Name = "Ford"))
    db |> insert(Car_27_where_to_(Id = 2, Name = "Honda"))
    db |> insert(Car_27_where_to_(Id = 3, Name = "Audi"))
}

[test(description = "to_lower in a where clause matches rows case-insensitively at runtime")]
def test_where_to_lower_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_27_where_to_(db)
        let target = "ford"
        let cars <- _sql(db |> select_from(type<Car_27_where_to_>)
                           |> _where(_.Name |> to_lower() == target))
        t |> equal(length(cars), 1, "case-insensitive match")
        t |> equal(cars[0].Name, "Ford", "matched")
    }
}

[test(description = "to_lower in a where clause compiles to the SQL LOWER function")]
def test_where_to_lower_emits_lower_call(t : T?) {
    var _db = SqlRunner()
    let _target = "x"
    let sql = _sql_text(_db |> select_from(type<Car_27_where_to_>)
                          |> _where(_.Name |> to_lower() == _target))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE LOWER(\"Name\") = ?",
        "to_lower -> LOWER(...)")
}

[test(description = "to_upper in a where clause compiles to the SQL UPPER function")]
def test_where_to_upper_emits_upper_call(t : T?) {
    var _db = SqlRunner()
    let _target = "X"
    let sql = _sql_text(_db |> select_from(type<Car_27_where_to_>)
                          |> _where(_.Name |> to_upper() == _target))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE UPPER(\"Name\") = ?",
        "to_upper -> UPPER(...)")
}

[sql_table(name="Cars")]
struct Car_28_where_len {
    @sql_primary_key Id : int
    Name  : string
}

def private fixture_28_where_len(db : SqlRunner) {
    db |> create_table(type<Car_28_where_len>)
    db |> insert(Car_28_where_len(Id = 1, Name = "Audi"))
    db |> insert(Car_28_where_len(Id = 2, Name = "Ferrari"))
    db |> insert(Car_28_where_len(Id = 3, Name = "Lamborghini"))
}

[test(description = "length of a string column in a where clause filters rows by character count")]
def test_where_length_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_28_where_len(db)
        let cutoff = 5
        let cars <- _sql(db |> select_from(type<Car_28_where_len>)
                           |> _where(length(_.Name) > cutoff))
        t |> equal(length(cars), 2, "Ferrari + Lamborghini have length > 5")
    }
}

[test(description = "length of a string column in a where clause compiles to the SQL LENGTH function")]
def test_where_length_emits_length_call(t : T?) {
    var _db = SqlRunner()
    let _cutoff = 5
    let sql = _sql_text(_db |> select_from(type<Car_28_where_len>)
                          |> _where(length(_.Name) > _cutoff))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE LENGTH(\"Name\") > ?",
        "length -> LENGTH(...)")
}

[sql_table(name="Score")]
struct Score {
    @sql_primary_key Id : int
    Delta : int
}

def private fixture_29_where_abs(db : SqlRunner) {
    db |> create_table(type<Score>)
    db |> insert(Score(Id = 1, Delta = -150))
    db |> insert(Score(Id = 2, Delta = 80))
    db |> insert(Score(Id = 3, Delta = 200))
    db |> insert(Score(Id = 4, Delta = -50))
}

[test(description = "abs of a column expression in a where clause filters rows by magnitude")]
def test_where_abs_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_29_where_abs(db)
        let threshold = 100
        let rows <- _sql(db |> select_from(type<Score>)
                           |> _where((_.Delta |> abs()) > threshold))
        t |> equal(length(rows), 2, "|delta|>100: Id 1 (-150) + Id 3 (200)")
    }
}

[test(description = "abs of a column expression in a where clause compiles to the SQL ABS function")]
def test_where_abs_emits_abs_call(t : T?) {
    var _db = SqlRunner()
    let _threshold = 100
    let sql = _sql_text(_db |> select_from(type<Score>)
                          |> _where((_.Delta |> abs()) > _threshold))
    t |> equal(sql,
        "SELECT \"Id\", \"Delta\" FROM \"Score\" WHERE ABS(\"Delta\") > ?",
        "abs -> ABS(...)")
}
