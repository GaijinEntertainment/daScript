options gen2

require daslib/sql
require daslib/option
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 18 — NULL handling: `Option<T>` everywhere.
//
// Nullability is type-driven. The struct field's daslang type is the
// single source of truth — there is no `@sql_nullable` annotation.
//
//   - `T` field          → column declared `NOT NULL`
//   - `Option<T>` field  → column allows `NULL`
//
// The `[sql_table]` macro inspects each field at compile time:
//
//   - DDL: emits `NOT NULL` for `T`, omits it for `Option<T>`
//   - INSERT/UPDATE bind:
//       `is_some(field)` → bind the underlying value
//       `is_none()`      → bind SQL NULL
//   - SELECT reader: checks `sqlite3_column_type == SQLITE_NULL` and
//     wraps in `some(v)` / `none()` for `Option<T>` fields
//
// Inside `_sql(...)` predicates, three Option methods translate to
// SQL null-comparison operators:
//
//   `_.Col |> is_some`        →  "Col" IS NOT NULL
//   `_.Col |> is_none`        →  "Col" IS NULL
//   `_.Col |> unwrap_or(d)`   →  COALESCE("Col", ?)   (d binds)
//
// Three-valued-logic gotcha: in SQL, `NULL = NULL` is NULL, never
// TRUE. The Option API steers users away from that footgun — write
// `_.Age |> is_none()` or `_.Age |> unwrap_or(0) >= 18`, never
// `_.Age == 0` for "missing or zero".

// `Option<T>` fields need `@safe_when_uninitialized` so the
// `[sql_table]` reader can build a default-initialized row without
// tripping `strict_smart_pointers`. The Option's internal `_value`
// is already so-marked inside the template; the wrapping struct
// field still needs its own annotation under strict mode.
[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name      : string
    @safe_when_uninitialized Age       : Option<int>
    @safe_when_uninitialized Nick      : Option<string>
    @safe_when_uninitialized DeletedAt : Option<int64>
}

def fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    // alice: known age, no nick, alive
    db |> insert(User(
        Id = 1, Name = "alice",
        Age = some(30), Nick = none(type<string>), DeletedAt = none(type<int64>)))
    // bob: unknown age, has nick, alive
    db |> insert(User(
        Id = 2, Name = "bob",
        Age = none(type<int>), Nick = some("bobby"), DeletedAt = none(type<int64>)))
    // carol: known age, has nick, soft-deleted
    db |> insert(User(
        Id = 3, Name = "carol",
        Age = some(40), Nick = some("c"), DeletedAt = some(1000l)))
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ── (1) Type-driven DDL ──────────────────────────────────────────
        // The CREATE TABLE statement skips `NOT NULL` on Option<T>
        // columns and keeps it on plain T columns. Inspect the DDL
        // string the macro generates:
        let ddl = _::_sql_create_table_sql(type<User>)
        to_log(LOG_INFO, "DDL:\n  {ddl}\n")

        // ── (2) Round-trip — some(v) and none() both store + read back ──
        let everyone <- db |> select_from(type<User>)
        to_log(LOG_INFO, "All users:\n")
        for (u in everyone) {
            // Each Option<T> field is read back as the same Option<T>.
            // Pattern-match with is_some / is_none / unwrap.
            if (u.Age |> is_some) {
                to_log(LOG_INFO, "  {u.Name}: age {u.Age |> unwrap}\n")
            } else {
                to_log(LOG_INFO, "  {u.Name}: age unknown\n")
            }
        }

        // ── (3) `is_some` / `is_none` in `_where` ────────────────────────
        // SQL: WHERE "Age" IS NOT NULL
        let known_age <- _sql(db |> select_from(type<User>)
                                |> _where(_.Age |> is_some)
                                |> _order_by(_.Id))
        to_log(LOG_INFO, "Users with known age:\n")
        for (u in known_age) {
            to_log(LOG_INFO, "  {u.Name}\n")
        }

        // SQL: WHERE "DeletedAt" IS NULL
        let alive <- _sql(db |> select_from(type<User>)
                            |> _where(_.DeletedAt |> is_none)
                            |> _order_by(_.Id))
        to_log(LOG_INFO, "Alive users:\n")
        for (u in alive) {
            to_log(LOG_INFO, "  {u.Name}\n")
        }

        // ── (4) `unwrap_or(default)` lowers to COALESCE ──────────────────
        // SQL: WHERE COALESCE("Age", ?) >= ?
        // Binds: [0, 18]
        // bob has Age=none, so COALESCE returns 0; 0 >= 18 is false →
        // bob is filtered out. alice and carol pass.
        let cutoff = 18
        let adults_or_unknown <- _sql(db |> select_from(type<User>)
                                        |> _where(_.Age |> unwrap_or(0) >= cutoff)
                                        |> _order_by(_.Id))
        to_log(LOG_INFO, "Adults (treating unknown age as 0):\n")
        for (u in adults_or_unknown) {
            to_log(LOG_INFO, "  {u.Name}\n")
        }

        // ── (5) Option<T> carries through full-row projection ────────────
        // No implicit unwrap. The result element is the source struct,
        // so `Option<int>` stays `Option<int>`.
        let with_age <- _sql(db |> select_from(type<User>)
                               |> _where(_.Age |> is_some)
                               |> _order_by(_.Id))
        to_log(LOG_INFO, "Projected rows preserve Option:\n")
        for (u in with_age) {
            to_log(LOG_INFO, "  {u.Name}: Age is_some={u.Age |> is_some}\n")
        }

        // ── (6) `_try_sql` composes with Option predicates ───────────────
        let attempt <- _try_sql(db |> select_from(type<User>)
                                  |> _where(_.Age |> is_some))
        if (attempt |> is_ok) {
            let rows <- attempt |> unwrap
            to_log(LOG_INFO, "_try_sql returned {length(rows)} known-age users\n")
        } else {
            to_log(LOG_ERROR, "query failed: {attempt |> unwrap_err}\n")
        }

        // ── Inspect emitted SQL ──────────────────────────────────────────
        let sql_is_some = _sql_text(db |> select_from(type<User>)
                                      |> _where(_.Age |> is_some))
        let sql_coalesce = _sql_text(db |> select_from(type<User>)
                                       |> _where(_.Age |> unwrap_or(0) >= cutoff))
        to_log(LOG_INFO, "  is_some SQL:  {sql_is_some}\n")
        to_log(LOG_INFO, "  COALESCE SQL: {sql_coalesce}\n")
    }
}
