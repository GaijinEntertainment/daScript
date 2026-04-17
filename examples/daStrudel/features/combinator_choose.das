options gen2
options indenting = 4
options persistent_heap

// choose randomly picks one pattern per cycle.
// Hear: each cycle plays a different melodic phrase chosen at random.
// Note: choose performs a discrete per-cycle pick (alias: chooseCycles).
/*
chooseCycles(
  note("c4 e4 g4").s("sine"),
  note("d4 f4 a4").s("sine"),
  note("e4 g4 b4").s("sine")
).cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- chooseCycles([
        note("c4 e4 g4", "sine"),
        note("d4 f4 a4", "sine"),
        note("e4 g4 b4", "sine")
    ])
    play_feature_cps(pat, 0.5lf)
}
