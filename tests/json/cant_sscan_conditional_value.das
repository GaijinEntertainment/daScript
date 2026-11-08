//! sscan_json into a conditional (cond ? var : temporary) must not
//! compile: a temporary branch cannot be a destination.
options gen2
options rtti
expect 30107

struct ScanDestination {
    value : int
}

def scan_selected(var value : ScanDestination; choose : bool) : bool =>
    sscan_json("\{\"value\":7\}", choose ? value : ScanDestination())

[export]
def main() {
    var value : ScanDestination
    scan_selected(value, true)
}
