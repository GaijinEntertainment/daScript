options gen2

//! three instances bounds-check `a[i]` at one source location
[never_inline, hint(noinline)]
def pick(a : array<auto(T)>; i : int) : T => a[i]

[export]
def main {
    let ints <- [1, 2, 3]
    let floats <- [1.5, 2.5]
    let names <- ["x", "y"]
    print("PICK {pick(ints, 2)} {pick(floats, 1)} {pick(names, 0)}\n")
}
