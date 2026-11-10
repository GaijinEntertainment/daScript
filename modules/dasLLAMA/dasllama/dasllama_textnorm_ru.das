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
require dasllama/dasllama_textnorm_ru_nouns

//! Russian text normalization for speech - the charter is `ARCHITECTURE_TTS_FRONT_END.md#tts-front-end`

//! The six cases a numeral is read in; every case table below has one row per case, in this order
enum RuCase {
    nom
    gen
    dat
    acc
    instr
    prep
}

let private CARD_UNITS_MASCULINE <- [
    ["ноль", "один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"],
    ["нуля", "одного", "двух", "трёх", "четырёх", "пяти", "шести", "семи", "восьми", "девяти"],
    ["нулю", "одному", "двум", "трём", "четырём", "пяти", "шести", "семи", "восьми", "девяти"],
    ["ноль", "один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"],
    ["нулём", "одним", "двумя", "тремя", "четырьмя", "пятью", "шестью", "семью", "восемью", "девятью"],
    ["нуле", "одном", "двух", "трёх", "четырёх", "пяти", "шести", "семи", "восьми", "девяти"]]
let private CARD_TEENS <- [
    ["десять", "одиннадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"],
    ["десяти", "одиннадцати", "двенадцати", "тринадцати", "четырнадцати", "пятнадцати", "шестнадцати", "семнадцати", "восемнадцати", "девятнадцати"],
    ["десяти", "одиннадцати", "двенадцати", "тринадцати", "четырнадцати", "пятнадцати", "шестнадцати", "семнадцати", "восемнадцати", "девятнадцати"],
    ["десять", "одиннадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"],
    ["десятью", "одиннадцатью", "двенадцатью", "тринадцатью", "четырнадцатью", "пятнадцатью", "шестнадцатью", "семнадцатью", "восемнадцатью", "девятнадцатью"],
    ["десяти", "одиннадцати", "двенадцати", "тринадцати", "четырнадцати", "пятнадцати", "шестнадцати", "семнадцати", "восемнадцати", "девятнадцати"]]
