options gen2

require dastest/testing_boost public
require daslib/linq_das
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// linq_das over a SQL source: `from c : Car in db` → `from_in(db, type<Car>)` → `select_from`, which
// `_fold` routes to `_sql` (WHERE/SELECT pushdown). The C# range variable is spliced verbatim; `_sql`
// normalizes the single-source lambda param to `_` internally, and absorbs the trailing `to_array()`.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key id : int
    name  : string
    price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(id = 1, name = "BMW",   price = 100))
    db |> insert(Car(id = 2, name = "Audi",  price = 200))
    db |> insert(Car(id = 3, name = "Tesla", price = 300))
}

[test]
def test_sql_where_select(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- %linq! from c : Car in db where c.price > 150 select c.name %%
        // parity with the hand-written placeholder chain through _sql
        let ref <- _fold(db |> select_from(type<Car>) |> _where(_.price > 150) |> _select(_.name))
        t |> equal(length(names), 2, "sql where/select (Audi, Tesla)")
        t |> equal(length(names), length(ref), "linq_das ≡ hand-written _sql chain")
        t |> equal(names[0], "Audi")
        t |> equal(names[1], "Tesla")
    }
}

[test]
def test_sql_identity_select(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- %linq! from c : Car in db where c.price >= 200 select c %%
        t |> equal(length(rows), 2, "sql identity select returns rows")
        t |> equal(rows[0].name, "Audi")
    }
}

[test]
def test_sql_iterator_output(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- [for (nm in %linq! from c : Car in db where c.price > 150 select c.name iterator %%); nm]
        t |> equal(length(names), 2, "sql iterator output")
    }
}

[test]
def test_sql_orderby_ascending(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // ORDER BY price ASC over the SQL source
        let names <- %linq! from c : Car in db orderby c.price select c.name %%
        let ref <- _fold(db |> select_from(type<Car>) |> _order_by(_.price) |> _select(_.name))
        t |> equal(length(names), 3)
        t |> equal(names[0], "BMW")     // 100
        t |> equal(names[2], "Tesla")   // 300
        t |> equal(length(names), length(ref), "linq_das orderby ≡ hand-written _sql chain")
    }
}

[test]
def test_sql_orderby_descending_with_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // WHERE price >= 200 then ORDER BY price DESC → Tesla(300), Audi(200)
        let names <- %linq! from c : Car in db where c.price >= 200 orderby c.price descending select c.name %%
        t |> equal(length(names), 2, "sql where + order by desc")
        t |> equal(names[0], "Tesla")
        t |> equal(names[1], "Audi")
    }
}

// ===== join over SQL — select-terminal + pre-join where push down (post-join transparent-id is the
// documented boundary: it would carry a whole-row tuple `_sql` rejects; see failed_linq_das_sql.das) =====

[sql_table(name = "JCars")]
struct JCar {
    @sql_primary_key id : int
    name  : string
    brand : string
    price : int
}

[sql_table(name = "JBrands")]
struct JBrand {
    @sql_primary_key brand : string
    country : string
}

def private join_fixture(db : SqlRunner) {
    db |> create_table(type<JCar>)
    db |> create_table(type<JBrand>)
    db |> insert(JCar(id = 1, name = "BMW",   brand = "de", price = 100))
    db |> insert(JCar(id = 2, name = "Audi",  brand = "de", price = 200))
    db |> insert(JCar(id = 3, name = "Tesla", brand = "us", price = 300))
    db |> insert(JBrand(brand = "de", country = "Germany"))
    db |> insert(JBrand(brand = "us", country = "USA"))
}

[test]
def test_sql_join_select_terminal_tuple(t : T?) {
    with_sqlite(":memory:") $(db) {
        join_fixture(db)
        let rows <- %linq! from c : JCar in db join b : JBrand in db on c.brand equals b.brand select (Name = c.name, Country = b.country) %%
        // parity with the hand-written _sql join (select-terminal, both range vars in the result selector)
        let ref <- _fold(db |> select_from(type<JCar>)
            |> _join(db |> select_from(type<JBrand>), $(c, b) => c.brand == b.brand, $(c, b) => (Name = c.name, Country = b.country)))
        t |> equal(length(rows), 3, "3 cars each match a brand HQ")
        t |> equal(length(rows), length(ref), "linq_das ≡ hand-written _sql join")
    }
}

