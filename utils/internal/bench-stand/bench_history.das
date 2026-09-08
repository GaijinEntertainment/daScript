options gen2
options indenting = 4

module bench_history public

require bench_suite
require bench_runner
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require strings
require math

//! What a night amounts to. `build_failed` never measured anything; `bench_failed` measured
//! what it could and at least one file failed. json_boost writes an enum as its name, so a
//! record and the viewer both read these spellings.
enum RunStatus {
    ok
    build_failed
    bench_failed
}

//! A verdict on one benchmark arm at the latest run. `first_seen` had no earlier point to
//! compare with; `missing` had a baseline and no point tonight.
enum ChangeKind {
    regression
    improvement
    first_seen
    missing
}

struct CommitInfo {
    sha : string
    date : string
    subject : string
    author : string
}

struct MachineInfo {
    host : string
    cpu : string
    kernel : string
    compiler : string
    cores : int
    mem_gb : int
    load_start : string
}

//! `status` defaults to `ok` so a meta that says nothing about a build - a local run driven by
//! hand - is not read as a night whose build failed. The driver always writes it explicitly.
struct BuildInfo {
    status : string = "ok"
    seconds : double
    log_tail : string
}

//! What the box-side driver hands `run`: identity of the night and of the tree it built.
struct RunMeta {
    run_id : string
    started : string
    commit : CommitInfo
    machine : MachineInfo
    build : BuildInfo = BuildInfo()
}

//! One night's record (`runs/<run_id>.json`). `status` is `ok`, `build_failed` or
//! `bench_failed`; `lanes` maps a lane to `ok` or the reason it did not run.
struct RunRecord {
    schema : int = 1
    run_id : string
    started : string
    finished : string
    seconds : double
    commit : CommitInfo
    machine : MachineInfo
    build : BuildInfo = BuildInfo()
    lanes : table<string; string>
    status : RunStatus
    files : array<FileResult>
}

struct Failure {
    path : string
    lane : string
    status : string
    message : string
}

//! One row of the viewer's run history. `failures` covers every non-ok, non-skipped file.
struct RunSummary {
    id : string
    sha : string
    date : string
    subject : string
    started : string
    status : RunStatus
    seconds : double
    build_status : string
    build_seconds : double
    build_log_tail : string
    host : string
    lanes : table<string; string>
    failures : array<Failure>
    skipped : array<Failure>
    files_ok : int
    regressions : int
    improvements : int
}

//! One benchmark arm in one lane over every run that measured it. The three arrays are one
//! point per run, in run order: the run's index into `Dataset.runs`, its ns/op minimum, and its
//! spread over that night's repeats.
struct Series {
    id : string
    group : string
    file : string
    lane : string
    runs : array<int>
    ns : array<double>
    spread : array<double>
}

//! `noise` is the baseline's relative median absolute deviation - the gate `change` had to pass.
struct Change {
    id : string
    group : string
    lane : string
    kind : ChangeKind
    baseline : double
    value : double
    change : double
    noise : double
}

//! Geometric-mean index of one group in one lane, 100 = each series at its own reference
//! (the median of its first points). One point per run, in run order.
struct IndexSeries {
    group : string
    lane : string
    runs : array<int>
    value : array<double>
}

//! `site/data.json`: everything the viewer renders, and what the summary is rendered from.
struct Dataset {
    schema : int = 1
    generated : string
    repo_url : string
    latest : int
    regression_threshold : double
    noise_multiplier : double
    baseline_runs : int
    runs : array<RunSummary>
    groups : array<string>
    series : array<Series>
    changes : array<Change>
    index : array<IndexSeries>
}

let INDEX_REFERENCE_POINTS = 5
let MIN_BASELINE_POINTS = 3

