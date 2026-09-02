options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_textnorm shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/strings_boost
require dasllama/dasllama_unicode

//! English text normalization for the TTS front end: numbers, ordinals, years and decades,
//! currency, percentages, clock times, dates, fractions, units, abbreviations, URLs and
//! e-mail addresses become the words a reader says; everything else keeps only the
//! punctuation that carries prosody. Runs before tokenization and G2P.

let private ONES <- ["", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
    "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"]
let private TENS <- ["", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"]
let private SCALE <- ["", "thousand", "million", "billion", "trillion", "quadrillion", "quintillion"]
let private DIGIT_WORDS <- ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]
let private DECADE_WORDS <- ["hundreds", "tens", "twenties", "thirties", "forties", "fifties", "sixties", "seventies", "eighties", "nineties"]

let private ORDINAL_EXCEPTIONS <- {
    "one" => "first", "two" => "second", "three" => "third", "four" => "fourth", "five" => "fifth",
    "six" => "sixth", "seven" => "seventh", "eight" => "eighth", "nine" => "ninth", "twelve" => "twelfth"
}

let private CURRENCY_UNITS <- {
    '$' => "dollar", 8364 => "euro", 163 => "pound", 165 => "yen", 8377 => "rupee", 8361 => "won", 8383 => "bitcoin"
}
let private SCALE_SUFFIX <- { 'K' => "thousand", 'M' => "million", 'B' => "billion", 'T' => "trillion" }

let private MONTHS <- {
    "jan" => "January", "january" => "January", "feb" => "February", "february" => "February",
    "mar" => "March", "march" => "March", "apr" => "April", "april" => "April", "may" => "May",
    "jun" => "June", "june" => "June", "jul" => "July", "july" => "July", "aug" => "August", "august" => "August",
    "sep" => "September", "sept" => "September", "september" => "September", "oct" => "October", "october" => "October",
    "nov" => "November", "november" => "November", "dec" => "December", "december" => "December"
}
let private MONTH_ABBREVIATIONS <- { "jan", "feb", "mar", "apr", "jun", "jul", "aug", "sep", "sept", "oct", "nov", "dec" }

let private ABBREVIATIONS <- {
    "dr" => "Doctor", "prof" => "Professor", "mr" => "Mister", "mrs" => "Misses", "ms" => "Ms",
    "fig" => "Figure", "figs" => "Figures", "pp" => "pages", "p" => "page", "ch" => "chapter", "sec" => "section"
}

let private UNITS <- {
    "km" => "kilometers", "kg" => "kilograms", "mg" => "milligrams", "ml" => "milliliters",
    "gb" => "gigabytes", "mb" => "megabytes", "kb" => "kilobytes", "tb" => "terabytes",
    "hz" => "hertz", "khz" => "kilohertz", "mhz" => "megahertz", "ghz" => "gigahertz",
    "mph" => "miles per hour", "kph" => "kilometers per hour",
    "ms" => "milliseconds", "ns" => "nanoseconds", "µs" => "microseconds",   // nolint:STYLE039
    "°c" => "degrees Celsius", "c°" => "degrees Celsius", "°f" => "degrees Fahrenheit", "f°" => "degrees Fahrenheit"   // nolint:STYLE039
}
//! denominators a "N/D" is read as a spoken fraction for; anything else stays a slash
let private FRACTION_DENOMINATORS <- { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 32, 64, 100 }

let private PROSODY_PUNCT <- { '.', ',', '?', '!', ';', ':', '-', 8212, 8211, 8230, '\'' }

// ---------------------------------------------------------------- number words

def private three_digits_to_words(n : int) : string {
    return "" if (n == 0)
    var inscope parts : array<string>
    let hundreds = n / 100
    let remainder = n % 100
    if (hundreds != 0) {
        parts |> push("{ONES[hundreds]} hundred")
    }
    if (remainder < 20) {
        if (remainder != 0) {
            parts |> push(ONES[remainder])
        }
    } else {
        let tens_word = TENS[remainder / 10]
        let ones_word = ONES[remainder % 10]
        parts |> push(empty(ones_word) ? tens_word : "{tens_word}-{ones_word}")
    }
    return join(parts, " ")
}

