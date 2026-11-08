//! sscan_json into a const struct must not compile: the destination
//! is written, so it cannot be const.
options gen2
options rtti
expect 30341

struct ScanDestination {
    value : int
}

[export]
def main() {
    let value : ScanDestination
    sscan_json("\{\"value\":7\}", value)
}
