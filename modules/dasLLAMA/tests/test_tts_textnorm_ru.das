options gen2
options _dasllama_internal = true
options _ascii_strings = false   // the expected Russian words are the claims
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_textnorm_ru

// Russian text normalization: the cardinal, ordinal, decimal and fraction readers with their
// agreement (один рубль, два рубля, пять рублей; тысяча), the case a preposition or a noun ending
// fixes for a cardinal, the ordinal forms a suffix, a date or a range asks for, and the sentence
// passes - years, dates, Roman numerals, clock times, percentages, units, money, telephone and
// ISBN groups, abbreviations, symbols - on pinned sentences, idempotence included. Model-free.

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
        t |> equal(number_to_words_ru(1000l), "тысяча")
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
        t |> equal(ordinal_words_ru(1998l, RuOrdinalForm.gen_m), "тысяча девятьсот девяносто восьмого")
        t |> equal(ordinal_words_ru(5l, RuOrdinalForm.gen_pl), "пятых")
    }
    t |> run("decimals: целых and the denominator by the fraction's width") @(t : T?) {
        t |> equal(decimal_words_ru("3", "14"), "три целых четырнадцать сотых")
        t |> equal(decimal_words_ru("1", "5"), "одна целая пять десятых")
        t |> equal(decimal_words_ru("2", "1"), "две целых одна десятая")
        t |> equal(decimal_words_ru("0", "75"), "ноль целых семьдесят пять сотых")
        t |> equal(decimal_words_ru("3", "1415926"), "три запятая один четыре один пять девять два шесть")
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
        t |> equal(normalize_text_ru("09.05.1945"), "девятого мая тысяча девятьсот сорок пятого года")
        t |> equal(normalize_text_ru("в 1998-м году"), "в тысяча девятьсот девяносто восьмом году")
        t |> equal(normalize_text_ru("Встреча в 14:05."), "Встреча в четырнадцать часов пять минут.")
        t |> equal(normalize_text_ru("Поезд 06:30"), "Поезд шесть часов тридцать минут")
        t |> equal(normalize_text_ru("в 12:30:15"), "в двенадцать часов тридцать минут пятнадцать секунд")
        t |> equal(normalize_text_ru("в 12:30:99"), "в двенадцать часов тридцать минут:девяносто девять", "a second past 59 is not a second")
    }
    t |> run("ordinal suffixes") @(t : T?) {
        t |> equal(normalize_text_ru("на 5-м этаже, 3-я дверь"), "на пятом этаже, третья дверь")
        t |> equal(normalize_text_ru("2-го числа"), "второго числа")
        t |> equal(normalize_text_ru("в 20-х годах"), "в двадцатых годах")
    }
    t |> run("a bare number after в, во or на numbering the noun after it reads as its ordinal") @(t : T?) {
        t |> equal(normalize_text_ru("на 81 году жизни, на 5 курсе, в 3 классе, на 2 этаже, в 1 туре, во 2 тайме"), "на восемьдесят первом году жизни, на пятом курсе, в третьем классе, на втором этаже, в первом туре, во втором тайме")
        t |> equal(normalize_text_ru("на 10 минуте, в 5 главе, на 3 месте, в 42 серии, в 2 части, в 5 часу"), "на десятой минуте, в пятой главе, на третьем месте, в сорок второй серии, в второй части, в пятом часу", "a feminine noun takes the feminine; the preposition stays as written")
        t |> equal(normalize_text_ru("в 5 классах, на 2 части, на 20 километре"), "в пяти классах, на две части, на двадцатом километре", "a plural noun, or на before a count the noun could be, stays a cardinal")
        t |> equal(normalize_text_ru("в 1 руке, на 1 стороне"), "в одной руке, на одной стороне", "a counted noun gives its gender, and after one stays a count")
    }
    t |> run("a bare number before a singular noun the count cannot govern reads as its ordinal in the noun's form") @(t : T?) {
        t |> equal(normalize_text_ru("уже 3 день, на 3 день, до 6 курса, во 2 раз, в 3 раз, в 5 раз, 3 раза"), "уже третий день, на третий день, до шестого курса, во второй раз, в третий раз, в пять раз, три раза")
        t |> equal(normalize_text_ru("5 дня пути, симметрией 5 порядка, со 2 этажа, 3 этажа"), "пятого дня пути, симметрией пятого порядка, со второго этажа, три этажа", "two to four before a genitive singular with no preposition stays a count")
        t |> equal(normalize_text_ru("победитель 2 отборочного поединка, во время 2 мировой войны, в 3 британский отдел"), "победитель второго отборочного поединка, во время второй мировой войны, в третий британский отдел", "a singular adjective spells the ordinal's form")
        t |> equal(normalize_text_ru("в 5 дня, с 2 дня, лет 15 тому назад, в 4 новая серия, 5 шампанского, раза в 3 медленнее"), "в пять дня, с двух дня, лет пятнадцать тому назад, в четыре новая серия, пять шампанского, раза в три медленнее", "a clock time, a count before its noun, a form the preposition cannot take and a word no noun follows stay counts")
        t |> equal(normalize_text_ru("во-1, во-2, в-3, Во 1, да, Во-7-де-сят"), "во-первых, во-вторых, в-третьих, Во первых, да, Во-семь-де-сят", "во and в before a digit enumerate")
        t |> equal(normalize_text_ru("на 6 войну, уже 2 дело, к 5 статье, из золота 999 пробы"), "на шестую войну, уже второе дело, к пятой статье, из золота девятьсот девяносто девятой пробы", "a dictionary noun's singular form the count cannot govern reads the ordinal too")
        t |> equal(normalize_text_ru("в 5 утра, эти 2 стража, А 2 башня"), "в пять утра, эти два стража, А два башня", "a clock time, a plural determiner and a letter's label stay counts")
        t |> equal(normalize_text_ru("капитана 1 ранга, советника 1 класса, в убийстве 2 степени, ученика 2 класса"), "капитана первого ранга, советника первого класса, в убийстве второй степени, ученика второго класса", "a rank noun's genitive after a noun and a number makes the genitive ordinal, one included")
        t |> equal(normalize_text_ru("Пилю 2 часть, Ждём 6 часть, выпустили 5 часть, вышла 2 часть, выходит 3 часть"), "Пилю вторую часть, Ждём шестую часть, выпустили пятую часть, вышла вторая часть, выходит третья часть", "a verb in the first person, an infinitive, an imperative or a past tense not feminine singular makes a nominative-like noun its object")
        t |> equal(normalize_text_ru("Каждый 20, каждая 2 машина, мой 3 пост, своей 2 половинки, при этом 1 вложение, свой 21 год"), "Каждый двадцатый, каждая вторая машина, мой третий пост, своей второй половинки, при этом одно вложение, свой двадцать один год", "a singular determiner gives the ordinal its form; при этом and a count of years stay")
        t |> equal(normalize_text_ru("на 1 и 2 этажах, 2 и 3 этажи, 2 или 3 попыткой, 3,7 метра"), "на первом и втором этажах, второй и третий этажи, второй или третьей попыткой, три целых семь десятых метра", "a list's numbers take the ordinal of its last; a decimal comma is no list")
    }
    t |> run("a number joined to a word reads as one compound word") @(t : T?) {
        t |> equal(normalize_text_ru("800-летию, 50-летия, 25-летний, 3-летний, 90-летие, 100-летие, 40-летие, 1000-летие"), "восьмисотлетию, пятидесятилетия, двадцатипятилетний, трёхлетний, девяностолетие, столетие, сорокалетие, тысячелетие")
        t |> equal(normalize_text_ru("5-этажный, 2-комнатная, 10-минутный, 12-часовой, 100-метровый, 3-кратный, 20-процентный, 2-местный"), "пятиэтажный, двухкомнатная, десятиминутный, двенадцатичасовой, стометровый, трёхкратный, двадцатипроцентный, двухместный")
        t |> equal(normalize_text_ru("21-летний, 150-летие, 2000-летие, 1500-летие, 4-ядерный, 2-ярусный"), "двадцатиоднолетний, стопятидесятилетие, двухтысячелетие, тысячепятисотлетие, четырёхъядерный, двухъярусный", "a compound number joins its parts; ъ before an iotated vowel")
        t |> equal(normalize_text_ru("к 25 летию, после 40 летней, 800 летию, 5 лет"), "к двадцатипятилетию, после сорокалетней, восьмисотлетию, пять лет", "across a space only a лет- word in the list joins")
        t |> equal(normalize_text_ru("в 40 градусную жару, 100 процентная весна, 12 цилиндровый двигатель"), "в сорокаградусную жару, стопроцентная весна, двенадцатицилиндровый двигатель", "and an adjective from a counted noun's stem")
    }
    t |> run("units, percentages, money and their agreement") @(t : T?) {
        t |> equal(normalize_text_ru("21 рубль, 22 рубля, 25 рублей"), "двадцать один рубль, двадцать два рубля, двадцать пять рублей")
        t |> equal(normalize_text_ru("Это стоит 1500 руб."), "Это стоит тысяча пятьсот рублей.")
        t |> equal(normalize_text_ru("около 15 $"), "около пятнадцати долларов")
        t |> equal(normalize_text_ru("Температура 25°C, влажность 80%."), "Температура двадцать пять градусов цельсия, влажность восемьдесят процентов.")
        t |> equal(normalize_text_ru("25℃ и 3,5℃, но 7°"), "двадцать пять градусов цельсия и три с половиной градуса цельсия, но семь градусов", "the one-symbol Celsius sign reads as the two-character one")
        t |> equal(normalize_text_ru("Цена выросла на 3,5%."), "Цена выросла на три с половиной процента.")
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
        t |> equal(normalize_text_ru("1500 млн руб."), "тысяча пятьсот миллионов рублей.")
        t |> equal(normalize_text_ru("минус 7 градусов, -5"), "минус семь градусов, минус пять")
        t |> equal(normalize_text_ru("ночью -5°C, днём -3,5%"), "ночью минус пять градусов цельсия, днём минус три с половиной процента", "the minus before a unit survives")
    }
    t |> run("punctuation survives, the rest goes; a normalized sentence is a fixed point") @(t : T?) {
        let once = normalize_text_ru("«Привет», — сказал он (тихо)… Да!")
        t |> equal(once, "Привет , — сказал он тихо … Да!")
        t |> equal(normalize_text_ru(once), once)
        let plain = "Двенадцать мешков муки нужно, чтобы открыть замок в замок."
        t |> equal(normalize_text_ru(plain), plain)
    }
}

