options gen2

//! a program for test_policy.das: one array literal passed to a
//! constant array parameter
var g_seed = 1

def total(xs : array<int>) : int {
    var sum = 0
    for (x in xs) {
        sum += x
    }
    return sum
}

[export]
def main() {
    print("{total([g_seed, 2, 3])}\n")
}
