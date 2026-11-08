//! sscan_json into a struct constructor expression must not compile:
//! a temporary struct cannot be a destination.
options gen2
options rtti
expect 30107

struct ScanDestination {
    value : int
}

[export]
def main() {
    sscan_json("\{\"value\":7\}", ScanDestination())
}
