options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options _ascii_strings = false   // the Russian word lists and the IPA inventory are the data

module dasllama_g2p_ru shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/strings_boost
require daslib/utf8_utils
require dasllama/dasllama_unicode
require dasllama/dasllama_g2p_pack

//! The Russian front end: stress, then letter-to-sound. Stress comes from the text (`+` before
//! the stressed vowel or U+0301 after it, which always wins), else from the pack `tts_g2p_ru.bin`
//! (harness/build_g2p_ru_data.py: every inflected form's stressed vowel and the ё spellings a text
//! written without ё needs; a form with two readings carries both, the first the default), else
//! from a rule over the word's shape. Letter-to-sound turns the stressed, lowercase words into the
//! IPA label shape the kokoro-ru checkpoints were trained on - palatalization, iotation, voicing
//! assimilation and final devoicing, the positional vowel reduction (ɐ in the first pretonic
//! syllable and word-initially, ə elsewhere, ɪ after a soft consonant), the orthoepic respellings
//! (-ого as -ово, the чн words said with шн, the silent clusters), long щ.

let private CYR_A = 1072
let private CYR_B = 1073
let private CYR_V = 1074
let private CYR_G = 1075
let private CYR_D = 1076
let private CYR_E = 1077
let private CYR_ZH = 1078
let private CYR_Z = 1079
let private CYR_I = 1080
let private CYR_J = 1081
let private CYR_K = 1082
let private CYR_L = 1083
let private CYR_M = 1084
let private CYR_N = 1085
let private CYR_O = 1086
let private CYR_P = 1087
let private CYR_R = 1088
let private CYR_S = 1089
let private CYR_T = 1090
let private CYR_U = 1091
let private CYR_F = 1092
let private CYR_KH = 1093
let private CYR_TS = 1094
let private CYR_CH = 1095
let private CYR_SH = 1096
let private CYR_SHCH = 1097
let private CYR_HARD = 1098
let private CYR_Y = 1099
let private CYR_SOFT = 1100
let private CYR_EH = 1101
let private CYR_YU = 1102
let private CYR_YA = 1103
let private CYR_YO = 1105
let private CYR_YO_CAP = 1025
let private ACUTE = 769
let private PLUS = '+'
let private GUILLEMET_OPEN = 171
let private GUILLEMET_CLOSE = 187
let private EM_DASH = 8212
let ELLIPSIS = 8230
//! a final ч the next word voices: the one sound the letter table has no letter for
let private VOICED_CH = -2

let private RU_VOWELS <- { CYR_A, CYR_E, CYR_YO, CYR_I, CYR_O, CYR_U, CYR_Y, CYR_EH, CYR_YU, CYR_YA }
let private RU_SOFTENING_VOWELS <- { CYR_E, CYR_YO, CYR_I, CYR_YU, CYR_YA }
//! the vowels spelled with an onset j after a vowel, a sign or at the word start
let private RU_IOTATED_VOWELS <- { CYR_E, CYR_YO, CYR_YU, CYR_YA }
let private RU_ALWAYS_HARD <- { CYR_ZH, CYR_SH, CYR_TS }
let private RU_ALWAYS_SOFT <- { CYR_CH, CYR_SHCH, CYR_J }
//! voiced obstruents that voice a consonant before them (в does not)
let private RU_VOICING <- { CYR_B, CYR_D, CYR_G, CYR_Z, CYR_ZH }
let private RU_VOICELESS <- { CYR_P, CYR_T, CYR_K, CYR_S, CYR_SH, CYR_F, CYR_KH, CYR_TS, CYR_CH, CYR_SHCH }
let private RU_SONORANT_LETTERS <- { CYR_L, CYR_M, CYR_N, CYR_R, CYR_J }
let private RU_DEVOICE <- { CYR_B => CYR_P, CYR_D => CYR_T, CYR_G => CYR_K, CYR_Z => CYR_S, CYR_ZH => CYR_SH, CYR_V => CYR_F }
let private RU_VOICE <- { CYR_P => CYR_B, CYR_T => CYR_D, CYR_K => CYR_G, CYR_S => CYR_Z, CYR_SH => CYR_ZH, CYR_F => CYR_V }
//! the consonants ь leaves hard in the reference's reading (р reads as its own sound, emit_consonant)
let private RU_HARD_BEFORE_SIGN <- { CYR_L, CYR_N, CYR_R, CYR_B, CYR_P, CYR_V, CYR_F }
let private RU_LABIAL <- { CYR_B, CYR_P, CYR_V, CYR_F }
let private HYPHEN_CLITICS <- { "из", "в", "во", "с", "со", "к", "ко", "по", "на", "за", "под", "над", "об", "от", "до", "у", "о", "не", "ни", "же", "бы", "ли" }
let private HYPHEN_PARTICLES <- { "либо" => "л+ибо", "нибудь" => "ниб+удь", "таки" => "так+и" }

// the IPA pieces, built from codepoints so the string literals that splice them stay ASCII
let private IPA_PAL = cpt_to_utf8(690)        // ʲ
let private IPA_STRESS = cpt_to_utf8(712)     // ˈ
let private IPA_LONG = cpt_to_utf8(720)       // ː
let private IPA_SCHWA = cpt_to_utf8(601)      // ə
let private IPA_TURNED_A = cpt_to_utf8(592)   // ɐ
let private IPA_SMALL_I = cpt_to_utf8(618)    // ɪ
let private IPA_SCRIPT_A = cpt_to_utf8(593)   // ɑ
let private IPA_EPS = cpt_to_utf8(603)        // ɛ
let private IPA_G = cpt_to_utf8(609)          // ɡ
let private IPA_ESH = cpt_to_utf8(643)        // ʃ
let private IPA_EZH = cpt_to_utf8(658)        // ʒ
let private IPA_TESH = cpt_to_utf8(679)       // ʧ
let private IPA_TS = cpt_to_utf8(678)         // ʦ
let private IPA_SCH = cpt_to_utf8(597)        // ɕ
let private IPA_DEZH = cpt_to_utf8(676)       // ʤ

//! the hard consonant symbols; a soft one appends IPA_PAL
let private HARD_CONSONANT_IPA <- {
    CYR_B => "b", CYR_V => "v", CYR_G => IPA_G, CYR_D => "d", CYR_ZH => IPA_EZH, CYR_Z => "z",
    CYR_J => "j", CYR_K => "k", CYR_L => "l", CYR_M => "m", CYR_N => "n", CYR_P => "p",
    CYR_R => "r", CYR_S => "s", CYR_T => "t", CYR_F => "f", CYR_KH => "x", CYR_TS => IPA_TS,
    CYR_CH => IPA_TESH, CYR_SH => IPA_ESH, CYR_SHCH => IPA_SCH + IPA_LONG
}

//! the words said other than written: -ого adverbs whose г stays and whose last vowel reads а, the чн words said with шн,
//! что- as што-, сегодня with в, интернет with a hard т; a value keeps its key's vowel count, the stress moving by vowel ordinal
let private RESPELL_WORD <- {
    "дорого" => "дорога", "недорого" => "недорога", "настрого" => "настрога",
    "конечно" => "конешно", "скучно" => "скушно", "скучный" => "скушный", "нарочно" => "нарошно",
    "яичница" => "яишница", "скворечник" => "скворешник", "девичник" => "девишник",
    "двоечник" => "двоешник", "троечник" => "троешник", "прачечная" => "прашечная",
    "горчичник" => "горчишник", "пустячный" => "пустяшный",
    "что" => "што", "чтобы" => "штобы", "чтоб" => "штоб",
    "сегодня" => "севодня", "сегодняшний" => "севодняшний",
    "интернет" => "интэрнет", "интернета" => "интэрнета", "интернете" => "интэрнете", "интернету" => "интэрнету", "интернетом" => "интэрнетом"
}

//! the adverbs and names whose -ого keeps its г; a word RESPELL_WORD carries is respelled before this is read
let private OGO_KEEP_G <- { "много", "немного", "намного", "ненамного", "строго", "нестрого", "полого", "убого", "лего", "диего", "ого", "огого" }

let private VOICED_PREPOSITIONS <- { "в", "об" }

