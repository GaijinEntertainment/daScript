//! A fixed array with dimension 0 must not compile (30109), because a
//! dimension must be positive.
// Stage 0 characterization (FIXED_ARRAY_REWORK.md): dimension <= 0 is
// a compile error.
options gen2
expect 30109

[export]
def main {
    var a : int[0]
    a[0] = 1
}
