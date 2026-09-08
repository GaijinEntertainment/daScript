options gen2

require dastest/testing_boost public
require bench_history
require bench_runner
require bench_suite
require daslib/fio
require daslib/json_boost
require strings
require daslib/strings_boost
require math

def private with_temp_dir(prefix : string; blk : block<(dir : string) : void>) {
    var err = ""
    let dir = create_temp_directory(prefix, err)
    verify(!empty(dir), "temp dir created")
    invoke(blk, dir)
    rmdir_rec(dir)
}

def private sample(id : string; ns : double) : Sample {
    return Sample(id = id, ns = ns, ns_median = ns, spread = 0.05lf, runs = 3)
}

def private file_ok(id, lane : string; samples : array<Sample>) : FileResult {
    var f <- FileResult(path = "{id}.das", id = id, group = dir_name(id), lane = lane, status = FileStatus.ok, tier = lane == "jit" ? "JIT" : "INTERP")
    f.samples := samples
    return <- f
}

def private file_bad(id, lane : string; status : FileStatus; message : string) : FileResult {
    return <- FileResult(path = "{id}.das", id = id, group = dir_name(id), lane = lane, status = status, message = message)
}

//! Night `n` (0-based): commit date and run start both increase with n, so commit order equals run order.
def private night(n : int; files : array<FileResult>) : RunRecord {
    var rec <- RunRecord(run_id = "r{n:02}", started = "2026-09-{n + 1:02}T03:00:00Z", finished = "2026-09-{n + 1:02}T04:00:00Z", seconds = 3600.0lf,
        commit = CommitInfo(sha = "sha{n:02}abcdef", date = "2026-09-{n + 1:02}T00:00:00Z", subject = "commit {n}", author = "dev"),
        machine = MachineInfo(host = "box", cpu = "cpu", cores = 4), build = BuildInfo(status = "ok", seconds = 600.0lf))
    rec.lanes["interp"] = "ok"
    rec.lanes["jit"] = "ok"
    rec.files := files
    return <- rec
}

def private write_night(dir : string; rec : RunRecord) {
    fwrite(path_join(dir, "{rec.run_id}.json"), sprint_json(rec, false))
}

def private load(dir : string) : array<RunRecord> {
    var errors : array<string>
    return <- load_runs(dir, errors)
}

//! The steady week plus an eighth night where a/x jumps +30% (interp) and drops -20% (jit), c/z
//! appears, b/y vanishes, and sort/s fails to compile.
def private eight_nights_with_a_jump(dir : string) {
    steady_nights(dir, 7)
    write_night(dir, night(7, [
        file_ok("core/hash/t", "interp", [sample("a/x", 130.0lf), sample("c/z", 5.0lf)]),
        file_ok("core/hash/t", "jit", [sample("a/x", 40.0lf)]),
        file_bad("sort/s", "interp", FileStatus.compile_error, "error[30344]: mismatch")]))
}

def private cfg() : SuiteConfig {
    var c = SuiteConfig()
    c.lanes <- ["interp", "jit"]
    return <- c
}

//! `count` nights near 100 ns (interp) and 50 ns (jit) for `core/hash/t#a/x` and near 200 ns for
//! `b/y`, jittered so the baseline has a nonzero MAD - the noise gate needs one to be exercised.
def private steady_nights(dir : string; count : int) {
    for (n in range(count)) {
        let jitter = double(n % 3) * 0.5lf
        write_night(dir, night(n, [
            file_ok("core/hash/t", "interp", [sample("a/x", 100.0lf + jitter), sample("b/y", 200.0lf - jitter)]),
            file_ok("core/hash/t", "jit", [sample("a/x", 50.0lf + jitter)])]))
    }
}

