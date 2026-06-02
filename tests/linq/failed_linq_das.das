options gen2

// linq_das reader-macro rejections. Each malformed query emits one macro_error (50503) and a benign
// `(0)` fallback, so the surrounding statement still parses — one 50503 per bad query, no cascade.
expect 50503   // `fromc` — not the standalone `from` keyword (read as one identifier)
expect 50503   // `orderby` before `group` (would emit a non-fusing _order_by |> _group_by_lazy)
expect 50503   // group element is not the range variable (element selectors unsupported)
expect 50503   // `group` with no `by` clause

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
