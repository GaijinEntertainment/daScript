//! A macro that injects a let expression with a null variable entry
//! yields a malformed tree that the compiler reports as an
//! internal-variable error.
options gen2
// internal_variable — malformed AST, let expression has a null
// variable entry
expect 50640

require _malformed_tree_macros

[export]
def main {
    for (i in nullvar_range(0, 10)) {
        print("{i}\n")
    }
}
