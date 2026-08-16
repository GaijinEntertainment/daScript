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
}
