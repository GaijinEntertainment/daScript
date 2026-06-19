// array/table literal build benchmark. A gen2 literal feeding to_array_move/to_table_move
// (`[..]`, `array<T>(..)`, `{..}`) builds the array<T> directly on the heap — no stack T[N]
// + memcpy. Each iteration move-assigns a fresh literal into a global: the move finalizes the
// previous value (no leak) and the store keeps the build live (not dead-code-eliminated).
options gen2
options persistent_heap
options no_aot

require dastest/testing_boost

struct Vec {
    x, y, z, w : float
}

var gInts : array<int>
var gVecs : array<Vec>
var gTab : table<int; int>

[benchmark]
def bench_array_literal(b : B?) {
    b |> run("array_literal/ints_12") {
        gInts <- [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120]
    }
    b |> run("array_literal/vecs_8") {
        gVecs <- [Vec(x = 1.0), Vec(y = 2.0), Vec(z = 3.0), Vec(w = 4.0),
                  Vec(x = 5.0), Vec(y = 6.0), Vec(z = 7.0), Vec(w = 8.0)]
    }
    b |> run("array_literal/table_8") {
        gTab <- {1 => 10, 2 => 20, 3 => 30, 4 => 40, 5 => 50, 6 => 60, 7 => 70, 8 => 80}
    }
}
