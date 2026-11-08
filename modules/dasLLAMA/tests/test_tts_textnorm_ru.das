options gen2
options _dasllama_internal = true
options _ascii_strings = false   // the expected Russian words are the claims
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_textnorm_ru

// Russian text normalization: the cardinal, ordinal and decimal readers with their agreement
// (один рубль, два рубля, пять рублей; две тысячи), the ordinal forms a suffix or a date asks
// for, and the sentence passes - years after в and before года, dates, clock times, percentages,
// units, money, abbreviations, symbols - on pinned sentences, idempotence included. Model-free.

[test]
def test_number_words_ru(t : T?) {
    t |> run("cardinals, the scale words agreeing with their count") @(t : T?) {
        t |> equal(number_to_words_ru(0l), "ноль")
        t |> equal(number_to_words_ru(1l), "один")
        t |> equal(number_to_words_ru(1l, RuGender.feminine), "одна")
        t |> equal(number_to_words_ru(2l, RuGender.feminine), "две")
        t |> equal(number_to_words_ru(1l, RuGender.neuter), "одно")
        t |> equal(number_to_words_ru(12l), "двенадцать")
        t |> equal(number_to_words_ru(21l), "двадцать один")
        t |> equal(number_to_words_ru(100l), "сто")
        t |> equal(number_to_words_ru(345l), "триста сорок пять")
        t |> equal(number_to_words_ru(1000l), "одна тысяча")
        t |> equal(number_to_words_ru(2000l), "две тысячи")
        t |> equal(number_to_words_ru(5000l), "пять тысяч")
        t |> equal(number_to_words_ru(21000l), "двадцать одна тысяча")
        t |> equal(number_to_words_ru(2500l), "две тысячи пятьсот")
        t |> equal(number_to_words_ru(1000000l), "один миллион")
        t |> equal(number_to_words_ru(3000000000l), "три миллиарда")
        t |> equal(number_to_words_ru(-7l), "минус семь")
    }
    t |> run("the grammatical number a count takes") @(t : T?) {
        for (n in [1l, 21l, 101l]) {
            t |> equal(plural_ru(n, "рубль", "рубля", "рублей"), "рубль", "{n}")
        }
        for (n in [2l, 3l, 4l, 22l, 104l]) {
            t |> equal(plural_ru(n, "рубль", "рубля", "рублей"), "рубля", "{n}")
        }
        for (n in [0l, 5l, 11l, 12l, 14l, 25l, 100l, 111l]) {
            t |> equal(plural_ru(n, "рубль", "рубля", "рублей"), "рублей", "{n}")
        }
    }
    t |> run("ordinals: the last word alone takes the ending, in the form asked for") @(t : T?) {
        t |> equal(ordinal_words_ru(1l), "первый")
        t |> equal(ordinal_words_ru(3l), "третий")
        t |> equal(ordinal_words_ru(3l, RuOrdinalForm.gen_m), "третьего")
        t |> equal(ordinal_words_ru(3l, RuOrdinalForm.nom_f), "третья")
        t |> equal(ordinal_words_ru(8l), "восьмой")
        t |> equal(ordinal_words_ru(8l, RuOrdinalForm.prep_m), "восьмом")
        t |> equal(ordinal_words_ru(21l), "двадцать первый")
        t |> equal(ordinal_words_ru(40l), "сороковой")
        t |> equal(ordinal_words_ru(100l), "сотый")
        t |> equal(ordinal_words_ru(300l), "трёхсотый")
        t |> equal(ordinal_words_ru(1000l), "тысячный")
        t |> equal(ordinal_words_ru(2000l), "двухтысячный")
        t |> equal(ordinal_words_ru(2024l, RuOrdinalForm.prep_m), "две тысячи двадцать четвёртом")
        t |> equal(ordinal_words_ru(1998l, RuOrdinalForm.gen_m), "одна тысяча девятьсот девяносто восьмого")
        t |> equal(ordinal_words_ru(5l, RuOrdinalForm.gen_pl), "пятых")
    }
    t |> run("decimals: целых and the denominator by the fraction's width") @(t : T?) {
        t |> equal(decimal_words_ru("3", "14"), "три целых четырнадцать сотых")
        t |> equal(decimal_words_ru("1", "5"), "одна целая пять десятых")
        t |> equal(decimal_words_ru("2", "1"), "две целых одна десятая")
        t |> equal(decimal_words_ru("0", "75"), "ноль целых семьдесят пять сотых")
        t |> equal(digits_to_words_ru("007"), "ноль ноль семь")
    }
}

