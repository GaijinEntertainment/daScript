# dasLLAMA architecture - text to speech, the Russian normalizer

A companion of `ARCHITECTURE_TTS_FRONT_END.md`: the mechanisms of the Russian text normalizer,
`dasllama_textnorm_ru.das` - the case a numeral takes, count agreement, a placing distance, years,
ordinal suffixes, telephone numbers, centuries and Roman numerals, a number joined to a word, a
bare number before the noun it numbers, and an approximation with its noun first. The file
charters are `ARCHITECTURE_TTS_FRONT_END.md#tts-front-end`; `ARCHITECTURE_COMMON.md` (repo root)
is the contract.

## Mechanisms

### How a Russian numeral gets its case {#tts-ru-case-government}

A numeral is read in a case other than the nominative only where a rule fixes it: the
preposition or governing word before it, looked up in a government table (около пятнадцати, к
двадцати пяти, в течение восьми) - a currency sign or a minus between them does not hide the word
(около минус пяти), и or или carries a number's governing word to the number after it (около
десяти и двадцати), and a - before a number or a fraction reads минус unless a digit stands before
it across spaces, a letter touches it, or a Latin capital stands before it across spaces (лист
карты O -37), and the hyphen then reads as nothing (PZL сто тридцать); a noun after it with a
plural prepositional ending after в or на (в 86 томах), or - after a count ending in one - a
prepositional singular of four letters or more in -е or -и that is no neuter nominative (в
двадцати одном городе, в одном километре от города), or an instrumental ending after с - singular
after a count ending in one, plural otherwise (с одним рублём, с 5 детьми); the noun of a year or
a century; the other half of its range, found across thousands group spaces (от тысячи - двух
тысяч); and the name before a regnal number. The cardinal's words in all six cases are tables in
the file; a counted noun's two genitives are stored with it, and its dative, accusative,
instrumental and prepositional are derived from its nominative's ending - -а, -я, -ия feminine,
-о, -е, -ие neuter - and a locative in -у is stored with it (году, часу); a feminine noun in -ь
(четверть, треть) is marked feminine, since its ending cannot tell. Where an ending cannot tell a
noun's gender or the cases its form spells, the noun dictionary `dasllama_textnorm_ru_nouns.das`
does: the frequent feminine and neuter nouns and the animate masculine nouns in -а or -я, each a
nominative with its genitives where the ending rules do not derive them, the other singular cases
derived by those rules; a form two genders spell belongs to the more frequent noun. Where no rule
fixes the case, the numeral stays in the nominative.

Whatever stands before it, a count not ending in one before a plural instrumental (-ами, -ями,
-ьми, an adjective in -ыми) reads in the instrumental where the word before allows it (за двумя
упряжками), before a plural dative after по in the dative (по двум причинам); two, three or four
alone in the nominative or accusative before a genitive plural in -ов, -ев or -ей - no unit's -
read in the genitive (мать двух аутичных детей); after one an adjective in -ую fixes the
accusative, in -ого the genitive, a feminine noun in -ку, -ну, -цу, -ту, -ию or -ню the accusative
(ещё одну кассету), a noun in -ой or -ью the instrumental. A plural pronoun before the number
whose noun agrees gives its case (этих двух дней, этими двумя руками); a count of раз and of лет
before назад is an accusative (тысячу раз). лет after a noun for a person, and after вид or
возрасте (на вид лет сорока), reads the age after it in the genitive (женщина лет тридцати).

After one a dictionary form spelling the accusative, the genitive or the instrumental alone fixes
it (одну руку, одного окна, одной рукой), and a word in -а or -я the dictionary does not hold,
outside -чка, -шка, -мка, -лька, -енка, -ица, -ия, -ха, -вна, is a masculine genitive (одного
билета) - not after ещё, всего, целая or целый, nor after a feminine past verb, a word in -ла the
dictionary holds no noun form of (ещё одна орхидея, входила сорок одна коммуна; до начала одного
урока); ни before one asks the genitive unless a nominative follows (ни одного слова, ни один
человек). A number before из takes the gender of the genitive plural after it (одна из машин),
the prepositional after в, во or на, the instrumental after являться or a linking verb other than
быть (стал одним из; это был один из), else the case the word before governs; a number after из
makes a score (на четыре из пяти). A bare thousand or million after несколько, сотни, сотен,
сотнями, сотню, десятки, десятков, десятками or пару reads as the scale word's genitive plural
(сотен тысяч).

