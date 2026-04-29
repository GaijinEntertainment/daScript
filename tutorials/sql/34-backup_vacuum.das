options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 34 — VACUUM, OPTIMIZE, integrity check, backup.
//
// This tutorial covers the maintenance / hygiene side of SQLite:
//
//   vacuum            — defragment the file, reclaim deleted-row space
//   vacuum_into       — write a snapshot of the live DB to a new file
//   optimize          — refresh the query planner's statistics
//   integrity_check   — full corruption scan ("ok" or list of issues)
//   quick_check       — cheaper corruption scan, skips index/table cross
//   backup_to         — copy the live DB to another connection or file
//
// Each helper has a strict variant (panics on failure) and a `try_*`
// variant returning `SqlError` / `Result<...>`.

[sql_table(name = "Logs")]
struct LogEntry {
    @sql_primary_key Id : int
    Severity : int
    Message  : string
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<LogEntry>)
        for (i in range(100)) {
            db |> insert(LogEntry(
                Id = i + 1,
                Severity = (i % 4),
                Message = "msg #{i}"))
        }

        // --- VACUUM ---------------------------------------------------
        // Rewrites the file from scratch. After large DELETE workloads
        // this reclaims free pages and packs surviving rows densely.
        // Cannot run inside an active transaction — the helper rejects
        // that case before reaching SQLite.
        db |> exec("DELETE FROM Logs WHERE Severity = 0")
        db |> vacuum()
        let n = db |> query_scalar("SELECT COUNT(*) FROM Logs", type<int>)
        to_log(LOG_INFO, "after VACUUM: {n} rows remain\n")

        // --- VACUUM in a transaction returns Err ---------------------
        // The strict form would panic; the try form surfaces the error
        // so the caller can recover without crashing.
        db |> with_transaction() {
            let err = db |> try_vacuum()
            if (err |> is_some) {
                to_log(LOG_INFO, "try_vacuum mid-tx: {err |> unwrap}\n")
            }
        }

        // --- VACUUM INTO (snapshot to a file) ------------------------
        // VACUUM INTO '<path>' writes a one-off copy of the live DB to
        // a new file without touching the source. The path is single-
        // quoted with `''` doubling — SQLite doesn't accept `?` binds
        // for VACUUM. Useful when you want a defragmented snapshot;
        // for a hot copy of the live DB without re-packing, use
        // `backup_to` (below) instead.
        //
        //     db |> vacuum_into("/tmp/snapshot.sqlite")
        //     db |> vacuum_into("with'apostrophe.sqlite")  // quotes are escaped

        // --- backup_to (online hot backup) ---------------------------
        // The Backup API copies a live database into another open
        // connection or to a path on disk. The destination receives the
        // full state of the source; transient SQLITE_BUSY/LOCKED
        // responses (concurrent writers on the source) are retried
        // automatically with a 50 ms backoff. Two overloads:
        //
        //     // Snapshot into another runner (e.g. an in-memory clone):
        //     with_sqlite(":memory:") $(snapshot) {
        //         db |> backup_to(snapshot)
        //         // ... query `snapshot` to inspect a frozen view ...
        //     }
        //
        //     // Snapshot directly to disk (file is opened, populated,
        //     // and closed for you):
        //     db |> backup_to("/tmp/hot_snapshot.sqlite")
        //
        // The strict form panics on any non-completion; the `try_`
        // form returns SqlError. backup_to replaces the destination
        // wholesale — pre-existing tables on the destination are gone
        // after the call.

        // --- OPTIMIZE -------------------------------------------------
        // `PRAGMA optimize` updates query-planner stats. Cheap; recommended
        // at connection close on long-lived connections, no-op when nothing
        // has changed.
        db |> optimize()

        // --- integrity_check ----------------------------------------
        // Walks every page checking for corruption. Returns ["ok"] on a
        // healthy database; on corruption returns one row per detected
        // issue (truncated file, missing index entry, etc.). The helper
        // returns the rows verbatim — the caller decides how to react.
        let issues <- db |> integrity_check()
        if (length(issues) == 1 && issues[0] == "ok") {
            to_log(LOG_INFO, "integrity_check: db is healthy\n")
        } else {
            to_log(LOG_ERROR, "integrity_check: {length(issues)} issue(s):\n")
            for (msg in issues) {
                to_log(LOG_ERROR, "  - {msg}\n")
            }
        }

        // --- quick_check --------------------------------------------
        // Cheaper sibling; same return shape, but skips the per-index
        // cross-table walk. Useful as a fast pre-flight before deciding
        // whether to run the expensive integrity_check.
        let quick <- db |> quick_check()
        to_log(LOG_INFO, "quick_check returned {length(quick)} row(s); first: {quick[0]}\n")
    }
}

// --- DEFERRED ---------------------------------------------------------
//
//   1. Backup progress callback (`sqlite3_backup_step(bp, N)` returning
//      remaining/pagecount, surfaced to a daslang block). Useful for
//      long-running backups under a "saving…" progress bar.
//
//   2. integrity_check / quick_check first-N rows mode. SQLite's
//      pragma accepts a row-cap argument (`PRAGMA integrity_check(10)`
//      stops after 10 issues). The current helper always runs the full
//      scan; add an optional N parameter once a real consumer asks.
