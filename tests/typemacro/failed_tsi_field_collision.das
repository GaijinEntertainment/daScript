//! An instance that redeclares an inherited template field without
//! override must not compile (20503).
options gen2
expect 20503

require daslib/typemacro_boost

[ |> template_struct_instance]
struct template FT {
    x : KT
    n : int = 1
}

struct FInst : FT {
    typedef KT = int
    // redeclared without `override` — the parser rejects this before the
    // reifier runs
    n : int = 2
}

[export]
def main {
}
