options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 31 — SQL views.
//
// A view is a saved SELECT: a virtual table whose rows are produced on
// demand by re-running its body. dasSQLITE represents a view with the
// same struct-driven story as a table — declare a struct describing the
// projected columns, attach `[sql_view]`, and the same read-side helpers
// (`select_from(type<V>)`, `_sql(db |> select_from(type<V>) |> ...)`,
// `column_info(type<V>)`) work without dispatching on table-vs-view.
//
// Mutation paths are blocked: views are read-only.
//   - Predicate-form: `_sql_update(type<V>, ...)`,  `_sql_delete(...)`,
//     `_sql_upsert(...)` — caught at compile time with a clear error.
//   - Row-form: `insert(viewRow)` / `update(...)` / `delete_(...)` —
//     panics at runtime with a "views are read-only" message.
//
// In production, view DDL belongs in a migration body so the schema
// lives alongside table DDL — see a future tut covering migrations.
// For now we create views inline at app setup.

[sql_table(name = "Customers")]
struct Customer {
    @sql_primary_key Id : int
    Name : string
    City : string
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    @sql_references = "Customer" CustomerId : int
    Amount : int
    Status : string
}

// --- A view describing a derived shape ----------------------------------
// `[sql_view]` accepts the same field-level annotations as `[sql_table]`
// in the read-only subset:
//   - `@sql_column = "..."`           rename the on-disk column
//   - `@sql_json` / `@sql_blob`       opt non-scalar storage adapters
// Anything that affects DDL of the underlying table is rejected loud:
// keys, uniqueness, computed columns, defaults, foreign keys.
[sql_view(name = "BigOrders")]
struct BigOrder {
    Id : int
    CustomerId : int
    Amount : int
    Status : string
}

// A second view projecting a join + alias. View column names come from
// the struct field names — the projection's record-name aliases (the
// `Name = ...` in `_select((Name = ...))`) only have to match by count
// and per-column type; the view's column names are taken from the V
// struct's field names (and `@sql_column` renames).
[sql_view(name = "OrderSummary")]
struct OrderSummary {
    OrderId : int
    Customer : string
    Amount : int
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Customer>)
        db |> create_table(type<Order>)
        db |> insert(Customer(Id = 1, Name = "Alice",   City = "Berlin"))
        db |> insert(Customer(Id = 2, Name = "Bob",     City = "Lyon"))
        db |> insert(Customer(Id = 3, Name = "Carmen",  City = "Madrid"))
        db |> insert(Order(Id = 10, CustomerId = 1, Amount = 50,  Status = "shipped"))
        db |> insert(Order(Id = 11, CustomerId = 1, Amount = 250, Status = "shipped"))
        db |> insert(Order(Id = 12, CustomerId = 2, Amount = 800, Status = "pending"))
        db |> insert(Order(Id = 13, CustomerId = 3, Amount = 30,  Status = "shipped"))

        // --- _create_view: filtered full-row view --------------------------
        // The chain looks identical to an `_sql(...)` chain. The macro
        // walks it, builds the SELECT, validates the projection vs V's
        // fields (count + per-position type), and emits a `db |> exec`
        // of the resulting `CREATE VIEW name(c1, c2, …) AS SELECT …`.
        db |> _create_view(type<BigOrder>,
            db |> select_from(type<Order>) |> _where(_.Amount >= 100))

        // --- Querying the view: same surface as a table --------------------
        // `select_from(type<V>)` works directly — the macro emits the
        // same `_sql_select_all_sql` helper that `[sql_table]` does, so
        // there is no view-specific call site.
        for (b in db |> select_from(type<BigOrder>)) {
            to_log(LOG_INFO, "BigOrder #{b.Id}: cust={b.CustomerId} amt={b.Amount} status={b.Status}\n")
        }

        // --- _create_view with a JOIN + named-tuple projection -------------
        // Multi-source chains (`_join_inner` / `_join_left`) produce a
        // SELECT with table aliases. The view's struct supplies the column
        // names; the `_select((... = ...))` aliases here just have to
        // match by position and type.
        db |> _create_view(type<OrderSummary>,
            db |> select_from(type<Order>)
                |> _join(db |> select_from(type<Customer>),
                         $(o : Order, c : Customer) => o.CustomerId == c.Id,
                         $(o : Order, c : Customer) => (OrderId = o.Id, Customer = c.Name, Amount = o.Amount)))

        for (s in db |> select_from(type<OrderSummary>)) {
            to_log(LOG_INFO, "OrderSummary #{s.OrderId} for {s.Customer}: {s.Amount}\n")
        }

        // --- column_info over a view --------------------------------------
        // The same metadata helper that works on tables works on views.
        // Note `is_pk` is always false (views don't have primary keys
        // even when the underlying table does).
        for (c in column_info(type<BigOrder>)) {
            to_log(LOG_INFO, "  col {c.name}  type={int(c.data_type)}  pk={c.is_pk}  nullable={c.is_nullable}\n")
        }

        // --- drop_view_if_exists --------------------------------------
        // Idempotent; missing-view is a no-op (matching DROP TABLE IF
        // EXISTS semantics). The strict form panics on real errors; the
        // try form returns SqlError.
        db |> drop_view_if_exists(type<BigOrder>)
        // Re-create with a tighter filter — proves the drop succeeded.
        db |> _create_view(type<BigOrder>,
            db |> select_from(type<Order>) |> _where(_.Amount >= 500))
        to_log(LOG_INFO, "rows in re-created BigOrder: {length(db |> select_from(type<BigOrder>))}\n")

        // --- Raw-SQL escape hatch ----------------------------------------
        // `_create_view`'s chain is the LINQ-shaped subset (single source
        // or join, FullRow / NamedTuple / GroupedNamedTuple / SingleColumn /
        // Aggregate). Anything outside that — window functions, recursive
        // CTEs, custom SQL functions — needs raw `db |> exec("CREATE VIEW
        // … AS …")`. The view created this way is queryable through the
        // same `select_from(type<V>)` rail as long as you declare a
        // matching V struct with `[sql_view]`.
        db |> exec(
            "CREATE VIEW IF NOT EXISTS CustomerRank AS " +
            "SELECT CustomerId, ROW_NUMBER() OVER (ORDER BY SUM(Amount) DESC) AS Rank, " +
            "       SUM(Amount) AS Total " +
            "FROM Orders GROUP BY CustomerId")
    }
}

