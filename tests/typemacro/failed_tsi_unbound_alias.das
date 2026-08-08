options gen2
expect 20800

require daslib/typemacro_boost

[ |> template_struct_instance]
struct template NT {
    x : KT
    y : VT
}

struct NInst : NT {
    typedef KT = int
    // VT deliberately unbound: the reifier must name the missing typedef
}

[export]
def main {
}
