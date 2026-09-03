options gen2

// Fixture for test_c_header_emit: what -lib-export-all offers, and what it leaves out.

def public_ok(x : int) : int {
    return x + 1
}

def public_array(a : array<int>) : int {
    return length(a)
}

def private hidden() : int {
    return 1
}

def generic_one(x) {
    return x
}

[init]
def setup() {}

[export]
def main() {
    print("{public_ok(1)}{public_array([1])}{hidden()}{generic_one(1)}\n")
}
