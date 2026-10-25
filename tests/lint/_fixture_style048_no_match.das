options gen2

// STYLE048 fixture: the same chain _fixture_style048_hints.das
// reports, in a module that does not require daslib/match - the
// rule stays silent here.

enum Mode {
    idle
    run
    stop
}

def enum_chain(m : Mode) : int {
    var r = 0
    if (m == Mode.idle) {
        r = 1
    } elif (m == Mode.run) {
        r = 2
    } elif (m == Mode.stop) {
        r = 3
    }
    return r
}
