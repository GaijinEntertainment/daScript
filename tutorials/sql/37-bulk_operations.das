options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 37 — bulk operations: making large writes fast.
//
// Concept-only tutorial — no new API. SQLite's per-statement overhead
// is small but not free; per-row autocommit (the default outside any
// explicit transaction) hits the WAL with `fdatasync` on every
// commit. Three tools, in order of how often you reach for them:
//
//   1. transactions          — group N inserts into one fdatasync
//   2. insert(array<T>)      — typed batch of homogeneous rows
//   3. INSERT…SELECT         — copy rows DB-side without round-tripping
//
// Order matters: WAL fsync dominates per-row latency at small batch
// sizes, then statement preparation, then column binding.

[sql_table(name = "Events")]
struct Event {
    @sql_primary_key Id : int
    Kind                : string
    PayloadBytes        : int
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> apply_recommended_pragmas()
        db |> create_table(type<Event>)

        // --- 1. Transactions: amortize commit cost --- nolint:STYLE014
        // The default autocommit makes every INSERT a separate transaction
        // → one fsync per row. Wrap a batch in `with_transaction` and the
        // whole batch is one fsync. ~50× speedup on cold WAL is typical.
        db |> with_transaction() {
            for (i in range(1000)) {
                db |> insert(Event(Id = i + 1, Kind = "tick", PayloadBytes = 32))
            }
        }
        to_log(LOG_INFO, "1000 rows inserted in one transaction")

        // --- 2. insert(array<T>): typed batch ----------------------------
        // Same prepared statement, reused across rows. Prefer over a manual
        // loop when the input is already an array.
        var events : array<Event>
        for (i in range(500)) {
            events |> emplace(Event(Id = 2000 + i, Kind = "burst", PayloadBytes = 16))
        }
        let inserted = db |> insert(events)
        to_log(LOG_INFO, "insert(array<Event>) wrote {inserted} rows")

        // --- 3. INSERT … SELECT: server-side copy --- nolint:STYLE014
        // Filter+copy that never round-trips the rows to daslang.
        // Useful for archive flows: copy yesterday's events out, delete
        // them in the same transaction.
        db |> exec(
            "INSERT INTO \"Events\" (\"Id\", \"Kind\", \"PayloadBytes\")
             SELECT \"Id\" + 100000, 'archived', \"PayloadBytes\"
             FROM \"Events\" WHERE \"Kind\" = 'tick'")
        let n_archived = _sql(db |> select_from(type<Event>)
                                |> _where(_.Kind == "archived")
                                |> count)
        to_log(LOG_INFO, "INSERT…SELECT archived {n_archived} rows")

        // --- 4. Combine: transaction + bulk for the absolute fastest -----
        // Transaction wraps the whole flow; `insert(array<T>)` reuses one
        // prepared statement; INSERT…SELECT is a single statement. Mix freely.
        db |> with_transaction() {
            db |> exec("DELETE FROM \"Events\" WHERE \"Kind\" = 'archived'")
            db |> exec("DELETE FROM \"Events\" WHERE \"Kind\" = 'tick'")
        }
    }
}

// --- WHEN TO REACH FOR WHICH --- nolint:STYLE014
//
//   Source data lives in an `array<T>` already?       insert(array<T>)
//   Mixed kinds / row-by-row decisions inside a batch? transaction { for ... insert(...) }
//   Filter-and-copy DB-side?                           exec("INSERT … SELECT …")
//   Single row?                                        plain insert(row) — autocommit is fine
//
// SQLite's hard ceiling is ~1M rows/sec on commodity SSD with WAL +
// transaction-batched insert(array<T>). Beyond that you're either CPU-bound
// in the daslang side or paying for indexes — drop indexes before
// bulk-load, recreate after, then VACUUM if free-space matters.
