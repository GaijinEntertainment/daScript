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
expect 50503   // multiple-from: three `from` clauses (N-ary SelectMany unsupported)
expect 50503   // multiple-from: `from … from` combined with `join`
expect 50503   // multiple-from: range var collides with the reserved transparent-id name
expect 50503   // correlated: `orderby` over a correlated `from … from` (non-copyable outer can't ride the carry)
expect 50503   // correlated: `group` over a correlated `from … from` (non-copyable outer can't ride the carry)
expect 50503   // correlated: post-`from` `where` references the OUTER range var (only inner-only is pushed down)
expect 50503   // let: binding name shadows the range variable
expect 50503   // let: missing `=` after the binding name
expect 50503   // let: empty right-hand-side expression
expect 50503   // let: same binding name bound twice
expect 50503   // let: trailing `let` after the `select` terminal (no clause uses it)

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

struct Line {
    sku : string
    qty : int
}

struct Order {
    id       : int
    customer : string
    lines    : array<Line>
}

def private one_order() {
    var o <- Order(id = 1, customer = "Acme")
    o.lines |> emplace(Line(sku = "a", qty = 2))
    return <- [o]
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

def trigger_correlated_orderby() {
    var orders <- one_order()
    // `orderby` over a correlated `from … from` — the non-copyable outer can't ride the transparent-id carry
    let x <- %linq! from o in orders from l in o.lines orderby l.qty select l.sku %%
    unused(x)
}

def trigger_correlated_group() {
    var orders <- one_order()
    let x <- %linq! from o in orders from l in o.lines group o by o.customer %%
    unused(x)
}

def trigger_correlated_outer_where() {
    var orders <- one_order()
    // a post-`from` `where` that references the OUTER `o` (only inner-only predicates are pushed down)
    let x <- %linq! from o in orders from l in o.lines where o.id > 0 select l.sku %%
    unused(x)
}

def trigger_multifrom_three_froms() {
    var cars <- one_car()
    let x <- %linq! from a in cars from b in cars from d in cars select a.name %%
    unused(x)
}

def trigger_multifrom_with_join() {
    var cars <- one_car()
    let x <- %linq! from a in cars from b in cars join d in cars on a.brand equals d.brand select a.name %%
    unused(x)
}

def trigger_multifrom_reserved_var_name() {
    var cars <- one_car()
    let x <- %linq! from linq_join_ti in cars from b in cars select linq_join_ti.name %%
    unused(x)
}

def trigger_let_shadows_range_var() {
    var cars <- one_car()
    let x <- %linq! from c in cars let c = c.price + 1 select c %%
    unused(x)
}

def trigger_let_missing_equals() {
    var cars <- one_car()
    let x <- %linq! from c in cars let n c.price select n %%
    unused(x)
}

def trigger_let_empty_rhs() {
    var cars <- one_car()
    let x <- %linq! from c in cars let n = select c.name %%
    unused(x)
}

def trigger_let_duplicate_binding() {
    var cars <- one_car()
    let x <- %linq! from c in cars let n = c.price let n = c.name select n %%
    unused(x)
}

def trigger_let_after_terminal() {
    var cars <- one_car()
    // a `let` after the `select` terminal has no clause to use it — must not be silently dropped
    let x <- %linq! from c in cars select c.name let n = c.price %%
    unused(x)
}
