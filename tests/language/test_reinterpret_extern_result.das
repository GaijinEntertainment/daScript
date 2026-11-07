options gen2
require dastest/testing_boost public

def offset(p : uint8?; n : int; zero : auto(TT)) : TT =>
    unsafe(reinterpret<TT>(unsafe(p + n))) - unsafe(reinterpret<TT>(p))

def offset_via_variable(p : uint8?; n : int) : uint64 {
    let q : uint8? = unsafe(p + n)
    return unsafe(reinterpret<uint64>(q)) - unsafe(reinterpret<uint64>(p))
}

[test]
def test_reinterpret_of_an_extern_uint64_result(t : T?) {
    let bits = heap_bytes_allocated()
    let via_local = unsafe(reinterpret<int?>(bits))
    let direct = unsafe(reinterpret<int?>(heap_bytes_allocated()))
    t |> equal(via_local == null, direct == null, "the direct and the two-step reinterpret agree")
}

[test]
def test_reinterpret_of_an_extern_pointer_result(t : T?) {
    var data : uint8[64]
    let p = unsafe(addr(data[0]))
    if (typeinfo sizeof(p) == 8) {
        t |> equal(offset_via_variable(p, 4), 4ul)
        t |> equal(offset(p, 4, 0ul), 4ul)
    }
    t |> equal(offset(p, 4, 0u), 4u)
}
