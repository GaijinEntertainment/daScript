options gen2
options indenting = 4
options persistent_heap

// mask keeps events only where the mask pattern has active events.
// Hear: melody plays, but muted on beats 2 and 4 (controlled by the mask).
/*
note("c4 d4 e4 f4").s("sine")
  .mask("1 0 1 0")
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 d4 e4 f4", "sine") |> mask(s("1 ~ 1 ~"))
    play_feature_cps(pat, 0.5lf)
}
