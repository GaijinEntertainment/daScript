options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 28 — JSON and BLOB columns: structured-but-not-columnar data.
//
// Two complementary annotations for shapes that don't belong in their own
// columns:
//
//   @sql_json : TEXT storage via daslib/json + daslib/json_boost. QUERYABLE
//               from SQL through `json_extract`, and the `_sql` walker
//               descends into JSON paths (pred-side and projection-side).
//               Human-readable in the sqlite3 shell. Tolerant of schema
//               drift — missing fields default, extras ignored.
//
//   @sql_blob : BLOB storage via daslib/archive. OPAQUE to SQL (no walker
//               descent), but faster encode/decode and smaller on disk.
//               Schema-FRAGILE — positional binary format; adding a field
//               breaks old rows.
//
// Both ride the tut-26 adapter rail: when [sql_table] sees the annotation
// on a struct/tuple-typed field, it generates the matching
// `sql_bind`/`sql_extract` pair at module scope. No new storage class, no
// registration table, no per-type lookup — the adapter pair IS the
// registration.
//
// EXPLICIT OPT-IN. Without an annotation, a struct/tuple-typed column is
// still a compile error from tut 26 — "is this column queryable JSON or
// opaque binary?" is a load-bearing decision (schema-drift story + SQL
// queryability), so daslang refuses to guess.
//
// LINQ DESCENT KEYS ON THE ANNOTATION. The walker reads `@sql_json` off
// the field, not off the type. A user who writes a custom string-returning
// `sql_bind(MyStruct)` (CSV, protobuf-text, whatever) gets TEXT storage but
// NOT walker descent — they use raw SQL for any structure-aware query.

// --- @sql_json: TEXT-backed, queryable -----------------------------------
// Prefs is a tuple; @sql_json makes [sql_table] emit, at module scope:
//
//     def sql_bind    (v : tuple<...>) : string {
//         return write_json(JV(v))
//     }
//     def sql_extract (v : string; t : type<tuple<...>>) : tuple<...> {
//         var err = ""
//         let jv = read_json(v, err)
//         if (err != "") {
//             panic("sql_extract: malformed JSON in column data: {err}")
//         }
//         return from_JV(jv, default<tuple<...>>)
//     }
//
// Storage is TEXT. The column carries the @sql_json marker so `_sql` can
// descend into JSON paths without confusing them with daslang field access.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name                : string
    @sql_json
    Prefs : tuple<theme : string; lang : string; notify : bool>
}

// --- @sql_blob: opaque binary archive -----------------------------------
// SessionSnapshot is a struct with arrays + tables. @sql_blob generates
// the daslib/archive variant of the adapter pair (const param + local
// `clone_to_move` because the catch-all binder passes `v` non-`var`,
// while `mem_archive_save` / `mem_archive_load` need a mutable reference):
//
//     def sql_bind    (v : T) : array<uint8> {
//         var v_local <- clone_to_move(v)
//         return <- mem_archive_save(v_local)
//     }
//     def sql_extract (v : array<uint8>; t : type<T>) : T {
//         var r = default<T>
//         var v_local <- clone_to_move(v)
//         mem_archive_load(v_local, r)   // panics on malformed bytes
//         return <- r
//     }
//
// Storage is BLOB. The column is opaque to `_sql` — descending into a
// @sql_blob column is a compile error (filter in daslang after round-trip
// instead, or use raw SQL for one-off introspection).

struct SessionSnapshot {
    last_scene : string
    unlocks : array<int>
    last_tick_ms : int64
}