[test]
def test_load_runs(t : T?) {
    with_temp_dir("bench_hist_") $(dir) {
        t |> run("records load in commit order, a corrupt file is named and skipped, status derives") @(tt : T?) {
            write_night(dir, night(2, [file_ok("core/t", "interp", [sample("a/x", 1.0lf)])]))
            write_night(dir, night(0, [file_ok("core/t", "interp", [sample("a/x", 1.0lf)])]))
            let broken <- night(1, [file_bad("core/t", "interp", FileStatus.timeout, "killed")])
            write_night(dir, broken)
            fwrite(path_join(dir, "zz_corrupt.json"), "\{ not json")
            fwrite(path_join(dir, "notes.txt"), "ignored")
            var errors : array<string>
            let runs <- load_runs(dir, errors)
            tt |> equal(length(runs), 3)
            tt |> equal(runs[0].run_id, "r00")
            tt |> equal(runs[1].run_id, "r01")
            tt |> equal(runs[2].run_id, "r02")
            tt |> equal(runs[1].status, RunStatus.bench_failed)
            tt |> equal(runs[0].status, RunStatus.ok)
            tt |> equal(runs[0].files[0].status, FileStatus.ok)
            tt |> equal(length(errors), 1)
            tt |> success(find(errors[0], "zz_corrupt.json") >= 0, "corrupt file named")
        }
        t |> run("a night that measured nothing is not ok, whatever emptied it") @(tt : T?) {
            let no_files : array<FileResult>
            let empty_night <- night(30, no_files)
            tt |> equal(derive_status(empty_night), RunStatus.bench_failed,
                "no file ran at all - every lane unavailable, or a filter that matched nothing")
            let only_skips <- night(31, [file_bad("core/x", "interp", FileStatus.skipped, "listed")])
            tt |> equal(derive_status(only_skips), RunStatus.bench_failed, "a night of nothing but skips carries no data point")
            let one_ok <- night(32, [file_ok("core/x", "interp", [sample("a/x", 1.0lf)])])
            tt |> equal(derive_status(one_ok), RunStatus.ok, "one measured file is a night")
        }
        t |> run("a build failure written by the driver derives build_failed") @(tt : T?) {
            let no_files : array<FileResult>
            var rec <- night(5, no_files)
            rec.build.status = "failed"
            tt |> equal(derive_status(rec), RunStatus.build_failed)
        }
    }
}

[test]
def test_statistics(t : T?) {
    t |> run("median, relative MAD, geomean") @(tt : T?) {
        var odd <- [3.0lf, 1.0lf, 2.0lf]
        tt |> success(abs(median(odd) - 2.0lf) < 1e-9lf, "odd median")
        var even <- [4.0lf, 1.0lf, 3.0lf, 2.0lf]
        tt |> success(abs(median(even) - 2.5lf) < 1e-9lf, "even median")
        tt |> success(abs(relative_mad([100.0lf, 110.0lf, 90.0lf, 100.0lf, 105.0lf]) - 0.05lf) < 1e-9lf, "MAD 5 over median 100")
        tt |> success(abs(geomean([1.0lf, 4.0lf]) - 2.0lf) < 1e-9lf, "geomean of 1 and 4")
        var none : array<double>
        tt |> equal(median(none), 0.0lf)
    }
}

[test]
def test_build_dataset(t : T?) {
    with_temp_dir("bench_hist_") $(dir) {
        eight_nights_with_a_jump(dir)
        t |> run("series are keyed by lane and arm, points carry the run index") @(tt : T?) {
            let runs <- load(dir)
            let ds <- build_dataset(runs, cfg(), "2026-09-08T05:00:00Z", "https://github.com/x/y")
            tt |> equal(length(ds.runs), 8)
            tt |> equal(ds.latest, 7)
            tt |> equal(length(ds.groups), 1)
            tt |> equal(ds.groups[0], "core/hash")
            tt |> equal(length(ds.series), 4)
            tt |> equal(ds.series[0].lane, "interp")
            tt |> equal(ds.series[0].id, "core/hash/t#a/x")
            tt |> equal(length(ds.series[0].runs), 8)
            tt |> equal(ds.series[0].runs[7], 7)
            tt |> success(abs(ds.series[0].ns[7] - 130.0lf) < 1e-9lf, "tonight's value")
            tt |> equal(ds.series[3].lane, "jit")
        }
        t |> run("a 30% jump over a quiet baseline is a regression, a 20% drop an improvement, a first sighting is new") @(tt : T?) {
            let runs <- load(dir)
            let ds <- build_dataset(runs, cfg(), "", "")
            let kinds <- { for (ch in ds.changes); "{ch.lane} {ch.id}" => "{ch.kind}" }
            tt |> equal(kinds?["interp core/hash/t#a/x"] ?? "", "regression")
            tt |> equal(kinds?["jit core/hash/t#a/x"] ?? "", "improvement")
            tt |> equal(kinds?["interp core/hash/t#c/z"] ?? "", "first_seen")
            tt |> equal(kinds?["interp core/hash/t#b/y"] ?? "", "missing")
            tt |> equal(length(ds.changes), 4)
            for (ch in ds.changes) {
                if (ch.kind == ChangeKind.regression) {
                    tt |> success(abs(ch.baseline - 100.5lf) < 1e-9lf, "baseline is the median of the 7 prior points: {ch.baseline}")
                    tt |> success(ch.change > 0.29lf && ch.change < 0.30lf, "change near +29%: {ch.change}")
                }
            }
            tt |> equal(ds.runs[7].regressions, 1)
            tt |> equal(ds.runs[7].improvements, 1)
        }
        t |> run("a series with fewer than three prior points gets no verdict") @(tt : T?) {
            var two : array<RunRecord>
            two |> reserve(3)
            for (n in range(3)) {
                two |> emplace(night(n, [file_ok("core/hash/t", "interp", [sample("a/x", n == 2 ? 300.0lf : 100.0lf)])]))
            }
            let ds <- build_dataset(two, cfg(), "", "")
            tt |> equal(length(ds.changes), 0)
            tt |> equal(ds.runs[2].regressions, 0)
        }
        t |> run("a run measuring fewer than half a group's series contributes no index point") @(tt : T?) {
            var nights : array<RunRecord>
            nights |> reserve(4)
            for (n in range(4)) {
                var arms <- [sample("a/x", 100.0lf), sample("b/y", 200.0lf), sample("c/z", 300.0lf)]
                if (n == 3) {
                    arms |> resize(1)
                }
                nights |> emplace(night(n, [file_ok("core/hash/t", "interp", arms)]))
            }
            let ds <- build_dataset(nights, cfg(), "", "")
            tt |> equal(length(ds.index), 1)
            tt |> equal(length(ds.index[0].runs), 3)
            tt |> equal(ds.index[0].runs[2], 2)
        }
    }
}

