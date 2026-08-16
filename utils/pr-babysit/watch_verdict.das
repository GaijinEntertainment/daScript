options gen2

// pure decision core for --watch; network reads live in main.das

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
