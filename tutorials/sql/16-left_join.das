options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 16 — every join shape: INNER, LEFT, RIGHT, FULL OUTER, CROSS.
//
// All five share the same surface: srca |> _<kind>_join(srcb, on, into).
// (Cross is the exception — no `on`, 3-arg form: srca |> _cross_join(srcb, into).)
//
// `into` is the projection lambda. The args' Option-ness changes per kind:
//
//   INNER       (a : TA,         b : TB)         — both present
//   LEFT        (a : TA,         b : Option<TB>) — TB may be none
//   RIGHT       (a : Option<TA>, b : TB)         — TA may be none
//   FULL OUTER  (a : Option<TA>, b : Option<TB>) — either may be none
//   CROSS       (a : TA,         b : TB)         — every pair
//
// `is_some` / `is_none` on an Option arg lowers to SQL:
//
//   <arg> |> is_some  →  "<arg-side>"."<arg-side join key>" IS NOT NULL
//   <arg> |> is_none  →  ... IS NULL
//
// Per-column nullability inside `Option<T>` is not yet wired — for `o.Total`
// when `o : Option<Order>` you'd need to drop to raw SQL.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice"))
    db |> insert(User(Id = 2, Name = "Bob"))
    db |> insert(User(Id = 3, Name = "Carol"))
    db |> insert(Order(Id = 10, UserId = 1,  Total = 100))
    db |> insert(Order(Id = 11, UserId = 1,  Total = 250))
    db |> insert(Order(Id = 12, UserId = 2,  Total =  50))
    db |> insert(Order(Id = 13, UserId = 99, Total = 999))   // orphan — no matching user
    // Asymmetries: Carol has no orders (LEFT-only); Order#13 has no user (RIGHT-only).
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ---- A. INNER JOIN — only matched pairs surface (3 rows). ----
        let inner <- _sql(db |> select_from(type<User>)
                            |> _join(db |> select_from(type<Order>),
                                     $(u : User, o : Order) => u.Id == o.UserId,
                                     $(u : User, o : Order) => (Name = u.Name, OrderId = o.Id)))
        to_log(LOG_INFO, "INNER — matched pairs only:")
        for (r in inner) {
            to_log(LOG_INFO, "  {r.Name}: order #{r.OrderId}")
        }
        let sql_in = _sql_text(db |> select_from(type<User>)
                                  |> _join(db |> select_from(type<Order>),
                                           $(u : User, o : Order) => u.Id == o.UserId,
                                           $(u : User, o : Order) => (Name = u.Name, OrderId = o.Id)))
        to_log(LOG_INFO, "  SQL: {sql_in}")

        // ---- B. LEFT JOIN — every User surfaces; Carol gets HasOrder=false (4 rows). ----
        let lefts <- _sql(db |> select_from(type<User>)
                            |> _left_join(db |> select_from(type<Order>),
                                          $(u : User, o : Order) => u.Id == o.UserId,
                                          $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))
        to_log(LOG_INFO, "LEFT — every user surfaces:")
        for (r in lefts) {
            to_log(LOG_INFO, "  {r.Name}: HasOrder = {r.HasOrder}")
        }
        let sql_lj = _sql_text(db |> select_from(type<User>)
                                  |> _left_join(db |> select_from(type<Order>),
                                                $(u : User, o : Order) => u.Id == o.UserId,
                                                $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))
        to_log(LOG_INFO, "  SQL: {sql_lj}")

        // ---- C. RIGHT JOIN — every Order surfaces; orphan Order#13 gets HasUser=false (4 rows). ----
        let rights <- _sql(db |> select_from(type<User>)
                             |> _right_join(db |> select_from(type<Order>),
                                            $(u : User, o : Order) => u.Id == o.UserId,
                                            $(u : Option<User>, o : Order) => (HasUser = u |> is_some, OrderId = o.Id)))
        to_log(LOG_INFO, "RIGHT — every order surfaces:")
        for (r in rights) {
            to_log(LOG_INFO, "  order #{r.OrderId}: HasUser = {r.HasUser}")
        }
        let sql_rj = _sql_text(db |> select_from(type<User>)
                                  |> _right_join(db |> select_from(type<Order>),
                                                 $(u : User, o : Order) => u.Id == o.UserId,
                                                 $(u : Option<User>, o : Order) => (HasUser = u |> is_some, OrderId = o.Id)))
        to_log(LOG_INFO, "  SQL: {sql_rj}")

        // ---- D. FULL OUTER JOIN — both sides surface; Carol AND Order#13 appear (5 rows). ----
        // Probes both args: `u |> is_some` lowers to LEFT key IS NOT NULL,
        // `o |> is_some` lowers to RIGHT key IS NOT NULL.
        let fulls <- _sql(db |> select_from(type<User>)
                            |> _full_outer_join(db |> select_from(type<Order>),
                                                $(u : User, o : Order) => u.Id == o.UserId,
                                                $(u : Option<User>, o : Option<Order>) => (HasUser = u |> is_some, HasOrder = o |> is_some)))
        to_log(LOG_INFO, "FULL OUTER — both sides surface:")
        for (r in fulls) {
            to_log(LOG_INFO, "  HasUser = {r.HasUser}, HasOrder = {r.HasOrder}")
        }
        let sql_fo = _sql_text(db |> select_from(type<User>)
                                  |> _full_outer_join(db |> select_from(type<Order>),
                                                      $(u : User, o : Order) => u.Id == o.UserId,
                                                      $(u : Option<User>, o : Option<Order>) => (HasUser = u |> is_some, HasOrder = o |> is_some)))
        to_log(LOG_INFO, "  SQL: {sql_fo}")

        // ---- E. CROSS JOIN — Cartesian product, no ON clause (3 × 4 = 12 rows). ----
        // Filter post-cross with `_where` to recover an inner-join shape.
        let cross <- _sql(db |> select_from(type<User>)
                            |> _cross_join(db |> select_from(type<Order>),
                                           $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id)))
        to_log(LOG_INFO, "CROSS — every (user, order) pair:")
        for (r in cross) {
            to_log(LOG_INFO, "  {r.UserName} × order #{r.OrderId}")
        }
        let sql_cx = _sql_text(db |> select_from(type<User>)
                                  |> _cross_join(db |> select_from(type<Order>),
                                                 $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id)))
        to_log(LOG_INFO, "  SQL: {sql_cx}")
    }
}
