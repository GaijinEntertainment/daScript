//! sscan_json into a vector constructor expression must not compile:
//! a temporary vector is const, not a destination.
options gen2
expect 30341

[export]
def main() {
    sscan_json("[1,2]", float2(1, 2))
}
