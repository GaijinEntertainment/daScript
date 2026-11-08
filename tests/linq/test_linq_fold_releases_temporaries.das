options gen2
options persistent_heap

require dastest/testing_boost public
require daslib/linq
require daslib/linq_boost

struct Car {
    id : int
    dealer : int
}

struct Dealer {
    id : int
}

def make_cars() : array<Car> => <- [for (i in range(1000)); Car(id = i, dealer = i % 10)]

def make_dealers() : array<Dealer> => <- [for (i in range(10)); Dealer(id = i)]

[test(description = "a folded group_join returns every bucket to the heap, leaving the allocated byte count unchanged after 20 runs")]
def test_group_join_fold_frees_its_buckets(t : T?) {
    var cars <- make_cars()
    var dealers <- make_dealers()
    let before = heap_bytes_allocated()
    for (_k in range(20)) {
        var inscope rows <- _fold(dealers |> _group_join(cars, $(d : Dealer, c : Car) => d.id == c.dealer,
                                                         $(d : Dealer, cs : array<Car>) => (id = d.id, n = length(cs))) |> to_array())
        t |> equal(length(rows), 10)
    }
    t |> equal(heap_bytes_allocated(), before)
}

[test(description = "a folded join returns its buckets to the heap, leaving the allocated byte count unchanged after 20 runs")]
def test_join_fold_frees_its_buckets(t : T?) {
    var cars <- make_cars()
    var dealers <- make_dealers()
    let before = heap_bytes_allocated()
    for (_k in range(20)) {
        var inscope rows <- _fold(dealers |> _join(cars, $(d : Dealer, c : Car) => d.id == c.dealer,
                                                   $(d : Dealer, c : Car) => (d = d.id, c = c.id)) |> to_array())
        t |> equal(length(rows), 1000)
    }
    t |> equal(heap_bytes_allocated(), before)
}

[test(description = "a folded reverse/take/select/to_array chain releases its intermediate buffer, leaving the allocated byte count unchanged after 20 runs")]
def test_terminal_select_frees_its_buffer(t : T?) {
    var cars <- make_cars()
    let before = heap_bytes_allocated()
    for (_k in range(20)) {
        var inscope ids <- _fold(each(cars).reverse().take(10)._select(_.id).to_array())
        t |> equal(ids[0], 999)
    }
    t |> equal(heap_bytes_allocated(), before)
}
