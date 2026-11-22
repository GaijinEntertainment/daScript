options gen2

require dastest/testing_boost public
require ../text_lines.das
require ../parallel_workers.das
require strings

[test(description = "non_empty_lines keeps the non-blank lines of a text")]
def test_non_empty_lines(t : T?) {
    t |> equal(non_empty_lines("a.txt\r\nb/c.txt\n"), ["a.txt", "b/c.txt"],
        "CR and the trailing newline dropped")
    t |> equal(length(non_empty_lines("")), 0, "empty output is no lines")
    t |> equal(length(non_empty_lines("\n\r\n   \n")), 0, "blank and whitespace-only lines dropped")
}

[test(description = "split_trimmed splits a text on a separator and trims each piece")]
def test_split_trimmed(t : T?) {
    t |> equal(split_trimmed(" a.bin, ,b.gguf ,", ","), ["a.bin", "b.gguf"],
        "pieces stripped, empty ones dropped, in order")
    t |> equal(length(split_trimmed("", ",")), 0, "an empty text is no pieces")
    t |> equal(split_trimmed("x", ","), ["x"], "no separator is one piece")
}

[test(description = "run_chunk_workers runs every argv as a child at once and returns each one's exit code, output and wall seconds in input order")]
def test_run_chunk_workers(t : T?) {
    var err : string
    let tmp = create_temp_directory("das_chunk_workers", err)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {err}")
        return
    }
    let script = path_join(tmp, "child.das")
    let body = ("options gen2\nrequire strings\n[export]\ndef main() : int \{\n"
        + "    let args <- get_command_line_arguments()\n"
        + "    let last = args[length(args) - 1]\n"
        + "    print(\"child \{last\}\\n\")\n    return to_int(last)\n\}\n")
    fwrite(script, body)
    let exe = get_command_line_arguments()[0]
    let runs <- run_chunk_workers([[exe, script, "--", "0"], [exe, script, "--", "3"]])
    t |> equal(length(runs), 2, "one result an argv")
    t |> equal(runs[0].exit_code, 0, "the first child's exit code")
    t |> equal(runs[1].exit_code, 3, "the second child's, in input order")
    t |> success(find(runs[0].stdout, "child 0") >= 0, "the first child's output")
    t |> success(find(runs[1].stdout, "child 3") >= 0, "the second child's output")
    let timed = runs[0].seconds > 0.0 && runs[1].seconds > 0.0
    t |> success(timed, "each child's wall time is measured")
    rmdir_rec(tmp, err)
}
