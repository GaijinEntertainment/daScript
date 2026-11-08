options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options _ascii_strings = false   // the words a Russian reader says are the data

module dasllama_textnorm_ru shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/strings_boost
require daslib/utf8_utils
require dasllama/dasllama_unicode
require dasllama/dasllama_textscan

//! Russian text normalization for speech: numbers, decimals, percentages, money, units, clock
//! times, dates and years, the ordinal suffixes (5-й, 5-го), the common abbreviations and
//! symbols become the words a reader says, agreeing with the count where Russian agrees
//! (один рубль, два рубля, пять рублей); prosodic punctuation survives, everything else becomes a
//! space. Pure code, no data file. Case and gender beyond the nominative are given only where
//! the pattern fixes them - a year after в or before года, a day of the month, one and two before
//! a noun whose ending shows its gender - the rest is the nominative masculine a reader falls
//! back on.

let private UNITS_M <- ["ноль", "один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"]
let private TEENS <- ["десять", "одиннадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"]
let private TENS <- ["", "", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят", "восемьдесят", "девяносто"]
let private HUNDREDS <- ["", "сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот", "восемьсот", "девятьсот"]
let private ORD_UNITS <- ["", "первый", "второй", "третий", "четвёртый", "пятый", "шестой", "седьмой", "восьмой", "девятый"]
let private ORD_TEENS <- ["десятый", "одиннадцатый", "двенадцатый", "тринадцатый", "четырнадцатый", "пятнадцатый", "шестнадцатый", "семнадцатый", "восемнадцатый", "девятнадцатый"]
let private ORD_TENS <- ["", "", "двадцатый", "тридцатый", "сороковой", "пятидесятый", "шестидесятый", "семидесятый", "восьмидесятый", "девяностый"]
let private ORD_HUNDREDS <- ["", "сотый", "двухсотый", "трёхсотый", "четырёхсотый", "пятисотый", "шестисотый", "семисотый", "восьмисотый", "девятисотый"]
let private ORD_THOUSAND_PREFIX <- ["", "", "двух", "трёх", "четырёх", "пяти", "шести", "семи", "восьми", "девяти"]
let private MONTHS_GEN <- ["января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"]

//! The grammatical number a count takes: 1 (not 11) is `one`, 2-4 (not 12-14) is `few`, the rest `many`
def plural_ru(n : int64; one, few, many : string) : string {
    let last = int(abs(n) % 10l)
    let last2 = int(abs(n) % 100l)
    return one if (last == 1 && last2 != 11)
    return few if (last >= 2 && last <= 4 && (last2 < 12 || last2 > 14))
    return many
}

//! The gender one and two take: masculine, feminine (одна, две), neuter (одно)
enum RuGender {
    masculine
    feminine
    neuter
}

def private unit_word(d : int; gender : RuGender) : string {
    if (d == 1) {
        return gender == RuGender.feminine ? "одна" : (gender == RuGender.neuter ? "одно" : "один")
    }
    if (d == 2 && gender == RuGender.feminine) {
        return "две"
    }
    return UNITS_M[d]
}

def private under_thousand(n : int; gender : RuGender; var parts : array<string>) {
    let h = n / 100
    let t = (n % 100) / 10
    let u = n % 10
    if (h > 0) {
        parts |> push(HUNDREDS[h])
    }
    if (t == 1) {
        parts |> push(TEENS[u])
    } else {
        if (t >= 2) {
            parts |> push(TENS[t])
        }
        if (u > 0) {
            parts |> push(unit_word(u, gender))
        }
    }
}

//! A cardinal in the nominative, `gender` picking один/одна/одно and два/две; a negative opens with минус.
def number_to_words_ru(n : int64; gender : RuGender = RuGender.masculine) : string {
    return "ноль" if (n == 0l)
    var inscope parts : array<string>
    if (n < 0l) {
        parts |> push("минус")
    }
    var v = abs(n)
    let scales <- [["квинтиллион", "квинтиллиона", "квинтиллионов"], ["квадриллион", "квадриллиона", "квадриллионов"],
        ["триллион", "триллиона", "триллионов"], ["миллиард", "миллиарда", "миллиардов"], ["миллион", "миллиона", "миллионов"],
        ["тысяча", "тысячи", "тысяч"]]
    var scale = 1000000000000000000l
    for (s in scales) {
        let k = v / scale
        if (k > 0l) {
            let fem = scale == 1000l
            under_thousand(int(k), fem ? RuGender.feminine : RuGender.masculine, parts)
            parts |> push(plural_ru(k, s[0], s[1], s[2]))
            v -= k * scale
        }
        scale /= 1000l
    }
    under_thousand(int(v), gender, parts)
    return join(parts, " ")
}

//! The forms an ordinal takes, by its ending
enum RuOrdinalForm {
    nom_m    //! пятый
    gen_m    //! пятого - a day of the month, a year before года
    dat_m    //! пятому
    prep_m   //! пятом - a year after в
    instr_m  //! пятым
    nom_f    //! пятая
    obl_f    //! пятой
    acc_f    //! пятую
    nom_n    //! пятое
    nom_pl   //! пятые
    gen_pl   //! пятых
    dat_pl   //! пятым
    instr_pl //! пятыми
}

//! one ending per `RuOrdinalForm`, in the enum's order: the hard stems (пятый, второй), then the one soft stem (третий)
let private HARD_ENDINGS <- ["", "ого", "ому", "ом", "ым", "ая", "ой", "ую", "ое", "ые", "ых", "ым", "ыми"]
let private SOFT_ENDINGS <- ["", "ьего", "ьему", "ьем", "ьим", "ья", "ьей", "ью", "ье", "ьи", "ьих", "ьим", "ьими"]

//! `nom` is the nominative masculine; the stem keeps its hard or soft ending family (пятый/пятого, третий/третьего)
def private inflect_ordinal(nom : string; form : RuOrdinalForm) : string {
    return nom if (form == RuOrdinalForm.nom_m)
    let soft_stem = nom |> ends_with("ий")
    var inscope cps <- utf8_to_cpts(nom)
    let stem = cpts_to_utf8(cps, 0, length(cps) - 2)
    return "{stem}{soft_stem ? SOFT_ENDINGS[int(form)] : HARD_ENDINGS[int(form)]}"
}

def private ordinal_under_thousand(n : int) : string {
    let h = n / 100
    let rest = n % 100
    return ORD_HUNDREDS[h] if (rest == 0)
    var inscope parts : array<string>
    if (h > 0) {
        parts |> push(HUNDREDS[h])
    }
    if (rest < 10) {
        parts |> push(ORD_UNITS[rest])
    } elif (rest < 20) {
        parts |> push(ORD_TEENS[rest - 10])
    } elif (rest % 10 == 0) {
        parts |> push(ORD_TENS[rest / 10])
    } else {
        parts |> push(TENS[rest / 10])
        parts |> push(ORD_UNITS[rest % 10])
    }
    return join(parts, " ")
}

//! An ordinal inflected into `form`, the ending on its last word alone (две тысячи двадцать четвёртый); zero, a negative or a million on reads as a cardinal.
def ordinal_words_ru(n : int64; form : RuOrdinalForm = RuOrdinalForm.nom_m) : string {
    return number_to_words_ru(n) if (n <= 0l || n >= 1000000l)
    let thousands = int(n / 1000l)
    let rest = int(n % 1000l)
    var nom = ""
    if (rest == 0) {
        nom = thousands < 10 ? "{ORD_THOUSAND_PREFIX[thousands]}тысячный" : "{number_to_words_ru(int64(thousands), RuGender.feminine)} тысячный"
    } else {
        var inscope parts : array<string>
        if (thousands > 0) {
            parts |> push(number_to_words_ru(int64(thousands) * 1000l))
        }
        parts |> push(ordinal_under_thousand(rest))
        nom = join(parts, " ")
    }
    var inscope words <- split(nom, " ")
    words[length(words) - 1] = inflect_ordinal(words[length(words) - 1], form)
    return join(words, " ")
}

def digits_to_words_ru(s : string) : string => digits_as_words(s, UNITS_M)

//! a decimal `3,14` as три целых четырнадцать сотых; the integer part feminine (целая/целых)
def decimal_words_ru(int_part, frac : string) : string {
    let whole = to_int64(int_part, false)
    let f = to_int64(frac, false)
    let fdigits = length(frac)
    var inscope den_words <- ["", "десятых", "сотых", "тысячных", "десятитысячных", "стотысячных", "миллионных"]
    return number_to_words_ru(whole) if (fdigits == 0 || fdigits > 6)
    let whole_words = number_to_words_ru(whole, RuGender.feminine)
    let whole_unit = plural_ru(whole, "целая", "целых", "целых")
    let frac_words = number_to_words_ru(f, RuGender.feminine)
    var den = den_words[fdigits]
    if (f % 10l == 1l && f % 100l != 11l) {   // одна десятая, двадцать одна сотая
        var inscope dcps <- utf8_to_cpts(den)
        den = "{cpts_to_utf8(dcps, 0, length(dcps) - 2)}ая"
    }
    return "{whole_words} {whole_unit} {frac_words} {den}"
}

def private is_letter(cp : int) : bool => cpt_is_cyrillic(cp) || is_alpha(cp) || (cp >= 128 && cpt_is_letter(cp))
def private is_word(cp : int) : bool => is_digit(cp) || is_letter(cp)

//! the index past the Cyrillic run starting at `i`
def private letters_end(c : array<int>; i : int) : int {
    var j = i
    while (cpt_is_cyrillic(cp_at(c, j))) {
        j++
    }
    return j
}

//! whether a dot at `i` ends the sentence too: the text ends after it, or a capital follows
def private sentence_ends_after(c : array<int>; i : int) : bool {
    let s = spaces_end(c, i)
    return s >= length(c) || cpt_is_cyrillic_upper(c[s])
}

//! whether the Cyrillic run at [a, b) is `what`, which is lowercase; the run's own case is ignored
def private run_is(c : array<int>; a, b : int; what : string) : bool {
    var inscope w <- utf8_to_cpts(what)
    return false if (b - a != length(w))
    for (k in range(length(w))) {
        return false if (int(utf32_to_lower(uint(c[a + k]))) != w[k])
    }
    return true
}

def private is_year(v : int64) : bool => v >= 1000l && v <= 2200l

//! a count with its unit's three forms (один метр, два метра, пять метров)
struct private UnitForms {
    one : string
    few : string
    many : string
}

def private forms(one, few, many : string) : UnitForms => UnitForms(one = one, few = few, many = many)

//! the units and currencies a number is followed by, as written (with or without a trailing dot)
let private UNITS_RU <- {
    "кг" => forms("килограмм", "килограмма", "килограммов"), "км" => forms("километр", "километра", "километров"),
    "см" => forms("сантиметр", "сантиметра", "сантиметров"), "мм" => forms("миллиметр", "миллиметра", "миллиметров"),
    "мл" => forms("миллилитр", "миллилитра", "миллилитров"), "мин" => forms("минута", "минуты", "минут"),
    "сек" => forms("секунда", "секунды", "секунд"), "руб" => forms("рубль", "рубля", "рублей"),
    "коп" => forms("копейка", "копейки", "копеек"), "тыс" => forms("тысяча", "тысячи", "тысяч"),
    "млн" => forms("миллион", "миллиона", "миллионов"), "млрд" => forms("миллиард", "миллиарда", "миллиардов"),
    "шт" => forms("штука", "штуки", "штук"), "чел" => forms("человек", "человека", "человек"),
    "га" => forms("гектар", "гектара", "гектаров")
}
//! the one-letter units, read only straight after a number
let private ONE_LETTER_UNITS_RU <- {
    "м" => forms("метр", "метра", "метров"), "л" => forms("литр", "литра", "литров"), "ч" => forms("час", "часа", "часов"),
    "г" => forms("грамм", "грамма", "граммов"), "с" => forms("секунда", "секунды", "секунд"), "т" => forms("тонна", "тонны", "тонн")
}
let private FEMININE_UNITS <- { "мин", "сек", "коп", "тыс", "шт", "т", "с" }
//! the currency signs, read before the number ($15) and after it (15 $)
let private CURRENCY_SIGNS <- { '$' => forms("доллар", "доллара", "долларов"), 8364 => forms("евро", "евро", "евро"),
    8381 => forms("рубль", "рубля", "рублей"), 163 => forms("фунт", "фунта", "фунтов") }
let private PERCENT <- forms("процент", "процента", "процентов")
let private DEGREES <- forms("градус", "градуса", "градусов")
let private DEGREE_SIGN = 176
let private DEGREE_CELSIUS_SIGN = 8451
let private CYR_S_CAP = 1057
let private CELSIUS = " цельсия"

//! the abbreviations a reader expands, keyed without the trailing dot
let private ABBREVIATIONS_RU <- {
    "т.е" => "то есть", "т.д" => "так далее", "т.п" => "тому подобное", "т.к" => "так как", "т.н" => "так называемый",
    "т.ч" => "том числе", "др" => "другие", "пр" => "прочее", "см" => "смотри", "стр" => "страница", "гг" => "годов",
    "ул" => "улица", "просп" => "проспект", "пл" => "площадь", "обл" => "область", "р-н" => "район", "кв" => "квартира", "д" => "дом",
    "тел" => "телефон", "напр" => "например", "им" => "имени", "проф" => "профессор", "акад" => "академик",
    "англ" => "английский", "рус" => "русский", "лат" => "латинский", "греч" => "греческий",
    "ок" => "около", "прим" => "примечание", "ред" => "редакция", "изд" => "издание", "св" => "святой",
    "руб" => "рублей", "коп" => "копеек", "тыс" => "тысяч", "млн" => "миллионов", "млрд" => "миллиардов",
    "н.э" => "нашей эры", "до н.э" => "до нашей эры", "в т.ч" => "в том числе", "и т.д" => "и так далее", "и т.п" => "и тому подобное"
}

let private PROSODY_PUNCT <- { '.', ',', '?', '!', ';', ':', '-', 8212, 8211, 8230 }

def private count_with_unit(v : int64; u : UnitForms; gender : RuGender) : string =>
    "{number_to_words_ru(v, gender)} {plural_ru(v, u.one, u.few, u.many)}"

//! the gender a unit's count agrees in, by the unit as written
def private unit_gender(word : string) : RuGender => key_exists(FEMININE_UNITS, word) ? RuGender.feminine : RuGender.masculine

//! the suffix after `5-` and the form it spells
let private ORDINAL_SUFFIX_FORMS <- {
    "й" => RuOrdinalForm.nom_m, "ый" => RuOrdinalForm.nom_m, "ий" => RuOrdinalForm.nom_m, "ой" => RuOrdinalForm.nom_m,
    "го" => RuOrdinalForm.gen_m, "ого" => RuOrdinalForm.gen_m, "его" => RuOrdinalForm.gen_m,
    "му" => RuOrdinalForm.dat_m, "ому" => RuOrdinalForm.dat_m, "ему" => RuOrdinalForm.dat_m,
    "м" => RuOrdinalForm.prep_m, "ом" => RuOrdinalForm.prep_m, "ем" => RuOrdinalForm.prep_m,
    "ым" => RuOrdinalForm.instr_m, "им" => RuOrdinalForm.instr_m,
    "я" => RuOrdinalForm.nom_f, "ая" => RuOrdinalForm.nom_f, "яя" => RuOrdinalForm.nom_f,
    "ю" => RuOrdinalForm.acc_f, "ую" => RuOrdinalForm.acc_f, "юю" => RuOrdinalForm.acc_f,
    "е" => RuOrdinalForm.nom_n, "ое" => RuOrdinalForm.nom_n, "ее" => RuOrdinalForm.nom_n,
    "ые" => RuOrdinalForm.nom_pl, "ие" => RuOrdinalForm.nom_pl,
    "х" => RuOrdinalForm.gen_pl, "ых" => RuOrdinalForm.gen_pl, "их" => RuOrdinalForm.gen_pl,
    "ми" => RuOrdinalForm.instr_pl, "ыми" => RuOrdinalForm.instr_pl, "ими" => RuOrdinalForm.instr_pl
}

//! `dd.mm.yyyy` at `i` as `<day> <month> <year> года`; the index past it, or `i` where no such date starts here
def private date_dotted(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    return i if (e - i > 2 || cp_at(c, e) != '.' || digits_end(c, e + 1) != e + 3 || cp_at(c, e + 3) != '.' || digits_end(c, e + 4) != e + 8 || is_word(cp_at(c, e + 8)))
    let day = digits_value(c, i, e)
    let month = digits_value(c, e + 1, e + 3)
    let year = digits_value(c, e + 4, e + 8)
    return i if (day < 1l || day > 31l || month < 1l || month > 12l)
    emit(out, "{ordinal_words_ru(day, RuOrdinalForm.gen_m)} {MONTHS_GEN[int(month) - 1]} {ordinal_words_ru(year, RuOrdinalForm.gen_m)} года")
    return e + 8
}

//! the month whose genitive the run at [a, b) spells, 0-based; -1 for none
def private month_index(c : array<int>; a, b : int) : int {
    for (k in range(12)) {
        return k if (run_is(c, a, b, MONTHS_GEN[k]))
    }
    return -1
}

//! `dd <month> [yyyy [года | г.]]` at `i`; the index past it, or `i` where no such date starts here
def private date_day_month(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    return i if (e - i > 2)
    let s = spaces_end(c, e)
    let me = letters_end(c, s)
    let month = month_index(c, s, me)
    let day = digits_value(c, i, e)
    return i if (month < 0 || day < 1l || day > 31l)
    emit(out, "{ordinal_words_ru(day, RuOrdinalForm.gen_m)} {MONTHS_GEN[month]}")
    let s2 = spaces_end(c, me)
    let ye = digits_end(c, s2)
    return me if (ye - s2 != 4 || !is_year(digits_value(c, s2, ye)) || is_word(cp_at(c, ye)))
    emit(out, " {ordinal_words_ru(digits_value(c, s2, ye), RuOrdinalForm.gen_m)} года")
    let s3 = spaces_end(c, ye)
    let we = letters_end(c, s3)
    return we if (run_is(c, s3, we, "года"))
    return we + 1 if (run_is(c, s3, we, "г") && cp_at(c, we) == '.')
    return ye
}

//! `в 2024 году`, `2024 года`, `2024 г.`, `25.12.2024`, `25 декабря 2024 года`, `25 декабря`: the day a genitive ordinal, the year in the case its word asks for
def private pass_dates(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && !is_word(cp_at(c, i - 1))) {
            let e = digits_end(c, i)
            let dotted = date_dotted(c, i, out)
            if (dotted > i) {
                i = dotted
                continue
            }
            let named = date_day_month(c, i, out)
            if (named > i) {
                i = named
                continue
            }
            if (e - i == 4 && is_year(digits_value(c, i, e))) {
                let year = digits_value(c, i, e)
                var j = e
                if (cp_at(c, j) == '-' && cpt_is_cyrillic(cp_at(c, j + 1))) {
                    j = letters_end(c, j + 1)
                }
                let s = spaces_end(c, j)
                let we = letters_end(c, s)
                if (run_is(c, s, we, "году")) {
                    emit(out, "{ordinal_words_ru(year, RuOrdinalForm.prep_m)} году")
                    i = we
                    continue
                }
                if (run_is(c, s, we, "года") || (run_is(c, s, we, "г") && cp_at(c, we) == '.')) {
                    emit(out, "{ordinal_words_ru(year, RuOrdinalForm.gen_m)} года")
                    i = run_is(c, s, we, "г") ? we + 1 : we
                    continue
                }
                if (run_is(c, s, we, "гг") && cp_at(c, we) == '.') {
                    emit(out, "{ordinal_words_ru(year, RuOrdinalForm.gen_pl)} годов")
                    i = we + 1
                    continue
                }
            }
            out |> push(c[i])
            i++
            continue
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! hh:mm [:ss] as `четырнадцать тридцать`, a minute under ten with its ноль
def private pass_time(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && !is_word(cp_at(c, i - 1))) {
            let e = digits_end(c, i)
            if (e - i <= 2 && cp_at(c, e) == ':' && digits_end(c, e + 1) == e + 3 && !is_digit(cp_at(c, e + 3))) {
                let h = digits_value(c, i, e)
                let m = digits_value(c, e + 1, e + 3)
                if (h <= 23l && m <= 59l) {
                    let minutes = m < 10l ? "ноль {number_to_words_ru(m)}" : number_to_words_ru(m)
                    emit(out, "{number_to_words_ru(h)} {minutes}")
                    var j = e + 3
                    if (cp_at(c, j) == ':' && digits_end(c, j + 1) == j + 3 && digits_value(c, j + 1, j + 3) <= 59l && !is_word(cp_at(c, j + 3))) {
                        let s = digits_value(c, j + 1, j + 3)
                        emit(out, " {count_with_unit(s, ONE_LETTER_UNITS_RU?["с"] ?? UnitForms(), RuGender.feminine)}")
                        j += 3
                    }
                    i = j
                    continue
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! 5-й, 5-го, 5-му, 5-м, 5-я, 5-ой, 5-ю, 5-е, 5-ые, 5-х, 5-ми: the ordinal in the form its suffix spells
def private pass_ordinals(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && !is_word(cp_at(c, i - 1))) {
            let e = digits_end(c, i)
            if (cp_at(c, e) == '-' && cpt_is_cyrillic(cp_at(c, e + 1)) && e - i <= MAX_DIGITS_AS_NUMBER) {
                let se = letters_end(c, e + 1)
                let suffix = cpts_str(c, e + 1, se)
                if (key_exists(ORDINAL_SUFFIX_FORMS, suffix)) {
                    emit(out, ordinal_words_ru(digits_value(c, i, e), ORDINAL_SUFFIX_FORMS?[suffix] ?? RuOrdinalForm.nom_m))
                    i = se
                    continue
                }
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! a decimal at `i` and its unit, the unit in the genitive singular (`few`: три целых пять десятых процента); the index past it, or `i`
def private decimal_with_unit(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    return i if (e - i > MAX_DIGITS_AS_NUMBER || (cp_at(c, e) != ',' && cp_at(c, e) != '.') || !is_digit(cp_at(c, e + 1)))
    let fe = digits_end(c, e + 1)
    let s = spaces_end(c, fe)
    let nx = cp_at(c, s)
    var unit = UnitForms()
    var end = s
    if (nx == '%') {
        unit = PERCENT
        end = s + 1
    } elif (nx == DEGREE_SIGN || nx == DEGREE_CELSIUS_SIGN) {
        unit = DEGREES
        end = s + 1
    } elif (cpt_is_cyrillic(nx) && s > fe) {
        let ue = letters_end(c, s)
        unit = UNITS_RU?[cpts_str(c, s, ue)] ?? (ONE_LETTER_UNITS_RU?[cpts_str(c, s, ue)] ?? UnitForms())
        end = cp_at(c, ue) == '.' ? ue + 1 : ue
    }
    return i if (empty(unit.one) || is_word(cp_at(c, end)))
    emit(out, "{decimal_words_ru(cpts_str(c, i, e), cpts_str(c, e + 1, fe))} {unit.few}{nx == DEGREE_CELSIUS_SIGN ? CELSIUS : ""}")
    return end
}

//! the number starting at the digit `i` with the unit, percent sign, degree mark or currency sign
//! after it, as words; the index past it, or `i` where no unit follows
def private number_with_unit(c : array<int>; i : int; var out : array<int>) : int {
    let decimal = decimal_with_unit(c, i, out)
    return decimal if (decimal > i)
    let e = digits_end(c, i)
    return i if (e - i > MAX_DIGITS_AS_NUMBER)
    let v = digits_value(c, i, e)
    let s = spaces_end(c, e)
    let nx = cp_at(c, s)
    if (nx == '%' && !is_word(cp_at(c, s + 1))) {
        emit(out, count_with_unit(v, PERCENT, RuGender.masculine))
        return s + 1
    }
    if ((nx == DEGREE_SIGN || nx == DEGREE_CELSIUS_SIGN) && !is_word(cp_at(c, s + 1))) {
        emit(out, "{count_with_unit(v, DEGREES, RuGender.masculine)}{nx == DEGREE_CELSIUS_SIGN ? CELSIUS : ""}")
        return s + 1
    }
    if (nx == DEGREE_SIGN && (cp_at(c, s + 1) == 'C' || cp_at(c, s + 1) == CYR_S_CAP) && !is_word(cp_at(c, s + 2))) {
        emit(out, "{count_with_unit(v, DEGREES, RuGender.masculine)}{CELSIUS}")
        return s + 2
    }
    let currency_after = CURRENCY_SIGNS?[nx] ?? UnitForms()
    if (!empty(currency_after.one) && !is_word(cp_at(c, s + 1))) {
        emit(out, count_with_unit(v, currency_after, RuGender.masculine))
        return s + 1
    }
    return i if (!cpt_is_cyrillic(nx) || s <= e)
    let ue = letters_end(c, s)
    let word = cpts_str(c, s, ue)
    let dotted = cp_at(c, ue) == '.'
    let after = cp_at(c, dotted ? ue + 1 : ue)
    let unit = UNITS_RU?[word] ?? (ONE_LETTER_UNITS_RU?[word] ?? UnitForms())
    let short = key_exists(ONE_LETTER_UNITS_RU, word)
    // a one-letter unit needs its dot or a boundary; a year's г is a date (pass_dates ran first)
    return i if (empty(unit.one) || is_word(after) || (short && !dotted && is_letter(cp_at(c, ue))))
    emit(out, count_with_unit(v, unit, unit_gender(word)))
    let end = dotted ? ue + 1 : ue
    if (dotted && sentence_ends_after(c, end)) {
        out |> push('.')
    }
    return end
}

//! a number followed by a unit, a percent sign or a degree mark, or led by a currency sign; a
//! minus before the number opens the phrase with минус
def private pass_units(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let currency = CURRENCY_SIGNS?[c[i]] ?? UnitForms()
        if (!empty(currency.one) && is_digit(cp_at(c, i + 1))) {
            let e = digits_end(c, i + 1)
            if (!is_word(cp_at(c, e)) && cp_at(c, e) != ',' && cp_at(c, e) != '.' && e - i - 1 <= MAX_DIGITS_AS_NUMBER) {
                emit(out, count_with_unit(digits_value(c, i + 1, e), currency, RuGender.masculine))
                i = e
                continue
            }
        }
        let neg = c[i] == '-' && is_digit(cp_at(c, i + 1))
        let start = neg ? i + 1 : i
        if (is_digit(cp_at(c, start)) && !is_word(cp_at(c, i - 1))) {
            var inscope phrase : array<int>
            let end = number_with_unit(c, start, phrase)
            if (end > start) {
                if (neg) {
                    emit(out, "минус ")
                }
                out |> push_from(phrase)
                i = end
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! the abbreviations of ABBREVIATIONS_RU, longest match first, the trailing dot consumed
def private pass_abbreviations(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (cpt_is_cyrillic(c[i]) && !is_word(cp_at(c, i - 1))) {
            var best = -1
            var j = i
            var seen_letter = false
            while (j < n && (cpt_is_cyrillic(c[j]) || c[j] == '.' || c[j] == '-' || (c[j] == ' ' && seen_letter)) && j - i < 10) {
                seen_letter = seen_letter || cpt_is_cyrillic(c[j])
                if (key_exists(ABBREVIATIONS_RU, utf8_to_lower(cpts_str(c, i, j + 1))) && cp_at(c, j + 1) == '.' && !is_word(cp_at(c, j + 2))) {
                    best = j + 1
                }
                j++
            }
            if (best > i) {
                emit(out, ABBREVIATIONS_RU?[utf8_to_lower(cpts_str(c, i, best))] ?? "")
                i = best + 1
                if (sentence_ends_after(c, i)) {
                    out |> push('.')
                }
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! № as номер, + as плюс before a number, = as равно, & as и, @ as собака
def private pass_symbols(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    for (i in range(n)) {
        let cp = c[i]
        if (cp == 8470) {
            emit(out, " номер ")
        } elif (cp == '&') {
            emit(out, " и ")
        } elif (cp == '@') {
            emit(out, " собака ")
        } elif (cp == '=') {
            emit(out, " равно ")
        } elif (cp == '+' && is_digit(cp_at(c, i + 1)) && (i == 0 || !is_word(cp_at(c, i - 1)))) {
            emit(out, " плюс ")
        } else {
            out |> push(cp)
        }
    }
    return <- out
}

//! the gender the noun after a bare count gives away by its ending - after one the nominative (одна минута,
//! одно окно, одно время), after two the genitive singular (две минуты); masculine where the count ends in
//! another digit, where no Cyrillic word follows, or where the ending says nothing (день and часть alike)
def private noun_gender_after(c : array<int>; e : int; v : int64) : RuGender {
    let last = int(abs(v) % 10l)
    return RuGender.masculine if (last != 1 && last != 2)
    let s = spaces_end(c, e)
    return RuGender.masculine if (s == e || !cpt_is_cyrillic(cp_at(c, s)))
    let word = utf8_to_lower(cpts_str(c, s, letters_end(c, s)))
    if (last == 1) {
        return RuGender.neuter if (word |> ends_with("мя") || word |> ends_with("о") || word |> ends_with("е") || word |> ends_with("ё"))
        return (word |> ends_with("а") || word |> ends_with("я")) ? RuGender.feminine : RuGender.masculine
    }
    return (word |> ends_with("ы") || word |> ends_with("и")) ? RuGender.feminine : RuGender.masculine
}

//! integers and decimals: a space between three-digit groups reads through, a comma or a dot before digits is the decimal mark, a run past eighteen digits reads digit by digit; one and two agree with the noun after them
def private pass_numbers(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let neg = c[i] == '-' && is_digit(cp_at(c, i + 1)) && !is_word(cp_at(c, i - 1))
        let s = neg ? i + 1 : i
        if (is_digit(cp_at(c, s)) && !is_word(cp_at(c, i - 1))) {
            var e = digits_end(c, s)
            var inscope digits : array<int>
            digits |> reserve(e - s + 16)
            for (k in range(s, e)) {
                digits |> push(c[k])
            }
            while ((cp_at(c, e) == ' ' || cp_at(c, e) == 160) && digits_end(c, e + 1) == e + 4 && !is_digit(cp_at(c, e + 4))) {
                for (k in range(e + 1, e + 4)) {
                    digits |> push(c[k])
                }
                e += 4
            }
            var frac = ""
            if ((cp_at(c, e) == ',' || cp_at(c, e) == '.') && is_digit(cp_at(c, e + 1))) {
                let fe = digits_end(c, e + 1)
                frac = cpts_str(c, e + 1, fe)
                e = fe
            }
            let int_part = cpts_to_utf8(digits)
            if (length(digits) > MAX_DIGITS_AS_NUMBER) {
                emit(out, digits_to_words_ru(int_part))
            } elif (!empty(frac)) {
                emit(out, "{neg ? "минус " : ""}{decimal_words_ru(int_part, frac)}")
            } else {
                let v = to_int64(int_part, false) * (neg ? -1l : 1l)
                emit(out, number_to_words_ru(v, noun_gender_after(c, e, v)))
            }
            i = e
            continue
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

//! the word characters and the prosodic marks `. , ? ! ; : - — – …` survive, everything else becomes a space; whitespace collapses, the ends trim
def private pass_punctuation(c : array<int>) : array<int> {
    var out : array<int>
    var pending_space = false
    for (cp in c) {
        if (is_space(cp) || !(is_word(cp) || key_exists(PROSODY_PUNCT, cp))) {
            pending_space = true
            continue
        }
        if (pending_space && !empty(out)) {
            out |> push(' ')
        }
        pending_space = false
        out |> push(cp)
    }
    return <- out
}

//! Normalize Russian text for speech: what is read as words expanded, the prosodic punctuation kept, whitespace collapsed.
def normalize_text_ru(text : string) : string {
    var inscope c <- utf8_to_cpts(text)
    let passes <- [@@pass_abbreviations, @@pass_dates, @@pass_time, @@pass_ordinals, @@pass_units, @@pass_symbols,
        @@pass_numbers, @@pass_punctuation]
    for (step in passes) {
        var inscope next <- invoke(step, c)
        delete c
        c <- next
    }
    return cpts_str(c, 0, length(c))
}
