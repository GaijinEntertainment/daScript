options gen2
options no_optimizations = true

require dastest/testing_boost public

def each_ref_unsafe(pa; n : int) {
    return <- generator<int const&> {
        for (index in range(n)) {
            unsafe {
                yield (*pa)[index]
            }
        }
        return false
    }
}

[test(description = "a generator that yields from an unsafe block resumes at its label with optimizations off")]
def test_generator_yield_in_unsafe_block(t : T?) {
    var a <- [1, 2, 3]
    var sum = 0
    var count = 0
    unsafe {
        for (x in each_ref_unsafe(addr(a), 3)) {
            sum += x
            count ++
        }
    }
    t |> equal(count, 3)
    t |> equal(sum, 6)
}

enum KeywordNamed {
    short
    long
}

def local_typedef_inc(x : int) : int {
    typedef Local = int
    let y : Local = x + 1
    return y
}

[test(description = "a local typedef and an enum value named like a C++ keyword survive to the emitter with optimizations off")]
def test_unoptimized_emitter_shapes(t : T?) {
    t |> equal(local_typedef_inc(1), 2)
    let w = KeywordNamed.short
    t |> success(w != KeywordNamed.long)
}
