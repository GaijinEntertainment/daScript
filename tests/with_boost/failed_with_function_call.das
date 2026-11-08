//! with_ over a function call result must not compile (50503),
//! because the container must be an indexed expression.
options gen2
expect 50503

require daslib/with_boost

struct A {
    f1 : int
}

def get_struct() : A {
    return A(f1 = 42)
}

[export]
def main {
    with_(get_struct()) {  // refuse: not ExprAt
        print("body")
    }
}