let private CARD_TENS <- [
    ["", "", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят", "восемьдесят", "девяносто"],
    ["", "", "двадцати", "тридцати", "сорока", "пятидесяти", "шестидесяти", "семидесяти", "восьмидесяти", "девяноста"],
    ["", "", "двадцати", "тридцати", "сорока", "пятидесяти", "шестидесяти", "семидесяти", "восьмидесяти", "девяноста"],
    ["", "", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят", "восемьдесят", "девяносто"],
    ["", "", "двадцатью", "тридцатью", "сорока", "пятьюдесятью", "шестьюдесятью", "семьюдесятью", "восемьюдесятью", "девяноста"],
    ["", "", "двадцати", "тридцати", "сорока", "пятидесяти", "шестидесяти", "семидесяти", "восьмидесяти", "девяноста"]]
let private CARD_HUNDREDS <- [
    ["", "сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот", "восемьсот", "девятьсот"],
    ["", "ста", "двухсот", "трёхсот", "четырёхсот", "пятисот", "шестисот", "семисот", "восьмисот", "девятисот"],
    ["", "ста", "двумстам", "трёмстам", "четырёмстам", "пятистам", "шестистам", "семистам", "восьмистам", "девятистам"],
    ["", "сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот", "восемьсот", "девятьсот"],
    ["", "ста", "двумястами", "тремястами", "четырьмястами", "пятьюстами", "шестьюстами", "семьюстами", "восемьюстами", "девятьюстами"],
    ["", "ста", "двухстах", "трёхстах", "четырёхстах", "пятистах", "шестистах", "семистах", "восьмистах", "девятистах"]]
let private ONE_FEMININE <- ["одна", "одной", "одной", "одну", "одной", "одной"]
let private ONE_NEUTER <- ["одно", "одного", "одному", "одно", "одним", "одном"]
let private TWO_FEMININE <- ["две", "двух", "двум", "две", "двумя", "двух"]
let private ORD_UNITS <- ["", "первый", "второй", "третий", "четвёртый", "пятый", "шестой", "седьмой", "восьмой", "девятый"]
let private ORD_TEENS <- ["десятый", "одиннадцатый", "двенадцатый", "тринадцатый", "четырнадцатый", "пятнадцатый", "шестнадцатый", "семнадцатый", "восемнадцатый", "девятнадцатый"]
let private ORD_TENS <- ["", "", "двадцатый", "тридцатый", "сороковой", "пятидесятый", "шестидесятый", "семидесятый", "восьмидесятый", "девяностый"]
let private ORD_HUNDREDS <- ["", "сотый", "двухсотый", "трёхсотый", "четырёхсотый", "пятисотый", "шестисотый", "семисотый", "восьмисотый", "девятисотый"]
let private ORD_THOUSAND_PREFIX <- ["", "", "двух", "трёх", "четырёх", "пяти", "шести", "семи", "восьми", "девяти"]
let private MONTHS_GEN <- ["января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"]

def private counts_as_one(n : int64) : bool => abs(n) % 10l == 1l && abs(n) % 100l != 11l
def private counts_as_few(n : int64) : bool => abs(n) % 10l >= 2l && abs(n) % 10l <= 4l && (abs(n) % 100l < 12l || abs(n) % 100l > 14l)

//! The grammatical number a count takes: 1 (not 11) is `one`, 2-4 (not 12-14) is `few`, the rest `many`
def plural_ru(n : int64; one, few, many : string) : string => counts_as_one(n) ? one : (counts_as_few(n) ? few : many)

def private ends_with_any(word : string; suffixes : array<string>) : bool {
    for (suffix in suffixes) {
        return true if (word |> ends_with(suffix))
    }
    return false
}

//! The gender one and two take: masculine, feminine (одна, две), neuter (одно)
enum RuGender {
    masculine
    feminine
    neuter
}

//! a counted noun: its nominative singular, genitive singular and genitive plural (один метр, два метра, пять метров), an adjective before it (квадратный), and words after it that never change (в час)
struct private UnitForms {
    one : string
    few : string
    many : string
    adj : string
    tail : string
    feminine : bool //! a feminine noun in -ь (четверть), which its ending does not tell from a masculine one
    locative : string //! a prepositional singular in -у (году, часу), which the ending rules do not give
}

def private forms(one, few, many : string) : UnitForms => UnitForms(one = one, few = few, many = many)
def private forms(adj, one, few, many, tail : string) : UnitForms => UnitForms(adj = adj, one = one, few = few, many = many, tail = tail)
def private is_indeclinable(u : UnitForms) : bool => u.one == u.few

//! The forms an adjective after a number takes - an ordinal, a counted part (целая, десятая), a unit's adjective (квадратный) - by its ending
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

//! one ending per `RuOrdinalForm`, in the enum's order: the hard stems (пятый, второй), the soft stem (третий), the velar stems before -ий (кубический)
let private HARD_ENDINGS <- ["", "ого", "ому", "ом", "ым", "ая", "ой", "ую", "ое", "ые", "ых", "ым", "ыми"]
let private SOFT_ENDINGS <- ["", "ьего", "ьему", "ьем", "ьим", "ья", "ьей", "ью", "ье", "ьи", "ьих", "ьим", "ьими"]
let private VELAR_ENDINGS <- ["", "ого", "ому", "ом", "им", "ая", "ой", "ую", "ое", "ие", "их", "им", "ими"]
let private VELAR_STEM_ENDINGS <- ["к", "г", "х"]

//! `nom` is a masculine nominative in -ый, -ой or -ий, its last word alone inflected; the stem keeps its ending family (пятый/пятого, третий/третьего, кубический/кубического)
def private inflect_adjective(nom : string; form : RuOrdinalForm) : string {
    return nom if (form == RuOrdinalForm.nom_m)
    let stem = chop_end(nom, 2)
    let f = int(form)
    let ending = !(nom |> ends_with("ий")) ? HARD_ENDINGS[f] : (ends_with_any(stem, VELAR_STEM_ENDINGS) ? VELAR_ENDINGS[f] : SOFT_ENDINGS[f])
    return "{stem}{ending}"
}

let private ORDINAL_FORM_M <- [RuOrdinalForm.nom_m, RuOrdinalForm.gen_m, RuOrdinalForm.dat_m, RuOrdinalForm.nom_m, RuOrdinalForm.instr_m, RuOrdinalForm.prep_m]
let private ORDINAL_FORM_F <- [RuOrdinalForm.nom_f, RuOrdinalForm.obl_f, RuOrdinalForm.obl_f, RuOrdinalForm.acc_f, RuOrdinalForm.obl_f, RuOrdinalForm.obl_f]
//! after a count not ending in one: the genitive plural in the nominative and accusative too (две целых, три четвёртых, пять квадратных метров)
let private PLURAL_COUNT_FORMS <- [RuOrdinalForm.gen_pl, RuOrdinalForm.gen_pl, RuOrdinalForm.dat_pl, RuOrdinalForm.gen_pl, RuOrdinalForm.instr_pl, RuOrdinalForm.gen_pl]

//! the singular form an adjective agreeing with a noun of `gender` in `cas` takes; an inanimate accusative is the nominative
def private singular_form(cas : RuCase; gender : RuGender) : RuOrdinalForm {
    return RuOrdinalForm.nom_n if (gender == RuGender.neuter && (cas == RuCase.nom || cas == RuCase.acc))
    return gender == RuGender.feminine ? ORDINAL_FORM_F[int(cas)] : ORDINAL_FORM_M[int(cas)]
}

//! the form an adjective or a counted part after the count `n` takes in `cas`: singular in `gender` after one, plural otherwise
[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-count-agreement")]
def private count_form(n : int64; cas : RuCase; gender : RuGender) : RuOrdinalForm {
    return PLURAL_COUNT_FORMS[int(cas)] if (!counts_as_one(n))
    return singular_form(cas, gender)
}

//! a feminine stem ending in one of these spells its instrumental -ей, not -ой (тысячей)
let private EI_STEM_ENDINGS <- ["ч", "ш", "щ", "ж", "ц"]

//! the dative, instrumental or prepositional singular of the noun `one` by its ending: a stem in -и takes -и (линии, поколении), a soft one -ю, -ем or -ём
def private singular_oblique(one, stem : string; cas : RuCase; soft, fem : bool) : string {
    let i_stem = stem |> ends_with("и")
    if (cas == RuCase.dat) {
        return "{stem}{fem ? (i_stem ? "и" : "е") : (soft ? "ю" : "у")}"
    }
    if (cas == RuCase.instr) {
        let hard_fem = ends_with_any(stem, EI_STEM_ENDINGS) ? "ей" : "ой"
        return "{stem}{fem ? (soft ? "ей" : hard_fem) : (soft ? (one |> ends_with("ь") ? "ём" : "ем") : "ом")}"
    }
    return "{stem}{i_stem ? "и" : "е"}"
}

//! the dative, accusative, instrumental or prepositional only - the nominative and genitive are the stored forms; a noun that is an adjective (дополнительный) declines as one
[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-case-government")]
def private noun_case(u : UnitForms; cas : RuCase; plural : bool) : string {
    return u.one if (is_indeclinable(u))
    return inflect_adjective(u.one, count_form(plural ? 2l : 1l, cas, RuGender.masculine)) if (ends_with_any(u.one, ["ый", "ий", "ой"]))
    return u.locative if (cas == RuCase.prep && !plural && !empty(u.locative))
    let soft = ends_with_any(u.one, ["ь", "й", "я", "е"])
    let fem = ends_with_any(u.one, ["а", "я"])
    let stem = (soft || fem || u.one |> ends_with("о")) ? chop_end(u.one, 1) : u.one
    if (plural) {
        let a = soft ? "я" : "а"
        return cas == RuCase.dat ? "{stem}{a}м" : (cas == RuCase.instr ? "{stem}{a}ми" : "{stem}{a}х")
    }
    if (cas == RuCase.acc) {
        return fem ? "{stem}{soft ? "ю" : "у"}" : u.one
    }
    if (u.feminine) {
        return cas == RuCase.instr ? "{u.one}ю" : "{stem}и"
    }
    return singular_oblique(u.one, stem, cas, soft, fem)
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-count-agreement")]
def private noun_for_count(u : UnitForms; n : int64; cas : RuCase) : string {
    let ones = counts_as_one(n)
    return plural_ru(n, u.one, u.few, u.many) if (cas == RuCase.nom || (cas == RuCase.acc && !ones))
    return (ones ? u.few : u.many) if (cas == RuCase.gen)
    return noun_case(u, cas, !ones)
}

def private unit_words(u : UnitForms; n : int64; cas : RuCase) : string {
    let adj = empty(u.adj) ? "" : "{inflect_adjective(u.adj, count_form(n, cas, RuGender.masculine))} "
    return "{adj}{noun_for_count(u, n, cas)}{u.tail}"
}

let private SCALES <- [forms("квинтиллион", "квинтиллиона", "квинтиллионов"), forms("квадриллион", "квадриллиона", "квадриллионов"),
    forms("триллион", "триллиона", "триллионов"), forms("миллиард", "миллиарда", "миллиардов"), forms("миллион", "миллиона", "миллионов"),
    forms("тысяча", "тысячи", "тысяч")]

def private unit_word(d : int; gender : RuGender; cas : RuCase) : string {
    return ONE_FEMININE[int(cas)] if (d == 1 && gender == RuGender.feminine)
    return ONE_NEUTER[int(cas)] if (d == 1 && gender == RuGender.neuter)
    return TWO_FEMININE[int(cas)] if (d == 2 && gender == RuGender.feminine)
    return CARD_UNITS_MASCULINE[int(cas)][d]
}

def private under_thousand(n : int; gender : RuGender; cas : RuCase; var parts : array<string>) {
    let row = int(cas)
    let h = n / 100
    let t = (n % 100) / 10
    let u = n % 10
    if (h > 0) {
        parts |> push(CARD_HUNDREDS[row][h])
    }
    if (t == 1) {
        parts |> push(CARD_TEENS[row][u])
    } else {
        if (t >= 2) {
            parts |> push(CARD_TENS[row][t])
        }
        if (u > 0) {
            parts |> push(unit_word(u, gender, cas))
        }
    }
}

//! A cardinal in `cas`, `gender` picking один/одна/одно and два/две; a thousand at the head reads тысяча alone, the way it is said; a negative opens with минус.
def cardinal_words_ru(n : int64; cas : RuCase; gender : RuGender = RuGender.masculine) : string {
    return CARD_UNITS_MASCULINE[int(cas)][0] if (n == 0l)
    var inscope parts : array<string>
    if (n < 0l) {
        parts |> push("минус")
    }
    var v = abs(n)
    var scale = 1000000000000000000l
    for (s in SCALES) {
        let k = v / scale
        if (k > 0l) {
            let thousand = scale == 1000l
            if (!(thousand && abs(n) < 2000l)) {
                under_thousand(int(k), thousand ? RuGender.feminine : RuGender.masculine, cas, parts)
            }
            parts |> push(noun_for_count(s, k, cas))
            v -= k * scale
        }
        scale /= 1000l
    }
    under_thousand(int(v), gender, cas, parts)
    return join(parts, " ")
}

//! A cardinal in the nominative, `gender` picking один/одна/одно and два/две; a negative opens with минус.
def number_to_words_ru(n : int64; gender : RuGender = RuGender.masculine) : string => cardinal_words_ru(n, RuCase.nom, gender)

def private ordinal_under_thousand(n : int) : string {
    let h = n / 100
    let rest = n % 100
    return ORD_HUNDREDS[h] if (rest == 0)
    var inscope parts : array<string>
    if (h > 0) {
        parts |> push(CARD_HUNDREDS[int(RuCase.nom)][h])
    }
    if (rest < 10) {
        parts |> push(ORD_UNITS[rest])
    } elif (rest < 20) {
        parts |> push(ORD_TEENS[rest - 10])
    } elif (rest % 10 == 0) {
        parts |> push(ORD_TENS[rest / 10])
    } else {
        parts |> push(CARD_TENS[int(RuCase.nom)][rest / 10])
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
    return inflect_adjective(nom, form)
}

def digits_to_words_ru(s : string) : string => digits_as_words(s, CARD_UNITS_MASCULINE[int(RuCase.nom)])

//! the denominator of a decimal by its digit count
let private DECIMAL_DENOMINATORS <- ["", "десятый", "сотый", "тысячный", "десятитысячный", "стотысячный", "миллионный"]

//! `count` of a feminine part named by its masculine nominative `part` (целый, десятый, второй) in `cas`: одна целая, две целых, трёх четвёртых
[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-count-agreement")]
def private count_of_part(count : int64; part : string; cas : RuCase) : string =>
    "{cardinal_words_ru(count, cas, RuGender.feminine)} {inflect_adjective(part, count_form(count, cas, RuGender.feminine))}"

//! a decimal `3,14` as три целых четырнадцать сотых, in `cas`; the integer part feminine (целая/целых)
def decimal_words_ru(int_part, frac : string; cas : RuCase = RuCase.nom) : string {
    let whole = to_int64(int_part, false)
    let fdigits = length(frac)
    return cardinal_words_ru(whole, cas) if (fdigits == 0)
    return "{cardinal_words_ru(whole, cas)} запятая {digits_to_words_ru(frac)}" if (fdigits > 6)
    return "{count_of_part(whole, "целый", cas)} {count_of_part(to_int64(frac, false), DECIMAL_DENOMINATORS[fdigits], cas)}"
}

//! a fraction `num/den` as одна вторая, три четвёртых, in `cas`; a whole part before it reads with целых (две целых одна вторая)
def fraction_words_ru(whole, num, den : int64; cas : RuCase = RuCase.nom) : string {
    let part = count_of_part(num, ordinal_words_ru(den), cas)
    return whole > 0l ? "{count_of_part(whole, "целый", cas)} {part}" : part
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

//! the Cyrillic run at `s`, lowercase
def private word_at(c : array<int>; s : int) : string => utf8_to_lower(cpts_str(c, s, letters_end(c, s)))

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

let private KOPECKS <- forms("копейка", "копейки", "копеек")
let private CENTS <- forms("цент", "цента", "центов")
let private PAGES <- forms("страница", "страницы", "страниц")
let private HOURS <- forms("час", "часа", "часов")

//! the units and currencies a number is followed by, as written (with or without a trailing dot)
let private UNITS_RU <- {
    "кг" => forms("килограмм", "килограмма", "килограммов"), "км" => forms("километр", "километра", "километров"),
    "см" => forms("сантиметр", "сантиметра", "сантиметров"), "мм" => forms("миллиметр", "миллиметра", "миллиметров"),
    "дм" => forms("дециметр", "дециметра", "дециметров"), "мкм" => forms("микрометр", "микрометра", "микрометров"),
    "нм" => forms("нанометр", "нанометра", "нанометров"), "мг" => forms("миллиграмм", "миллиграмма", "миллиграммов"),
    "мл" => forms("миллилитр", "миллилитра", "миллилитров"), "мин" => forms("минута", "минуты", "минут"),
    "сек" => forms("секунда", "секунды", "секунд"), "руб" => forms("рубль", "рубля", "рублей"),
    "коп" => KOPECKS, "тыс" => forms("тысяча", "тысячи", "тысяч"),
    "млн" => forms("миллион", "миллиона", "миллионов"), "млрд" => forms("миллиард", "миллиарда", "миллиардов"),
    "шт" => forms("штука", "штуки", "штук"), "чел" => forms("человек", "человека", "человек"),
    "га" => forms("гектар", "гектара", "гектаров"), "долл" => forms("доллар", "доллара", "долларов"),
    "грн" => forms("гривна", "гривны", "гривен"), "Вт" => forms("ватт", "ватта", "ватт"), "кВт" => forms("киловатт", "киловатта", "киловатт"),
    "МВт" => forms("мегаватт", "мегаватта", "мегаватт"), "ГВт" => forms("гигаватт", "гигаватта", "гигаватт"),
    "экз" => forms("экземпляр", "экземпляра", "экземпляров"), "доп" => forms("дополнительный", "дополнительных", "дополнительных"),
    "стр" => PAGES, "час" => HOURS
}
//! the one-letter units, read after a number and a space, never glued to it
let private ONE_LETTER_UNITS_RU <- {
    "м" => forms("метр", "метра", "метров"), "л" => forms("литр", "литра", "литров"), "ч" => forms("час", "часа", "часов"),
    "г" => forms("грамм", "грамма", "граммов"), "с" => forms("секунда", "секунды", "секунд"), "т" => forms("тонна", "тонны", "тонн")
}
let private CURRENCY_CODES <- { "USD" => forms("", "доллар", "доллара", "долларов", " США"), "EUR" => forms("евро", "евро", "евро"),
    "RUB" => forms("рубль", "рубля", "рублей") }
//! the currency signs, read before the number ($15) and after it (15 $)
let private CURRENCY_SIGNS <- { '$' => forms("доллар", "доллара", "долларов"), 8364 => forms("евро", "евро", "евро"),
    8381 => forms("рубль", "рубля", "рублей"), 163 => forms("фунт", "фунта", "фунтов") }
//! a currency's minor unit, keyed by the currency's nominative: one row a currency
let private MINOR_UNITS <- { "рубль" => KOPECKS, "гривна" => KOPECKS, "доллар" => CENTS, "евро" => CENTS,
    "фунт" => forms("пенни", "пенни", "пенни") }
let private PERCENT <- forms("процент", "процента", "процентов")
let private DEGREES <- forms("градус", "градуса", "градусов")
let private VOLUMES <- forms("том", "тома", "томов")
let private QUARTERS <- UnitForms(one = "четверть", few = "четверти", many = "четвертей", feminine = true)
let private THIRDS <- UnitForms(one = "треть", few = "трети", many = "третей", feminine = true)
//! пол takes a hyphen before л or a vowel (пол-литра, пол-евро)
let private POL_HYPHEN_INITIALS <- utf8_to_cpts("лаеёиоуэюя")
let private DEGREE_SIGN = 176
let private DEGREE_CELSIUS_SIGN = 8451
let private SUPERSCRIPT_TWO = 178
let private SUPERSCRIPT_THREE = 179
let private CYR_S_CAP = 1057
let private CELSIUS = " цельсия"
let private PER_TIME <- { "ч" => " в час", "час" => " в час", "с" => " в секунду", "сек" => " в секунду", "мин" => " в минуту" }

//! the abbreviations a reader expands, keyed without the trailing dot
let private ABBREVIATIONS_RU <- {
    "т.е" => "то есть", "т.д" => "так далее", "т.п" => "тому подобное", "т.к" => "так как", "т.н" => "так называемый",
    "т.ч" => "том числе", "др" => "другие", "пр" => "прочее", "см" => "смотри", "стр" => "страница", "гг" => "годов",
    "ул" => "улица", "просп" => "проспект", "пл" => "площадь", "обл" => "область", "р-н" => "район", "кв" => "квартира", "д" => "дом",
    "тел" => "телефон", "напр" => "например", "им" => "имени", "проф" => "профессор", "акад" => "академик",
    "англ" => "английский", "рус" => "русский", "лат" => "латинский", "греч" => "греческий",
    "ок" => "около", "прим" => "примечание", "ред" => "редакция", "изд" => "издание", "св" => "святой",
    "руб" => "рублей", "коп" => "копеек", "тыс" => "тысяч", "млн" => "миллионов", "млрд" => "миллиардов", "долл" => "долларов",
    "н.э" => "нашей эры", "до н.э" => "до нашей эры", "в т.ч" => "в том числе", "и т.д" => "и так далее", "и т.п" => "и тому подобное"
}

let private PROSODY_PUNCT <- { '.', ',', '?', '!', ';', ':', '-', EM_DASH, EN_DASH, 8230 }

def private unit_gender(u : UnitForms) : RuGender {
    return RuGender.masculine if (is_indeclinable(u))
    return RuGender.feminine if (u.feminine || ends_with_any(u.one, ["а", "я"]))
    return ends_with_any(u.one, ["о", "е"]) ? RuGender.neuter : RuGender.masculine
}

def private count_with_unit(v : int64; u : UnitForms; cas : RuCase) : string =>
    "{cardinal_words_ru(v, cas, unit_gender(u))} {unit_words(u, v, cas)}"

//! the case before any override by the noun after the numeral
let private CASE_GOVERNED_BY <- {
    "от" => RuCase.gen, "до" => RuCase.gen, "с" => RuCase.gen, "со" => RuCase.gen, "около" => RuCase.gen, "более" => RuCase.gen,
    "менее" => RuCase.gen, "свыше" => RuCase.gen, "больше" => RuCase.gen, "меньше" => RuCase.gen, "из" => RuCase.gen,
    "без" => RuCase.gen, "для" => RuCase.gen, "у" => RuCase.gen, "после" => RuCase.gen, "кроме" => RuCase.gen,
    "среди" => RuCase.gen, "вокруг" => RuCase.gen, "порядка" => RuCase.gen, "течение" => RuCase.gen, "протяжении" => RuCase.gen,
    "возрасте" => RuCase.gen, "размере" => RuCase.gen, "количестве" => RuCase.gen, "пределах" => RuCase.gen,
    "старше" => RuCase.gen, "младше" => RuCase.gen, "выше" => RuCase.gen, "ниже" => RuCase.gen, "против" => RuCase.gen,
    "к" => RuCase.dat, "ко" => RuCase.dat, "согласно" => RuCase.dat, "благодаря" => RuCase.dat, "вопреки" => RuCase.dat,
    "равный" => RuCase.dat, "равная" => RuCase.dat, "равное" => RuCase.dat, "равные" => RuCase.dat, "равен" => RuCase.dat,
    "равна" => RuCase.dat, "равны" => RuCase.dat,
    "между" => RuCase.instr, "перед" => RuCase.instr, "над" => RuCase.instr,
    "о" => RuCase.prep, "об" => RuCase.prep, "при" => RuCase.prep,
    "в" => RuCase.acc, "во" => RuCase.acc, "на" => RuCase.acc, "за" => RuCase.acc, "через" => RuCase.acc, "про" => RuCase.acc,
    "спустя" => RuCase.acc, "составляет" => RuCase.acc, "составляют" => RuCase.acc, "составлял" => RuCase.acc,
    "составляла" => RuCase.acc, "составляло" => RuCase.acc, "составляли" => RuCase.acc, "составил" => RuCase.acc,
    "составила" => RuCase.acc, "составило" => RuCase.acc, "составили" => RuCase.acc, "насчитывает" => RuCase.acc,
    "насчитывают" => RuCase.acc, "насчитывал" => RuCase.acc, "насчитывала" => RuCase.acc, "насчитывало" => RuCase.acc,
    "насчитывали" => RuCase.acc, "имеет" => RuCase.acc, "имеют" => RuCase.acc, "получил" => RuCase.acc,
    "получила" => RuCase.acc, "получили" => RuCase.acc
}

//! the Cyrillic word ending at the spaces before `i`, as a span; empty unless spaces alone stand between them (С. 54 has no preposition)
def private word_span_before(c : array<int>; i : int) : int2 {
    let j = spaces_start(c, i)
    var a = j
    while (a > 0 && cpt_is_cyrillic(c[a - 1])) {
        a--
    }
    return (j == i || is_word(cp_at(c, a - 1))) ? int2(j, j) : int2(a, j)
}

def private word_before(c : array<int>; i : int) : string {
    let span = word_span_before(c, i)
    return utf8_to_lower(cpts_str(c, span.x, span.y))
}

def private word_after(c : array<int>; e : int) : string {
    let s = spaces_end(c, e)
    return s == e ? "" : word_at(c, s)
}

def private is_group_space(cp : int) : bool => cp == ' ' || cp == 160

//! the end of the number starting at the digit `i`, a space or a no-break space between three-digit groups read through (29 784)
def private grouped_end(c : array<int>; i : int) : int {
    var e = digits_end(c, i)
    return e if (e - i > 3)
    while (is_group_space(cp_at(c, e)) && digits_end(c, e + 1) == e + 4 && !is_digit(cp_at(c, e + 4))) {
        e += 4
    }
    return e
}

//! whether the number at `i` continues a word or a number before it across a comma, a dot, a slash or a hyphen (1,5-литровый, 1/2, COVID-19)
def private continues_number(c : array<int>; i : int) : bool {
    let p = cp_at(c, i - 1)
    return (p == ',' || p == '.' || p == '/' || p == '-') && is_word(cp_at(c, i - 2))
}

//! the start of the number a dash ties the number at `i` to as a range's first half (от 38 - 40, от 1 000 — 2 000), or -1
def private range_first_half_start(c : array<int>; i : int) : int {
    let d = spaces_start(c, i)
    return -1 if (d == 0 || !is_dash(c[d - 1]))
    let j = spaces_start(c, d - 1)
    return -1 if (j == 0 || !is_digit(c[j - 1]))
    var a = j
    while (a > 0 && (is_digit(c[a - 1]) || ((c[a - 1] == ',' || c[a - 1] == '.' || (is_group_space(c[a - 1]) && digits_end(c, a) == a + 3)) && is_digit(cp_at(c, a - 2))))) {
        a--
    }
    return a
}

//! the end of the range's second half when a dash after `e` ties a number to the one ending there, else `e`
def private range_second_half_end(c : array<int>; e : int) : int {
    let s = spaces_end(c, e)
    return e if (!is_dash(cp_at(c, s)))
    let b = spaces_end(c, s + 1)
    return e if (!is_digit(cp_at(c, b)))
    var f = grouped_end(c, b)
    if ((cp_at(c, f) == ',' || cp_at(c, f) == '.') && is_digit(cp_at(c, f + 1))) {
        f = digits_end(c, f + 1)
    }
    return f
}

//! the start of the number of one to three digits a list goes on with after the number ending at `e` - across и, или or a comma (1 и 2, 3, 4 или 5) - or -1
def private list_next_start(c : array<int>; e : int) : int {
    let s = spaces_end(c, e)
    let we = letters_end(c, s)
    let comma = cp_at(c, s) == ',' && is_space(cp_at(c, s + 1))
    let joined = comma ? s + 1 : ((run_is(c, s, we, "и") || run_is(c, s, we, "или")) && we > s ? we : -1)
    let n = joined < 0 ? -1 : spaces_end(c, joined)
    return (n > 0 && is_digit(cp_at(c, n)) && c[n] != '0' && digits_end(c, n) - n <= 3) ? n : -1
}

//! past the last number of the range or the list the number ending at `e` opens (5-10, 2 или 3, 1, 2 и 3), where the noun they count stands
def private counted_span_end(c : array<int>; e : int) : int {
    var tail = range_second_half_end(c, e)
    var next = list_next_start(c, tail)
    while (next >= 0) {
        tail = digits_end(c, next)
        next = list_next_start(c, tail)
    }
    return tail
}

//! the start of the number и or или joins to the number at `i` (около 10 и 20), or -1
def private joined_number_start(c : array<int>; i : int) : int {
    let span = word_span_before(c, i)
    let joint = utf8_to_lower(cpts_str(c, span.x, span.y))
    let j = spaces_start(c, span.x)
    return -1 if ((joint != "и" && joint != "или") || j == span.x || !is_digit(cp_at(c, j - 1)))
    var a = j - 1
    while (a > 0 && is_digit(c[a - 1])) {
        a--
    }
    return a
}

//! read before a currency sign or a sign leading the number (свыше $100 млн, около -5), before the first half of a range it closes and before a number и or или joins it to
def private governing_word(c : array<int>; i : int) : string {
    let head = range_first_half_start(c, i)
    var start = head >= 0 ? head : i
    if (key_exists(CURRENCY_SIGNS, cp_at(c, start - 1))) {
        start--
    }
    if ((cp_at(c, start - 1) == '-' || cp_at(c, start - 1) == '+') && !is_word(cp_at(c, start - 2))) {
        start--
    }
    let joined = joined_number_start(c, start)
    return joined >= 0 ? governing_word(c, joined) : word_before(c, start)
}

//! the nouns a bare number numbers, by their nominative, genitive singular and genitive plural; after в, во or на their prepositional singular makes the number their ordinal even after a count ending in one (на восемьдесят первом году)
let private NUMBERED_NOUNS <- [
    UnitForms(one = "год", few = "года", many = "лет", locative = "году"), UnitForms(one = "час", few = "часа", many = "часов", locative = "часу"),
    UnitForms(one = "ряд", few = "ряда", many = "рядов", locative = "ряду"), forms("этаж", "этажа", "этажей"), forms("курс", "курса", "курсов"),
    forms("класс", "класса", "классов"), forms("тур", "тура", "туров"), forms("раунд", "раунда", "раундов"), forms("тайм", "тайма", "таймов"),
    forms("период", "периода", "периодов"), forms("сезон", "сезона", "сезонов"), forms("том", "тома", "томов"),
    forms("квартал", "квартала", "кварталов"), forms("канал", "канала", "каналов"), forms("километр", "километра", "километров"),
    forms("съезд", "съезда", "съездов"), forms("месяц", "месяца", "месяцев"), forms("матч", "матча", "матчей"),
    forms("этап", "этапа", "этапов"), forms("выпуск", "выпуска", "выпусков"), forms("раздел", "раздела", "разделов"),
    forms("пункт", "пункта", "пунктов"), forms("круг", "круга", "кругов"), forms("гейм", "гейма", "геймов"),
    forms("сет", "сета", "сетов"), forms("эпизод", "эпизода", "эпизодов"), forms("номер", "номера", "номеров"),
    forms("созыв", "созыва", "созывов"), forms("место", "места", "мест"), forms("издание", "издания", "изданий"),
    forms("заседание", "заседания", "заседаний"), forms("чтение", "чтения", "чтений"), forms("минута", "минуты", "минут"),
    forms("секунда", "секунды", "секунд"), forms("глава", "главы", "глав"), forms("серия", "серии", "серий"),
    forms("страница", "страницы", "страниц"), UnitForms(one = "часть", few = "части", many = "частей", feminine = true),
    forms("строка", "строки", "строк"), forms("строчка", "строчки", "строчек"), forms("неделя", "недели", "недель"),
    forms("позиция", "позиции", "позиций"), UnitForms(one = "ступень", few = "ступени", many = "ступеней", feminine = true),
    forms("стадия", "стадии", "стадий"), forms("группа", "группы", "групп"), forms("лига", "лиги", "лиг"),
    forms("статья", "статьи", "статей"), forms("книга", "книги", "книг"), forms("линия", "линии", "линий"),
    forms("школа", "школы", "школ"), forms("квартира", "квартиры", "квартир"), forms("палата", "палаты", "палат"),
    forms("версия", "версии", "версий"), forms("игра", "игры", "игр"), forms("партия", "партии", "партий"),
    forms("половина", "половины", "половин")
]
//! the nouns a bare number counts or numbers, read as its ordinal only in a singular form the count cannot govern (уже третий день, вторая рука)
let private COUNTED_NOUNS <- [
    forms("день", "дня", "дней"), forms("раз", "раза", "раз"), forms("план", "плана", "планов"),
    forms("вариант", "варианта", "вариантов"), forms("отряд", "отряда", "отрядов"), forms("пост", "поста", "постов"),
    forms("уровень", "уровня", "уровней"), forms("взгляд", "взгляда", "взглядов"), forms("случай", "случая", "случаев"),
    forms("вопрос", "вопроса", "вопросов"), forms("палец", "пальца", "пальцев"), forms("урок", "урока", "уроков"),
    forms("размер", "размера", "размеров"), forms("ранг", "ранга", "рангов"), forms("срок", "срока", "сроков"),
    forms("тип", "типа", "типов"), forms("сорт", "сорта", "сортов"), forms("акт", "акта", "актов"), forms("шаг", "шага", "шагов"),
    forms("батальон", "батальона", "батальонов"), forms("полк", "полка", "полков"), forms("вагон", "вагона", "вагонов"),
    forms("подъезд", "подъезда", "подъездов"), forms("кабинет", "кабинета", "кабинетов"), forms("состав", "состава", "составов"),
    forms("порядок", "порядка", "порядков"), UnitForms(one = "степень", few = "степени", many = "степеней", feminine = true),
    UnitForms(one = "очередь", few = "очереди", many = "очередей", feminine = true), QUARTERS,
    forms("рука", "руки", "рук"), forms("попытка", "попытки", "попыток"), forms("точка", "точки", "точек"),
    forms("половинка", "половинки", "половинок"), forms("сторона", "стороны", "сторон"), forms("смена", "смены", "смен"),
    forms("годовщина", "годовщины", "годовщин"), forms("дивизия", "дивизии", "дивизий"), forms("рота", "роты", "рот"),
    forms("бригада", "бригады", "бригад"), forms("армия", "армии", "армий"), forms("категория", "категории", "категорий"),
    forms("волна", "волны", "волн"), forms("лицо", "лица", "лиц"), forms("поколение", "поколения", "поколений"),
    forms("пришествие", "пришествия", "пришествий")
]
let private ALL_CASES <- [RuCase.nom, RuCase.gen, RuCase.dat, RuCase.acc, RuCase.instr, RuCase.prep]
//! the bit a genitive plural takes beside the six `RuCase` bits of a form's cases
let private GENITIVE_PLURAL = 6

//! each singular form of `u` with its `RuCase` as an int, then its genitive plural with `GENITIVE_PLURAL`
def private each_noun_form(u : UnitForms; blk : block<(form : string; bit : int) : void>) {
    for (cas in ALL_CASES) {
        invoke(blk, noun_for_count(u, 1l, cas), int(cas))
    }
    invoke(blk, u.many, GENITIVE_PLURAL)
}

def private add_noun_forms(var index : table<string; int2>; nouns : array<UnitForms>; list : int) {
    for (k, u in count(), nouns) {
        each_noun_form(u) $(form, _) {
            index |> insert(form, int2(list, k))
        }
    }
}

//! every singular form and the genitive plural of a numbered or counted noun, keyed to (0 for `NUMBERED_NOUNS` or 1 for `COUNTED_NOUNS`, the noun's index); a form two nouns share names the later
def private noun_forms_index() : table<string; int2> {
    var index : table<string; int2>
    add_noun_forms(index, COUNTED_NOUNS, 1)
    add_noun_forms(index, NUMBERED_NOUNS, 0)
    return <- index
}
let private NOUN_FORMS <- noun_forms_index()

//! the table noun `word` is a form of; empty forms where it is none
def private noun_of_form(word : string) : UnitForms {
    let k = NOUN_FORMS?[word] ?? int2(-1, -1)
    return UnitForms() if (k.x < 0)
    return k.x == 0 ? NUMBERED_NOUNS[k.y] : COUNTED_NOUNS[k.y]
}

let private VOWEL_ENDINGS <- ["а", "я", "о", "е", "ё", "и", "ы", "у", "ю", "э"]

//! the genitive singular of a feminine or neuter nominative by its ending (работа/работы, рука/руки, ночь/ночи, окно/окна, сердце/сердца); any other nominative is its own (кофе)
def private noun_genitive_singular(one : string) : string {
    let stem = chop_end(one, 1)
    if (one |> ends_with("а")) {
        let velar_or_hushing = ends_with_any(stem, VELAR_STEM_ENDINGS) || (ends_with_any(stem, EI_STEM_ENDINGS) && !(stem |> ends_with("ц")))
        return "{stem}{velar_or_hushing ? "и" : "ы"}"
    }
    return "{stem}и" if (ends_with_any(one, ["я", "ь"]))
    return "{stem}а" if (one |> ends_with("о"))
    return "{stem}{ends_with_any(stem, EI_STEM_ENDINGS) ? "а" : "я"}" if (one |> ends_with("е"))
    return one
}

//! the genitive plural of a feminine or neuter nominative by its ending (работ, линий, ночей, недель, идей); any other nominative is its own
def private noun_genitive_plural(one : string) : string {
    let stem = chop_end(one, 1)
    return "{stem}й" if (ends_with_any(one, ["ия", "ие"]))
    return "{stem}ей" if (one |> ends_with("ь"))
    return stem if (ends_with_any(one, ["а", "о"]))
    return "{stem}{ends_with_any(stem, VOWEL_ENDINGS) ? "й" : "ь"}" if (one |> ends_with("я"))
    return one
}

//! a dictionary noun's form: the noun's gender and the forms it spells, a bit a `RuCase` in the singular and `GENITIVE_PLURAL`
struct private NounForm {
    gender : RuGender
    cases : int
}

//! a string of `dasllama_textnorm_ru_nouns` as a noun's forms: the nominative, then the genitive singular and plural where its ending does not give them
def private dictionary_noun(entry : string; gender : RuGender) : UnitForms {
    var inscope parts <- split(entry, " ")
    let one = parts[0]
    let many = length(parts) > 1 ? parts[length(parts) - 1] : noun_genitive_plural(one)
    let few = length(parts) == 3 ? parts[1] : noun_genitive_singular(one)
    return UnitForms(one = one, few = few, many = many == "-" ? "" : many, feminine = gender == RuGender.feminine && one |> ends_with("ь"))
}

def private add_dictionary_nouns(var index : table<string; NounForm>; nouns : array<string>; gender : RuGender) {
    for (entry in nouns) {
        each_noun_form(dictionary_noun(entry, gender)) $(form, bit) {
            let key = replace(form, "ё", "е")
            var known = index?[key] ?? NounForm(gender = gender)
            if (!empty(key) && known.gender == gender) {
                known.cases |= 1 << bit
                index |> insert(key, known)
            }
        }
    }
}

//! every form of the dictionary's nouns, ё spelled е; a form two nouns of one gender spell carries the cases of both, one nouns of two genders spell keeps the first gender's
def private noun_dictionary() : table<string; NounForm> {
    var index : table<string; NounForm>
    add_dictionary_nouns(index, RU_FEMININE_NOUNS, RuGender.feminine)
    add_dictionary_nouns(index, RU_NEUTER_NOUNS, RuGender.neuter)
    add_dictionary_nouns(index, RU_MASCULINE_NOUNS, RuGender.masculine)
    return <- index
}
let private NOUN_DICTIONARY <- noun_dictionary()

//! the dictionary's form the lowercase `word` spells; no case bit where it holds none
def private dictionary_form(word : string) : NounForm => NOUN_DICTIONARY?[replace(word, "ё", "е")] ?? NounForm()
def private spells(f : NounForm; bit : int) : bool => (f.cases & (1 << bit)) != 0
def private spells_only(f : NounForm; bit : int) : bool => f.cases == 1 << bit

//! every singular form of the masculine nouns in a consonant, -ь or -й, ё spelled е, the genitive in -а or -я where the string gives none
def private masculine_noun_forms() : table<string> {
    var forms : table<string>
    for (entry in RU_MASCULINE_CONSONANT_NOUNS) {
        var inscope parts <- split(entry, " ")
        let one = parts[0]
        let few = length(parts) > 1 ? parts[1] : (ends_with_any(one, ["ь", "й"]) ? "{chop_end(one, 1)}я" : "{one}а")
        each_noun_form(UnitForms(one = one, few = few)) $(form, _) {
            forms |> insert(replace(form, "ё", "е"))
        }
    }
    return <- forms
}
let private MASCULINE_NOUN_FORMS <- masculine_noun_forms()
let private NOUN_HOMOGRAPHS <- { for (form in RU_NOUN_HOMOGRAPHS); form }

//! whether the lowercase `word` is a noun's form and no verb's, adjective's or other word's (доме, капитана, стоимостью; not жила, стали, дома)
def private is_noun_form(word : string) : bool {
    let key = replace(word, "ё", "е")
    return false if (empty(key) || key_exists(NOUN_HOMOGRAPHS, key))
    return dictionary_form(key).cases != 0 || key_exists(NOUN_FORMS, key) || key_exists(MASCULINE_NOUN_FORMS, key)
}

//! the endings of a neuter noun in -ие in the prepositional singular (здании, решении, условии); any other -и is feminine (части, линии, партии)
let private NEUTER_PREPOSITIONAL_ENDINGS <- ["ании", "ении", "етии", "ятии", "ытии", "итии", "одии", "овии", "елии"]

//! whether the prepositional singular `word` is a feminine noun's - a table noun's, the dictionary's, else one in -и outside the neuter endings; masculine and neuter read alike there (одном)
def private is_feminine_prepositional(word : string) : bool {
    let u = noun_of_form(word)
    return unit_gender(u) == RuGender.feminine if (!empty(u.one) && noun_for_count(u, 1l, RuCase.prep) == word)
    let known = dictionary_form(word)
    return known.gender == RuGender.feminine if (spells(known, int(RuCase.prep)))
    return word |> ends_with("и") && !ends_with_any(word, NEUTER_PREPOSITIONAL_ENDINGS)
}

//! whether the word after a count ending in one is a prepositional singular (в двадцати одном городе): the dictionary's form, else four letters or more in -е or -и - no conjunction и, no numeral две - with no neuter nominative in -е (решение, поле) and no в час ночи
def private is_prepositional_singular(word : string) : bool {
    let known = dictionary_form(word)
    return spells(known, int(RuCase.prep)) if (known.cases != 0 && word != "ночи")
    let neuter_nominative = word == "поле" || word == "море" || ends_with_any(word, ["ие", "ье", "ое", "ее", "це", "ще"])
    return false if (utf8_length(word) < 4 || word == "ночи" || neuter_nominative)
    return ends_with_any(word, ["е", "и"])
}

let private FEMININE_OBLIQUE_ENDINGS <- ["ы", "и", "е", "ью", "ой", "ей"]
//! the stems before -ий whose word is an adjective (русский, хороший), not a genitive plural (линий, стратегий)
let private ADJECTIVE_II_STEMS <- ["к", "ж", "ч", "ш", "щ"]
//! the endings of a word an adjective agrees through to the noun after it (две тонкие металлические цепочки); -ой only before a feminine noun in the singular of an oblique case
let private AGREEING_ENDINGS <- ["ые", "ие", "ых", "их", "ыми", "ими", "ая", "яя", "ую", "юю", "ого", "его", "ому", "ему", "ый", "ое"]
//! the neuter nouns in -ие (решение, событие, условие, усилие), no adjectives
let private NEUTER_IE_ENDINGS <- ["ние", "тие", "вие", "лие", "сие", "рие"]
//! the adverbs that end as an adjective does
let private ADJECTIVE_LIKE_ADVERBS <- { "потому", "почему", "поэтому", "наверное" }

//! a word's last two letters with ы read as и, which two adjectives agreeing with one noun share (тонкие металлические, новых русских)
def private agreement_class(word : string) : string => replace(slice(word, length(word) - 4), "ы", "и")

//! whether `next` is a feminine noun in the singular of an oblique case (группе, войны), no plural in -ми
def private is_feminine_oblique(next : string) : bool => ends_with_any(next, FEMININE_OBLIQUE_ENDINGS) && !(next |> ends_with("ми"))

//! a second adjective in another case or number ends the phrase (три украшенные ветвистыми рогами важенки)
def private agrees_through(word, next : string) : bool {
    return false if (utf8_length(word) < 4 || ends_with_any(word, NEUTER_IE_ENDINGS) || key_exists(ADJECTIVE_LIKE_ADVERBS, word))
    let adjective = ends_with_any(word, AGREEING_ENDINGS) || (word |> ends_with("ий") && ends_with_any(chop_end(word, 2), ADJECTIVE_II_STEMS))
    return false if (adjective && ends_with_any(next, AGREEING_ENDINGS) && agreement_class(word) != agreement_class(next))
    return adjective || (word |> ends_with("ой") && is_feminine_oblique(next))
}

//! the noun after a number, the adjective right before it and the lowercase word after it, read across up to four adjectives a lowercase word of three letters or more, no preposition, follows; a capitalized word is a name, whose ending fixes no case
struct private NounPhrase {
    noun : string
    adjective : string
    next : string
    named : bool
}

def private noun_phrase_after(c : array<int>; e : int) : NounPhrase {
    var phrase : NounPhrase
    var s = spaces_end(c, e)
    return phrase if (s == e)
    phrase.noun = word_at(c, s)
    phrase.named = cpt_is_cyrillic_upper(cp_at(c, s))
    return phrase if (phrase.named)
    for (_ in range(4)) {
        let n = spaces_end(c, letters_end(c, s))
        let next = (n == letters_end(c, s) || cpt_is_cyrillic_upper(cp_at(c, n))) ? "" : word_at(c, n)
        phrase.next = (utf8_length(next) < 3 || key_exists(CASE_GOVERNED_BY, next)) ? "" : next
        if (empty(phrase.next) || !agrees_through(phrase.noun, phrase.next)) {
            break
        }
        phrase.adjective = phrase.noun
        phrase.noun = phrase.next
        s = n
    }
    return phrase
}

//! past из after the number ending at `e` (один из них, два из пяти); -1 where no из follows
def private partitive_end(c : array<int>; e : int) : int {
    let s = spaces_end(c, e)
    let we = letters_end(c, s)
    return (s == e || !run_is(c, s, we, "из")) ? -1 : we
}

//! the noun phrase whose genitive plural a number before из is part of, across a number after из (один из 2 изваяний)
def private partitive_whole(c : array<int>; of : int) : NounPhrase {
    let n = spaces_end(c, of)
    return noun_phrase_after(c, is_digit(cp_at(c, n)) ? grouped_end(c, n) : of)
}

//! the accusative endings of a feminine noun (кассету, страницу, серию, спальню) no verb in the first person and no adverb shares
let private FEMININE_ACCUSATIVE_ENDINGS <- ["ку", "ну", "цу", "ту", "ию", "ню"]

//! a masculine noun's nominative in no vowel, which makes a word in -ой before it an adjective (один большой дом)
def private is_masculine_nominative(word : string) : bool => utf8_length(word) >= 3 && !ends_with_any(word, VOWEL_ENDINGS) && !(word |> ends_with("сь"))

//! the endings of a word in -а or -я the dictionary does not hold that are no masculine genitive: a past tense, an adjective, a reflexive, the feminine suffixes (строчка, туфелька, спаленка, ученица, унция)
let private NOT_MASCULINE_GENITIVE_ENDINGS <- ["ла", "ая", "яя", "ся", "чка", "шка", "мка", "лька", "енка", "ица", "ия", "ха", "вна"]

//! whether the noun after one is a word in -а or -я the dictionary does not hold, no adjective before it, read as a masculine genitive (одного билета)
def private is_masculine_genitive_guess(phrase : NounPhrase) : bool {
    let unknown = dictionary_form(phrase.noun).cases == 0 && empty(phrase.adjective) && utf8_length(phrase.noun) >= 4
    return unknown && ends_with_any(phrase.noun, ["а", "я"]) && !ends_with_any(phrase.noun, NOT_MASCULINE_GENITIVE_ENDINGS)
}

//! the words before one that ask a nominative of the noun after it (ещё одна орхидея); not только, which asks either (иметь только одного ребёнка)
let private NOMINATIVE_CONTEXT_WORDS <- { "ещё", "еще", "всего", "целая", "целый" }

//! whether the word before one leaves a masculine genitive guess standing: no feminine past verb - a word in -ла the dictionary holds no noun form of (входила сорок одна коммуна; до начала одного урока) - and no word asking a nominative
def private allows_genitive_guess(prep : string) : bool {
    let feminine_past = prep |> ends_with("ла") && dictionary_form(prep).cases == 0
    return !feminine_past && !key_exists(NOMINATIVE_CONTEXT_WORDS, prep)
}

//! the case the noun phrase's own ending fixes whatever stands before a count not ending in one: -ами, -ями, -ьми or an adjective in -ыми, -ими the instrumental, a noun of four letters or more in -ам or -ям, or an adjective in -ым, -им before one, the dative; after one a dictionary form spelling one case alone (одну руку, одного окна, одной рукой), an adjective in -ым, -им before a masculine instrumental, a word in -а or -я the dictionary does not hold the masculine genitive (одного билета), an adjective in -ую the accusative, one in -ого the genitive and a noun of five letters or more in -ой or -ью, no masculine noun after it, the instrumental; nominative where it fixes none
def private phrase_case(phrase : NounPhrase; one : bool) : RuCase {
    return RuCase.acc if (phrase.noun == "раз" || (phrase.noun == "лет" && phrase.next == "назад"))
    if (one) {
        let known = dictionary_form(phrase.noun)
        return RuCase.acc if (known.gender == RuGender.feminine && spells_only(known, int(RuCase.acc)))
        return RuCase.gen if (spells_only(known, int(RuCase.gen)))
        let dictionary_instrumental = spells_only(known, int(RuCase.instr)) && (empty(phrase.adjective) || ends_with_any(phrase.adjective, ["ой", "ей", "ым", "им"]))
        let adjective_instrumental = ends_with_any(phrase.noun, ["ым", "им"]) && ends_with_any(phrase.next, ["ом", "ем", "ём", "ым", "им"])
        return RuCase.instr if (dictionary_instrumental || adjective_instrumental)
        return RuCase.gen if (is_masculine_genitive_guess(phrase))
        let feminine_accusative = empty(phrase.adjective) && utf8_length(phrase.noun) >= 5 && ends_with_any(phrase.noun, FEMININE_ACCUSATIVE_ENDINGS)
        return RuCase.acc if (ends_with_any(phrase.adjective, ["ую", "юю"]) || feminine_accusative)
        return RuCase.gen if (ends_with_any(phrase.adjective, ["ого", "его"]))
        let instrumental = utf8_length(phrase.noun) >= 5 && ends_with_any(phrase.noun, ["ой", "ью"]) && !is_masculine_nominative(phrase.next)
        return (empty(phrase.adjective) && instrumental) ? RuCase.instr : RuCase.nom
    }
    let bare = empty(phrase.adjective)
    let instrumental = ends_with_any(phrase.noun, ["ами", "ями", "ьми"]) && (bare || ends_with_any(phrase.adjective, ["ыми", "ими"]))
    return RuCase.instr if (instrumental || ends_with_any(phrase.adjective, ["ыми", "ими"]))
    let dative = utf8_length(phrase.noun) >= 4 && ends_with_any(phrase.noun, ["ам", "ям"]) && (bare || ends_with_any(phrase.adjective, ["ым", "им"]))
    let adjective_dative = ends_with_any(phrase.noun, ["ым", "им"]) && utf8_length(phrase.next) >= 4 && ends_with_any(phrase.next, ["ам", "ям"])
    return (dative || adjective_dative) && phrase.noun != "пополам" ? RuCase.dat : RuCase.nom
}

//! whether the phrase is a genitive plural, which a count of two to four takes only in an oblique case or as an animate accusative (двух детей, двух толстеньких лягушек, сохранение трёх фаз): the dictionary's genitive plural that is no singular form, a noun of four letters or more in -ов, -ев or -ей - no adjective before a feminine noun (нашей заправки) - or a noun in -ек or -ок after an adjective in -ых or -их
def private is_genitive_plural(phrase : NounPhrase) : bool {
    let noun = phrase.noun
    let known = dictionary_form(noun)
    return known.cases == 1 << GENITIVE_PLURAL if (known.cases != 0)
    let ei = noun |> ends_with("ей") && !(noun |> ends_with("цей")) && !is_feminine_oblique(phrase.next)
    return true if (utf8_length(noun) >= 4 && (ends_with_any(noun, ["ов", "ев"]) || ei))
    return ends_with_any(phrase.adjective, ["ых", "их"]) && ends_with_any(noun, ["ек", "ок"])
}

//! whether `noun` is a unit's genitive plural, which a measure written loosely puts after two to four (в длину 4 метров)
def private is_unit_plural(noun : string) : bool {
    for (u in values(UNITS_RU)) {
        return true if (u.many == noun)
    }
    for (u in values(ONE_LETTER_UNITS_RU)) {
        return true if (u.many == noun)
    }
    return false
}

//! the plural pronouns that agree with a number after them and the case they give it (этими двумя, этих двух)
let private PLURAL_MODIFIERS <- {
    "этих" => RuCase.gen, "тех" => RuCase.gen, "всех" => RuCase.gen, "моих" => RuCase.gen, "твоих" => RuCase.gen, "своих" => RuCase.gen,
    "наших" => RuCase.gen, "ваших" => RuCase.gen, "этими" => RuCase.instr, "теми" => RuCase.instr, "всеми" => RuCase.instr,
    "моими" => RuCase.instr, "твоими" => RuCase.instr, "своими" => RuCase.instr, "нашими" => RuCase.instr, "вашими" => RuCase.instr,
    "этим" => RuCase.dat, "моим" => RuCase.dat, "твоим" => RuCase.dat, "своим" => RuCase.dat, "нашим" => RuCase.dat, "вашим" => RuCase.dat
}

//! the stems of the nouns for a person whose age лет before a number gives in the genitive (женщина лет тридцати, мальчика лет пяти)
let private PERSON_STEMS <- ["мужчин", "женщин", "девушк", "девочк", "девчонк", "парн", "парен", "мальчик", "мальчишк", "ребён", "ребен",
    "дочк", "дочь", "дочер", "сын", "малыш", "брат", "сестр", "тётк", "тетк", "мужик", "юнош", "блондин", "брюнет", "дама", "даму",
    "даме", "дамой", "матрос", "подрост", "дети", "детей", "детьми", "внук", "внучк", "старик", "старух", "бабушк", "дедушк", "официант", "пацан", "школьни", "студент", "дит", "незнаком",
    "человек"]

//! whether the word before лет at `i` names a person or is вид or возрасте, which makes лет and the number after it an age (женщина лет тридцати, на вид лет сорока); after a comma the age is a parenthesis, read either way, and stays as written
def private is_age_after_person(c : array<int>; i : int) : bool {
    let span = word_span_before(c, i)
    return false if (span.x == span.y || !run_is(c, span.x, span.y, "лет"))
    let person = word_before(c, span.x)
    return true if (person == "вид" || person == "возрасте")
    for (stem in PERSON_STEMS) {
        return true if (person |> starts_with(stem))
    }
    return false
}

//! the prepositions an inverted approximation puts between its noun and its number (минуты через две, метрах в двадцати)
let private INVERTING_PREPOSITIONS <- { "в", "через", "на", "за", "с" }
//! the table nouns of time and way an inverted approximation counts beside the units (дня через два, недели две, шагах в двух)
let private INVERTING_TABLE_NOUNS <- { "день", "неделя", "месяц", "шаг", "раз" }

def private add_inverting_forms(var index : table<string; UnitForms>; u : UnitForms) {
    return if (is_indeclinable(u) || is_scale(u))
    for (form in [u.few, u.many, noun_case(u, RuCase.prep, true)]) {
        index |> insert(form, u)
    }
}

//! the genitive singular and plural and the prepositional plural of the units and of `INVERTING_TABLE_NOUNS`, keyed to their forms - no indeclinable unit and no scale word; лет is the age's, never counted so
def private inverting_count_nouns() : table<string; UnitForms> {
    var index : table<string; UnitForms>
    for (u in values(UNITS_RU)) {
        add_inverting_forms(index, u)
    }
    for (u in values(ONE_LETTER_UNITS_RU)) {
        add_inverting_forms(index, u)
    }
    for (nominative in keys(INVERTING_TABLE_NOUNS)) {
        add_inverting_forms(index, noun_of_form(nominative))
    }
    return <- index
}
let private INVERTING_COUNT_NOUNS <- inverting_count_nouns()

//! the count noun of an inverted approximation and the case it gives the number after it (часа два, минуты через две, метрах в двадцати): the prepositional plural's own, else the case of a preposition between them, else of the word before the noun; empty forms where none stands
struct private InvertedCount {
    forms : UnitForms
    cas : RuCase
}

//! whether the word after the number ending at `e` is no noun it counts - none, a short word, a preposition, an adverb of time, or a word that is no dictionary, table or unit noun and has no plural noun ending (недели две назад, минуты две павловчанам)
def private nothing_counted_after(c : array<int>; e : int) : bool {
    let s = spaces_end(c, range_second_half_end(c, e))
    let word = word_at(c, s)
    let lowercase = cpt_is_cyrillic(cp_at(c, s)) && !cpt_is_cyrillic_upper(cp_at(c, s))
    let noun = dictionary_form(word).cases != 0 || key_exists(NOUN_FORMS, word) || key_exists(UNITS_RU, word) || key_exists(ONE_LETTER_UNITS_RU, word)
    let plural = ends_with_any(word, ["ами", "ями", "ах", "ях", "ам", "ям"])
    return !lowercase || utf8_length(word) < 3 || key_exists(CASE_GOVERNED_BY, word) || word == "назад" || !(noun || plural)
}

//! whether the lowercase `word` is a cardinal's word in any case (двадцать, пяти, сорока), as a number read before it leaves it
def private is_cardinal_word(word : string) : bool {
    return false if (empty(word))
    for (cas in ALL_CASES) {
        let row = int(cas)
        let unit_or_teen = find_index(CARD_UNITS_MASCULINE[row], word) >= 0 || find_index(CARD_TEENS[row], word) >= 0
        return true if (unit_or_teen || find_index(CARD_TENS[row], word) >= 0 || find_index(CARD_HUNDREDS[row], word) >= 0)
    }
    return false
}

//! с before the noun of an approximation is about, the accusative (с минут десять); a number before the noun, as digits or read already, counts it (двадцать минут двадцать первого)
[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-inverted-approximation")]
def private inverted_count_before(c : array<int>; i, e : int) : InvertedCount {
    return InvertedCount() if (grouped_end(c, i) - i > 3)
    var span = word_span_before(c, i)
    let between = utf8_to_lower(cpts_str(c, span.x, span.y))
    let across = key_exists(INVERTING_PREPOSITIONS, between)
    if (across) {
        span = word_span_before(c, span.x)
    }
    let noun = utf8_to_lower(cpts_str(c, span.x, span.y))
    let u = INVERTING_COUNT_NOUNS?[noun] ?? UnitForms()
    let counted_before = is_digit(prev_nonspace(c, span.x)) || is_cardinal_word(word_before(c, span.x))
    return InvertedCount() if (empty(u.one) || noun == "лет" || counted_before || !nothing_counted_after(c, e))
    let prepositional = noun == noun_case(u, RuCase.prep, true)
    let outer = word_before(c, span.x)
    let governed = across ? (CASE_GOVERNED_BY?[between] ?? RuCase.nom) : ((outer == "с" || outer == "со") ? RuCase.acc : (CASE_GOVERNED_BY?[outer] ?? RuCase.nom))
    return InvertedCount(forms = u, cas = prepositional ? RuCase.prep : governed)
}

//! whether a preposition `prep` may govern the instrumental (с, за, под, над, между, перед) or governs nothing
def private takes_instrumental(prep : string) : bool =>
    !key_exists(CASE_GOVERNED_BY, prep) || prep == "с" || prep == "со" || prep == "за" || prep == "под" || (CASE_GOVERNED_BY?[prep] ?? RuCase.nom) == RuCase.instr

//! the case one of a partitive takes (один из): the prepositional after в, во or на, the instrumental after являться or a linking verb but быть (стал одним из; это был один из), else the case the word before governs - ни один из, the subject, as ни одного из, the object
def private partitive_case(prep : string) : RuCase {
    return RuCase.prep if (prep == "в" || prep == "во" || prep == "на")
    let linking = (key_exists(LINKING_VERBS, prep) && !(prep |> starts_with("был"))) || prep |> starts_with("явля")
    return linking ? RuCase.instr : (CASE_GOVERNED_BY?[prep] ?? RuCase.nom)
}

//! whether ни before one asks the genitive of negation: no nominative after it (ни одного слова, ни один человек)
def private negated_to_genitive(prep, noun : string; one : bool) : bool {
    let known = dictionary_form(noun)
    let word = utf8_length(noun) >= 3 && !key_exists(CASE_GOVERNED_BY, noun)
    return prep == "ни" && one && word && !spells(known, int(RuCase.nom)) && !(known.cases == 0 && is_masculine_nominative(noun))
}

//! the numbered nouns a number after a noun gives its rank with, the genitive ordinal's form each takes (капитана первого ранга, ожог второй степени)
let private RANK_GENITIVES <- { "ранга" => RuOrdinalForm.gen_m, "класса" => RuOrdinalForm.gen_m, "разряда" => RuOrdinalForm.gen_m,
    "степени" => RuOrdinalForm.obl_f, "категории" => RuOrdinalForm.obl_f }

//! whether the word right before the number at `i`, no preposition between, is a noun the number labels - a noun's form, or a capitalized word inside a sentence (в доме семь, Терминала один) - no word that governs the number's case (в течение, порядка)
def private labelled_by_noun(c : array<int>; i : int) : bool {
    let span = word_span_before(c, i)
    let word = utf8_to_lower(cpts_str(c, span.x, span.y))
    let before = prev_nonspace(c, span.x)
    let named = span.y > span.x && cpt_is_cyrillic_upper(c[span.x]) && before >= 0 && before != '.' && before != '!' && before != '?' && !is_dash(before)
    return (is_noun_form(word) || named) && !key_exists(CASE_GOVERNED_BY, word)
}

//! whether the word after the number ending at `e` heads a number after it, as digits or read already (Терминала один стоимостью сорок два), so it is no noun the number counts
def private heads_next_number(c : array<int>; e : int) : bool {
    let s = spaces_end(c, e)
    let we = letters_end(c, s)
    let n = spaces_end(c, we)
    return s > e && we > s && n > we && (is_digit(cp_at(c, n)) || is_cardinal_word(word_at(c, n)))
}

//! the case the words around the number at [i, e) give it, a partitive's and an inverted approximation's aside
def private case_from_words(c : array<int>; i, e : int) : RuCase {
    let prep = governing_word(c, i)
    let whole = grouped_end(c, i) == e
    let phrase = noun_phrase_after(c, range_second_half_end(c, e))
    let noun = phrase.noun
    let v = digits_value(c, i, grouped_end(c, i))
    let one = counts_as_one(v)
    let modifier = one ? RuCase.nom : (PLURAL_MODIFIERS?[prep] ?? RuCase.nom)
    let genitive_modifier = modifier == RuCase.gen && (utf8_length(noun) < 3 || is_genitive_plural(phrase) || ends_with_any(noun, ["ах", "ях", "ек", "ок"]))
    let head = range_first_half_start(c, i)
    let age = prep == "лет" && is_age_after_person(c, head >= 0 ? head : i)
    let governed = (genitive_modifier || age || (whole && negated_to_genitive(prep, noun, one))) ? RuCase.gen : (CASE_GOVERNED_BY?[prep] ?? RuCase.nom)
    let plural_prepositional = ends_with_any(noun, ["ах", "ях"])
    let prepositional = plural_prepositional || (one && is_prepositional_singular(noun))
    return RuCase.prep if (((prep == "в" || prep == "во" || prep == "на") && prepositional) || (plural_prepositional && (governed == RuCase.prep || modifier == RuCase.gen)))
    let instrumental = one && ends_with_any(noun, ["ом", "ём", "ем", "ой", "ей", "ью"])
    return RuCase.instr if ((prep == "с" || prep == "со") && instrumental)
    return RuCase.dat if (prep == "по" && one && ends_with_any(noun, ["у", "ю"]))
    let single = one && range_second_half_end(c, e) == e
    let guessed = single && is_masculine_genitive_guess(phrase) && !allows_genitive_guess(prep)
    let own = (whole && !phrase.named && !heads_next_number(c, e) && !guessed) ? phrase_case(phrase, single) : RuCase.nom
    let open = governed == RuCase.nom || governed == RuCase.acc
    let free = !key_exists(CASE_GOVERNED_BY, prep) && range_second_half_end(c, e) == e
    let dative = prep == "по" || modifier == RuCase.dat || free || governed == RuCase.dat
    return own if ((own == RuCase.instr && takes_instrumental(prep)) || (own == RuCase.dat && dative) || ((own == RuCase.acc || own == RuCase.gen) && open))
    let alone = whole && !phrase.named && range_second_half_end(c, e) == e && v >= 2l && v <= 4l
    return (alone && open && is_genitive_plural(phrase) && !is_unit_plural(noun)) ? RuCase.gen : governed
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-case-government")]
def private governed_case(c : array<int>; i, e : int) : RuCase {
    let whole = grouped_end(c, i) == e
    let inverted = inverted_count_before(c, i, e)
    return inverted.cas if (whole && !empty(inverted.forms.one))
    let of = whole ? partitive_end(c, e) : -1
    return partitive_case(governing_word(c, i)) if (of >= 0 && !is_digit(cp_at(c, spaces_end(c, of))))
    return case_from_words(c, i, e)
}

let private PLACING_WORDS <- { "от", "к", "ко", "на", "по", "выше", "ниже", "вверх", "вниз", "юго", "северо" }

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-placing-distance")]
def private governed_unit_case(c : array<int>; i, unit_end : int) : RuCase {
    let prep = governing_word(c, i)
    let next = word_after(c, unit_end)
    if ((prep == "в" || prep == "во") && (key_exists(PLACING_WORDS, next) || next |> ends_with("нее"))) {
        return RuCase.prep
    }
    return CASE_GOVERNED_BY?[prep] ?? RuCase.nom
}

//! the suffix after `5-` and the form it spells; for -е, -й and -м that form is the fallback the words around the number override (`ordinal_suffix_form`)
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

//! an opening parenthesis marks a date standing alone, read in the nominative (пятое мая)
def private day_form(c : array<int>; i : int) : RuOrdinalForm {
    let prep = word_before(c, i)
    return RuOrdinalForm.nom_n if (prep == "на" || prep == "по" || prev_nonspace(c, i) == '(')
    return RuOrdinalForm.dat_m if (prep == "к" || prep == "ко")
    return RuOrdinalForm.gen_m
}

def private year_genitive(year : int64) : string => "{ordinal_words_ru(year, RuOrdinalForm.gen_m)} года"

//! `dd.mm.yyyy` and `dd-mm-yyyy`, or `yyyy-m-d` with one or two digits for the month and the day where no hyphen chain runs into it (a DOI's 1471-2180-9-16 is no date)
def private date_numeric(c : array<int>; i : int; var out : array<int>) : int {
    let e1 = digits_end(c, i)
    let sep = cp_at(c, e1)
    return i if ((sep != '.' && sep != '-') || !is_digit(cp_at(c, e1 + 1)))
    let e2 = digits_end(c, e1 + 1)
    return i if (cp_at(c, e2) != sep || !is_digit(cp_at(c, e2 + 1)))
    let e3 = digits_end(c, e2 + 1)
    return i if (is_word(cp_at(c, e3)))
    let year_first = sep == '-' && e1 - i == 4 && e2 - e1 - 1 <= 2 && e3 - e2 - 1 <= 2 && prev_nonspace(c, i) != '-'
    return i if (!year_first && (e1 - i > 2 || e2 - e1 - 1 != 2 || e3 - e2 - 1 != 4))
    let month = digits_value(c, e1 + 1, e2)
    let day = digits_value(c, year_first ? e2 + 1 : i, year_first ? e3 : e1)
    let year = digits_value(c, year_first ? i : e2 + 1, year_first ? e1 : e3)
    return i if (day < 1l || day > 31l || month < 1l || month > 12l || (year_first && !is_year(year)))
    emit(out, "{ordinal_words_ru(day, day_form(c, i))} {MONTHS_GEN[int(month) - 1]} {year_genitive(year)}")
    return e3
}

//! the month whose genitive the run at [a, b) spells, 0-based; -1 for none
def private month_index(c : array<int>; a, b : int) : int {
    for (k in range(12)) {
        return k if (run_is(c, a, b, MONTHS_GEN[k]))
    }
    return -1
}

def private month_at(c : array<int>; e : int) : int {
    let s = spaces_end(c, e)
    return month_index(c, s, letters_end(c, s))
}

let private MONTH_CASE_ENDINGS <- { "", "ь", "й", "я", "а", "е", "ю", "у", "ём", "ем", "ом" }

//! the byte length of the month stem the lowercase `w` starts with (июл of июле), март tried before ма; -1 for none
def private month_stem_length(w : string) : int {
    for (genitive in MONTHS_GEN) {
        let stem = chop_end(genitive, 1)
        return length(stem) if (w |> starts_with(stem))
    }
    return -1
}

//! whether the lowercase `w` names a month in any case (июль, июле, июлем, мая)
def private is_month_name(w : string) : bool {
    let n = month_stem_length(w)
    return n >= 0 && key_exists(MONTH_CASE_ENDINGS, slice(w, n))
}

//! what joins a number to a second one: a dash or a slash - a slash joins as a dash does (сезон 1993/94) - or the word по, до, на, и (lowercase, empty for a dash); `joint` is what reads between the two, `second_start` where the second starts, -1 where no link follows
struct private RangeLink {
    by_dash : bool
    by_slash : bool
    word : string
    joint : string
    second_start : int
}

def private range_link(c : array<int>; e : int) : RangeLink {
    let s = spaces_end(c, e)
    return RangeLink(by_dash = true, joint = " — ", second_start = spaces_end(c, s + 1)) if (is_dash(cp_at(c, s)))
    return RangeLink(by_dash = true, by_slash = true, joint = " — ", second_start = spaces_end(c, s + 1)) if (cp_at(c, s) == '/')
    let we = letters_end(c, s)
    let w = word_at(c, s)
    if (s > e && spaces_end(c, we) > we && (w == "по" || w == "до" || w == "на" || w == "и")) {
        return RangeLink(word = w, joint = " {cpts_str(c, s, we)} ", second_start = spaces_end(c, we))
    }
    return RangeLink(second_start = -1)
}

def private is_day_at(c : array<int>; b : int) : bool {
    let e = digits_end(c, b)
    let v = digits_value(c, b, e)
    return e > b && e - b <= 2 && v >= 1l && v <= 31l
}

def private year_after_month(c : array<int>; me : int; var out : array<int>) : int {
    let s2 = spaces_end(c, me)
    let ye = digits_end(c, s2)
    return me if (ye - s2 != 4 || !is_year(digits_value(c, s2, ye)) || is_word(cp_at(c, ye)))
    emit(out, " {year_genitive(digits_value(c, s2, ye))}")
    let s3 = spaces_end(c, ye)
    let we = letters_end(c, s3)
    return we if (run_is(c, s3, we, "года"))
    return we + 1 if (run_is(c, s3, we, "г") && cp_at(c, we) == '.')
    return ye
}

def private date_day_month(c : array<int>; i : int; var out : array<int>) : int {
    return i if (!is_day_at(c, i))
    var e = digits_end(c, i)
    var day = digits_value(c, i, e)
    var form = day_form(c, i)
    let link = range_link(c, e)
    if (link.second_start >= 0 && is_day_at(c, link.second_start) && month_at(c, digits_end(c, link.second_start)) >= 0) {
        emit(out, "{ordinal_words_ru(day, form)}{link.joint}")
        if (link.word == "до") {
            form = RuOrdinalForm.gen_m
        } elif (link.word == "по" || link.word == "на") {
            form = RuOrdinalForm.nom_n
        }
        e = digits_end(c, link.second_start)
        day = digits_value(c, link.second_start, e)
    }
    let month = month_at(c, e)
    return i if (month < 0)
    emit(out, "{ordinal_words_ru(day, form)} {MONTHS_GEN[month]}")
    return year_after_month(c, letters_end(c, spaces_end(c, e)), out)
}

//! the form a year or a century takes before its noun; a plural noun asks the singular of each half of its range (в 2001—2003 годах)
let private ORDINAL_NOUN_FORMS <- {
    "год" => RuOrdinalForm.nom_m, "года" => RuOrdinalForm.gen_m, "году" => RuOrdinalForm.prep_m, "годом" => RuOrdinalForm.instr_m,
    "годы" => RuOrdinalForm.nom_m, "годов" => RuOrdinalForm.gen_m, "годам" => RuOrdinalForm.dat_m, "годами" => RuOrdinalForm.instr_m,
    "годах" => RuOrdinalForm.prep_m, "век" => RuOrdinalForm.nom_m, "века" => RuOrdinalForm.gen_m, "веку" => RuOrdinalForm.dat_m,
    "веком" => RuOrdinalForm.instr_m, "веке" => RuOrdinalForm.prep_m, "веков" => RuOrdinalForm.gen_m, "векам" => RuOrdinalForm.dat_m,
    "веками" => RuOrdinalForm.instr_m, "веках" => RuOrdinalForm.prep_m, "столетие" => RuOrdinalForm.nom_n,
    "столетия" => RuOrdinalForm.gen_m, "столетию" => RuOrdinalForm.dat_m, "столетием" => RuOrdinalForm.instr_m,
    "столетии" => RuOrdinalForm.prep_m, "тысячелетие" => RuOrdinalForm.nom_n, "тысячелетия" => RuOrdinalForm.gen_m,
    "тысячелетию" => RuOrdinalForm.dat_m, "тысячелетием" => RuOrdinalForm.instr_m, "тысячелетии" => RuOrdinalForm.prep_m
}

//! a year or century mark: its abbreviation for one (г) and for several (гг), and what it reads as in each `RuCase` after one numeral and after a range or the plural abbreviation
struct private Mark {
    abbreviation : string
    plural_abbreviation : string
    reads_singular : array<string>
    reads_plural : array<string>
}

let private YEAR_MARK <- Mark(abbreviation = "г", plural_abbreviation = "гг", reads_singular <- ["года", "года", "году", "год", "годом", "году"],
    reads_plural <- ["годов", "годов", "годам", "годы", "годами", "годах"])
let private CENTURY_MARK <- Mark(abbreviation = "в", plural_abbreviation = "вв", reads_singular <- ["века", "века", "веку", "век", "веком", "веке"],
    reads_plural <- ["веков", "веков", "векам", "века", "веками", "веках"])
let private BARE_YEAR_FORMS <- {
    "в" => RuOrdinalForm.prep_m, "во" => RuOrdinalForm.prep_m, "с" => RuOrdinalForm.gen_m, "со" => RuOrdinalForm.gen_m,
    "до" => RuOrdinalForm.gen_m, "после" => RuOrdinalForm.gen_m, "от" => RuOrdinalForm.gen_m, "к" => RuOrdinalForm.dat_m,
    "ко" => RuOrdinalForm.dat_m, "по" => RuOrdinalForm.nom_m, "на" => RuOrdinalForm.nom_m
}

//! with no governing word a mark reads as a date's genitive in running text (2004 года)
def private mark_case(prep : string) : RuCase {
    return RuCase.prep if (prep == "в" || prep == "во")
    return RuCase.acc if (prep == "по")
    return CASE_GOVERNED_BY?[prep] ?? RuCase.gen
}

//! the ordinal form a noun or mark after a numeral asks for; `words` - what a mark (г., вв.) reads as - is empty for a noun the text keeps; `end` is past the mark or at the kept noun, -1 where neither stands
struct private OrdinalNoun {
    form : RuOrdinalForm
    words : string
    end : int
}

def private mark_at(c : array<int>; s : int; prep : string; ranged : bool; mark : Mark) : OrdinalNoun {
    let we = letters_end(c, s)
    let noun = word_at(c, s)
    return OrdinalNoun(end = -1) if ((noun != mark.abbreviation && noun != mark.plural_abbreviation) || cp_at(c, we) != '.')
    let cas = mark_case(prep)
    let plural = noun == mark.plural_abbreviation || ranged
    return OrdinalNoun(form = ORDINAL_FORM_M[int(cas)], words = plural ? mark.reads_plural[int(cas)] : mark.reads_singular[int(cas)], end = we + 1)
}

//! a year noun or a year mark at `s`, or a century noun or a century mark when `century`
def private ordinal_noun_at(c : array<int>; s : int; prep : string; ranged, century : bool) : OrdinalNoun {
    let noun = word_at(c, s)
    if (key_exists(ORDINAL_NOUN_FORMS, noun) && (noun |> starts_with("год")) != century) {
        let dative = noun == "году" && (prep == "по" || (CASE_GOVERNED_BY?[prep] ?? RuCase.nom) == RuCase.dat)
        return OrdinalNoun(form = dative ? RuOrdinalForm.dat_m : (ORDINAL_NOUN_FORMS?[noun] ?? RuOrdinalForm.nom_m), end = s)
    }
    return mark_at(c, s, prep, ranged, century ? CENTURY_MARK : YEAR_MARK)
}

let private AFTER_BARE_YEAR <- { "в", "во", "на", "с", "со", "по", "из", "к", "за", "у", "и", "а", "но", "или", "он", "она", "оно", "они", "мы", "я" }

def private nothing_counted_at(c : array<int>; s : int; prep : string) : bool {
    let nx = cp_at(c, s)
    let plain = nx < 0 || nx == ',' || nx == '.' || nx == ';' || nx == ':' || nx == ')' || nx == '!' || nx == '?' || is_dash(nx) || cpt_is_cyrillic_upper(nx)
    return plain || ((prep == "в" || prep == "во") && key_exists(AFTER_BARE_YEAR, word_at(c, s)))
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-year")]
def private year_phrase(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    return i if (e - i < 3 || e - i > 4 || (cp_at(c, e) == '-' && cpt_is_cyrillic(cp_at(c, e + 1))))
    let year = digits_value(c, i, e)
    let full = e - i == 4 && is_year(year)
    let prep = word_before(c, i)
    let link = range_link(c, e)
    var last = e
    var second_year = -1l
    if (full && link.second_start >= 0 && is_digit(cp_at(c, link.second_start))) {
        let e2 = digits_end(c, link.second_start)
        let v2 = digits_value(c, link.second_start, e2)
        return i if (cp_at(c, e2) == '-' && cpt_is_cyrillic(cp_at(c, e2 + 1)))
        let later = !link.by_slash || (e2 - link.second_start == 4 ? v2 > year : (v2 > year % 100l || v2 + 100l - year % 100l <= 10l))
        if (later && ((e2 - link.second_start == 4 && is_year(v2)) || (e2 - link.second_start == 2 && link.by_dash))) {
            second_year = v2
            last = e2
        }
    }
    let s = spaces_end(c, last)
    let noun = ordinal_noun_at(c, s, prep, second_year >= 0l, false)
    var second_half_form = noun.form
    var end = empty(noun.words) ? last : noun.end
    if (noun.end < 0 && full && second_year < 0l && is_month_name(prep)) {
        emit(out, ordinal_words_ru(year, RuOrdinalForm.gen_m))
        return e
    }
    if (noun.end < 0) {
        let bare_prep = (second_year >= 0l && !link.by_dash && link.word != "и") ? link.word : prep
        let paired = link.by_slash && second_year >= 0l
        let bare = key_exists(BARE_YEAR_FORMS, prep) && key_exists(BARE_YEAR_FORMS, bare_prep) && (paired || nothing_counted_at(c, s, prep))
        return i if (!full || !bare)
        second_half_form = BARE_YEAR_FORMS?[bare_prep] ?? RuOrdinalForm.nom_m
        end = last
    } elif (!full && !(e - i == 3 && second_year < 0l && (empty(noun.words) ? word_at(c, s) : noun.words) == "году")) {
        return i
    }
    let together = second_year < 0l || link.by_dash || link.word == "и"
    let first_half_form = together ? second_half_form : (BARE_YEAR_FORMS?[prep] ?? RuOrdinalForm.gen_m)
    emit(out, ordinal_words_ru(year, first_half_form))
    if (second_year >= 0l) {
        emit(out, "{link.joint}{ordinal_words_ru(second_year, second_half_form)}")
    }
    if (!empty(noun.words)) {
        emit(out, " {noun.words}")
    }
    return end
}

def private pass_dates(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && !is_word(cp_at(c, i - 1))) {
            var past = date_numeric(c, i, out)
            if (past == i) {
                past = date_day_month(c, i, out)
            }
            if (past == i) {
                past = year_phrase(c, i, out)
            }
            if (past > i) {
                i = past
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

let private MINUTES <- forms("минута", "минуты", "минут")
let private SECONDS <- forms("секунда", "секунды", "секунд")
//! how far back a set score looks for the parenthesis it sits in: past one line of a match report, never into the next
let private SET_SCORE_LOOKBACK = 200

//! whether the clock-shaped `h:mm` at `i` sits in a parenthesized list after a match score (3: 1 (25:21, 22:25)): a set score, not a time
def private is_set_score(c : array<int>; i : int) : bool {
    var j = i - 1
    while (j >= 0 && i - j < SET_SCORE_LOOKBACK && c[j] != '(' && c[j] != ')') {
        j--
    }
    return false if (j < 0 || c[j] != '(')
    var d = spaces_start(c, j)
    return false if (d == 0 || !is_digit(c[d - 1]))
    while (d > 0 && is_digit(c[d - 1])) {
        d--
    }
    return prev_nonspace(c, d) == ':'
}

def private clock_words(h, m, s : int64; cas : RuCase) : string {
    var words = (h == 1l && (cas == RuCase.nom || cas == RuCase.acc)) ? "час" : count_with_unit(h, HOURS, cas)
    if (m > 0l) {
        words = "{words} {count_with_unit(m, MINUTES, cas)}"
    }
    if (s > 0l) {
        words = "{words} {count_with_unit(s, SECONDS, cas)}"
    }
    return words
}

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
                    var j = e + 3
                    if (is_set_score(c, i)) {
                        emit(out, "{number_to_words_ru(h)} {m < 10l ? "ноль " : ""}{number_to_words_ru(m)}")
                        i = j
                        continue
                    }
                    var s = 0l
                    if (cp_at(c, j) == ':' && digits_end(c, j + 1) == j + 3 && digits_value(c, j + 1, j + 3) <= 59l && !is_word(cp_at(c, j + 3))) {
                        s = digits_value(c, j + 1, j + 3)
                        j += 3
                    }
                    emit(out, clock_words(h, m, s, governed_case(c, i, j)))
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

//! the verbs a predicate ordinal after them takes the instrumental with (стал 4-м)
let private LINKING_VERBS <- { "стал", "стала", "стало", "был", "была", "было", "оказался", "оказалась", "являлся", "являлась",
    "остался", "осталась", "финишировал", "финишировала" }

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-ordinal-suffix")]
def private ordinal_suffix_form(c : array<int>; i, e, se : int) : RuOrdinalForm {
    let suffix = cpts_str(c, e + 1, se)
    let form = ORDINAL_SUFFIX_FORMS?[suffix] ?? RuOrdinalForm.nom_m
    let next = word_after(c, se)
    if (suffix == "е") {
        return ends_with_any(next, ["ы", "и"]) ? RuOrdinalForm.nom_pl : form
    }
    if (suffix == "й") {
        if (ends_with_any(next, ["ой", "ей"])) {
            let noun = word_after(c, letters_end(c, spaces_end(c, se)))
            return (empty(noun) || ends_with_any(noun, FEMININE_OBLIQUE_ENDINGS)) ? RuOrdinalForm.obl_f : form
        }
        return ends_with_any(next, FEMININE_OBLIQUE_ENDINGS) ? RuOrdinalForm.obl_f : form
    }
    if (suffix == "м") {
        let prep = word_before(c, i)
        return RuOrdinalForm.dat_pl if (prep == "к" || prep == "ко")
        let instrumental = ends_with_any(next, ["ым", "им"]) || prep == "с" || prep == "со" || key_exists(LINKING_VERBS, prep)
        return instrumental ? RuOrdinalForm.instr_m : form
    }
    return form
}

def private ordinal_suffix_end(c : array<int>; e : int) : int {
    return -1 if (cp_at(c, e) != '-' || !cpt_is_cyrillic(cp_at(c, e + 1)))
    let se = letters_end(c, e + 1)
    return key_exists(ORDINAL_SUFFIX_FORMS, cpts_str(c, e + 1, se)) ? se : -1
}

def private suffixed_ordinal_at(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    return i if (e - i > MAX_DIGITS_AS_NUMBER)
    let se = ordinal_suffix_end(c, e)
    if (se > 0) {
        emit(out, ordinal_words_ru(digits_value(c, i, e), ordinal_suffix_form(c, i, e, se)))
        return se
    }
    let link = range_link(c, e)
    return i if (!link.by_dash || !is_digit(cp_at(c, link.second_start)))
    let e2 = digits_end(c, link.second_start)
    let se2 = ordinal_suffix_end(c, e2)
    return i if (se2 < 0 || e2 - link.second_start > MAX_DIGITS_AS_NUMBER)
    let form = ordinal_suffix_form(c, link.second_start, e2, se2)
    emit(out, "{ordinal_words_ru(digits_value(c, i, e), form)}{link.joint}{ordinal_words_ru(digits_value(c, link.second_start, e2), form)}")
    return se2
}

//! the second parts a number joins across a space (25 летию as двадцатипятилетию); across a hyphen any word of four letters or more joins
let private SPACED_COMPOUND_PARTS <- { "летие", "летия", "летию", "летием", "летии", "летний", "летнего", "летнему", "летним", "летнем",
    "летняя", "летней", "летнюю", "летнее", "летние", "летних", "летними" }
//! the stems of the adjectives a number joins across a space (40 градусную as сорокаградусную), the adjective's ending after them
let private SPACED_COMPOUND_STEMS <- ["процентн", "градусн", "этажн", "комнатн", "кратн", "разов", "дневн", "месячн", "минутн",
    "часов", "секундн", "серийн", "томн", "метров", "мильн", "зубов", "пушечн", "цилиндров", "арочн", "тысячн", "секционн"]
let private ADJECTIVE_ENDINGS <- ["ый", "ий", "ой", "ая", "яя", "ое", "ее", "ые", "ие", "ого", "его", "ому", "ему", "ым", "им", "ом", "ем",
    "ую", "юю", "ых", "их", "ыми", "ими"]

def private is_spaced_compound(word : string) : bool {
    for (stem in SPACED_COMPOUND_STEMS) {
        return true if (word |> starts_with(stem) && length(word) > length(stem) && ends_with_any(word, ADJECTIVE_ENDINGS))
    }
    return key_exists(SPACED_COMPOUND_PARTS, word)
}
let private COMPOUND_PART_OVERRIDES <- { "одного" => "одно", "девяноста" => "девяносто", "ста" => "сто" }
let private IOTATED_INITIALS <- utf8_to_cpts("еёюя")

//! a number as the first part of a compound word: each part in the genitive, except одно-, девяносто- and сто-, the thousands as тысяче- (двадцатипяти-, стопятидесяти-, двухтысяче-)
def private compound_prefix(n : int) : string {
    var inscope parts : array<string>
    let thousands = n / 1000
    if (thousands > 0) {
        parts |> push("{ORD_THOUSAND_PREFIX[thousands]}тысяче")
    }
    under_thousand(n % 1000, RuGender.masculine, RuCase.gen, parts)
    return join([for (p in parts); COMPOUND_PART_OVERRIDES?[p] ?? p], "")
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-compound")]
def private compound_at(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    let we = letters_end(c, e + 1)
    let word = cpts_str(c, e + 1, we)
    let hyphened = cp_at(c, e) == '-' && we - e - 1 >= 4
    let spaced = cp_at(c, e) == ' ' && is_spaced_compound(word)
    let lowercase = !cpt_is_cyrillic_upper(cp_at(c, e + 1))
    return i if (e - i > 4 || c[i] == '0' || continues_number(c, i) || !(hyphened || spaced) || !lowercase || is_word(cp_at(c, we)))
    let prefix = compound_prefix(int(digits_value(c, i, e)))
    let hard_sign = (ends_with_any(prefix, ["х", "т"]) && find_index(IOTATED_INITIALS, c[e + 1]) >= 0) ? "ъ" : ""
    emit(out, "{prefix}{hard_sign}{word}")
    return we
}


//! whether a number after `prep` may stand in `cas`: any case where `prep` governs none, else its own, the instrumental after с and за, the prepositional after в and на
def private allows_case(prep : string; cas : RuCase) : bool {
    return true if (!key_exists(CASE_GOVERNED_BY, prep))
    let governed = CASE_GOVERNED_BY?[prep] ?? RuCase.nom
    let second = (prep == "с" || prep == "со" || prep == "за") ? RuCase.instr : ((prep == "в" || prep == "во" || prep == "на") ? RuCase.prep : governed)
    return cas == governed || cas == second
}

//! the singular cases whose form of `u` the lowercase `word` is, a bit a `RuCase`
def private singular_cases_of(u : UnitForms; word : string) : int {
    var cases = 0
    for (cas in ALL_CASES) {
        cases |= noun_for_count(u, 1l, cas) == word ? 1 << int(cas) : 0
    }
    return cases
}

let private OBJECT_VERB_ENDINGS <- ["ю", "у", "ем", "ём", "им", "ть", "ти", "чь", "и", "те", "л", "ли", "ло"]

//! a verb form that makes the number after it its object: the first person, an infinitive, an imperative, a past tense not feminine singular (пилю, ждём, посмотреть, смотри, снял, выпустили); no dictionary noun
def private is_object_verb(word : string) : bool {
    return false if (utf8_length(word) < 4 || dictionary_form(word).cases != 0 || ends_with_any(word, ["ся", "сь"]))
    return ends_with_any(word, OBJECT_VERB_ENDINGS)
}

//! the singular case a noun spelling `cases` stands in after `prep`, `governed` tried first - the accusative where its nominative is its accusative and a verb that takes an object stands before the number (пилю вторую часть); -1 where `prep` allows none
def private numbered_case(cases : int; prep : string; governed : RuCase) : int {
    let both = (1 << int(RuCase.nom)) | (1 << int(RuCase.acc))
    let first = (governed == RuCase.nom && (cases & both) == both && is_object_verb(prep)) ? RuCase.acc : governed
    return int(first) if ((cases & (1 << int(first))) != 0 && allows_case(prep, first))
    for (cas in ALL_CASES) {
        return int(cas) if ((cases & (1 << int(cas))) != 0 && allows_case(prep, cas))
    }
    return -1
}

//! the genitives a clock time takes (восемь утра, в два ночи), дня only after a preposition (с двух дня; пятого дня пути)
let private CLOCK_GENITIVES <- { "дня", "утра", "вечера", "ночи" }
//! the plural determiners a count follows, never an ordinal (эти две стражи, все три части)
let private PLURAL_DETERMINERS <- { "эти", "те", "все", "мои", "твои", "свои", "наши", "ваши", "оба", "обе" }

//! the form a count not ending in one takes its noun in, a bit as in `NounForm.cases`: the genitive singular after two to four and the genitive plural after the rest in the nominative and accusative, the genitive plural in the genitive; -1 in a case the dictionary holds no plural of
def private count_form_bit(v : int64; governed : RuCase) : int {
    if (governed == RuCase.nom || governed == RuCase.acc) {
        return counts_as_few(v) ? int(RuCase.gen) : GENITIVE_PLURAL
    }
    return governed == RuCase.gen ? GENITIVE_PLURAL : -1
}

//! a dictionary noun's singular form at `s` after a count not ending in one, in a form the count cannot govern (третью маршрутку, второе дело, к пятой статье); the thousand stays a count, and a form a verb's past in -ла or -ли spells too is the verb (в доме семь жила)
def private numbered_dictionary_noun(c : array<int>; i, e, s : int; prep, word : string) : OrdinalNoun {
    let known = dictionary_form(word)
    let v = digits_value(c, i, e)
    let singular = known.cases & ((1 << GENITIVE_PLURAL) - 1)
    let homograph = key_exists(NOUN_HOMOGRAPHS, replace(word, "ё", "е")) && ends_with_any(word, ["ла", "ли"])
    return OrdinalNoun(end = -1) if (singular == 0 || homograph || counts_as_one(v) || word |> starts_with("тысяч"))
    let governed = governed_case(c, i, e)
    let own = count_form_bit(v, governed)
    return OrdinalNoun(end = -1) if (own >= 0 && spells(known, own))
    let cas = numbered_case(singular, prep, governed)
    return cas < 0 ? OrdinalNoun(end = -1) : OrdinalNoun(form = singular_form(ALL_CASES[cas], known.gender), end = s)
}

//! a table noun at `s` after the number at [i, e), else a dictionary noun where `dictionary`: a numbered noun's prepositional singular after в, во or на - not after на where the count governs it (на две части) - or a singular form the count cannot govern, a count ending in one excepted; a clock genitive is a clock time
def private numbered_table_noun(c : array<int>; i, e, s : int; prep : string; dictionary : bool) : OrdinalNoun {
    let word = word_at(c, s)
    let k = NOUN_FORMS?[word] ?? int2(-1, -1)
    return OrdinalNoun(end = -1) if (key_exists(CLOCK_GENITIVES, word) && (word != "дня" || key_exists(CASE_GOVERNED_BY, prep)))
    if (k.x < 0) {
        return dictionary ? numbered_dictionary_noun(c, i, e, s, prep, word) : OrdinalNoun(end = -1)
    }
    let u = k.x == 0 ? NUMBERED_NOUNS[k.y] : COUNTED_NOUNS[k.y]
    let v = digits_value(c, i, e)
    let governed = governed_case(c, i, e)
    let required = noun_for_count(u, v, governed)
    let gender = unit_gender(u)
    let prepositional = k.x == 0 && (prep == "в" || prep == "во" || prep == "на") && word == noun_for_count(u, 1l, RuCase.prep)
    if (prepositional && !(prep == "на" && word == noun_for_count(u, v, RuCase.acc))) {
        return OrdinalNoun(form = singular_form(RuCase.prep, gender), end = s)
    }
    return OrdinalNoun(end = -1) if (counts_as_one(v) || word == required)
    let cas = numbered_case(singular_cases_of(u, word), prep, governed)
    return cas < 0 ? OrdinalNoun(end = -1) : OrdinalNoun(form = singular_form(ALL_CASES[cas], gender), end = s)
}

//! a singular adjective ending and the ordinal form it spells; -ой, -ий and the case each allows are read by `singular_adjective`
let private SINGULAR_ADJECTIVE_FORMS <- {
    "ого" => RuOrdinalForm.gen_m, "его" => RuOrdinalForm.gen_m, "ому" => RuOrdinalForm.dat_m, "ему" => RuOrdinalForm.dat_m,
    "ую" => RuOrdinalForm.acc_f, "юю" => RuOrdinalForm.acc_f, "ая" => RuOrdinalForm.nom_f, "яя" => RuOrdinalForm.nom_f,
    "ое" => RuOrdinalForm.nom_n, "ый" => RuOrdinalForm.nom_m
}

//! whether `noun` has the ending of a noun an adjective in `form` agrees with; the masculine nominative's noun ends in no vowel
def private agrees_with_noun(form : RuOrdinalForm; noun : string) : bool {
    return ends_with_any(noun, ["а", "я", "ь"]) if (form == RuOrdinalForm.nom_f)
    return ends_with_any(noun, ["у", "ю", "ь"]) if (form == RuOrdinalForm.acc_f)
    return ends_with_any(noun, ["о", "е", "ё"]) if (form == RuOrdinalForm.nom_n)
    return ends_with_any(noun, ["а", "я"]) if (form == RuOrdinalForm.gen_m)
    return ends_with_any(noun, ["у", "ю"]) if (form == RuOrdinalForm.dat_m)
    return !ends_with_any(noun, VOWEL_ENDINGS)
}

//! whether a number after `prep` may stand in the case of an adjective in `form` whose noun is `noun`: the nominative and accusative alike for a masculine or neuter nominative, the accusative for a masculine genitive (animate), any oblique case for the feminine oblique
def private adjective_fits(form : RuOrdinalForm; noun, prep : string) : bool {
    if (form == RuOrdinalForm.obl_f) {
        let oblique = allows_case(prep, RuCase.gen) || allows_case(prep, RuCase.dat) || allows_case(prep, RuCase.instr) || allows_case(prep, RuCase.prep)
        return oblique && (empty(noun) || ends_with_any(noun, FEMININE_OBLIQUE_ENDINGS))
    }
    let agrees = agrees_with_noun(form, noun)
    let cas = form == RuOrdinalForm.gen_m ? RuCase.gen : (form == RuOrdinalForm.dat_m ? RuCase.dat : (form == RuOrdinalForm.acc_f ? RuCase.acc : RuCase.nom))
    let accusative = form != RuOrdinalForm.nom_f && form != RuOrdinalForm.dat_m && allows_case(prep, RuCase.acc)
    return agrees && (allows_case(prep, cas) || accusative)
}
//! the pronouns and adverbs that end as an adjective does
let private NON_ADJECTIVES <- { "такой", "такая", "такое", "такого", "такому", "какой", "какая", "какое", "какого", "какому", "другой",
    "другая", "другое", "другого", "другому", "который", "которая", "которое", "которого", "которому", "своего", "своему", "моего",
    "моему", "твоего", "твоему", "нашего", "нашему", "вашего", "вашему", "этого", "этому", "всего", "всему", "самый", "самая", "самое",
    "самого", "самому", "самой", "одного", "одному", "каждый", "каждая", "каждое", "каждого", "каждому", "каждой", "любой", "любая",
    "любое", "любого", "любому", "никому", "ничему", "никого", "ничего", "многого" }
//! participle endings: a participle after a number tells of its subject, not of a noun the number numbers
let private PARTICIPLE_ENDINGS <- ["щий", "щая", "щее", "вший", "вшая", "вшее"]

//! a singular adjective at `s` after a number - a word of five letters or more with a following word, the noun, unless it ends in -ой - as the ordinal form it spells; -ой is the feminine oblique before a noun with a feminine oblique ending or none, the masculine nominative otherwise
def private singular_adjective(c : array<int>; s : int; prep : string) : OrdinalNoun {
    let word = word_at(c, s)
    let noun = word_after(c, letters_end(c, s))
    var form = RuOrdinalForm.nom_m
    var found = false
    for (ending, spelled in keys(SINGULAR_ADJECTIVE_FORMS), values(SINGULAR_ADJECTIVE_FORMS)) {
        if (word |> ends_with(ending)) {
            form = spelled
            found = true
        }
    }
    let ii = word |> ends_with("ий") && ends_with_any(chop_end(word, 2), ADJECTIVE_II_STEMS)
    let oi = word |> ends_with("ой")
    if (oi) {
        form = (empty(noun) || ends_with_any(noun, FEMININE_OBLIQUE_ENDINGS)) ? RuOrdinalForm.obl_f : RuOrdinalForm.nom_m
    }
    let pronoun = key_exists(NON_ADJECTIVES, word) || key_exists(ADJECTIVE_LIKE_ADVERBS, word)
    let named = utf8_length(word) >= 5 && !pronoun && !ends_with_any(word, PARTICIPLE_ENDINGS)
    let adjective = (found || ii || oi) && named && (oi || !empty(noun))
    return (adjective && adjective_fits(form, noun, prep)) ? OrdinalNoun(form = form, end = s) : OrdinalNoun(end = -1)
}

//! the singular determiners a number after them is the ordinal of, and the form each spells (каждый двадцатый, мой третий пост, своей второй половинки)
let private SINGULAR_DETERMINERS <- {
    "мой" => RuOrdinalForm.nom_m, "моя" => RuOrdinalForm.nom_f, "моё" => RuOrdinalForm.nom_n, "мое" => RuOrdinalForm.nom_n, "мою" => RuOrdinalForm.acc_f,
    "моего" => RuOrdinalForm.gen_m, "моей" => RuOrdinalForm.obl_f, "моему" => RuOrdinalForm.dat_m, "моём" => RuOrdinalForm.prep_m, "моем" => RuOrdinalForm.prep_m,
    "твой" => RuOrdinalForm.nom_m, "твоя" => RuOrdinalForm.nom_f, "твоё" => RuOrdinalForm.nom_n, "твое" => RuOrdinalForm.nom_n, "твою" => RuOrdinalForm.acc_f,
    "твоего" => RuOrdinalForm.gen_m, "твоей" => RuOrdinalForm.obl_f, "твоему" => RuOrdinalForm.dat_m, "твоём" => RuOrdinalForm.prep_m, "твоем" => RuOrdinalForm.prep_m,
    "свой" => RuOrdinalForm.nom_m, "своя" => RuOrdinalForm.nom_f, "своё" => RuOrdinalForm.nom_n, "свое" => RuOrdinalForm.nom_n, "свою" => RuOrdinalForm.acc_f,
    "своего" => RuOrdinalForm.gen_m, "своей" => RuOrdinalForm.obl_f, "своему" => RuOrdinalForm.dat_m, "своём" => RuOrdinalForm.prep_m, "своем" => RuOrdinalForm.prep_m,
    "наш" => RuOrdinalForm.nom_m, "наша" => RuOrdinalForm.nom_f, "наше" => RuOrdinalForm.nom_n, "нашу" => RuOrdinalForm.acc_f, "нашего" => RuOrdinalForm.gen_m,
    "нашей" => RuOrdinalForm.obl_f, "нашему" => RuOrdinalForm.dat_m, "нашем" => RuOrdinalForm.prep_m,
    "ваш" => RuOrdinalForm.nom_m, "ваша" => RuOrdinalForm.nom_f, "ваше" => RuOrdinalForm.nom_n, "вашу" => RuOrdinalForm.acc_f, "вашего" => RuOrdinalForm.gen_m,
    "вашей" => RuOrdinalForm.obl_f, "вашему" => RuOrdinalForm.dat_m, "вашем" => RuOrdinalForm.prep_m,
    "этот" => RuOrdinalForm.nom_m, "эта" => RuOrdinalForm.nom_f, "эту" => RuOrdinalForm.acc_f, "этой" => RuOrdinalForm.obl_f,
    "каждый" => RuOrdinalForm.nom_m, "каждая" => RuOrdinalForm.nom_f, "каждое" => RuOrdinalForm.nom_n, "каждую" => RuOrdinalForm.acc_f,
    "каждого" => RuOrdinalForm.gen_m, "каждой" => RuOrdinalForm.obl_f, "каждому" => RuOrdinalForm.dat_m, "каждом" => RuOrdinalForm.prep_m
}

//! a bare number of one to three digits after a singular determiner, a noun after it or none, as the ordinal in the determiner's form (это, этого and этом are pronouns: при этом, после этого); no unit, year count, range or second digit group after it
def private determined_ordinal_at(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    let form = SINGULAR_DETERMINERS?[word_before(c, i)] ?? RuOrdinalForm.nom_m
    let s = spaces_end(c, e)
    let after = word_at(c, s)
    let unit = key_exists(UNITS_RU, after) || key_exists(ONE_LETTER_UNITS_RU, after) || after == "год" || after == "года" || after == "лет"
    let fraction = (cp_at(c, e) == ',' || cp_at(c, e) == '.' || cp_at(c, e) == '/') && is_digit(cp_at(c, e + 1))
    let alone = e - i <= 3 && c[i] != '0' && !continues_number(c, i) && !is_word(cp_at(c, e)) && cp_at(c, e) != '-' && !fraction && !is_digit(cp_at(c, s))
    return i if (!key_exists(SINGULAR_DETERMINERS, word_before(c, i)) || !alone || unit)
    emit(out, ordinal_words_ru(digits_value(c, i, e), form))
    return e
}

//! the table or dictionary noun whose nominative `stem` with one of the nominative endings spells, as its gender with the nominative bit; no case bit where none
def private nominative_from_stem(stem : string) : NounForm {
    for (ending in ["", "а", "я", "о", "е", "ь"]) {
        let u = noun_of_form("{stem}{ending}")
        return NounForm(gender = unit_gender(u), cases = 1 << int(RuCase.nom)) if (u.one == "{stem}{ending}")
        let known = dictionary_form("{stem}{ending}")
        return known if (spells(known, int(RuCase.nom)))
    }
    return NounForm()
}

//! a plural noun at `s` after the last number of a list, numbered one by one: its prepositional (на первом и втором этажах) or a masculine nominative (первый и второй курсы) - a feminine or neuter one is the genitive singular a count takes - in the singular form of its gender
def private listed_plural_noun(c : array<int>; s : int; prep : string) : OrdinalNoun {
    let word = word_at(c, s)
    let prepositional = ends_with_any(word, ["ах", "ях"])
    let noun = nominative_from_stem(chop_end(word, prepositional ? 2 : 1))
    let cas = prepositional ? RuCase.prep : RuCase.nom
    let plural = prepositional || (ends_with_any(word, ["ы", "и"]) && noun.gender == RuGender.masculine)
    return (noun.cases != 0 && plural && allows_case(prep, cas)) ? OrdinalNoun(form = singular_form(cas, noun.gender), end = s) : OrdinalNoun(end = -1)
}

//! the numbers of a list before the noun they number one by one, each as the ordinal the last one is (на первом и втором этажах, вторым или третьим выстрелом, первый и второй курсы); a count's own form after the last makes them counts (два или три раза)
def private listed_ordinal_at(c : array<int>; i : int; var out : array<int>) : int {
    var last = list_next_start(c, digits_end(c, i))
    let first = digits_end(c, i) - i <= 3 && c[i] != '0' && !continues_number(c, i) && !is_digit(prev_nonspace(c, i)) && cp_at(c, i - 1) != '-'
    return i if (last < 0 || !first)
    while (list_next_start(c, digits_end(c, last)) >= 0) {
        last = list_next_start(c, digits_end(c, last))
    }
    let e = digits_end(c, last)
    let s = spaces_end(c, e)
    let prep = governing_word(c, i)
    let worded = s > e && letters_end(c, s) - s >= 3 && !cpt_is_cyrillic_upper(cp_at(c, s)) && !is_word(cp_at(c, letters_end(c, s)))
    var noun = worded ? numbered_table_noun(c, last, e, s, prep, true) : OrdinalNoun(end = -1)
    if (noun.end < 0 && worded) {
        noun = listed_plural_noun(c, s, prep)
    }
    return i if (noun.end < 0)
    var k = i
    while (k < e) {
        let ke = digits_end(c, k)
        emit(out, ordinal_words_ru(digits_value(c, k, ke), noun.form))
        let next = k == last ? e : list_next_start(c, ke)
        emit(out, cpts_str(c, ke, next))
        k = next
    }
    return e
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-numbered-noun")]
def private numbered_noun_at(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    let s = spaces_end(c, e)
    let we = letters_end(c, s)
    let next = word_after(c, we)
    let placing = key_exists(PLACING_WORDS, next) || next |> ends_with("нее") || next == "назад"
    let bare = s > e && we > s && e - i <= 3 && c[i] != '0' && !continues_number(c, i) && !is_word(cp_at(c, we)) && range_first_half_start(c, i) < 0
    let before = prev_nonspace(c, i)
    let prep = word_before(c, i)
    let abbreviated = key_exists(UNITS_RU, word_at(c, s)) && (cp_at(c, we) == '.' || is_digit(cp_at(c, spaces_end(c, we))))
    let signed = cp_at(c, i - 1) == '-' || is_digit(before) || is_alpha(before)
    let noun_label = labelled_by_noun(c, i)
    if (bare && !signed && noun_label && key_exists(RANK_GENITIVES, word_at(c, s))) {
        emit(out, ordinal_words_ru(digits_value(c, i, e), RANK_GENITIVES?[word_at(c, s)] ?? RuOrdinalForm.gen_m))
        return e
    }
    return i if (!bare || placing || abbreviated || signed || key_exists(NOUN_FORMS, prep))
    let labelled = before == '.' || is_dash(before) || cpt_is_cyrillic_upper(c[s])
    let letter = word_span_before(c, i)
    let lettered = letter.y - letter.x == 1 && cpt_is_cyrillic_upper(c[letter.x]) && !key_exists(CASE_GOVERNED_BY, prep)
    let counted = labelled || lettered || key_exists(PLURAL_DETERMINERS, prep) || cp_at(c, we) == '-'
    var noun = numbered_table_noun(c, i, e, s, prep, !counted)
    if (noun.end < 0 && !labelled && !counts_as_one(digits_value(c, i, e))) {
        noun = singular_adjective(c, s, prep)
    }
    return i if (noun.end < 0)
    emit(out, ordinal_words_ru(digits_value(c, i, e), noun.form))
    return e
}

//! the lowercase word joined by a hyphen before `i` (во of во-2), empty where none stands
def private hyphened_word_before(c : array<int>; i : int) : string {
    return "" if (cp_at(c, i - 1) != '-')
    var a = i - 1
    while (a > 0 && cpt_is_cyrillic(c[a - 1])) {
        a--
    }
    return is_word(cp_at(c, a - 1)) ? "" : utf8_to_lower(cpts_str(c, a, i - 1))
}

//! во-1, во-2, в-3 to в-9, and во 1 or во 2 before a comma, as the enumerating adverb (во-первых, в-третьих)
[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-ordinal-suffix")]
def private enumeration_at(c : array<int>; i : int; var out : array<int>) : int {
    let e = digits_end(c, i)
    let v = digits_value(c, i, e)
    let hyphened = hyphened_word_before(c, i)
    let spaced = word_before(c, i) == "во" && v <= 2l && cp_at(c, e) == ','
    let enumerating = hyphened == "во" || (hyphened == "в" && v >= 3l) || spaced
    return i if (e - i != 1 || v == 0l || is_word(cp_at(c, e)) || cp_at(c, e) == '-' || !enumerating)
    emit(out, ordinal_words_ru(v, RuOrdinalForm.gen_pl))
    return e
}

def private pass_ordinals(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_digit(c[i]) && !is_word(cp_at(c, i - 1))) {
            var past = enumeration_at(c, i, out)
            if (past == i) {
                past = suffixed_ordinal_at(c, i, out)
            }
            if (past == i) {
                past = compound_at(c, i, out)
            }
            if (past == i) {
                past = listed_ordinal_at(c, i, out)
            }
            if (past == i) {
                past = determined_ordinal_at(c, i, out)
            }
            if (past == i) {
                past = numbered_noun_at(c, i, out)
            }
            if (past > i) {
                i = past
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-phone")]
def private phone_group_words(c : array<int>; a, b : int) : string {
    var inscope parts : array<string>
    var k = a
    while (k < b) {
        if (c[k] == '0') {
            parts |> push("ноль")
            k++
            continue
        }
        let rest = b - k
        let take = rest <= 3 ? rest : (rest == 4 ? 2 : 3)
        parts |> push(number_to_words_ru(digits_value(c, k, k + take)))
        k += take
    }
    return join(parts, " ")
}

def private latin_word_before(c : array<int>; i : int) : string {
    var j = i
    while (j > 0 && (is_space(c[j - 1]) || c[j - 1] == ':')) {
        j--
    }
    var a = j
    while (a > 0 && is_alpha(c[a - 1])) {
        a--
    }
    return cpts_str(c, a, j)
}

//! the digit groups of a telephone or book number, their spans and where they end
struct private PhoneGroups {
    spans : array<int2>
    led_by_plus : bool
    has_area_code : bool
    end : int
}

def private phone_groups(c : array<int>; i : int) : PhoneGroups {
    var g = PhoneGroups(led_by_plus = c[i] == '+', end = i)
    var p = g.led_by_plus ? i + 1 : i
    if (g.led_by_plus && is_digit(cp_at(c, p)) && digits_end(c, p) - p <= 3) {
        g.spans |> push(int2(p, digits_end(c, p)))
        p = spaces_end(c, digits_end(c, p))
    }
    let close = digits_end(c, p + 1)
    if (cp_at(c, p) == '(' && close > p + 1 && close - p - 1 <= 5 && cp_at(c, close) == ')') {
        g.has_area_code = true
        g.spans |> push(int2(p + 1, close))
        p = spaces_end(c, close + 1)
    }
    while (is_digit(cp_at(c, p))) {
        let e = digits_end(c, p)
        g.spans |> push(int2(p, e))
        g.end = e
        if (cp_at(c, e) != '-' || !is_digit(cp_at(c, e + 1))) {
            break
        }
        p = e + 1
    }
    return <- g
}

//! whether hyphenated groups have the shape of a date or of a list of years: four digits first with a one-digit group after them, two or more four-digit groups (a leading zero makes a group a code, not a year), or dd-mm-yyyy and yyyy-mm-dd
def private looks_dated(c : array<int>; spans : array<int2>) : bool {
    var four_digit_years = 0
    var one_digit_after_first = false
    for (k, span in count(), spans) {
        let width = span.y - span.x
        four_digit_years += (width == 4 && c[span.x] != '0') ? 1 : 0
        one_digit_after_first = one_digit_after_first || (k > 0 && width == 1)
    }
    let first = spans[0].y - spans[0].x
    let dmy_or_ymd = length(spans) == 3 && spans[1].y - spans[1].x == 2 && ((first <= 2 && spans[2].y - spans[2].x == 4) || (first == 4 && spans[2].y - spans[2].x == 2))
    return dmy_or_ymd || four_digit_years >= 2 || (first == 4 && one_digit_after_first)
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-phone")]
def private phone_at(c : array<int>; i : int; var out : array<int>) : int {
    var inscope g <- phone_groups(c, i)
    let groups = length(g.spans)
    return i if (groups == 0 || g.end <= i || is_word(cp_at(c, g.end)))
    var digits = 0
    for (span in g.spans) {
        digits += span.y - span.x
    }
    let book = latin_word_before(c, i) == "ISBN" || latin_word_before(c, i) == "ISSN"
    let coded = g.led_by_plus || g.has_area_code
    let chained = groups >= 3 && digits >= 7 && !looks_dated(c, g.spans)
    return i if (!(book && digits >= 8) && !(coded && groups >= 3 && digits >= 7) && !chained)
    if (g.led_by_plus) {
        emit(out, "плюс ")
    }
    for (k, span in count(), g.spans) {
        emit(out, "{k > 0 ? ", " : ""}{phone_group_words(c, span.x, span.y)}")
    }
    return g.end
}

def private pass_phones(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let opens = c[i] == '+' || c[i] == '(' || is_digit(c[i])
        if (opens && !is_word(cp_at(c, i - 1)) && cp_at(c, i - 1) != '-') {
            let end = phone_at(c, i, out)
            if (end > i) {
                i = end
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

let private ROMAN_VALUES <- [1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1]
let private ROMAN_SYMBOLS <- ["M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"]
let private MAX_CENTURY = 21

def private roman_digit(cp : int) : int =>
    cp == 'I' ? 1 : (cp == 'V' ? 5 : (cp == 'X' ? 10 : (cp == 'L' ? 50 : (cp == 'C' ? 100 : (cp == 'D' ? 500 : (cp == 'M' ? 1000 : 0))))))

def private roman_text(v : int) : string {
    var rest = v
    return build_string() $(var writer) {
        for (value, symbol in ROMAN_VALUES, ROMAN_SYMBOLS) {
            while (rest >= value) {
                writer |> write(symbol)
                rest -= value
            }
        }
    }
}

def private roman_end(c : array<int>; i : int) : int {
    return -1 if (roman_digit(cp_at(c, i)) == 0 || is_word(cp_at(c, i - 1)))
    var b = i
    while (roman_digit(cp_at(c, b)) != 0) {
        b++
    }
    return is_word(cp_at(c, b)) ? -1 : b
}

//! the value of the Roman numeral at [a, b) written the canonical way (XIV, not XIIII), or 0
def private canonical_roman_value(c : array<int>; a, b : int) : int {
    var total = 0
    for (k in range(a, b)) {
        let v = roman_digit(c[k])
        let next = k + 1 < b ? roman_digit(c[k + 1]) : 0
        total += v < next ? -v : v
    }
    return (total > 0 && total < 4000 && roman_text(total) == cpts_str(c, a, b)) ? total : 0
}

//! a canonical numeral spelled with I, V and X alone - one under forty: the spelling a regnal, section, suffixed or standalone numeral has
def private is_numbering_roman(c : array<int>; a, b : int) : bool {
    for (k in range(a, b)) {
        return false if (c[k] != 'I' && c[k] != 'V' && c[k] != 'X')
    }
    return true
}

let private FEMININE_REGNAL_NOMINATIVES <- { "екатерина", "елизавета", "анна", "мария", "изабелла", "виктория", "елена", "софия", "маргарита",
    "жанна", "ирина", "ольга", "тамара", "кристина", "луиза", "шарлотта", "беатрикс", "юлиана" }
let private SECTION_NOUNS <- { "том" => RuOrdinalForm.nom_m, "глава" => RuOrdinalForm.nom_f, "часть" => RuOrdinalForm.nom_f,
    "раздел" => RuOrdinalForm.nom_m, "книга" => RuOrdinalForm.nom_f, "серия" => RuOrdinalForm.nom_f, "выпуск" => RuOrdinalForm.nom_m,
    "сезон" => RuOrdinalForm.nom_m, "класс" => RuOrdinalForm.nom_m, "статья" => RuOrdinalForm.nom_f, "акт" => RuOrdinalForm.nom_m,
    "этап" => RuOrdinalForm.nom_m, "тур" => RuOrdinalForm.nom_m, "съезд" => RuOrdinalForm.nom_m }

def private is_feminine_regnal_stem(stem : string) : bool =>
    key_exists(FEMININE_REGNAL_NOMINATIVES, "{stem}а") || key_exists(FEMININE_REGNAL_NOMINATIVES, "{stem}я")

def private regnal_form(name : string) : RuOrdinalForm {
    return RuOrdinalForm.nom_f if (key_exists(FEMININE_REGNAL_NOMINATIVES, name))
    return RuOrdinalForm.obl_f if (ends_with_any(name, ["ы", "и"]))
    if (ends_with_any(name, ["ой", "ей"])) {
        return is_feminine_regnal_stem(chop_end(name, 2)) ? RuOrdinalForm.obl_f : RuOrdinalForm.nom_m
    }
    if (ends_with_any(name, ["у", "ю"])) {
        return is_feminine_regnal_stem(chop_end(name, 1)) ? RuOrdinalForm.acc_f : RuOrdinalForm.dat_m
    }
    if (name |> ends_with("е")) {
        return is_feminine_regnal_stem(chop_end(name, 1)) ? RuOrdinalForm.obl_f : RuOrdinalForm.prep_m
    }
    return RuOrdinalForm.instr_m if (ends_with_any(name, ["ом", "ем", "ём"]))
    return ends_with_any(name, ["а", "я"]) ? RuOrdinalForm.gen_m : RuOrdinalForm.nom_m
}

//! the name or section noun before `i` that numbers a Roman numeral: a section noun in any case of letter, else a capitalized word of two letters or more that is no preposition; empty where none
def private numbering_word_before(c : array<int>; i : int) : string {
    let span = word_span_before(c, i)
    let word = utf8_to_lower(cpts_str(c, span.x, span.y))
    return "" if (empty(word) || key_exists(CASE_GOVERNED_BY, word))
    return word if (key_exists(SECTION_NOUNS, word))
    return (span.y - span.x > 1 && cpt_is_cyrillic_upper(c[span.x])) ? word : ""
}

//! a century numeral at `s`: a canonical Roman one, or one or two Arabic digits; its value and its end, the value 0 where none stands
def private century_numeral_at(c : array<int>; s : int) : int2 {
    if (is_digit(cp_at(c, s))) {
        let e = digits_end(c, s)
        let plain = e - s <= 2 && c[s] != '0' && !is_word(cp_at(c, s - 1)) && !is_word(cp_at(c, e)) && !continues_number(c, s)
        return plain ? int2(int(digits_value(c, s, e)), e) : int2(0, s)
    }
    let b = roman_end(c, s)
    return b > s ? int2(canonical_roman_value(c, s, b), b) : int2(0, s)
}

//! the words between two centuries that need no dash before them (XVIII начале XIX века)
let private PERIOD_WORDS <- { "начале", "конце", "середине", "половине", "начала", "конца", "середины", "половины" }

//! the century noun after the next numeral across a word (XIX — начале XX века), which numbers the numeral ending at `b` too; `end` -1 where none stands
def private century_noun_across_word(c : array<int>; b : int; prep : string) : OrdinalNoun {
    let s = spaces_end(c, b)
    let dashed = is_dash(cp_at(c, s))
    let w = dashed ? spaces_end(c, s + 1) : s
    let we = letters_end(c, w)
    let next = century_numeral_at(c, spaces_end(c, we))
    let joined = dashed || (s > b && key_exists(PERIOD_WORDS, word_at(c, w)))
    return OrdinalNoun(end = -1) if (!joined || we == w || next.x == 0 || next.x > MAX_CENTURY)
    let noun = ordinal_noun_at(c, spaces_end(c, next.y), prep, false, true)
    return (noun.end >= 0 && empty(noun.words)) ? noun : OrdinalNoun(end = -1)
}

//! the century noun or mark numbering the numeral ending at `b`, the second numeral of a range before it (value 0 and end `b` where none), and what reads between the two; `noun.end` -1 where no noun stands
struct private CenturyPhrase {
    noun : OrdinalNoun
    second : int2
    joint : string
}

def private century_phrase(c : array<int>; i, b : int) : CenturyPhrase {
    let prep = word_before(c, i)
    let link = range_link(c, b)
    let found = (link.second_start >= 0 && (link.by_dash || link.word == "и")) ? century_numeral_at(c, link.second_start) : int2(0, b)
    let second = found.x > 0 ? found : int2(0, b)
    var noun = ordinal_noun_at(c, spaces_end(c, second.y), prep, second.x > 0, true)
    if (noun.end < 0 && second.x == 0) {
        noun = century_noun_across_word(c, b, prep)
    }
    return CenturyPhrase(noun = noun, second = second, joint = link.joint)
}

def private emit_century(v : int; p : CenturyPhrase; var out : array<int>) : int {
    emit(out, ordinal_words_ru(int64(v), p.noun.form))
    if (p.second.x > 0) {
        emit(out, "{p.joint}{ordinal_words_ru(int64(p.second.x), p.noun.form)}")
    }
    return p.second.y if (empty(p.noun.words))
    emit(out, " {p.noun.words}")
    return p.noun.end
}

//! whether an Arabic number counts the century noun after it rather than numbering it: a plural noun with no range (около 4 веков), or a genitive singular after two to four in the nominative or accusative (на 2 столетия) with no period word before it and no нашей эры after it
def private counts_centuries(c : array<int>; i, v : int; p : CenturyPhrase) : bool {
    return false if (!empty(p.noun.words) || p.second.x > 0)
    let noun = word_at(c, p.noun.end)
    return true if (ends_with_any(noun, ["ов", "ам", "ами", "ах"]))
    let prep = word_before(c, i)
    let cas = CASE_GOVERNED_BY?[prep] ?? RuCase.nom
    let tail = letters_end(c, p.noun.end)
    let after = word_after(c, tail)
    let era_word = after == "до" ? word_after(c, letters_end(c, spaces_end(c, tail))) : after
    let era = era_word == "нашей" || era_word == "н"
    return v >= 2 && v <= 4 && ends_with_any(noun, ["а", "я"]) && (cas == RuCase.nom || cas == RuCase.acc) && !key_exists(PERIOD_WORDS, prep) && !era
}

//! an Arabic number a century noun or a lowercase mark numbers reads as a Roman one does (в 19 веке; 5 В. is volts); past twenty-one, closing a range of other numbers, or counting the noun, it stays for the cardinal passes
def private arabic_century_at(c : array<int>; i : int; var out : array<int>) : int {
    return i if (!is_digit(c[i]) || range_first_half_start(c, i) >= 0)
    let first = century_numeral_at(c, i)
    return i if (first.x == 0)
    let century = century_phrase(c, i, first.y)
    let capital_mark = !empty(century.noun.words) && cpt_is_cyrillic_upper(cp_at(c, spaces_end(c, century.second.y)))
    let numbered = century.noun.end >= 0 && !capital_mark && first.x <= MAX_CENTURY && century.second.x <= MAX_CENTURY
    return (numbered && !counts_centuries(c, i, first.x, century)) ? emit_century(first.x, century, out) : i
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-roman")]
def private pass_roman(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let arabic = arabic_century_at(c, i, out)
        if (arabic > i) {
            i = arabic
            continue
        }
        let b = roman_end(c, i)
        let v = b > i ? canonical_roman_value(c, i, b) : 0
        if (v > 0) {
            let numbering = is_numbering_roman(c, i, b)
            let se = ordinal_suffix_end(c, b)
            if (se > 0 && numbering) {
                emit(out, ordinal_words_ru(int64(v), ordinal_suffix_form(c, i, b, se)))
                i = se
                continue
            }
            let century = century_phrase(c, i, b)
            if (century.noun.end >= 0 && (v > MAX_CENTURY || century.second.x > MAX_CENTURY)) {
                emit(out, cpts_str(c, i, b))
                i = b
                continue
            }
            if (century.noun.end >= 0) {
                i = emit_century(v, century, out)
                continue
            }
            let name = numbering ? numbering_word_before(c, i) : ""
            if (!empty(name)) {
                emit(out, ordinal_words_ru(int64(v), SECTION_NOUNS?[name] ?? regnal_form(name)))
                i = b
                continue
            }
            if (b - i >= 2 && numbering) {
                emit(out, number_to_words_ru(int64(v)))
                i = b
                continue
            }
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

def private digits_text(c : array<int>; a, b : int) : string {
    var inscope d : array<int>
    for (k in range(a, b)) {
        if (is_digit(c[k])) {
            d |> push(c[k])
        }
    }
    return cpts_to_utf8(d)
}

//! a number's extent: the integer digits with their group spaces, past the fraction after a comma or a dot, and where the next word starts
struct private NumberSpan {
    int_end : int
    end : int
    next : int
}

def private number_span(c : array<int>; i : int) : NumberSpan {
    let e = grouped_end(c, i)
    let fe = ((cp_at(c, e) == ',' || cp_at(c, e) == '.') && is_digit(cp_at(c, e + 1))) ? digits_end(c, e + 1) : e
    return NumberSpan(int_end = e, end = fe, next = spaces_end(c, fe))
}

//! a unit written after a number: its forms, the index past it and its dot, the word as written, and whether a Celsius sign came with it
struct private UnitAt {
    forms : UnitForms
    word : string
    end : int
    dotted : bool
    celsius : bool
}

//! a bare one-letter unit needs a boundary after it, and a bare с before a word is the preposition, not секунда
def private unit_at(c : array<int>; s : int) : UnitAt {
    var at = UnitAt(end = s)
    if (is_alpha(cp_at(c, s))) {
        var le = s
        while (is_alpha(cp_at(c, le))) {
            le++
        }
        at.word = cpts_str(c, s, le)
        at.forms = CURRENCY_CODES?[at.word] ?? UnitForms()
        at.end = le
        return at if (!is_word(cp_at(c, le)))
        return UnitAt(end = s)
    }
    return at if (!cpt_is_cyrillic(cp_at(c, s)))
    let ue = letters_end(c, s)
    at.word = cpts_str(c, s, ue)
    if (at.word == "кв" && cp_at(c, ue) == '.') {
        var square = unit_at(c, spaces_end(c, ue + 1))
        square.forms.adj = "квадратный"
        return (empty(square.forms.one) || square.end == spaces_end(c, ue + 1)) ? UnitAt(end = s) : square
    }
    let short = key_exists(ONE_LETTER_UNITS_RU, at.word)
    at.forms = UNITS_RU?[at.word] ?? (ONE_LETTER_UNITS_RU?[at.word] ?? UnitForms())
    var k = ue
    if (cp_at(c, k) == SUPERSCRIPT_TWO || cp_at(c, k) == SUPERSCRIPT_THREE) {
        at.forms.adj = cp_at(c, k) == SUPERSCRIPT_TWO ? "квадратный" : "кубический"
        k++
    } elif (cp_at(c, k) == '/' && cpt_is_cyrillic(cp_at(c, k + 1))) {
        let pe = letters_end(c, k + 1)
        let per = PER_TIME?[cpts_str(c, k + 1, pe)] ?? ""
        if (!empty(per)) {
            at.forms.tail = per
            k = pe
        }
    }
    at.dotted = cp_at(c, k) == '.'
    at.end = at.dotted ? k + 1 : k
    let bare = short && k == ue && !at.dotted
    if (empty(at.forms.one) || is_word(cp_at(c, at.end)) || (bare && is_letter(cp_at(c, k))) || (bare && at.word == "с" && cpt_is_cyrillic(cp_at(c, spaces_end(c, k))))) {
        return UnitAt(end = s)
    }
    return at
}

//! a unit word or currency code at `s` after a number ending at `e`; glued to the number only a unit word of two letters or more
def private unit_word_after(c : array<int>; e, s : int) : UnitAt {
    var at = unit_at(c, s)
    if (s == e && (key_exists(ONE_LETTER_UNITS_RU, at.word) || is_alpha(cp_at(c, s)))) {
        at = UnitAt(end = s)
    }
    return at
}

//! the unit after the number spanning `span`: a percent sign, a degree mark (℃, °C and °С with Celsius), a currency sign, or a unit word; `forms.one` empty where none follows
def private unit_or_sign_after(c : array<int>; span : NumberSpan) : UnitAt {
    let s = span.next
    let nx = cp_at(c, s)
    if (nx == DEGREE_SIGN && (cp_at(c, s + 1) == 'C' || cp_at(c, s + 1) == CYR_S_CAP) && !is_word(cp_at(c, s + 2))) {
        return UnitAt(forms = DEGREES, end = s + 2, celsius = true)
    }
    let sign = nx == '%' ? PERCENT : ((nx == DEGREE_SIGN || nx == DEGREE_CELSIUS_SIGN) ? DEGREES : (CURRENCY_SIGNS?[nx] ?? UnitForms()))
    if (!empty(sign.one)) {
        return is_word(cp_at(c, s + 1)) ? UnitAt(end = s) : UnitAt(forms = sign, end = s + 1, celsius = nx == DEGREE_CELSIUS_SIGN)
    }
    return unit_word_after(c, span.end, s)
}

def private is_scale(u : UnitForms) : bool {
    for (scale in SCALES) {
        return true if (scale.one == u.one)
    }
    return false
}

def private is_oblique(cas : RuCase) : bool => cas != RuCase.nom && cas != RuCase.acc

//! a whole and a half: полтора / полторы, полутора in an oblique case, the unit counted as by two; any other whole с половиной, the unit counted by the whole
def private with_half(whole : int64; u : UnitForms; cas : RuCase) : string {
    let gender = unit_gender(u)
    if (whole == 1l) {
        let half = is_oblique(cas) ? "полутора" : (gender == RuGender.feminine ? "полторы" : "полтора")
        return "{half} {unit_words(u, 2l, cas)}"
    }
    return "{cardinal_words_ru(whole, cas, gender)} с половиной {unit_words(u, whole, cas)}"
}

//! half a unit as one word: пол and the genitive singular (полчаса, пол-литра), полу and the singular of an oblique case (около получаса)
def private half_of(u : UnitForms; cas : RuCase) : string {
    return "полу{noun_for_count(u, 1l, cas)}{u.tail}" if (is_oblique(cas))
    var inscope initial <- utf8_to_cpts(u.few)
    let hyphen = find_index(POL_HYPHEN_INITIALS, initial[0]) >= 0 ? "-" : ""
    return "пол{hyphen}{u.few}{u.tail}"
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-count-agreement")]
def private part_of_unit(whole : int64; num, den : int; u : UnitForms; cas : RuCase) : string {
    let half = num == 1 && den == 2
    if (whole > 0l) {
        return (half && (whole == 1l || !counts_as_one(whole))) ? with_half(whole, u, cas) : ""
    }
    return "" if (is_scale(u))
    return (empty(u.adj) ? half_of(u, cas) : "") if (half)
    let part = den == 4 ? QUARTERS : (den == 3 ? THIRDS : UnitForms())
    return "" if (empty(part.one))
    let count = num == 1 ? noun_for_count(part, 1l, cas) : count_with_unit(int64(num), part, cas)
    return "{count} {unit_words(u, 1l, RuCase.gen)}"
}

//! the fraction's one or two digits count the minor unit, one digit as tens (,5 as пятьдесят копеек)
def private money_words(whole : int64; frac : string; u, minor : UnitForms; cas : RuCase) : string {
    let cents = to_int64(frac, false) * (length(frac) == 1 ? 10l : 1l)
    return count_with_unit(whole, u, cas) if (cents == 0l)
    let minor_words = count_with_unit(cents, minor, cas)
    return whole == 0l ? minor_words : "{count_with_unit(whole, u, cas)} {minor_words}"
}

[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-count-agreement")]
def private decimal_with_unit(int_part, frac : string; u : UnitForms; cas : RuCase) : string {
    let whole = to_int64(int_part, false)
    let minor = MINOR_UNITS?[u.one] ?? UnitForms()
    return money_words(whole, frac, u, minor, cas) if (!empty(minor.one) && length(frac) <= 2)
    let part = frac == "5" ? part_of_unit(whole, 1, 2, u, cas) : ""
    return empty(part) ? "{decimal_words_ru(int_part, frac, cas)} {unit_words(u, 1l, RuCase.gen)}" : part
}

//! whether `и <N> доп.` follows at `e`: the supplements after a count of volumes (82 т. и 4 доп.)
def private supplements_after(c : array<int>; e : int) : bool {
    let s = spaces_end(c, e)
    let d = spaces_end(c, letters_end(c, s))
    let w = spaces_end(c, digits_end(c, d))
    return word_at(c, s) == "и" && is_digit(cp_at(c, d)) && word_at(c, w) == "доп" && cp_at(c, letters_end(c, w)) == '.'
}

//! what a count's unit counts and the case it is read in
struct private Counted {
    forms : UnitForms
    cas : RuCase
}

//! a dotted one-letter unit in a citation counts what the citation means: с. after a dash is pages, т. after в (в 86 томах) or before supplements (82 тома и 4 дополнительных) volumes; anything else counts its own unit
def private counted_as(c : array<int>; i : int; at : UnitAt) : Counted {
    let cas = governed_unit_case(c, i, at.end)
    return Counted(forms = PAGES, cas = cas) if (at.dotted && at.word == "с" && prev_nonspace(c, i) == EM_DASH)
    if (at.dotted && at.word == "т") {
        let prep = governing_word(c, i)
        return Counted(forms = VOLUMES, cas = RuCase.prep) if (prep == "в" || prep == "во")
        return Counted(forms = VOLUMES, cas = cas) if (supplements_after(c, at.end))
    }
    return Counted(forms = at.forms, cas = cas)
}

//! the number at `i` spanning `span` with the unit `at` after it, as words in the case the words around it give; an integer with a leading zero or an overlong one reads digit by digit, its unit in the genitive plural; a leading-zero decimal after `digit.` is a dotted chain's tail (21.07.05 г.), no decimal; the index past the unit, or `i` where none follows
def private read_with_unit(c : array<int>; i : int; span : NumberSpan; at : UnitAt; var out : array<int>) : int {
    return i if (empty(at.forms.one))
    let int_part = digits_text(c, i, span.int_end)
    let decimal = span.end > span.int_end
    let overlong = length(int_part) > MAX_DIGITS_AS_NUMBER
    let leading_zero = length(int_part) > 1 && starts_with(int_part, "0")
    let chain_tail = cp_at(c, i - 1) == '.' && is_digit(cp_at(c, i - 2))
    return i if (decimal && (overlong || (leading_zero && chain_tail)))
    let celsius = at.celsius ? CELSIUS : ""
    if (overlong || (!decimal && leading_zero)) {
        emit(out, "{digits_to_words_ru(int_part)} {unit_words(at.forms, 0l, RuCase.gen)}{celsius}")
        return at.end
    }
    let cas = governed_unit_case(c, i, at.end)
    if (decimal) {
        emit(out, "{decimal_with_unit(int_part, cpts_str(c, span.int_end + 1, span.end), at.forms, cas)}{celsius}")
        return at.end
    }
    let counted = counted_as(c, i, at)
    emit(out, "{count_with_unit(to_int64(int_part, false), counted.forms, counted.cas)}{celsius}")
    return at.end
}

def private is_form_of(word : string; u : UnitForms) : bool => word == u.one || word == u.few || word == u.many

//! the scale word written out at `s` in any of its counted forms (миллиарда, тысяч), lowercase - a capital starts a sentence; empty forms where none stands
def private spelled_scale_at(c : array<int>; s : int) : UnitAt {
    return UnitAt(end = s) if (cpt_is_cyrillic_upper(cp_at(c, s)))
    let word = word_at(c, s)
    for (scale in SCALES) {
        return UnitAt(forms = scale, word = word, end = letters_end(c, s)) if (is_form_of(word, scale))
    }
    return UnitAt(end = s)
}

//! the scale word after a currency ending at `e`, abbreviated (млрд) or written out (миллиарда); empty forms where none follows
def private scale_after(c : array<int>; e : int) : UnitAt {
    let s = spaces_end(c, e)
    let at = unit_word_after(c, e, s)
    return at if (is_scale(at.forms))
    return s > e ? spelled_scale_at(c, s) : UnitAt(end = e)
}

//! an amount the scale word `scale` counts, the currency after it in the genitive plural (два миллиарда долларов), the currency written out again after the scale word gone; `i` where no scale word stands
def private scaled_amount(c : array<int>; i : int; span : NumberSpan; scale : UnitAt; currency : UnitForms; var out : array<int>) : int {
    return i if (empty(scale.forms.one))
    let end = read_with_unit(c, i, span, scale, out)
    return end if (end == i)
    emit(out, " {unit_words(currency, 0l, RuCase.gen)}")
    let s = spaces_end(c, end)
    return (s > end && is_form_of(word_at(c, s), currency)) ? letters_end(c, s) : end
}

//! the counted noun of time or measure the lowercase `word` spells as a count's genitive or a prepositional plural (года, лет, метров, километрах), a unit or a table noun; empty forms where it spells none
def private spelled_count_noun(word : string) : UnitForms {
    let unit = INVERTING_COUNT_NOUNS?[word] ?? UnitForms()
    let u = empty(unit.one) ? noun_of_form(word) : unit
    return u if (word == u.few || word == u.many || word == noun_case(u, RuCase.prep, true))
    return UnitForms()
}

//! a whole and a half - N,5 or N.5, N one or more - before a counted noun written out reads as before its unit (полтора года, два с половиной часа, в полутора километрах), the noun re-read in the number's case: a prepositional plural's own, else the case the words before give, a placing distance only for a unit; `i` where none follows
def private half_before_noun(c : array<int>; i : int; span : NumberSpan; var out : array<int>) : int {
    let we = letters_end(c, span.next)
    let word = word_at(c, span.next)
    let u = spelled_count_noun(word)
    let whole = to_int64(digits_text(c, i, span.int_end), false)
    return i if (span.end != span.int_end + 2 || cp_at(c, span.end - 1) != '5' || whole < 1l || span.next == span.end || empty(u.one) || is_word(cp_at(c, we)))
    let unit = !empty((INVERTING_COUNT_NOUNS?[word] ?? UnitForms()).one) && !key_exists(INVERTING_TABLE_NOUNS, u.one)
    let governed = unit ? governed_unit_case(c, i, we) : (CASE_GOVERNED_BY?[governing_word(c, i)] ?? RuCase.nom)
    let words = part_of_unit(whole, 1, 2, u, word == noun_case(u, RuCase.prep, true) ? RuCase.prep : governed)
    return i if (empty(words))
    emit(out, words)
    return we
}

//! a currency sign or code after the amount with a scale word after it (2,2$ млрд) reads as when the sign leads
def private number_with_unit(c : array<int>; i : int; var out : array<int>) : int {
    let span = number_span(c, i)
    let half = half_before_noun(c, i, span, out)
    return half if (half > i)
    let at = unit_or_sign_after(c, span)
    if (key_exists(MINOR_UNITS, at.forms.one)) {
        let end = scaled_amount(c, i, span, scale_after(c, at.end), at.forms, out)
        return end if (end > i)
    }
    return read_with_unit(c, i, span, at, out)
}

//! a dash after a digit is a range and one after a letter joins words, a Latin capital across a space included (лист карты O -37, F -15); any other `-` before a number reads минус
def private is_minus_at(c : array<int>; i : int) : bool {
    let before = prev_nonspace(c, i)
    return c[i] == '-' && !is_word(cp_at(c, i - 1)) && !is_digit(before) && !(before >= 'A' && before <= 'Z')
}

//! a hyphen after a Latin capital and a space, touching a number, joins a designation and is read as nothing (PZL -130 as PZL сто тридцать)
def private is_designation_hyphen(c : array<int>; i : int) : bool {
    let before = prev_nonspace(c, i)
    return c[i] == '-' && is_space(cp_at(c, i - 1)) && is_digit(cp_at(c, i + 1)) && before >= 'A' && before <= 'Z'
}

let private VULGAR_FRACTIONS <- { 189 => int2(1, 2), 188 => int2(1, 4), 190 => int2(3, 4), 8531 => int2(1, 3), 8532 => int2(2, 3), 8539 => int2(1, 8) }

//! a fraction's numerator and denominator and the index past it; `num` 0 where none stands
struct private SimpleFraction {
    num : int
    den : int
    end : int
}

def private simple_fraction(c : array<int>; p : int) : SimpleFraction {
    let sign = VULGAR_FRACTIONS?[cp_at(c, p)] ?? int2(0, 0)
    return SimpleFraction(num = sign.x, den = sign.y, end = p + 1) if (sign.x > 0)
    let e = digits_end(c, p)
    let me = digits_end(c, e + 1)
    return SimpleFraction(end = p) if (e == p || e - p > 2 || cp_at(c, e) != '/' || me == e + 1 || me - e - 1 > 3 || cp_at(c, p - 1) == '/' || cp_at(c, me) == '/' || is_word(cp_at(c, me)))
    let num = int(digits_value(c, p, e))
    let den = int(digits_value(c, e + 1, me))
    return (num >= 1 && num < den && den <= 100) ? SimpleFraction(num = num, den = den, end = me) : SimpleFraction(end = p)
}

//! the words a fraction before them keeps as a fraction: a part of a whole (одна третья часть, доля) and, by its start, a round of a cup (одна четвёртая финала)
let private FRACTION_KEEPING_WORDS <- { "часть", "части", "частью", "частей", "доля", "доли", "долю", "долей" }

//! a lowercase Cyrillic word of three letters or more one space after a fraction ending at `e`, as written, no abbreviation's dot after it; empty where none stands
def private written_word_after(c : array<int>; e : int) : string {
    let we = letters_end(c, e + 1)
    let dotted = cp_at(c, we) == '.' && !sentence_ends_after(c, we + 1)
    let lowercase = !cpt_is_cyrillic_upper(cp_at(c, e + 1))
    return "" if (cp_at(c, e) != ' ' || we - e - 1 < 3 || !lowercase || dotted || is_word(cp_at(c, we)) || cp_at(c, we) == '-')
    let word = cpts_str(c, e + 1, we)
    let lower = utf8_to_lower(word)
    return (key_exists(FRACTION_KEEPING_WORDS, lower) || lower |> starts_with("финал")) ? "" : word
}

//! пол joins a written word only in the nominative or accusative, and only a genitive singular: a word in -а, -я, -и or -ы that is no adjective (-ая, -ыми), no reflexive verb (-ся) and no plural past-tense verb (-али, -или)
def private joins_pol(word : string; cas : RuCase) : bool {
    return false if (is_oblique(cas) || ends_with_any(word, ["ая", "яя", "ими", "ыми", "ся", "али", "яли", "или", "ыли", "ели", "ули"]))
    return ends_with_any(word, ["а", "я", "и", "ы"])
}

//! whether a fraction and a comma stand right before the fraction at `i`: a list of fractions (поезда 1/2, 3/4)
def private closes_fraction_list(c : array<int>; i : int) : bool {
    let j = spaces_start(c, i)
    return false if (j == i || cp_at(c, j - 1) != ',')
    return true if (key_exists(VULGAR_FRACTIONS, cp_at(c, j - 2)))
    var d = j - 1
    while (is_digit(cp_at(c, d - 1))) {
        d--
    }
    return d < j - 1 && cp_at(c, d - 1) == '/' && is_digit(cp_at(c, d - 2))
}

//! a half, a quarter or a third before a word written out reads as a word too (полстакана, четверть века, две трети голосов), except in a list of fractions; `i` where it does not
def private part_of_written_word(c : array<int>; i : int; f : SimpleFraction; var out : array<int>) : int {
    let word = written_word_after(c, f.end)
    let cas = governed_case(c, i, f.end)
    return i if (empty(word) || (f.den == 2 && !joins_pol(word, cas)) || closes_fraction_list(c, i))
    let part = part_of_unit(0l, f.num, f.den, UnitForms(one = word, few = word, many = word), cas)
    return i if (empty(part))
    emit(out, part)
    return letters_end(c, f.end + 1)
}

def private fraction_at(c : array<int>; i : int; var out : array<int>) : int {
    var whole = 0l
    var f = simple_fraction(c, i)
    if (f.num == 0 && is_digit(c[i])) {
        let e = digits_end(c, i)
        let p = cp_at(c, e) == ' ' ? e + 1 : e
        f = e - i <= 3 ? simple_fraction(c, p) : SimpleFraction(end = i)
        whole = digits_value(c, i, e)
    }
    return i if (f.num == 0)
    let s = spaces_end(c, f.end)
    let at = unit_word_after(c, f.end, s)
    if (whole == 0l && empty(at.forms.one)) {
        let past = part_of_written_word(c, i, f, out)
        return past if (past > i)
    }
    let end = empty(at.forms.one) ? f.end : at.end
    let cas = governed_case(c, i, end)
    let part = empty(at.forms.one) ? "" : part_of_unit(whole, f.num, f.den, at.forms, cas)
    if (!empty(part)) {
        emit(out, part)
        return end
    }
    emit(out, fraction_words_ru(whole, int64(f.num), int64(f.den), cas))
    if (!empty(at.forms.one)) {
        emit(out, " {unit_words(at.forms, 1l, RuCase.gen)}")
    }
    return end
}

def private pass_fractions(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let neg = is_minus_at(c, i) && (is_digit(cp_at(c, i + 1)) || key_exists(VULGAR_FRACTIONS, cp_at(c, i + 1)))
        let s = neg ? i + 1 : i
        if ((is_digit(c[s]) && !is_word(cp_at(c, s - 1))) || key_exists(VULGAR_FRACTIONS, c[s])) {
            var inscope phrase : array<int>
            let end = fraction_at(c, s, phrase)
            if (end > s) {
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

//! a currency sign before a number: the amount counts the currency (пятнадцать долларов), or counts a scale word the currency follows (один миллиард долларов); a currency code after the amount repeats the sign and goes
def private currency_first(c : array<int>; i : int; currency : UnitForms; var out : array<int>) : int {
    let span = number_span(c, i + 1)
    return i if (is_word(cp_at(c, span.end)) || length(digits_text(c, i + 1, span.int_end)) > MAX_DIGITS_AS_NUMBER)
    if (span.next > span.end) {
        let end = scaled_amount(c, i + 1, span, scale_after(c, span.end), currency, out)
        return end if (end > i + 1)
    }
    let after = unit_or_sign_after(c, span)
    let end = read_with_unit(c, i + 1, span, UnitAt(forms = currency, end = span.end), out)
    return (span.next > span.end && key_exists(CURRENCY_CODES, after.word)) ? after.end : end
}

//! a unit's dot that ends the sentence stays the sentence's
def private pass_units(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let currency = CURRENCY_SIGNS?[c[i]] ?? UnitForms()
        let neg = is_minus_at(c, i) && is_digit(cp_at(c, i + 1))
        let start = neg ? i + 1 : i
        var end = -1
        var inscope phrase : array<int>
        if (!empty(currency.one) && is_digit(cp_at(c, i + 1))) {
            end = currency_first(c, i, currency, phrase)
        } elif (is_digit(cp_at(c, start)) && !is_word(cp_at(c, i - 1))) {
            end = number_with_unit(c, start, phrase)
        }
        if (end > start) {
            if (neg) {
                emit(out, "минус ")
            }
            out |> push_from(phrase)
            if (c[end - 1] == '.' && sentence_ends_after(c, end)) {
                out |> push('.')
            }
            i = end
            continue
        }
        out |> push(c[i])
        i++
    }
    return <- out
}

def private reads_with_number(key : string) : bool => key_exists(UNITS_RU, key) || key_exists(ONE_LETTER_UNITS_RU, key) || key == "гг"

//! кв. at [i, dot) between a number and a unit is the unit's квадратный (26 кв. км), no квартира
def private is_square_mark(c : array<int>; i, dot : int) : bool => run_is(c, i, dot, "кв") && !empty(unit_at(c, i).forms.one)

//! т. before a number and after no number is the volume it names (т. 1 as том один)
def private is_volume_before_number(c : array<int>; i : int) : bool =>
    run_is(c, i, i + 1, "т") && cp_at(c, i + 1) == '.' && is_digit(cp_at(c, spaces_end(c, i + 2))) && !is_digit(prev_nonspace(c, i))

//! longest match first, the dot consumed; a unit or гг. right after a number is left to the number passes
def private pass_abbreviations(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (cpt_is_cyrillic(c[i]) && !is_word(cp_at(c, i - 1)) && is_volume_before_number(c, i)) {
            emit(out, "том")
            i += 2
            continue
        }
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
            let before = prev_nonspace(c, i)
            let with_number = reads_with_number(utf8_to_lower(cpts_str(c, i, best))) || is_square_mark(c, i, best)
            if (best > i && (is_digit(before) || key_exists(VULGAR_FRACTIONS, before)) && with_number) {
                best = -1
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

//! the gender a singular adjective's ending tells after one in `cas` (одна большая, одной большой); masculine where it tells none
def private adjective_gender(adjective : string; cas : RuCase) : RuGender {
    if (cas == RuCase.nom || cas == RuCase.acc) {
        return RuGender.feminine if (ends_with_any(adjective, ["ая", "яя", "ую", "юю"]))
        return ends_with_any(adjective, ["ое", "ее"]) ? RuGender.neuter : RuGender.masculine
    }
    return ends_with_any(adjective, ["ой", "ей"]) ? RuGender.feminine : RuGender.masculine
}

//! the noun read across its adjectives, after из the genitive plural the number is one of (одна из машин); the dictionary's gender, else masculine wherever the ending cannot tell it; the prepositional asks `is_feminine_prepositional`
def private noun_gender_after(c : array<int>; e : int; v : int64; cas : RuCase) : RuGender {
    let last = int(abs(v) % 10l)
    return RuGender.masculine if (last != 1 && last != 2)
    let of = partitive_end(c, e)
    if (of >= 0) {
        let known = dictionary_form(partitive_whole(c, of).noun)
        return spells(known, GENITIVE_PLURAL) ? known.gender : RuGender.masculine
    }
    let phrase = noun_phrase_after(c, e)
    let word = phrase.noun
    let plural_adjective = last == 1 && ends_with_any(phrase.adjective, ["ые", "ие", "ых", "их", "ым", "им", "ыми", "ими"])
    return RuGender.masculine if (utf8_length(word) < 3 || plural_adjective)
    let told = last == 1 ? adjective_gender(phrase.adjective, cas) : RuGender.masculine
    return told if (told != RuGender.masculine)
    let known = dictionary_form(word)
    let counted = last == 2 ? (spells(known, int(RuCase.gen)) || spells(known, GENITIVE_PLURAL)) : (spells(known, int(cas)) || (cas == RuCase.acc && spells(known, int(RuCase.nom))))
    return known.gender if (counted)
    var feminine = false
    if (last == 2) {
        let verb_like = ends_with_any(word, ["ми", "ли"]) && !empty(phrase.adjective)
        feminine = (cas == RuCase.nom || cas == RuCase.acc) && ends_with_any(word, ["ы", "и"]) && !verb_like
    } elif (cas == RuCase.nom || cas == RuCase.acc) {
        return RuGender.neuter if (ends_with_any(word, ["мя", "о", "е", "ё"]))
        feminine = cas == RuCase.nom ? ends_with_any(word, ["а", "я"]) : ends_with_any(word, ["у", "ю"])
    } elif (cas == RuCase.gen) {
        feminine = ends_with_any(word, ["ы", "и"])
    } elif (cas == RuCase.dat) {
        feminine = ends_with_any(word, ["е", "и"])
    } elif (cas == RuCase.instr) {
        feminine = ends_with_any(word, ["ой", "ей", "ью"])
    } elif (cas == RuCase.prep) {
        feminine = is_feminine_prepositional(word)
    }
    return feminine ? RuGender.feminine : RuGender.masculine
}

//! a decimal with a leading zero after a number and a dot is a code's tail (специальность 07.00.02), read digit by digit
[arch(at = "../ARCHITECTURE_TTS_TEXTNORM_RU.md#tts-ru-phone")]
def private is_code_tail(c : array<int>; s : int; int_part, frac : string) : bool {
    let d = spaces_start(c, s)
    return !empty(frac) && length(int_part) == 2 && starts_with(int_part, "0") && cp_at(c, d - 1) == '.' && is_digit(cp_at(c, d - 2))
}

//! the quantity words that count a bare thousand or million after them (несколько тысяч, сотни миллионов)
let private SCALE_COUNTING_WORDS <- { "несколько", "сотни", "сотен", "сотнями", "сотню", "десятки", "десятков", "десятками", "пару" }

//! a bare power of a thousand at `s` that a quantity word before it counts, as its scale word's genitive plural (сотен тысяч); empty where it is none
def private scale_counted_before(c : array<int>; s : int; v : int64) : string {
    return "" if (!key_exists(SCALE_COUNTING_WORDS, word_before(c, s)))
    var scale = 1000000000000000000l
    for (u in SCALES) {
        return u.many if (v == scale)
        scale /= 1000l
    }
    return ""
}

//! a leading zero (0425) marks a code, not a quantity: it reads digit by digit
def private pass_numbers(c : array<int>) : array<int> {
    var out : array<int>
    let n = length(c)
    var i = 0
    while (i < n) {
        let neg = is_minus_at(c, i) && is_digit(cp_at(c, i + 1))
        let s = neg ? i + 1 : i
        if (is_digit(cp_at(c, s)) && !is_word(cp_at(c, i - 1))) {
            let span = number_span(c, s)
            let int_part = digits_text(c, s, span.int_end)
            let frac = cpts_str(c, min(span.int_end + 1, span.end), span.end)
            if (length(int_part) > MAX_DIGITS_AS_NUMBER || (length(int_part) > 1 && starts_with(int_part, "0") && empty(frac))) {
                emit(out, "{neg ? "минус " : ""}{digits_to_words_ru(int_part)}")
            } elif (is_code_tail(c, s, int_part, frac)) {
                emit(out, "{digits_to_words_ru(int_part)} {digits_to_words_ru(frac)}")
            } else {
                let cas = governed_case(c, s, span.end)
                if (!empty(frac)) {
                    emit(out, "{neg ? "минус " : ""}{decimal_words_ru(int_part, frac, cas)}")
                } else {
                    let v = to_int64(int_part, false) * (neg ? -1l : 1l)
                    let scale = scale_counted_before(c, s, v)
                    let counted = counts_as_one(v) ? range_second_half_end(c, span.end) : counted_span_end(c, span.end)
                    let inverted = inverted_count_before(c, s, span.end)
                    let gender = empty(inverted.forms.one) ? noun_gender_after(c, counted, v, cas) : unit_gender(inverted.forms)
                    emit(out, !empty(scale) ? scale : cardinal_words_ru(v, cas, gender))
                }
            }
            i = span.end
            continue
        }
        if (!is_designation_hyphen(c, i)) {
            out |> push(c[i])
        }
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
    let passes <- [@@pass_abbreviations, @@pass_phones, @@pass_roman, @@pass_dates, @@pass_time, @@pass_ordinals, @@pass_fractions, @@pass_units,
        @@pass_symbols, @@pass_numbers, @@pass_punctuation]
    for (step in passes) {
        var inscope next <- invoke(step, c)
        delete c
        c <- next
    }
    return cpts_str(c, 0, length(c))
}
