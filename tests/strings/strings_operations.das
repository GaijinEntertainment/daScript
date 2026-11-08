options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/strings_boost
require daslib/faker
require daslib/fuzzer
require daslib/regex

[test(description = "peek_data and modify_data survive fuzzed strings")]
def test_string_as_array(t : T?) {
    peek_data("123") $(arr) {
        t |> equal(length(arr), 3)
        t |> equal(arr[0], uint8('1'))
        t |> equal(arr[1], uint8('2'))
        t |> equal(arr[2], uint8('3'))
    }
    t |> run("fuzz peek_data") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let st = fake |> any_string
            peek_data(st) $(arr) {
                t |> equal(length(st), length(arr))
            }
        }
    }

    let SST = "123"
    let SSTR = modify_data(SST) $(arr) {
        t |> equal(length(arr), 3)
        t |> equal(arr[0], uint8('1'))
        t |> equal(arr[1], uint8('2'))
        t |> equal(arr[2], uint8('3'))
        arr[0] = uint8('4')
        arr[1] = uint8('5')
        arr[2] = uint8('6')
    }
    t |> equal(SSTR, "456")
    t |> run("fuzz modify_data") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let st = fake |> any_string
            modify_data(st) $(arr) {
                t |> equal(length(st), length(arr))
                for (Ch in arr) {
                    Ch = uint8(fake |> random_int)
                }
            }
        }
    }
}

[test(description = "split and split_by_chars survive fuzzed strings and separators")]
def test_split_and_join(t : T?) {
    let spl <- split("12,34,56", ",")
    t |> equal(length(spl), 3)
    t |> equal(spl[0], "12")
    t |> equal(spl[1], "34")
    t |> equal(spl[2], "56")
    t |> run("fuzz split") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit())
        var fake <- Faker()
        fuzz() {
            let txt = fake |> any_string
            let delim = fake |> any_string
            var foo <- split(txt, delim)
            t |> success(length(foo) <= limit)
            delete foo
        }
    }

    split("12,34,56", ",") $(splb) {
        t |> equal(length(splb), 3)
        t |> equal(splb[0], "12")
        t |> equal(splb[1], "34")
        t |> equal(splb[2], "56")
    }
    t |> run("fuzz split") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit())
        var fake <- Faker()
        fuzz() {
            let txt = fake |> any_string
            let delim = fake |> any_string
            split(txt, delim) $(foo) {
                t |> success(length(foo) <= limit)
            }
        }
    }

    let splbc <- split_by_chars("12,34;56", ",;")
    t |> equal(length(splbc), 3)
    t |> equal(splbc[0], "12")
    t |> equal(splbc[1], "34")
    t |> equal(splbc[2], "56")
    t |> run("fuzz split_by_chars") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit())
        var fake <- Faker()
        fuzz() {
            let txt = fake |> any_string
            let delim = fake |> any_string
            var foo <- split(txt, delim)
            t |> success(length(foo) <= limit)
            delete foo
        }
    }

    split_by_chars("12,34;56", ",;") $(splb) {
        t |> equal(length(splb), 3)
        t |> equal(splb[0], "12")
        t |> equal(splb[1], "34")
        t |> equal(splb[2], "56")
    }
    t |> run("fuzz split_by_chars") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit())
        var fake <- Faker()
        fuzz() {
            let txt = fake |> any_string
            let delim = fake |> any_string
            split_by_chars(txt, delim) $(foo) {
                t |> success(length(foo) <= limit)
            }
        }
    }

    t |> equal(join([ "1", "2", "3"], ","), "1,2,3")
}

