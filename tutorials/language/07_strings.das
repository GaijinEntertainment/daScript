// Tutorial 7: Strings
//
// This tutorial covers:
//   - String literals and escape sequences
//   - String interpolation (recap from tutorial 1)
//   - String operations: length, slice, character access
//   - Converting to/from strings
//   - require strings — to_int, to_float
//   - require daslib/strings_boost — split, join, starts_with, etc.
//
// Run: daslang.exe tutorials/language/07_strings.das

options gen2

require strings
require daslib/strings_boost

[export]
def main {

    // === String literals ===
    let greeting = "Hello, World!"
    print("{greeting}\n")

    // Escape sequences
    print("tab:\there\n")
    print("newline:\nhere\n")
    print("backslash: \\\n")
    print("quote: \"\n")

    // === String interpolation ===
    // Any expression inside {braces} is evaluated and inserted.
    let name = "daScript"
    let version = 0.5
    print("{name} version {version}\n")

    // Complex expressions work too
    print("powers of 2: {1} {2} {4} {8} {16}\n")

    // === String length ===
    let text = "Hello"
    print("length of \"{text}\" = {length(text)}\n")

    // === Character access ===
    // character_at returns an int (character code)
    let ch = character_at(text, 0)
    print("first char code of \"{text}\" = {ch}\n")
    // character_uat returns the character as int as well
    // to_char converts int back to a single-character string
    print("first char = {to_char(ch)}\n")

    // === Substring with slice ===
    let phrase = "Hello, World!"
    print("slice(0,5) = \"{slice(phrase, 0, 5)}\"\n")
    print("slice(7) = \"{slice(phrase, 7)}\"\n")

    // === String comparison ===
    print("\"abc\" == \"abc\" is {"abc" == "abc"}\n")
    print("\"abc\" == \"xyz\" is {"abc" == "xyz"}\n")
    print("\"abc\" < \"xyz\" is {"abc" < "xyz"}\n")

    // === Converting other types to string ===
    // string(value) works for numeric types
    let numStr = string(42)
    let fltStr = string(3.14)
    print("string(42) = \"{numStr}\"\n")
    print("string(3.14) = \"{fltStr}\"\n")
    // For booleans, use string interpolation
    let flag = true
    print("bool via interpolation = \"{flag}\"\n")

    // === Converting strings to numbers ===
    // Requires:  require strings
    // int("123") does NOT work — use to_int / to_float
    let n = to_int("123")
    let f = to_float("3.14")
    print("to_int(\"123\") = {n}\n")
    print("to_float(\"3.14\") = {f}\n")

    // === strings_boost utilities ===
    // Requires:  require daslib/strings_boost

    // starts_with / ends_with
    let filename = "image.png"
    print("\"{filename}\" starts_with \"image\" = {starts_with(filename, "image")}\n")
    print("\"{filename}\" ends_with \".png\" = {ends_with(filename, ".png")}\n")

    // join — concatenate array of strings with separator
    let parts = fixed_array("one", "two", "three")
    print("join: {join(parts, ", ")}\n")

    // split — split string by delimiter, returns array<string>
    let csv = "apple,banana,cherry"
    var items <- split(csv, ",")
    print("split: ")
    for (item in items) {
        print("[{item}] ")
    }
    print("\n")

    // replace — replace all occurrences
    let original = "aabbcc"
    print("replace: {replace(original, "bb", "XX")}\n")

    // strip — remove leading/trailing whitespace
    let padded = "  hello  "
    print("strip: \"{strip(padded)}\"\n")
}

// output:
// Hello, World!
// tab:	here
// newline:
// here
// backslash: \
// quote: "
// daScript version 0.5
// powers of 2: 1 2 4 8 16
// length of "Hello" = 5
// first char code of "Hello" = 72
// first char = H
// slice(0,5) = "Hello"
// slice(7) = "World!"
// "abc" == "abc" is true
// "abc" == "xyz" is false
// "abc" < "xyz" is true
// string(42) = "42"
// string(3.14) = "3.14"
// bool via interpolation = "true"
// to_int("123") = 123
// to_float("3.14") = 3.14
// "image.png" starts_with "image" = true
// "image.png" ends_with ".png" = true
// join: one, two, three
// split: [apple] [banana] [cherry]
// replace: aaXXcc
// strip: "hello"
