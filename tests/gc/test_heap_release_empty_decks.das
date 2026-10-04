// Reserved capacity (heap_total_allocated) drops after a dead burst is collected. Interpreter only:
// the test reaches heap_collect with a collectable argument, which AOT refuses to compile.
options gen2
options persistent_heap
options gc
options no_aot

require dastest/testing_boost public

struct Pebble {
    value : int
    weight : float
}

var g_pebbles : array<Pebble?>

def private make_pebbles(n : int) {
    g_pebbles |> resize(n)
    for (p, i in g_pebbles, count()) {
        p = new Pebble(value = i, weight = float(i))
    }
}

[test]
def test_collection_releases_empty_decks(t : T?) {
    let reserved_idle = heap_total_allocated()
    make_pebbles(200000)
    let reserved_peak = heap_total_allocated()
    t |> success(reserved_peak > reserved_idle, "the burst grows the reserved capacity")
    g_pebbles |> resize(0)
    unsafe(heap_collect(true, false))
    let reserved_after = heap_total_allocated()
    t |> success(reserved_after < reserved_peak, "a collection releases the burst's empty decks")
    t |> success(reserved_after >= heap_bytes_allocated(), "reserved capacity stays at or above live bytes")
    t |> success(reserved_after > 0ul, "reserved capacity stays non-zero")
    make_pebbles(1000)
    t |> equal(g_pebbles[999].value, 999, "allocation continues on the kept decks")
    g_pebbles |> resize(0)
}