[test(description = "join survives fuzzed input and its block forms instantiate")]
def test_join_block_forms(t : T?) {
    t |> run("fuzz join") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit())
        var fake <- Faker()
        fuzz() {
            let elem = fake |> random_uint % 10u
            let arr <- [for (_x in urange(elem)); fake |> any_string]
            t |> accept(join(arr, fake |> any_string))
        }
    }
    t |> run("join block forms instantiate") @@(t : T?) {
        // the block overloads are generics - an uncompilable one is inert until a call
        // site instantiates it (the iterator+block form sat broken exactly this way)
        let arr <- [ 1, 2, 3 ]
        t |> equal(join(arr, "-") $(var writer; elem) { write(writer, elem * 10) }, "10-20-30")
        t |> equal(join(unsafe(each(arr)), "-") $(var writer; elem) { write(writer, elem + 1) }, "2-3-4")
    }
}

[test(description = "join over an iterator formats numeric elements as the array form does, keeps string elements and joins a bare string character by character")]
def test_join_iterator_elements(t : T?) {
    t |> run("numeric elements join as numbers, exactly as the array form does") @@(t : T?) {
        let ints <- [ 1, 2, 3 ]
        t |> equal(join(ints, ","), "1,2,3")
        t |> equal(join(unsafe(each(ints)), ","), "1,2,3")
        let floats <- [ 1.5, 2.5 ]
        t |> equal(join(unsafe(each(floats)), ","), "1.5,2.5")
    }
    t |> run("string elements survive the iterator overload") @@(t : T?) {
        let words <- [ "a", "b" ]
        t |> equal(join(unsafe(each(words)), "-"), "a-b")
    }
    t |> run("a string is still joined character by character") @@(t : T?) {
        t |> equal(join("abc", "-"), "a-b-c")
    }
}


[test(description = "split keeps empty fields between and around separators and join restores the original text")]
def tricky_test_split_and_join(t : T?) {
    let splStr = ",,12,,34,56,,"
    let spl <- split(splStr, ",")
    t |> equal(length(spl), 8)
    t |> equal(spl[0], "")
    t |> equal(spl[1], "")
    t |> equal(spl[2], "12")
    t |> equal(spl[3], "")
    t |> equal(spl[4], "34")
    t |> equal(spl[5], "56")
    t |> equal(spl[6], "")
    t |> equal(spl[7], "")
    t |> equal(join(spl, ","), splStr)


    split(splStr, ",") $(_splb) {
        t |> equal(length(spl), 8)
        t |> equal(spl[0], "")
        t |> equal(spl[1], "")
        t |> equal(spl[2], "12")
        t |> equal(spl[3], "")
        t |> equal(spl[4], "34")
        t |> equal(spl[5], "56")
        t |> equal(spl[6], "")
        t |> equal(spl[7], "")
        t |> equal(join(spl, ","), splStr)
    }

    let splbcStr = ";;12,,34;56,,"
    let splbc <- split_by_chars(splbcStr, ",;")
    t |> equal(length(spl), 8)
    t |> equal(splbc[0], "")
    t |> equal(splbc[1], "")
    t |> equal(splbc[2], "12")
    t |> equal(splbc[3], "")
    t |> equal(splbc[4], "34")
    t |> equal(splbc[5], "56")
    t |> equal(splbc[6], "")
    t |> equal(splbc[7], "")

    split_by_chars(splbcStr, ",;") $(splb) {
        t |> equal(length(spl), 8)
        t |> equal(splb[0], "")
        t |> equal(splb[1], "")
        t |> equal(splb[2], "12")
        t |> equal(splb[3], "")
        t |> equal(splb[4], "34")
        t |> equal(splb[5], "56")
        t |> equal(splb[6], "")
        t |> equal(splb[7], "")
    }
}

