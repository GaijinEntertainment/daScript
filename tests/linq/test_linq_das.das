options gen2
options rtti

require dastest/testing_boost public
require daslib/linq_das
require daslib/decs_boost

// linq_das: C#-style `%linq! from c [: Row] in src [where <pred>] select <proj> [iterator] %%` reader
// macro. Array source (untyped) and decs source (`from c : CarComp in decs`) are covered here; SQL and
// XML live with their modules (tests/dasSQLITE, tests/dasPUGIXML). The range variable is spliced
// verbatim as the block parameter; a trailing `iterator` keyword yields a lazy `iterator<T>`.

struct Car {
    name  : string
    price : int
}

def private mk_cars() : array<Car> {
    return <- [Car(name = "cheap", price = 50),
               Car(name = "mid", price = 150),
               Car(name = "lux", price = 300)]
}

[decs_template(prefix = "ld_")]
struct CarComp {
    name  : string
    price : int
}

def private seed_decs() {
    restart()
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.ld_name := "car{i}"
            cmp.ld_price := 100 * (i + 1)   // 100, 200, 300
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
