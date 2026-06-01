options gen2

// linq_das must reject a query whose first word is not the standalone `from` keyword.
// `fromc` (missing the space) is read as a single identifier, not `from` + range var.
expect 50503

require daslib/linq_das
require strings

struct Car {
    name  : string
    price : int
}

def trigger() {
    var cars <- [Car(name = "a", price = 50)]
    let x <- %linq! fromc in cars select c %%
    unused(x)
}