[sql_table(name = "Sessions")]
struct SessionRow {
    @sql_primary_key UserId : int
    StartedAt : int64
    @sql_blob State : SessionSnapshot
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<User>)
        db |> create_table(type<SessionRow>)

        // Inspect emitted DDL — @sql_json picks TEXT, @sql_blob picks BLOB,
        // bypassing the normal `sql_storage_type_for(type<T>)` dispatch
        // (which would fail for tuple<...> / struct anyway).
        to_log(LOG_INFO, "Users DDL:    {_sql_create_table_sql(type<User>)}\n")
        to_log(LOG_INFO, "Sessions DDL: {_sql_create_table_sql(type<SessionRow>)}\n")

        // INSERT — the [sql_table] macro emits sqlite3_bind_text with the
        // serialized JSON for the @sql_json column, sqlite3_bind_blob with
        // the archived bytes for the @sql_blob column.
        db |> insert(User(Id = 1, Name = "alice",
            Prefs = (theme = "dark",  lang = "en", notify = true)))
        db |> insert(User(Id = 2, Name = "bob",
            Prefs = (theme = "light", lang = "ru", notify = false)))
        db |> insert(User(Id = 3, Name = "carol",
            Prefs = (theme = "dark",  lang = "fr", notify = false)))

        var snap = SessionSnapshot(
            last_scene = "tutorial",
            unlocks <- [101, 202, 303],
            last_tick_ms = 4242l)
        db |> insert(SessionRow(UserId = 1, StartedAt = 1000l, State <- snap))

        // SELECT round-trips both adapters in reverse — TEXT goes through
        // read_json + from_JV for the JSON column, BLOB goes through
        // mem_archive_load for the archive column.
        for (u in db |> select_from(type<User>)) {
            to_log(LOG_INFO, "user {u.Id} {u.Name}: theme={u.Prefs.theme} notify={u.Prefs.notify}\n")
        }
        for (s in db |> select_from(type<SessionRow>)) {
            to_log(LOG_INFO, "session {s.UserId}: last_scene={s.State.last_scene} unlocks={length(s.State.unlocks)}\n")
        }

        // --- _sql walker — JSON-path descent on @sql_json columns -------
        //
        // `_.<json_col>.<path…>` on an @sql_json field lowers to
        //     json_extract("<col>", '$.<path…>')
        // both in WHERE clauses and in SELECT projections. The leaf type
        // (string / bool / int / …) drives result-side adapter dispatch.

        // Pred-side (WHERE) descent.
        let dark_users <- _sql(db |> select_from(type<User>)
            |> _where(_.Prefs.theme == "dark"))
        // SQL: SELECT Id, Name, Prefs FROM "Users"
        //      WHERE json_extract("Prefs", '$.theme') = ?
        to_log(LOG_INFO, "dark users: {length(dark_users)}\n")

        // Projection-side (SELECT) descent — string scalar.
        let themes <- _sql(db |> select_from(type<User>)
            |> _select(_.Prefs.theme))
        // SQL: SELECT json_extract("Prefs", '$.theme') FROM "Users"
        for (t in themes) {
            to_log(LOG_INFO, "theme: {t}\n")
        }

        // Mixed projection — plain column + JSON path.
        let mixed <- _sql(db |> select_from(type<User>)
            |> _select((Name = _.Name, Theme = _.Prefs.theme)))
        // SQL: SELECT "Name", json_extract("Prefs", '$.theme') FROM "Users"
        for (m in mixed) {
            to_log(LOG_INFO, "{m.Name}: {m.Theme}\n")
        }

        // Bool subfield — json_extract returns 0/1 for JSON booleans;
        // tut 26's bool adapter handles the int-to-bool flip on extract.
        let notifiable_count = _sql(db |> select_from(type<User>)
            |> _where(_.Prefs.notify == true)
            |> count())
        // SQL: SELECT COUNT(*) FROM "Users"
        //      WHERE json_extract("Prefs", '$.notify') = ?
        to_log(LOG_INFO, "notifiable users: {notifiable_count}\n")

        // --- Trying to descend into a @sql_blob column would fail at
        // compile time:
        //
        //     _where(_.State.last_scene == "tutorial")
        //
        // emits "cannot descend into @sql_blob column State" — filter in
        // daslang after round-trip, or use raw `query` SQL for one-offs.
    }
}

// --- Nested struct paths inside @sql_json -------------------------------
// Walker descent is arbitrary depth. Field-by-field type lookup walks
// struct fields / tuple argNames+argTypes to find the leaf scalar type
// used for binding the comparison value or shaping the result column.

struct Address {
    City : string
    Zip  : string
}
struct Profile {
    Addr : Address
    Bio  : string
}

[sql_table(name = "Accounts")]
struct Account {
    @sql_primary_key Id : int
    @sql_json Profile   : Profile
}

def in_city(db : SqlRunner; city : string) : array<Account> {
    return <- _sql(db |> select_from(type<Account>)
        |> _where(_.Profile.Addr.City == city))
    // SQL: SELECT Id, Profile FROM "Accounts"
    //      WHERE json_extract("Profile", '$.Addr.City') = ?
}

// --- Choosing @sql_json vs @sql_blob ------------------------------------
//
// Feature                       | @sql_json        | @sql_blob
// ------------------------------+------------------+-------------------
// Storage class                 | TEXT             | BLOB
// `_sql` field descent          | json_extract     | compile error
// Queryable from SQL            | yes              | no
// Encode/decode speed           | slower           | faster
// On-disk size                  | larger           | smaller
// Human-readable (sqlite3 CLI)  | yes              | no
// Type breadth                  | daslib/json_boost| daslib/archive
//                               |                  | (also smart_ptr,
//                               |                  |  variant, matrix)
// Schema drift tolerance        | HIGH — missing   | LOW — positional;
//                               | fields default,  | adding/removing a
//                               | extras ignored   | field breaks rows
//
// Rule of thumb:
//   - Shape will evolve                     → @sql_json.
//   - Shape is pinned / versioned schema    → @sql_blob fine.
//   - Need SQL-side filter inside the col   → @sql_json (only option).
//
// Both forms panic on bad stored data — malformed JSON trips
// `read_json` (the generated `sql_extract` checks the parser error
// string and panics), corrupt archive bytes trip `mem_archive_load`.
// `try_select_from` / `try_query` only catch sqlite3-level prepare /
// step errors; they do NOT convert adapter panics to `Err`. Defensive
// reads should validate JSON shape externally (a CHECK constraint, a
// `json_valid` filter in the SELECT) before the row reaches daslang.

// --- DEFERRED -----------------------------------------------------------
//
//   1. @sql_json_index("$.path") — auto-emitted SQLite expression index
//      on a JSON path. Cross-provider DDL diverges (PG, MySQL); raw
//      `CREATE INDEX` until a second backend lands.
//   2. _json_extract<T>(col, path) — explicit escape hatch for paths the
//      walker can't reach (array subscripts, custom JSON operators).
//   3. @sql_json(fast) — switches the generated body to
//      `json_sprint`/`json_sscan` for the limited-type fast path.
//   4. JSON patch / merge operators (`json_set` / `json_patch`) inside
//      `_update` chains. Raw `exec` covers v1.
//   5. JSONB binary storage (SQLite 3.45+) via `@sql_json(binary)`.
//   6. @sql_blob compression (zstd / lz4) and versioned-format headers.
//   7. Cross-provider JSON-path emitter (PG `->>`, MSSQL `JSON_VALUE`).
//      v1 emits SQLite `json_extract` syntax; the walker rule is
//      universal, the per-provider spelling is the swap-out point.
