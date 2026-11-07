options gen2
options persistent_heap
require dastest/testing_boost public
require daslib/regex

//! on the persistent heap a free is exact, so the byte count
//! sees a node nothing owns
[test]
def test_a_compiled_quantifier_frees_its_pass_end(t : T?) {
    for (pat in ["a*b+c?", "(ab|cd)*x", "(a\{2,3\})+"]) {
        var warm <- regex_compile(pat)
        unsafe {
            delete warm
        }
        let before = heap_bytes_allocated()
        for (_ in range(100)) {
            var r <- regex_compile(pat)
            unsafe {
                delete r
            }
        }
        t |> equal(before, heap_bytes_allocated(), "{pat} leaks per compile")
    }
}
