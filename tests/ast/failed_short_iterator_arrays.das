//! A macro that leaves the for loop's iteratorsAt and iteratorsAka
//! shorter than its iterators yields a malformed tree that the
//! compiler reports as an internal-expression error.
options gen2
// internal_expression — malformed AST, iteratorsAt/iteratorsAka
// shorter than iterators
expect 50607

require _malformed_tree_macros

[export]
def main {
    for (i in short_iters_range(0, 10)) {
        print("{i}\n")
    }
}
