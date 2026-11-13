//! a block whose result does not match the block signature of a C++
//! binding does not compile
options gen2
expect 30341

require UnitTest

[export]
def main {
    test_das_string($() => [int()])
}