[test(description = "string builder write, write_char, write_chars, write_escape_string and format survive fuzzed input")]
def test_builder(t : T?) {
    // TODO: manually test builder

    t |> run("fuzz write") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            build_string() $(writer) {
                let str = fake |> any_string
                writer |> write(str)
            }
        }
    }

    t |> run("fuzz write_char") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            build_string() $(writer) {
                // TODO: implement all different formats?
                let Ch = fake |> random_int
                writer |> write_char(Ch)
            }
        }
    }

    t |> run("fuzz write_chars") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            build_string() $(writer) {
                let Ch = fake |> random_int
                let Count = fake |> random_int % 100
                writer |> write_chars(Ch, Count)
            }
        }
    }

    t |> run("fuzz write_escape_string") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            build_string() $(writer) {
                let str = fake |> any_string
                writer |> write_escape_string(str)
            }
        }
    }

    /*
    t |> run("fuzz format") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            build_string() $(writer) {
                let str = fake |> any_string
                writer |> format(str, 0)
            }
        }
    }
    */
}

[test(description = "charset construction, set_element and is_char_in_charset work and survive fuzzed input")]
def test_charset(t : T?) {
    t |> run("charset") @@(t : T?) {
        var ch : uint[8]
        t |> success(!is_char_in_set('A', ch))
        ch['A' / 32] |= 1u << uint('A' & 31)
        t |> success(is_char_in_set('A', ch))
        t |> equal(set_total(ch), 1u)
        t |> equal(set_element(0, ch), 'A')
    }
    t |> run("fuzz charset") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let ch = fake |> any_set
            var total = 0u
            for (c in ch) {
                var bits = c
                while (bits != 0u) {
                    bits &= bits - 1u
                    total ++
                }
            }
            let stotal = set_total(ch)
            t |> equal(total, stotal)
            var elem = 0
            for (i, c in urange(8), ch) {
                let bits = c
                for (j in urange(32)) {
                    if ((bits & (1u << j)) != 0u) {
                        let Char = int(i * 32u + j)
                        t |> success(is_char_in_set(Char, ch))
                        let EChar = set_element(elem, ch)
                        t |> equal(Char, EChar)
                        elem ++
                    }
                }
            }
        }
    }
    t |> run("fuzz set_element") @@(t : T?) {
        var fake <- Faker()
        var count = 0
        fuzz(1000) {
            let ch = fake |> any_set
            let total = int(set_total(ch))
            let idx = fake |> random_int
            let elem = set_element(idx, ch)
            t |> success(elem == -1 || (elem >= 0 && elem < total))
            count ++
        }
        t |> equal(count, 1000)
    }
    t |> run("fuzz is_char_in_charset") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let ch = fake |> any_set
            let idx = fake |> random_int
            t |> accept(is_char_in_set(idx, ch))
        }
    }
}

[test(description = "repeat, strip variants, chop, slice, reverse, case conversion, escape, unescape and replace survive fuzzed strings")]
def test_modifications(t : T?) { // nolint:STYLE038 one flat pin+fuzz arm per modification entry point under test
    t |> equal(repeat("ab", 3), "ababab")
    t |> run("fuzz repeat") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let count = fake |> random_int % 10
            t |> accept(repeat(str, count))
        }
    }

    t |> equal(strip(" ab "), "ab")
    t |> run("fuzz strip") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            t |> accept(strip(str))
        }
    }

    t |> equal(strip_right(" ab "), " ab")
    t |> run("fuzz strip_right") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            t |> accept(strip_right(str))
        }
    }

    t |> equal(rtrim(" ab "), " ab")
    t |> run("fuzz rtrim") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            t |> accept(rtrim(str))
        }
    }

    t |> equal(strip_left(" ab "), "ab ")
    t |> run("fuzz strip_left") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            t |> accept(strip_left(str))
        }
    }

    t |> equal(chop(" ab ", 1, 2), "ab")
    t |> run("fuzz chop") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit()) * 2
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let start = fake |> random_int % limit
            let len = fake |> random_int % limit
            t |> accept(chop(str, start, len))
        }
    }

    t |> equal(slice(" ab ", 1), "ab ")
    t |> run("fuzz slice") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit()) * 2
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let start = fake |> random_int % limit
            t |> accept(slice(str, start))
        }
    }

    t |> equal(slice(" ab ", 1, 3), "ab")
    t |> run("fuzz slice2") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit()) * 2
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let start = fake |> random_int % limit
            let end = fake |> random_int % limit
            t |> accept(slice(str, start, end))
        }
    }

    t |> equal(reverse("1234"), "4321")
    t |> run("fuzz reverse") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            t |> accept(reverse(str))
        }
    }

    t |> equal(to_upper("abCD"), "ABCD")
    t |> run("fuzz to_upper") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = to_upper(fake |> any_string)
            for (ch in str) {
                t |> success(!(ch >= 'a' && ch <= 'z'))
            }
        }
    }

    t |> equal(to_lower("abCD"), "abcd")
    t |> run("fuzz to_lower") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = to_lower(fake |> any_string)
            for (ch in str) {
                t |> success(!(ch >= 'A' && ch <= 'Z'))
            }
        }
    }

    t |> equal(escape("\n"), "\\n")
    t |> run("fuzz escape") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            t |> accept(escape(str))
        }
    }

    t |> equal(unescape("\\n"), "\n")
    t |> run("fuzz unescape") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            t |> accept(unescape(str))
        }
    }

    t |> equal(replace("1234", "23", "a"), "1a4")
    t |> run("fuzz replace") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let what = fake |> any_string
            let to = fake |> any_string
            t |> accept(replace(str, what, to))
        }
    }
}