[test(description = "a thousand at the head of a number reads тысяча, the way it is said; a count of thousands ending in one keeps its одна")]
def test_spoken_thousand_ru(t : T?) {
    t |> equal(number_to_words_ru(1000l), "тысяча")
    t |> equal(number_to_words_ru(1500l), "тысяча пятьсот")
    t |> equal(number_to_words_ru(21000l), "двадцать одна тысяча")
    t |> equal(number_to_words_ru(1001000l), "один миллион одна тысяча")
    t |> equal(ordinal_words_ru(1998l, RuOrdinalForm.gen_m), "тысяча девятьсот девяносто восьмого")
    t |> equal(normalize_text_ru("1000 рублей"), "тысяча рублей")
    t |> equal(normalize_text_ru("в 1998 году"), "в тысяча девятьсот девяносто восьмом году")
    t |> equal(normalize_text_ru("09.05.1945"), "девятого мая тысяча девятьсот сорок пятого года")
}

[test(description = "a unit abbreviation after a number reads as the unit even when its dot ends the sentence, never as the abbreviation it spells alone")]
def test_unit_before_sentence_dot_ru(t : T?) {
    t |> equal(normalize_text_ru("Рост 75 см."), "Рост семьдесят пять сантиметров.")
    t |> equal(normalize_text_ru("Цена 21 руб."), "Цена двадцать один рубль.")
    t |> equal(normalize_text_ru("Всего 3 тыс."), "Всего три тысячи.")
    t |> equal(normalize_text_ru("см. стр. 5"), "смотри страница пять", "without a number before it the abbreviation still expands")
    t |> equal(normalize_text_ru("— 352 стр. Время в пути: 10 час."), "— триста пятьдесят две страницы. Время в пути: десять часов.", "стр. and час. after a number are units")
}

