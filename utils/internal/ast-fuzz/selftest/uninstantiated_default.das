options gen2
require daslib/ast_verify
require uninstantiated_default_dep

// an unconstructed struct's ctor field default, CROSS-module: infer types the decl init but
// never binds its ctor func (nothing generates the foreign ctor here) - the post-infer walk
// must not report it; the same-module shape resolves eagerly and never had the hole
struct Outer {
    inner : Inner = Inner()
}

[export]
def main {
    print("uninstantiated-default-ok\n")
}
