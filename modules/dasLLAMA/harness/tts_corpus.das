options gen2
options indenting = 4

module tts_corpus shared public

require daslib/fio
require daslib/json_boost

//! The 200-sentence g2p corpus every TTS rig and cell reads (`id`, `text`, `ps_espeak` per sentence),
//! minted by `harness/mint_tts_g2p_fixture.py`.
def tts_corpus_path() : string => path_join(get_das_root(), "modules/dasLLAMA/tests/_tts_fixtures/g2p_corpus.json")

//! One sentence of the corpus a synthesis speaks.
struct TtsSentence {
    id : string
    text : string
}

//! The corpus head: the first `n` sentences that carry an id and a text, in corpus order; fewer
//! where the corpus runs short, none where it does not parse.
def tts_corpus_head(json_text : string; n : int) : array<TtsSentence> {
    var out : array<TtsSentence>
    var err = ""
    let rows = read_json(json_text, err)
    return <- out if (rows == null || !(rows.value is _array))
    for (r in rows.value as _array) {
        break if (length(out) >= n)
        let id = r?["id"] ?? ""
        let text = r?["text"] ?? ""
        continue if (empty(id) || empty(text))
        out |> push(TtsSentence(id = id, text = text))
    }
    return <- out
}
