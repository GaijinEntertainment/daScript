options gen2
options indenting = 4
options persistent_heap

// chop(n) subdivides each event in place into n back-to-back sample slices.
// Unlike striate (which spreads slices across the whole cycle), chop keeps the
// slices inside each event's own time slot.
// Hear: one gong sample walked through in 8 consecutive grains.
/*
s("gong").chop(8)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    let pat <- s("gong") |> chop(8) |> gain(0.6)
    play_feature_cps(pat, 0.5lf) {
        // index :0 is gong.wav (audio/ sorted alphabetically)
        strudel_load_sound("{media}/audio", "gong")
    }
}
