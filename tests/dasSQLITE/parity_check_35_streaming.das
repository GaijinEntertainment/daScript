options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 35 — `_each_sql` streaming iteration.
//
// Same chain shape as `_sql(...)`, but the terminal is `iterator<T>`
// instead of `array<T>`. The parity claim is that streaming and
// materialization produce IDENTICAL row sequences:
//
//   M1a (materialized) : var rows <- _sql(chain); iterate rows
//   M1b (streamed)     : for (r in _each_sql(chain)) { collect }
//   M2                 : for (r in (db |> chain))   { collect }
//   M3                 : for (r in (arr |> chain))  { collect }
//
// The fourth shape (M3) is plain-array iteration — no SQL, no streaming
// machinery. M1/M2/M3 row order matches the chain's `_order_by`. M1a vs
// M1b is the interesting comparison: does `_each_sql` match `_sql`
// row-for-row at the same chain.

[sql_table(name = "Logs")]
struct LogEntry {
    @sql_primary_key Id : int
    Severity : int
    Message  : string
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<LogEntry>)
    for (i in range(20)) {
        db |> insert(LogEntry(
            Id = i + 1,
            Severity = (i % 4),
            Message = "msg #{i}"))
    }
}

def fixture_array() : array<LogEntry> {
    var arr : array<LogEntry>
    for (i in range(20)) {
        arr |> emplace(LogEntry(
            Id = i + 1,
            Severity = (i % 4),
            Message = "msg #{i}"))
    }
    return <- arr
}

def log_eq(a : LogEntry; b : LogEntry) : bool {
    return (a.Id == b.Id
        && a.Severity == b.Severity
        && a.Message == b.Message)
}

def logs_eq(a : array<LogEntry>; b : array<LogEntry>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!log_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

[test]
def parity_each_sql_vs_materialized(t : T?) {
    // Same `_where + _order_by` chain, four delivery shapes. Streaming
    // (M1b) must match the materialized variant (M1a) row-for-row.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1a <- _sql(db |> select_from(type<LogEntry>) |> _where(_.Severity >= 2) |> _order_by(_.Id))

        var m1b : array<LogEntry>
        for (e in _each_sql(db |> select_from(type<LogEntry>) |> _where(_.Severity >= 2) |> _order_by(_.Id))) {
            m1b |> push(e)
        }

        var m2 : array<LogEntry>
        for (e in (db |> select_from(type<LogEntry>) |> _where(_.Severity >= 2) |> _order_by(_.Id))) {
            m2 |> push(e)
        }

        var m3 : array<LogEntry>
        for (e in (arr |> _where(_.Severity >= 2) |> _order_by(_.Id))) {
            m3 |> push(e)
        }

        t |> equal(length(m1a), 10, "stream: 10 severe logs")
        t |> success(logs_eq(m1a, m1b), "stream: M1a (materialized) ≡ M1b (streamed)")
        t |> success(logs_eq(m1b, m2),  "stream: M1b ≡ M2")
        t |> success(logs_eq(m2, m3),   "stream: M2 ≡ M3")
    }
}

[test]
def parity_each_sql_early_break(t : T?) {
    // Iterate until predicate, then break. The first matching row's Id
    // must be the same across modes.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var first_id_m1 = 0
        for (e in _each_sql(db |> select_from(type<LogEntry>) |> _order_by(_.Id))) {
            if (e.Severity == 3) {
                first_id_m1 = e.Id
                break
            }
        }

        var first_id_m2 = 0
        for (e in (db |> select_from(type<LogEntry>) |> _order_by(_.Id))) {
            if (e.Severity == 3) {
                first_id_m2 = e.Id
                break
            }
        }

        var first_id_m3 = 0
        for (e in (arr |> _order_by(_.Id))) {
            if (e.Severity == 3) {
                first_id_m3 = e.Id
                break
            }
        }

        t |> equal(first_id_m1, 4, "early break: first severity=3 is Id=4")
        t |> equal(first_id_m1, first_id_m2, "early break: M1 ≡ M2")
        t |> equal(first_id_m2, first_id_m3, "early break: M2 ≡ M3")

        // Sanity that the iterator's finally ran — a subsequent INSERT must succeed.
        db |> insert(LogEntry(Id = 100, Severity = 0, Message = "after break"))
        let n = db |> query_scalar("SELECT COUNT(*) FROM Logs", type<int>)
        t |> equal(n, 21, "after break: insert succeeded (finally ran)")
    }
}

[test]
def parity_each_sql_scalar_projection(t : T?) {
    // _select(_.Message) — streaming a single-column projection.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 : array<string>
        for (s in _each_sql(db |> select_from(type<LogEntry>) |> _order_by(_.Id) |> _select(_.Message))) {
            m1 |> push(s)
        }

        var m2 : array<string>
        for (s in (db |> select_from(type<LogEntry>) |> _order_by(_.Id) |> _select(_.Message))) {
            m2 |> push(s)
        }

        var m3 : array<string>
        for (s in (arr |> _order_by(_.Id) |> _select(_.Message))) {
            m3 |> push(s)
        }

        t |> equal(length(m1), 20, "scalar stream: 20 messages")
        t |> equal(length(m1), length(m2), "scalar stream: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "scalar stream: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i], m2[i], "msg[{i}]: M1 ≡ M2")
            t |> equal(m2[i], m3[i], "msg[{i}]: M2 ≡ M3")
        }
    }
}
