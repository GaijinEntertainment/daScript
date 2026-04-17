options gen2
options indenting = 4
options persistent_heap

// palindrome alternates forward and reversed cycles.
// Hear: ascending scale on cycle 0/2/4, descending scale on cycle 1/3/5.
/*
note("c4 e4 g4 c5").s("sine")
  .palindrome()
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 c5", "sine") |> palindrome
    play_feature_cps(pat, 0.5lf)
}
