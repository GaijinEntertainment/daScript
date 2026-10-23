options gen2
options rtti
expect 31200

struct ScanDestination {
    value : int
}

def value() : ScanDestination => ScanDestination()

[export]
def main() {
    sscan_json("\{\"value\":7\}", value())
}
