// A borrowed array view (a mapped file plane) whose byte length is a whole multiple of 4 GiB: the mark walk sizes
// the range in 64 bits. Sized in 32, the range read as empty and the validating collect indexed the heap's chunk
// table by it.
options gen2
options persistent_heap
options gc
options no_aot   // the test calls heap_collect with its `T?` argument live, and AOT refuses a function whose frame the collector must walk (error 50503)

require dastest/testing_boost public

var g_backing : uint64[16]   // global memory, outside the heap, as a mapping is
var g_plane : array<uint64>

[test]
def test_gc_borrowed_range_of_4gib(t : T?) {
    let n = 1l << 29l   // 2^29 uint64 = 4 GiB of range over 16 real elements; the walk reads no element of a uint64 array
    unsafe {
        _builtin_make_temp_array_i64(g_plane, addr<void?>(g_backing[0]), n)
        heap_collect(true, true)
    }
    t |> equal(long_length(g_plane), n, "the view survives a validating collect")
    unsafe(_builtin_forget_temp_array(g_plane))
}