//! What opens the next word, for the sandhi on this word's last consonant
enum NextWord {
    none          //! nothing, punctuation, or a voiceless consonant
    voiced        //! a voiced obstruent: б д г з ж
    sonorant      //! л м н р й: a preposition's final consonant keeps its voice
    vowel         //! a vowel: a preposition's final consonant keeps its voice, and a final soft л н р в keeps its softness
}

//! A word taken apart: its letters, lowercase and bare, which vowels carry stress, and per
//! letter the voicing the context gives it, whether it is soft, and whether it is silent
struct private RuWord {
    letters : array<int>
    stressed : array<bool>
    assimilated : array<int>    //! the letter after voicing assimilation, devoiced or voiced
    soft : array<bool>
    silent : array<bool>
    next_side_opens_stressed : bool      //! a hyphenated side whose next side opens on its stressed vowel
}

def private is_ru_vowel(cp : int) : bool => key_exists(RU_VOWELS, cp)

def private parse_marked(word : string) : RuWord {
    var inscope cps <- utf8_to_cpts(word)
    var w = RuWord()
    var plus = false
    for (cp in cps) {
        if (cp == PLUS) {
            plus = true
            continue
        }
        if (cp == ACUTE) {
            if (!empty(w.stressed)) {
                w.stressed[length(w.stressed) - 1] = true
            }
            continue
        }
        continue if (!cpt_is_cyrillic(cp))
        let lc = int(utf32_to_lower(uint(cp)))
        w.letters |> push(lc)
        w.stressed |> push(plus && is_ru_vowel(lc))
        plus = false
    }
    for (i, c in iter_range(w.letters), w.letters) {
        if (c == CYR_YO) {
            w.stressed[i] = true
        }
    }
    return <- w
}

def private stressed_ordinals(w : RuWord) : array<int> {
    var inscope out : array<int>
    var n = 0
    for (c, s in w.letters, w.stressed) {
        if (is_ru_vowel(c)) {
            n++
            if (s) {
                out |> push(n)
            }
        }
    }
    return <- out
}

def private reseat(var w : RuWord; respelled : string; ordinals : array<int>) {
    var inscope cps <- utf8_to_cpts(respelled)
    w.letters := cps
    w.stressed |> clear()
    w.stressed |> resize(length(cps))
    var n = 0
    for (i, c in iter_range(cps), cps) {
        if (is_ru_vowel(c)) {
            n++
            w.stressed[i] = find_index(ordinals, n) >= 0
        }
    }
}

def private respell(var w : RuWord) {
    var bare = cpts_to_utf8(w.letters)
    let whole = RESPELL_WORD?[bare] ?? ""
    if (!empty(whole)) {
        bare = whole
    } elif (!key_exists(OGO_KEEP_G, bare)) {
        // -ого / -его, bare or before the reflexive -ся
        let n = length(w.letters)
        let e = (n >= 5 && w.letters[n - 1] == CYR_YA && w.letters[n - 2] == CYR_S) ? n - 2 : n
        if (e >= 3 && w.letters[e - 1] == CYR_O && w.letters[e - 2] == CYR_G && (w.letters[e - 3] == CYR_O || w.letters[e - 3] == CYR_E)) {
            var inscope cps := w.letters
            cps[e - 2] = CYR_V
            bare = cpts_to_utf8(cps)
        } else {
            return
        }
    }
    var inscope stressed_vowels <- stressed_ordinals(w)
    reseat(w, bare, stressed_vowels)
}

//! marked, not removed, so the neighbours keep their written context for the softness and voicing passes
def private mark_silent(var w : RuWord) {
    let n = length(w.letters)
    w.silent |> resize(n)
    for (i in range(1, n - 1)) {
        let c = w.letters[i]
        let p = w.letters[i - 1]
        let nx = w.letters[i + 1]
        let nx2 = i + 2 < n ? w.letters[i + 2] : -1
        if (c == CYR_T && p == CYR_S && (nx == CYR_N || nx == CYR_L)) {
            w.silent[i] = true
        } elif (c == CYR_D && ((p == CYR_Z && nx == CYR_N) || (p == CYR_R && nx == CYR_TS))) {
            w.silent[i] = true
        } elif (c == CYR_L && nx == CYR_N && nx2 == CYR_TS) {
            w.silent[i] = true
        } elif (c == CYR_V && nx == CYR_S && nx2 == CYR_T && i + 3 < n && w.letters[i + 3] == CYR_V && i >= 1 && (p == CYR_U || p == CYR_A)) {
            let bare = cpts_to_utf8(w.letters)
            w.silent[i] = find(bare, "чувств") >= 0 || find(bare, "здравств") >= 0
        }
    }
}

//! the index of the letter after `i` that is said, skipping the signs and the silent letters;
//! -1 at the end. `skipped_sign` reports whether a sign was skipped.
def private next_said(w : RuWord; i : int; var skipped_sign : bool&) : int {
    skipped_sign = false
    var j = i + 1
    while (j < length(w.letters) && (w.letters[j] == CYR_SOFT || w.letters[j] == CYR_HARD || w.silent[j])) {
        skipped_sign = skipped_sign || w.letters[j] == CYR_SOFT || w.letters[j] == CYR_HARD
        j++
    }
    return j < length(w.letters) ? j : -1
}

//! soft by the vowel after it alone - the softness that spreads to the consonant before
def private soft_by_vowel(w : RuWord; i : int) : bool {
    let c = w.letters[i]
    return false if (key_exists(RU_ALWAYS_HARD, c))
    return i + 1 < length(w.letters) && key_exists(RU_SOFTENING_VOWELS, w.letters[i + 1])
}

def private soft_by_sign(w : RuWord; i : int) : bool => i + 1 < length(w.letters) && w.letters[i + 1] == CYR_SOFT

def private mark_soft(var w : RuWord) {
    let n = length(w.letters)
    w.soft |> resize(n)
    for (i in range(n)) {
        let c = w.letters[i]
        continue if (is_ru_vowel(c) || c == CYR_SOFT || c == CYR_HARD)
        if (key_exists(RU_ALWAYS_SOFT, c)) {
            w.soft[i] = true
            continue
        }
        continue if (key_exists(RU_ALWAYS_HARD, c))
        if (soft_by_vowel(w, i) || soft_by_sign(w, i)) {
            w.soft[i] = true
            continue
        }
        continue if (i + 1 >= n)
        let nx = w.letters[i + 1]
        let nx_vowel_soft = soft_by_vowel(w, i + 1)
        if (c == CYR_S || c == CYR_Z) {
            w.soft[i] = ((nx == CYR_T || nx == CYR_D || nx == CYR_S || nx == CYR_Z) && nx_vowel_soft) || (nx == CYR_N && (nx_vowel_soft || soft_by_sign(w, i + 1)))
            // зть reads soft where сть reads hard
            w.soft[i] = w.soft[i] || (c == CYR_Z && nx == CYR_T && soft_by_sign(w, i + 1))
        } elif (c == CYR_T) {
            w.soft[i] = (nx == CYR_T || nx == CYR_D || nx == CYR_N || nx == CYR_S || nx == CYR_Z) && nx_vowel_soft
        } elif (c == CYR_D) {
            w.soft[i] = (nx == CYR_T || nx == CYR_D || nx == CYR_S || nx == CYR_Z) && nx_vowel_soft
        } elif (c == CYR_M) {
            w.soft[i] = key_exists(RU_LABIAL, nx) && (nx_vowel_soft || soft_by_sign(w, i + 1))
        }
    }
}

def private is_obstruent(cp : int) : bool => key_exists(RU_VOICING, cp) || key_exists(RU_VOICELESS, cp) || cp == CYR_V

def private has_stress_before(w : RuWord; i : int) : bool {
    for (k in range(i)) {
        return true if (w.stressed[k])
    }
    return false
}