[test(description = "length, empty, starts_with and ends_with answer correctly")]
def test_properties(t : T?) {
    t |> run("length") @@(t : T?) {
        t |> equal(length(""), 0)
        t |> equal(length("123"), 3)
        var fake <- Faker()
        let limit = re_gen_get_rep_limit()
        fuzz() {
            let str = fake |> any_string
            let len = str |> length
            t |> success(len >= 0 && len < int(limit))
        }
        fuzz(100) {
            let str = fake |> long_string
            let len = str |> length
            t |> success(len >= 0 && len < int(fake.max_long_string))
        }
    }

    t |> run("empty") @@(t : T?) {
        t |> success(empty(""))
        t |> success(!empty("123"))
        var fake <- Faker()
        let limit = re_gen_get_rep_limit()
        fuzz() {
            let str = fake |> any_string
            t |> accept(str |> empty)
        }
    }

    t |> run("starts_with") @@(t : T?) {
        t |> success(starts_with("123", "12"))
        t |> success(starts_with("123", "123"))
        t |> success(!starts_with("123", "23"))
        t |> success(!starts_with("123", "1234"))
        t |> success(starts_with("123", 1, "23"))
        t |> success(starts_with("123", 1, "23", 2u))
        t |> success(starts_with("123", 1, "2", 1u))
        t |> success(starts_with("123", 2, "3"))
        t |> success(starts_with("123", 2, "3", 2u)) // yes, it's ok
        t |> success(!starts_with("123", 3, "4"))
        t |> success(!starts_with("123", 3, "4", 2u))
        var fake <- Faker()
        let limit = re_gen_get_rep_limit()
        fuzz() {
            let str = fake |> any_string
            let cmp = fake |> any_string
            t |> accept(starts_with(str, cmp))
        }
    }

    t |> run("ends_with") @@(t : T?) {
        t |> success(ends_with("123", "23"))
        t |> success(ends_with("123", "123"))
        t |> success(!ends_with("123", "12"))
        t |> success(!ends_with("123", "0123"))
        var fake <- Faker()
        let limit = re_gen_get_rep_limit()
        fuzz() {
            let str = fake |> any_string
            let cmp = fake |> any_string
            t |> accept(ends_with(str, cmp))
        }
    }
}

