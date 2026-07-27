options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// LINT018: narrowing the size argument of memcpy / memcmp
//
// Problem: int(n) truncates above 2^31, so the copy or compare silently covers
// the wrong number of bytes. memcpy and memcmp both carry uint / int64 / uint64
// size overloads, so the size can be passed straight through.
//
// Bad pattern:
//   memcpy(dst, src, int(nbytes))   // nbytes : int64
//
// Good pattern:
//   memcpy(dst, src, nbytes)

// bad: int() narrowing of an int64 size (1 warning)
def bad_memcpy_int64(dst, src : void?; nbytes : int64) {
    unsafe(memcpy(dst, src, int(nbytes)))
}

// bad: int() narrowing of a uint64 size (1 warning)
def bad_memcpy_uint64(dst, src : void?; nbytes : uint64) {
    unsafe(memcpy(dst, src, int(nbytes)))
}

// bad: int() narrowing on memcmp (1 warning)
def bad_memcmp_int64(dst, src : void?; nbytes : int64) : int {
    return unsafe(memcmp(dst, src, int(nbytes)))
}

// good: the 64-bit size goes straight to the 64-bit overload
def good_memcpy_int64(dst, src : void?; nbytes : int64) {
    unsafe(memcpy(dst, src, nbytes))
}

// good: an int-typed size needs no cast at all
def good_memcpy_int(dst, src : void?; nbytes : int) {
    unsafe(memcpy(dst, src, nbytes))
}
