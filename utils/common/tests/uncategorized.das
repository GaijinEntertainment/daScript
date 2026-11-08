options gen2

require dastest/testing_boost public
require ../text_lines.das

[test]
def test_non_empty_lines(t : T?) {
    t |> equal(non_empty_lines("a.txt\r\nb/c.txt\n"), ["a.txt", "b/c.txt"],
        "CR and the trailing newline dropped")
    t |> equal(length(non_empty_lines("")), 0, "empty output is no lines")
    t |> equal(length(non_empty_lines("\n\r\n   \n")), 0, "blank and whitespace-only lines dropped")
}

[test]
def test_split_trimmed(t : T?) {
    t |> equal(split_trimmed(" a.bin, ,b.gguf ,", ","), ["a.bin", "b.gguf"],
        "pieces stripped, empty ones dropped, in order")
    t |> equal(length(split_trimmed("", ",")), 0, "an empty text is no pieces")
    t |> equal(split_trimmed("x", ","), ["x"], "no separator is one piece")
}
