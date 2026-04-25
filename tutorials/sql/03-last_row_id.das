options gen2

require daslib/sql
require sqlite/sqlite_boost

// `@sql_primary_key Id : int` maps to SQLite's INTEGER PRIMARY KEY — which is
// the ROWID alias. When you construct `Friend(Name="Tom")` the Id field
// defaults to 0; the [sql_table]-generated `_sql_pk_is_unset` notices and
// the `insert(...)` overload picks the no-PK INSERT form
// (`INSERT INTO "Friends" ("Name") VALUES (?)`), letting SQLite assign the
// rowid for you. The returned int64 is `sqlite3_last_insert_rowid` — also
// available directly via `db |> last_insert_rowid()` for ad-hoc queries.
[sql_table(name="Friends")]
struct Friend {
    @sql_primary_key Id : int
    Name : string
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let last_id = db |> insert([
            Friend(Name = "Tom"),
            Friend(Name = "Rebecca"),
            Friend(Name = "Jim"),
            Friend(Name = "Roger"),
            Friend(Name = "Robert")
        ])
        to_log(LOG_INFO, "The last Id of the inserted row is {last_id}\n")
    }
}
