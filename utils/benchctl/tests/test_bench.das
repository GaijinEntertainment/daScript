options gen2

require dastest/testing_boost public
require ../bench_history.das
require ../bench_runner.das
require _test_common
require daslib/fio
require daslib/clargs
require daslib/json_boost
require ../benchstat.das
require daslib/strings_boost
require strings
require math
require ../bench_suite.das

let private TOOL_DIR = "utils/benchctl"

def private write_meta(dir : string; build_status : string) : string {
    let path = path_join(dir, "meta.json")
    fwrite(path, write_json(JV((run_id = "20260908T030000Z-abcdef12", started = "2026-09-08T03:00:00Z",
        commit = JV((sha = "abcdef1234567890", date = "2026-09-08T01:00:00Z", subject = "a commit", author = "dev")),
        machine = JV((host = "box", cores = 4)),
        build = JV((status = build_status, seconds = 12, log_tail = "ninja: build stopped"))))))
    return path
}

def private run_tool(args : array<string>; var output : string&) : int {
    var argv <- [get_host_binary(), path_join(TOOL_DIR, "main.das"), "--"]
    argv |> push_from(args)
    return run_and_capture(argv, output, 240.0)
}

def private write_run_record(runs_dir : string; n : int; ns : double) {
    var rec <- RunRecord(run_id = "r{n}", started = "2026-09-0{n + 1}T03:00:00Z", commit = CommitInfo(sha = "sha{n}", date = "2026-09-0{n + 1}T01:00:00Z", subject = "c{n}"),
        machine = MachineInfo(host = "box"), build = BuildInfo(status = "ok", seconds = 1.0lf))
    rec.lanes["interp"] = "ok"
    var f <- FileResult(path = "core/one.das", id = "core/one", group = "core", lane = "interp", status = FileStatus.ok)
    f.samples |> emplace(Sample(id = "b/arm", ns = ns, ns_median = ns, runs = 1))
    rec.files |> emplace(f)
    fwrite(path_join(runs_dir, "r{n}.json"), sprint_json(rec, false))
}

[test(description = "the run verb records one file per lane and exits on the night's verdict, --failed records a night that never ran and report exits 0 or 1", timeout = 1200)]
def test_verbs(t : T?) {
    with_temp_dir("bench_cli_") $(dir) {
        let suite = path_join(dir, "suite.json")
        fwrite(suite, write_json(JV((root = "benchmarks", exclude = JV(["**/_*.das"]), lanes = JV(["interp"]), repeat = 1, timeout_seconds = 120))))
        t |> run("run records one file per lane and exits on the night's verdict; --failed records a night that never ran") @(tt : T?) {
            fake_repo(dir, "ok")
            fwrite(path_join(dir, "benchmarks/x/z.das"), "options gen2\n")
            let record = path_join(dir, "run.json")
            var out = ""
            let code = run_tool(["run", "--root", dir, "--bin", get_host_binary(), "--suite", suite, "--meta", write_meta(dir, "ok"), "--out", record, "--filter", "y.das"], out)
            tt |> equal(code, 0, "exit 0 when every file ran: {out}")
            var rec = RunRecord()
            tt |> success(sscan_json(fread(record), rec), "the record parses")
            tt |> equal(rec.status, RunStatus.ok)
            tt |> equal(length(rec.files), 1, "--filter narrowed two files to one")
            tt |> equal(rec.files[0].path, "x/y.das")
            tt |> equal(length(rec.files[0].samples), 1)
            let failed = path_join(dir, "failed.json")
            tt |> equal(run_tool(["run", "--root", dir, "--bin", get_host_binary(), "--suite", suite, "--meta", write_meta(dir, "failed"), "--out", failed, "--failed", "the build failed"], out), 1)
            tt |> success(sscan_json(fread(failed), rec) && rec.status == RunStatus.build_failed && empty(rec.files), "a failed build's record carries no file")
            tt |> equal(run_tool(["run", "--root", dir, "--out", record], out), 1, "a missing --meta exits 1")
        }
        t |> run("report exits 0 on a clean night and 1 with no records") @(tt : T?) {
            let runs_dir = path_join(dir, "runs")
            mkdir_rec(runs_dir)
            var out = ""
            tt |> equal(run_tool(["report", "--suite", suite, "--runs", runs_dir, "--out-data", path_join(dir, "d0.json")], out), 1, "no records yet")
            for (n in range(4)) {
                write_run_record(runs_dir, n, 100.0lf)
            }
            let data = path_join(dir, "data.json")
            tt |> equal(run_tool(["report", "--suite", suite, "--runs", runs_dir, "--out-data", data, "--repo-url", "https://example.invalid/repo"], out), 0, "a clean latest night: {out}")
            var ds = Dataset()
            tt |> success(sscan_json(fread(data), ds), "data.json parses")
            tt |> equal(length(ds.runs), 4)
            tt |> equal(length(ds.series), 1)
            tt |> equal(ds.repo_url, "https://example.invalid/repo")
        }
    }
}

