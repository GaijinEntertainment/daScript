//! A @template_gate of terms joined by & in which one term names no
//! bool axis must not compile (20800).
options gen2
expect 20800

require daslib/typemacro_boost

[ |> template_struct_instance]
struct template GT {
    x : KT
    @template_gate = "BATCHED&!NO_SUCH_AXIS" y : KT
    @template_constant BATCHED : bool = true
}

struct GInst : GT {
    typedef KT = int
}

[export]
def main {
}
