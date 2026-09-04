// Shapes the fuzzer reaches that no hand-written test does. Every block below compiles
// clean and must keep compiling: the file carries no assertions, because a regression is a
// compiler crash or a spurious error, not a wrong value.
options gen2

// A template structure's typedef holds a block in the dim expression of its own alias. The
// type expressions of a template are never inferred, so that block reaches every post-infer
// visitor with no type and outside of any function - a visitor keying off its enclosing
// function reads a null one.
struct template r {
    def v => 0
    typedef r = r<iterator<a>[ @ {0 = (0, C())}]>
}

[export]
def main {
}