[test(description = "replace_multiple substitutes several patterns in one pass")]
def test_replace_multiple(t : T?) {
    t |> run("replace multiple") @@(t : T?) {
        // Test with multiple occurrences of different substrings
        t |> equal("_1_2_1_2", replace_multiple("_foo_bar_foo_bar", [(text = "foo", replacement = "1"), (text = "bar", replacement = "2")]))

        // Test with multiple occurrences of the same substring
        t |> equal("1_1_1", replace_multiple("foo_foo_foo", [(text = "foo", replacement = "1")]))

        // Test with substring as a prefix and a suffix
        t |> equal("1_text_2", replace_multiple("foo_text_bar", [(text = "foo", replacement = "1"), (text = "bar", replacement = "2")]))

        // Test with no matching substring (no replacement)
        t |> equal("hello_world", replace_multiple("hello_world", [(text = "foo", replacement = "1"), (text = "bar", replacement = "2")]))

        // Test with special characters in the substrings
        // we do not support overlap, otherwise 1_2_3 occurs
        t |> equal("1$23", replace_multiple("@#$_*&^_%$", [(text = "@#", replacement = "1"), (text = "_*&^", replacement = "2"), (text = "_%$", replacement = "3")]))

        // Test with an empty source string
        t |> equal("", replace_multiple("", [(text = "foo", replacement = "1"), (text = "bar", replacement = "2")]))

        // Test with an empty array of replacements
        let empty : array<tuple<text : string; replacement : string>>
        t |> equal("hello_world", replace_multiple("hello_world", empty))

        // Test with overlapping replacements
        // we do not support overlap, otherwise 1_23 occurs
        t |> equal("13baz", replace_multiple("foobarbaz", [(text = "foo", replacement = "1"), (text = "oob", replacement = "2"), (text = "bar", replacement = "3")]))

        // Test with multiple replacements in the same substring
        t |> equal("1_2_3", replace_multiple("foo_bar_baz", [(text = "foo", replacement = "1"), (text = "bar", replacement = "2"), (text = "baz", replacement = "3")]))
    }
}

[test(description = "find, find1 and rfind on strings and chars survive fuzzed input")]
def test_search(t : T?) { // nolint:STYLE038 one flat pin+fuzz arm per search entry point under test

    t |> equal(find("123", "23"), 1)
    t |> equal(find("123", "22"), -1)
    t |> run("fuzz find") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let substr = fake |> any_string
            t |> accept(find(str, substr))
        }
    }

    t |> equal(find("123", '2'), 1)
    t |> equal(find("123", '4'), -1)
    t |> run("fuzz find char") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let substr = fake |> random_int % 256
            t |> accept(find(str, substr))
        }
    }

    t |> equal(find("123", "23", 1), 1)
    t |> equal(find("123", "23", 2), -1)
    t |> run("fuzz find1") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit()) * 2
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let substr = fake |> any_string
            let start = fake |> random_int % limit
            t |> accept(find(str, substr, start))
        }
    }

    t |> equal(find("123", '2', 1), 1)
    t |> equal(find("123", '2', 2), -1)
    t |> run("fuzz find1 char") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit()) * 2
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let substr = fake |> random_int % 255
            let start = fake |> random_int % limit
            t |> accept(find(str, substr, start))
        }
    }

    // rfind — reverse find
    t |> equal(rfind("hello/world/foo", "/"), 11)
    t |> equal(rfind("hello/world/foo", "world"), 6)
    t |> equal(rfind("hello", "/"), -1)
    t |> equal(rfind("", "/"), -1)
    t |> equal(rfind("abc", ""), -1)
    t |> equal(rfind("aaa", "a"), 2)
    t |> run("fuzz rfind") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let substr = fake |> any_string
            t |> accept(rfind(str, substr))
        }
    }

    // rfind with start position
    t |> equal(rfind("hello/world/foo", "/", 10), 5)
    t |> equal(rfind("hello/world/foo", "/", 11), 11)
    t |> equal(rfind("hello/world/foo", "/", 4), -1)
    t |> equal(rfind("aaa", "a", 1), 1)
    t |> equal(rfind("aaa", "a", 0), 0)
    t |> run("fuzz rfind1") @@(t : T?) {
        let limit = int(re_gen_get_rep_limit()) * 2
        var fake <- Faker()
        fuzz() {
            let str = fake |> any_string
            let substr = fake |> any_string
            let start = fake |> random_int % limit
            t |> accept(rfind(str, substr, start))
        }
    }
}

