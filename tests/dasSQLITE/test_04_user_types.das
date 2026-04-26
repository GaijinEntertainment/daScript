options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/strings_boost
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

[unused_argument(t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<Color>) : Color {
    let s = sqlite3_column_text_(stmt, col)
    let parts <- split(s, ",")
    return Color(r = to_int(parts[0]), g = to_int(parts[1]), b = to_int(parts[2]))
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

[test]
def test_user_type_select_from(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Pixel>)
        db |> insert(Pixel(Id = 1, Tint = Color(r = 255, g = 64, b = 0)))
        db |> insert(Pixel(Id = 2, Tint = Color(r = 0, g = 128, b = 255)))
        let rows <- db |> select_from(type<Pixel>)
        t |> equal(length(rows), 2, "select_from returns all rows")
        t |> equal(rows[0].Id, 1, "row 0 Id")
        t |> equal(rows[0].Tint.r, 255, "row 0 Tint.r — round-tripped through user sqlite_read")
        t |> equal(rows[0].Tint.g, 64, "row 0 Tint.g")
        t |> equal(rows[0].Tint.b, 0, "row 0 Tint.b")
        t |> equal(rows[1].Tint.r, 0, "row 1 Tint.r")
        t |> equal(rows[1].Tint.g, 128, "row 1 Tint.g")
        t |> equal(rows[1].Tint.b, 255, "row 1 Tint.b")
    }
}
