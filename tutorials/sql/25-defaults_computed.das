options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 25 — column defaults and computed (generated) columns.
//
// THREE WAYS TO DEFAULT A COLUMN:
//
//   (1) Native daslang field initializer — the literal becomes the SQL
//       DEFAULT clause:
//           Active : bool = true            -> DEFAULT 1
//           Quantity : int = 100            -> DEFAULT 100
//           Tag : string = "unknown"        -> DEFAULT 'unknown'
//       Supports bool / int / int64 / float / double / string literals.
//       Non-literal initializers (e.g. `some(5)`) are silently dropped
//       from the DDL but still construct in daslang.
//
//   (2) `@sql_default_fn = "FN"` — whitelisted SQL built-ins:
//           CURRENT_TIMESTAMP, CURRENT_DATE, CURRENT_TIME
//       Use this when SQLite must compute the default at INSERT time
//       (e.g. timestamps).
//
// COMPUTED COLUMNS via `@sql_computed = "expression"`:
//   - Default storage is VIRTUAL (recomputed on every read).
//   - Add `@sql_stored = true` for STORED (materialized on write).
//   - The expression is passed verbatim to SQLite — column refs and
//     function names are validated by SQLite at CREATE TABLE time.
//   - INSERT and UPDATE bind paths skip computed columns automatically;
//     SELECT reads them as ordinary columns.
//   - `[sql_table]` rejects: `@sql_primary_key + @sql_computed`,
//     `@sql_computed + field initializer`, `@sql_computed +
//     @sql_default_fn`, `field initializer + @sql_default_fn`.

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int

    Name : string

    // (1) literal default via daslang field init.
    Active : bool = true
    Quantity : int = 100
    Tag : string = "unknown"

    // (2) SQL built-in default.
    @sql_default_fn = "CURRENT_TIMESTAMP"
    CreatedAt : string

    // VIRTUAL computed column — re-derived on every SELECT.
    @sql_computed = "Quantity * 2"
    DoubleQty : int

    // STORED computed column — materialized on INSERT/UPDATE, read
    // straight off disk on SELECT.
    @sql_computed = "Quantity + 1"
    @sql_stored = true
    QtyPlusOne : int
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Item>)

        // The DDL helper is callable directly — handy for inspection
        // and migration scripts.
        // Sample fragment of what `_sql_create_table_sql(type<Item>)`
        // emits for the columns above:
        //   "Active"     INTEGER NOT NULL DEFAULT 1,
        //   "Quantity"   INTEGER NOT NULL DEFAULT 100,
        //   "Tag"        TEXT    NOT NULL DEFAULT 'unknown',
        //   "CreatedAt"  TEXT    NOT NULL DEFAULT CURRENT_TIMESTAMP,
        //   "DoubleQty"  INTEGER GENERATED ALWAYS AS (Quantity * 2) VIRTUAL,
        //   "QtyPlusOne" INTEGER GENERATED ALWAYS AS (Quantity + 1) STORED
        let ddl = _sql_create_table_sql(type<Item>)
        to_log(LOG_INFO, "DDL:\n{ddl}\n")

        // (a) Macro-generated INSERT binds every NON-COMPUTED column ----
        // The struct field's value is what gets written; defaults never
        // fire through this path. Computed columns are skipped — SQLite
        // computes them itself, so any value the struct holds for
        // `DoubleQty` / `QtyPlusOne` is ignored.
        // SQL: INSERT INTO "Items" ("Id","Name","Active","Quantity","Tag",
        //                           "CreatedAt") VALUES (?,?,?,?,?,?)
        db |> insert(Item(Id = 1, Name = "thing", Active = true, Quantity = 7,
                          Tag = "t", CreatedAt = "",
                          DoubleQty = 9999, QtyPlusOne = 9999))
        let r1 = _sql(db |> select_from(type<Item>) |> _where(_.Id == 1) |> _first())
        to_log(LOG_INFO, "row 1: Quantity={r1.Quantity} DoubleQty={r1.DoubleQty} QtyPlusOne={r1.QtyPlusOne}\n")

        // (b) Defaults fire when the column is omitted from the INSERT --
        // The macro INSERT always names every non-computed column, so
        // the DEFAULT clause only fires from raw SQL or column-list
        // forms that omit the column.
        // SQL: INSERT INTO "Items" ("Id","Name") VALUES (?,?)
        //      → "Active" DEFAULT 1, "Quantity" DEFAULT 100,
        //        "Tag" DEFAULT 'unknown', "CreatedAt" DEFAULT
        //        CURRENT_TIMESTAMP all fire
        db |> exec("INSERT INTO Items (Id, Name) VALUES (2, 'defaulted')")
        let r2 = _sql(db |> select_from(type<Item>) |> _where(_.Id == 2) |> _first())
        to_log(LOG_INFO, "row 2 (defaulted): Active={r2.Active} Quantity={r2.Quantity} Tag={r2.Tag}\n")
        to_log(LOG_INFO, "row 2 CreatedAt length: {length(r2.CreatedAt)} (expect ≥ 19)\n")

        // (c) UPDATE re-derives computed columns automatically ----------
        // Mutating `Quantity` is enough — the SET clause omits any
        // `@sql_computed` column (rejected at macro-compile time, so a
        // typo reaches the user). DoubleQty / QtyPlusOne update behind
        // the scenes when SQLite re-evaluates the expression.
        // SQL: UPDATE "Items" SET "Quantity" = ? WHERE "Id" = ?
        db |> _sql_update(type<Item>, _.Id == 1, (Quantity = 11))
        let r1b = _sql(db |> select_from(type<Item>) |> _where(_.Id == 1) |> _first())
        to_log(LOG_INFO, "after update: Quantity={r1b.Quantity} DoubleQty={r1b.DoubleQty} QtyPlusOne={r1b.QtyPlusOne}\n")
    }
}

// --- VIRTUAL vs STORED ------------------------------------------------
//
// VIRTUAL (the default): the expression is recomputed on every SELECT.
// Cheap to write, slightly more expensive to read, no disk cost.
//
// STORED: the expression is evaluated on INSERT/UPDATE and persisted.
// Faster reads, more disk, the computed value is part of the row layout
// (so changing the expression requires drop + recreate — neither form
// supports ALTER TABLE on the expression).
//
// Use STORED when the expression is expensive and reads dominate writes.
// Default to VIRTUAL otherwise.
//
// SQLITE 3.31+ is required for generated columns.