[test]
def test_normalize_ru(t : T?) {
    t |> run("years, dates and clock times") @(t : T?) {
        t |> equal(normalize_text_ru("В 2024 году вышло 12 игр."), "В две тысячи двадцать четвёртом году вышло двенадцать игр.")
        t |> equal(normalize_text_ru("25 декабря 2023 года"), "двадцать пятого декабря две тысячи двадцать третьего года")
        t |> equal(normalize_text_ru("1 мая"), "первого мая")
        t |> equal(normalize_text_ru("25\xC2\xA0декабря"), "двадцать пятого декабря", "a no-break space before the month is a space")
        t |> equal(normalize_text_ru("09.05.1945"), "девятого мая одна тысяча девятьсот сорок пятого года")
        t |> equal(normalize_text_ru("в 1998-м году"), "в одна тысяча девятьсот девяносто восьмом году")
        t |> equal(normalize_text_ru("Встреча в 14:05."), "Встреча в четырнадцать ноль пять.")
        t |> equal(normalize_text_ru("Поезд 06:30"), "Поезд шесть тридцать")
        t |> equal(normalize_text_ru("в 12:30:15"), "в двенадцать тридцать пятнадцать секунд")
        t |> equal(normalize_text_ru("в 12:30:99"), "в двенадцать тридцать:девяносто девять", "a second past 59 is not a second")
    }
    t |> run("ordinal suffixes") @(t : T?) {
        t |> equal(normalize_text_ru("на 5-м этаже, 3-я дверь"), "на пятом этаже, третья дверь")
        t |> equal(normalize_text_ru("2-го числа"), "второго числа")
        t |> equal(normalize_text_ru("в 20-х годах"), "в двадцатых годах")
    }
    t |> run("units, percentages, money and their agreement") @(t : T?) {
        t |> equal(normalize_text_ru("21 рубль, 22 рубля, 25 рублей"), "двадцать один рубль, двадцать два рубля, двадцать пять рублей")
        t |> equal(normalize_text_ru("Это стоит 1500 руб."), "Это стоит одна тысяча пятьсот рублей.")
        t |> equal(normalize_text_ru("около 15 $"), "около пятнадцать долларов")
        t |> equal(normalize_text_ru("Температура 25°C, влажность 80%."), "Температура двадцать пять градусов цельсия, влажность восемьдесят процентов.")
        t |> equal(normalize_text_ru("25℃ и 3,5℃, но 7°"), "двадцать пять градусов цельсия и три целых пять десятых градуса цельсия, но семь градусов", "the one-symbol Celsius sign reads as the two-character one")
        t |> equal(normalize_text_ru("Цена выросла на 3,5%."), "Цена выросла на три целых пять десятых процента.")
        t |> equal(normalize_text_ru("Он пробежал 10 км за 45 мин."), "Он пробежал десять километров за сорок пять минут.")
        t |> equal(normalize_text_ru("вес 2 кг, 1 минута, 2 минуты"), "вес два килограмма, одна минута, две минуты", "a spelled-out noun is not a unit; one and two still agree with it")
        t |> equal(normalize_text_ru("1 стол, 2 стола, 1 окно, 2 окна, 1 имя, 21 неделя, 22 недели"), "один стол, два стола, одно окно, два окна, одно имя, двадцать одна неделя, двадцать две недели")
        t |> equal(normalize_text_ru("11 минут, 12 минут, 1 день, 1 раз, 2 раза, 2"), "одиннадцать минут, двенадцать минут, один день, один раз, два раза, два", "teens, an ending that says nothing and a bare count stay masculine")
        t |> equal(normalize_text_ru("1 000 000 человек"), "один миллион человек")
        t |> equal(normalize_text_ru("1234567890123456789012 руб."), "один два три четыре пять шесть семь восемь девять ноль один два три четыре пять шесть семь восемь девять ноль один два рублей.", "a run past eighteen digits reads digit by digit before its unit")
        t |> equal(normalize_text_ru("$1234567890123456789012"), "один два три четыре пять шесть семь восемь девять ноль один два три четыре пять шесть семь восемь девять ноль один два", "and after a currency sign, which goes")
    }
    t |> run("abbreviations and symbols") @(t : T?) {
        t |> equal(normalize_text_ru("Дом № 7, кв. 12"), "Дом номер семь, квартира двенадцать")
        t |> equal(normalize_text_ru("книги, игры и т.д."), "книги, игры и так далее.", "an abbreviation's dot at the end of the text is the sentence's too")
        t |> equal(normalize_text_ru("книги, игры и т.д. и книги"), "книги, игры и так далее и книги")
        t |> equal(normalize_text_ru("т.е. завтра. Потом"), "то есть завтра. Потом")
        t |> equal(normalize_text_ru("1500 млн руб."), "одна тысяча пятьсот миллионов рублей.")
        t |> equal(normalize_text_ru("минус 7 градусов, -5"), "минус семь градусов, минус пять")
        t |> equal(normalize_text_ru("ночью -5°C, днём -3,5%"), "ночью минус пять градусов цельсия, днём минус три целых пять десятых процента", "the minus before a unit survives")
    }
    t |> run("punctuation survives, the rest goes; a normalized sentence is a fixed point") @(t : T?) {
        let once = normalize_text_ru("«Привет», — сказал он (тихо)… Да!")
        t |> equal(once, "Привет , — сказал он тихо … Да!")
        t |> equal(normalize_text_ru(once), once)
        let plain = "Двенадцать мешков муки нужно, чтобы открыть замок в замок."
        t |> equal(normalize_text_ru(plain), plain)
    }
}