//! The median of `vals`, which it does not reorder; 0 for an empty input.
def median(vals : array<double>) : double {
    return 0.0lf if (empty(vals))
    var sorted := vals
    sort(sorted)
    let n = length(sorted)
    return (n % 2 == 1) ? sorted[n / 2] : (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0lf
}

//! Median absolute deviation divided by the median; 0 unless the median is positive.
def relative_mad(vals : array<double>) : double {
    return 0.0lf if (empty(vals))
    let med = median(vals)
    return 0.0lf if (med <= 0.0lf)
    var devs <- [for (x in vals); abs(x - med)]
    return median(devs) / med
}

def geomean(vals : array<double>) : double {
    return 0.0lf if (empty(vals))
    var acc = 0.0lf
    for (x in vals) {
        acc += log(x)
    }
    return exp(acc / double(length(vals)))
}

//! The status a record earns from its parts. A night that measured nothing is not `ok`, whatever
//! the reason - every lane unavailable, a filter matching no file, a suite that vanished: an
//! empty night carries no data point and must not read as a good one.
def derive_status(rec : RunRecord) : RunStatus {
    return RunStatus.build_failed if (rec.build.status != "ok")
    var measured = 0
    for (f in rec.files) {
        return RunStatus.bench_failed if (is_failure(f.status))
        measured++ if (f.status == FileStatus.ok)
    }
    return RunStatus.bench_failed if (measured == 0)
    return RunStatus.ok
}

let DRIVER_FILE_PATH = "utils/internal/bench-stand/nightly.sh"

//! Fill `rec` in for a night that never benchmarked. A build that failed needs no file - its log
//! tail already rides `build` - while a build that passed and a driver that could not run the
//! suite gets one synthetic failed file carrying `reason`, so the summary and the viewer name it
//! instead of showing an empty night. `rec` keeps the identity the driver's meta gave it.
def mark_failed_run(var rec : RunRecord&; finished, reason : string) {
    rec.finished = finished
    rec.seconds = 0.0lf
    rec.files |> clear()
    if (rec.build.status == "ok") {
        rec.files |> emplace(FileResult(path = DRIVER_FILE_PATH, id = "bench-stand", group = "stand",
            lane = "driver", status = FileStatus.exit_nonzero, exit_code = 1,
            message = empty(reason) ? "the driver could not run the suite" : reason))
    }
    rec.status = derive_status(rec)
}

//! Every `*.json` under `dir`, oldest commit first (commit date, then start time). A file that
//! does not parse is named in `errors` and left out, so one corrupt night cannot hide the rest.
def load_runs(runs_dir : string; var errors : array<string>) : array<RunRecord> {
    var names : array<string>
    dir(runs_dir) $(name) {
        return if (extension(name) != ".json")
        names |> push(name)
    }
    sort(names)
    var runs : array<RunRecord>
    for (name in names) {
        let path = path_join(runs_dir, name)
        var rec = RunRecord()
        if (!sscan_json(fread(path), rec) || empty(rec.run_id)) {
            errors |> push("run record does not parse or has no run_id: {path}")
            continue
        }
        rec.status = derive_status(rec)
        runs |> emplace(rec)
    }
    sort(runs) $(a, b) {
        return a.commit.date != b.commit.date ? a.commit.date < b.commit.date : a.started < b.started
    }
    return <- runs
}

//! Index of the run that started last - the night the summary and the changes describe; -1 when
//! there are no runs, which every caller branches on.
def latest_run_index(runs : array<RunRecord>) : int {
    var best = -1
    for (i in range(length(runs))) {
        if (best < 0 || runs[i].started >= runs[best].started) {
            best = i
        }
    }
    return best
}

def private summarize_run(rec : RunRecord) : RunSummary {
    var s <- RunSummary(id = rec.run_id, sha = rec.commit.sha, date = rec.commit.date, subject = rec.commit.subject,
        started = rec.started, status = rec.status, seconds = rec.seconds, build_status = rec.build.status,
        build_seconds = rec.build.seconds, build_log_tail = rec.build.log_tail, host = rec.machine.host)
    s.lanes := rec.lanes
    for (f in rec.files) {
        if (f.status == FileStatus.skipped) {
            s.skipped |> push(Failure(path = f.path, lane = f.lane, status = "{f.status}", message = f.message))
        } elif (is_failure(f.status)) {
            s.failures |> push(Failure(path = f.path, lane = f.lane, status = "{f.status}", message = f.message))
        } else {
            s.files_ok++
        }
    }
    return <- s
}

def private series_key(f : FileResult; sample_id : string) : string {
    return "{f.lane}\t{f.id}#{sample_id}"
}

//! `groups` collects only the groups that have at least one series - a group whose every file
//! failed every night has no chart to filter, and the failure panel is where it shows.
def private build_series(runs : array<RunRecord>; var groups : table<string>) : array<Series> {
    var index_of : table<string; int>
    var out : array<Series>
    for (ri, rec in range(length(runs)), runs) {
        for (f in rec.files) {
            continue if (f.status == FileStatus.skipped)
            if (!empty(f.samples)) {
                groups |> insert(f.group)
            }
            for (smp in f.samples) {
                let key = series_key(f, smp.id)
                var si = index_of?[key] ?? -1
                if (si < 0) {
                    si = length(out)
                    index_of[key] = si
                    out |> emplace(Series(id = "{f.id}#{smp.id}", group = f.group, file = f.id, lane = f.lane))
                }
                let n = length(out[si].runs)
                continue if (n > 0 && out[si].runs[n - 1] == ri)
                out[si].runs |> push(ri)
                out[si].ns |> push(smp.ns)
                out[si].spread |> push(smp.spread)
            }
        }
    }
    sort(out) $(a, b) {
        return a.lane != b.lane ? a.lane < b.lane : a.id < b.id
    }
    return <- out
}

def private lane_ok_at(rec : RunRecord; lane, file_id : string) : bool {
    for (f in rec.files) {
        return f.status == FileStatus.ok if (f.lane == lane && f.id == file_id)
    }
    return false
}

//! Verdicts for the latest run: each series measured there is compared with the median of its
//! last `baseline_runs` earlier points; a move beyond `regression_threshold` and beyond
//! `noise_multiplier` times the baseline's relative MAD is a regression or an improvement. A
//! series with no earlier point is `new`; one with a baseline but no point tonight, whose file
//! ran ok in that lane, is `missing`.
def detect_changes(runs : array<RunRecord>; series : array<Series>; latest : int; cfg : SuiteConfig) : array<Change> {
    var out : array<Change>
    return <- out if (latest < 0)
    for (ser in series) {
        var tonight = -1.0lf
        var prior : array<double>
        for (i in range(length(ser.runs))) {
            if (ser.runs[i] == latest) {
                tonight = ser.ns[i]
            } elif (ser.runs[i] < latest) {
                prior |> push(ser.ns[i])
            }
        }
        if (length(prior) > cfg.baseline_runs) {
            prior |> erase(0, length(prior) - cfg.baseline_runs)
        }
        if (tonight < 0.0lf) {
            if (!empty(prior) && lane_ok_at(runs[latest], ser.lane, ser.file)) {
                out |> push(Change(id = ser.id, group = ser.group, lane = ser.lane, kind = ChangeKind.missing, baseline = median(prior)))
            }
            continue
        }
        if (empty(prior)) {
            out |> push(Change(id = ser.id, group = ser.group, lane = ser.lane, kind = ChangeKind.first_seen, value = tonight))
            continue
        }
        continue if (length(prior) < MIN_BASELINE_POINTS)
        let baseline = median(prior)
        continue if (baseline <= 0.0lf)
        let noise = relative_mad(prior)
        let change = tonight / baseline - 1.0lf
        let gate = max(cfg.regression_threshold, cfg.noise_multiplier * noise)
        continue if (change <= gate && change >= -gate)
        let kind = change > gate ? ChangeKind.regression : ChangeKind.improvement
        out |> push(Change(id = ser.id, group = ser.group, lane = ser.lane, kind = kind,
            baseline = baseline, value = tonight, change = change, noise = noise))
    }
    return <- out
}

def private reference_of(ser : Series) : double {
    let firsts <- [for (i in range(min(INDEX_REFERENCE_POINTS, length(ser.ns)))); ser.ns[i]]
    return median(firsts)
}

//! Per group and lane, the geometric mean over its series of value / reference at every run
//! where at least half of the group's series were measured, times 100.
def build_index(series : array<Series>; run_count : int) : array<IndexSeries> {
    var index_of : table<string; int>
    var keys_in_order : array<string>
    var members : array<array<int>>
    for (si, ser in range(length(series)), series) {
        continue if (length(ser.ns) < MIN_BASELINE_POINTS)
        let key = "{ser.group}\t{ser.lane}"
        var gi = index_of?[key] ?? -1
        if (gi < 0) {
            gi = length(members)
            index_of[key] = gi
            keys_in_order |> push(key)
            var fresh_group : array<int>
            members |> emplace(fresh_group)
        }
        members[gi] |> push(si)
    }
    var out : array<IndexSeries>
    out |> reserve(length(keys_in_order))
    for (key, group_members in keys_in_order, members) {
        let tab = find(key, "\t")
        var idx <- IndexSeries(group = slice(key, 0, tab), lane = slice(key, tab + 1))
        let refs <- [for (si in group_members); reference_of(series[si])]
        for (ri in range(run_count)) {
            var ratios : array<double>
            for (mi, si in range(length(group_members)), group_members) {
                continue if (refs[mi] <= 0.0lf)
                for (pi in range(length(series[si].runs))) {
                    if (series[si].runs[pi] == ri) {
                        ratios |> push(series[si].ns[pi] / refs[mi])
                        break
                    }
                }
            }
            continue if (length(ratios) * 2 < length(group_members))
            idx.runs |> push(ri)
            idx.value |> push(geomean(ratios) * 100.0lf)
        }
        out |> emplace(idx)
    }
    sort(out) $(a, b) {
        return a.group != b.group ? a.group < b.group : a.lane < b.lane
    }
    return <- out
}

def build_dataset(runs : array<RunRecord>; cfg : SuiteConfig; generated, repo_url : string) : Dataset {
    var ds <- Dataset(generated = generated, repo_url = repo_url, regression_threshold = cfg.regression_threshold,
        noise_multiplier = cfg.noise_multiplier, baseline_runs = cfg.baseline_runs)
    ds.latest = latest_run_index(runs)
    var groups : table<string>
    ds.series <- build_series(runs, groups)
    ds.groups <- [for (g in keys(groups)); g]
    sort(ds.groups)
    ds.changes <- detect_changes(runs, ds.series, ds.latest, cfg)
    ds.index <- build_index(ds.series, length(runs))
    ds.runs |> reserve(length(runs))
    for (rec in runs) {
        ds.runs |> emplace(summarize_run(rec))
    }
    if (ds.latest >= 0) {
        for (ch in ds.changes) {
            if (ch.kind == ChangeKind.regression) {
                ds.runs[ds.latest].regressions++
            } elif (ch.kind == ChangeKind.improvement) {
                ds.runs[ds.latest].improvements++
            }
        }
    }
    return <- ds
}

def private count_kind(changes : array<Change>; kind : ChangeKind) : int {
    var n = 0
    for (ch in changes) {
        n++ if (ch.kind == kind)
    }
    return n
}

def private write_change_rows(var w : StringBuilderWriter; changes : array<Change>; kind : ChangeKind) {
    w |> write("| benchmark | lane | baseline ns/op | now ns/op | change | noise |\n|---|---|---:|---:|---:|---:|\n")
    for (ch in changes) {
        continue if (ch.kind != kind)
        w |> write("| `{ch.id}` | {ch.lane} | {ch.baseline:.1f} | {ch.value:.1f} | {ch.change * 100.0lf:+.1f}% | {ch.noise * 100.0lf:.1f}% |\n")
    }
    w |> write("\n")
}

//! The markdown the workflow posts as its step summary and the site serves as `summary.md`.
def render_summary(ds : Dataset; site_url : string) : string {
    return build_string() $(var w) {
        if (ds.latest < 0) {
            w |> write("# Benchmark stand\n\nNo run records yet.\n")
            return
        }
        assume run = ds.runs[ds.latest]
        w |> write("# Benchmark stand - {run.status} - {run.started}\n\n")
        // a hand-driven local run carries no commit and no machine; naming neither beats naming
        // both as empty backticks
        if (!empty(run.sha) || !empty(run.subject)) {
            let short_sha = length(run.sha) > 8 ? slice(run.sha, 0, 8) : run.sha
            w |> write("Commit `{short_sha}` {run.subject} ({run.date})")
            w |> write(empty(run.host) ? ".\n\n" : " on `{run.host}`.\n\n")
        } elif (!empty(run.host)) {
            w |> write("On `{run.host}`.\n\n")
        }
        if (run.build_status != "ok") {
            w |> write("## The build failed after {run.build_seconds:.0f} s - nothing was measured\n\n")
            w |> write(empty(run.build_log_tail)
                ? "The driver recorded no build log; `logs/build-{run.id}.log` on the box has it.\n\n"
                : "Last lines of the build log (`logs/build-{run.id}.log` on the box has all of it):\n\n```\n{run.build_log_tail}\n```\n\n")
            if (!empty(site_url)) {
                w |> write("Charts: {site_url}\n")
            }
            return
        }
        w |> write("Build {run.build_seconds:.0f} s, suite {run.seconds:.0f} s; {run.files_ok} file runs ok, {length(run.failures)} failed, {length(run.skipped)} skipped.\n\n")
        w |> write("Lanes:")
        for (lane, state in keys(run.lanes), values(run.lanes)) {
            w |> write(" {lane} = {state};")
        }
        w |> write("\n\n")
        if (!empty(run.failures)) {
            w |> write("## Failures ({length(run.failures)})\n\n| file | lane | status | message |\n|---|---|---|---|\n")
            for (f in run.failures) {
                w |> write("| `{f.path}` | {f.lane} | {f.status} | {replace(f.message, "|", "\\|")} |\n")
            }
            w |> write("\nEach failure's last log lines are in the run record `runs/{run.id}.json`.\n\n")
        }
        let regs = count_kind(ds.changes, ChangeKind.regression)
        let imps = count_kind(ds.changes, ChangeKind.improvement)
        let news = count_kind(ds.changes, ChangeKind.first_seen)
        let miss = count_kind(ds.changes, ChangeKind.missing)
        w |> write("## Changes: {regs} regressions, {imps} improvements, {news} first seen, {miss} missing\n\n")
        w |> write("A change counts when it passes both {ds.regression_threshold * 100.0lf:.0f}% and {ds.noise_multiplier:.1f}x the baseline noise; baseline = median of the previous {ds.baseline_runs} runs.\n\n")
        if (regs > 0) {
            w |> write("### Regressions\n\n")
            write_change_rows(w, ds.changes, ChangeKind.regression)
        }
        if (imps > 0) {
            w |> write("### Improvements\n\n")
            write_change_rows(w, ds.changes, ChangeKind.improvement)
        }
        if (miss > 0) {
            w |> write("### Missing tonight (measured before, file ran ok)\n\n")
            for (ch in ds.changes) {
                continue if (ch.kind != ChangeKind.missing)
                w |> write("- `{ch.id}` ({ch.lane})\n")
            }
            w |> write("\n")
        }
        if (!empty(run.skipped)) {
            w |> write("## Skipped ({length(run.skipped)})\n\n")
            for (f in run.skipped) {
                w |> write("- `{f.path}` ({f.lane}): {f.message}\n")
            }
            w |> write("\n")
        }
        if (!empty(site_url)) {
            w |> write("Charts: {site_url}\n")
        }
    }
}

//! 0 when the latest run is ok; 1 when it failed (build or any file); 2 when it is ok but
//! regressed and the caller asked regressions to fail.
def summary_exit_code(ds : Dataset; fail_on_regression : bool) : int {
    return 1 if (ds.latest < 0 || ds.runs[ds.latest].status != RunStatus.ok)
    return 2 if (fail_on_regression && ds.runs[ds.latest].regressions > 0)
    return 0
}
