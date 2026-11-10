options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_textnorm
require dasllama/dasllama_unicode
require strings
require _tts_corpus

// TTS text normalization: the number/ordinal/year/decimal readers, the twelve upstream
// normalizer defects as failing-first cases, the codepoint codec every pass counts through,
// and the 200-sentence corpus against the reference normalizer's output (rows the fixes change
// carry their own expectation).

[test(description = "The English number readers give exact words for cardinals up to billions and negatives, decimals digit by digit, ordinals, and years read as pairs, hundreds or full numbers")]
def test_number_words(t : T?) {
    t |> run("cardinals") @(t : T?) {
        t |> equal(number_to_words(0l), "zero")
        t |> equal(number_to_words(1l), "one")
        t |> equal(number_to_words(12l), "twelve")
        t |> equal(number_to_words(20l), "twenty")
        t |> equal(number_to_words(99l), "ninety-nine")
        t |> equal(number_to_words(100l), "one hundred")
        t |> equal(number_to_words(1000l), "one thousand")
        t |> equal(number_to_words(1200l), "twelve hundred")
        t |> equal(number_to_words(2500l), "two thousand five hundred")
        t |> equal(number_to_words(15000l), "fifteen thousand")
        t |> equal(number_to_words(1000000l), "one million")
        t |> equal(number_to_words(-42l), "negative forty-two")
        t |> equal(number_to_words(999999999l), "nine hundred ninety-nine million nine hundred ninety-nine thousand nine hundred ninety-nine")
        t |> equal(number_to_words(7900000000l), "seven billion nine hundred million")
    }
    t |> run("decimals read digit by digit") @(t : T?) {
        t |> equal(float_to_words("3.14"), "three point one four")
        t |> equal(float_to_words("-0.5"), "negative zero point five")
        t |> equal(float_to_words("3.10"), "three point one zero")
        t |> equal(float_to_words("1.007"), "one point zero zero seven")
        t |> equal(float_to_words(".5"), "zero point five")
    }
    t |> run("ordinals") @(t : T?) {
        t |> equal(ordinal_words(1l), "first")
        t |> equal(ordinal_words(2l), "second")
        t |> equal(ordinal_words(3l), "third")
        t |> equal(ordinal_words(4l), "fourth")
        t |> equal(ordinal_words(5l), "fifth")
        t |> equal(ordinal_words(8l), "eighth")
        t |> equal(ordinal_words(9l), "ninth")
        t |> equal(ordinal_words(12l), "twelfth")
        t |> equal(ordinal_words(20l), "twentieth")
        t |> equal(ordinal_words(21l), "twenty-first")
        t |> equal(ordinal_words(33l), "thirty-third")
        t |> equal(ordinal_words(42l), "forty-second")
        t |> equal(ordinal_words(100l), "one hundredth")
        t |> equal(ordinal_words(1000l), "one thousandth")
    }
    t |> run("years") @(t : T?) {
        t |> equal(year_to_words(1900l), "nineteen hundred")
        t |> equal(year_to_words(1999l), "nineteen ninety-nine")
        t |> equal(year_to_words(2000l), "two thousand")
        t |> equal(year_to_words(2005l), "two thousand five")
        t |> equal(year_to_words(2021l), "twenty twenty-one")
        t |> equal(year_to_words(1850l), "one thousand eight hundred fifty")
        t |> equal(number_or_year_to_words("1,000"), "one thousand")
        t |> equal(number_or_year_to_words("1984"), "nineteen eighty-four")
        t |> equal(number_or_year_to_words("1.50"), "one point five zero")
    }
}

