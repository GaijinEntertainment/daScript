//! A 16-bit lattice vector such as half4 must not be accepted as a
//! table key in a declaration (30254).
options gen2

expect 30254

[export]
def main {
    // 30254: a 16-bit lattice vector is not a hashable key, in a
    // declaration as in a literal
    var tab : table<half4; int>
}
