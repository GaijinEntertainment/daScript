options gen2
options indenting = 4
options persistent_heap

// slice(n, idx_pat) cuts the sample into n slices and plays them in the order
// (and rhythm) given by idx_pat. Structure comes from idx_pat, the sample from
// the source. Hear: the gong reshuffled into a non-linear order.
/*
s("gong").slice(8, "0 2 4 6 1 3 5 7")
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    let pat <- s("gong") |> slice(8, note("0 2 4 6 1 3 5 7")) |> gain(0.6)
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sound("{media}/audio", "gong")
    }
}
