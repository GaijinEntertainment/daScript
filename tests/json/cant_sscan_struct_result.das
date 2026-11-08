//! sscan_json into a function call's struct result must not compile:
//! a call result is a temporary, not a destination.
options gen2
options rtti
expect 30107

struct ScanDestination {
    value : int
}

def value() : ScanDestination => ScanDestination()

[export]
def main() {
    sscan_json("\{\"value\":7\}", value())
}