A genitive plural pronoun (этих, своих) gives the genitive with no noun or before a genitive
plural (этих двух организаций). A dative plural phrase - a noun in -ам or -ям, or an adjective in
-ым or -им before one - fixes the dative where no preposition governs and the number opens no
range (позвонила двум своим подружкам). The first numbers of a list take the gender of the noun
after the last (два или три раза); a word under three letters gives no gender. A word after the
number that heads a number after it, as digits or already read, is no noun the number counts
(Терминала один стоимостью сорок два).

In the prepositional the noun gives one its gender: feminine when its ending is -и outside the
neuter endings of an -ие noun (-ании, -ении, -етии, -ятии, -ытии, -итии, -одии, -овии, -елии), or
when the noun dictionary holds it as a feminine noun's prepositional (стране, семье, неделе),
masculine otherwise (в ста девяноста одной стране, о двадцати одном городе); a table noun's
prepositional gender is its own (в одной руке). One takes its gender from a singular adjective's
ending (одна большая собака) and none from a plural one (один-два плохих кадра). The noun is read
across up to four agreeing adjectives; an adjective in another case or number, a preposition, a
word under three letters or a capital ends the phrase. Равный, равная,
равное, равные, равен, равна and равны govern the dative (равна пяти гектарам, равная четверти
денария). A verb of amount - составлять, насчитывать, иметь, получить in their present and past
forms - governs the accusative (составляет пятьдесят одну тысячу, получила одну награду).

### What a Russian count does to the word it counts {#tts-ru-count-agreement}

In the nominative and the accusative a counted noun takes the count's grammatical number: the
nominative singular after a count ending in one (not eleven), the genitive singular after two to
four (not twelve to fourteen), the genitive plural after the rest (один метр, два метра, пять
метров); after one the accusative is the noun's own accusative singular (одну минуту). In the
other four cases the noun takes the count's case, singular after a count ending in one and
plural otherwise (одного метра, пяти метров, к двадцати пяти участникам). An adjective before
the noun takes the count's case, singular after a count ending in one and plural otherwise, so
after two to four it is plural where the noun is singular (два квадратных метра). After a decimal
or a fraction the unit stands in the genitive singular whatever case the number takes
(восемнадцать целых восемь десятых квадратного километра, больше одной тридцатой секунды): the
number ends in a part, and a part counts its unit in the genitive singular. A number read digit by
digit - more than eighteen digits, or an integer with a leading zero - takes its unit in the
genitive plural with the unit's adjective and tail (ноль один два три квадратных километров,
километров в час), a minus before it still read; a decimal with a leading zero reads as a decimal
(01,5 кг as полтора килограмма), unless it follows a digit and a dot as a dotted chain's tail
(21.07.05). A decimal reads as two counted feminine parts -
целая after the whole part, the denominator its digit count names (десятая, сотая, тысячная, up
to миллионная) after the fraction digits - each in the number's case, singular after a count
ending in one and plural otherwise; the plural's nominative and accusative is the genitive form,
as Russian says it (одна целая, две целых пять десятых, двух целых пяти десятых). A fraction of
more than six digits reads as the whole part, запятая, and the digits one by one.

A half, a quarter or a third before a unit abbreviation reads as a word, as Russian says it. A
half - the one-half sign, 1/2 or 0,5 - is пол joined to the unit's genitive singular, with a
hyphen before л or a vowel (полчаса, пол-литра), and полу joined to the unit's singular of an
oblique case (около получаса). A quarter or a third is четверть or треть, a count of them три
четверти or две трети, in the number's case, with the unit after it in the genitive singular
(четверть часа, около трёх четвертей тонны). A whole and a half - N,5 or N 1/2 - reads полтора /
полторы with the unit counted as after two, or полутора in an oblique case with the unit in that
case's plural (полтора килограмма, около полутора тонн, в полутора километрах); any other whole
reads N с половиной with the unit counted by N (пять с половиной тонн). A whole ending in one,
other than one itself (21,5), keeps целых. A scale word takes only the whole-and-a-half readings
(полтора миллиона, ноль целых пять десятых миллиона), and a unit with an adjective takes no пол.
A whole and a half, N,5 or N.5 with N one or more, before a counted noun written out reads as
before its unit, the noun re-read in the number's case (полтора года, два с половиной часа, около
полутора метров); кв. between a number and a unit is квадратный.