[test(description = "a cardinal after a preposition or a governing word takes the case it governs; a plural noun after в or на makes it prepositional, after с instrumental")]
def test_cardinal_case_ru(t : T?) {
    t |> run("the genitive") @(t : T?) {
        t |> equal(normalize_text_ru("около 15 подразделений"), "около пятнадцати подразделений")
        t |> equal(normalize_text_ru("в течение 8 недель"), "в течение восьми недель")
        t |> equal(normalize_text_ru("более 3000 человек"), "более трёх тысяч человек")
        t |> equal(normalize_text_ru("от 38 - 40 лет"), "от тридцати восьми - сорока лет", "the second half of a range takes the case of the first")
        t |> equal(normalize_text_ru("до 1 минуты"), "до одной минуты")
        t |> equal(normalize_text_ru("около 15 $"), "около пятнадцати долларов")
        t |> equal(normalize_text_ru("не более 2,5 кг"), "не более двух с половиной килограммов")
        t |> equal(normalize_text_ru("до 70 %"), "до семидесяти процентов")
        t |> equal(normalize_text_ru("С. 54"), "С. пятьдесят четыре", "a letter with its own dot is no preposition")
        t |> equal(normalize_text_ru("около -5 кг"), "около минус пяти килограммов", "a minus between the preposition and the number does not hide it")
        t |> equal(normalize_text_ru("при -40 градусах"), "при минус сорока градусах")
        t |> equal(normalize_text_ru("от 1 000 — 2 000 км"), "от тысячи — двух тысяч километров", "a range's halves are found across their group spaces")
    }
    t |> run("the dative, instrumental, prepositional and accusative") @(t : T?) {
        t |> equal(normalize_text_ru("к 2014 году"), "к две тысячи четырнадцатому году")
        t |> equal(normalize_text_ru("по 2014 году"), "по две тысячи четырнадцатому году")
        t |> equal(normalize_text_ru("к 25 участникам"), "к двадцати пяти участникам")
        t |> equal(normalize_text_ru("с 5 детьми"), "с пятью детьми")
        t |> equal(normalize_text_ru("в 86 томах"), "в восьмидесяти шести томах")
        t |> equal(normalize_text_ru("через 1000 лет"), "через тысячу лет")
        t |> equal(normalize_text_ru("в 16 км к северу"), "в шестнадцати километрах к северу", "a distance before от or к is a place")
        t |> equal(normalize_text_ru("в гонке на 10 км"), "в гонке на десять километров")
        t |> equal(normalize_text_ru("в 2 раза"), "в два раза")
        t |> equal(normalize_text_ru("с 1 рублём"), "с одним рублём", "after one, с before a singular instrumental is the instrumental")
        t |> equal(normalize_text_ru("приравнять к 0"), "приравнять к нулю")
        t |> equal(normalize_text_ru("составляет 51 567 человек"), "составляет пятьдесят одну тысячу пятьсот шестьдесят семь человек", "a verb of amount takes the accusative")
        t |> equal(normalize_text_ru("насчитывает 21 000 жителей, получила 1 награду"), "насчитывает двадцать одну тысячу жителей, получила одну награду")
        t |> equal(normalize_text_ru("в 1 километре от города, в 191 стране мира, в 21 городе"), "в одном километре от города, в ста девяноста одной стране мира, в двадцати одном городе", "after в or на a prepositional singular after a count ending in one fixes the prepositional")
        t |> equal(normalize_text_ru("в 31 деревне, в 1 здании, о 21 стране, на 1 странице"), "в тридцати одной деревне, в одном здании, о двадцати одной стране, на первой странице", "the prepositional noun gives the gender; a numbered noun stays an ordinal")
        t |> equal(normalize_text_ru("на 1 решение, в 1 неделю"), "на одно решение, в одну неделю", "a neuter nominative in -е and an accusative stay the accusative")
        t |> equal(normalize_text_ru("площадь равна 5 га, равен 21 рублю"), "площадь равна пяти гектарам, равен двадцати одному рублю", "равный and its forms govern the dative")
    }
    t |> run("the counted noun and its adjective in every case") @(t : T?) {
        t |> equal(normalize_text_ru("около 5 евро"), "около пяти евро", "an indeclinable noun keeps its form")
        t |> equal(normalize_text_ru("с 5 рублями"), "с пятью рублями")
        t |> equal(normalize_text_ru("к 5 рублям"), "к пяти рублям")
        t |> equal(normalize_text_ru("о 5 рублях"), "о пяти рублях")
        t |> equal(normalize_text_ru("за 1 рубль"), "за один рубль")
        t |> equal(normalize_text_ru("к 1 рублю"), "к одному рублю")
        t |> equal(normalize_text_ru("к 1 метру"), "к одному метру")
        t |> equal(normalize_text_ru("с 1 метром"), "с одним метром")
        t |> equal(normalize_text_ru("о 1 метре"), "о одном метре")
        t |> equal(normalize_text_ru("с 1 тысячей"), "с одной тысячей")
        t |> equal(normalize_text_ru("до 21 км"), "до двадцати одного километра")
        t |> equal(normalize_text_ru("около 1 км²"), "около одного квадратного километра")
        t |> equal(normalize_text_ru("к 1 км²"), "к одному квадратному километру")
        t |> equal(normalize_text_ru("о 1 км²"), "о одном квадратном километре")
        t |> equal(normalize_text_ru("к 5 км²"), "к пяти квадратным километрам")
        t |> equal(normalize_text_ru("между 5 км²"), "между пятью квадратными километрами")
        t |> equal(normalize_text_ru("около 0 км"), "около нуля километров")
    }
    t |> run("one and two agree with the noun's ending in their case") @(t : T?) {
        t |> equal(normalize_text_ru("через 1 минуту"), "через одну минуту")
        t |> equal(normalize_text_ru("к 1 неделе"), "к одной неделе")
        t |> equal(normalize_text_ru("к 1 ночи"), "к одной ночи")
        t |> equal(normalize_text_ru("с 1 неделей"), "с одной неделей")
        t |> equal(normalize_text_ru("с 1 ночью"), "с одной ночью")
        t |> equal(normalize_text_ru("до 1 - 21 недели"), "до одной - двадцати одной недели", "a range's first half takes the gender of the noun after the second")
        t |> equal(normalize_text_ru("2 отличительные черты, 2 разные ситуации, 2 тонкие металлические цепочки"), "две отличительные черты, две разные ситуации, две тонкие металлические цепочки", "the noun is read across its adjectives")
        t |> equal(normalize_text_ru("в 1 большой группе, 1 большая собака, 1 большой дом, 1-2 плохих кадра"), "в одной большой группе, одна большая собака, один большой дом, один-два плохих кадра", "a singular adjective tells the gender, a plural one tells none")
        t |> equal(normalize_text_ru("ещё 1 кассету, провёл 1 настоящую атаку, ещё 1 твоего приятеля, 1 рукой за шею"), "ещё одну кассету, провёл одну настоящую атаку, ещё одного твоего приятеля, одной рукой за шею", "a feminine accusative, a masculine genitive and an instrumental after one fix its case")
    }
    t |> run("the noun phrase's own ending and the words before the number fix the case") @(t : T?) {
        t |> equal(normalize_text_ru("за 2 упряжками, с 3 голубыми камнями, по 2 причинам, этими 2 руками, этих 2 дней"), "за двумя упряжками, с тремя голубыми камнями, по двум причинам, этими двумя руками, этих двух дней")
        t |> equal(normalize_text_ru("мать 2 аутичных детей, купили 2 толстеньких лягушек, в длину 4 метров, составлял 37 273 долларов"), "мать двух аутичных детей, купили двух толстеньких лягушек, в длину четыре метров, составлял тридцать семь тысяч двести семьдесят три долларов", "a genitive plural after two to four alone, no unit, makes the genitive")
        t |> equal(normalize_text_ru("3 украшенные ветвистыми рогами важенки, 4 пропущенных от мамы, Целых 5 дней"), "три украшенные ветвистыми рогами важенки, четыре пропущенных от мамы, Целых пять дней", "an adjective in another case ends the phrase; a preposition is no noun")
        t |> equal(normalize_text_ru("около 10 и 20 минут, между 5 и 10, 1000 раз, 1000 лет назад"), "около десяти и двадцати минут, между пятью и десятью, тысячу раз, тысячу лет назад", "и carries the case to the second number; a count of times is an accusative")
        t |> equal(normalize_text_ru("женщина лет 30, мальчик лет 5, лет 20 назад"), "женщина лет тридцати, мальчик лет пяти, лет двадцать назад", "лет after a person gives the age in the genitive")
    }
    t |> run("what counts as a preposition or a range") @(t : T?) {
        t |> equal(normalize_text_ru("в 5 км южнее"), "в пяти километрах южнее")
        t |> equal(normalize_text_ru("вX 5 км"), "вX пять километров", "a Cyrillic letter glued to a Latin one is no preposition")
        t |> equal(normalize_text_ru("до 10 - 12,5 кг"), "до десяти - двенадцати с половиной килограммов")
        t |> equal(normalize_text_ru("5-10 кг"), "пять-десять килограммов", "a hyphen right after a digit is a range, not минус")
        t |> equal(normalize_text_ru("5-10 человек"), "пять-десять человек")
        t |> equal(normalize_text_ru("Лист карты O -37 -78, F -15, PZL -130"), "Лист карты O тридцать семь -семьдесят восемь, F пятнадцать, PZL сто тридцать", "a hyphen after a Latin capital is no minus and is read as nothing")
    }
}

