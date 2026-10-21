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
