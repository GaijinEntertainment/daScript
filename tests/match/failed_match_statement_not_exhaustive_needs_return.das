//! a statement match over an int with no catch-all arm can fall
//! through, so the function still needs a return after it
options gen2

require daslib/match

expect 30310

def digit_name(x : int) : string {
    match (x) {
        0 => $ { return "zero" }
        1 => $ { return "one" }
    }
}

[export]
def test {
    print("{digit_name(1)}\n")
}