[test(description = "the noun dictionary, a verb's dative or instrumental, a quantity word and the noun before an inverted approximation give a cardinal the gender and case its noun's ending cannot")]
def test_case_from_dictionary_ru(t : T?) {
    t |> run("the dictionary gives the gender and the case") @(t : T?) {
        t |> equal(normalize_text_ru("в 1 семье, 2 мужчины, 1 цель, численностью в 32 тысяч, с работой 1 окна"), "в одной семье, два мужчины, одна цель, численностью в тридцать две тысяч, с работой одного окна", "the noun dictionary gives the gender and the case an ending cannot tell")
        t |> equal(normalize_text_ru("Мужчина 1 рукой открыл дверь, цитировала 1 фразу, ни 1 слова, вместо 1 косы"), "Мужчина одной рукой открыл дверь, цитировала одну фразу, ни одного слова, вместо одной косы", "a dictionary form spelling one case alone fixes it after one")
        t |> equal(normalize_text_ru("1 из этих собак ощенилась, В 1 из банков, на 1 из фотографий, на 4 из 5 звезд"), "одна из этих собак ощенилась, В одном из банков, на одной из фотографий, на четыре из пяти звезд", "one of a genitive plural takes its gender, after в or на the prepositional; a score keeps its case")
        t |> equal(normalize_text_ru("сохранение 3 фаз, увидел 2 девочек, в составе 4 сел"), "сохранение трёх фаз, увидел двух девочек, в составе четырёх сел", "a dictionary genitive plural after two to four alone makes the genitive")
        t |> equal(normalize_text_ru("Проходит реконструкция Терминала 1 стоимостью 42 млн, в доме 7 жила"), "Проходит реконструкция Терминала один стоимостью сорок два миллиона, в доме семь жила", "a word heading the number after it and a verb's past are no noun the number counts")
    }
    t |> run("the genitive of a head, a quantity word or negation") @(t : T?) {
        t |> equal(normalize_text_ru("несколько 1000 евро, сотни 1000 военных, сотен 1000 подаяний, десятки 1000000 людей"), "несколько тысяч евро, сотни тысяч военных, сотен тысяч подаяний, десятки миллионов людей", "a quantity word counts a bare thousand or million as its scale word")
        t |> equal(normalize_text_ru("этих 2 организаций, для этих 15 минут, об этих 3, моих 2 пальца"), "этих двух организаций, для этих пятнадцати минут, об этих трёх, моих два пальца", "a plural pronoun in the genitive gives it with no noun or a genitive plural, not before a count's genitive singular")
        t |> equal(normalize_text_ru("Стоимость 1 билета, ни 1 твита, ни 1 звезды, Выведи 1 ребёнка"), "Стоимость одного билета, ни одного твита, ни одной звезды, Выведи одного ребёнка", "a masculine genitive after one, and ни before a noun in no nominative, ask the genitive")
        t |> equal(normalize_text_ru("1 строчка, на 1-2 раза, ни 1, ни 2, Ни 1 из них не"), "одна строчка, на один-два раза, ни один, ни два, Ни один из них не", "a feminine suffix, a range and ни with no noun stay as they are")
        t |> equal(normalize_text_ru("покинула нас ещё 1 орхидея, входила 41 коммуна, до начала 1 урока, иметь только 1 ребёнка"), "покинула нас ещё одна орхидея, входила сорок одна коммуна, до начала одного урока, иметь только одного ребёнка", "a feminine past verb or ещё before one leaves an unknown word in -а a nominative")
    }
    t |> run("the dative and the instrumental a verb takes, a list's gender") @(t : T?) {
        t |> equal(normalize_text_ru("позвонила 2 своим подружкам, махнув рукой 2 парням, подобно 2 кинжалам"), "позвонила двум своим подружкам, махнув рукой двум парням, подобно двум кинжалам", "a dative plural ending fixes the dative with no preposition")
        t |> equal(normalize_text_ru("в сезоне 2004 - 05 основным голеадорам, в 5:20 2."), "в сезоне две тысячи четыре - ноль пять основным голеадорам, в пять часов двадцать минут два.", "a range takes no dative from the words after it; a noun a clock time read already is no approximation's")
        t |> equal(normalize_text_ru("являться 1 из них, стал 1 из лучших, Это был 1 из лучших, накрываясь 1 жиденьким рваным спальником"), "являться одним из них, стал одним из лучших, Это был один из лучших, накрываясь одним жиденьким рваным спальником", "являться and a linking verb but быть make one of the instrumental; so does a masculine instrumental adjective")
        t |> equal(normalize_text_ru("2 или 3 раза, 2, 3 смены"), "два или три раза, две, три смены", "a list's first number takes the gender of the noun after the last")
    }
    t |> run("an inverted approximation and an age") @(t : T?) {
        t |> equal(normalize_text_ru("метрах в 20 от дороги, в метрах 10 от машины, Недели 2 назад, минуты через 2 я, За часа 2 они, После минут 5 его, секунды 2-3"), "метрах в двадцати от дороги, в метрах десяти от машины, Недели две назад, минуты через две я, За часа два они, После минут пяти его, секунды две-три", "an inverted approximation takes its gender from the noun before it, its case from that noun's prepositional plural or the preposition")
        t |> equal(normalize_text_ru("на вид лет 40, в возрасте лет 7, в 2 километрах с 23 станциями, в течение часа 2 раза, через 15 минут 1 поединок"), "на вид лет сорока, в возрасте лет семи, в двух километрах с двадцатью тремя станциями, в течение часа два раза, через пятнадцать минут один поединок", "на вид and возрасте make an age; a noun after the number is what it counts, a number before the noun counts it")
    }
}

