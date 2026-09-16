options gen2

require dastest/testing_boost public
require ../bench_history.das
require ../benchstat.das
require ../bench_runner.das
require _test_common
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require strings
require math

def private sample(id : string; ns : double) : Sample {
    return Sample(id = id, ns = ns, ns_median = ns, spread = 0.05lf, runs = 3)
}

def private file_ok(id, lane : string; samples : array<Sample>) : FileResult {
    var f <- FileResult(path = "{id}.das", id = id, group = dir_name(id), lane = lane, status = FileStatus.ok)
    f.samples := samples
    return <- f
}

def private file_bad(id, lane : string; status : FileStatus; message : string) : FileResult {
    return <- FileResult(path = "{id}.das", id = id, group = dir_name(id), lane = lane, status = status, message = message)
}

def private night(n : int; files : array<FileResult>) : RunRecord {
    var rec <- RunRecord(run_id = "r{n:02}", started = "2026-09-{n + 1:02}T03:00:00Z", finished = "2026-09-{n + 1:02}T04:00:00Z", seconds = 3600.0lf,
        commit = CommitInfo(sha = "sha{n:02}abcdef", date = "2026-09-{n + 1:02}T00:00:00Z", subject = "commit {n}", author = "dev"),
        machine = MachineInfo(host = "box", cores = 4), build = BuildInfo(status = "ok", seconds = 600.0lf))
    rec.lanes["interp"] = "ok"
    rec.lanes["jit"] = "ok"
    rec.files := files
    rec.status = derive_status(rec)
    return <- rec
}

def private eight_nights_with_a_jump() : array<RunRecord> {
    var runs <- [for (n in range(7)); night(n, [
        file_ok("core/hash/t", "interp", [sample("a/x", 100.0lf + double(n % 3) * 0.5lf), sample("b/y", 200.0lf - double(n % 3) * 0.5lf)]),
        file_ok("core/hash/t", "jit", [sample("a/x", 50.0lf + double(n % 3) * 0.5lf)])])]
    runs |> emplace(night(7, [
        file_ok("core/hash/t", "interp", [sample("a/x", 130.0lf), sample("c/z", 5.0lf)]),
        file_ok("core/hash/t", "jit", [sample("a/x", 40.0lf)]),
        file_bad("sort/s", "interp", FileStatus.compile_error, "error[30344]: mismatch")]))
    return <- runs
}

[test]
def test_load_runs(t : T?) {
    with_temp_dir("bench_hist_") $(dir) {
        t |> run("records load in commit order, a corrupt file is named and skipped, status derives") @(tt : T?) {
            for (rec in [night(2, [file_ok("core/t", "interp", [sample("a/x", 1.0lf)])]), night(0, [file_ok("core/t", "interp", [sample("a/x", 1.0lf)])]), night(1, [file_bad("core/t", "interp", FileStatus.timeout, "killed")])]) {
                fwrite(path_join(dir, "{rec.run_id}.json"), sprint_json(rec, false))
            }
            fwrite(path_join(dir, "zz_corrupt.json"), "\{ not json")
            fwrite(path_join(dir, "notes.txt"), "ignored")
            var errors : array<string>
            let runs <- load_runs(dir, errors)
            tt |> equal(join([for (r in runs); r.run_id], " "), "r00 r01 r02")
            tt |> equal(runs[1].status, RunStatus.bench_failed)
            tt |> equal(runs[0].status, RunStatus.ok)
            tt |> equal(length(errors), 1)
            tt |> success(find(errors[0], "zz_corrupt.json") >= 0, "corrupt file named")
        }
        t |> run("a night that measured nothing is not ok, a failed build is build_failed, a driver failure names the driver") @(tt : T?) {
            let no_files : array<FileResult>
            tt |> equal(derive_status(night(30, no_files)), RunStatus.bench_failed, "no file ran at all")
            tt |> equal(derive_status(night(31, [file_bad("core/x", "interp", FileStatus.skipped, "listed")])), RunStatus.bench_failed, "nothing but skips is no data point")
            tt |> equal(derive_status(night(32, [file_ok("core/x", "interp", [sample("a/x", 1.0lf)])])), RunStatus.ok, "one measured file is a night")
            var rec <- night(5, no_files)
            rec.build.status = "failed"
            tt |> equal(derive_status(rec), RunStatus.build_failed)
            var driver <- night(6, no_files)
            mark_failed_run(driver, "now", "the tool crashed")
            tt |> equal(driver.status, RunStatus.bench_failed)
            tt |> equal(length(driver.files), 1)
        }
    }
}

[test]
def test_build_dataset(t : T?) {
    t |> run("series are keyed by lane and arm, points carry the run index; the dataset round-trips") @(tt : T?) {
        let ds <- build_dataset(eight_nights_with_a_jump(), "2026-09-08T05:00:00Z", "https://github.com/x/y")
        tt |> equal(length(ds.runs), 8)
        tt |> equal(ds.latest, 7)
        tt |> equal(join(ds.groups, " "), "core/hash")
        tt |> equal(length(ds.series), 4)
        tt |> equal(ds.series[0].lane, "interp")
        tt |> equal(ds.series[0].id, "core/hash/t#a/x")
        tt |> equal(length(ds.series[0].runs), 8)
        tt |> success(abs(ds.series[0].ns[7] - 130.0lf) < 1e-9lf, "tonight's value")
        tt |> equal(ds.series[3].lane, "jit")
        var back = Dataset()
        tt |> success(sscan_json(sprint_json(ds, false), back), "parses back")
        tt |> equal(length(back.series), 4)
        tt |> equal(back.runs[7].commit.sha, "sha07abcdef")
    }
    t |> run("the run summary carries failures with their message and the ok count; a failed night exits 1") @(tt : T?) {
        let ds <- build_dataset(eight_nights_with_a_jump(), "", "")
        tt |> equal(ds.runs[7].status, RunStatus.bench_failed)
        tt |> equal(ds.runs[7].files_ok, 2)
        tt |> equal(length(ds.runs[7].failures), 1)
        tt |> equal(ds.runs[7].failures[0].path, "sort/s.das")
        tt |> equal(ds.runs[7].failures[0].status, "compile_error")
        tt |> equal(ds.runs[7].failures[0].message, "error[30344]: mismatch")
        tt |> equal(report_verdict(ds), StandExit.failed)
    }
}
