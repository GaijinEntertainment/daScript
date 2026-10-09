//fmt:ignore-file
options gen2
options auto_inline_functions = false
options _ascii_strings = true

require _fixture_lint_fix_proxy
require daslib/strings_boost
require daslib/rtti  // a comment keeps the line

class Counter {
    n : int

    def bump() : int {
        n++
        return n
    }

    def twice() : int {
        let a = self->bump()
        let b = self -> bump()
        return a + b
    }
}

def leaf() : int => lint_fix_leaf_value()

def dash() : string => "a—b"

def dash_in_builder(n : int) : string => "{n}—{n}"
