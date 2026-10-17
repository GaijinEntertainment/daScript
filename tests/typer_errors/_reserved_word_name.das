// Fixture for test_reserved_word_name.das - a keyword where a
// parameter name belongs is a parse error, and the message names the
// word as reserved. `_`-prefix keeps dastest from auto-running it;
// the `expect` directive makes lint skip it.
expect 30151

options gen2

def f(var class : int) {
    return class
}

[export]
def main() {
    print("{f(1)}")
}
