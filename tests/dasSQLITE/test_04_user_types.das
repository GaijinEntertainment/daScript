options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

// User-defined column type. Stored as TEXT, formatted "r,g,b" on bind, parsed
// on read-back.
struct Color {
    r : int
    g : int
    b : int
}

[unused_argument(witness)]
def sqlite_sql_type(witness : Color) : string {
    return "TEXT"
}

def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : Color) : void {
    let s = "{value.r},{value.g},{value.b}"
    sqlite3_bind_text(stmt, idx, s)
}

[sql_table(name="Pixels")]
struct Pixel {
    @sql_primary_key Id : int
    Tint : Color
}

[test]
def test_user_type_in_create_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Pixel>)
    t |> success(ddl |> find("\"Tint\" TEXT NOT NULL") >= 0, "user sqlite_sql_type used in DDL")
}

[test]
def test_user_type_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Pixel>)
        db |> insert(Pixel(Id = 1, Tint = Color(r = 255, g = 64, b = 0)))
        let s = db |> query_scalar("SELECT Tint FROM Pixels WHERE Id = 1", type<string>)
        t |> equal(s, "255,64,0", "user sqlite_bind formatted the value on insert")
    }
}
