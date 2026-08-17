options gen2
require dastest/testing_boost public
require watch_verdict

[test]
def test_watch_verdict(t : T?) {
    t |> run("red CI outranks everything") @(t : T?) {
        t |> equal(watch_verdict(1, 0, false, 0), WatchVerdict.ci_red)
        t |> equal(watch_verdict(1, 3, true, 2), WatchVerdict.ci_red)
        t |> equal(watch_verdict(2, 0, true, 0), WatchVerdict.ci_red)
    }
    t |> run("review on tip with open threads means triage, even mid-matrix") @(t : T?) {
        t |> equal(watch_verdict(0, 5, true, 1), WatchVerdict.review_to_triage)
        t |> equal(watch_verdict(0, 0, true, 3), WatchVerdict.review_to_triage)
    }
    t |> run("ready only when green, dry, and zero unresolved") @(t : T?) {
        t |> equal(watch_verdict(0, 0, true, 0), WatchVerdict.ready_to_merge)
        t |> equal(watch_verdict(0, 1, true, 0), WatchVerdict.keep_polling)
        t |> equal(watch_verdict(0, 0, false, 0), WatchVerdict.keep_polling)
    }
    t |> run("unreviewed tip polls regardless of thread state") @(t : T?) {
        t |> equal(watch_verdict(0, 0, false, 4), WatchVerdict.keep_polling)
        t |> equal(watch_verdict(0, 9, false, 0), WatchVerdict.keep_polling)
    }
    t |> run("check lines classify by bucket; pass and skip are neither") @(t : T?) {
        let r <- classify_checks(["fail build_linux", "pending docs", "pass lint", "skipping macos", "fail extended checks"])
        t |> equal(length(r.fails), 2)
        t |> equal(r.fails[0], "build_linux")
        t |> equal(r.fails[1], "extended checks")
        t |> equal(r.pending, 1)
        let none <- classify_checks([])
        t |> equal(length(none.fails), 0)
        t |> equal(none.pending, 0)
    }
    t |> run("verdicts map to the documented exits; keep_polling has none") @(t : T?) {
        t |> equal(exit_for(WatchVerdict.ci_red), 2)
        t |> equal(exit_for(WatchVerdict.review_to_triage), 3)
        t |> equal(exit_for(WatchVerdict.ready_to_merge), 0)
        t |> equal(exit_for(WatchVerdict.keep_polling), -1)
    }
    t |> run("poll budget clamps both zeroes") @(t : T?) {
        t |> equal(poll_budget(45, 30), 90)
        t |> equal(poll_budget(1, 5), 12)
        t |> equal(poll_budget(0, 30), 1)
        t |> equal(poll_budget(1, 0), 60)
    }
}