[test(description = "character trait predicates answer correctly and survive fuzzed characters")]
def test_traits(t : T?) {
    t |> run("traits") @@(t : T?) {
        t |> success(is_alpha('A'))
        t |> success(!is_alpha('9'))
        t |> success(is_new_line('\n'))
        t |> success(!is_new_line(' '))
        t |> success(is_white_space('\n'))
        t |> success(is_white_space('\f'))
        t |> success(is_white_space(11))     // vertical tab - the char-literal lexer has no \v escape
        t |> success(!is_white_space('#'))
        t |> success(is_number('3'))
        t |> success(!is_number('A'))
    }
    t |> run("traits fuzz is_alpha") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let Ch = fake |> any_char
            t |> accept(is_alpha(Ch))
        }
    }
    t |> run("traits fuzz is_new_line") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let Ch = fake |> any_char
            t |> accept(is_new_line(Ch))
        }
    }
    t |> run("traits fuzz is_white_space") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let Ch = fake |> any_char
            t |> accept(is_white_space(Ch))
        }
    }
    t |> run("traits fuzz is_number") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let Ch = fake |> any_char
            t |> accept(is_number(Ch))
        }
    }
}

// Tests for strings_boost functions that lacked coverage: wide, is_character_at, eq, join overloads

[test(description = "wide pads a string with spaces to a width and returns it unchanged when it is already that wide or wider")]
def test_wide(t : T?) {
    t |> run("basic padding") @@(t : T?) {
        t |> equal(wide("hi", 5), "hi   ")
        t |> equal(wide("hello", 10), "hello     ")
        t |> equal(wide("", 3), "   ")
    }

    t |> run("no padding needed") @@(t : T?) {
        t |> equal(wide("hello", 5), "hello")
        t |> equal(wide("hello", 3), "hello")
        t |> equal(wide("hello", 0), "hello")
        t |> equal(wide("hello", -1), "hello")
    }

    t |> run("exact width") @@(t : T?) {
        t |> equal(wide("abc", 3), "abc")
    }

    t |> run("single char") @@(t : T?) {
        t |> equal(wide("x", 1), "x")
        t |> equal(wide("x", 4), "x   ")
    }
}


[test(description = "is_character_at tests the character at an index for letters, digits and special characters")]
def test_is_character_at(t : T?) {
    t |> run("basic character check") @@(t : T?) {
        let arr <- array<uint8>(uint8('A'), uint8('B'), uint8('C'))
        t |> equal(is_character_at(arr, 0, 'A'), true)
        t |> equal(is_character_at(arr, 1, 'B'), true)
        t |> equal(is_character_at(arr, 2, 'C'), true)
        t |> equal(is_character_at(arr, 0, 'B'), false)
        t |> equal(is_character_at(arr, 1, 'A'), false)
    }

    t |> run("numeric values") @@(t : T?) {
        let arr <- array<uint8>(uint8('0'), uint8('1'), uint8('9'))
        t |> equal(is_character_at(arr, 0, '0'), true)
        t |> equal(is_character_at(arr, 2, '9'), true)
        t |> equal(is_character_at(arr, 0, '1'), false)
    }

    t |> run("special characters") @@(t : T?) {
        let arr <- array<uint8>(uint8('\n'), uint8('\t'), uint8(' '))
        t |> equal(is_character_at(arr, 0, '\n'), true)
        t |> equal(is_character_at(arr, 1, '\t'), true)
        t |> equal(is_character_at(arr, 2, ' '), true)
    }
}


