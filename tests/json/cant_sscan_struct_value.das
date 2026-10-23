options gen2
options rtti
expect 31200

struct ScanDestination {
    value : int
}

[export]
def main() {
    sscan_json("\{\"value\":7\}", ScanDestination())
}