[test(description = "`normalize_text` reproduces the reference normalizer's own examples exactly: money, percentages, ordinals, clock times, ranges, versions, HTML, URLs and emails, abbreviations, dates and whitespace")]
def test_upstream_pipeline_shapes(t : T?) {
    t |> run("the reference normalizer's own examples") @(t : T?) {
        t |> equal(normalize_text("There are 1200 students and 42 teachers."), "There are twelve hundred students and forty-two teachers.")
        t |> equal(normalize_text("The project costs $1,000,000 and took 365 days."), "The project costs one million dollars and took three hundred sixty-five days.")
        t |> equal(normalize_text("Temperature dropped to -5 degrees overnight."), "Temperature dropped to negative five degrees overnight.")
        t |> equal(normalize_text("Pi is approximately 3.14159."), "Pi is approximately three point one four one five nine.")
        t |> equal(normalize_text("The population is 7,900,000,000."), "The population is seven billion nine hundred million.")
        t |> equal(normalize_text("A coffee costs $4.99 here."), "A coffee costs four dollars and ninety-nine cents here.")
        t |> equal(normalize_text("Rent is €1,200 per month."), "Rent is twelve hundred euros per month.")   // nolint:STYLE039
        t |> equal(normalize_text("The salary was $85K per year."), "The salary was eighty-five thousand dollars per year.")
        t |> equal(normalize_text("We raised $2.5M in seed funding."), "We raised two point five million dollars in seed funding.")
        t |> equal(normalize_text("Inflation rose by 3.5% last quarter."), "Inflation rose by three point five percent last quarter.")
        t |> equal(normalize_text("Stocks fell -2% today."), "Stocks fell negative two percent today.")
        t |> equal(normalize_text("She finished 1st, he came 2nd, I was 3rd."), "She finished first, he came second, I was third.")
        t |> equal(normalize_text("It's the 21st century and the 100th anniversary."), "It's the twenty-first century and the one hundredth anniversary.")
        t |> equal(normalize_text("The meeting starts at 3:30pm."), "The meeting starts at three thirty p m.")
        t |> equal(normalize_text("Departure at 14:00."), "Departure at fourteen.")
        t |> equal(normalize_text("Alarm set for 9:05 AM."), "Alarm set for nine oh five a m.")
        t |> equal(normalize_text("Read pages 10-20 for homework."), "Read pages ten to twenty for homework.")
        t |> equal(normalize_text("The war lasted from 2020-2024."), "The war lasted from twenty twenty to twenty twenty-four.")
        t |> equal(normalize_text("gpt-3 is pretty sick."), "gpt three is pretty sick.")
        t |> equal(normalize_text("They upgraded to GPT-3.5 last month."), "They upgraded to GPT three point five last month.")
        t |> equal(normalize_text("Requires Python-3.10 or higher."), "Requires Python three point ten or higher.")
        t |> equal(normalize_text("Both CUDA-11 and CUDA-12 are supported."), "Both CUDA eleven and CUDA twelve are supported.")
        t |> equal(normalize_text("Running v2.1.0 now."), "Running v two point one point zero now.")
        t |> equal(normalize_text("<b>Hello</b> World! It's a great day."), "Hello World! It's a great day.")
        t |> equal(normalize_text("Visit https://example.com or email hello@example.com."), "Visit e x a m p l e dot c o m or email h e l l o at e x a m p l e dot c o m.")
        t |> equal(normalize_text("Dr. Smith and Prof. Jones, see Fig. 3 and pp. 10-12."), "Doctor Smith and Professor Jones, see Figure three and pages ten to twelve.")
        t |> equal(normalize_text("Smith et al. found it."), "Smith et al found it.")
        t |> equal(normalize_text("on May 5th, 2021 and Sept. 2020"), "on May fifth, twenty twenty-one and September twenty twenty")
        t |> equal(normalize_text("On Jan 5 2020 it rained."), "On January fifth, twenty twenty it rained.")
        t |> equal(normalize_text("The result — 42 — surprised everyone."), "The result — forty-two — surprised everyone.")   // nolint:STYLE039
        t |> equal(normalize_text("He paused... then spoke."), "He paused... then spoke.")
        t |> equal(normalize_text("  spaced   out  "), "spaced out")
    }
}