[test(description = "the halves of a date or year range take the forms their prepositions and their noun ask for; a range of bare years with no preposition and no noun stays cardinal")]
def test_ranges_ru(t : T?) {
    t |> run("days of the month") @(t : T?) {
        t |> equal(normalize_text_ru("с 10 по 16 декабря"), "с десятого по шестнадцатое декабря")
        t |> equal(normalize_text_ru("с 4 по 5 января"), "с четвёртого по пятое января")
        t |> equal(normalize_text_ru("в ночь с 17 на 18 мая"), "в ночь с семнадцатого на восемнадцатое мая")
        t |> equal(normalize_text_ru("10—16 декабря 2001 года"), "десятого — шестнадцатого декабря две тысячи первого года")
        t |> equal(normalize_text_ru("на 20 февраля 2015"), "на двадцатое февраля две тысячи пятнадцатого года")
        t |> equal(normalize_text_ru("(5 мая 2014)"), "пятое мая две тысячи четырнадцатого года", "a date standing alone in parentheses is the nominative")
        t |> equal(normalize_text_ru("13-01-2012"), "тринадцатого января две тысячи двенадцатого года")
        t |> equal(normalize_text_ru("к 5 мая"), "к пятому мая")
        t |> equal(normalize_text_ru("с 5 до 10 мая"), "с пятого до десятого мая")
        t |> equal(normalize_text_ru("2024-13-45"), "две тысячи двадцать четыре-тринадцать-сорок пять", "a month past twelve is no date")
    }
    t |> run("years") @(t : T?) {
        t |> equal(normalize_text_ru("в 2001—2003 годах"), "в две тысячи первом — две тысячи третьем годах")
        t |> equal(normalize_text_ru("в 1941-1945 гг."), "в тысяча девятьсот сорок первом — тысяча девятьсот сорок пятом годах")
        t |> equal(normalize_text_ru("с 1956 по 1999 год"), "с тысяча девятьсот пятьдесят шестого по тысяча девятьсот девяносто девятый год")
        t |> equal(normalize_text_ru("за 2014 год"), "за две тысячи четырнадцатый год")
        t |> equal(normalize_text_ru("В 2004 г. вышел"), "В две тысячи четвёртом году вышел")
        t |> equal(normalize_text_ru("в 988 году"), "в девятьсот восемьдесят восьмом году")
        t |> equal(normalize_text_ru("умерла в 665 г."), "умерла в шестьсот шестьдесят пятом году", "a three-digit year before г. after в reads as году")
        t |> equal(normalize_text_ru("в 2002 Робредо"), "в две тысячи втором Робредо", "a year after в with no noun is still a year")
        t |> equal(normalize_text_ru("Толстой (1828 — 1910)"), "Толстой тысяча восемьсот двадцать восемь — тысяча девятьсот десять")
        t |> equal(normalize_text_ru("за 2 года"), "за два года", "a count of years is no year")
        t |> equal(normalize_text_ru("по 2004 г."), "по две тысячи четвёртый год")
        t |> equal(normalize_text_ru("к 2004 г."), "к две тысячи четвёртому году")
        t |> equal(normalize_text_ru("2004 г."), "две тысячи четвёртого года")
        t |> equal(normalize_text_ru("в 1941—45 годах"), "в тысяча девятьсот сорок первом — сорок пятом годах")
        t |> equal(normalize_text_ru("с 1956 по 1999"), "с тысяча девятьсот пятьдесят шестого по тысяча девятьсот девяносто девятый")
        t |> equal(normalize_text_ru("в 3000 году"), "в три тысячи году", "a number past 2200 is no year")
        t |> equal(normalize_text_ru("300 года"), "трёхсотого года", "a three-digit number is a year only before году; before года it is the ordinal a count cannot govern")
        t |> equal(normalize_text_ru("в 1554/55 году, в 1588/89, сезона 1993/94, март 2013 / 11:09"), "в тысяча пятьсот пятьдесят четвёртом — пятьдесят пятом году, в тысяча пятьсот восемьдесят восьмом — восемьдесят девятом, сезона тысяча девятьсот девяносто три девяносто четыре, март две тысячи тринадцатого одиннадцать часов девять минут", "a slash joins a later year as a dash does; a pair with no noun and no preposition stays cardinal")
        t |> equal(normalize_text_ru("в июле 2008 перешёл"), "в июле две тысячи восьмого перешёл", "a year after a month name is its genitive")
        t |> equal(normalize_text_ru("июль 2008, С мая 1917 по июнь 1918 г."), "июль две тысячи восьмого, С мая тысяча девятьсот семнадцатого по июнь тысяча девятьсот восемнадцатого года")
    }
    t |> run("ordinal suffixes in ranges and before plural nouns") @(t : T?) {
        t |> equal(normalize_text_ru("в 1960—1970-х годах"), "в тысяча девятьсот шестидесятых — тысяча девятьсот семидесятых годах")
        t |> equal(normalize_text_ru("в 50-е годы"), "в пятидесятые годы")
        t |> equal(normalize_text_ru("заняла 118-е место"), "заняла сто восемнадцатое место")
        t |> equal(normalize_text_ru("5-й танковой армией"), "пятой танковой армией")
        t |> equal(normalize_text_ru("стал 4-м в зачёте"), "стал четвёртым в зачёте")
        t |> equal(normalize_text_ru("с 498-м гаубичным полком"), "с четыреста девяносто восьмым гаубичным полком")
        t |> equal(normalize_text_ru("5-й мировой рекорд"), "пятый мировой рекорд", "an adjective in -ой before a masculine noun keeps -й masculine")
        t |> equal(normalize_text_ru("5-й роты"), "пятой роты")
        t |> equal(normalize_text_ru("к 60-м годам"), "к шестидесятым годам")
        t |> equal(normalize_text_ru("5-м гаубичным"), "пятым гаубичным")
        t |> equal(normalize_text_ru("с 5-м"), "с пятым")
    }
}

[test(description = "a Roman numeral after a name or before век reads as an ordinal in the form they ask for; with nothing numbering it, a numeral of two letters or more under forty reads as a cardinal; a Roman letter inside a Latin word stays")]
def test_roman_ru(t : T?) {
    t |> equal(normalize_text_ru("Павел II"), "Павел второй")
    t |> equal(normalize_text_ru("по указанию Петра I, был"), "по указанию Петра первого, был")
    t |> equal(normalize_text_ru("Екатерина II и Екатерины II"), "Екатерина вторая и Екатерины второй")
    t |> equal(normalize_text_ru("с Николаем II"), "с Николаем вторым")
    t |> equal(normalize_text_ru("XIX век"), "девятнадцатый век")
    t |> equal(normalize_text_ru("в XX веке"), "в двадцатом веке")
    t |> equal(normalize_text_ru("в XVI—XVII веках"), "в шестнадцатом — семнадцатом веках")
    t |> equal(normalize_text_ru("в конце XIX — начале XX века"), "в конце девятнадцатого — начале двадцатого века")
    t |> equal(normalize_text_ru("Выпуск (XV — XVI)"), "Выпуск пятнадцать — шестнадцать")
    t |> equal(normalize_text_ru("Глава II"), "Глава вторая", "a section noun gives its own gender")
    t |> equal(normalize_text_ru("в XIX в."), "в девятнадцатом веке")
    t |> equal(normalize_text_ru("I love VIVA and MIX"), "I love VIVA and MIX")
    t |> equal(normalize_text_ru("в XVI—XVII вв."), "в шестнадцатом — семнадцатом веках")
    t |> equal(normalize_text_ru("по XX в."), "по двадцатый век")
    t |> equal(normalize_text_ru("IIII век и IC век"), "IIII век и IC век", "a numeral not written the canonical way stays")
    t |> equal(normalize_text_ru("с Екатериной II и Петром I"), "с Екатериной второй и Петром первым")
    t |> equal(normalize_text_ru("Екатерину II и Петру I"), "Екатерину вторую и Петру первому")
    t |> equal(normalize_text_ru("Екатерине II, о Петре I"), "Екатерине второй, о Петре первом")
    t |> equal(normalize_text_ru("До II, Б II"), "До два, Б два", "a preposition or a one-letter capital names nothing")
    t |> equal(normalize_text_ru("XVIII начале XIX века"), "восемнадцатого начале девятнадцатого века", "a period word ties two centuries with no dash")
    t |> equal(normalize_text_ru("в 19 веке, в начале 19 века, 19 в., в 19 в."), "в девятнадцатом веке, в начале девятнадцатого века, девятнадцатого века, в девятнадцатом веке", "an Arabic century reads as a Roman one")
    t |> equal(normalize_text_ru("18 - начале 19 века, в конце 18 начале 19 века, в 20 —21 вв."), "восемнадцатого - начале девятнадцатого века, в конце восемнадцатого начале девятнадцатого века, в двадцатом — двадцать первом веках")
    t |> equal(normalize_text_ru("в начале 2 века, 2 век до н. э., в 3 тысячелетии"), "в начале второго века, второй век до н. э., в третьем тысячелетии")
    t |> equal(normalize_text_ru("в 25 веке, около 4 веков, на 2 века старше, 2 тысячелетия назад, 3,3 — 5 В."), "в двадцать пять веке, около четырёх веков, на два века старше, два тысячелетия назад, три целых три десятых — пять В.", "past twenty-one, a count of centuries and volts stay cardinals")
}