def private assimilate_voicing(var w : RuWord; next : NextWord; preposition : bool) {
    let n = length(w.letters)
    w.assimilated := w.letters
    var i = n - 1
    while (i >= 0) {
        let c = w.letters[i]
        i--
        continue if (!is_obstruent(c) || w.silent[i + 1])
        let at = i + 1
        var skipped_sign = false
        let j = next_said(w, at, skipped_sign)
        if (j < 0) {
            // a final вь devoices whatever follows; в ж ш ч and a final зь voice before a voiced opening
            if (next == NextWord.voiced && ((c == CYR_V && !skipped_sign) || c == CYR_ZH || c == CYR_SH || c == CYR_CH || (c == CYR_Z && skipped_sign))) {
                w.assimilated[at] = c == CYR_CH ? VOICED_CH : (RU_VOICE?[c] ?? c)
            } elif ((next == NextWord.sonorant || next == NextWord.vowel) && preposition) {
                w.assimilated[at] = c
            } else {
                w.assimilated[at] = RU_DEVOICE?[c] ?? c
            }
            continue
        }
        let nx = w.letters[j]
        // a consonant already devoiced carries the devoicing back through the cluster; дж keeps its д
        let nx_voiceless = key_exists(RU_VOICELESS, nx) || (key_exists(RU_VOICELESS, w.assimilated[j]) && !(c == CYR_D && nx == CYR_ZH))
        if (nx_voiceless) {
            // д keeps its voice before ф, and before т unless a softening vowel follows the т directly; бь too
            continue if (c == CYR_D && (nx == CYR_F || (nx == CYR_T && (skipped_sign || !soft_by_vowel(w, j)))) || c == CYR_B && skipped_sign && nx == CYR_T)
            w.assimilated[at] = RU_DEVOICE?[c] ?? c
        } elif (key_exists(RU_VOICING, nx) && c != CYR_V && !(skipped_sign && c == CYR_T)) {
            w.assimilated[at] = RU_VOICE?[c] ?? c
        }
    }
}

//! 1-based syllable ordinals per letter, 0 elsewhere; a р before ь counts, the reference reading it with a vowel of its own
def private vowel_ordinals(w : RuWord) : array<int> {
    var inscope out : array<int>
    var n = 0
    for (i, c in iter_range(w.letters), w.letters) {
        if (is_ru_vowel(c) || (c == CYR_R && soft_by_sign(w, i))) {
            n++
            out |> push(n)
        } else {
            out |> push(0)
        }
    }
    return <- out
}

//! the syllable ordinal of the first stressed vowel `step` letters at a time from `from` (1 forward, -1 back), -1 when none
def private stressed_ordinal_from(w : RuWord; syllable_of : array<int>; from, step : int) : int {
    var i = from + step
    while (i >= 0 && i < length(w.letters)) {
        if (syllable_of[i] > 0 && w.stressed[i]) {
            return syllable_of[i]
        }
        i += step
    }
    return -1
}

def private has_vowel_from(w : RuWord; from : int) : bool {
    for (i in range(from, length(w.letters))) {
        return true if (is_ru_vowel(w.letters[i]))
    }
    return false
}

//! а о and я after a consonant, the vowels the reduction reaches; stressed а reads a before a stressed syllable, ɑ otherwise;
//! `first_degree` is the ɐ of the first pretonic syllable and the word start, where every other syllable reads ə
def private emit_reducible(c : int; stressed : bool; mark : string; after_soft, first_degree, stressed_before_stress : bool; var out : array<string>) {
    if (stressed) {
        out |> push("{mark}{c == CYR_O ? "o" : (c == CYR_A && stressed_before_stress ? "a" : IPA_SCRIPT_A)}")
    } elif (after_soft) {
        out |> push(IPA_SMALL_I)
    } else {
        out |> push(first_degree ? IPA_TURNED_A : IPA_SCHWA)
    }
}

def private emit_plain_vowel(c : int; stressed : bool; mark : string; after_soft, after_hard_sibilant, first_degree, initial, final, stressed_before_stress : bool; var out : array<string>) {
    if (c == CYR_A || c == CYR_O || c == CYR_YA) {
        emit_reducible(c, stressed, mark, after_soft, first_degree, stressed_before_stress, out)
    } elif (c == CYR_E) {
        if (stressed) {
            out |> push("{mark}{after_hard_sibilant ? IPA_EPS : "e"}")
        } else {
            out |> push(after_hard_sibilant ? "y" : "i")
        }
    } elif (c == CYR_YO) {
        out |> push("{mark}o")
    } elif (c == CYR_I) {
        if (after_hard_sibilant) {
            out |> push("{mark}y")
        } elif (stressed) {
            out |> push("{mark}i")
        } else {
            out |> push((initial || final) ? IPA_SMALL_I : "i")
        }
    } elif (c == CYR_Y) {
        out |> push("{mark}y")
    } elif (c == CYR_U || c == CYR_YU) {
        out |> push("{mark}u")
    } else {   // э
        out |> push("{mark}{IPA_EPS}")
    }
}

def private emit_vowel(w : RuWord; i : int; syllable_of : array<int>; var out : array<string>) {
    let c = w.letters[i]
    let stressed = w.stressed[i]
    let n = length(w.letters)
    let prev = i > 0 ? w.letters[i - 1] : -1
    let initial = i == 0
    let final = i + 1 == n
    let after_vowel = prev >= 0 && is_ru_vowel(prev)
    let after_sign = prev == CYR_SOFT || prev == CYR_HARD
    let after_hard_sibilant = key_exists(RU_ALWAYS_HARD, prev)
    // a soft з reads ʒ, which carries no softness mark into the reduction; й does carry one
    let after_soft = prev == CYR_J || (prev >= 0 && !is_ru_vowel(prev) && !after_sign && !key_exists(RU_ALWAYS_SOFT, prev) && !after_hard_sibilant && key_exists(RU_SOFTENING_VOWELS, c) && prev != CYR_Z)
    let mark = stressed ? IPA_STRESS : ""
    let next_st = stressed_ordinal_from(w, syllable_of, i, 1)
    let last_vowel = !has_vowel_from(w, i + 1)
    let pretonic = next_st == syllable_of[i] + 1 || (last_vowel && w.next_side_opens_stressed)
    let stressed_before_stress = stressed && pretonic
    if (after_sign || ((initial || after_vowel) && key_exists(RU_IOTATED_VOWELS, c))) {
        let onset = after_sign && !stressed ? "jj" : "j"
        if (c == CYR_YA) {
            if (stressed) {
                out |> push(after_sign ? "j{mark}a" : "{mark}ja")
            } else {
                out |> push("{onset}{IPA_SMALL_I}")
            }
        } elif (c == CYR_E) {
            if (stressed) {
                out |> push("j{mark}e")
            } else {
                let first_posttonic = stressed_ordinal_from(w, syllable_of, i, -1) == syllable_of[i] - 1
                let reduced = initial || (final && (after_sign || !first_posttonic))
                out |> push("{onset}{reduced ? IPA_SMALL_I : "i"}")
            }
        } elif (c == CYR_YO) {
            out |> push("j{mark}o")
        } elif (c == CYR_YU) {
            if (stressed) {
                out |> push(after_sign ? "j{mark}ju" : "{mark}ju")
            } else {
                out |> push("{onset}u")
            }
        } elif (c == CYR_I) {   // after a sign
            out |> push(stressed ? "j{mark}i" : (final ? "j{IPA_SMALL_I}" : "ji"))
        } else {   // а о у ы э after a sign: as written
            emit_plain_vowel(c, stressed, mark, after_soft, after_hard_sibilant, pretonic || initial, initial, final, stressed_before_stress, out)
        }
        return
    }
    emit_plain_vowel(c, stressed, mark, after_soft, after_hard_sibilant, pretonic || initial, initial, final, stressed_before_stress, out)
}