def private sample(id : string; ns : double) : Sample =>
    Sample(id = id, ns = ns, ns_median = ns, spread = 0.05lf, runs = 3)

def private file_ok(id, lane : string; samples : array<Sample>) : FileResult {
    var f <- FileResult(path = "{id}.das", id = id, group = dir_name(id), lane = lane, status = FileStatus.ok)
    f.samples := samples
    return <- f
}

def private file_bad(id, lane : string; status : FileStatus; message : string) : FileResult =>
    <- FileResult(path = "{id}.das", id = id, group = dir_name(id), lane = lane, status = status, message = message)

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

[test(description = "run records load in commit order, a corrupt file is named and skipped, and a night's status derives from its results")]
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

[test(description = "dataset series are keyed by lane and arm with run-indexed points, and the run summary carries failures and the ok count")]
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

def private fresh_result(status : FileStatus = FileStatus.ok) : FileResult =>
    <- FileResult(path = "core/x.das", id = "core/x", group = "core", lane = "interp", status = status)

def private parsed(lines : array<string>; report : string; exit_code : int; status : FileStatus = FileStatus.ok) : FileResult {
    var res <- fresh_result(status)
    classify_run_output(lines, report, exit_code, res)
    return <- res
}

[test(description = "a run's output classifies into samples, compile errors, failed functions, shutdown deaths and timeouts")]
def test_classify_run_output(t : T?) {
    t |> run("samples aggregate over repeats: min, median, spread, per-op allocation medians") @(tt : T?) {
        let res <- parsed([stats_line("b", "insert", 10, 1000l, 20), stats_line("b", "insert", 10, 1200l, 30), stats_line("b", "insert", 10, 1100l, 40), "noise"], report_json(["b"], []), 0)
        tt |> equal(res.status, FileStatus.ok)
        tt |> equal(length(res.samples), 1)
        tt |> equal(res.samples[0].id, "b/insert")
        tt |> equal(res.samples[0].ns, 100.0lf)
        tt |> equal(res.samples[0].ns_median, 110.0lf)
        tt |> success(res.samples[0].spread > 0.19lf && res.samples[0].spread < 0.21lf, "spread is (max - min) / min")
        tt |> equal(res.samples[0].runs, 3)
        tt |> equal(res.samples[0].allocs, 3l)
        tt |> equal(res.log_tail, "noise")
    }
    t |> run("a compile error names the first error line") @(tt : T?) {
        let res <- parsed(["x.das:3:1: error[30001]: bad", "Failed to compile x.das"], "", 1)
        tt |> equal(res.status, FileStatus.compile_error)
        tt |> success(find(res.message, "error[30001]") >= 0, "message carries the error line")
    }
    t |> run("a failed benchmark function drops its arms and names itself; the others survive") @(tt : T?) {
        let res <- parsed([stats_line("good", "a", 10, 100l), stats_line("bad", "b", 10, 100l), "--- FAIL 'bad' in 'x.das'"], report_json(["good", "bad"], ["bad"]), 1)
        tt |> equal(res.status, FileStatus.failed)
        tt |> equal(length(res.samples), 1)
        tt |> equal(res.samples[0].id, "good/a")
        tt |> success(find(res.message, "bad") >= 0, "message names the failed function")
    }
    t |> run("a measured file that dies at shutdown keeps its numbers; one with no finished report is a failure") @(tt : T?) {
        let shutdown <- parsed([stats_line("b", "a", 10, 100l)], report_json(["b"], []), 1)
        tt |> equal(shutdown.status, FileStatus.ok)
        tt |> success(find(shutdown.message, "shutdown") >= 0, "message says the shutdown failed")
        let crashed <- parsed([stats_line("b", "a", 10, 100l), "FATAL: boom"], "", 139)
        tt |> equal(crashed.status, FileStatus.exit_nonzero)
        tt |> equal(crashed.message, "FATAL: boom")
    }
    t |> run("a timeout verdict survives whatever the output says; a clean exit with no samples is not ok") @(tt : T?) {
        let killed <- parsed([stats_line("b", "a", 10, 100l)], report_json(["b"], []), 0, FileStatus.timeout)
        tt |> equal(killed.status, FileStatus.timeout)
        tt |> equal(length(killed.samples), 1)
        tt |> equal(parsed(["nothing"], report_json([], []), 0).status, FileStatus.failed)
        tt |> success(is_failure(FileStatus.exit_nonzero) && !is_failure(FileStatus.skipped), "is_failure separates ok and skipped from the rest")
    }
}

