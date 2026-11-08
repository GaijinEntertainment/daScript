options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options _ascii_strings = false   // the Russian word lists and the IPA inventory are the data

module dasllama_g2p_ru shared public

require dasllama/dasllama_lint public
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
let private ELLIPSIS = 8230
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

//! the index of the first stressed vowel at or after `from`, -1 when none; a consonant run
//! between counts as nothing, a vowel ends the search
def private next_vowel_if_stressed(w : RuWord; from : int) : int {
    for (i in range(from, length(w.letters))) {
        if (is_ru_vowel(w.letters[i])) {
            return w.stressed[i] ? i : -1
        }
    }
    return -1
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
    let last_vowel = next_vowel_if_stressed(w, i + 1) < 0 && next_st < 0
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
        if (!(by_sign && !soft_by_vowel(w, i) && key_exists(RU_HARD_BEFORE_SIGN, raw) && !(final && next == NextWord.vowel))) {
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
    return mark_ordinal(part, 1)
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

//! The stress pack: the file bytes (the tables are searched in place), the stress table (bare
//! form -> its stressed vowel ordinals, the first the default) and the ё table (a form written
//! with е -> its ё spelling).
struct RuStressPack {
    data : array<uint8>
    source : string
    stress : LexTable
    yo : LexTable
}

let private RU_PACK_MAGIC = 0x55524754u
let private RU_PACK_VERSION = 1

//! the prepositions, conjunctions and particles that lean on the next word and take no stress of their own
let private RU_CLITICS <- { "и", "а", "но", "да", "не", "ни", "же", "ж", "бы", "б", "ли", "ль", "на", "за", "по", "под", "подо",
    "над", "надо", "об", "обо", "о", "от", "ото", "до", "из", "изо", "без", "безо", "при", "про", "для", "с", "со", "в", "во",
    "к", "ко", "у", "что", "уж", "аж", "перед", "передо", "через", "сквозь", "между", "ради" }
//! Load tts_g2p_ru.bin. Panics on a missing file, a wrong magic or an unsupported version.
def load_ru_stress_pack(path : string) : RuStressPack {
    var p = RuStressPack()
    p.data <- pack_read_bytes(path)
    if (empty(p.data)) {
        panic("dasLLAMA tts: cannot open the Russian stress pack '{path}'")
    }
    var o = 0l
    p.source = read_pack_header(p.data, path, "tts_g2p_ru.bin", "harness/build_g2p_ru_data.py", RU_PACK_MAGIC, RU_PACK_VERSION, o)
    p.stress = read_lex_table(p.data, o)
    p.yo = read_lex_table(p.data, o)
    return <- p
}

def ru_pack_loaded(p : RuStressPack) : bool => !empty(p.data)

//! the 1-based stressed-vowel ordinals the pack holds for a lowercase form, the default first; empty when it holds none
def ru_stress_variants(p : RuStressPack; word : string) : array<int> {
    var inscope out : array<int>
    return <- out if (empty(p.data))
    let i = lex_find(p.data, p.stress, word)
    return <- out if (i < 0)
    var a = 0l
    var b = 0l
    lex_value_span(p.data, p.stress, i, a, b)
    let n = rd_u8(p.data, a)
    out |> reserve(n)
    for (k in range(n)) {
        out |> push(rd_u8(p.data, a + 1l + int64(k)))
    }
    return <- out
}

//! the ё spelling of a form written with е, "" when the pack has none for it
def ru_yo_form(p : RuStressPack; word : string) : string {
    return "" if (empty(p.data))
    let i = lex_find(p.data, p.yo, word)
    return "" if (i < 0)
    var a = 0l
    var b = 0l
    lex_value_span(p.data, p.yo, i, a, b)
    return bytes_str(p.data, a, int(b - a))
}

def private has_mark(cps : array<int>) : bool {
    for (cp in cps) {
        return true if (cp == PLUS || cp == ACUTE || cp == CYR_YO || cp == CYR_YO_CAP)
    }
    return false
}

//! `word` with `+` before its vowel of ordinal `ordinal` (1-based)
def private mark_ordinal(word : string; ordinal : int) : string {
    var inscope cps <- utf8_to_cpts(word)
    var n = 0
    for (i, cp in iter_range(cps), cps) {
        if (is_ru_vowel(int(utf32_to_lower(uint(cp))))) {
            n++
            if (n == ordinal) {
                return "{cpts_to_utf8(cps, 0, i)}+{cpts_to_utf8(cps, i, length(cps))}"
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

//! the pack's default ordinal for a lowercase form, a reflexive read through its base form; 0 where the pack has neither
def private pack_ordinal(p : RuStressPack; lower : string) : int {
    var inscope variants <- ru_stress_variants(p, lower)
    return variants[0] if (!empty(variants))
    return 0 if (!(lower |> ends_with("ся")) && !(lower |> ends_with("сь")))
    var inscope lcps <- utf8_to_cpts(lower)
    var inscope base <- ru_stress_variants(p, cpts_to_utf8(lcps, 0, length(lcps) - 2))
    return empty(base) ? 0 : base[0]
}

//! One word of running text stress-marked: the text's own mark wins, then the clitics, the pack's ё spelling, its default reading, the shape rule; a hyphenated word side by side.
def ru_mark_word(p : RuStressPack; word : string) : string {
    if (find(word, "-") >= 0) {
        var inscope parts <- split(word, "-")
        for (part in parts) {
            part = ru_mark_word(p, part)
        }
        return join(parts, "-")
    }
    var inscope cps <- utf8_to_cpts(word)
    return word if (empty(cps) || has_mark(cps))
    let lower = utf8_to_lower(word)
    return word if (key_exists(RU_CLITICS, lower))
    let yo = ru_yo_form(p, lower)
    return yo if (!empty(yo))   // the ё carries the stress
    let known = pack_ordinal(p, lower)
    let ordinal = known > 0 ? known : guess_stress_ordinal(lower)
    return ordinal <= 0 ? word : mark_ordinal(word, ordinal)
}

//! Running text with every Russian word stress-marked (`ru_mark_word`), everything else as it was.
def ru_mark_stress(p : RuStressPack; text : string) : string {
    var inscope cps <- utf8_to_cpts(text)
    let n = length(cps)
    var inscope cur : array<int>
    return build_string() $(var wr) {
        var i = 0
        while (i <= n) {
            let cp = i < n ? cps[i] : -1
            let hyphen_inside = cp == '-' && !empty(cur) && i + 1 < n && (cpt_is_cyrillic(cps[i + 1]) || cps[i + 1] == PLUS)
            if (cp >= 0 && (cpt_is_cyrillic(cp) || cp == PLUS || cp == ACUTE || hyphen_inside)) {
                cur |> push(cp)
            } else {
                if (!empty(cur)) {
                    wr |> write(ru_mark_word(p, cpts_to_utf8(cur)))
                    cur |> clear()
                }
                if (cp >= 0) {
                    wr |> write(cpt_to_utf8(cp))
                }
            }
            i++
        }
    }
}

//! Normalized Russian text to phonemes: the stress pass, then letter-to-sound.
def ru_phonemize(p : RuStressPack; text : string) : string => ru_phonemize_marked(ru_mark_stress(p, text))
