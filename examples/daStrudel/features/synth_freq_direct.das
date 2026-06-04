options gen2
options indenting = 4
options persistent_heap

// freq(hz) sets the oscillator frequency directly in Hz, bypassing the MIDI
// note → frequency conversion. A frequency pattern drives an arpeggio in raw Hz.
// Hear: a sawtooth stepping through 220 / 277 / 330 / 440 Hz (an A-major-ish chord).
/*
s("sawtooth*4").freq("220 277 330 440")
  .lpf(2500).release(0.2)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- (
        s("sawtooth*4")
        |> freq(note("220 277 330 440"))
        |> lpf(2500) |> release(0.2) |> gain(0.5)
    )
    play_feature_cps(pat, 0.5lf)
}
