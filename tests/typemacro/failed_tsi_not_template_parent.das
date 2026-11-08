//! template_struct_instance on a struct whose parent is not a
//! template must not compile (20800).
options gen2
expect 20800

require daslib/typemacro_boost

struct Plain {
    x : int
}

[template_struct_instance]
struct Inst : Plain {
    y : int
}

[export]
def main {
}
