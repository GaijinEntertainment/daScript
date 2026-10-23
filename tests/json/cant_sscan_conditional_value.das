options gen2
options rtti
expect 31200

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
