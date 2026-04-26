options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 14 — `_group_by` and `_having`.
//
// `_group_by(key)` partitions rows into buckets. After grouping, the
// chain element is a (key, group-rows) tuple shaped like C# LINQ's
// `IGrouping<K, T>`:
//
//   - `_._0`        — the group key (single-key `_group_by(_.City)`)
//   - `_._0._N`     — the Nth key column (multi-key `(_.City, _.Age)`)
//   - `_._1`        — the array of rows in this bucket
//
// Aggregates on the group:
//
//   - `_._1 |> length`                                   → COUNT(*)
//   - `_._1 |> count`                                    → COUNT(*) (alias)
//   - `_._1 |> select($(u : T) => u.Field) |> sum`       → SUM("Field")
//   - `_._1 |> select($(u : T) => u.Field) |> average`   → AVG("Field")
//   - `_._1 |> select($(u : T) => u.Field) |> min`       → MIN("Field")
//   - `_._1 |> select($(u : T) => u.Field) |> max`       → MAX("Field")
//
// The inner `select` lambda parameter is **named** (`$(u : User)`),
// not `_`. The outer `_select` already binds `_` to the group tuple,
// and daslang flags a same-name-different-type lambda nested inside
// as a shadowing error. Naming the inner parameter sidesteps it.
// The macro recognizes the body as `<param>.<field>` for any param
// name.
//
// `_having(predicate)` filters on the aggregated buckets — it sits
// between `_group_by` and the projection. `_where` (pre-aggregate)
// stays before `_group_by`. The macro routes binds correctly:
// WHERE binds first, HAVING next, LIMIT last, in placeholder order.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name   : string
    City   : string
    Age    : int
    Salary : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100))
    db |> insert(User(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80))
    db |> insert(User(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120))
    db |> insert(User(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90))
    db |> insert(User(Id = 5, Name = "Eve",   City = "Chicago", Age = 50, Salary = 200))
    db |> insert(User(Id = 6, Name = "Frank", City = "Boston",  Age = 28, Salary = 70))
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ── Single-key group_by: head-count per city ─────────────────────
        // SELECT "City", COUNT(*) FROM "Users" GROUP BY "City"
        let by_city <- _sql(db |> select_from(type<User>)
                              |> _group_by(_.City)
                              |> _order_by(_._0)
                              |> _select((City = _._0, N = _._1 |> length)))
        to_log(LOG_INFO, "Users per city:\n")
        for (r in by_city) {
            to_log(LOG_INFO, "  {r.City}: {r.N}\n")
        }

        // ── Multiple aggregates in one projection ────────────────────────
        // SELECT "City", COUNT(*), MIN("Age"), MAX("Age"), SUM("Salary")
        // FROM "Users" GROUP BY "City"
        let stats <- _sql(db |> select_from(type<User>)
                            |> _group_by(_.City)
                            |> _order_by(_._0)
                            |> _select((
                                City   = _._0,
                                N      = _._1 |> length,
                                MinAge = _._1 |> select($(u : User) => u.Age)    |> min,
                                MaxAge = _._1 |> select($(u : User) => u.Age)    |> max,
                                Total  = _._1 |> select($(u : User) => u.Salary) |> sum)))
        to_log(LOG_INFO, "Per-city stats:\n")
        for (r in stats) {
            to_log(LOG_INFO, "  {r.City}: N={r.N} ages={r.MinAge}..{r.MaxAge} salary={r.Total}\n")
        }

        // ── _where (pre-aggregate filter) before _group_by ───────────────
        // SQL clause order: WHERE → GROUP BY. The macro routes
        // `_where` to the WHERE clause (filters input rows) and the
        // result is grouped over what's left.
        let by_city_seniors <- _sql(db |> select_from(type<User>)
                                      |> _where(_.Age >= 30)
                                      |> _group_by(_.City)
                                      |> _order_by(_._0)
                                      |> _select((City = _._0, N = _._1 |> length)))
        to_log(LOG_INFO, "Seniors per city (Age>=30):\n")
        for (r in by_city_seniors) {
            to_log(LOG_INFO, "  {r.City}: {r.N}\n")
        }

        // ── _having (post-aggregate filter) ─────────────────────────────
        // SELECT "City", COUNT(*) FROM "Users"
        // GROUP BY "City" HAVING COUNT(*) > ?
        // Binds: [1]
        let popular <- _sql(db |> select_from(type<User>)
                              |> _group_by(_.City)
                              |> _having(_._1 |> length > 1)
                              |> _order_by(_._0)
                              |> _select((City = _._0, N = _._1 |> length)))
        to_log(LOG_INFO, "Cities with more than 1 user:\n")
        for (r in popular) {
            to_log(LOG_INFO, "  {r.City}: {r.N}\n")
        }

        // ── Multi-key grouping ───────────────────────────────────────────
        // Group key is a tuple. Each tuple field becomes its own
        // GROUP BY column; project them with `_._0._N`.
        // SQL: SELECT "City", "Age", COUNT(*) FROM "Users"
        //      GROUP BY "City", "Age"
        db |> insert(User(Id = 7, Name = "Adam", City = "NYC", Age = 30, Salary = 110))
        let by_city_age <- _sql(db |> select_from(type<User>)
                                  |> _group_by((_.City, _.Age))
                                  |> _having(_._1 |> length > 1)
                                  |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))
        to_log(LOG_INFO, "Multi-key buckets with 2+ members:\n")
        for (r in by_city_age) {
            to_log(LOG_INFO, "  ({r.City}, age {r.Age}): {r.N}\n")
        }

        // ── Full reporting query: WHERE → GROUP BY → HAVING → ORDER BY → LIMIT
        // The chain mirrors SQL clause order one-for-one. Bind ordering
        // tracks: WHERE binds first, HAVING next, LIMIT last.
        let min_age = 25
        let min_count = 1
        let report <- _sql(db |> select_from(type<User>)
                             |> _where(_.Age >= min_age)
                             |> _group_by(_.City)
                             |> _having(_._1 |> length >= min_count)
                             |> _order_by(_._0)
                             |> _select((
                                 City      = _._0,
                                 N         = _._1 |> length,
                                 AvgSalary = _._1 |> select($(u : User) => u.Salary) |> average))
                             |> take(10))
        to_log(LOG_INFO, "Reporting query:\n")
        for (r in report) {
            to_log(LOG_INFO, "  {r.City}: N={r.N} avg salary={r.AvgSalary}\n")
        }

        // ── Inspect emitted SQL ──────────────────────────────────────────
        let sql = _sql_text(db |> select_from(type<User>)
                              |> _where(_.Age >= min_age)
                              |> _group_by(_.City)
                              |> _having(_._1 |> length >= min_count)
                              |> _order_by(_._0)
                              |> _select((City = _._0, N = _._1 |> length))
                              |> take(10))
        to_log(LOG_INFO, "  emitted SQL: {sql}\n")
    }
}
