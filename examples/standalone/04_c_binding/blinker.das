options gen2
options stack = 16384

// Tier A + output. This is the shape an embedded program actually has: C owns
// the hardware and the loop, daslang owns the decision. Each tick the C side
// asks for a lamp pattern and writes whatever comes back to its GPIO register.
//
// `print` is the one call here that leaves the runtime. On a host it reaches
// stdout; on a board it reaches whatever das_nano_set_print was pointed at.
// Nothing else in nano does I/O.

let LAMP_COUNT = 4

// A Larson scanner: one lit lamp bouncing between the ends.
[export]
def lamp_pattern(tick : int) : int {
    let period = (LAMP_COUNT - 1) * 2
    let phase = tick % period
    let position = phase < LAMP_COUNT ? phase : period - phase
    return 1 << position
}

[export]
def announce(tick, pattern : int) {
    print("tick {tick}: lamps {pattern}\n")
}