[test]
def test_change_gates(t : T?) {
    with_temp_dir("bench_hist_") $(dir) {
        eight_nights_with_a_jump(dir)
        t |> run("a noisy baseline widens the gate past the threshold, so the same jump is no verdict") @(tt : T?) {
            // the prior week swings +-20%, so 3x its MAD is a 60% gate and a 30% jump is inside it
            var noisy : array<RunRecord>
            noisy |> reserve(8)
            let swing <- [100.0lf, 120.0lf, 80.0lf, 120.0lf, 80.0lf, 120.0lf, 80.0lf]
            for (n, v in range(7), swing) {
                noisy |> emplace(night(n, [file_ok("core/hash/t", "interp", [sample("a/x", v)])]))
            }
            noisy |> emplace(night(7, [file_ok("core/hash/t", "interp", [sample("a/x", 130.0lf)])]))
            let ds <- build_dataset(noisy, cfg(), "", "")
            tt |> equal(length(ds.changes), 0, "a 30% move over a +-20% baseline is noise, not a regression")
            var quiet <- cfg()
            quiet.noise_multiplier = 0.0lf
            let ds_quiet <- build_dataset(noisy, quiet, "", "")
            tt |> equal(length(ds_quiet.changes), 1, "with the noise term off, the threshold alone calls it")
            tt |> equal(ds_quiet.changes[0].kind, ChangeKind.regression)
            tt |> success(ds_quiet.changes[0].noise > 0.15lf, "the recorded noise is the baseline's own: {ds_quiet.changes[0].noise}")
        }
        t |> run("the baseline is the last baseline_runs points, not every point on record") @(tt : T?) {
            // four quiet nights at 10 ns, then three at 100: a window of 3 makes 100 the baseline
            var shifted : array<RunRecord>
            shifted |> reserve(8)
            for (n in range(7)) {
                shifted |> emplace(night(n, [file_ok("core/hash/t", "interp", [sample("a/x", n < 4 ? 10.0lf : 100.0lf)])]))
            }
            shifted |> emplace(night(7, [file_ok("core/hash/t", "interp", [sample("a/x", 105.0lf)])]))
            var windowed <- cfg()
            windowed.baseline_runs = 3
            let ds <- build_dataset(shifted, windowed, "", "")
            tt |> equal(length(ds.changes), 0, "105 against a 100 baseline is inside the gate")
            let ds_wide <- build_dataset(shifted, cfg(), "", "")
            tt |> equal(length(ds_wide.changes), 1, "a seven-run window makes the baseline 10 and the night a regression")
            tt |> success(ds_wide.changes[0].baseline < 20.0lf, "the wide baseline is the old level: {ds_wide.changes[0].baseline}")
        }
        t |> run("a move inside the threshold or inside the noise band is not a change") @(tt : T?) {
            let runs <- load(dir)
            var c <- cfg()
            c.regression_threshold = 0.5lf
            let ds <- build_dataset(runs, c, "", "")
            var regs = 0
            for (ch in ds.changes) {
                regs++ if (ch.kind == ChangeKind.regression || ch.kind == ChangeKind.improvement)
            }
            tt |> equal(regs, 0)
        }
    }
}

