//! with_ over an element of an array literal must not compile
//! (50503), because the container must be a named variable, not a
//! temporary.
options gen2
expect 50503

require daslib/with_boost

struct A {
    f1 : int
}

[export]
def main {
    // Array literal as container — temp lifetime ends with the
    // expression, can't be ref-bound. Refused at macro time.
    with_([A(f1 = 1), A(f1 = 2)][0]) {
        print("body")
    }
}
