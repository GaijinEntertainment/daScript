options gen2
options indenting = 4
options persistent_heap

// Pentatonic arpeggio — degrees 0-4 map to C D E G A (no dissonance).
/*
n("0 1 2 3 4").scale("C4:major:pentatonic").s("sine")
  .attack(0.001).decay(0.1).sustain(0).release(0.2)
  .room(0).delay(0)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- scale_pattern("0 1 2 3 4", "C4:major:pentatonic") |> set_attack(0.001) |> set_decay(0.1) |> set_sustain(0.0) |> set_release(0.2)
    play_feature_cps(pat, 0.5lf)
}
