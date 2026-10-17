// Fixture for test_reserved_word_name.das - an identifier where the
// grammar allows none is bison's `unexpected name`, and the message
// carries no reserved-word note. `_`-prefix keeps dastest from
// auto-running it; the `expect` directive makes lint skip it.
expect 30151

options gen2

def f(a b : int) {
    return a
}

[export]
def main() {
    print("{f(1)}")
}