//! Cardinal words: 1200 -> "twelve hundred", 1000 -> "one thousand", -42 -> "negative forty-two".
def number_to_words(n : int64) : string {
    return "zero" if (n == 0l)
    return "negative {number_to_words(-n)}" if (n < 0l)
    if (n >= 100l && n <= 9999l && n % 100l == 0l && n % 1000l != 0l) {
        let hundreds = int(n / 100l)
        return "{ONES[hundreds]} hundred" if (hundreds < 20)
    }
    var inscope parts : array<string>
    var rest = n
    for (scale in SCALE) {
        let chunk = int(rest % 1000l)
        if (chunk != 0) {
            let words = three_digits_to_words(chunk)
            parts |> push(empty(scale) ? words : "{words} {scale}")
        }
        rest /= 1000l
        break if (rest == 0l)
    }
    return build_string() $(var writer) {
        var i = length(parts) - 1
        while (i >= 0) {
            writer |> write(parts[i])
            if (i > 0) {
                writer |> write(" ")
            }
            i--
        }
    }
}

//! Ordinal words: 1 -> "first", 21 -> "twenty-first", 20 -> "twentieth", 100 -> "one hundredth".
def ordinal_words(n : int64) : string {
    let word = number_to_words(n)
    var prefix = ""
    var last = word
    var joiner = ""
    let hyphen = rfind(word, "-")
    let space = rfind(word, " ")
    if (hyphen >= 0) {
        prefix = slice(word, 0, hyphen)
        last = slice(word, hyphen + 1)
        joiner = "-"
    } elif (space >= 0) {
        prefix = slice(word, 0, space)
        last = slice(word, space + 1)
        joiner = " "
    }
    var last_ord = ORDINAL_EXCEPTIONS?[last] ?? ""
    if (empty(last_ord)) {
        if (ends_with(last, "y")) {
            last_ord = "{slice(last, 0, length(last) - 1)}ieth"
        } elif (ends_with(last, "t")) {
            last_ord = "{last}h"
        } elif (ends_with(last, "e")) {
            last_ord = "{slice(last, 0, length(last) - 1)}th"
        } else {
            last_ord = "{last}th"
        }
    }
    return empty(prefix) ? last_ord : "{prefix}{joiner}{last_ord}"
}

def private digit_string_value(s : string) : int64 {
    var v = 0l
    peek_data(s) $(d : array<uint8> const#) {
        for (b in d) {
            let cp = int(b)
            if (is_number(cp)) {
                v = v * 10l + int64(cp - '0')
            }
        }
    }
    return v
}

//! Decimal words, reading the fraction digit by digit: "3.14" -> "three point one four",
//! "1.50" -> "one point five zero", "-0.5" -> "negative zero point five".
def float_to_words(text : string) : string {
    var body = text
    let negative = starts_with(body, "-")
    if (negative) {
        body = slice(body, 1)
    }
    let dot = find(body, '.')
    var result = ""
    if (dot >= 0) {
        let int_part = slice(body, 0, dot)
        let dec_part = slice(body, dot + 1)
        let int_words = empty(int_part) ? "zero" : number_to_words(digit_string_value(int_part))
        let dec_words = build_string() $(var writer) {
            var first = true
            peek_data(dec_part) $(d : array<uint8> const#) {
                for (b in d) {
                    let cp = int(b)
                    if (is_number(cp)) {
                        if (!first) {
                            writer |> write(" ")
                        }
                        writer |> write(DIGIT_WORDS[cp - '0'])
                        first = false
                    }
                }
            }
        }
        result = "{int_words} point {dec_words}"
    } else {
        result = number_to_words(digit_string_value(body))
    }
    return negative ? "negative {result}" : result
}

//! "1.50" as a float literal prints "1.5": the fraction drops its trailing zeros, keeping one.
def private float_literal_words(raw : string) : string {
    let dot = find(raw, '.')
    return float_to_words(raw) if (dot < 0)
    var end = length(raw)
    peek_data(raw) $(d : array<uint8> const#) {
        while (end > dot + 2 && int(d[end - 1]) == '0') {
            end--
        }
    }
    return float_to_words(slice(raw, 0, end))
}

//! Years read the way they are said: 1999 -> "nineteen ninety-nine", 2005 -> "two thousand five",
//! 2021 -> "twenty twenty-one", 1900 -> "nineteen hundred".
def year_to_words(year : int64) : string {
    if (year >= 1900l && year <= 1999l) {
        let rest = year % 100l
        return rest != 0l ? "nineteen {number_to_words(rest)}" : "nineteen hundred"
    }
    if (year >= 2000l && year <= 2009l) {
        let rest = year % 100l
        return rest != 0l ? "two thousand {number_to_words(rest)}" : "two thousand"
    }
    if (year >= 2010l && year <= 2099l) {
        let rest = year % 100l
        return rest != 0l ? "twenty {number_to_words(rest)}" : "twenty hundred"
    }
    return number_to_words(year)
}

