options gen2

require dastest/testing_boost public
require daslib/fio
require classify

[test]
def test_verdict_lines(t : T?) {
    t |> equal("OK a/b.das", verdict_line("a/b.das", 0, 1.0, 60.0, ""))
    t |> equal("TIMEOUT a/b.das 61.5s", verdict_line("a/b.das", 1, 61.5, 60.0, "whatever"))
    let failed = verdict_line("a/b.das", 1, 2.0, 60.0, "\nerror[20605]: missing prerequisite 'x'\nmore\n")
    t |> equal("FAIL a/b.das rc=1 error[20605]: missing prerequisite 'x'", failed)
    t |> equal("FAIL a/b.das rc=3 ", verdict_line("a/b.das", 3, 2.0, 60.0, ""), "empty output keeps the line parseable")
}

[test]
def test_expected_list_round_trip(t : T?) {
    let fixture_r = create_temp_file_result("test_release_expected", ".txt")
    t |> success(fixture_r is value, "temp fixture created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    t |> success(fwrite(fixture, "# comment\n\nexamples/a.das needs the telegram package\ntutorials/b.das\n"))
    var inscope exp <- load_expected(fixture)
    t |> equal(2, length(exp))
    t |> equal("needs the telegram package", exp ?["examples/a.das"] ?? "MISSING")
    t |> equal("(no reason recorded)", exp ?["tutorials/b.das"] ?? "MISSING")
    t |> success(remove(fixture))
}

[test]
def test_summarize_buckets(t : T?) {
    var inscope expected : table<string; string>
    expected["e/known.das"] = "by design"
    expected["e/fixed.das"] = "was broken"
    let verdicts <- ["OK e/clean.das", "OK e/fixed.das",
                     "FAIL e/known.das rc=1 error[20605]: x",
                     "FAIL e/new.das rc=1 error[30000]: y",
                     "TIMEOUT e/spin.das 60.1s"]
    var inscope r <- summarize(verdicts, expected)
    t |> equal(1, r.ok_count)
    t |> equal(1, r.expected_count)
    t |> equal(1, r.unexpected_count, "a fail off the list reds")
    t |> equal(1, r.stale_count, "an expected entry that passes reds as stale")
    t |> equal(1, r.timeout_count)
}
