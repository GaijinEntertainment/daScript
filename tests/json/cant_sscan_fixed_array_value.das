//! sscan_json into a fixed-array literal must not compile: a
//! temporary fixed array cannot be a destination.
options gen2
options rtti
expect 30107

[export]
def main() {
    sscan_json("[1,2]", [1, 2])
}
