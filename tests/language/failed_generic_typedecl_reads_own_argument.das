//! A generic argument declared as typedecl of another argument of the
//! same generic can match no call and must not compile (31330),
//! called or not.
options gen2
expect 31330

def own_argument_typedecl(_a : auto(TT); _b : typedecl(_a)) : int => 1

[export]
def main {
}