//! the merged cluster at `i`, as the reference writes it; returns the letters consumed, 0 where none starts here
def private emit_cluster(w : RuWord; i : int; var out : array<string>) : int {
    let n = length(w.letters)
    let raw = w.letters[i]
    let nx = i + 1 < n ? w.letters[i + 1] : -1
    let nx2 = i + 2 < n ? w.letters[i + 2] : -1
    let nx3 = i + 3 < n ? w.letters[i + 3] : -1
    if (raw == CYR_T && nx == CYR_S && nx2 == CYR_YA && i + 3 == n) {
        out |> push(IPA_TS)
        out |> push(IPA_SCHWA)
        return 3
    }
    if (raw == CYR_T && nx == CYR_SOFT && nx2 == CYR_S && nx3 == CYR_YA && i + 4 == n) {
        out |> push(IPA_TS)
        out |> push(IPA_SCHWA)
        return 4
    }
    if ((raw == CYR_S || raw == CYR_Z || raw == CYR_ZH) && nx == CYR_CH) {
        out |> push(IPA_SCH + IPA_LONG)
        return 2
    }
    if ((raw == CYR_S || raw == CYR_Z) && nx == CYR_SH) {
        out |> push(IPA_ESH + IPA_ESH)
        return 2
    }
    if ((raw == CYR_S || raw == CYR_Z) && nx == CYR_ZH) {
        out |> push(IPA_EZH + IPA_EZH)
        return 2
    }
    if ((raw == CYR_T || raw == CYR_D) && nx == CYR_TS) {   // тц дц -> tʦ after the stress or right before it, ʦ further ahead of it
        if (has_stress_before(w, i) || (nx2 >= 0 && is_ru_vowel(nx2) && w.stressed[i + 2])) {
            out |> push("t")
        }
        out |> push(IPA_TS)
        return 2
    }
    if (raw == CYR_G && nx == CYR_K) {
        out |> push("x")
        return 1
    }
    if (raw == CYR_K && nx == CYR_K) {
        return 1
    }
    if (raw == CYR_CH && nx == CYR_SH) {
        let stress_follows = nx2 >= 0 && is_ru_vowel(nx2) && w.stressed[i + 2]
        out |> push(stress_follows ? "t" : IPA_TESH)
        return 1
    }
    return 0
}

def private emit_consonant(w : RuWord; i : int; next : NextWord; var out : array<string>) : int {
    let consumed = emit_cluster(w, i, out)
    return consumed if (consumed > 0)
    let n = length(w.letters)
    let c = w.assimilated[i]
    let raw = w.letters[i]
    if (c == VOICED_CH) {
        out |> push(IPA_DEZH + IPA_PAL)
        return 1
    }
    let soft = w.soft[i]
    let by_sign = soft_by_sign(w, i)
    let final = i + 1 == n || (by_sign && i + 2 == n)
    if (c == CYR_Z && soft) {   // a soft з reads ʒ in the reference's inventory
        out |> push(IPA_EZH)
        return 1
    }
    if (raw == CYR_R && by_sign) {
        out |> push((final && next == NextWord.vowel) ? "r{IPA_PAL}" : "r{IPA_SMALL_I}")
        return 1
    }
    out |> push(HARD_CONSONANT_IPA?[c] ?? "")
    if (soft && !key_exists(RU_ALWAYS_SOFT, c) && !key_exists(RU_ALWAYS_HARD, c) && c != CYR_KH) {
        let before_stressed_o = by_sign && i + 2 < n && w.letters[i + 2] == CYR_O && w.stressed[i + 2]
        if (!(by_sign && !soft_by_vowel(w, i) && key_exists(RU_HARD_BEFORE_SIGN, raw) && !(final && next == NextWord.vowel) && !before_stressed_o)) {
            out |> push(IPA_PAL)
        }
    }
    return 1
}

//! one word, or one side of a hyphenated one; `next_side_opens_stressed` says the next side opens stressed
def private side_ipa(word : string; next : NextWord; next_side_opens_stressed : bool) : string {
    var inscope w <- parse_marked(word)
    return "" if (empty(w.letters))
    w.next_side_opens_stressed = next_side_opens_stressed
    respell(w)
    mark_silent(w)
    mark_soft(w)
    assimilate_voicing(w, next, key_exists(VOICED_PREPOSITIONS, cpts_to_utf8(w.letters)))
    var inscope syllable_of <- vowel_ordinals(w)
    let n = length(w.letters)
    var inscope out : array<string>
    out |> reserve(n * 2)
    var i = 0
    while (i < n) {
        let c = w.letters[i]
        if (w.silent[i] || c == CYR_SOFT || c == CYR_HARD) {
            i++
        } elif (is_ru_vowel(c)) {
            emit_vowel(w, i, syllable_of, out)
            i++
        } else {
            i += emit_consonant(w, i, next, out)
        }
    }
    return join(out, "")
}

//! the IPA of one marked word; `next` is what opens the word after it, for the sandhi on its last consonant
def ru_word_ipa(word : string; next : NextWord = NextWord.none) : string => side_ipa(word, next, false)

let private KEEP_PUNCT <- { ',', '.', '!', '?', ':', ';', EM_DASH, ELLIPSIS }

//! a voiceless first letter a voiced obstruent follows (сделать) opens the word voiced
def private opening_of(word : string) : NextWord {
    var inscope cps <- utf8_to_cpts(word)
    var inscope letters <- [for (cp in cps); int(utf32_to_lower(uint(cp))); where cp != PLUS && cp != ACUTE]
    return NextWord.none if (empty(letters))
    let lc = letters[0]
    return NextWord.voiced if (key_exists(RU_VOICING, lc) || key_exists(RU_VOICELESS, lc) && length(letters) > 1 && key_exists(RU_VOICING, letters[1]))
    return NextWord.vowel if (is_ru_vowel(lc))
    return NextWord.sonorant if (key_exists(RU_SONORANT_LETTERS, lc))
    return NextWord.none
}

//! a side of a hyphenated word: a particle with its own stress, a one-vowel side stressed unless it is a clitic or already marked
def private stress_lone_vowel(part : string) : string {
    let lower = utf8_to_lower(part)
    let known = HYPHEN_PARTICLES?[lower] ?? ""
    return known if (!empty(known))
    var inscope cps <- utf8_to_cpts(part)
    return part if (has_mark(cps) || key_exists(HYPHEN_CLITICS, lower) || guess_vowel_count(cps) != 1)
    return mark_pick(part, 1)
}

def private opens_stressed(part : string) : bool {
    var inscope cps <- utf8_to_cpts(part)
    for (i, cp in iter_range(cps), cps) {
        let lc = int(utf32_to_lower(uint(cp)))
        if (is_ru_vowel(lc)) {
            return (i > 0 && cps[i - 1] == PLUS) || (i + 1 < length(cps) && cps[i + 1] == ACUTE) || lc == CYR_YO
        }
    }
    return false
}

def private hyphen_parts_ipa(word : string; next : NextWord) : string {
    var inscope parts <- split(word, "-")
    return ru_word_ipa(word, next) if (length(parts) == 1)
    for (part in parts) {
        part = stress_lone_vowel(part)
    }
    return build_string() $(var wr) {
        for (k, p in iter_range(parts), parts) {
            continue if (empty(p))
            let last = k + 1 >= length(parts)
            wr |> write(side_ipa(p, last ? next : opening_of(parts[k + 1]), !last && opens_stressed(parts[k + 1])))
        }
    }
}

def private is_pass_through_word_cpt(cp : int) : bool => cp > ' ' && !cpt_is_cyrillic(cp) && !key_exists(KEEP_PUNCT, cp) && cp != '"' && cp != '(' && cp != ')' && cp != GUILLEMET_OPEN && cp != GUILLEMET_CLOSE && cp != '-' && cp != PLUS && cp != ACUTE

//! One token of a marked sentence
struct private RuToken {
    text : string           //! the word, or "" for punctuation standing alone
    punct_after : string    //! the kept punctuation following it, "" where none does
    pass_through : bool     //! another script's run, carried through as written
}

//! the run `cur` holds as a token, `cur` emptied
def private take_token(var tokens : array<RuToken>; var cur : array<int>; pass_through : bool) {
    tokens |> push(RuToken(text = cpts_to_utf8(cur), pass_through = pass_through))
    cur |> clear()
}

