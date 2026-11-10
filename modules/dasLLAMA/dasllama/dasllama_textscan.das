options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_textscan shared public

require dasllama/dasllama_lint public
require strings
require daslib/strings_boost
require dasllama/dasllama_unicode

//! The codepoint scanning the text front end's passes share, in no language: a bounds-safe read,
//! the digit, whitespace and dash classes, the run ends both ways, a digit run's value, and the
//! pieces of a pass's output - a string emitted as codepoints, a span taken back as a string.

def cp_at(c : array<int>; i : int) : int => (i >= 0 && i < length(c)) ? c[i] : -1
def is_digit(cp : int) : bool => is_number(cp)
def is_space(cp : int) : bool => cp >= 0 && cpt_is_whitespace(cp)

let EN_DASH = 8211
let EM_DASH = 8212
//! the hyphen and the two dashes
def is_dash(cp : int) : bool => cp == '-' || cp == EN_DASH || cp == EM_DASH

//! the index past the digit run starting at `i`
def digits_end(c : array<int>; i : int) : int {
    var j = i
    while (is_digit(cp_at(c, j))) {
        j++
    }
    return j
}

//! the index past the whitespace run starting at `i`
def spaces_end(c : array<int>; i : int) : int {
    var j = i
    while (is_space(cp_at(c, j))) {
        j++
    }
    return j
}

//! the start of the whitespace run ending at `i` (`i` itself when none ends there)
def spaces_start(c : array<int>; i : int) : int {
    var j = i
    while (j > 0 && is_space(c[j - 1])) {
        j--
    }
    return j
}

//! the last codepoint before `i` that is not whitespace; -1 at the start of the text
def prev_nonspace(c : array<int>; i : int) : int => cp_at(c, spaces_start(c, i) - 1)

//! the value of the digits in [a, b); any other codepoint among them is skipped
def digits_value(c : array<int>; a, b : int) : int64 {
    var v = 0l
    for (i in range(a, b)) {
        if (is_digit(c[i])) {
            v = v * 10l + int64(c[i] - '0')
        }
    }
    return v
}

//! the most digits a run reads as one cardinal; a longer run is read digit by digit, and its value never computed
let MAX_DIGITS_AS_NUMBER = 18

def cpts_str(c : array<int>; a, b : int) : string => cpts_to_utf8(c, a, b)

def emit(var out : array<int>; s : string) {
    var inscope cps <- utf8_to_cpts(s)
    out |> push_from(cps)
}

//! every digit of `s` as its own word from a ten-word table: "102" -> "one zero two"
def digits_as_words(s : string; words : array<string>) : string {
    var inscope cps <- utf8_to_cpts(s)
    var inscope parts <- [for (cp in cps); words[cp - '0']; where is_digit(cp)]
    return join(parts, " ")
}