def private is_year(v : int64) : bool => v >= 1900l && v <= 2099l

//! A bare number in running text: a decimal reads digit by digit, a four-digit value in
//! 1900..2099 reads as a year, anything else as a cardinal.
def number_or_year_to_words(raw : string) : string {
    let cleaned = replace(raw, ",", "")
    return float_to_words(cleaned) if (find(cleaned, '.') >= 0)
    let negative = starts_with(cleaned, "-")
    let value = digit_string_value(cleaned)
    return "negative {number_to_words(value)}" if (negative)
    return is_year(value) ? year_to_words(value) : number_to_words(value)
}

// ---------------------------------------------------------------- codepoint scanning

def private cp_at(c : array<int>; i : int) : int => (i >= 0 && i < length(c)) ? c[i] : -1
def private is_digit(cp : int) : bool => is_number(cp)
def private is_ascii_alpha(cp : int) : bool => cp < 128 && is_alpha(cp)
def private is_word(cp : int) : bool => cp == '_' || is_digit(cp) || (cp >= 128 ? (cpt_is_letter(cp) || cpt_is_number(cp)) : is_ascii_alpha(cp))
def private is_space(cp : int) : bool => cp >= 0 && cpt_is_whitespace(cp)
def private at_boundary(c : array<int>; i : int) : bool => is_word(cp_at(c, i - 1)) != is_word(cp_at(c, i))
def private lower_ascii(cp : int) : int => (cp >= 'A' && cp <= 'Z') ? cp + 32 : cp

def private digits_end(c : array<int>; i : int) : int {
    var j = i
    while (is_digit(cp_at(c, j))) {
        j++
    }
    return j
}

def private spaces_end(c : array<int>; i : int) : int {
    var j = i
    while (is_space(cp_at(c, j))) {
        j++
    }
    return j
}

def private ascii_alpha_end(c : array<int>; i : int) : int {
    var j = i
    while (is_ascii_alpha(cp_at(c, j))) {
        j++
    }
    return j
}

def private digits_value(c : array<int>; a, b : int) : int64 {
    var v = 0l
    for (i in range(a, b)) {
        if (is_digit(c[i])) {
            v = v * 10l + int64(c[i] - '0')
        }
    }
    return v
}

def private cpts_str(c : array<int>; a, b : int) : string => cpts_to_utf8(c, a, b)

def private lower_str(c : array<int>; a, b : int) : string {
    var inscope lowered <- [for (i in range(a, b)); lower_ascii(c[i])]
    return cpts_str(lowered, 0, length(lowered))
}

def private emit(var out : array<int>; s : string) {
    var inscope cps <- utf8_to_cpts(s)
    out |> push_from(cps)
}

def private emit_range(var out : array<int>; c : array<int>; a, b : int) {
    out |> reserve(length(out) + (b - a))
    for (i in range(a, b)) {
        out |> push(c[i])
    }
}

//! Spell a URL or e-mail piece character by character: letters by name, digits as words,
//! ". - _ @ / ? & =" as "dot dash underscore at slash question mark and equals".
def private spell_characters(c : array<int>; a, b : int) : string {
    var inscope parts : array<string>
    for (i in range(a, b)) {
        let cp = c[i]
        if (is_digit(cp)) {
            parts |> push(DIGIT_WORDS[cp - '0'])
        } elif (is_ascii_alpha(cp) || (cp >= 128 && cpt_is_letter(cp))) {
            parts |> push(cpts_str([lower_ascii(cp)], 0, 1))
        } elif (cp == '.') {
            parts |> push("dot")
        } elif (cp == '-') {
            parts |> push("dash")
        } elif (cp == '_') {
            parts |> push("underscore")
        } elif (cp == '@') {
            parts |> push("at")
        } elif (cp == '/') {
            parts |> push("slash")
        } elif (cp == '?') {
            parts |> push("question mark")
        } elif (cp == '&') {
            parts |> push("and")
        } elif (cp == '=') {
            parts |> push("equals")
        }
    }
    return join(parts, " ")
}

