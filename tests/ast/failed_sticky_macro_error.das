//! A macro_sticky_error raised on an otherwise valid tree still fails
//! the compile.
options gen2
// runtime_macro — macro_sticky_error on a VALID tree must still fail
// the compile
expect 50503

require _malformed_tree_macros

[export]
def main {
    for (i in sticky_range(0, 10)) {
        print("{i}\n")
    }
}
