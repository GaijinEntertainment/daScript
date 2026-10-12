//fmt:ignore-file
options gen2
options auto_inline_functions = false
options _ascii_strings = true

require _fixture_lint_fix_leaf
require daslib/rtti  // a comment keeps the line

class Counter {
    n : int

    def bump() : int {
        n++
        return n
    }

    def twice() : int {
        let a = bump()
        let b = bump()
        return a + b
    }
}

def leaf() : int => lint_fix_leaf_value()

def dash() : string => "a\xE2\x80\x94b"

def dash_in_builder(n : int) : string => "{n}\xE2\x80\x94{n}"
