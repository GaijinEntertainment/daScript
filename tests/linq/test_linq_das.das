options gen2
options rtti
// decs `orderby` / `group` chains have no dedicated decs splice arm for these minimal shapes, so they
// fall to the tier-2 materialize path and emit a `_fold` perf advisory. Correct, just unfused — suppress
// the cosmetic warning here (a decs-fold gap, not a linq_das issue).
options _no_linq_perf_warn = true

require dastest/testing_boost public
require daslib/linq_das
require daslib/decs_boost

// linq_das: C#-style `%linq! from c [: Row] in src [where <pred>] [orderby <expr> [descending]]
// ( select <proj> | group c by <key> ) [iterator] %%` reader macro. Array source (untyped) and decs
// source (`from c : CarComp in decs`) are covered here; SQL and XML live with their modules
// (tests/dasSQLITE, tests/dasPUGIXML). The range variable is spliced verbatim as the block parameter;
// a trailing `iterator` keyword yields an `iterator<T>` over the materialized result.

struct Car {
    name  : string
    price : int
    brand : string
}

def private mk_cars() : array<Car> {
    return <- [Car(name = "cheap", price = 50,  brand = "eco"),
               Car(name = "mid",   price = 150, brand = "eco"),
               Car(name = "lux",   price = 300, brand = "lux")]
}

[decs_template(prefix = "ld_")]
struct CarComp {
    name  : string
    price : int
    brand : string
}

def private seed_decs() {
    restart()
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.ld_name := "car{i}"
            cmp.ld_price := 100 * (i + 1)        // 100, 200, 300
            cmp.ld_brand := i < 2 ? "a" : "b"    // a, a, b
        }
    }
    commit()
}

// ===== array source =====

[test]
def test_array_where_select(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from c in cars where c.price > 100 select c.name %%
    t |> equal(length(names), 2, "where filters, select projects")
    t |> equal(names[0], "mid")
    t |> equal(names[1], "lux")
}

[test]
def test_array_select_only(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from c in cars select c.name %%
    t |> equal(length(names), 3, "no where: every element projected")
    t |> equal(names[0], "cheap")
}

[test]
def test_array_identity_select(t : T?) {
    let cars <- mk_cars()
    let kept <- %linq! from c in cars where c.price >= 150 select c %%
    t |> equal(length(kept), 2, "identity select returns filtered structs")
    t |> equal(kept[0].name, "mid")
}

[test]
def test_array_numeric_projection_and_parens(t : T?) {
    let cars <- mk_cars()
    let prices <- %linq! from c in cars where (c.price > 100) && c.price < 300 select c.price %%
    t |> equal(length(prices), 1, "only mid (150)")
    t |> equal(prices[0], 150)
}

[test]
def test_array_string_literal_in_predicate(t : T?) {
    // the keyword 'select' and range var 'c' inside a string literal must NOT be touched
    let cars <- mk_cars()
    let names <- %linq! from c in cars where c.name == "lux" select c.name %%
    t |> equal(length(names), 1, "string-literal predicate matches exactly one")
    t |> equal(names[0], "lux")
}

[test]
def test_array_arbitrary_range_var_name(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from car in cars where car.price > 100 select car.name %%
    t |> equal(length(names), 2, "multi-char range var splices verbatim")
    t |> equal(names[0], "mid")
}

[test]
def test_array_inline_in_larger_expression(t : T?) {
    let cars <- mk_cars()
    let n = length(%linq! from c in cars where c.price > 100 select c.name %%)
    t |> equal(n, 2, "query result used directly as a call argument")
}

[test]
def test_array_multiline_query(t : T?) {
    let cars <- mk_cars()
    let names <- %linq!
        from c in cars
        where c.price > 100
        select c.name
    %%
    t |> equal(length(names), 2, "multi-line query body parses identically")
}

[test]
def test_comments_in_body_are_stripped(t : T?) {
    // line/block comments are blanked before parsing — a keyword or range var inside a comment must not
    // confuse the clause splitter, and the comment must not leak into the spliced chain.
    let cars <- mk_cars()
    let names <- %linq!
        from c in cars
        // this where / select / c.name inside a comment must be ignored
        where c.price > 100   /* and so must this block: select c here */
        select c.name
    %%
    t |> equal(length(names), 2, "comments stripped, clauses split correctly")
    t |> equal(names[0], "mid")
}