A half, a quarter or a third with no whole part, before a lowercase word of three letters or more
one space after it, reads as a word too, the word kept as written: a quarter or a third in the
number's case (четверть века, три четверти часа, около двух третей длины), a half as пол joined to
the word in the nominative or accusative only, and only to a word in -а, -я, -и or -ы that is no
adjective, reflexive verb or plural past-tense verb (полстакана, пол-литра, на полдлины). A half
in an oblique case, a word with an abbreviation's dot, another fraction, финала, часть and доля
keep the fraction (около одной второй стакана, одна четвёртая финала, одна четвёртая часть, три
пятых голосов). A fraction that closes a comma list of fractions keeps the plain reading (поезда
одна вторая, три четвёртых).

An amount in a currency with a minor unit - рубль and гривна with копейка, доллар and евро with
цент, фунт with пенни - reads a fraction of one or two digits, after a comma or a dot, as the
minor unit, one digit as tens (0,5 руб. as пятьдесят копеек, $1.50 as один доллар пятьдесят
центов). Each half is counted in the number's case (около двух рублей пятидесяти копеек). A zero
whole drops the currency, a zero fraction drops the minor unit, and three fraction digits or more
read as a decimal. A scale word after the amount, abbreviated or written out, counts the amount
instead, and the currency follows the scale word in the genitive plural wherever its sign stands
(2,2$ млрд as две целых две десятых миллиарда долларов); a currency written out again after the
scale word is read once.

### A distance after в places something {#tts-ru-placing-distance}

в before a count governs the accusative (длиной в пять километров), except where the count and
its unit are a distance that places something: when the word after the unit is от, к, ко, на, по,
выше, ниже, вверх, вниз, a compass prefix (юго-, северо-) or a comparative in -нее, the count and
unit read in the prepositional (в шестнадцати километрах к северу, в одиннадцати километрах
юго-восточнее деревни). The test reads the word after the unit, because the words before it are
the same in both senses.

### When a Russian number reads as a year {#tts-ru-year}

A four-digit number from 1000 to 2200 reads as a year ordinal in three shapes; a three-digit
number does only before году, written out or as the mark г. read as году (в девятьсот восемьдесят
восьмом году, к 988 г.). Before a year noun (год, года, году, годом, годы, годов, годам, годами,
годах) it takes the form the noun asks for (за две тысячи четырнадцатый год); году is the dative
after по and after a word the government table maps to the dative (к, согласно, благодаря), the
prepositional after в. Before г. or гг. it takes the form of the case its preposition fixes - the
genitive with no preposition, the accusative after по - and the mark reads as that case of год -
of годы for гг. or a range (в 2004 г. reads в две тысячи четвёртом году). A second year after a
dash, и, по, до or на - four digits, or two after a dash - makes a range: a dash or и gives both
halves the noun's form; по, до or на give the first half its own preposition's form and the
second the noun's (с тысяча девятьсот пятьдесят шестого по тысяча девятьсот девяносто девятый
год). A slash joins a later second year, four digits or two, as a dash does (в тысяча пятьсот
пятьдесят четвёртом - пятьдесят пятом году) and two days before a month (десятого - одиннадцатого
января); a slash pair with no year noun and no preposition stays cardinal. With no noun after it,
a four-digit year after в, во, с, со, до, после, от, к, ко, по or на is a year only where nothing
it could count follows: the end of the text, a punctuation mark, a dash, a capital, or - after в -
a preposition, a conjunction or a pronoun (в две тысячи втором Робредо, в тысяча девятьсот
семьдесят седьмом в сумме). Anything else is a count and stays a cardinal (за два года), as does a
year with no preposition and no noun (Толстой (1828 - 1910)).
A four-digit year right after a month name in any case, with no year noun after it and no second
year, reads as the genitive ordinal (в июле две тысячи восьмого, с мая тысяча девятьсот
семнадцатого по июнь тысяча девятьсот восемнадцатого года).

