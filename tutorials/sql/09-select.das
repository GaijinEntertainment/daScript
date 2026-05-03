options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 09 — `_select` projections.
//
// `_select(...)` controls what columns the SQL emits and what shape the
// result has. Three forms are supported:
//
//   - default (no _select): SELECT all columns, materialize as the source
//                           struct  →  array<Car>
//   - single column:        _select(_.FieldName)
//                           SELECT one column, materialize as that column's type
//                                  →  array<string> (etc.)
//   - named tuple:          _select((Name = _.Name, Price = _.Price))
//                           SELECT chosen columns, materialize as a tuple
//                           with `recordNames` so you read result.Name etc.
//                                  →  array<tuple<Name:string;Price:int>>
//
// The named-tuple form lets you rename columns at the result-type level
// (`(Identifier=_.Id, Label=_.Name)` returns rows whose `Identifier` field
// holds the underlying `Id`).

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
        db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
        db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))

        // ── default projection: full Car ─────────────────────────────────
        let cars <- _sql(db |> select_from(type<Car>))
        to_log(LOG_INFO, "Default projection — {length(cars)} cars:\n")
        for (c in cars) {
            to_log(LOG_INFO, "  Id={c.Id} Name={c.Name} Price={c.Price}\n")
        }

        // ── single-column projection: _select(_.Field) ───────────────────
        let names <- _sql(db |> select_from(type<Car>) |> _select(_.Name))
        to_log(LOG_INFO, "Single-column — {length(names)} names:\n")
        for (n in names) {
            to_log(LOG_INFO, "  {n}\n")
        }

        // ── named-tuple projection ───────────────────────────────────────
        // Result type is tuple<Name:string;Price:int>. The tuple's
        // recordNames preserve `Name` / `Price` so you read row.Name.
        let pairs <- _sql(db |> select_from(type<Car>)
                            |> _select((Name = _.Name, Price = _.Price)))
        to_log(LOG_INFO, "Named-tuple — {length(pairs)} pairs:\n")
        for (p in pairs) {
            to_log(LOG_INFO, "  {p.Name} (price={p.Price})\n")
        }

        // ── Renaming via named tuple: result fields differ from source ───
        let renamed <- _sql(db |> select_from(type<Car>)
                              |> _select((Identifier = _.Id, Label = _.Name)))
        to_log(LOG_INFO, "Renamed projection — {length(renamed)} rows:\n")
        for (r in renamed) {
            to_log(LOG_INFO, "  Identifier={r.Identifier} Label={r.Label}\n")
        }

        // ── Combining with terminals: _first, _first_opt ─────────────────
        let head = _sql(db |> select_from(type<Car>)
                          |> _select((Name = _.Name, Price = _.Price))
                          |> _first())
        to_log(LOG_INFO, "First named-tuple: {head.Name} ({head.Price})\n")

        // ── Inspect the emitted SQL ──────────────────────────────────────
        let sql = _sql_text(db |> select_from(type<Car>)
                              |> _select((Name = _.Name, Price = _.Price)))
        to_log(LOG_INFO, "  emitted SQL: {sql}\n")
    }
}
