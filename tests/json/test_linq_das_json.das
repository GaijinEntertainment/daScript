options gen2

require daslib/json_boost
require daslib/linq_das
require daslib/linq_boost public
require dastest/testing_boost public

// The C# query reader (`%linq! … %%`) over a JSON source — `from c : Row in <JsonValue>` dispatches through
// from_in to from_json. Each query is compared against the equivalent array-source `%linq!` so the JSON
// typed-source path is verified to match the (untyped) array path exactly.

struct Car {
    id    : int
    price : int
    qty   : int
    brand : string
}

def fixture_array() : array<Car> {
    return <- [
        Car(id = 1, price = 100,   qty = 2, brand = "Ford"),
        Car(id = 2, price = 9999,  qty = 4, brand = "Ferrari"),
        Car(id = 3, price = 200,   qty = 6, brand = "Honda"),
        Car(id = 4, price = 50,    qty = 8, brand = "Ford"),
        Car(id = 5, price = 88888, qty = 1, brand = "Ferrari"),
        Car(id = 6, price = 300,   qty = 3, brand = "Honda"),
        Car(id = 7, price = 150,   qty = 5, brand = "Ford")]
}

def fixture_json() : JsonValue? {
    return JV([for (c in fixture_array()); JV(c)])
}

[test]
def query_where_select(t : T?) {
    let jv = fixture_json()
    let arr <- fixture_array()
    let j <- %linq! from c : Car in jv where c.price > 150 select c.id %%
    let a <- %linq! from c in arr where c.price > 150 select c.id %%
    t |> equal(j, a)
}

[test]
def query_orderby(t : T?) {
    let jv = fixture_json()
    let arr <- fixture_array()
    let j <- %linq! from c : Car in jv orderby c.price descending select c.id %%
    let a <- %linq! from c in arr orderby c.price descending select c.id %%
    t |> equal(j, a)
}

[test]
def query_let_and_multi_where(t : T?) {
    let jv = fixture_json()
    let arr <- fixture_array()
    let j <- %linq! from c : Car in jv let total = c.price * c.qty where total > 400 where c.brand != "Ferrari" orderby c.id select "{c.brand}:{total}" %%
    let a <- %linq! from c in arr let total = c.price * c.qty where total > 400 where c.brand != "Ferrari" orderby c.id select "{c.brand}:{total}" %%
    t |> equal(j, a)
}

[test]
def query_group_by(t : T?) {
    let jv = fixture_json()
    let arr <- fixture_array()
    let j <- %linq! from c : Car in jv group c by c.brand into g orderby g.key select "{g.key}={g |> length}" %%
    let a <- %linq! from c in arr group c by c.brand into g orderby g.key select "{g.key}={g |> length}" %%
    t |> equal(j, a)
}

[test]
def query_iterator_keyword(t : T?) {
    let jv = fixture_json()
    let arr <- fixture_array()
    let ji <- [for (x in %linq! from c : Car in jv where c.qty > 3 select c.id iterator %%); x]
    let ai <- [for (x in %linq! from c in arr where c.qty > 3 select c.id iterator %%); x]
    t |> equal(ji, ai)
}
