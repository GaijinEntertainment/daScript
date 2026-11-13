//! a debug() of an undeclared name in the finally of a generator
//! that dimensions a self-referencing tuple does not compile, and
//! inference does not crash
options gen2
expect 30109:2, 30503:3, 30504, 30838

tuple T {
    a : int[T()] | int[{ generator<uint>() <| $ { pass } finally { debug(l) } }]
}

[export]
def main {
    pass
}
