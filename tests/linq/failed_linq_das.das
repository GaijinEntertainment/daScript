options gen2

// linq_das reader-macro rejections. Each malformed query emits one macro_error (50503) and a benign
// `(0)` fallback, so the surrounding statement still parses — one 50503 per bad query, no cascade.
expect 50503   // `fromc` — not the standalone `from` keyword (read as one identifier)
expect 50503   // `orderby` before `group` (would emit a non-fusing _order_by |> _group_by_lazy)
expect 50503   // group element is not the range variable (element selectors unsupported)
expect 50503   // `group` with no `by` clause
expect 50503   // join: two `join` clauses (nested joins unsupported)
expect 50503   // join: missing `on`
expect 50503   // join: `on` without `equals`
expect 50503   // join: composite key (second `equals` before the terminal)
expect 50503   // join: `join … into` (group-join) unsupported
expect 50503   // join: `where` inside the join clause (between join and equals)
expect 50503   // join: two `where` clauses (one pre-join, one post-join)
expect 50503   // join: range var collides with the reserved transparent-id name

require daslib/linq_das
require strings

struct Car {
    name  : string
    price : int
    brand : string
}

def private one_car() {
    return <- [Car(name = "a", price = 50, brand = "x")]
}

def trigger_bad_keyword() {
    var cars <- one_car()
    let x <- %linq! fromc in cars select c %%
    unused(x)
}

def trigger_orderby_before_group() {
    var cars <- one_car()
    let x <- %linq! from c in cars orderby c.price group c by c.brand %%
    unused(x)
}

def trigger_group_element_mismatch() {
    var cars <- one_car()
    let x <- %linq! from c in cars group c.name by c.brand %%
    unused(x)
}

def trigger_group_missing_by() {
    var cars <- one_car()
    let x <- %linq! from c in cars group c %%
    unused(x)
}

def trigger_join_two_joins() {
    var cars <- one_car()
    let x <- %linq! from c in cars join b in cars on c.brand equals b.brand join d in cars on c.name equals d.name select c.name %%
    unused(x)
}

def trigger_join_missing_on() {
    var cars <- one_car()
    let x <- %linq! from c in cars join b in cars select c.name %%
    unused(x)
}

def trigger_join_on_without_equals() {
    var cars <- one_car()
    let x <- %linq! from c in cars join b in cars on c.brand select c.name %%
    unused(x)
}

def trigger_join_composite_key() {
    var cars <- one_car()
    let x <- %linq! from c in cars join b in cars on c.brand equals b.brand && c.name equals b.name select c.name %%
    unused(x)
}

def trigger_join_into_group_join() {
    var cars <- one_car()
    let x <- %linq! from c in cars join b in cars on c.brand equals b.brand into g select c.name %%
    unused(x)
}

def trigger_join_where_inside_clause() {
    var cars <- one_car()
    let x <- %linq! from c in cars join b in cars where c.price > 0 on c.brand equals b.brand select c.name %%
    unused(x)
}

def trigger_join_two_wheres() {
    var cars <- one_car()
    let x <- %linq! from c in cars where c.price > 0 join b in cars on c.brand equals b.brand where c.name == "a" select c.name %%
    unused(x)
}

def trigger_join_reserved_var_name() {
    var cars <- one_car()
    let x <- %linq! from linq_join_ti in cars join b in cars on linq_join_ti.brand equals b.brand select linq_join_ti.name %%
    unused(x)
}