[test]
def test_array_iterator_output(t : T?) {
    // trailing `iterator` keyword → lazy iterator<T> (consumed here by a comprehension)
    let cars <- mk_cars()
    let got <- [for (nm in %linq! from c in cars where c.price > 100 select c.name iterator %%); nm]
    t |> equal(length(got), 2, "iterator output streams projected elements")
    t |> equal(got[0], "mid")
    t |> equal(got[1], "lux")
}

// ===== decs source (typed range variable `: CarComp` + the `decs` keyword marker) =====

[test]
def test_decs_where_select(t : T?) {
    seed_decs()
    let names <- %linq! from c : CarComp in decs where c.price > 100 select c.name %%
    t |> equal(length(names), 2, "decs: where filters, select projects")
}

[test]
def test_decs_identity_select(t : T?) {
    seed_decs()
    let rows <- %linq! from c : CarComp in decs where c.price >= 200 select c %%
    t |> equal(length(rows), 2, "decs identity select returns the row tuples")
}

[test]
def test_decs_iterator_output(t : T?) {
    seed_decs()
    let names <- [for (nm in %linq! from c : CarComp in decs where c.price > 100 select c.name iterator %%); nm]
    t |> equal(length(names), 2, "decs iterator output")
}

// ===== orderby (single key, optional `descending`) =====

[test]
def test_array_orderby_asc(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from c in cars orderby c.price select c.name %%
    t |> equal(length(names), 3)
    t |> equal(names[0], "cheap")   // 50
    t |> equal(names[1], "mid")     // 150
    t |> equal(names[2], "lux")     // 300
}

[test]
def test_array_orderby_descending(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from c in cars orderby c.price descending select c.name %%
    t |> equal(names[0], "lux")
    t |> equal(names[1], "mid")
    t |> equal(names[2], "cheap")
}

[test]
def test_array_where_orderby(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from c in cars where c.price >= 150 orderby c.price descending select c.name %%
    t |> equal(length(names), 2, "where then sort")
    t |> equal(names[0], "lux")
    t |> equal(names[1], "mid")
}

[test]
def test_array_orderby_identity_select(t : T?) {
    let cars <- mk_cars()
    let rows <- %linq! from c in cars orderby c.price select c %%
    t |> equal(length(rows), 3)
    t |> equal(rows[0].name, "cheap")
    t |> equal(rows[2].name, "lux")
}

[test]
def test_array_orderby_iterator(t : T?) {
    let cars <- mk_cars()
    let got <- [for (nm in %linq! from c in cars orderby c.price descending select c.name iterator %%); nm]
    t |> equal(got[0], "lux")
    t |> equal(got[2], "cheap")
}

[test]
def test_decs_orderby(t : T?) {
    seed_decs()
    let names <- %linq! from c : CarComp in decs orderby c.price descending select c.name %%
    t |> equal(length(names), 3)
    t |> equal(names[0], "car2")   // price 300
    t |> equal(names[2], "car0")   // price 100
}

// ===== group … by (terminal; IGrouping `(key, [elems])` per bucket; in-memory sources) =====

[test]
def test_array_group_by(t : T?) {
    let cars <- mk_cars()
    let groups <- %linq! from c in cars group c by c.brand %%
    t |> equal(length(groups), 2, "two brand buckets")
    var ecoN = 0
    var luxN = 0
    for (g in groups) {
        if (g._0 == "eco") { ecoN = length(g._1) }
        if (g._0 == "lux") { luxN = length(g._1) }
    }
    t |> equal(ecoN, 2, "eco bucket has cheap+mid")
    t |> equal(luxN, 1, "lux bucket has lux")
}

[test]
def test_array_group_with_where(t : T?) {
    let cars <- mk_cars()
    // price>=150 keeps mid(eco), lux(lux) → one element each
    let groups <- %linq! from c in cars where c.price >= 150 group c by c.brand %%
    t |> equal(length(groups), 2, "filtered then grouped")
    for (g in groups) { t |> equal(length(g._1), 1) }
}

[test]
def test_array_group_iterator(t : T?) {
    let cars <- mk_cars()
    var n = 0
    for (_g in %linq! from c in cars group c by c.brand iterator %%) { n++ }
    t |> equal(n, 2, "group iterator yields the buckets")
}

[test]
def test_decs_group_by(t : T?) {
    seed_decs()
    let groups <- %linq! from c : CarComp in decs group c by c.brand %%
    t |> equal(length(groups), 2, "decs group buckets a/b")
    for (g in groups) {
        if (g._0 == "a") { t |> equal(length(g._1), 2, "bucket a: car0+car1") }
        if (g._0 == "b") { t |> equal(length(g._1), 1, "bucket b: car2") }
    }
}
