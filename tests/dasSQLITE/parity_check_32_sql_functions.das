options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 32 — `[sql_function]` user-defined SQL scalars.
//
// Chain shape: `_where(score(_.Hp, _.Armor) >= 100.0lf)` — a daslang
// function tagged `[sql_function]` invoked inside the predicate. The
// annotation does two things at once:
//   1. Auto-registers the function as a SQLite scalar at open() time
//   2. Tells the `_sql` analyzer it's safe to lower the call to SQL
//      (instead of falling back to per-row binds)
//
// Mode 1 emits `WHERE score("Hp", "Armor") >= ?` — SQLite calls back
// into the registered trampoline.
// Mode 2/3 just call `score(...)` as a normal daslang function over each
// row. Same function body, different execution path.
//
// Parity hinges on `[sql_function]` being a TRANSPARENT annotation: the
// function works identically from daslang code regardless of the
// SQL-binding side.

[sql_function(deterministic=true)]
def score(hp : float; armor : int) : double {
    return double(hp) * (1.0lf - double(armor) / 100.0lf)
}

[sql_table(name = "Enemies")]
struct Enemy {
    @sql_primary_key Id : int
    Name : string
    Hp   : float
    Armor : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Enemy>)
    db |> insert(Enemy(Id = 1, Name = "goblin", Hp = 100.0f, Armor = 10))   // score = 90
    db |> insert(Enemy(Id = 2, Name = "ogre",   Hp = 800.0f, Armor = 30))   // score = 560
    db |> insert(Enemy(Id = 3, Name = "dragon", Hp = 5000.0f, Armor = 60))  // score = 2000
}

def fixture_array() : array<Enemy> {
    var arr : array<Enemy>
    arr |> emplace(Enemy(Id = 1, Name = "goblin", Hp = 100.0f, Armor = 10))
    arr |> emplace(Enemy(Id = 2, Name = "ogre",   Hp = 800.0f, Armor = 30))
    arr |> emplace(Enemy(Id = 3, Name = "dragon", Hp = 5000.0f, Armor = 60))
    return <- arr
}

def enemy_eq(a : Enemy; b : Enemy) : bool {
    return (a.Id == b.Id
        && a.Name == b.Name
        && a.Hp == b.Hp
        && a.Armor == b.Armor)
}

def enemies_eq(a : array<Enemy>; b : array<Enemy>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!enemy_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

def sort_by_id(var rows : array<Enemy>) : array<Enemy> {
    rows |> sort() <| $(a, b) {
        return a.Id < b.Id
    }
    return <- rows
}

[test]
def parity_sql_function_in_where(t : T?) {
    // _where(score(_.Hp, _.Armor) >= 100.0lf) — keeps ogre + dragon
    // (scores 560 and 2000); drops goblin (score 90).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<Enemy>) |> _where(score(_.Hp, _.Armor) >= 100.0lf))
        var m2 <- (db |> select_from(type<Enemy>) |> _where(score(_.Hp, _.Armor) >= 100.0lf))
        var m3 <- (arr |> _where(score(_.Hp, _.Armor) >= 100.0lf))

        m1 <- sort_by_id(m1)
        m2 <- sort_by_id(m2)
        m3 <- sort_by_id(m3)

        t |> equal(length(m1), 2, "sql_function where: 2 strong enemies")
        t |> success(enemies_eq(m1, m2), "sql_function where: M1 ≡ M2")
        t |> success(enemies_eq(m2, m3), "sql_function where: M2 ≡ M3")
    }
}