//! The phonemes of one normalized, stress-marked sentence: kept punctuation attached, one space between tokens, another script's run passed through.
def ru_phonemize_marked(text : string) : string {
    var inscope cps <- utf8_to_cpts(text)
    let n = length(cps)
    var inscope tokens : array<RuToken>
    var inscope cur : array<int>
    var cur_pass_through = false
    var i = 0
    while (i < n) {
        let cp = cps[i]
        let hyphen_inside = cp == '-' && !empty(cur) && i + 1 < n && (cpt_is_cyrillic(cps[i + 1]) || cps[i + 1] == PLUS)
        if (cpt_is_cyrillic(cp) || cp == PLUS || cp == ACUTE || hyphen_inside) {
            if (cur_pass_through) {   // a script change inside a run splits it
                take_token(tokens, cur, true)
                cur_pass_through = false
            }
            cur |> push(cp)
            i++
            continue
        }
        if (is_pass_through_word_cpt(cp)) {
            if (!empty(cur) && !cur_pass_through) {
                take_token(tokens, cur, false)
            }
            cur |> push(cp)
            cur_pass_through = true
            i++
            continue
        }
        if (!empty(cur)) {
            take_token(tokens, cur, cur_pass_through)
            cur_pass_through = false
        }
        if (key_exists(KEEP_PUNCT, cp)) {
            if (!empty(tokens) && (i == 0 || cps[i - 1] != ' ')) {
                tokens[length(tokens) - 1].punct_after += cpt_to_utf8(cp)
            } else {
                tokens |> push(RuToken(punct_after = cpt_to_utf8(cp)))
            }
        }
        i++
    }
    if (!empty(cur)) {
        take_token(tokens, cur, cur_pass_through)
    }
    return build_string() $(var wr) {
        for (k, tok in iter_range(tokens), tokens) {
            let next = k + 1 < length(tokens) && empty(tok.punct_after) && !tokens[k + 1].pass_through ? opening_of(tokens[k + 1].text) : NextWord.none
            let ipa = empty(tok.text) ? "" : (tok.pass_through ? tok.text : hyphen_parts_ipa(tok.text, next))
            if (k > 0) {
                wr |> write(" ")
            }
            wr |> write(ipa)
            wr |> write(tok.punct_after)
        }
    }
}

//! The stress pack: the file bytes (searched in place), the stress table (bare form -> its stressed vowel ordinals, the default first), the ё table
//! (a form written with е -> its ё spelling) and the sections a pack may carry after them - the ending table (an ending, reversed -> the stressed
//! vowel counted from the word's end) and the rule tier (a form -> its family, the pick of either side and its unruled side, then the rules).
struct RuStressPack {
    data : array<uint8>
    source : string
    stress : FrontTable
    yo : FrontTable
    endings : FrontTable
    rule_forms : FrontTable
    rule_families : int
    rule_index : int64      //! one u32 a family and one past the last: where its rules start in the rule bytes
    rule_bytes : int64
}

let private RU_PACK_MAGIC = 0x55524754u
let private RU_PACK_VERSION = 2
let private RU_SECTION_ENDINGS = 0x53444E45u    //! 'ENDS'
let private RU_SECTION_RULES = 0x534C5552u      //! 'RULS'
let private SECTION_HEAD = 8l                   //! a section's tag and byte count
//! bit k set where the key letter of code k is a vowel: а е и о у ы э ю я ё
let private KEY_VOWELS = 0x7A0210484ul
let private ABBREVIATION_MAX_VOWELS = 3
let private RULE_SLOTS = 5                      //! the neighbours a rule may read: three words back to two ahead
let private RULE_SLOTS_BEHIND = 3               //! the slots of the words behind; the slots after them are the words ahead
let private RULE_SLOT_BITS = 3                  //! a rule's first byte holds the neighbour's slot in these low bits, its kind above them
let private RULE_NEAR_BYTES = RULE_SLOTS * FRONT_KEY_MAX
let private RULE_HEAD = 3l                      //! a rule's kind, verdict and key length
let private RULE_VALUE_SIDES = 1l               //! a rule form's value run: its family, then the pick of side 0 and of side 1
let private RULE_VALUE_UNRULED = 3l             //! ... then its unruled side
let private CYR_E_CAP = 1045
let private KEY_HYPHEN = 1
let private KEY_A = 2       //! а; the letters to п follow it
let private KEY_V = 4       //! в
let private KEY_E = 7       //! е
let private KEY_R = 18      //! р; the letters to я follow it
let private KEY_YO = 34
let private UTF8_LEAD_A_TO_P : int = 0xD0
let private UTF8_LEAD_R_TO_YO : int = 0xD1
let private UTF8_A_TAIL = 176
let private UTF8_P_TAIL = 191
let private UTF8_R_TAIL = 128
let private UTF8_YA_TAIL = 143
let private UTF8_E_TAIL = 181
let private UTF8_YO_TAIL = 145

//! What a rule asks of its neighbour, by the number the pack writes: the neighbour is this `word`; the neighbour, at least two letters longer, has this `ending`; the sentence's `edge` comes before any such neighbour.
enum RuRuleKind : uint8 {
    word
    ending
    edge
}

//! the prepositions, conjunctions and particles that lean on the next word and take no stress of their own
let private RU_CLITICS <- { "и", "а", "но", "да", "не", "ни", "же", "ж", "бы", "б", "ли", "ль", "на", "за", "по", "под", "подо",
    "над", "надо", "об", "обо", "о", "от", "ото", "до", "из", "изо", "без", "безо", "при", "про", "для", "с", "со", "в", "во",
    "к", "ко", "у", "что", "уж", "аж", "перед", "передо", "через", "сквозь", "между", "ради" }
//! the tagged sections after the two tables, from `at` to the file's end; a tag this build does not know is passed over
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-stress-pack")]
def private read_ru_pack_sections(var p : RuStressPack; path : string; at : int64) {
    let size = long_length(p.data)
    var o = at
    while (o + SECTION_HEAD <= size) {
        let tag = rd_u32(p.data, o)
        var body = o + SECTION_HEAD
        o = body + int64(rd_u32(p.data, o + 4l))
        if (o > size) {
            panic("dasLLAMA tts: a section of '{path}' runs past the file's end - rebuild it with harness/build_g2p_ru_data.py")
        }
        if (tag == RU_SECTION_ENDINGS) {
            p.endings = read_front_table(p.data, body)
        } elif (tag == RU_SECTION_RULES) {
            p.rule_forms = read_front_table(p.data, body)
            p.rule_families = rd_i32(p.data, body)
            p.rule_index = body + 4l
            p.rule_bytes = p.rule_index + int64(p.rule_families + 1) * 4l
        }
    }
}

//! Load tts_g2p_ru.bin. Panics on a missing file, a wrong magic or version, a section past the file's end, or a key longer than FRONT_KEY_MAX.
def load_ru_stress_pack(path : string) : RuStressPack {
    var p = RuStressPack()
    p.data <- pack_read_bytes(path)
    if (empty(p.data)) {
        panic("dasLLAMA tts: cannot open the Russian stress pack '{path}'")
    }
    var o = 0l
    p.source = read_pack_header(p.data, path, "tts_g2p_ru.bin", "harness/build_g2p_ru_data.py", RU_PACK_MAGIC, RU_PACK_VERSION, o)
    p.stress = read_front_table(p.data, o)
    p.yo = read_front_table(p.data, o)
    read_ru_pack_sections(p, path, o)
    let longest = max(max(p.stress.key_max, p.yo.key_max), max(p.endings.key_max, p.rule_forms.key_max))
    if (longest > FRONT_KEY_MAX) {
        panic("dasLLAMA tts: '{path}' holds a key of {longest} letters, and this build reads keys of {FRONT_KEY_MAX} at most - rebuild it with harness/build_g2p_ru_data.py")
    }
    return <- p
}

def ru_pack_loaded(p : RuStressPack) : bool => !empty(p.data)

//! `word` in the pack's key alphabet, a byte a letter; empty where it holds anything but lowercase Russian letters and hyphens
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-stress-pack")]
def private ru_pack_key(word : string; var q : uint8[FRONT_KEY_MAX]) : int {
    var used = 0
    peek_data(word) $(d : array<uint8> const#) {
        let n = length(d)
        var i = 0
        while (i < n && used >= 0) {
            let lead = int(d[i])
            let tail = i + 1 < n ? int(d[i + 1]) : 0
            var code = 0
            if (lead == '-') {
                code = KEY_HYPHEN
            } elif (lead == UTF8_LEAD_A_TO_P && tail >= UTF8_A_TAIL && tail <= UTF8_P_TAIL) {
                code = KEY_A + tail - UTF8_A_TAIL
            } elif (lead == UTF8_LEAD_R_TO_YO && tail >= UTF8_R_TAIL && tail <= UTF8_YA_TAIL) {
                code = KEY_R + tail - UTF8_R_TAIL
            } elif (lead == UTF8_LEAD_R_TO_YO && tail == UTF8_YO_TAIL) {
                code = KEY_YO
            }
            if (code == 0 || used == FRONT_KEY_MAX) {
                used = -1
            } else {
                q[used] = uint8(code)
                used++
                i += code == KEY_HYPHEN ? 1 : 2
            }
        }
    }
    return max(used, 0)
}

