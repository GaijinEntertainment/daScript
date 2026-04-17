options gen2
options indenting = 4
options persistent_heap

// superimpose stacks the original pattern with a transformed copy.
// Hear: melody in root position AND a fifth up simultaneously — parallel harmony.
/*
note("c4 e4 g4 c5").s("sine")
  .superimpose(x => x.add(7))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 c5", "sine") |> superimpose(@(var x : Pattern) => transpose(x, 7.0))
    play_feature_cps(pat, 0.5lf)
}
