options gen2

require dastest/testing_boost public
require ../bench_history.das
require ../bench_runner.das
require _test_common
require daslib/fio
require daslib/clargs
require daslib/json_boost

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
    var argv <- ["bin/daslang", path_join(TOOL_DIR, "main.das"), "--"]
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

[test]
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