//! where the value run of a lowercase form, less its last `drop` letters, starts in a table of the pack; -1 when the table holds no such key
def private ru_pack_find(p : RuStressPack; t : FrontTable; word : string; drop : int = 0) : int64 {
    return -1l if (empty(p.data))
    var q : uint8[FRONT_KEY_MAX]
    return front_find(p.data, t, q, ru_pack_key(word, q) - drop)
}

//! the 1-based stressed-vowel ordinals the pack holds for a lowercase form, the default first; empty when it holds none
def ru_stress_variants(p : RuStressPack; word : string) : array<int> {
    var inscope out : array<int>
    var o = ru_pack_find(p, p.stress, word)
    return <- out if (o < 0l)
    let end = front_value_end(p.data, o)
    out |> reserve(end - o)
    while (o < end) {
        out |> push(front_value(p.data, o))
        o++
    }
    return <- out
}

//! the pack's default ordinal for a lowercase form less its last `drop` letters, 0 when it holds none; allocates nothing
def private ru_stress_default(p : RuStressPack; word : string; drop : int = 0) : int {
    let o = ru_pack_find(p, p.stress, word, drop)
    return o < 0l ? 0 : front_value(p.data, o)
}

//! Whether the pack has `ordinal` among the readings of `lower`.
def ru_stress_pack_has(p : RuStressPack; lower : string; ordinal : int) : bool {
    var o = ru_pack_find(p, p.stress, lower)
    return false if (o < 0l)
    let end = front_value_end(p.data, o)
    while (o < end) {
        return true if (front_value(p.data, o) == ordinal)
        o++
    }
    return false
}

//! the ё spelling of a form written with е, "" when the pack has none for it
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-yo")]
def ru_yo_form(p : RuStressPack; word : string) : string {
    let o = ru_pack_find(p, p.yo, word)
    return "" if (o < 0l)
    let mask = yo_run_mask(p.data, o)
    return modify_data(word) $(var bytes : array<uint8>#) {
        var nth = 0
        var i = 0
        while (i + 1 < length(bytes)) {
            if (int(bytes[i]) == UTF8_LEAD_A_TO_P && int(bytes[i + 1]) == UTF8_E_TAIL) {
                if (yo_mask_has(mask, nth)) {
                    bytes[i] = uint8(UTF8_LEAD_R_TO_YO)
                    bytes[i + 1] = uint8(UTF8_YO_TAIL)
                }
                nth++
            }
            i += int(bytes[i]) == '-' ? 1 : 2
        }
    }
}

//! the ё table's value run at `o` as one mask over the key's е letters: bit `nth` set where the `nth` е, from 0, is a ё
def private yo_run_mask(data : array<uint8>; o : int64) : uint64 {
    var mask = 0ul
    var at = o
    var shift = 0
    while (true) {
        mask |= uint64(front_value(data, at)) << uint64(shift)
        break if (!front_value_more(data, at))
        at++
        shift += FRONT_VALUE_BITS
    }
    return mask
}

def private yo_mask_has(mask : uint64; nth : int) : bool => ((mask >> uint64(nth)) & 1ul) != 0ul

def private has_mark(cps : array<int>) : bool {
    for (cp in cps) {
        return true if (cp == PLUS || cp == ACUTE || cp == CYR_YO || cp == CYR_YO_CAP)
    }
    return false
}

let RU_PICK_YO = 64   //! added to a vowel's ordinal: that vowel, an е, read as ё

//! `word` as `pick` reads it
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-yo-pick")]
def private mark_pick(word : string; pick : int) : string {
    let as_yo = pick > RU_PICK_YO
    let ordinal = as_yo ? pick - RU_PICK_YO : pick
    var inscope cps <- utf8_to_cpts(word)
    var n = 0
    for (i, cp in iter_range(cps), cps) {
        if (is_ru_vowel(int(utf32_to_lower(uint(cp))))) {
            n++
            if (n == ordinal) {
                return "{cpts_to_utf8(cps, 0, i)}+{cpts_to_utf8(cps, i, length(cps))}" if (!as_yo)
                return cp == CYR_E || cp == CYR_E_CAP ? yo_spelled(cps, int2(0, length(cps)), i) : word
            }
        }
    }
    return word
}

def private guess_vowel_count(cps : array<int>) : int {
    var n = 0
    for (cp in cps) {
        if (is_ru_vowel(int(utf32_to_lower(uint(cp))))) {
            n++
        }
    }
    return n
}

//! the shape rule: the vowel before the last; a one-vowel word its one, a word with none 0
def private guess_stress_ordinal(word : string) : int {
    var inscope cps <- utf8_to_cpts(word)
    let n = guess_vowel_count(cps)
    return n <= 1 ? n : n - 1
}

def private key_is_vowel(code : uint8) : bool => ((KEY_VOWELS >> uint64(code)) & 1ul) != 0ul

//! The ending table's ordinal for a lowercase word: its longest ending the table holds names the stressed vowel counted from the word's end; 0 where the table holds no ending of the word, or names a vowel the word lacks. Allocates nothing.
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-endings")]
def ru_ending_ordinal(p : RuStressPack; lower : string) : int {
    return 0 if (p.endings.n_keys == 0)
    var q : uint8[FRONT_KEY_MAX]
    let n = ru_pack_key(lower, q)
    var reversed : uint8[FRONT_KEY_MAX]
    var vowels = 0
    for (k in range(n)) {
        vowels += key_is_vowel(q[k]) ? 1 : 0
        reversed[k] = q[n - 1 - k]
    }
    var letters = min(p.endings.key_max, n - 1)
    while (letters > 0) {
        let o = front_find(p.data, p.endings, reversed, letters)
        let from_end = o < 0l ? 0 : front_value(p.data, o)
        return vowels - from_end + 1 if (from_end > 0 && from_end <= vowels)
        letters--
    }
    return 0
}

//! whether a word as written is an abbreviation the last-vowel rule reads: two letters or more, every one a capital, the last a consonant
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-abbreviations")]
def private is_capital_abbreviation(cps : array<int>) : bool {
    return false if (length(cps) < 2)
    for (cp in cps) {
        return false if (!cpt_is_cyrillic_upper(cp))
    }
    return !is_ru_vowel(int(utf32_to_lower(uint(cps[length(cps) - 1]))))
}

//! the case endings a word may shed to reach a form the pack holds, longest first
let private CASE_ENDINGS = fixed_array("ими", "их", "у", "е", "ы", "и", "о")

//! how many of the key's first `letters` bytes are vowels
def private key_vowels(q : uint8[FRONT_KEY_MAX]; letters : int) : int {
    var vowels = 0
    for (k in range(letters)) {
        vowels += key_is_vowel(q[k]) ? 1 : 0
    }
    return vowels
}

//! how many of the key's first `letters` bytes are an е
def private key_e_count(q : uint8[FRONT_KEY_MAX]; letters : int) : int {
    var count = 0
    for (k in range(letters)) {
        count += int(q[k]) == KEY_E ? 1 : 0
    }
    return count
}

//! The stem's pick, 0 where no ending leaves such a stem; allocates nothing.
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-case-stem")]
def ru_stem_pick(p : RuStressPack; lower : string) : int {
    return 0 if (empty(p.data))
    var q : uint8[FRONT_KEY_MAX]
    let n = ru_pack_key(lower, q)
    for (ending in CASE_ENDINGS) {
        let stem_letters = n - utf8_length(ending)
        continue if (stem_letters < 2 || !ends_with(lower, ending))
        let vowels = key_vowels(q, stem_letters)
        continue if (vowels < 2)
        let o = front_find(p.data, p.stress, q, stem_letters)
        if (o >= 0l) {
            continue if (front_value_more(p.data, o))
            return front_value(p.data, o)
        }
        continue if (int(q[stem_letters - 1]) != KEY_V || int(q[stem_letters - 2]) != KEY_E)
        let restored = front_find(p.data, p.yo, q, stem_letters)
        continue if (restored < 0l)
        let last_e = key_e_count(q, stem_letters) - 1
        return RU_PICK_YO + vowels if (yo_run_mask(p.data, restored) == 1ul << uint64(last_e))
    }
    return 0
}

let private PATRONYMIC_TAIL_MAX = 2     //! the longest case ending read past a short patronymic's suffix

