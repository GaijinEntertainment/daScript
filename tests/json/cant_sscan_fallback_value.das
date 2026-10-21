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
