options gen2
options stack = 16384
options heap_size_hint = 65536

// Tier B: the das heap. Arrays, tables and `new`/`delete` all allocate from the
// context's own heap, sized by `options heap_size_hint` - a single malloc on a
// host, and on a board the one allocation the runtime makes.
//
// Nothing here uses strings: string interpolation and the string builders are
// the next tier up, and nano leaves them out on purpose.

struct Sample {
    id : int
    value : float
}

[export]
def sum_range(n : int) : int {
    var values : array<int>
    values |> reserve(n)
    for (i in range(n)) {
        values |> push(i * i)
    }
    var total = 0
    for (v in values) {
        total += v
    }
    return total
}

[export]
def histogram_peak(n : int) : int {
    var counts : table<int; int>
    for (i in range(n)) {
        let bucket = i % 7
        counts[bucket] = (counts?[bucket] ?? 0) + 1
    }
    var peak = 0
    for (c in values(counts)) {
        if (c > peak) {
            peak = c
        }
    }
    return peak
}

// An explicit `delete` frees through the scope-free path, the only part of the
// collector nano carries. There is no GC pass behind it: what a script does not
// delete stays allocated until the context does.
[export]
def alloc_and_free(id : int) : float {
    var sample = new Sample(id = id, value = float(id) * 0.5)
    let v = sample.value
    unsafe {
        delete sample
    }
    return v
}
