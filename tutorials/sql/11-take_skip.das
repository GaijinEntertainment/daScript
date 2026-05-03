options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 11 — `take(n)` / `skip(n)`: LIMIT and OFFSET pagination.
//
// `take(n)` emits `LIMIT ?`. `skip(n)` emits `OFFSET ?`; on its own
// (without `take`), SQLite requires `LIMIT -1 OFFSET ?` (LIMIT-of-
// negative-one means no limit), and the macro emits exactly that.
//
// Chain order matters. `_sql` honors linq's positional semantics:
// `skip(m) |> take(n)` is the canonical pagination shape (skip first,
// then take) and lowers to flat `LIMIT n OFFSET m`. The reverse,
// `take(n) |> skip(m)`, means "take n, then drop m" — lowered to a
// nested SELECT to preserve that meaning.
//
// Bind ordering: LIMIT and OFFSET binds always go *after* WHERE
// binds in the placeholder index sequence, so a `_where(...) |>
// take(n)` chain produces `WHERE col > ? LIMIT ?` with binds in
// `[wherevalue, n]` order — no surprises.
//
// Single-row terminals (`_first` / `_first_opt`) override `take`'s
// LIMIT and emit `LIMIT 1` instead.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ── take(n) ──────────────────────────────────────────────────────
        let first_two <- _sql(db |> select_from(type<Car>) |> take(2))
        to_log(LOG_INFO, "take(2) — first two rows:\n")
        for (c in first_two) {
            to_log(LOG_INFO, "  {c.Name}\n")
        }

        // ── skip(n) — needs LIMIT -1 in SQLite without a take ────────────
        let skip_three <- _sql(db |> select_from(type<Car>) |> skip(3))
        to_log(LOG_INFO, "skip(3) — drop the first three:\n")
        for (c in skip_three) {
            to_log(LOG_INFO, "  {c.Name}\n")
        }

        // ── skip + take — canonical pagination window ────────────────────
        // `skip(m) |> take(n)` = "skip m rows, then take n" — page k of size n
        // = `skip((k-1)*n) |> take(n)`. So `skip(2) |> take(2)` returns rows
        // 3..4 (page 2 of size 2). Lowers to a flat `LIMIT n OFFSET m`.
        let window <- _sql(db |> select_from(type<Car>) |> skip(2) |> take(2))
        to_log(LOG_INFO, "skip(2) take(2) — rows 3-4:\n")
        for (c in window) {
            to_log(LOG_INFO, "  {c.Name}\n")
        }

        // ── take with WHERE — placeholder ordering ───────────────────────
        // Canonical: `_where(...) |> take(n)` = filter then take; lowers
        // flat to `WHERE Price > ? LIMIT ?` with binds `[threshold, 2]`.
        let threshold = 100
        let cheap_two <- _sql(db |> select_from(type<Car>)
                                |> _where(_.Price > threshold)
                                |> take(2))
        to_log(LOG_INFO, "WHERE Price>100 LIMIT 2:\n")
        for (c in cheap_two) {
            to_log(LOG_INFO, "  {c.Name}: {c.Price}\n")
        }

        // ── Pair with order_by for stable pagination ─────────────────────
        // LIMIT/OFFSET without ORDER BY isn't deterministic — SQLite can
        // return rows in any order. Always combine the two when you care
        // about page boundaries.
        let stable_page <- _sql(db |> select_from(type<Car>)
                                  |> _order_by(_.Price)
                                  |> skip(1) |> take(2))
        to_log(LOG_INFO, "ORDER BY Price ASC LIMIT 2 OFFSET 1:\n")
        for (c in stable_page) {
            to_log(LOG_INFO, "  {c.Name}: {c.Price}\n")
        }

        // ── Single-row terminals override take's LIMIT ───────────────────
        // `_first` always emits `LIMIT 1`, ignoring any earlier `take`.
        let head = _sql(db |> select_from(type<Car>) |> take(5) |> _first())
        to_log(LOG_INFO, "_first overrides take's LIMIT: {head.Name}\n")

        // ── Inspect emitted SQL ──────────────────────────────────────────
        let sql = _sql_text(db |> select_from(type<Car>)
                              |> _order_by(_.Price)
                              |> skip(1) |> take(2))
        to_log(LOG_INFO, "  emitted SQL: {sql}\n")
    }
}