[test(description = "`normalize_text` gives the corrected reading for each upstream normalizer defect: bare commas, h:mm:ss, fractions, glued units, pence, degrees, decades, apostrophes, sentence-final versions, -ieth ordinals, dollar plurals, p.m., glued digits")]
def test_upstream_defects_fixed(t : T?) {
    t |> run("a bare comma is not a number (etc.,)") @(t : T?) {
        t |> equal(normalize_text("Apples, pears, etc., and more."), "Apples, pears, etc., and more.")
        t |> equal(normalize_text("I have 5, you have 6."), "I have five, you have six.")
    }
    t |> run("h:mm:ss keeps the space after it") @(t : T?) {
        t |> equal(normalize_text("at 3:12:45 today"), "at three twelve and forty-five seconds today")
        t |> equal(normalize_text("at 3:12:45pm today"), "at three twelve and forty-five seconds p m today")
    }
    t |> run("fractions are read as fractions") @(t : T?) {
        t |> equal(normalize_text("Cut the recipe in 1/2 cup."), "Cut the recipe in one half cup.")
        t |> equal(normalize_text("Add 3/4 cup of sugar and 1/4 teaspoon of salt."), "Add three quarters cup of sugar and one quarter teaspoon of salt.")
        t |> equal(normalize_text("The pipe is 5/8 inch."), "The pipe is five eighths inch.")
        t |> equal(normalize_text("Open 24/7 since 9/11."), "Open twenty-four seven since nine eleven.")
    }
    t |> run("units glued to numbers are read") @(t : T?) {
        t |> equal(normalize_text("a 5GB file at 3.6GHz in 12ms"), "a five gigabytes file at three point six gigahertz in twelve milliseconds")
        t |> equal(normalize_text("The trail is 42km long and 500 kg heavy."), "The trail is forty-two kilometers long and five hundred kilograms heavy.")
        t |> equal(normalize_text("The measurement was 0.75 mm plus or minus 0.02 mm."), "The measurement was zero point seven five millimeters plus or minus zero point zero two millimeters.")
        t |> equal(normalize_text("A 12 cm ruler, 5 lbs of flour, 8 oz of milk, a 6 ft fence."), "A twelve centimeters ruler, five pounds of flour, eight ounces of milk, a six feet fence.")
    }
    t |> run("pounds carry pence") @(t : T?) {
        t |> equal(normalize_text("We shipped 250 units at £14.50 each."), "We shipped two hundred fifty units at fourteen pounds and fifty pence each.")   // nolint:STYLE039
        t |> equal(normalize_text("It cost £3.01 and $3.01."), "It cost three pounds and one penny and three dollars and one cent.")   // nolint:STYLE039
    }
    t |> run("degrees survive") @(t : T?) {
        t |> equal(normalize_text("Water boils at 100°C."), "Water boils at one hundred degrees Celsius.")   // nolint:STYLE039
        t |> equal(normalize_text("It was 72°F outside."), "It was seventy-two degrees Fahrenheit outside.")   // nolint:STYLE039
    }
    t |> run("decades") @(t : T?) {
        t |> equal(normalize_text("The 80s music scene."), "The eighties music scene.")
        t |> equal(normalize_text("She grew up on 1990s grunge."), "She grew up on nineteen nineties grunge.")
        t |> equal(normalize_text("The 2000s brought social media."), "The two thousands brought social media.")
        t |> equal(normalize_text("AI took off in the 2020s."), "AI took off in the twenty twenties.")
        t |> equal(normalize_text("Born in the '90s."), "Born in the nineties.")
    }
    t |> run("apostrophes stay") @(t : T?) {
        t |> equal(normalize_text("The soft cushion broke the man's fall."), "The soft cushion broke the man's fall.")
        t |> equal(normalize_text("I don't know, they've left."), "I don't know, they've left.")
        t |> equal(normalize_text("It’s the man’s fall."), "It's the man's fall.")   // nolint:STYLE039
    }
    t |> run("a version ending a sentence does not crash") @(t : T?) {
        t |> equal(normalize_text("We upgraded to GPT-3."), "We upgraded to GPT three.")
        t |> equal(normalize_text("Try v1.2.3."), "Try v one point two point three.")
    }
    t |> run("twentieth, not twentyth") @(t : T?) {
        t |> equal(normalize_text("the 20th time"), "the twentieth time")
        t |> equal(normalize_text("the 30th and 90th"), "the thirtieth and ninetieth")
    }
    t |> run("one dollar, two dollars") @(t : T?) {
        t |> equal(normalize_text("It costs $1.50 now."), "It costs one dollar and fifty cents now.")
        t |> equal(normalize_text("It costs $1 now, or $2.01."), "It costs one dollar now, or two dollars and one cent.")
    }
    t |> run("p.m. is not a page") @(t : T?) {
        t |> equal(normalize_text("at 3 p.m. today"), "at three p.m. today")
        t |> equal(normalize_text("see p. 12"), "see page twelve")
    }
    t |> run("a digit run glued to letters is left alone") @(t : T?) {
        t |> equal(normalize_text("abc123 def"), "abc123 def")
        t |> equal(normalize_text("IPv6 and mp3"), "IPv6 and mp3")
    }
}

