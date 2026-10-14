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