// --- Mutation attempts fail loud ---------------------------------------
//
// The compile-time gate fires on:
//   - `_sql_update(type<BigOrder>, ...)`
//   - `_sql_delete(type<BigOrder>, ...)`
//   - `_sql_upsert(bigOrderRow, ...)`
// each with: "cannot mutate [sql_view] 'BigOrder' — views are read-only.
// Mutate the underlying table(s) and re-query the view."
//
// The runtime gate fires on row-form mutations (`insert(bigOrderRow)`,
// `update(bigOrderRow)`, `delete_(bigOrderRow)`) — these panic with the
// same message. Real mutating workflows update the underlying tables
// (Customer / Order here) and re-query the view to see the new state.

// --- Captured locals via `to_sql_literal` -------------------------------
//
// SQLite stores view bodies as text in `sqlite_schema` and rejects `?`
// inside a view definition, so `_create_view` cannot use parameter binds
// the way `_sql(...)` does. Instead it stringifies each bind expression
// once when the CREATE VIEW DDL runs and inlines the literal into the
// view body. Anything that's not a column ref or a recognized SQL
// operator is treated as a bind — captured locals, struct fields,
// function calls, arithmetic over the above:
//
//     let cutoff = 100
//     db |> _create_view(type<BigOrder>,
//         db |> select_from(type<Order>) |> _where(_.Amount >= cutoff))
//     // sqlite_schema now stores: ... WHERE "Amount" >= 100
//
//     struct Cfg { threshold : int }
//     let cfg = Cfg(threshold = 200)
//     db |> _create_view(type<HugeOrder>,
//         db |> select_from(type<Order>) |> _where(_.Amount >= cfg.threshold))
//
// The value is FROZEN at view-creation time. Changing `cutoff` later
// does not update the view; drop and re-create to re-bake.
//
// All numeric primitives, `bool`, and `string` work out of the box.
// User types extend by supplying a one-line `to_sql_literal` overload:
//
//     enum Status { Pending = 1; Shipped = 2 }
//     def to_sql_literal(s : Status) : string => "{int(s)}"
//
//     let want = Status.Shipped
//     db |> _create_view(type<ShippedOrders>,
//         db |> select_from(type<Order>) |> _where(_.StatusVal == want))
//
// Runtime filtering still uses `_sql(...)` with normal `?`-binds at the
// query site — `?` placeholders are fine in SELECTs, just not in DDL.
