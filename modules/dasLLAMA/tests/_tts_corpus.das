options gen2
options _dasllama_internal = true

module _tts_corpus shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/fio

//! One sentence of the TTS front-end fixture (`_tts_fixtures/g2p_corpus.json`, minted by
//! `harness/mint_tts_g2p_fixture.py`): the raw text, the reference-normalized text every arm
//! was fed, arm E's phonemes in both inventories, spaCy's tokens with PTB tags and trailing
//! whitespace, and for the heteronym block the two readings that must both appear.
struct TtsHeteronym {
    word : string
    form1 : string
    form2 : string
}

struct TtsCorpusRow {
    id : string
    category : string
    text : string
    norm : string
    expected : string   // the hand-corrected spoken form where `norm` is not what a person says; "" elsewhere
    ps_misaki : string
    ps_espeak : string
    tokens : array<array<string>>
    heteronym : TtsHeteronym
}

def tts_corpus_path() : string => "{get_das_root()}/modules/dasLLAMA/tests/_tts_fixtures/g2p_corpus.json"

//! The whole fixture, in file order. Panics on a missing or malformed file: the fixture is
//! checked in, so its absence is a defect, never a skip.
def load_tts_corpus() : array<TtsCorpusRow> {
    let path = tts_corpus_path()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("tts corpus fixture missing: {path}")
    }
    let text = fread(f)
    fclose(f)
    var rows : array<TtsCorpusRow>
    if (!sscan_json(text, rows)) {
        panic("tts corpus fixture does not parse: {path}")
    }
    if (length(rows) != 200) {
        panic("tts corpus fixture has {length(rows)} rows, expected 200")
    }
    return <- rows
}
