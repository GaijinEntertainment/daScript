//! under options lint = false a table literal with a duplicate
//! constant key still does not compile
options gen2
options lint = false

expect 30706

[export]
def main {
    var tab <- {
        1 => "one",
        1 => "one again"
    }
    feint("{tab}\n")
}