[test]
def test_sql_join_select_terminal_scalar(t : T?) {
    with_sqlite(":memory:") $(db) {
        join_fixture(db)
        let names <- %linq! from c : JCar in db join b : JBrand in db on c.brand equals b.brand select c.name %%
        t |> equal(length(names), 3, "scalar select-terminal pushes down")
    }
}

[test]
def test_sql_join_pre_join_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        join_fixture(db)
        // `where` before `join` filters JCars (price >= 200 → Audi, Tesla), then joins — all pushed to SQL.
        let rows <- %linq! from c : JCar in db where c.price >= 200 join b : JBrand in db on c.brand equals b.brand select (Name = c.name, Country = b.country) %%
        t |> equal(length(rows), 2, "pre-join where pushes down to a SQL WHERE")
    }
}

// ===== multiple `from` over SQL (uncorrelated → CROSS JOIN pushdown) — same select-terminal / pre-from
// where boundary as join; a post-from where carries the (c, d) tuple in-memory (the documented boundary) =====

[test]
def test_sql_multifrom_tuple(t : T?) {
    with_sqlite(":memory:") $(db) {
        join_fixture(db)
        let rows <- %linq! from c : JCar in db from b : JBrand in db select (Name = c.name, Country = b.country) %%
        // parity with the hand-written _sql cross (projection in the _cross_join `into` lambda)
        let ref <- _fold(db |> select_from(type<JCar>)
            |> _cross_join(db |> select_from(type<JBrand>), $(c, b) => (Name = c.name, Country = b.country)))
        t |> equal(length(rows), 6, "3 cars × 2 brands cartesian product")
        t |> equal(length(rows), length(ref), "linq_das ≡ hand-written _sql cross")
    }
}

[test]
def test_sql_multifrom_emits_cross_join(t : T?) {
    with_sqlite(":memory:") $(db) {
        join_fixture(db)
        // the reader's SQL emit shape — confirm it pushes down to a CROSS JOIN, not an in-memory product
        let sqlText = _sql_text(db |> select_from(type<JCar>)
            |> _cross_join(db |> select_from(type<JBrand>), $(c, b) => (Name = c.name, Country = b.country))
            |> to_array())
        t |> success(sqlText |> find("CROSS JOIN") >= 0, "SQL: {sqlText}")
    }
}

[test]
def test_sql_multifrom_pre_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        join_fixture(db)
        // pre-from `where` filters JCars (price >= 200 → Audi, Tesla), then crosses — all pushed to SQL.
        let rows <- %linq! from c : JCar in db where c.price >= 200 from b : JBrand in db select (Name = c.name, Country = b.country) %%
        t |> equal(length(rows), 4, "2 filtered cars × 2 brands")
    }
}

// ===== let bindings over SQL — the inlined computed expression pushes down (no in-memory step) =====
// `let` is textual inlining, so `let net = c.price - 10` reused in where/orderby/select becomes the
// computed column / key forms `_sql` renders directly (see test_99_computed_select_subquery for the
// pushdown machinery). The whole query stays a single SELECT.

[test]
def test_sql_let_pushdown(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- %linq! from c : Car in db let net = c.price - 10 where net < 250 orderby net
            select (Name = c.name, Net = net) %%
        // parity with the hand-written chain (the form `let` inlines to)
        let ref <- _fold(db |> select_from(type<Car>)
            |> _where((_.price - 10) < 250) |> _order_by(_.price - 10)
            |> _select((Name = _.name, Net = _.price - 10)))
        t |> equal(length(rows), 2, "BMW(90), Audi(190) — Tesla(290) excluded")
        t |> equal(length(rows), length(ref), "linq_das let ≡ hand-written _sql chain")
        t |> equal(rows[0].Name, "BMW")
        t |> equal(rows[0].Net, 90)
        t |> equal(rows[1].Name, "Audi")
        t |> equal(rows[1].Net, 190)
    }
}

[test]
def test_sql_let_emits_computed_pushdown(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // confirm the reader's emit shape pushes the inlined `let` arithmetic into the SQL itself
        let sqlText = _sql_text(db |> select_from(type<Car>)
            |> _where((_.price - 10) < 250) |> _order_by(_.price - 10)
            |> _select((Name = _.name, Net = _.price - 10)) |> to_array())
        t |> success(sqlText |> find("ORDER BY (\"price\") - (10)") >= 0, "computed key pushed down: {sqlText}")
        t |> success(sqlText |> find("WHERE (\"price\") - (?)") >= 0, "computed predicate pushed down: {sqlText}")
    }
}