def private starts_with_ascii_ci(c : array<int>; i : int; what : string) : bool {
    var k = 0
    var ok = true
    peek_data(what) $(d : array<uint8> const#) {
        for (b in d) {
            if (lower_ascii(cp_at(c, i + k)) != int(b)) {
                ok = false
            }
            k++
        }
    }
    return ok
}

def private starts_with_ascii(c : array<int>; i : int; what : string) : bool {
    var k = 0
    var ok = true
    peek_data(what) $(d : array<uint8> const#) {
        for (b in d) {
            if (cp_at(c, i + k) != int(b)) {
                ok = false
            }
            k++
        }
    }
    return ok
}

// ---------------------------------------------------------------- the passes

//! `<[^>]+>` -> " "
def private pass_html(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (c[i] == '<') {
            var j = i + 1
            while (j < n && c[j] != '>') {
                j++
            }
            if (j < n && j > i + 1) {
                out |> push(' ')
                i = j + 1
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `https?://\S+|www\.\S+` -> the address spelled out
def private pass_url(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        var skip = -1
        if (starts_with_ascii(c, i, "http://")) {
            skip = 7
        } elif (starts_with_ascii(c, i, "https://")) {
            skip = 8
        } elif (starts_with_ascii(c, i, "www.")) {
            skip = 0
        }
        if (skip >= 0) {
            var j = i
            while (j < n && !is_space(c[j])) {
                j++
            }
            if (j > i + max(skip, 4)) {
                var inscope body : array<int>
                emit_range(body, c, i + skip, j)
                if (starts_with_ascii_ci(body, 0, "www.")) {
                    var inscope spelled : array<int>
                    emit(spelled, "www dot ")
                    emit_range(spelled, body, 4, length(body))
                    emit(out, spell_characters(spelled, 0, length(spelled)))
                } else {
                    emit(out, spell_characters(body, 0, length(body)))
                }
                i = j
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

def private is_email_local(cp : int) : bool => is_word(cp) || cp == '.' || cp == '+' || cp == '-'
def private is_email_domain(cp : int) : bool => is_word(cp) || cp == '-'

//! `\b[\w.+-]+@[\w-]+\.[a-z]{2,}\b` -> "local at domain dot tld", spelled out
def private pass_email(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_email_local(c[i]) && at_boundary(c, i)) {
            var j = i
            while (j < n && is_email_local(c[j])) {
                j++
            }
            if (cp_at(c, j) == '@') {
                let at = j
                var k = at + 1
                while (k < n && is_email_domain(c[k])) {
                    k++
                }
                if (k > at + 1 && cp_at(c, k) == '.') {
                    var e = k + 1
                    while (e < n && is_ascii_alpha(c[e])) {
                        e++
                    }
                    if (e >= k + 3 && at_boundary(c, e)) {
                        emit(out, "{spell_characters(c, i, at)} at {spell_characters(c, at + 1, e)}")
                        i = e
                        continue
                    }
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! A month name at `i` (full, or an abbreviation with its optional dot): returns the index past it
//! and the canonical month name, or -1.
def private match_month(c : array<int>; i : int; var name : string&) : int {
    return -1 if (!is_ascii_alpha(c[i]) || !at_boundary(c, i))
    let j = ascii_alpha_end(c, i)
    let key = lower_str(c, i, j)
    name = MONTHS?[key] ?? ""
    return -1 if (empty(name))
    return (key_exists(MONTH_ABBREVIATIONS, key) && cp_at(c, j) == '.') ? j + 1 : j
}

def private is_ordinal_suffix(c : array<int>; i : int) : bool {
    let a = lower_ascii(cp_at(c, i))
    let b = lower_ascii(cp_at(c, i + 1))
    return (a == 's' && b == 't') || (a == 'n' && b == 'd') || (a == 'r' && b == 'd') || (a == 't' && b == 'h')
}

//! `\d{4}\b` at `i`: the index past the year, or -1
def private match_year(c : array<int>; i : int) : int {
    let e = digits_end(c, i)
    return (e == i + 4 && !is_word(cp_at(c, e))) ? e : -1
}

//! "Month D(st|nd|rd|th)?(,)? YYYY" -> "Month ordinal, year"; then "Month YYYY" -> "Month year"
def private pass_dates(c : array<int>; with_day : bool) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        var month = ""
        let m = match_month(c, i, month)
        if (m > 0) {
            let s = spaces_end(c, m)
            if (s > m) {
                if (with_day) {
                    let d = digits_end(c, s)
                    if (d > s && d <= s + 2) {
                        var k = d
                        if (is_ordinal_suffix(c, k)) {
                            k += 2
                        }
                        if (cp_at(c, k) == ',') {
                            k++
                        }
                        let s2 = spaces_end(c, k)
                        let y = s2 > k ? match_year(c, s2) : -1
                        if (y > 0) {
                            emit(out, "{month} {ordinal_words(digits_value(c, s, d))}, {year_to_words(digits_value(c, s2, y))}")
                            i = y
                            continue
                        }
                    }
                } else {
                    let y = match_year(c, s)
                    if (y > 0) {
                        emit(out, "{month} {year_to_words(digits_value(c, s, y))}")
                        i = y
                        continue
                    }
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `\s*[ap]\.?m\.?\b` at `i` (the trailing dot only when a word follows it): returns the index
//! past the suffix, or -1
def private match_meridiem(c : array<int>; i : int; var letter : int&) : int {
    let s = spaces_end(c, i)
    let a = lower_ascii(cp_at(c, s))
    return -1 if (a != 'a' && a != 'p')
    var k = s + 1
    if (cp_at(c, k) == '.') {
        k++
    }
    return -1 if (lower_ascii(cp_at(c, k)) != 'm')
    k++
    letter = a
    return k + 1 if (cp_at(c, k) == '.' && is_word(cp_at(c, k + 1)))
    return at_boundary(c, k) ? k : -1
}

def private time_words(h_in, mins, secs : int64; letter : int) : string {
    var h = h_in
    if (letter != 0 && h > 12l) {
        h -= 12l
    }
    let h_words = number_to_words(h)
    var result = h_words
    if (mins != 0l) {
        result = mins < 10l ? "{h_words} oh {number_to_words(mins)}" : "{h_words} {number_to_words(mins)}"
    }
    if (secs >= 0l) {
        result = "{result} and {number_to_words(secs)} seconds"
    }
    if (letter == 'a') {
        result = "{result} a m"
    } elif (letter == 'p') {
        result = "{result} p m"
    }
    return result
}

//! `\b(\d{1,2}):(\d{2})(?::(\d{2}))?(?:\s*(a\.?m\.?|p\.?m\.?))?\b` -> "three thirty p m"
def private pass_time(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && at_boundary(c, i)) {
            let h_end = digits_end(c, i)
            if (h_end <= i + 2 && cp_at(c, h_end) == ':') {
                let m_end = digits_end(c, h_end + 1)
                if (m_end == h_end + 3) {
                    var secs = -1l
                    var e = m_end
                    if (cp_at(c, m_end) == ':' && digits_end(c, m_end + 1) == m_end + 3) {
                        secs = digits_value(c, m_end + 1, m_end + 3)
                        e = m_end + 3
                    }
                    var letter = 0
                    var t = match_meridiem(c, e, letter)
                    if (t < 0 && secs >= 0l && !at_boundary(c, e)) {
                        secs = -1l
                        e = m_end
                        t = match_meridiem(c, e, letter)
                    }
                    if (t < 0 && at_boundary(c, e)) {
                        t = e
                    }
                    if (t >= 0) {
                        emit(out, time_words(digits_value(c, i, h_end), digits_value(c, h_end + 1, m_end), secs, letter))
                        i = t
                        continue
                    }
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `[\d,]+(?:\.\d+)?` at `i` with at least one digit: the index past the amount, or -1
def private match_amount(c : array<int>; i : int) : int {
    var j = i
    var digits = 0
    while (is_digit(cp_at(c, j)) || cp_at(c, j) == ',') {
        if (is_digit(c[j])) {
            digits++
        }
        j++
    }
    return -1 if (digits == 0)
    if (cp_at(c, j) == '.') {
        let d = digits_end(c, j + 1)
        if (d > j + 1) {
            j = d
        }
    }
    return j
}

def private amount_words(raw : string) : string {
    return find(raw, '.') >= 0 ? float_to_words(raw) : number_to_words(digit_string_value(raw))
}

def private plural(unit : string; count : int64) : string => count != 1l ? "{unit}s" : unit

//! "$1,200.50" -> "twelve hundred dollars and fifty cents", "$85K" -> "eighty-five thousand dollars"
def private currency_words(unit : string; raw : string; scale : string) : string {
    if (!empty(scale)) {
        let num = find(raw, '.') >= 0 ? float_to_words(raw) : number_to_words(digit_string_value(raw))
        return empty(unit) ? "{num} {scale}" : "{num} {scale} {unit}s"
    }
    let dot = find(raw, '.')
    if (dot >= 0) {
        let int_value = digit_string_value(slice(raw, 0, dot))
        var cents_text = slice(raw, dot + 1)
        cents_text = length(cents_text) >= 2 ? slice(cents_text, 0, 2) : "{cents_text}0"
        let cents = digit_string_value(cents_text)
        let int_words = number_to_words(int_value)
        var result = empty(unit) ? int_words : "{int_words} {plural(unit, int_value)}"
        if (cents != 0l) {
            result = "{result} and {number_to_words(cents)} {plural("cent", cents)}"
        }
        return result
    }
    let value = digit_string_value(raw)
    let words = number_to_words(value)
    return empty(unit) ? words : "{words} {plural(unit, value)}"
}

//! `([$€£¥₹₩₿])\s*([\d,]+(?:\.\d+)?)\s*([KMBT])?(?![a-zA-Z\d])` -> the amount in words
def private pass_currency(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let unit = CURRENCY_UNITS?[c[i]] ?? ""
        if (!empty(unit)) {
            let s = spaces_end(c, i + 1)
            let a = match_amount(c, s)
            if (a > 0) {
                let raw = replace(cpts_str(c, s, a), ",", "")
                var e = a
                var scale = ""
                let k = spaces_end(c, a)
                let suffix = SCALE_SUFFIX?[cp_at(c, k)] ?? ""
                if (!empty(suffix) && !is_ascii_alpha(cp_at(c, k + 1)) && !is_digit(cp_at(c, k + 1))) {
                    scale = suffix
                    e = k + 1
                }
                if (!empty(scale) || (!is_ascii_alpha(cp_at(c, a)) && !is_digit(cp_at(c, a)))) {
                    emit(out, currency_words(unit, raw, scale))
                    i = e
                    continue
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `(-?[\d,]+(?:\.\d+)?)\s*%` -> "fifty percent", "three point five percent"
def private pass_percent(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let neg = c[i] == '-'
        let s = neg ? i + 1 : i
        if (is_digit(cp_at(c, s)) || cp_at(c, s) == ',') {
            let a = match_amount(c, s)
            if (a > 0 && cp_at(c, spaces_end(c, a)) == '%') {
                let raw = replace(cpts_str(c, s, a), ",", "")
                let words = find(raw, '.') >= 0 ? float_literal_words(raw) : number_to_words(digit_string_value(raw))
                emit(out, neg ? "negative {words} percent" : "{words} percent")
                i = spaces_end(c, a) + 1
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `\b(\d+)(st|nd|rd|th)\b` -> "twenty-first"
def private pass_ordinals(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && at_boundary(c, i)) {
            let d = digits_end(c, i)
            if (is_ordinal_suffix(c, d) && !is_word(cp_at(c, d + 2))) {
                emit(out, ordinal_words(digits_value(c, i, d)))
                i = d + 2
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `\bet\s+al\.` -> "et al"; `\b(Dr|Prof|Mr|Mrs|Ms|Fig|Figs|pp|p|ch|sec)\.` -> the full word.
//! A "p." that opens "p.m." is the meridiem, not a page.
def private pass_abbreviations(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_ascii_alpha(c[i]) && at_boundary(c, i)) {
            let j = ascii_alpha_end(c, i)
            let key = lower_str(c, i, j)
            if (key == "et") {
                let s = spaces_end(c, j)
                if (s > j && starts_with_ascii_ci(c, s, "al.")) {
                    emit(out, "et al")
                    i = s + 3
                    continue
                }
            }
            if (cp_at(c, j) == '.') {
                let expansion = ABBREVIATIONS?[key] ?? ""
                let meridiem = key == "p" && lower_ascii(cp_at(c, j + 1)) == 'm' && cp_at(c, j + 2) == '.'
                if (!empty(expansion) && !meridiem) {
                    emit(out, expansion)
                    i = j + 1
                    continue
                }
            }
            emit_range(out, c, i, j)
            i = j
            continue
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! "1.2.3" -> "one point two point three", "v2.1.0" -> "v two point one point zero"
def private version_words(c : array<int>; a, b : int) : string {
    var inscope parts : array<string>
    var start = a
    var prefix = ""
    if (lower_ascii(cp_at(c, a)) == 'v') {
        prefix = "v "
        start = a + 1
    }
    var k = start
    while (k < b) {
        let d = digits_end(c, k)
        if (d > k) {
            parts |> push(number_to_words(digits_value(c, k, d)))
            k = d
        } else {
            k++
        }
    }
    return "{prefix}{join(parts, " point ")}"
}

//! `\d[\d.]*` at `i` that keeps at least `min_groups` "digits.digits" groups and never ends on a
//! dot: the index past the version, or -1
def private match_version_digits(c : array<int>; i : int; min_groups : int; need_boundary : bool) : int {
    var e = digits_end(c, i)
    return -1 if (e == i)
    var groups = 0
    var inscope ends : array<int>
    ends |> push(e)
    while (cp_at(c, e) == '.' && is_digit(cp_at(c, e + 1))) {
        e = digits_end(c, e + 1)
        groups++
        ends |> push(e)
    }
    var pick = length(ends) - 1
    while (pick >= 0 && pick >= min_groups && need_boundary && is_word(cp_at(c, ends[pick]))) {
        pick--
    }
    return (pick >= 0 && pick >= min_groups) ? ends[pick] : -1
}

//! `\b[vV]?\d+(?:\.\d+){2,}\b` -> "v two point one point zero"
def private pass_versions(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (at_boundary(c, i)) {
            let start = (lower_ascii(c[i]) == 'v') ? i + 1 : i
            if (is_digit(cp_at(c, start))) {
                let e = match_version_digits(c, start, 2, true)
                if (e > 0) {
                    emit(out, version_words(c, i, e))
                    i = e
                    continue
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

def private is_unit_char(cp : int) : bool => is_ascii_alpha(cp) || cp == 176 || cp == 181

//! `(\d+(?:\.\d+)?)\s*(km|kg|...|°C|ms|µs)\b` -> "five gigabytes", "twenty-five degrees Celsius"
def private pass_units(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && !is_ascii_alpha(cp_at(c, i - 1))) {
            var a = digits_end(c, i)
            if (cp_at(c, a) == '.' && is_digit(cp_at(c, a + 1))) {
                a = digits_end(c, a + 1)
            }
            let s = spaces_end(c, a)
            var u = s
            while (is_unit_char(cp_at(c, u))) {
                u++
            }
            if (u > s && !is_word(cp_at(c, u))) {
                let expansion = UNITS?[lower_str(c, s, u)] ?? ""
                if (!empty(expansion)) {
                    emit(out, "{float_literal_words(cpts_str(c, i, a))} {expansion}")
                    i = u
                    continue
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `\b(\d{1,3})0s\b` -> "the eighties", "the nineteen sixties"; a leading apostrophe ('90s) goes too
def private pass_decades(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && at_boundary(c, i)) {
            let d = digits_end(c, i)
            let len = d - i
            if (len >= 2 && len <= 4 && c[d - 1] == '0' && cp_at(c, d) == 's' && !is_word(cp_at(c, d + 1))) {
                let base = int(digits_value(c, i, d - 1))
                let decade = DECADE_WORDS[base % 10]
                var words = decade
                if (base == 200) {
                    words = "two thousands"
                } elif (base >= 10) {
                    words = "{number_to_words(int64(base / 10))} {decade}"
                }
                if (!empty(out) && out[length(out) - 1] == '\'') {
                    out |> pop()
                }
                emit(out, words)
                i = d + 1
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

def private fraction_words(num, den : int64) : string {
    let num_words = number_to_words(num)
    var denom = ""
    if (den == 2l) {
        denom = num == 1l ? "half" : "halves"
    } elif (den == 4l) {
        denom = num == 1l ? "quarter" : "quarters"
    } else {
        denom = ordinal_words(den)
        if (num != 1l) {
            denom = "{denom}s"
        }
    }
    return "{num_words} {denom}"
}

//! `\b(\d+)\s*/\s*(\d+)\b`, proper and with a spoken denominator -> "one half", "three quarters", "five eighths"
def private pass_fractions(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && at_boundary(c, i)) {
            let d = digits_end(c, i)
            let s = spaces_end(c, d)
            if (cp_at(c, s) == '/') {
                let t = spaces_end(c, s + 1)
                let e = digits_end(c, t)
                if (e > t && !is_word(cp_at(c, e))) {
                    let num = digits_value(c, i, d)
                    let den = digits_value(c, t, e)
                    if (num >= 1l && num < den && key_exists(FRACTION_DENOMINATORS, int(den))) {
                        emit(out, fraction_words(num, den))
                        i = e
                        continue
                    }
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `(?<!\w)(\d+)-(\d+)(?!\w)` -> "ten to twenty", years read as years
def private pass_ranges(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && !is_word(cp_at(c, i - 1))) {
            let d = digits_end(c, i)
            if (cp_at(c, d) == '-' && is_digit(cp_at(c, d + 1))) {
                let e = digits_end(c, d + 1)
                if (!is_word(cp_at(c, e))) {
                    let lo = digits_value(c, i, d)
                    let hi = digits_value(c, d + 1, e)
                    if (is_year(lo) && is_year(hi)) {
                        emit(out, "{year_to_words(lo)} to {year_to_words(hi)}")
                    } else {
                        emit(out, "{number_to_words(lo)} to {number_to_words(hi)}")
                    }
                    i = e
                    continue
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `\b([a-zA-Z][a-zA-Z0-9]*)-(\d[\d.]*)` -> "GPT three point five", "COVID nineteen"
def private pass_model_names(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_ascii_alpha(c[i]) && at_boundary(c, i)) {
            var j = i + 1
            while (is_ascii_alpha(cp_at(c, j)) || is_digit(cp_at(c, j))) {
                j++
            }
            if (cp_at(c, j) == '-' && is_digit(cp_at(c, j + 1))) {
                let e = match_version_digits(c, j + 1, 0, false)
                if (e > 0) {
                    emit_range(out, c, i, j)
                    emit(out, " {version_words(c, j + 1, e)}")
                    i = e
                    continue
                }
            }
            emit_range(out, c, i, j)
            i = j
            continue
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! `-?\d+(,\d{3})*(\.\d+)?` not glued to a letter -> cardinal, year or decimal words
def private pass_numbers(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let neg = c[i] == '-' && is_digit(cp_at(c, i + 1))
        let s = neg ? i + 1 : i
        if (is_digit(cp_at(c, s)) && !is_ascii_alpha(cp_at(c, i - 1))) {
            var e = digits_end(c, s)
            while (cp_at(c, e) == ',' && digits_end(c, e + 1) == e + 4) {
                e += 4
            }
            if (cp_at(c, e) == '.' && is_digit(cp_at(c, e + 1))) {
                e = digits_end(c, e + 1)
            }
            emit(out, number_or_year_to_words(cpts_str(c, i, e)))
            i = e
            continue
        } elif (is_digit(c[i])) {
            emit_range(out, c, i, digits_end(c, i))
            i = digits_end(c, i)
            continue
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! Keep letters, digits, whitespace, the prosodic marks `. , ? ! ; : - — – …` and the apostrophe
//! (curly quotes fold to `'`); everything else becomes a space. Then whitespace collapses to
//! single spaces and the ends are trimmed.
def private pass_punctuation(c : array<int>) : array<int> {
    var out : array<int>
    var pending_space = false
    for (cp in c) {
        var keep = cp
        if (cp == 8216 || cp == 8217) {
            keep = '\''
        }
        if (is_space(keep) || !(is_word(keep) || key_exists(PROSODY_PUNCT, keep))) {
            pending_space = true
            continue
        }
        if (pending_space && !empty(out)) {
            out |> push(' ')
        }
        pending_space = false
        out |> push(keep)
    }
    return <- out
}

//! Normalize English text for speech: expand what is read as words, keep the punctuation that
//! carries prosody, collapse whitespace.
def private pass_dates_with_day(c : array<int>) : array<int> => pass_dates(c, true)
def private pass_dates_year_only(c : array<int>) : array<int> => pass_dates(c, false)

def normalize_text(text : string) : string {
    var inscope c <- utf8_to_cpts(text)
    let passes <- [@@pass_html, @@pass_url, @@pass_email, @@pass_dates_with_day, @@pass_dates_year_only,
        @@pass_time, @@pass_currency, @@pass_percent, @@pass_ordinals, @@pass_abbreviations, @@pass_versions,
        @@pass_units, @@pass_decades, @@pass_fractions, @@pass_ranges, @@pass_model_names, @@pass_numbers,
        @@pass_punctuation]
    for (step in passes) {
        var next <- invoke(step, c)
        delete c
        c <- next
    }
    return cpts_str(c, 0, length(c))
}