[test(description = "A digit run past 18 digits reads digit by digit while 18 digits read as a cardinal, `digit_string_value` holds values past 2^31, and exactly one of a unit reads singular, other counts plural")]
def test_number_width_and_units(t : T?) {
    t |> run("a digit run past the accumulator's width reads digit by digit") @(t : T?) {
        t |> equal(normalize_text("Call 12345678901234567890 now."),
            "Call one two three four five six seven eight nine zero one two three four five six seven eight nine zero now.")
        t |> equal(digit_count("12345678901234567890"), 20)
        t |> equal(normalize_text("Account 123456789012345678 is open."),
            "Account one hundred twenty-three quadrillion four hundred fifty-six trillion seven hundred eighty-nine billion twelve million three hundred forty-five thousand six hundred seventy-eight is open.",
            "eighteen digits still read as a cardinal")
        t |> equal(digits_to_words("407"), "four zero seven")
        t |> equal(digit_string_value("1,024"), 1024l)
        t |> equal(digit_string_value("3000000000"), 3000000000l, "past 2^31, where to_int answers zero")
    }
    t |> run("exactly one of a unit is singular") @(t : T?) {
        t |> equal(normalize_text("A 1 cm gap."), "A one centimeter gap.")
        t |> equal(normalize_text("A 1.5 cm gap."), "A one point five centimeters gap.")
        t |> equal(normalize_text("It weighs 1 kg and runs 1 km."), "It weighs one kilogram and runs one kilometer.")
        t |> equal(normalize_text("A 1 ft board and a 6 ft fence."), "A one foot board and a six feet fence.")
        t |> equal(normalize_text("At 1 Hz, then 60 Hz."), "At one hertz, then sixty hertz.")
        t |> equal(normalize_text("Chilled to 1°C, boiled at 100°C."), "Chilled to one degree Celsius, boiled at one hundred degrees Celsius.")   // nolint:STYLE039
        t |> equal(normalize_text("Idling at 1 rpm."), "Idling at one revolution per minute.")
        t |> equal(normalize_text("A 1 mW laser and a 5 mW one."), "A one megawatt laser and a five megawatts one.",
            "mw stays megawatts: the key is lowercased, and a milliwatt is not what the table carries")
    }
}

[test(description = "`normalize_text` spells www and scheme URLs as the reference does, reads ordinal edges up to millionth, and wraps a 24-hour time past noon only when a meridiem follows")]
def test_normalizer_arms(t : T?) {
    t |> run("a www address spells out without a scheme") @(t : T?) {
        // the arm prepends "www dot " and then spells the whole thing, so the "dot" it added
        // spells too - which is what the reference normalizer says
        t |> equal(normalize_text("Go to www.abc.com now."), "Go to w w w d o t a b c dot c o m now.")
        t |> equal(normalize_text("Try http://a.co/x?y=1&z"), "Try a dot c o slash x question mark y equals one and z")
    }
    t |> run("the ordinal reader's edges") @(t : T?) {
        t |> equal(ordinal_words(11l), "eleventh")
        t |> equal(ordinal_words(13l), "thirteenth")
        t |> equal(ordinal_words(40l), "fortieth")
        t |> equal(ordinal_words(1000000l), "one millionth")
        t |> equal(normalize_text("the 1000000th visitor"), "the one millionth visitor")
    }
    t |> run("a twelve-hour clock wraps past noon") @(t : T?) {
        t |> equal(normalize_text("It ended at 13:45 p.m."), "It ended at one forty-five p m.")
        t |> equal(normalize_text("It ended at 13:45."), "It ended at thirteen forty-five.", "no meridiem, no wrap")
        t |> equal(normalize_text("Set for 12:00 pm."), "Set for twelve p m.")
    }
}

[test(description = "`utf8_to_cpts` passes a malformed lead byte through as itself and still decodes the next byte, well-formed codepoints round-trip, and `cpts_to_utf8` maps values outside 0..U+10FFFF to U+FFFD")]
def test_codepoint_codec(t : T?) {
    t |> run("a malformed lead passes through as its own byte") @(t : T?) {
        var inscope truncated <- utf8_to_cpts("a{cpt_to_utf8(195)}")   // 0xC3 with nothing after it
        t |> equal(length(truncated), 2)
        t |> equal(truncated[0], 'a')
        t |> equal(truncated[1], 195, "the lead byte itself, not its five payload bits")
        var inscope stray <- utf8_to_cpts(string([uint8(224), uint8('A')]))   // a three-byte lead, then 'A'
        t |> equal(length(stray), 2)
        t |> equal(stray[0], 224)
        t |> equal(stray[1], 'A', "the byte after a malformed lead is still decoded")
    }
    t |> run("well-formed sequences still round-trip") @(t : T?) {
        for (cp in ['a', 233, 8212, 128512]) {   // ascii, latin-1, em dash, an astral emoji
            var inscope back <- utf8_to_cpts(cpt_to_utf8(cp))
            t |> equal(length(back), 1, "one codepoint for {cp}")
            t |> equal(back[0], cp)
        }
    }
    t |> run("the encoder clamps outside the unicode range") @(t : T?) {
        let replacement = cpt_to_utf8(65533)   // U+FFFD
        t |> equal(cpts_to_utf8([-1]), replacement, "a negative codepoint")
        t |> equal(cpts_to_utf8([1114112]), replacement, "past U+10FFFF")
        t |> equal(cpts_to_utf8([1114111]), cpt_to_utf8(1114111), "U+10FFFF itself is in range")
        t |> equal(cpts_to_utf8([128512]), cpt_to_utf8(128512), "the astral arm is untouched")
        t |> equal(length(cpts_to_utf8([128512])), 4, "four bytes for an astral codepoint")
    }
}

