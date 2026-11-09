//! A tuple literal with more elements than the declared tuple result
//! must report ordinary unresolved-type errors, never read past the
//! result's element types while seeding its empty array literal.
options gen2
expect 31312, 31329, 31302, 31327

def two() : tuple<array<int>; int> {
    let k = 1
    return ([], k, 3)
}

[export]
def main() {
    let _t <- two()
}