[test]
def test_build_dataset_index_and_summary_rows(t : T?) {
    with_temp_dir("bench_hist_") $(dir) {
        eight_nights_with_a_jump(dir)
        t |> run("the group index is 100 at the reference and moves with the group") @(tt : T?) {
            let runs <- load(dir)
            let ds <- build_dataset(runs, cfg(), "", "")
            tt |> equal(length(ds.index), 2)
            tt |> equal(ds.index[0].group, "core/hash")
            tt |> equal(ds.index[0].lane, "interp")
            tt |> equal(length(ds.index[0].runs), 8)
            tt |> success(abs(ds.index[0].value[0] - 100.0lf) < 1.0lf, "first night near 100: {ds.index[0].value[0]}")
            tt |> success(ds.index[0].value[7] > 105.0lf, "tonight's regression lifts the index: {ds.index[0].value[7]}")
            tt |> equal(ds.index[1].lane, "jit")
            tt |> success(ds.index[1].value[7] < 90.0lf, "jit improvement lowers its index: {ds.index[1].value[7]}")
        }
        t |> run("the run summary carries failures with their message and the ok count") @(tt : T?) {
            let runs <- load(dir)
            let ds <- build_dataset(runs, cfg(), "", "")
            tt |> equal(ds.runs[7].status, RunStatus.bench_failed)
            tt |> equal(ds.runs[7].files_ok, 2)
            tt |> equal(length(ds.runs[7].failures), 1)
            tt |> equal(ds.runs[7].failures[0].path, "sort/s.das")
            tt |> equal(ds.runs[7].failures[0].status, "compile_error")
            tt |> equal(ds.runs[7].failures[0].message, "error[30344]: mismatch")
            tt |> equal(ds.runs[7].lanes?["jit"] ?? "", "ok")
        }
        t |> run("the dataset round-trips through JSON") @(tt : T?) {
            let runs <- load(dir)
            let ds <- build_dataset(runs, cfg(), "now", "")
            var back = Dataset()
            tt |> success(sscan_json(sprint_json(ds, false), back), "parses back")
            tt |> equal(length(back.series), 4)
            tt |> equal(back.latest, 7)
            tt |> equal(length(back.changes), 4)
        }
    }
}

[test]
def test_summary(t : T?) {
    with_temp_dir("bench_hist_") $(dir) {
        steady_nights(dir, 7)
        t |> run("a clean night renders ok and exits 0") @(tt : T?) {
            var errors : array<string>
            let runs <- load_runs(dir, errors)
            let ds <- build_dataset(runs, cfg(), "", "")
            let md = render_summary(ds, "https://daslang.io/bench/")
            tt |> success(starts_with(md, "# Benchmark stand - ok - "), "title says ok: {slice(md, 0, 40)}")
            tt |> success(find(md, "0 regressions, 0 improvements") >= 0, "counts zero changes")
            tt |> success(find(md, "https://daslang.io/bench/") >= 0, "links the site")
            tt |> equal(summary_exit_code(ds, true), 0)
        }
        t |> run("a failed night names the file and exits 1; a regression exits 2 only on request") @(tt : T?) {
            write_night(dir, night(7, [
                file_ok("core/hash/t", "interp", [sample("a/x", 150.0lf), sample("b/y", 200.0lf)]),
                file_ok("core/hash/t", "jit", [sample("a/x", 50.0lf)]),
                file_bad("sort/s", "jit", FileStatus.timeout, "killed after 900 s (timeout_seconds)")]))
            var errors : array<string>
            let runs <- load_runs(dir, errors)
            let ds <- build_dataset(runs, cfg(), "", "")
            let md = render_summary(ds, "")
            tt |> success(find(md, "`sort/s.das` | jit | timeout | killed after 900 s") >= 0, "failure row present")
            tt |> success(find(md, "### Regressions") >= 0, "regression section present")
            tt |> success(find(md, "`core/hash/t#a/x` | interp | 100.5 | 150.0 | +49.3%") >= 0, "regression row formatted: {md}")
            tt |> equal(summary_exit_code(ds, false), 1)
            var clean := runs[7]
            clean.files |> erase(2)
            write_night(dir, clean)
            var errors2 : array<string>
            let runs2 <- load_runs(dir, errors2)
            let ds2 <- build_dataset(runs2, cfg(), "", "")
            tt |> equal(summary_exit_code(ds2, false), 0)
            tt |> equal(summary_exit_code(ds2, true), 2)
        }
    }
}

