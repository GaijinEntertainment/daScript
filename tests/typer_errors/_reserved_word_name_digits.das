// Fixture for test_reserved_word_name.das - a type keyword with
// digits in it where a parameter name belongs is a parse error, and
// the message names the word as reserved. `_`-prefix keeps dastest
// from auto-running it; the `expect` directive makes lint skip it.
expect 30151

options gen2

def f(var int2 : int) {
    return int2
}

[export]
def main() {
    print("{f(1)}")
}
