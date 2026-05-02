options gen2

// _create_view body inlines bind values via _::to_sql_literal. A bind whose
// type has no overload (here: a whole struct, not a primitive field of one)
// hits the catch-all `to_sql_literal(auto(TT))` at sqlite_linq.das, which
// concept_asserts (40103). Confirms the user gets a pointed "define
// to_sql_literal for your type" message instead of an opaque overload error.
expect 40103:1

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    Price : int
}

[sql_view(name = "Big")]
struct Big {
    Id : int
    Price : int
}

struct Cfg {
    val : int
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Item>)
        let cfg = Cfg(val = 100)
        // Whole struct used as a bind — Cfg has no `to_sql_literal` overload.
        // (cfg.val WOULD work — that's an int and matches the default overload.)
        db |> _create_view(type<Big>,
            db |> select_from(type<Item>) |> _where(_.Price >= cfg))
    }
}