def private bench_file() : BenchFile =>
    BenchFile(path = "x/y.das", id = "x/y", group = "x", timeout_seconds = 2)

[test(description = "a skipped file never spawns, a clean child is ok, a child past its timeout is killed keeping its arms and an unrunnable binary reports nothing measured", timeout = 1200)]
def test_run_bench_file(t : T?) {
    with_temp_dir("bench_run_") $(root) {
        t |> run("a skipped file never spawns and carries its reason") @(tt : T?) {
            var f = bench_file()
            f.skip = "too slow"
            let res <- run_bench_file(get_host_binary(), root, f, "interp", RunLimits())
            tt |> equal(res.status, FileStatus.skipped)
            tt |> equal(res.message, "too slow")
        }
        t |> run("a clean child is ok, and the jit lane's -jit reaches the host that runs it") @(tt : T?) {
            fake_repo(root, "ok")
            let jit <- run_bench_file(get_host_binary(), root, bench_file(), "jit", RunLimits(timeout_seconds = 120))
            tt |> equal(jit.status, FileStatus.ok, "{jit.message} {jit.log_tail}")
            tt |> equal(length(jit.samples), 1)
            tt |> equal(jit.samples[0].id, "b/arm")
            tt |> equal(jit.samples[0].ns, 100.0lf)
            tt |> success(find(jit.log_tail, "lane=jit") >= 0, "the child ran jitted: {jit.log_tail}")
            let interp <- run_bench_file(get_host_binary(), root, bench_file(), "interp", RunLimits(timeout_seconds = 120))
            tt |> success(find(interp.log_tail, "lane=interp") >= 0, "the interp lane passes no -jit: {interp.log_tail}")
        }
        t |> run("a child that outlives timeout_seconds is killed; the arms it printed survive") @(tt : T?) {
            fake_repo(root, "hang")
            // the fake prints its one arm at once and then hangs; the
            // limit only has to outlive that print
            let res <- run_bench_file(get_host_binary(), root, bench_file(), "interp",
                                      RunLimits(timeout_seconds = 3))
            tt |> equal(res.status, FileStatus.timeout)
            tt |> success(find(res.message, "timeout_seconds") >= 0, "message names the knob")
            tt |> equal(length(res.samples), 1, "the arm it printed before the kill stands")
        }
        t |> run("an unrunnable binary reports that nothing was measured") @(tt : T?) {
            let res <- run_bench_file("/nonexistent/daslang", root, bench_file(), "interp", RunLimits(timeout_seconds = 30))
            tt |> success(is_failure(res.status), "a missing binary is a failure: {res.status}")
            tt |> equal(length(res.samples), 0)
        }
    }
}

[test(description = "the jit probe reports the lane available on a daslang that runs it and names the binary that cannot", timeout = 1200)]
def test_probe_jit(t : T?) {
    t |> run("a daslang that runs the probe reports the lane available; one that cannot run says so") @(tt : T?) {
        tt |> equal(probe_jit(get_host_binary()), "", "the running host prints the marker")
        let why = probe_jit("/nonexistent/daslang")
        tt |> success(find(why, "/nonexistent/daslang") >= 0, "the reason names the binary it could not run: {why}")
    }
}

def private touch(dir, rel : string) {
    let p = path_join(dir, rel)
    mkdir_rec(dir_name(p))
    fwrite(p, "options gen2\n")
}

