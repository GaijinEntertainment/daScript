options gen2
options _dasllama_internal = true

module _tts_corpus_gb shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/fio

//! One sentence of the British half of the TTS front-end fixture
//! (`_tts_fixtures/g2p_corpus_gb.json`, minted by `harness/mint_tts_g2p_gb_fixture.py`): the
//! same normalized text as the American fixture, the reference front end's British phonemes,
//! and whether any word of the row reached the reference's espeak fallback - our own
//! out-of-vocabulary chain is American rewritten into the British inventory, so an `oov` row
//! is not a lexicon-parity row.
struct TtsGbRow {
    id : string
    norm : string
    ps_gb : string
    oov : bool
}

//! An out-of-vocabulary probe word with what the reference's espeak answers in each dialect
struct TtsGbOov {
    word : string
    ps_us_ref : string
    ps_gb_ref : string
}

//! The fixture: the British inventory read off `gb_gold.json`, the rows, and the probe words
struct TtsGbCorpus {
    gb_vocab : string
    rows : array<TtsGbRow>
    oov_words : array<TtsGbOov>
}

def tts_corpus_gb_path() : string => "{get_das_root()}/modules/dasLLAMA/tests/_tts_fixtures/g2p_corpus_gb.json"

//! The whole British fixture. Panics on a missing or malformed file: the fixture is checked
//! in, so its absence is a defect, never a skip.
def load_tts_corpus_gb() : TtsGbCorpus {
    let path = tts_corpus_gb_path()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("tts British corpus fixture missing: {path}")
    }
    let text = fread(f)
    fclose(f)
    var corpus = TtsGbCorpus()
    if (!sscan_json(text, corpus)) {
        panic("tts British corpus fixture does not parse: {path}")
    }
    if (length(corpus.rows) != 200) {
        panic("tts British corpus fixture has {length(corpus.rows)} rows, expected 200")
    }
    if (empty(corpus.gb_vocab) || empty(corpus.oov_words)) {
        panic("tts British corpus fixture carries no inventory or no probe words: {path}")
    }
    return <- corpus
}
