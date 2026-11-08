options gen2
options _dasllama_internal = true

module _tts_corpus shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/fio
require ../harness/tts_corpus.das public   // tts_corpus_path - the one path every TTS reader takes
require _json_file public   // load_json_file

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

//! The rows of a checked-in fixture, `n` of them. Panics on a missing or malformed file: the fixture
//! is checked in, so its absence is a defect, never a skip.
def load_tts_fixture_rows(path, kind : string; n : int; var rows : array<auto>) {
    if (!load_json_file(path, rows)) {
        panic("tts {kind} fixture missing: {path}")
    }
    if (length(rows) != n) {
        panic("tts {kind} fixture has {length(rows)} rows, expected {n}")
    }
}

//! The whole fixture, in file order.
def load_tts_corpus() : array<TtsCorpusRow> {
    var rows : array<TtsCorpusRow>
    load_tts_fixture_rows(tts_corpus_path(), "corpus", 200, rows)
    return <- rows
}
