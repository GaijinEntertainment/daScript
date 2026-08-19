options gen2

require strings
require daslib/strings_boost
require daslib/fio

// The pure half of test-release: verdict-line formatting/parsing, the expected-failures
// list, and report summarization — everything a fixture test can pin without a bundle.

//! One machine-parseable verdict line per checked item (a bundle-relative path, or a utils
//! row name). Child stderr/stdout is folded to one line so parallel workers can never
//! interleave a report.
def verdict_line(subject : string; rc : int; sec, timeout : float; out : string) : string {
    if (rc == 0) {
        return "OK {subject}"
    }
    if (sec >= timeout) {
        return "TIMEOUT {subject} {sec}s"
    }
    var head = ""
    for (ln in split(out, "\n")) {
        let s = strip(replace(ln, "\r", ""))
        if (!empty(s)) {
            head = s
            break
        }
    }
    return "FAIL {subject} rc={rc} {head}"
}

//! Expected-failure entries: `path  reason`, one per line, `#` comments. Returns
//! path → reason.
def load_expected(path : string) : table<string; string> {
    var out : table<string; string>
    let text = fread(path)
    for (raw in split(text, "\n")) {
        let s = strip(replace(raw, "\r", ""))
        continue if (empty(s) || s |> starts_with("#"))
        let sp = find(s, ' ')
        if (sp < 0) {
            out[s] = "(no reason recorded)"
        } else {
            out[slice(s, 0, sp)] = strip(slice(s, sp, length(s)))
        }
    }
    return <- out
}

struct Report {
    lines : array<string>
    ok_count : int
    expected_count : int
    unexpected_count : int
    stale_count : int
    timeout_count : int
}

//! Fold worker verdict lines against the expected list. Unexpected failures and
//! timeouts red the gate; an expected entry that now PASSES is stale and reds too —
//! the list must shrink when the tree improves.
def summarize(verdicts : array<string>; expected : table<string; string>) : Report {
    var r : Report
    for (v in verdicts) {
        if (v |> starts_with("OK ")) {
            let f = slice(v, 3, length(v))
            if (key_exists(expected, f)) {
                r.lines |> push("STALE-EXPECTED {f} — passes now; drop it from the list")
                r.stale_count++
            } else {
                r.ok_count++
            }
        } elif (v |> starts_with("TIMEOUT ")) {
            r.lines |> push(v)
            r.timeout_count++
        } elif (v |> starts_with("FAIL ")) {
            let rest = slice(v, 5, length(v))
            let sp = find(rest, ' ')
            let f = sp < 0 ? rest : slice(rest, 0, sp)
            if (key_exists(expected, f)) {
                let reason = expected ?[f] ?? ""
                r.lines |> push("expected-fail {f} ({reason})")
                r.expected_count++
            } else {
                r.lines |> push("UNEXPECTED {v}")
                r.unexpected_count++
            }
        }
    }
    return <- r
}