[test(description = "a regnal or section numeral is spelled with I, V and X and is at most forty, a century at most twenty-one; Latin capitals that fail the gate stay as written")]
def test_roman_gate_ru(t : T?) {
    t |> equal(normalize_text_ru("Вашингтон DC"), "Вашингтон DC")
    t |> equal(normalize_text_ru("Витамин C нужен всем."), "Витамин C нужен всем.")
    t |> equal(normalize_text_ru("Размер XL"), "Размер XL")
    t |> equal(normalize_text_ru("Посмотри CV и XML"), "Посмотри CV и XML")
    t |> equal(normalize_text_ru("в XXV веке"), "в XXV веке")
    t |> equal(normalize_text_ru("Иван IV, Людовик XIV, глава XII"), "Иван четвёртый, Людовик четырнадцатый, глава двенадцатая")
}

[test(description = "a Roman numeral with an ordinal suffix reads as the ordinal in the suffix's form, the way an Arabic one does")]
def test_roman_suffix_ru(t : T?) {
    t |> equal(normalize_text_ru("в XX-м веке"), "в двадцатом веке")
    t |> equal(normalize_text_ru("XIX-й век"), "девятнадцатый век")
}

[test(description = "a clock time reads with its часов and минут in the case the word before it asks for; a set score in a list after a match score is not a time")]
def test_clock_times_ru(t : T?) {
    t |> equal(normalize_text_ru("11:00"), "одиннадцать часов")
    t |> equal(normalize_text_ru("9:15"), "девять часов пятнадцать минут")
    t |> equal(normalize_text_ru("в 16:00"), "в шестнадцать часов")
    t |> equal(normalize_text_ru("16:30"), "шестнадцать часов тридцать минут")
    t |> equal(normalize_text_ru("01:31"), "час тридцать одна минута")
    t |> equal(normalize_text_ru("21:02"), "двадцать один час две минуты")
    t |> equal(normalize_text_ru("к 18:00"), "к восемнадцати часам")
    t |> equal(normalize_text_ru("Уругвай 3: 0 (15: 9, 17:15, 18:16)."), "Уругвай три: ноль пятнадцать: девять, семнадцать пятнадцать, восемнадцать шестнадцать .")
    t |> equal(normalize_text_ru("3: 0 (15:09, 15:10)"), "три: ноль пятнадцать ноль девять, пятнадцать десять")
    t |> equal(normalize_text_ru("(15:10)"), "пятнадцать часов десять минут", "parentheses with no score before them hold a time")
    t |> equal(normalize_text_ru("к 1:00"), "к одному часу")
}

[test(description = "square, cubic and per-time units read whole, agreeing with the count and its case; a decimal takes the genitive singular, except a half: пол, полтора, с половиной")]
def test_measures_ru(t : T?) {
    t |> equal(normalize_text_ru("0,5 кг, 0,5 л, 0,5 км/ч"), "полкилограмма, пол-литра, полкилометра в час")
    t |> equal(normalize_text_ru("1,5 кг, 1,5 т, 1,5 км/ч"), "полтора килограмма, полторы тонны, полтора километра в час")
    t |> equal(normalize_text_ru("около 1,5 т, в 1,5 км к северу"), "около полутора тонн, в полутора километрах к северу")
    t |> equal(normalize_text_ru("2,5 кг, 5,5 т, около 2,5 т, 2,5 км²"), "два с половиной килограмма, пять с половиной тонн, около двух с половиной тонн, два с половиной квадратных километра")
    t |> equal(normalize_text_ru("21,5 кг, 1,25 кг, 0,5 км²"), "двадцать одна целая пять десятых килограмма, одна целая двадцать пять сотых килограмма, ноль целых пять десятых квадратного километра", "a whole ending in one, another fraction and a half before an adjective keep целых")
    t |> equal(normalize_text_ru("вес 01,5 кг, 01,25 кг"), "вес полтора килограмма, одна целая двадцать пять сотых килограмма", "a leading zero before a decimal's comma is no code")
    t |> equal(normalize_text_ru("21.07.05 г."), "двадцать одна целая семь сотых.ноль пять граммов.", "a leading-zero decimal after a digit and a dot is a dotted chain's tail, no decimal")
    t |> equal(normalize_text_ru("площадь 18,8 км²."), "площадь восемнадцать целых восемь десятых квадратного километра.")
    t |> equal(normalize_text_ru("длилась 1.5 года, через 2,5 часа, 9.5 лет назад, около 1.5 метра, в 3.5 раза длиннее"), "длилась полтора года, через два с половиной часа, девять с половиной лет назад, около полутора метров, в три с половиной раза длиннее", "a whole and a half before a counted noun written out reads as before its unit, after a comma or a dot")
    t |> equal(normalize_text_ru("версия 2.5 вышла, 0,5 человека, в 3.5. Потом"), "версия две целых пять десятых вышла, ноль целых пять десятых человека, в три целых пять десятых. Потом", "no counted noun, a zero whole or a sentence's dot keep the decimal")
    t |> equal(normalize_text_ru("1 км², 2 м², 5 км²"), "один квадратный километр, два квадратных метра, пять квадратных километров")
    t |> equal(normalize_text_ru("около 3 км²"), "около трёх квадратных километров")
    t |> equal(normalize_text_ru("5 см³"), "пять кубических сантиметров")
    t |> equal(normalize_text_ru("60 км/ч"), "шестьдесят километров в час")
    t |> equal(normalize_text_ru("до 20 м/сек"), "до двадцати метров в секунду")
    t |> equal(normalize_text_ru("1 м/с"), "один метр в секунду")
    t |> equal(normalize_text_ru("25 °C"), "двадцать пять градусов цельсия")
    t |> equal(normalize_text_ru("— 432 с. —"), "— четыреста тридцать две страницы —", "a count with с. after a dash is a citation's page count")
    t |> equal(normalize_text_ru("словарь: в 86 т. (82 т. и 4 доп.)."), "словарь: в восьмидесяти шести томах восемьдесят два тома и четыре дополнительных .", "т. after в or before supplements counts volumes")
    t |> equal(normalize_text_ru("атлас: в 4 т.; 2004 — 2008, т. 1"), "атлас: в четырёх томах; две тысячи четыре — две тысячи восемь, том один", "т. before a number is the volume it names")
    t |> equal(normalize_text_ru("груз 5 т. угля"), "груз пять тонн угля", "anywhere else т. is tonnes")
    t |> equal(normalize_text_ru("000123 км², 0123 км/ч"), "ноль ноль ноль один два три квадратных километров, ноль один два три километров в час", "a number read digit by digit keeps its unit's adjective and tail")
    t |> equal(normalize_text_ru("1234567890123456789012 км²"), "один два три четыре пять шесть семь восемь девять ноль один два три четыре пять шесть семь восемь девять ноль один два квадратных километров")
    t |> equal(normalize_text_ru("50. 000 экз."), "пятьдесят. ноль ноль ноль экземпляров.", "a leading zero reads digit by digit before a unit too")
    t |> equal(normalize_text_ru("— 1500 экз. — 480 с., ил."), "— тысяча пятьсот экземпляров — четыреста восемьдесят страниц, ил.")
    t |> equal(normalize_text_ru("в 11 км юго-восточнее деревни"), "в одиннадцати километрах юго-восточнее деревни")
    t |> equal(normalize_text_ru("площадью 26 кв. км, около 5 кв. км, 10 000 кв. м, кв. 12"), "площадью двадцать шесть квадратных километров, около пяти квадратных километров, десять тысяч квадратных метров, квартира двенадцать", "кв. between a number and a unit is квадратный")
    t |> equal(normalize_text_ru("объектив 50мм"), "объектив пятьдесят миллиметров", "a unit of two letters or more may stand glued to its number")
    t |> equal(normalize_text_ru("5 USD5, 5 м/x, 5 с лишним"), "пять USD5, пять метров x, пять с лишним", "a unit glued to more letters, an unknown per-unit and с before a word are not taken")
    t |> equal(normalize_text_ru("5м, 15USD"), "пятьм, пятнадцатьUSD", "a one-letter unit or a code glued to the number is not taken")
    t |> equal(normalize_text_ru("1234567890123456789012°C"), "один два три четыре пять шесть семь восемь девять ноль один два три четыре пять шесть семь восемь девять ноль один два градусов цельсия")
    t |> equal(normalize_text_ru("1234567890123456789012 $"), "один два три четыре пять шесть семь восемь девять ноль один два три четыре пять шесть семь восемь девять ноль один два долларов")
    t |> equal(normalize_text_ru("площадь 39 486,678 км²"), "площадь тридцать девять тысяч четыреста восемьдесят шесть целых шестьсот семьдесят восемь тысячных квадратного километра")
}

