options gen2

struct S {
    a : int64
    b : int64
}

//! the element distance between two pointers into one array
[never_inline, hint(noinline)]
def distance(p : S?; base : S?) : int => int(p - base)

[never_inline, hint(noinline)]
def distance64(p : S?; base : S?) : int64 => p - base

[export]
def main {
    var arr : S[64]
    unsafe {
        let fwd = distance(addr(arr[5]), addr(arr[0]))
        let back = distance(addr(arr[2]), addr(arr[7]))
        print("DIST {fwd} {back}\n")
        print("DIST64 {distance64(addr(arr[2]), addr(arr[7]))}\n")
    }
}
