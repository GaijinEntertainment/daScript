options gen2

require dastest/testing_boost public
require daslib/linq_das

// linq_das v1: C#-style `%linq! from c in src where <pred> select <proj> %%` over an array source.
// The reader macro renames the single range variable to the `_` placeholder and emits a `_fold(...)`
// chain materialized via `to_array()`.

struct Car {
    name  : string
    price : int
}

def private mk_cars() : array<Car> {
    return <- [Car(name="cheap", price=50),
               Car(name="mid", price=150),
               Car(name="lux", price=300)]
}

[test]
def test_where_select(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from c in cars where c.price > 100 select c.name %%
    t |> equal(length(names), 2, "where filters, select projects names")
    t |> equal(names[0], "mid")
    t |> equal(names[1], "lux")
}

[test]
def test_select_only(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from c in cars select c.name %%
    t |> equal(length(names), 3, "no where: every element projected")
    t |> equal(names[0], "cheap")
    t |> equal(names[2], "lux")
}

[test]
def test_identity_select(t : T?) {
    // `select c` is identity — no `_select` is emitted; the result is the filtered elements.
    let cars <- mk_cars()
    let kept <- %linq! from c in cars where c.price >= 150 select c %%
    t |> equal(length(kept), 2, "identity select returns filtered structs")
    t |> equal(kept[0].name, "mid")
    t |> equal(kept[1].name, "lux")
}

[test]
def test_where_no_match(t : T?) {
    let cars <- mk_cars()
    let names <- %linq! from c in cars where c.price > 10000 select c.name %%
    t |> equal(length(names), 0, "no element matches → empty result")
}

[test]
def test_numeric_projection_and_parens(t : T?) {
    // parenthesized predicate + multiple range-var refs + numeric projection
    let cars <- mk_cars()
    let prices <- %linq! from c in cars where (c.price > 100) && c.price < 300 select c.price %%
    t |> equal(length(prices), 1, "only mid (150) is >100 and <300")
    t |> equal(prices[0], 150)
}

[test]
def test_string_literal_in_predicate(t : T?) {
    // the keyword 'select' and the range var 'c' inside a string literal must NOT be touched
    let cars <- mk_cars()
    let names <- %linq! from c in cars where c.name == "lux" select c.name %%
    t |> equal(length(names), 1, "string-literal predicate matches exactly one")
    t |> equal(names[0], "lux")
}

[test]
def test_inline_in_larger_expression(t : T?) {
    // %linq! is an inline reader macro — it composes inside a larger expression
    let cars <- mk_cars()
    let n = length(%linq! from c in cars where c.price > 100 select c.name %%)
    t |> equal(n, 2, "query result used directly as a call argument")
}

[test]
def test_arbitrary_range_var_name(t : T?) {
    // the range variable is spliced verbatim as the block parameter — any identifier name works,
    // and it is kept (not rewritten to `_`), appearing in both predicate and projection
    let cars <- mk_cars()
    let names <- %linq! from car in cars where car.price > 100 select car.name %%
    t |> equal(length(names), 2, "multi-char range var splices verbatim")
    t |> equal(names[0], "mid")
}

[test]
def test_multiline_query(t : T?) {
    let cars <- mk_cars()
    let names <- %linq!
        from c in cars
        where c.price > 100
        select c.name
    %%
    t |> equal(length(names), 2, "multi-line query body parses identically")
    t |> equal(names[0], "mid")
}