//! the pack's ordinal for the full patronymic of the key `q` of `n` letters: `cut` letters at `at` replaced by `full`, the case ending past them kept; 0 where the pack holds no such form, reads it two ways, or stresses it past the `stem_vowels` first vowels
def private patronymic_full_ordinal(p : RuStressPack; q : uint8[FRONT_KEY_MAX]; n, at, cut : int; full : string; stem_vowels : int) : int {
    var full_key : uint8[FRONT_KEY_MAX]
    let full_letters = ru_pack_key(full, full_key)
    return 0 if (n - cut + full_letters > FRONT_KEY_MAX)
    var long_key : uint8[FRONT_KEY_MAX]
    for (k in range(at)) {
        long_key[k] = q[k]
    }
    for (k in range(full_letters)) {
        long_key[at + k] = full_key[k]
    }
    for (k in range(at + cut, n)) {
        long_key[k - cut + full_letters] = q[k]
    }
    let o = front_find(p.data, p.stress, long_key, n - cut + full_letters)
    return 0 if (o < 0l || front_value_more(p.data, o))
    let ordinal = front_value(p.data, o)
    return ordinal <= stem_vowels ? ordinal : 0
}

//! whether the `letters` first bytes of `key` sit in `q` from `at`
def private key_at(q : uint8[FRONT_KEY_MAX]; at : int; key : uint8[FRONT_KEY_MAX]; letters : int) : bool =>
    bytes_compare(q, int64(at), letters, key, 0l, letters) == 0

//! The stress of a short patronymic through its full form: -еич read as -еевич and -ыч as -ович, a case ending of two letters at most kept, where the pack holds the full form with one reading stressed in the name's stem; 0 otherwise. Allocates nothing.
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-short-patronymic")]
def ru_patronymic_ordinal(p : RuStressPack; lower : string) : int {
    return 0 if (empty(p.data))
    var q : uint8[FRONT_KEY_MAX]
    let n = ru_pack_key(lower, q)
    var short_eich, short_ych : uint8[FRONT_KEY_MAX]
    let eich_letters = ru_pack_key("еич", short_eich)
    let ych_letters = ru_pack_key("ыч", short_ych)
    let ich_letters = eich_letters - 1
    for (tail in range(PATRONYMIC_TAIL_MAX + 1)) {
        let eich = n - tail - eich_letters
        let ych = n - tail - ych_letters
        let stem_vowels = key_vowels(q, max(ych, 0))
        if (eich >= 2 && key_at(q, eich, short_eich, eich_letters)) {
            let ich = eich + 1
            let o = patronymic_full_ordinal(p, q, n, ich, ich_letters, "евич", stem_vowels)
            return o if (o > 0)
        }
        if (ych >= 2 && key_at(q, ych, short_ych, ych_letters)) {
            let o = patronymic_full_ordinal(p, q, n, ych, ych_letters, "ович", stem_vowels)
            return o if (o > 0)
        }
    }
    return 0
}

//! The stressed vowel of a word the pack does not hold, `word` as written and `lower` its lowercase: the vowel of the form it is a case of, else of the full patronymic it is short for, else an abbreviation's last vowel, else the ending table's vowel, else the shape rule's.
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-unknown-word")]
def private ru_unknown_ordinal(p : RuStressPack; word, lower : string) : int {
    let by_stem = ru_stem_pick(p, lower)
    return by_stem if (by_stem > 0)
    let by_patronymic = ru_patronymic_ordinal(p, lower)
    return by_patronymic if (by_patronymic > 0)
    var inscope cps <- utf8_to_cpts(word)
    let vowels = guess_vowel_count(cps)
    return vowels if (vowels >= 2 && vowels <= ABBREVIATION_MAX_VOWELS && is_capital_abbreviation(cps))
    let by_ending = ru_ending_ordinal(p, lower)
    return by_ending > 0 ? by_ending : guess_stress_ordinal(lower)
}

//! The pack's default ordinal for a lowercase form, a reflexive read through its base form; 0 where the pack has neither.
def ru_pack_default(p : RuStressPack; lower : string) : int {
    let own = ru_stress_default(p, lower)
    return own if (own > 0)
    return 0 if (!(lower |> ends_with("ся")) && !(lower |> ends_with("сь")))
    return ru_stress_default(p, lower, 2)
}

//! One word, or one hyphenated word whole, ahead of the stress lookup: `settled` with its final `text` where a mark, its clitics or a ё spelling decide it, else its `lower` form for the lookup.
struct private RuPreLookup {
    settled : bool
    text : string
    lower : string
}

//! whether a lowercase word is a clitic, a hyphenated one made of clitics on every side (из-за, из-под): such a word leans on the next one and takes no stress
def private all_sides_clitic(lower : string) : bool {
    return key_exists(RU_CLITICS, lower) if (find(lower, "-") < 0)
    var inscope sides <- split(lower, "-")
    for (side in sides) {
        return false if (!key_exists(RU_CLITICS, side))
    }
    return true
}

def private ru_word_pre_lookup(p : RuStressPack; word : string) : RuPreLookup {
    var inscope cps <- utf8_to_cpts(word)
    return RuPreLookup(settled = true, text = word) if (empty(cps) || has_mark(cps))
    let lower = utf8_to_lower(word)
    return RuPreLookup(settled = true, text = word) if (all_sides_clitic(lower))
    let yo = ru_yo_form(p, lower)
    return RuPreLookup(settled = !empty(yo), text = yo, lower = lower)
}

//! Whether a context pass may decide `word`: one word with no mark, no ё, no ё spelling to restore, and no clitic.
def ru_word_needs_context(p : RuStressPack; word : string) : bool => find(word, "-") < 0 && !ru_word_pre_lookup(p, word).settled

//! whether the `.`, `!`, `?` or ellipsis at `i` closes a sentence: whitespace or the text's end follows it
def ru_is_sentence_end(cps : array<int>; i : int) : bool {
    let cp = cps[i]
    return (cp == '.' || cp == '!' || cp == '?' || cp == ELLIPSIS) && (i + 1 == length(cps) || cpt_is_whitespace(cps[i + 1]))
}

//! The sentence every word of `words` (the `ru_text_words` of `cps`) sits in, counted from 0.
def ru_word_sentences(cps : array<int>; words : array<int2>) : array<int> {
    var sentences : array<int>
    sentences |> reserve(length(words))
    var sentence = 0
    var from = 0
    for (w in words) {
        for (i in range(from, w.x)) {
            if (ru_is_sentence_end(cps, i)) {
                sentence += from > 0 ? 1 : 0
                break
            }
        }
        sentences |> push(sentence)
        from = w.y
    }
    return <- sentences
}

//! whether the rule at `key` (its `letters` key bytes) of kind `kind`, a `RuRuleKind` by number, holds for the neighbour whose key is the `near_letters` bytes of `near_keys` from `near_at`; `near_letters` is negative where the sentence holds no such neighbour, 0 where the neighbour is no key
def private rule_fires(data : array<uint8>; key : int64; letters, kind : int; near_keys : uint8[RULE_NEAR_BYTES]; near_at, near_letters : int) : bool {
    return near_letters < 0 if (kind == int(RuRuleKind.edge))
    return false if (near_letters < letters || (kind == int(RuRuleKind.word) && near_letters != letters) || (kind == int(RuRuleKind.ending) && near_letters < letters + 2))
    return bytes_compare(data, key, letters, near_keys, int64(near_at + near_letters - letters), letters) == 0
}