[test(description = "strings compare equal to das_string in both directions and handle empty operands")]
def test_eq(t : T?) {
    t |> run("string eq das_string") @@(t : T?) {
        using() $(var ds : das_string) {
            ds := "hello"
            let r1 = eq("hello", ds)
            t |> equal(r1, true)
            let r2 = eq("world", ds)
            t |> equal(r2, false)
        }
    }

    t |> run("das_string eq string") @@(t : T?) {
        using() $(var ds : das_string) {
            ds := "hello"
            let r1 = eq(ds, "hello")
            t |> equal(r1, true)
            let r2 = eq(ds, "world")
            t |> equal(r2, false)
        }
    }

    t |> run("empty comparisons") @@(t : T?) {
        using() $(var ds : das_string) {
            let r1 = eq("", ds)
            t |> equal(r1, true)
            let r2 = eq(ds, "")
            t |> equal(r2, true)
        }
    }
}


[test(description = "join over an iterator handles fixed arrays, one element, an empty separator, a block formatter and integers")]
def test_join_iterator(t : T?) {
    t |> run("join with fixed array") @@(t : T?) {
        let arr = ["a", "b", "c"]
        t |> equal(join(arr, ", "), "a, b, c")
    }

    t |> run("join single element") @@(t : T?) {
        let arr = ["only"]
        t |> equal(join(arr, ", "), "only")
    }

    t |> run("join empty separator") @@(t : T?) {
        let arr = ["a", "b", "c"]
        t |> equal(join(arr, ""), "abc")
    }

    t |> run("join with block formatter") @@(t : T?) {
        let arr = [1, 2, 3]
        let result = join(arr, "-") $(var writer : StringBuilderWriter; elem : int) {
            write(writer, elem * 10)
        }
        t |> equal(result, "10-20-30")
    }

    t |> run("join integers") @@(t : T?) {
        let arr = [1, 2, 3]
        t |> equal(join(arr, "+"), "1+2+3")
    }
}


[test(description = "glob_match handles literal, question-mark, star and double-star patterns including empty inputs")]
def test_glob_match(t : T?) {
    // exact match
    t |> equal(glob_match("hello", "hello"), true)
    t |> equal(glob_match("hello", "world"), false)

    // ? matches single character
    t |> equal(glob_match("h?llo", "hello"), true)
    t |> equal(glob_match("h?llo", "hallo"), true)
    t |> equal(glob_match("h?llo", "hllo"), false)
    t |> equal(glob_match("?", "a"), true)
    t |> equal(glob_match("?", ""), false)

    // * matches any sequence
    t |> equal(glob_match("*.das", "foo.das"), true)
    t |> equal(glob_match("*.das", "bar.das"), true)
    t |> equal(glob_match("*.das", "foo.txt"), false)
    t |> equal(glob_match("*", "anything"), true)
    t |> equal(glob_match("*", ""), true)
    t |> equal(glob_match("foo*", "foobar"), true)
    t |> equal(glob_match("foo*", "foo"), true)
    t |> equal(glob_match("foo*", "fo"), false)

    // combined * and ?
    t |> equal(glob_match("t?st*.das", "test_foo.das"), true)
    t |> equal(glob_match("t?st*.das", "tast.das"), true)
    t |> equal(glob_match("t?st*.das", "test.txt"), false)

    // multiple *
    t |> equal(glob_match("*foo*bar*", "xxxfooYYYbar"), true)
    t |> equal(glob_match("*foo*bar*", "foobar"), true)
    t |> equal(glob_match("*foo*bar*", "foobaz"), false)

    // empty strings
    t |> equal(glob_match("", ""), true)
    t |> equal(glob_match("", "a"), false)
    t |> equal(glob_match("*", ""), true)
    t |> equal(glob_match("**", ""), true)
    t |> equal(glob_match("?", ""), false)

    // all wildcards
    t |> equal(glob_match("***", "abc"), true)
    t |> equal(glob_match("???", "abc"), true)
    t |> equal(glob_match("???", "ab"), false)
    t |> equal(glob_match("???", "abcd"), false)
}
