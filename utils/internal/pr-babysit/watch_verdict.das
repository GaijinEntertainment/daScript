options gen2

// pure decision core for --watch; network reads live in main.das

require strings
require math

enum public WatchVerdict {
    keep_polling
    ci_red
    review_to_triage
    ready_to_merge
}

def public watch_verdict(fail_count : int; pending_count : int; reviewed_tip : bool; unresolved : int) : WatchVerdict {
    // a red check outranks everything - babysit fixes CI first, Copilot re-reviews the fix anyway
    return WatchVerdict.ci_red if (fail_count > 0)
    // triage does not wait for the matrix - the loop iterates against the review, not CI
    return WatchVerdict.review_to_triage if (reviewed_tip && unresolved > 0)
    return WatchVerdict.ready_to_merge if (reviewed_tip && unresolved == 0 && pending_count == 0)
    return WatchVerdict.keep_polling
}

// "<bucket> <name>" check lines -> failing names + pending count; other buckets pass or skip.
// A cancelled lane is red until it is re-run - between a cancel and a rerun it must not read as "nothing pending".
// A failing check named in `ignored` - a lane red on master with a control posted on the PR - is
// reported apart and does not make the verdict red.
def public classify_checks(checks : array<string>; ignored : array<string>) : tuple<fails : array<string>; ignored : array<string>; pending : int> {
    var r : tuple<fails : array<string>; ignored : array<string>; pending : int>
    for (c in checks) {
        var name = ""
        if (c |> starts_with("fail ")) {
            name = slice(c, 5)
        } elif (c |> starts_with("cancel ")) {
            name = slice(c, 7)
        } elif (c |> starts_with("pending ")) {
            r.pending++
        }
        continue if (empty(name))
        if (ignored |> has_value(name)) {
            r.ignored |> push(name)
        } else {
            r.fails |> push(name)
        }
    }
    return <- r
}

def public classify_checks(checks : array<string>) : tuple<fails : array<string>; ignored : array<string>; pending : int> {
    let none : array<string>
    return <- classify_checks(checks, none)
}

// process exit per verdict; keep_polling has none
def public exit_for(verdict : WatchVerdict) : int {
    return 2 if (verdict == WatchVerdict.ci_red)
    return 3 if (verdict == WatchVerdict.review_to_triage)
    return 0 if (verdict == WatchVerdict.ready_to_merge)
    return -1
}

def public poll_budget(timeout_min : int; interval_sec : int) : int {
    return max(1, (timeout_min * 60) / max(1, interval_sec))
}
