options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 35 — _each_sql: streaming iteration over a SELECT.
//
// `_sql(...)` materializes the result set as `array<T>` up front. Fine for
// hundreds of rows, problematic for millions: every row sits in memory at
// the same time, and the first user-visible row only arrives after the
// whole array is built.
//
// `_each_sql(...)` is the streaming variant. Same chain shape (any
// `select_from + _where + _select + _order_by + _join`-style query that
// `_sql` accepts), but instead of returning `array<T>`, it returns
// `iterator<T>` — rows are produced lazily, one per `sqlite3_step`.
//
// Lifecycle (handled for you):
//   prepare → bind → step+yield → step+yield → ... → finalize
//
// The underlying prepared statement is finalized in the generator's
// `finally` block, which runs on:
//   - normal exhaustion (loop runs to completion)
//   - early `break` from the for-loop
//   - panic from inside the loop body
// In every case, the statement is released; subsequent writes to the
// same DB are NOT blocked by a stale read transaction.

[sql_table(name = "Logs")]
struct LogEntry {
    @sql_primary_key Id : int
    Severity : int       // 0 = trace, 1 = info, 2 = warn, 3 = error
    Message  : string
}

def fixture(db : SqlRunner) {
    db |> create_table(type<LogEntry>)
    for (i in range(20)) {
        db |> insert(LogEntry(
            Id = i + 1,
            Severity = (i % 4),
            Message = "msg #{i}"))
    }
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // --- Streaming iteration --------------------------------------
        // `for (row in _each_sql(...))` lazily materializes one row at a
        // time. The iterator's lifetime is the for-loop body; the
        // statement is finalized when the loop exits.
        var n = 0
        for (e in _each_sql(db |> select_from(type<LogEntry>) |> _where(_.Severity >= 2))) {
            n++
            to_log(LOG_INFO, "  [{e.Severity}] #{e.Id} — {e.Message}\n")
        }
        to_log(LOG_INFO, "streamed {n} severe logs\n")

        // --- Early break ---------------------------------------------
        // Process rows until a condition is met, then `break`. The
        // generator's `finally` finalizes the prepared statement, so a
        // subsequent INSERT on the same DB succeeds without contention.
        var first_error_id = 0
        for (e in _each_sql(db |> select_from(type<LogEntry>))) {
            if (e.Severity == 3) {
                first_error_id = e.Id
                break
            }
        }
        to_log(LOG_INFO, "first error has id {first_error_id}\n")
        // The INSERT below would block (or panic) if the iterator's
        // statement leaked. It succeeds → finally ran.
        db |> insert(LogEntry(Id = 100, Severity = 0, Message = "after break"))

        // --- Single-column projection in a stream --------------------
        // The iterator type follows the projection — `_select(_.Field)`
        // streams scalars, named-tuple `_select((A=..., B=...))` streams
        // tuples, FullRow streams structs.
        var total_chars = 0
        for (msg in _each_sql(db |> select_from(type<LogEntry>) |> _select(_.Message))) {
            total_chars += length(msg)
        }
        to_log(LOG_INFO, "total Message bytes streamed: {total_chars}\n")

        // --- Captured local in the predicate -------------------------
        // Unlike `_create_view`, `_each_sql` accepts captured locals in
        // the chain — they bind to the prepared statement at run time
        // (the standard `?` placeholder mechanism).
        let cutoff = 2
        for (e in _each_sql(db |> select_from(type<LogEntry>) |> _where(_.Severity >= cutoff))) {
            to_log(LOG_INFO, "  bound-cutoff: id={e.Id}\n")
        }
    }
}

// --- One-shot semantics ----------------------------------------------
//
// A daslang generator is consumed once. To re-run the same query, write
// the `_each_sql(...)` call site again — each evaluation builds a fresh
// prepared statement. Don't try to "rewind" an iterator; it has no
// rewind operation, and capturing the iterator into a variable then
// iterating twice will fail (the second pass yields zero rows).

// --- When to pick _each_sql vs _sql ----------------------------------
//
// Pick `_each_sql` when:
//   - The result set is large (millions of rows) and per-row processing
//     is cheap.
//   - You break out early on a condition (e.g. "find first matching
//     row" or "process until budget exhausted").
//   - You're piping rows into another iterator-shaped pipeline (linq /
//     algorithm functions that take an iterator).
//
// Pick `_sql` when:
//   - The result set is small (hundreds of rows) — array materialization
//     overhead is negligible.
//   - You want to index into the result, count rows up front, or pass
//     the array to a function that expects `array<T>`.
//   - The downstream code mutates the same DB while iterating; with
//     `_each_sql` the iterator holds a read transaction (under WAL) or
//     a shared lock (default) that may interact poorly with concurrent
//     writes.

// --- Concurrent-writer note + WAL mitigation -------------------------
//
// While the iterator is stepping, SQLite holds a read transaction on
// the connection. Default journal mode (`DELETE`) blocks writes from
// the same connection until the cursor finalizes. If you need to write
// to the DB while iterating from the SAME connection, switch to
// write-ahead logging:
//
//     db |> set_pragma("journal_mode", "WAL")
//
// Under WAL, readers and one writer can coexist on the same connection,
// at the cost of an extra `-wal` / `-shm` file alongside the database.
// See tut 33 for the pragma surface.

// --- Restrictions vs _sql --------------------------------------------
//
// `_each_sql` rejects the following terminals at compile time, with a
// clear pointer to `_sql(...)` as the alternative:
//
//   _to_array       — `_each_sql` already streams rows; `_to_array` would
//                     force materialization, which is what `_sql(...)` is for
//   _first          — single-row materializer
//   _first_opt      — single-row Option<T> materializer
//   _count / _sum / _avg / _min / _max — aggregates (one scalar row)
//
// For all of these, use `_sql(...)` — the macro emits the right
// runtime to materialize one scalar / row / array.
