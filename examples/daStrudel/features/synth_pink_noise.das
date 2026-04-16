options gen2
options indenting = 4
options persistent_heap

// Pink noise through a low-pass filter at 300Hz.
// Hear: gentle ocean hiss, warm and dark.
/*
s("pink")
  .sustain(1)
  .lpf(300)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- atom("pink") |> set_sustain(1.0) |> set_lpf(300.0)
    play_feature_cps(pat, 0.5lf)
}