def private good_config() : string {
    let files <- { "core/slow.das" => JV((skip = "too slow")), "core/big.das" => JV((timeout_seconds = 60)) }
    let globs <- { "sort/**" => "not here" }
    return write_json(JV((root = "benchmarks", exclude = JV(["**/tests/**", "**/_*.das"]), lanes = JV(["interp"]),
        repeat = 2, timeout_seconds = 30, lane_excludes = JV((interp = JV(globs))), files = JV(files))))
}

def private load_cfg(dir : string) : SuiteConfig {
    var error = ""
    return <- load_suite_config(path_join(dir, "suite.json"), error)
}

[test(description = "a well-formed suite config parses with its overrides and a missing, unparseable or out-of-range one fails closed naming the fault")]
def test_load_suite_config(t : T?) {
    with_temp_dir("bench_stand_") $(dir) {
        t |> run("a well-formed config parses with its overrides") @(tt : T?) {
            fwrite(path_join(dir, "suite.json"), good_config())
            var error = ""
            let cfg <- load_suite_config(path_join(dir, "suite.json"), error)
            tt |> equal(error, "")
            tt |> equal(cfg.repeat, 2)
            tt |> equal(cfg.timeout_seconds, 30)
            tt |> equal(length(cfg.exclude), 2)
            tt |> equal(length(cfg.files), 2)
            cfg.files |> get("core/big.das") $(ov) {
                tt |> equal(ov.timeout_seconds, 60)
            }
        }
        t |> run("a missing, unparseable or out-of-range config fails closed, naming what is wrong") @(tt : T?) {
            var error = ""
            load_suite_config(path_join(dir, "missing.json"), error)
            tt |> success(find(error, "missing.json") >= 0, "error names the path")
            let path = path_join(dir, "bad.json")
            var bodies <- [
                "not a config at all",
                write_json(JV((lanes = JV(array<string>())))),
                write_json(JV((lanes = JV(["interp"]), repeat = 0))),
                write_json(JV((lanes = JV(["vulkan"])))),
                write_json(JV((lanes = JV(["interp"]), lane_excludes = JV((jit = JV(table<string; string>()))))))
            ]
            for (body, word in bodies, ["parse", "no lanes", "positive", "vulkan", "does not run"]) {
                fwrite(path, body)
                load_suite_config(path, error)
                tt |> success(find(error, word) >= 0, "{body} -> {error}")
            }
            fwrite(path, write_json(JV((lanes = JV(["interp", "jit"])))))
            load_suite_config(path, error)
            tt |> equal(error, "", "the two real lanes are accepted")
        }
    }
}

[test(description = "suite discovery excludes helpers, test dirs and non-das files, sorts the rest, applies overrides and reports an override naming a missing file")]
def test_discover_files(t : T?) {
    with_temp_dir("bench_stand_") $(dir) {
        fwrite(path_join(dir, "suite.json"), good_config())
        for (rel in ["core/hash/test02.das", "core/hash/_common.das", "core/slow.das", "core/big.das", "sql/tests/test_update.das", "sort/sort.das", "sort/README.md"]) {
            touch(dir, "benchmarks/{rel}")
        }
        t |> run("helpers, test dirs and non-das files are excluded; the rest is sorted, with identity from the path and overrides applied") @(tt : T?) {
            var err = ""
            let files <- discover_files(dir, load_cfg(dir), err)
            tt |> equal(err, "")
            tt |> equal(join([for (f in files); f.path], " "), "core/big.das core/hash/test02.das core/slow.das sort/sort.das")
            tt |> equal(files[1].group, "core/hash")
            tt |> equal(files[1].id, "core/hash/test02")
            tt |> equal(group_of("top.das"), "root")
            tt |> equal(stem_path("top.das"), "top")
            tt |> equal(files[2].skip, "too slow")
            tt |> equal(files[2].timeout_seconds, 30)
            tt |> equal(files[0].timeout_seconds, 60)
            tt |> equal(files[1].skip, "")
            tt |> equal(skip_reason(files[3], "interp"), "not here", "a lane exclude reaches the file it matches")
            tt |> equal(skip_reason(files[1], "interp"), "")
        }
        t |> run("an override naming a missing file is reported") @(tt : T?) {
            let cfg <- load_cfg(dir)
            remove(path_join(dir, "benchmarks/core/slow.das"))
            var err = ""
            let files <- discover_files(dir, cfg, err)
            tt |> equal(length(files), 3)
            tt |> success(find(err, "core/slow.das") >= 0, "error names the stale override")
        }
    }
}
