//! pointer arithmetic on a pointee too big for an int stride is
//! refused while the stride is inferred, not later by lint
options gen2
expect 50300:3

require dastest/testing_boost public

[export]
def pointer_stride_too_big() {
    unsafe {
        type<uint64[268435456]?> - 6
        type<uint64[268435456]?> + 6
        var p : uint64[268435456]?
        var q : uint64[268435456]?
        p - q
    }
}
