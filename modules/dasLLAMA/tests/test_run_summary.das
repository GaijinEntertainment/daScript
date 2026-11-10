options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require run   // the suite runner's own helper — bare same-dir require; run.das is [export] main only, no [init]
require _scratch_dir   // scratch_file / scratch_path: the log fixtures' files
require daslib/strings_boost

// The runner's log scraper, model-free: log_summary walks a dastest log line by line and keeps the
// LAST line carrying " tests, ". End-of-file closes a line just like a newline does, so a run
// killed mid-print still reports the summary it managed to write.

[test(description = "`log_summary` finds a newline-terminated summary line among noise and returns the whole line, including text before the marker")]
def test_log_summary_terminated(t : T?) {
    t |> run("a terminated matching line is found among noise") @(t : T?) {
        let p = scratch_file("terminated.log",
                          "=== RUN 'arm12'\n--- PASS 'arm12' (1.5s)\n7 tests, 7 passed, 0 failed, 0 errors, 0 skipped\nSUCCESS! (1.5s)\n")
        t |> equal(log_summary(p), "7 tests, 7 passed, 0 failed, 0 errors, 0 skipped")
    }
    t |> run("the WHOLE line is reported, not just the text from the marker on") @(t : T?) {
        // the marker sits mid-line: the reported summary must start at the line start
        let p = scratch_file("midline.log", "noise\n[E] 3 tests, 2 passed, 1 failed\ntrailing noise\n")
        t |> equal(log_summary(p), "[E] 3 tests, 2 passed, 1 failed")
    }
}

[test(description = "`log_summary` treats end of file as closing a line, so an unterminated final summary line, and a single-line log with no newline, are found")]
def test_log_summary_unterminated(t : T?) {
    t |> run("an UNTERMINATED final line matches - end of file closes it") @(t : T?) {
        // a run killed mid-print leaves no trailing newline, and that line is the one worth having
        let p = scratch_file("unterminated.log", "=== RUN 'arm12'\n9 tests, 8 passed, 1 failed")
        t |> equal(log_summary(p), "9 tests, 8 passed, 1 failed")
    }
    t |> run("an unterminated single-line log matches too") @(t : T?) {
        let p = scratch_file("unterminated_only.log", "4 tests, 4 passed")
        t |> equal(log_summary(p), "4 tests, 4 passed")
    }
}

[test(description = "When several lines carry the summary marker, `log_summary` returns the last one, terminated or not")]
def test_log_summary_last_wins(t : T?) {
    t |> run("two matching lines: the LAST one wins") @(t : T?) {
        let p = scratch_file("two.log",
                          "1 tests, 1 passed, 0 failed\nnoise between\n2 tests, 0 passed, 2 failed\nSUCCESS!\n")
        t |> equal(log_summary(p), "2 tests, 0 passed, 2 failed")
    }
    t |> run("of three, the last one wins even though no newline terminates it") @(t : T?) {
        let p = scratch_file("three.log", "1 tests, a\n2 tests, b\n3 tests, c")
        t |> equal(log_summary(p), "3 tests, c")
    }
}

[test(description = "A line carrying the summary marker twice yields that one whole line from `log_summary`, whether terminated or the unterminated last line")]
def test_log_summary_marker_twice(t : T?) {
    t |> run("a line carrying the marker TWICE yields one result: that line") @(t : T?) {
        // the walk reports whole lines, so a repeated marker inside one is still one result
        let p = scratch_file("twice.log", "before\n5 tests, ran 5 tests, all passed\nafter\n")
        t |> equal(log_summary(p), "5 tests, ran 5 tests, all passed")
    }
    t |> run("a doubled marker on an unterminated LAST line still yields that one line") @(t : T?) {
        let p = scratch_file("twice_unterminated.log", "before\n5 tests, ran 5 tests, all passed")
        t |> equal(log_summary(p), "5 tests, ran 5 tests, all passed")
    }
}