[test]
def test_summary_of_a_night_that_failed(t : T?) {
    with_temp_dir("bench_hist_") $(dir) {
        t |> run("a build failure leads with its reason and the build log tail, and nothing else") @(tt : T?) {
            let no_files : array<FileResult>
            var rec <- night(7, no_files)
            rec.build.status = "failed"
            rec.build.seconds = 41.0lf
            rec.build.log_tail = "ninja: error: unknown target 'daslang'"
            write_night(dir, rec)
            let ds <- build_dataset(load(dir), cfg(), "", "https://daslang.io/bench/")
            let md = render_summary(ds, "https://daslang.io/bench/")
            tt |> success(find(md, "build_failed") >= 0, "the title names the status")
            tt |> success(find(md, "## The build failed after 41 s - nothing was measured") >= 0, "the build section leads: {md}")
            tt |> success(find(md, "unknown target 'daslang'") >= 0, "the log tail is quoted")
            tt |> success(find(md, "## Changes") < 0, "no change section on a night that measured nothing")
            tt |> success(find(md, "https://daslang.io/bench/") >= 0, "the charts link survives")
            tt |> equal(summary_exit_code(ds, false), 1)
            remove(path_join(dir, "{rec.run_id}.json"))
        }
        t |> run("improvements, first sightings, missing arms, skips and a dark lane all render") @(tt : T?) {
            eight_nights_with_a_jump(dir)
            var rec <- night(8, [
                file_ok("core/hash/t", "interp", [sample("a/x", 40.0lf), sample("d/w", 7.0lf)]),
                file_bad("core/hash/slow", "interp", FileStatus.skipped, "takes an hour")])
            rec.lanes["jit"] = "daslang -jit exited with code 1: no dasLLVM in this build"
            write_night(dir, rec)
            let ds <- build_dataset(load(dir), cfg(), "", "")
            let md = render_summary(ds, "")
            tt |> success(find(md, "### Improvements") >= 0, "improvement section")
            tt |> success(find(md, "`core/hash/t#a/x` | interp | 100.5 | 40.0 | -60.2%") >= 0, "improvement row: {md}")
            tt |> success(find(md, "### Missing tonight") >= 0, "missing section")
            tt |> success(find(md, "`core/hash/t#b/y`") >= 0, "the missing arm is named")
            tt |> success(find(md, "first seen") >= 0, "first sightings counted")
            tt |> success(find(md, "## Skipped (1)") >= 0, "skip section")
            tt |> success(find(md, "`core/hash/slow.das` (interp): takes an hour") >= 0, "the skip states its reason")
            tt |> success(find(md, "jit = daslang -jit exited with code 1") >= 0, "a dark lane says why")
            tt |> success(find(md, "1 file runs ok, 0 failed, 1 skipped") >= 0, "the counts line")
            remove(path_join(dir, "{rec.run_id}.json"))
        }
        t |> run("a night that never benchmarked names the driver as the failure") @(tt : T?) {
            var rec <- night(9, [file_ok("core/hash/t", "interp", [sample("a/x", 100.0lf)])])
            mark_failed_run(rec, "2026-09-10T04:00:00Z", "the tool exited 1 without writing a record")
            tt |> equal(rec.status, RunStatus.bench_failed)
            tt |> equal(length(rec.files), 1)
            tt |> equal(rec.files[0].lane, "driver")
            tt |> success(find(rec.files[0].message, "exited 1") >= 0, "the reason survives")
            tt |> equal(rec.seconds, 0.0lf)
            tt |> equal(rec.finished, "2026-09-10T04:00:00Z")
            let none_either : array<FileResult>
            var build_broke <- night(10, none_either)
            build_broke.build.status = "failed"
            mark_failed_run(build_broke, "2026-09-11T04:00:00Z", "")
            tt |> equal(build_broke.status, RunStatus.build_failed)
            tt |> success(empty(build_broke.files), "a failed build carries no file - its log tail is the reason")
        }
        t |> run("a run with no commit names no commit, rather than empty backticks") @(tt : T?) {
            let no_files : array<FileResult>
            var bare <- night(20, no_files)
            bare.commit = CommitInfo()
            bare.machine = MachineInfo()
            var one : array<RunRecord>
            one |> emplace(bare)
            let md = render_summary(build_dataset(one, cfg(), "", ""), "")
            tt |> success(find(md, "Commit ``") < 0, "no empty commit line: {md}")
            tt |> success(find(md, " on ``") < 0, "and no empty host clause")
        }
        t |> run("no records renders a note and exits 1") @(tt : T?) {
            let none : array<RunRecord>
            let ds <- build_dataset(none, cfg(), "", "")
            tt |> success(find(render_summary(ds, ""), "No run records") >= 0, "note")
            tt |> equal(summary_exit_code(ds, false), 1)
        }
    }
}
