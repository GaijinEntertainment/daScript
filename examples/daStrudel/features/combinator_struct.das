options gen2
options indenting = 4
options persistent_heap

// struct_ restructures a pattern using a boolean mask pattern.
// Hear: single note c4, played only on the rhythm specified by the boolean pattern.
// struct_ replaces the pattern's timing with the mask's timing.
/*
note("c4").s("sine")
  .struct("1 0 1 1 0 1 0 1")
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4", "sine") |> struct_(s("1 ~ 1 1 ~ 1 ~ 1"))
    play_feature_cps(pat, 0.5lf)
}
