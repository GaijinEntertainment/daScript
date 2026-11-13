//! A generic argument whose dimension reads the generic's own TT can
//! match no call and must not compile (30109, 30837), called or not.
options gen2
expect 30109, 30837

def own_type_dim(_a : auto(TT); _b : int[typeinfo sizeof(type<TT>)]) : int => 1

[export]
def main {
}