[test(description = "fractions read with their denominator as an ordinal: одна вторая, три четвёртых, a whole part with целых; before a unit or a word written out a half, a quarter or a third reads as a word (полчаса, полстакана, четверть века, полторы тонны); a minus before a fraction reads минус; a season or a day pair is no fraction")]
def test_fractions_ru(t : T?) {
    t |> equal(normalize_text_ru("½ т, 1/2 т, ½ л, ½ ч, ½ м, ½ мин, ½ сек"), "полтонны, полтонны, пол-литра, полчаса, полметра, полминуты, полсекунды", "пол before л or a vowel takes a hyphen")
    t |> equal(normalize_text_ru("около ½ ч, к ½ ч, с ½ т"), "около получаса, к получасу, с полутонны")
    t |> equal(normalize_text_ru("1/4 ч, 3/4 т, 1/3 т, 2/3 т"), "четверть часа, три четверти тонны, треть тонны, две трети тонны")
    t |> equal(normalize_text_ru("около 1/4 ч, около 3/4 т, к 1/4 ч"), "около четверти часа, около трёх четвертей тонны, к четверти часа")
    t |> equal(normalize_text_ru("над 1/3 т, между 2/3 т, о 3/4 т"), "над третью тонны, между двумя третями тонны, о трёх четвертях тонны")
    t |> equal(normalize_text_ru("1/8 т, ½ км², 1/4 км², ½ млн"), "одна восьмая тонны, одна вторая квадратного километра, четверть квадратного километра, одна вторая миллиона", "an eighth, a half before an adjective and a half of a scale word stay fractions")
    t |> equal(normalize_text_ru("1 ½ т, 21 ½ т"), "полторы тонны, двадцать одна целая одна вторая тонны")
    t |> equal(normalize_text_ru("оценка -1/2 балла"), "оценка минус полбалла")
    t |> equal(normalize_text_ru("сдвиг -½ тона"), "сдвиг минус полтона")
    t |> equal(normalize_text_ru("5-1/2, тип-1/2"), "пять-одна вторая, тип-одна вторая", "a dash after a digit or a letter is no minus")
    t |> equal(normalize_text_ru("1/2"), "одна вторая")
    t |> equal(normalize_text_ru("3/4"), "три четвёртых")
    t |> equal(normalize_text_ru("2 1/2"), "две целых одна вторая")
    t |> equal(normalize_text_ru("½ руб."), "полрубля")
    t |> equal(normalize_text_ru("2 ½ руб."), "два с половиной рубля")
    t |> equal(normalize_text_ru("2/3 сенаторов"), "две трети сенаторов")
    t |> equal(normalize_text_ru("½ стакана"), "полстакана")
    t |> equal(normalize_text_ru("1/2 литра, ½ яблока, за ничью ½ очка"), "пол-литра, пол-яблока, за ничью пол-очка")
    t |> equal(normalize_text_ru("1/4 века, 3/4 часа, 1/3 пути, 2/3 голосов"), "четверть века, три четверти часа, треть пути, две трети голосов")
    t |> equal(normalize_text_ru("около 2/3 длины, на ½ длины, к ¼ стакана"), "около двух третей длины, на полдлины, к четверти стакана")
    t |> equal(normalize_text_ru("равная 1/4 денария, равна 2/3 длины"), "равная четверти денария, равна двум третям длины")
    t |> equal(normalize_text_ru("Поезда 1/2, 3/4 курсируют"), "Поезда одна вторая, три четвёртых курсируют", "a fraction closing a list of fractions keeps the plain reading")
    t |> equal(normalize_text_ru("1/4 финала, 1/8 финала, 3/5 голосов, ¼ часть суши"), "одна четвёртая финала, одна восьмая финала, три пятых голосов, одна четвёртая часть суши", "a round of a cup, another fraction and a part of a whole stay fractions")
    t |> equal(normalize_text_ru("около ½ стакана, более ½ валовой продукции, 1/2 проводится, ½ дес., усадебной"), "около одной второй стакана, более одной второй валовой продукции, одна вторая проводится, одна вторая дес., усадебной", "пол joins no oblique case, adjective, verb or abbreviation")
    t |> equal(normalize_text_ru("0,5"), "ноль целых пять десятых")
    t |> equal(normalize_text_ru("Сезон 2012/13"), "Сезон две тысячи двенадцать тринадцать")
    t |> equal(normalize_text_ru("выдержка больше 1/30 сек."), "выдержка больше одной тридцатой секунды", "a fraction takes its preposition's case, its unit the genitive singular")
    t |> equal(normalize_text_ru("10/11 января"), "десятого — одиннадцатого января", "a day pair before a month is no fraction")
    t |> equal(normalize_text_ru("5/3, 1/200, 12/05/2020"), "пять три, один двести, двенадцать ноль пять две тысячи двадцать", "an improper fraction, a denominator past a hundred and a date are no fraction")
    t |> equal(normalize_text_ru("1234 1/2"), "тысяча двести тридцать четыре одна вторая", "a whole part of four digits does not start a mixed fraction")
}

[test(description = "a telephone number or an ISBN reads in groups with a pause between them; a number with a leading zero reads digit by digit")]
def test_phones_ru(t : T?) {
    t |> equal(normalize_text_ru("+7 (495) 123-45-67"), "плюс семь, четыреста девяносто пять, сто двадцать три, сорок пять, шестьдесят семь")
    t |> equal(normalize_text_ru("8-800-200-00-00"), "восемь, восемьсот, двести, ноль ноль, ноль ноль")
    t |> equal(normalize_text_ru("тел. 123-45-67"), "телефон сто двадцать три, сорок пять, шестьдесят семь")
    t |> equal(normalize_text_ru("ISBN 5-98472-008-6."), "ISBN пять, девятьсот восемьдесят четыре семьдесят два, ноль ноль восемь, шесть.")
    t |> equal(normalize_text_ru("ISSN 1313-2970"), "ISSN тринадцать тринадцать, двадцать девять семьдесят")
    t |> equal(normalize_text_ru("Телефонный код — 0425."), "Телефонный код — ноль четыре два пять.")
    t |> equal(normalize_text_ru("Код -0425 найден"), "Код минус ноль четыре два пять найден", "a minus survives the digit-by-digit reading")
    t |> equal(normalize_text_ru("специальность 07. 00.02"), "специальность ноль семь. ноль ноль ноль два", "a leading-zero decimal after a number and a dot is a code's tail")
    t |> equal(normalize_text_ru("— 2013-02-14. Депутаты"), "— четырнадцатого февраля две тысячи тринадцатого года. Депутаты", "a dash before a date is punctuation, not a hyphen chain")
    t |> equal(normalize_text_ru("Проверено 2008-11-29."), "Проверено двадцать девятого ноября две тысячи восьмого года.", "an ISO date is a date, not a number in groups")
}

