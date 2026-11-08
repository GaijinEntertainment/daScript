//! sscan_json into a `pointer ?? temporary` fallback must not
//! compile: the temporary side cannot be a destination.
options gen2
options rtti
expect 30107

struct ScanDestination {
    value : int
}

[export]
def main() {
    var pointer : ScanDestination?
    sscan_json("\{\"value\":7\}", pointer ?? ScanDestination())
}
