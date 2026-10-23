options gen2
options rtti
expect 31200

struct ScanDestination {
    value : int
}

[export]
def main() {
    var pointer : ScanDestination?
    sscan_json("\{\"value\":7\}", pointer ?? ScanDestination())
}