[test(description = "`log_summary` returns empty for a log with no marker, an empty log, near-miss tokens lacking the marker's spaces or comma, and a missing file")]
def test_log_summary_no_match(t : T?) {
    t |> run("no marker returns empty (the run died before printing one)") @(t : T?) {
        let p = scratch_file("nomatch.log", "compiling...\nerror[20605]: missing prerequisite\n")
        t |> equal(log_summary(p), "")
    }
    t |> run("an empty log returns empty") @(t : T?) {
        let p = scratch_file("empty.log", "")
        t |> equal(log_summary(p), "")
    }
    t |> run("a near-miss token is not a hit (the marker carries its spaces)") @(t : T?) {
        // "tests," with no leading space, and " tests" with no comma — neither is " tests, "
        let p = scratch_file("nearmiss.log", "ran 5 tests,now done\n6 tests done\n")
        t |> equal(log_summary(p), "")
    }
    t |> run("a missing file returns empty") @(t : T?) {
        t |> equal(log_summary(scratch_path("no_such_file_xyz.log")), "")
    }
}

[test(description = "`log_verdicts` collects line-start PASS and FAIL file verdicts by base name in order, color codes allowed, flags a root compile failure or refusal, and ignores per-test, mid-line and bare verdict words")]
def test_log_verdicts(t : T?) {
    t |> run("plain verdict lines land in their sets, in order, by base name") @(t : T?) {
        let p = scratch_file("verdicts.log",
            "=== RUN 'a'\nPASS modules/dasLLAMA/tests/test_a.das (1.5s)\nFAIL modules/dasLLAMA/tests/test_b.das (0.2s)\nPASS /abs/dir/test_c.das (3s)\n3 tests, 2 passed, 1 failed\n")
        let v <- log_verdicts(p)
        t |> equal(join(v.passed, ","), "test_a.das,test_c.das")
        t |> equal(join(v.failed, ","), "test_b.das")
    }
    t |> run("a color code glued ahead of the word still reads as the verdict") @(t : T?) {
        let p = scratch_file("colored.log", "\x1b[32mPASS tests/test_a.das (1s)\x1b[0m\n\x1b[31mFAIL tests/test_b.das (1s)\x1b[0m\n")
        let v <- log_verdicts(p)
        t |> equal(join(v.passed, ","), "test_a.das")
        t |> equal(join(v.failed, ","), "test_b.das")
    }
    t |> run("a root that failed to compile or refused the list marks every FAIL the root's, not a file's") @(t : T?) {
        let p = scratch_file("rootfail.log",
            "[E] Failed to compile the together root /tmp/x/dastest_together_1.das\nerror[20605]: missing prerequisite\nFAIL tests/test_a.das (0.000000s)\nFAIL tests/test_b.das (0.000000s)\n2 tests, 0 passed, 2 failed\n")
        let v <- log_verdicts(p)
        t |> success(v.root_failed, "the compile failure is the root's")
        t |> equal(length(v.failed), 2, "the synthetic verdicts are still read, the caller decides")
        let q = scratch_file("rootrefuse.log", "[E] together: the files' options disagree - a.das: `options stack = 1` against `options stack = 2`\nFAIL a.das (0s)\n")
        t |> success((log_verdicts(q)).root_failed, "a refusal is the root's too")
        let clean = scratch_file("rootok.log", "PASS tests/test_a.das (1s)\nFAIL tests/test_b.das (1s)\n2 tests, 1 passed, 1 failed\n")
        t |> success(!(log_verdicts(clean)).root_failed, "a file's own FAIL is not the root's")
    }
    t |> run("dastest's colored per-test line is no verdict either") @(t : T?) {
        let p = scratch_file("colored_per_test.log", "\x1b[32m--- PASS 'test_one'\x1b[0m\n\x1b[31m--- FAIL 'test_two'\x1b[0m\n\x1b[32mPASS tests/test_a.das (1s)\x1b[0m\n")
        let v <- log_verdicts(p)
        t |> equal(join(v.passed, ","), "test_a.das")
        t |> equal(length(v.failed), 0, "the per-test FAIL names a test, not a file")
    }
    t |> run("the word mid-line, a bare word and a missing file are no verdict") @(t : T?) {
        let p = scratch_file("noverdict.log", "the cell PASS tests/test_a.das rode\nPASS\n--- PASS 'arm12' (1s)\n")
        let v <- log_verdicts(p)
        t |> equal(length(v.passed), 0)
        t |> equal(length(v.failed), 0)
        let none <- log_verdicts(scratch_path("no_such_verdicts.log"))
        t |> equal(length(none.passed) + length(none.failed), 0)
    }
}
