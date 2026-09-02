options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_kitten shared public

require dasllama/dasllama_lint public
require strings

//! The KittenTTS family (nano, mini): what is specific to how these models read phonemes.
//! The models were trained on espeak-style IPA, so the front end's inventory is rewritten
//! into that symbol space here - the diphthong letters expand, the flap and the schwa
//! superscript map, and the calibrated length and rhotic rules apply (settled by hill-climbing
//! agreement with the training-time front end on held-out words).

//! Rewrite phonemes from the front end's inventory into the espeak-style IPA these models
//! consume. Sequential rewrites, in this order: the diphthong and consonant letters, then
//! the calibrated length and rhotic rules.
def kitten_phonemes_from_misaki(ps : string) : string {
    var s = ps
    s = replace(s, "ʤ", "dʒ")   // nolint:STYLE039
    s = replace(s, "ʧ", "tʃ")   // nolint:STYLE039
    s = replace(s, "A", "eɪ")   // nolint:STYLE039
    s = replace(s, "I", "aɪ")   // nolint:STYLE039
    s = replace(s, "Y", "ɔɪ")   // nolint:STYLE039
    s = replace(s, "O", "oʊ")   // nolint:STYLE039
    s = replace(s, "Q", "əʊ")   // nolint:STYLE039
    s = replace(s, "W", "aʊ")   // nolint:STYLE039
    s = replace(s, "T", "ɾ")   // nolint:STYLE039
    s = replace(s, "ᵊ", "ə")   // nolint:STYLE039
    // the calibrated rules: length marks espeak keeps, the rhotic vowel spellings
    s = replace(s, "ɜɹ", "ɜːɹ")   // nolint:STYLE039
    s = replace(s, "ɑ", "ɑː")   // nolint:STYLE039
    s = replace(s, "ɔ", "ɔː")   // nolint:STYLE039
    s = replace(s, "u", "uː")   // nolint:STYLE039
    s = replace(s, "əɹ", "ɚ")   // nolint:STYLE039
    s = replace(s, "ɜːɹ", "ɜː")   // nolint:STYLE039
    return s
}