### An ordinal suffix that leaves the form open {#tts-ru-ordinal-suffix}

The one-letter suffixes -е, -й and -м each spell more than one form of an ordinal, and the words
around the number pick one (-я, -ю and -х spell one form each and read from the suffix table): -е
is the plural before a word in -ы or -и (пятидесятые годы) and the neuter otherwise (сто
восемнадцатое место); -й is the feminine oblique before a word with a feminine oblique ending -
an adjective in -ой or -ей hands the choice to the noun after it (пятой армии, пятой танковой
армией) - and the masculine nominative otherwise; -м is the dative plural after к (к шестидесятым
годам), the instrumental after с, after a linking verb or before an adjective in -ым or -им (с
четыреста девяносто восьмым гаубичным полком, стал четвёртым), and the prepositional otherwise. A
suffix of two letters or more reads from the suffix table alone. во before a digit joined by a
hyphen, в before 3 to 9 joined so, and во before 1 or 2 and a comma enumerate, read as the
genitive plural ordinal (во-первых, в-третьих).

### What reads as a telephone or book number {#tts-ru-phone}

Telephone and book numbers are read before dates and plain numbers, so their shapes keep clear
of a date's. A run reads in groups when it is three or more digit groups joined by single
hyphens, seven digits or more, not shaped as a date or a list of years (dd-mm-yyyy, yyyy-mm-dd,
four digits first with a one-digit group after them, or two four-digit groups not opening with a
zero); a + with its country code, or an area code in parentheses, before hyphenated groups - three
groups or more counting the codes, seven digits or more (+7 (495) 123-45-67); or groups after ISBN
or ISSN holding eight digits or more (ISSN 1313-2970). Each group reads on its own, a comma
between groups for the pause, in chunks: a zero at the head of what is left reads as ноль,
otherwise a chunk is three digits from the left, or two pairs when four digits are left (0516 as
ноль пятьсот шестнадцать, 98472 as девятьсот восемьдесят четыре семьдесят два, 1005 as десять
ноль пять). A decimal whose whole part is two digits with a leading zero, after a number and a
dot, is a code's tail and reads digit by digit (специальность ноль семь. ноль ноль ноль два).

### A century or a Roman numeral reads as an ordinal only where something numbers it {#tts-ru-roman}

Latin capitals standing as a word read as an ordinal only when written the canonical way (XIV,
never XIIII) and something around them numbers them: an ordinal suffix gives the form the suffix
spells, as after an Arabic numeral (в XX-м веке as в двадцатом веке); a century noun or mark after
them gives the ordinal its form by the year rules (в XIX в. as в девятнадцатом веке, XIX в. alone
as девятнадцатого века), a range of two included (в шестнадцатом - семнадцатом веках), and so
does a century noun that numbers the numeral across a dash and a word (в конце девятнадцатого -
начале двадцатого века); a section noun before them gives its gender (Глава вторая); a
capitalized name before them gives its case by its ending (Петра первого, Екатерины второй). One
gate serves the suffix, section-noun and name readings and the standalone cardinal: the numeral
is spelled with I, V and X alone, which keeps it under forty, so Latin capitals such as DC, C, XL
or CV after a capitalized word stay as written; a century is at most twenty-one, so XXV веке stays
as written too. With none of these a numeral reads as a cardinal only at two letters or more
(Выпуск (XV - XVI) as пятнадцать - шестнадцать), so the English I and a capitalized Latin word
such as MIX stay as written.

An Arabic number of one or two digits takes the century readings too, through the same code (в
девятнадцатом веке; 19 в. as девятнадцатого века), unless it is past twenty-one, closes a range of
other numbers, has a capital mark after it (5 В. is volts), or counts the noun: a plural noun with
no range (около четырёх веков), or a genitive singular after two to four in the nominative or
accusative (на два века старше) with no period word before it and no н. э. after it. A century
noun numbers the numeral before it across a word after a dash, or across начале, конце, середине
or половине (or their genitives) with no dash (в конце восемнадцатого начале девятнадцатого
века).