[test(description = "a hyphenated run is no telephone number when it has the shape of a date - four digits first, then a one-digit group - or two four-digit groups, unless an area code or a book number marks it")]
def test_phone_gate_ru(t : T?) {
    t |> equal(normalize_text_ru("Встреча 2024-1-15 в офисе"), "Встреча пятнадцатого января две тысячи двадцать четвёртого года в офисе")
    t |> equal(normalize_text_ru("Коды 1941-1945-1991"), "Коды тысяча девятьсот сорок один-тысяча девятьсот сорок пять-тысяча девятьсот девяносто один")
    t |> equal(normalize_text_ru("ISBN 1-4612-1712-1"), "ISBN один, сорок шесть двенадцать, семнадцать двенадцать, один")
    t |> equal(normalize_text_ru("(495) 12-03-2018"), "четыреста девяносто пять, двенадцать, ноль три, двадцать восемнадцать", "an area code keeps a date-shaped run a telephone number")
    t |> equal(normalize_text_ru("ISBN: 5-98472-008-6"), "ISBN: пять, девятьсот восемьдесят четыре семьдесят два, ноль ноль восемь, шесть")
    t |> equal(normalize_text_ru("A-123-45-67"), "A-сто двадцать три-сорок пять-шестьдесят семь", "a run right after a hyphen continues a code, not a telephone number")
    t |> equal(normalize_text_ru("DOI 1471 - 2180-9-16"), "DOI тысяча четыреста семьдесят один - две тысячи сто восемьдесят-девять-шестнадцать", "a hyphen chain running into a year-first date shape is no date")
    t |> equal(normalize_text_ru("(4951) 2345-67-89"), "сорок девять пятьдесят один, двадцать три сорок пять, шестьдесят семь, восемьдесят девять")
}

[test(description = "money: one or two fraction digits as the minor unit, the currency after a scale word in the genitive plural, a half of a scale as с половиной, the ISO codes as currencies")]
def test_money_ru(t : T?) {
    t |> equal(normalize_text_ru("0,5 руб."), "пятьдесят копеек.")
    t |> equal(normalize_text_ru("1,5 руб."), "один рубль пятьдесят копеек.")
    t |> equal(normalize_text_ru("2,50 руб."), "два рубля пятьдесят копеек.")
    t |> equal(normalize_text_ru("1,25 руб."), "один рубль двадцать пять копеек.")
    t |> equal(normalize_text_ru("1,05 руб."), "один рубль пять копеек.")
    t |> equal(normalize_text_ru("$0.99, 0,99 $"), "девяносто девять центов, девяносто девять центов")
    t |> equal(normalize_text_ru("$1.50, €1.50, €0,01"), "один доллар пятьдесят центов, один евро пятьдесят центов, один цент")
    t |> equal(normalize_text_ru("5,00 руб."), "пять рублей.")
    t |> equal(normalize_text_ru("около 2,50 руб."), "около двух рублей пятидесяти копеек.")
    t |> equal(normalize_text_ru("к 1,5 руб."), "к одному рублю пятидесяти копейкам.")
    t |> equal(normalize_text_ru("2,22 грн, 0,01 руб, 0,02 руб"), "две гривны двадцать две копейки, одна копейка, две копейки")
    t |> equal(normalize_text_ru("1,50 USD"), "один доллар США пятьдесят центов")
    t |> equal(normalize_text_ru("1,234 руб."), "одна целая двести тридцать четыре тысячных рубля.", "three fraction digits are no money")
    t |> equal(normalize_text_ru("22,2 млн руб."), "двадцать две целых две десятых миллиона рублей.")
    t |> equal(normalize_text_ru("$3,2 миллиарда, свыше $3 миллиардов, $2,5 миллиарда, $3 триллиона"), "три целых две десятых миллиарда долларов, свыше трёх миллиардов долларов, два с половиной миллиарда долларов, три триллиона долларов", "a scale word written out after a sign's amount counts it, the currency after it")
    t |> equal(normalize_text_ru("стоимостью 2,2$ млрд, около 35 € миллиардов"), "стоимостью две целых две десятых миллиарда долларов, около тридцати пяти миллиардов евро", "and after an amount the sign follows")
    t |> equal(normalize_text_ru("на $10 миллионов долларов"), "на десять миллионов долларов", "the currency written out again after the scale word goes")
    t |> equal(normalize_text_ru("стоит 5 руб. Миллион человек"), "стоит пять рублей. Миллион человек", "a capital starts a sentence, no scale word")
    t |> equal(normalize_text_ru("1500 руб."), "тысяча пятьсот рублей.")
    t |> equal(normalize_text_ru("15 $"), "пятнадцать долларов")
    t |> equal(normalize_text_ru("€20"), "двадцать евро")
    t |> equal(normalize_text_ru("2,5 млн руб."), "два с половиной миллиона рублей.")
    t |> equal(normalize_text_ru("1,5 тыс. руб."), "полторы тысячи рублей.")
    t |> equal(normalize_text_ru("около 2,5 млн"), "около двух с половиной миллионов")
    t |> equal(normalize_text_ru("$1 млрд"), "один миллиард долларов")
    t |> equal(normalize_text_ru("150 млн долл."), "сто пятьдесят миллионов долларов.")
    t |> equal(normalize_text_ru("29 784 USD"), "двадцать девять тысяч семьсот восемьдесят четыре доллара США")
    t |> equal(normalize_text_ru("$38 008 USD"), "тридцать восемь тысяч восемь долларов", "a currency code after a sign's amount says nothing new")
    t |> equal(normalize_text_ru("свыше $100 млн."), "свыше ста миллионов долларов.", "the preposition before a currency sign governs the amount after it")
    t |> equal(normalize_text_ru("против 18 948 USD"), "против восемнадцати тысяч девятисот сорока восьми долларов США")
    t |> equal(normalize_text_ru("около 1,5 млн, к 1,5 тыс."), "около полутора миллионов, к полутора тысячам.")
    t |> equal(normalize_text_ru("1,5 млн"), "полтора миллиона")
    t |> equal(normalize_text_ru("21,5 млн, 0,5 млн"), "двадцать одна целая пять десятых миллиона, ноль целых пять десятых миллиона", "a whole part ending in one or zero takes no с половиной")
    t |> equal(normalize_text_ru("$1,5"), "один доллар пятьдесят центов")
    t |> equal(normalize_text_ru("около 2,5"), "около двух целых пяти десятых")
    t |> equal(normalize_text_ru("в 1977 в сумме"), "в тысяча девятьсот семьдесят седьмом в сумме", "a year after в before a preposition is a year")
}
