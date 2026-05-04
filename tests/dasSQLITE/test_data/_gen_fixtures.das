options gen2

// One-shot fixture builder for `[sql_table(schema_from=...)]` tests.
// Run via:
//   bin/Release/daslang tests/dasSQLITE/test_data/_gen_fixtures.das
// Commits the resulting .db files to the repo. The companion .sql files are
// documentation — they show what each .db was built from.

require sqlite/sqlite_boost
require fio_core

let DBS <- [
    "schema_from_v1.db",
    "schema_from_v2.db",
    "schema_from_nullable.db",
    "schema_from_blob.db",
    "schema_from_all_affinities.db"
]

def remove_if_exists(path : string) {
    if (fexist(path)) {
        remove(path)
    }
}

[export]
def main() {
    let here = "tests/dasSQLITE/test_data"
    for (name in DBS) {
        remove_if_exists(path_join(here, name))
    }

    with_sqlite(path_join(here, "schema_from_v1.db")) <| $(db) {
        db |> exec("CREATE TABLE Logs (Id INTEGER PRIMARY KEY, Message TEXT NOT NULL)")
    }
    with_sqlite(path_join(here, "schema_from_v2.db")) <| $(db) {
        db |> exec("CREATE TABLE Logs (Id INTEGER PRIMARY KEY, Severity INTEGER NOT NULL, Message TEXT NOT NULL)")
    }
    with_sqlite(path_join(here, "schema_from_nullable.db")) <| $(db) {
        db |> exec("CREATE TABLE Items (Id INTEGER PRIMARY KEY, Note TEXT, Score REAL)")
    }
    with_sqlite(path_join(here, "schema_from_blob.db")) <| $(db) {
        db |> exec("CREATE TABLE Files (Id INTEGER PRIMARY KEY, Hash BLOB NOT NULL, Body BLOB)")
    }
    with_sqlite(path_join(here, "schema_from_all_affinities.db")) <| $(db) {
        db |> exec("CREATE TABLE M (I INTEGER NOT NULL, R REAL NOT NULL, T TEXT NOT NULL, B BLOB NOT NULL, N NUMERIC NOT NULL)")
    }

    print("fixtures generated under {here}\n")
}
