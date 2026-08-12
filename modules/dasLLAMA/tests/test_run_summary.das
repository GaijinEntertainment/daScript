options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require run   // the suite runner's own helper — bare same-dir require; run.das is [export] main only, no [init]
require daslib/fio

// The runner's log scraper, model-free: log_summary walks a dastest log line by line and keeps the
// LAST NEWLINE-TERMINATED line carrying " tests, ". An unterminated final line (a run killed
// mid-print) is therefore invisible to it — the rows below pin that, carrying RESWEEP markers.

// Per-process temp directory, created on first use and removed by [finalize] — writing fixtures
// into the worktree would leave untracked state, and a shared path races parallel dastest runs.
var private _tmp_dir : string
var private _tmp_dir_inited = false

def private fixture_path(name : string) : string {
    if (!_tmp_dir_inited) {
        let r = create_temp_directory_result("dasllama_run_summary")
        if (!(r is value)) {   // "" would resolve to the CWD and defeat the hermetic goal
            panic("could not create temp dir for the log_summary test: {unsafe(r.error)}")
        }
        _tmp_dir = unsafe(r.value)
        _tmp_dir_inited = true
    }
    return path_join(_tmp_dir, name)
}

def private write_log(name : string; content : string) : string {
    let p = fixture_path(name)
    fopen(p, "wb") $(f) {
        if (f != null) {
            fwrite(f, content)
        }
    }
    return p
}

[finalize]
def cleanup_temp_dir {
    if (_tmp_dir_inited && !empty(_tmp_dir)) {
        rmdir_rec(_tmp_dir)
    }
}

[test]
def test_log_summary_terminated(t : T?) {
    t |> run("a terminated matching line is found among noise") @(t : T?) {
        let p = write_log("terminated.log",
                          "=== RUN 'arm12'\n--- PASS 'arm12' (1.5s)\n7 tests, 7 passed, 0 failed, 0 errors, 0 skipped\nSUCCESS! (1.5s)\n")
        t |> equal(log_summary(p), "7 tests, 7 passed, 0 failed, 0 errors, 0 skipped")
    }
    t |> run("the WHOLE line is reported, not just the text from the marker on") @(t : T?) {
        // the marker sits mid-line: the reported summary must start at the line start
        let p = write_log("midline.log", "noise\n[E] 3 tests, 2 passed, 1 failed\ntrailing noise\n")
        t |> equal(log_summary(p), "[E] 3 tests, 2 passed, 1 failed")
    }
}

[test]
def test_log_summary_unterminated(t : T?) {
    t |> run("an UNTERMINATED final line does NOT match - the per-line walk needs its newline") @(t : T?) {
        // a run killed mid-print leaves no trailing newline, so that line never closes
        let p = write_log("unterminated.log", "=== RUN 'arm12'\n9 tests, 8 passed, 1 failed")
        t |> equal(log_summary(p), "")   // RESWEEP: v1 widened this — re-sweep decides consciously
    }
    t |> run("an unterminated single-line log does not match either") @(t : T?) {
        let p = write_log("unterminated_only.log", "4 tests, 4 passed")
        t |> equal(log_summary(p), "")   // RESWEEP: v1 widened this — re-sweep decides consciously
    }
}

[test]
def test_log_summary_last_wins(t : T?) {
    t |> run("two matching lines: the LAST one wins") @(t : T?) {
        let p = write_log("two.log",
                          "1 tests, 1 passed, 0 failed\nnoise between\n2 tests, 0 passed, 2 failed\nSUCCESS!\n")
        t |> equal(log_summary(p), "2 tests, 0 passed, 2 failed")
    }
    t |> run("of three, the last TERMINATED one wins - the unterminated third is skipped") @(t : T?) {
        let p = write_log("three.log", "1 tests, a\n2 tests, b\n3 tests, c")
        t |> equal(log_summary(p), "2 tests, b")   // RESWEEP: v1 widened this — re-sweep decides consciously
    }
}

[test]
def test_log_summary_marker_twice(t : T?) {
    t |> run("a line carrying the marker TWICE yields one result: that line") @(t : T?) {
        // the walk reports whole lines, so a repeated marker inside one is still one result
        let p = write_log("twice.log", "before\n5 tests, ran 5 tests, all passed\nafter\n")
        t |> equal(log_summary(p), "5 tests, ran 5 tests, all passed")
    }
    t |> run("a doubled marker on an unterminated LAST line is invisible, doubled or not") @(t : T?) {
        let p = write_log("twice_unterminated.log", "before\n5 tests, ran 5 tests, all passed")
        t |> equal(log_summary(p), "")   // RESWEEP: v1 widened this — re-sweep decides consciously
    }
}

[test]
def test_log_summary_no_match(t : T?) {
    t |> run("no marker returns empty (the run died before printing one)") @(t : T?) {
        let p = write_log("nomatch.log", "compiling...\nerror[20605]: missing prerequisite\n")
        t |> equal(log_summary(p), "")
    }
    t |> run("an empty log returns empty") @(t : T?) {
        let p = write_log("empty.log", "")
        t |> equal(log_summary(p), "")
    }
    t |> run("a near-miss token is not a hit (the marker carries its spaces)") @(t : T?) {
        // "tests," with no leading space, and " tests" with no comma — neither is " tests, "
        let p = write_log("nearmiss.log", "ran 5 tests,now done\n6 tests done\n")
        t |> equal(log_summary(p), "")
    }
    t |> run("a missing file returns empty") @(t : T?) {
        t |> equal(log_summary(fixture_path("no_such_file_xyz.log")), "")
    }
}
