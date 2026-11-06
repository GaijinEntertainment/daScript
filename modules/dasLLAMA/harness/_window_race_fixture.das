// The window races' shared fixture: the counting prompt both window race beds read.

options gen2
options _dasllama_internal = true

require strings
require dasllama/dasllama   // encode

//! `n` tokens of a counting text from `seed`, the BOS first: two seeds make two prompts of the same length
def counting_tokens(tr : Model; n, seed : int64) : array<int64> {
    let text = build_string() $(var w : StringBuilderWriter) {
        var i = seed
        var chars = 0
        while (chars < int(n) * 6) {   //! a number and its comma is two to four tokens; the encode is cut to n
            let piece = "{i}, "
            w |> write(piece)
            chars += length(piece)
            i ++
        }
    }
    var all <- encode(tr, text, true, false)
    var toks <- [for (k in range64(n)); all[k]]
    delete all
    return <- toks
}