//! The rule tier's pick for word `i` of the lowercased words of a text, `sentences` their `ru_word_sentences`, or 0 to abstain: the first rule of the word's family that holds decides, and where none holds the form's own unruled side, if it names one. Allocates nothing.
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-rule-tier")]
def ru_stress_rule(p : RuStressPack; words : array<string>; sentences : array<int>; i : int) : int {
    return 0 if (p.rule_forms.n_keys == 0)
    var q : uint8[FRONT_KEY_MAX]
    let form = front_find(p.data, p.rule_forms, q, ru_pack_key(words[i], q))
    return 0 if (form < 0l)
    let family = front_value(p.data, form)
    return 0 if (family <= 0 || family > p.rule_families)
    var near_keys : uint8[RULE_NEAR_BYTES]
    var near_letters : int[RULE_SLOTS]
    for (slot in range(RULE_SLOTS)) {
        let j = i + slot - RULE_SLOTS_BEHIND + (slot >= RULE_SLOTS_BEHIND ? 1 : 0)
        near_letters[slot] = -1
        if (j >= 0 && j < length(words) && sentences[j] == sentences[i]) {
            near_letters[slot] = ru_pack_key(words[j], q)
            for (k in range(near_letters[slot])) {
                near_keys[slot * FRONT_KEY_MAX + k] = q[k]
            }
        }
    }
    var o = p.rule_bytes + int64(rd_u32(p.data, p.rule_index + int64(family - 1) * 4l))
    let end = p.rule_bytes + int64(rd_u32(p.data, p.rule_index + int64(family) * 4l))
    while (o < end) {
        let packed = int(p.data[o])
        let slot = packed & ((1 << RULE_SLOT_BITS) - 1)
        let kind = packed >> RULE_SLOT_BITS
        let verdict = int(p.data[o + 1l])
        let letters = int(p.data[o + 2l])
        if (rule_fires(p.data, o + RULE_HEAD, letters, kind, near_keys, slot * FRONT_KEY_MAX, near_letters[slot])) {
            return front_value(p.data, form + RULE_VALUE_SIDES + int64(verdict & 1))
        }
        o += RULE_HEAD + int64(letters)
    }
    let unruled = front_value(p.data, form + RULE_VALUE_UNRULED)
    return unruled == 0 ? 0 : front_value(p.data, form + int64(unruled))
}

//! A hyphenated word as the pack's whole key reads it - its ё spelling, else `picked` (0 for none), else its default reading marked; the word as written where it carries a mark, is made of clitics, or the pack holds no such key and nothing picked.
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-hyphenated")]
def private ru_mark_whole(p : RuStressPack; word : string; picked : int) : string {
    let pre = ru_word_pre_lookup(p, word)
    return pre.text if (pre.settled)
    let pick = picked > 0 ? picked : ru_stress_default(p, pre.lower)
    return pick > 0 ? mark_pick(word, pick) : word
}

//! One word of running text stress-marked: the text's own mark wins, then the clitics, the pack's ё spelling, `picked` (the pick of the rule tier or of a context pass, 0 for none), the pack's default reading, then a word the pack does not hold - an abbreviation's last vowel, the ending table, the shape rule; a hyphenated word as the pack's whole key marks it, then side by side.
def ru_mark_word(p : RuStressPack; word : string; picked : int = 0) : string {
    if (find(word, "-") >= 0) {
        var inscope parts <- split(ru_mark_whole(p, word, picked), "-")
        for (part in parts) {
            part = ru_mark_word(p, part)
        }
        return join(parts, "-")
    }
    let pre = ru_word_pre_lookup(p, word)
    return pre.text if (pre.settled)
    let known = picked > 0 ? picked : ru_pack_default(p, pre.lower)
    let pick = known > 0 ? known : ru_unknown_ordinal(p, word, pre.lower)
    return pick <= 0 ? word : mark_pick(word, pick)
}

//! The words of running text the stress pass marks, in order, as codepoint spans [x, y): runs of Cyrillic letters with their stress marks and inner hyphens.
def ru_text_words(cps : array<int>) : array<int2> {
    var words : array<int2>
    let n = length(cps)
    var start = -1
    for (i in range(n + 1)) {
        let cp = i < n ? cps[i] : -1
        let hyphen_inside = cp == '-' && start >= 0 && i + 1 < n && (cpt_is_cyrillic(cps[i + 1]) || cps[i + 1] == PLUS)
        if (cp >= 0 && (cpt_is_cyrillic(cp) || cp == PLUS || cp == ACUTE || hyphen_inside)) {
            start = start < 0 ? i : start
        } elif (start >= 0) {
            words |> push(int2(start, i))
            start = -1
        }
    }
    return <- words
}

//! the pronoun forms a text without ё spells with е, each after the prepositions that leave it no other reading
let private YO_AFTER_PREPOSITION <- { "в чем", "на чем", "о чем", "при чем", "в нем", "на нем", "о нем", "при нем",
    "во всем", "на всем", "обо всем", "при всем", "в моем", "о моем" }
let private YO_RULE_WORDS <- { "чем", "нем", "всем", "моем" }

def private is_space_gap(cps : array<int>; from, to : int) : bool {
    for (i in range(from, to)) {
        return false if (cps[i] != ' ')
    }
    return from < to
}

//! whether the full stop at `at` closes an abbreviation: a comma or a small letter comes next (на нем. яз.)
def private is_abbreviation_stop(cps : array<int>; at : int) : bool {
    return false if (at >= length(cps) || cps[at] != '.')
    var i = at + 1
    while (i < length(cps) && cps[i] == ' ') {
        i++
    }
    return i < length(cps) && (cps[i] == ',' || (cpt_is_cyrillic(cps[i]) && !cpt_is_cyrillic_upper(cps[i])))
}

//! Whether word `k` is a е spelling the rule writes with ё: a pronoun form right after its preposition, nothing but spaces between.
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-yo-rule")]
def private ru_takes_yo_by_rule(cps : array<int>; words : array<int2>; lowers : array<string>; k : int) : bool {
    return false if (k == 0 || !key_exists(YO_RULE_WORDS, lowers[k]) || !is_space_gap(cps, words[k - 1].y, words[k].x) || !key_exists(YO_AFTER_PREPOSITION, "{lowers[k - 1]} {lowers[k]}"))
    let after_not = lowers[k] == "чем" && k >= 2 && lowers[k - 2] == "не" && is_space_gap(cps, words[k - 2].y, words[k - 1].x)
    return !after_not && !(lowers[k] == "нем" && is_abbreviation_stop(cps, words[k].y))
}

//! the word at `w` with its е at `at` spelled ё in the letter's own case
def private yo_spelled(cps : array<int>; w : int2; at : int) : string {
    let yo = cps[at] == CYR_E_CAP ? CYR_YO_CAP : CYR_YO
    return "{cpts_to_utf8(cps, w.x, at)}{cpt_to_utf8(yo)}{cpts_to_utf8(cps, at + 1, w.y)}"
}

//! The codepoints `cps` with every word of `words` (its `ru_text_words`) stress-marked (`ru_mark_word`), everything else as it was; `picked` maps a word's index to a context pass's pick, and a word it leaves out is the rule tier's to pick.
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-stress-context")]
def ru_mark_words(p : RuStressPack; cps : array<int>; words : array<int2>; picked : table<int; int>) : string {
    var inscope lowers <- [for (w in words); utf8_to_lower(cpts_to_utf8(cps, w.x, w.y))]
    var inscope sentences <- ru_word_sentences(cps, words)
    return build_string() $(var wr) {
        var pos = 0
        for (w, k in words, count()) {
            for (i in range(pos, w.x)) {
                wr |> write(cpt_to_utf8(cps[i]))
            }
            if (ru_takes_yo_by_rule(cps, words, lowers, k)) {
                wr |> write(yo_spelled(cps, w, w.y - 2))   // the е of a `YO_RULE_WORDS` form is its letter before the last
            } else {
                let by_context = picked?[k] ?? 0
                let pick = by_context > 0 ? by_context : ru_stress_rule(p, lowers, sentences, k)
                wr |> write(ru_mark_word(p, cpts_to_utf8(cps, w.x, w.y), pick))
            }
            pos = w.y
        }
        for (i in range(pos, length(cps))) {
            wr |> write(cpt_to_utf8(cps[i]))
        }
    }
}

//! Running text with every Russian word stress-marked; `picked` maps a word's index among `ru_text_words` to a context pass's pick.
def ru_mark_stress(p : RuStressPack; text : string; picked : table<int; int>) : string {
    var inscope cps <- utf8_to_cpts(text)
    var inscope words <- ru_text_words(cps)
    return ru_mark_words(p, cps, words, picked)
}

//! `ru_mark_stress` with no context pass: the pack and the rules alone.
def ru_mark_stress(p : RuStressPack; text : string) : string {
    var inscope none : table<int; int>
    return ru_mark_stress(p, text, none)
}

//! Normalized Russian text to phonemes: the stress pass, then letter-to-sound.
def ru_phonemize(p : RuStressPack; text : string) : string => ru_phonemize_marked(ru_mark_stress(p, text))