[test(description = "`normalize_text` matches the reference normalizer's output, or the fixture's corrected form where one is carried, on all 200 corpus sentences, and is idempotent on each"), arch(at = "../ARCHITECTURE_TTS.md#tts-rig-scoring")]
def test_corpus(t : T?) {
    t |> run("200 sentences match the reference normalizer") @(t : T?) {
        let rows <- load_tts_corpus()
        // where the reference normalizer's reading is wrong, the fixture carries the corrected
        // spoken form (the minter's table) and that is the reading
        var mismatches = 0
        for (r in rows) {
            let got = normalize_text(r.text)
            let want = empty(r.expected) ? r.norm : r.expected
            if (got != want) {
                mismatches++
                t |> failure("{r.id}: got '{got}', want '{want}'")
            }
        }
        t |> equal(mismatches, 0, "mismatching sentences")
    }
    t |> run("normalization is idempotent") @(t : T?) {
        let rows <- load_tts_corpus()
        for (r in rows) {
            let once = normalize_text(r.text)
            t |> equal(normalize_text(once), once, r.id)
        }
    }
}

[test(description = "`cpt_is_letter`, `cpt_is_number` and `cpt_is_whitespace` classify ASCII and non-ASCII codepoints (A-umlaut, one-half, ideographic space) as the upstream flags do, and a multi-byte UTF-8 word round-trips")]
def test_unicode(t : T?) {
    // Category classification must match the upstream codepoint-flag classification.
    t |> run("letters") @(t : T?) {
        t |> success(cpt_is_letter(65))      // 'A'
        t |> success(cpt_is_letter(122))     // 'z'
        t |> success(cpt_is_letter(196))     // 'Ä' U+00C4
        t |> success(!cpt_is_letter(48))     // '0'
        t |> success(!cpt_is_letter(32))     // space
        t |> success(!cpt_is_letter(33))     // '!'
    }
    t |> run("numbers") @(t : T?) {
        t |> success(cpt_is_number(48))      // '0'
        t |> success(cpt_is_number(57))      // '9'
        t |> success(cpt_is_number(189))     // '½' U+00BD (category No)
        t |> success(!cpt_is_number(65))     // 'A'
        t |> success(!cpt_is_number(32))     // space
    }
    t |> run("whitespace") @(t : T?) {
        t |> success(cpt_is_whitespace(32))  // space
        t |> success(cpt_is_whitespace(9))   // tab
        t |> success(cpt_is_whitespace(10))  // newline
        t |> success(cpt_is_whitespace(13))  // CR
        t |> success(cpt_is_whitespace(12288)) // U+3000 ideographic space
        t |> success(!cpt_is_whitespace(65)) // 'A'
        t |> success(!cpt_is_whitespace(48)) // '0'
    }
    // UTF-8 codec round-trips, including a multi-byte codepoint.
    t |> run("utf8 round-trip") @(t : T?) {
        let cpts <- utf8_to_cpts("Äpfel")   // nolint:STYLE039 the multi-byte codepoint IS the fixture
        t |> success(length(cpts) == 5)
        t |> success(cpts[0] == 196)         // Ä
        t |> success(cpts[1] == 112)         // p
        let rebuilt = build_string() $(var w) {
            for (c in cpts) {
                w |> write(cpt_to_utf8(c))
            }
        }
        t |> success(rebuilt == "Äpfel")   // nolint:STYLE039 the multi-byte codepoint IS the fixture
    }
}