### A number joined to a word reads as one word {#tts-ru-compound}

A number up to 9999 joined by a hyphen to a lowercase Cyrillic word of four letters or more, by a
space to a лет- word (летие, летия, летию, летием, летии, летний in all its forms), or by a space
to an adjective whose stem is a counted noun's (процентный, градусный, кратный, часовой, пушечный
and the rest: в сорокаградусную жару), reads as one word: the number's genitive joined to the word
as written - одно for one, девяносто for ninety, сто for a hundred, тысяче for the thousands, a
compound number joining its parts (двадцатипятилетний, сорокалетие, стопятидесятилетие,
двухтысячелетие) - with ъ between a part ending in a consonant and е, ё, ю or я
(четырёхъядерный). A number after a comma, a dot, a slash or a hyphen that follows a letter or a
digit, a number with a leading zero, a capitalized word, and a tail of three letters or fewer (an
ordinal suffix, 5-ти) are left to the other passes.

### A bare number before the noun it numbers {#tts-ru-numbered-noun}

A bare number of one to three digits before a noun from one of two tables reads as the ordinal in
that noun's case and gender. A numbered noun (год, курс, этаж, место, глава, серия, часть and the
rest) after в, во or на in its prepositional singular reads so for any count, one included (на
восемьдесят первом году), except after на where the count governs it (на две части). Any table
noun - the counted nouns add день, раз, план, уровень, рука, точка, сторона, порядок - after a
count not ending in one reads so when it is a singular form other than the one the count
requires, in a case the word before the number allows (уже третий день, до шестого курса, во
второй раз, пятого дня пути); two to four before a genitive singular with no preposition stay a
count (три этажа), and so does дня after a preposition (в пять дня). With no table noun, a
singular adjective of five letters or more whose noun agrees reads the ordinal in the form its
ending spells (победитель второго отборочного поединка, во время второй мировой войны), never in a
case the preposition cannot take (в четыре новая серия). No number after a minus, a digit, a Latin
letter or a table noun's form, no range, and no noun before a placing word, назад or an
abbreviation's dot reads so.

Any dictionary noun after a count not ending in one reads so in a singular form other than the
count's own, in a case the word before allows (на шестую войну, уже второе дело, к пятой статье);
a clock genitive (утра, вечера, ночи; дня after a preposition), a thousand, a plural determiner
before it, a one-letter capital label, a hyphen after the noun, a dot, a dash or a capital stay
counts. A dictionary form that is also a verb's past in -ла or -ли is the verb (в доме семь
жила). Where the nominative is the accusative, a first-person verb, an infinitive, an imperative
or a past tense not feminine singular before the number makes it the object (пилю вторую часть;
вышла вторая часть).

A singular determiner before one to three digits - мой, твой, свой, наш, ваш, каждый in all forms,
этот, эта, эту, этой - makes it the ordinal in the determiner's form (каждый двадцатый, своей
второй половинки); a unit or a year count after it keeps the count. Numbers joined by и, или or a
comma and a space take the ordinal the last would; a plural prepositional or a masculine
nominative plural numbers them one by one (на первом и втором этажах, первый и второй курсы), and
the count's own form keeps counts (два или три раза). A number between a noun and the genitive
ранга, класса, разряда, степени or категории reads as the genitive ordinal, one included (капитана
первого ранга); a noun is a form the dictionary or a table holds, no verb's or adjective's, or a
capitalized word inside a sentence.

### An approximation puts its noun before the number {#tts-ru-inverted-approximation}

A unit or a noun of time or way (день, неделя, месяц, шаг, раз) in the genitive or the
prepositional plural before the number, or before в, через, на, за or с before it, gives the
number its gender and its case - the prepositional plural's own, else the preposition's between,
else the governor's of the noun (часа два, минуты через две, метрах в двадцати); a number before
the noun, as digits or already read, a noun the number counts after it, лет, a scale word or an
indeclinable unit end the reading.
