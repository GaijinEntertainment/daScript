//! A @template_constant overridden with a non-constant expression
//! must not compile (20800).
options gen2
expect 20800

require daslib/typemacro_boost

def side_value : int {
    return 13
}

[ |> template_struct_instance]
struct template CT {
    x : KT
    @template_constant N : int = 1
}

struct CInst : CT {
    typedef KT = int
    override N = side_value()
}

[export]
def main {
}
