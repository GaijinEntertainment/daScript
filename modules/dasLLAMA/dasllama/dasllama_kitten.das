options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_kitten shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_common
require dasllama/dasllama_unicode
require dasllama/dasllama_styletts2

//! The KittenTTS family (nano, mini): the reference driver's token table, re-spacing and
//! style-row rules, its speed priors and voice aliases, its tail trim, and the rewrite of the
//! front end's inventory into the espeak-style IPA these models consume. The model itself is
//! the shared StyleTTS2 assembly.

// ===== the reference driver's symbol table and token rule =====

let private KITTEN_SYMBOL_CPTS = fixed_array<int>(36, 59, 58, 44, 46, 33, 63, 161, 191, 8212, 8230, 171, 187, 34, 32, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 593, 592, 594, 230, 595, 665, 946, 596, 597, 231, 599, 598, 240, 676, 601, 600, 602, 603, 604, 605, 606, 607, 644, 609, 608, 610, 667, 614, 615, 295, 613, 668, 616, 618, 669, 621, 620, 619, 622, 671, 625, 623, 624, 331, 627, 626, 628, 248, 629, 632, 952, 339, 630, 664, 633, 634, 638, 635, 640, 641, 637, 642, 643, 648, 679, 649, 650, 651, 11377, 652, 611, 612, 653, 967, 654, 655, 657, 656, 658, 660, 673, 661, 674, 448, 449, 450, 451, 712, 716, 720, 721, 700, 692, 688, 689, 690, 695, 736, 740, 734, 8595, 8593, 8594, 8599, 8600, 809, 39, 7547)
let private KITTEN_SYMBOL_IDS = fixed_array<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 175, 176, 177)

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

let private ELLIPSIS_CPT = 8230   // the driver closes every sequence with an ellipsis

def private kitten_word_cpt(c : int) : bool {
    return cpt_is_letter(c) || cpt_is_number(c) || c == '_'
}

//! The token ids the model reads for an espeak-inventory phoneme string: the reference
//! driver's re-spacing (runs of word characters and single punctuation marks, one space
//! between), unknown symbols dropped, wrapped in pad / ellipsis / pad.
def kitten_token_ids(ps : string) : array<int> {
    var inscope sym <- {for (c, i in KITTEN_SYMBOL_CPTS, KITTEN_SYMBOL_IDS); c => i}
    var inscope cps <- utf8_to_cpts(ps)
    var inscope ids : array<int>
    ids |> push(0)
    let space_id = sym?[' '] ?? 0
    var in_word = false
    var first = true
    for (c in cps) {
        if (c == ' ' || c == '\t' || c == '\n') {
            in_word = false
            continue
        }
        // an unknown phoneme takes its separator with it - a lone space token between two
        // words the model never saw a space between
        let id = sym?[c] ?? -1
        continue if (id < 0)
        let word = kitten_word_cpt(c)
        if (!first && !(word && in_word)) {
            ids |> push(space_id)
        }
        first = false
        in_word = word
        ids |> push(id)
    }
    ids |> push(sym?[ELLIPSIS_CPT] ?? 0)
    ids |> push(0)
    return <- ids
}

//! The style row the reference driver picks: the chunk's character count, clamped to the pack's
//! rows. The loader guarantees at least one row; the lower bound is what keeps a pack that
//! declared none from indexing behind the first.
def kitten_style_row(norm_text : string; rows : int) : int {
    var inscope cps <- utf8_to_cpts(norm_text)
    return clamp(length(cps), 0, rows - 1)
}

//! Kitten's driver data beside the shared model: per-voice speed priors and the alias names.
struct KittenFamily {
    speed_priors : table<string; float>
    aliases : table<string; string>
}

//! Read the `kitten.*` metadata of a converted KittenTTS GGUF.
def load_kitten_family(path : string) : KittenFamily {
    var inscope fam = KittenFamily()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA kitten: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        var inscope voices <- gguf_str_array(m, bytes, "styletts2.voices")
        for (v in voices) {
            if (gguf_has(m, "kitten.speed_prior.{v}")) {
                fam.speed_priors[v] = gguf_f32(m, bytes, "kitten.speed_prior.{v}")
            }
        }
        var inscope anames <- gguf_str_array(m, bytes, "kitten.alias_names")
        var inscope avoices <- gguf_str_array(m, bytes, "kitten.alias_voices")
        for (an, av in anames, avoices) {
            fam.aliases[an] = av
        }
    }
    fclose(f)
    return <- fam
}

//! Resolve a voice name or alias to a canonical voice; "" when unknown.
def kitten_voice(fam : KittenFamily; m : StyleTts2Model; name : string) : string {
    if (styletts2_has_voice(m, name)) {
        return name
    }
    return fam.aliases?[name] ?? ""
}

let private TRIM_TAIL = 5000l

//! The reference driver's tail trim: a synthesis ends with 5000 samples of silence.
def kitten_trim(var wave : array<float>) {
    let n = long_length(wave)
    if (n > TRIM_TAIL) {
        wave |> resize(n - TRIM_TAIL)
    }
}
