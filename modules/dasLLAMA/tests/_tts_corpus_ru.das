options gen2
options _dasllama_internal = true

module _tts_corpus_ru shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require _tts_corpus   // tts_fixture_path, load_tts_fixture_rows

//! One sentence of the Russian front-end fixture (`_tts_fixtures/g2p_corpus_ru.json`, minted by
//! `harness/mint_tts_g2p_ru_fixture.py`): the plain text, the same text with RUAccent's `+`
//! before every stressed vowel, and the phonemes kokoro-ru's own front end reads the marked
//! text as - letter-to-sound alone, no stress decision of its own.
struct TtsRuCorpusRow {
    id : string
    text : string
    marked : string
    ps : string
}

//! The whole fixture, in file order.
def load_tts_corpus_ru() : array<TtsRuCorpusRow> {
    var rows : array<TtsRuCorpusRow>
    load_tts_fixture_rows(tts_fixture_path("g2p_corpus_ru.json"), "russian corpus", 200, rows)
    return <- rows
}
