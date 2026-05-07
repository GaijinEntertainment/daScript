options gen2

// Predicate-side JSON-path validation. Each `_where` below names a JSON-path
// segment that doesn't exist on the daslang-side struct/tuple — typos that
// would otherwise compile to `json_extract(...)` SQL and silently match no
// rows. The predicate-side recognizer mirrors the SELECT-side guard: it
// walks the daslang type via `resolve_nested_field_type` and emits a
// macro_error when a segment can't be resolved.
//
// Each typo fires TWO errors: 40104 = the macro_error from `_sql`'s JSON-path
// validation, and 30503 = daslang's typer rejecting the field access after
// `_sql` returns null and the chain is re-checked. The cascade confirms the
// macro's failure path properly propagates (without the SqlQuery.hadError
// fix, the macro_error was queued but operator handlers like `==` wrapped
// the empty fragment as `" = ?"` and analyze_predicate treated it as success).
expect 30928:2, 30831, 50503:3

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

struct InnerAddress {
    City : string
    Zip : string
}

struct InnerProfile {
    Addr : InnerAddress
    Bio : string
}

[sql_table(name = "Accounts")]
struct Account {
    @sql_primary_key Id : int
    @sql_json Profile : InnerProfile
}

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    @sql_json Prefs : tuple<theme : string; lang : string; notify : bool>
}

[export]
def main() {
    var db = SqlRunner()

    // 1. Tuple field typo — `cuty` instead of `theme`/`lang`/`notify`.
    let bad1 <- _sql(db |> select_from(type<User>) |> _where(_.Prefs.cuty == "X"))

    // 2. Outer-struct field typo at top level — `Addr` exists, `Bia` doesn't.
    let bad2 <- _sql(db |> select_from(type<Account>) |> _where(_.Profile.Bia == "X"))

    // 3. Nested path typo — `Addr` exists, `City` exists, but `Zipp` doesn't.
    let bad3 <- _sql(db |> select_from(type<Account>) |> _where(_.Profile.Addr.Zipp == "X"))
}
