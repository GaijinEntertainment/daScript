options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of .github/workflows/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang .github/workflows/REVIEW.das - exit 0 clean, 1 with findings.

let private WORKFLOWS = ".github/workflows"

struct private JobKeys {
    name : string
    line : int
    has_runs_on : bool
    has_timeout : bool
}

//! a job that calls a reusable workflow (`uses:`) has no runs-on, and GitHub rejects timeout-minutes
//! on it - the called workflow's own jobs carry the timeout
def private report_untimed(path : string; job : JobKeys) {
    if (job.has_runs_on && !job.has_timeout) {
        gate_finding(path, job.line, "job `{job.name}` sets runs-on and no timeout-minutes - one hung step then holds its runner until GitHub's six-hour ceiling; add timeout-minutes, about twice the job's slowest recent run")
    }
}

def private is_jobs_key(text : string) : bool => text == "jobs:" || (text |> starts_with("jobs:") && strip(slice(text, 5)) |> starts_with("#"))

def private check_job_timeouts(path : string) {
    var in_jobs = false
    var job_indent = -1
    var key_indent = -1
    var job : JobKeys
    var line_no = 0
    for (raw in split(fread(path), "\n")) {
        line_no++
        let text = strip_right(raw)
        let body = strip_left(text)
        continue if (empty(body) || body |> starts_with("#"))
        let indent = length(text) - length(body)
        if (indent == 0) {
            report_untimed(path, job)
            job = JobKeys()
            in_jobs = is_jobs_key(body)
            job_indent = -1
            continue
        }
        continue if (!in_jobs)
        if (job_indent < 0) {
            job_indent = indent
        }
        if (indent <= job_indent) {
            report_untimed(path, job)
            job = JobKeys(name = slice(body, 0, find(body, ":")), line = line_no)
            key_indent = -1
            continue
        }
        if (key_indent < 0) {
            key_indent = indent
        }
        if (indent == key_indent) {
            job.has_runs_on ||= body |> starts_with("runs-on:")
            job.has_timeout ||= body |> starts_with("timeout-minutes:")
        }
    }
    report_untimed(path, job)
}

[export]
def main() : int {
    if (!fexist("REVIEW_COMMON.md") || !stat(WORKFLOWS).is_dir) {
        to_log(LOG_ERROR, "{WORKFLOWS}/REVIEW.das: run from the repo root\n")
        return 2
    }
    dir(WORKFLOWS) $(name) {
        return if (!(name |> ends_with(".yml")) && !(name |> ends_with(".yaml")))
        check_job_timeouts("{WORKFLOWS}/{name}")
    }
    return gate_verdict(WORKFLOWS)
}
