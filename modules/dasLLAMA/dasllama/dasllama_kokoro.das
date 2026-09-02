options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_kokoro shared public

require dasllama/dasllama_lint public
require math
require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_unicode

//! The Kokoro family (Kokoro-82M): the reference pipeline's symbol table (the front end's own
//! inventory - no rewrite), its token wrapping and style-row rule. The model itself is the
//! shared StyleTTS2 assembly; the voice packs carry one style row per phoneme count.

//! Kokoro's driver data beside the shared model: codepoint -> token id.
struct KokoroFamily {
    sym : table<int; int>
}

//! Read the `kokoro.*` metadata of a converted Kokoro GGUF.
def load_kokoro_family(path : string) : KokoroFamily {
    var fam = KokoroFamily()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA kokoro: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        let cpts <- gguf_int_array(m, bytes, "kokoro.symbol_cpts")
        let ids <- gguf_int_array(m, bytes, "kokoro.symbol_ids")
        for (c, i in cpts, ids) {
            fam.sym[int(c)] = int(i)
        }
    }
    fclose(f)
    return <- fam
}

//! The token ids the model reads for a phoneme string in the front end's inventory: every
//! symbol the vocabulary knows, in order, wrapped in the pad token on both sides.
def kokoro_token_ids(fam : KokoroFamily; ps : string) : array<int> {
    var inscope cps <- utf8_to_cpts(ps)
    var ids : array<int>
    ids |> reserve(length(cps) + 2)
    ids |> push(0)
    for (c in cps) {
        let id = fam.sym?[c] ?? -1
        if (id >= 0) {
            ids |> push(id)
        }
    }
    ids |> push(0)
    return <- ids
}

//! The style row the reference pipeline picks: the phoneme string's character count, less
//! one, capped at the last row.
def kokoro_style_row(ps : string; rows : int) : int {
    var inscope cps <- utf8_to_cpts(ps)
    return clamp(length(cps) - 1, 0, rows - 1)
}
