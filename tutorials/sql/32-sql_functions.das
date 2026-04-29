options gen2

require sqlite/sqlite_boost
require sqlite/sqlite_linq
require math

// Tutorial 32 — User-defined SQL scalar functions.
//
// SQL is fluent at filtering and grouping but speaks a small built-in
// vocabulary. When an expression doesn't fit (a custom hash, a domain-
// specific scoring formula, a string normalizer), `register_function`
// binds an ordinary daslang function as a SQL scalar so it can be used
// inside any expression — `WHERE`, projection, `ORDER BY`, indexes,
// view bodies, anywhere a built-in scalar would go.
//
// Lifetime: the registration is per-connection. The function is
// available until the connection closes (or `DROP FUNCTION` if exposed
// in a future chunk). Re-registering the same name replaces the prior
// binding.
//
// NULL handling: if any argument is SQL NULL, the trampoline emits NULL
// without invoking the daslang function — the standard SQL behavior of
// built-in scalars (`abs(NULL) -> NULL`, `length(NULL) -> NULL`).
// Explicit `Option<T>` arg types for in-function NULL handling are
// planned but not in this chunk.
//
// Panic recovery: a panic inside the daslang function is caught and
// surfaced to SQLite as an error on that statement; `try_query` returns
// `Err` carrying the panic message. The connection itself is unaffected
// and the next statement runs normally.

// --- Supported types ---------------------------------------------------
// register_function maps daslang scalar types to SQLite cells:
//
//   int, int64, bool   ↔ INTEGER
//   float, double      ↔ REAL
//   string             ↔ TEXT
//
// Unsupported types (structs, arrays, pointers, lambdas, classes) fail
// at compile time with a per-position diagnostic.

// --- Example 1 — A scoring formula -------------------------------------
// Game-style damage calculation: base × (1 − armor/100) × multiplier.
// Marked deterministic so SQLite is allowed to factor it out of inner
// loops or use it as the indexed expression in a `CREATE INDEX … ON …`.
def damage_formula(base : float; armor : int; mult : float) : float {
    return base * (1.0f - float(armor) / 100.0f) * mult
}

// --- Example 2 — String normalization ----------------------------------
// A trivial "hash" — in real code this would be a domain hasher.
def short_hash(s : string) : int64 {
    var h = 1469598103934665603l
    for (c in s) {
        h = (h ^ int64(c)) * 1099511628211l
    }
    return h
}

// --- Example 3 — Trip-by-trip distance ---------------------------------
def euclid(x1 : float; y1 : float; x2 : float; y2 : float) : float {
    let dx = x2 - x1
    let dy = y2 - y1
    return sqrt(dx * dx + dy * dy)
}

[sql_table(name = "Enemies")]
struct Enemy {
    @sql_primary_key Id : int
    Name : string
    Hp : float
    Armor : int
}

[sql_table(name = "Trips")]
struct Trip {
    @sql_primary_key Id : int
    StartX : float
    StartY : float
    EndX : float
    EndY : float
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Enemy>)
        db |> create_table(type<Trip>)

        db |> insert(Enemy(Id = 1, Name = "goblin",  Hp = 100.0f, Armor = 10))
        db |> insert(Enemy(Id = 2, Name = "ogre",    Hp = 800.0f, Armor = 30))
        db |> insert(Enemy(Id = 3, Name = "dragon",  Hp = 5000.0f, Armor = 60))
        db |> insert(Trip(Id = 1, StartX = 0.0f, StartY = 0.0f, EndX = 3.0f, EndY = 4.0f))
        db |> insert(Trip(Id = 2, StartX = 0.0f, StartY = 0.0f, EndX = 6.0f, EndY = 8.0f))

        // --- Register: type-derived from the function pointer's signature
        // The macro inspects `@@damage_formula` at compile time, derives
        // (float, int, float) -> float, and ships per-arg + return tags
        // to the C++ trampoline. Up to 4 arguments supported in v1.
        db |> register_function("damage_formula", @@damage_formula, true)
        db |> register_function("short_hash",     @@short_hash,     true)
        db |> register_function("euclid",         @@euclid,         true)

        // --- Use it inside a SELECT projection -----------------------------
        // A 100-base attack with a 1.5× multiplier, factored against each
        // enemy's armor. The expression embeds the registered fn just like
        // any built-in SQLite scalar. For per-row aggregates we drop to
        // `try_run_select` with an explicit row block so single-column
        // results can land in `array<float>`.
        let dmg <- (db |> try_run_select(
            "SELECT damage_formula(100.0, Armor, 1.5) FROM Enemies ORDER BY Id",
            $(var stmt : sqlite3_stmt?) {},
            $(stmt : sqlite3_stmt?) : float {
                return float(sqlite3_column_double(stmt, 0))
            })) |> unwrap
        for (i in range(dmg |> length)) {
            to_log(LOG_INFO, "enemy #{i + 1} -> dmg={dmg[i]}\n")
        }

        // --- Use it in a WHERE clause --------------------------------------
        // A custom predicate works the same way. Determinism lets SQLite
        // apply normal CSE/optimizer treatment.
        let weak_count : int = db |> query_scalar(
            "SELECT COUNT(*) FROM Enemies WHERE damage_formula(100.0, Armor, 1.5) > 50",
            type<int>)
        to_log(LOG_INFO, "enemies that take >50 damage: {weak_count}\n")

        // --- Use it in ORDER BY -------------------------------------------
        // String → int64 fn used as the sort key. The first row after
        // sorting:
        let first_by_hash = db |> query_scalar(
            "SELECT Name FROM Enemies ORDER BY short_hash(Name) LIMIT 1",
            type<string>)
        to_log(LOG_INFO, "first enemy by short_hash: {first_by_hash}\n")

        // --- Multi-arg numeric fn -----------------------------------------
        let total = db |> query_scalar(
            "SELECT SUM(euclid(StartX, StartY, EndX, EndY)) FROM Trips",
            type<double>)
        to_log(LOG_INFO, "total trip distance: {total}\n")

        // --- Optional flags: deterministic, directonly --------------------
        // - deterministic=true (default false): same inputs always yield
        //   the same output — required to use the fn inside `CREATE
        //   INDEX … ON tbl(myfn(col))` or in a generated column.
        // - directonly=true: the fn cannot be called from triggers, views,
        //   or `CHECK(...)` constraints — used to lock down side-effecting
        //   helpers that should run only from application SQL.
        //
        //     db |> register_function("now_iso", @@iso_now, false, true)
        //
        // The arity is positional: `register_function(db, name, fn,
        // deterministic, directonly)`. Pipe form just reorders db.
    }
}
