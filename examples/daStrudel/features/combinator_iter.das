options gen2
options indenting = 4
options persistent_heap

// iter shifts the pattern by 1/n each cycle.
// Hear: 4-note phrase that rotates its starting point each cycle (c-e-g-a, then e-g-a-c, then g-a-c-e...).
/*
note("c4 e4 g4 a4").s("sine")
  .iter(4)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 a4", "sine") |> iter(4)
    play_feature_cps(pat, 0.5lf)
}
