//! a fixed-array dimension written as a generator expression does not
//! compile
options gen2
expect 30109, 30816

variant var9 {}

var v : var9[generator<type<lambda<>>> {}]

[export]
def main {
    pass
}
