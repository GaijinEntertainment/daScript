options gen2

require dastest/testing_boost public
require bench_history
require bench_runner
require daslib/fio
require daslib/json_boost
require strings
require daslib/strings_boost

//! The two programs the box drives: `main.das`'s verbs and `nightly.sh`'s ssh gate. Both are
//! spawned rather than called, because their contract with the driver is argv in, exit code and
//! written file out. Every arm skips where it has no shell.

let private TOOL_DIR = "utils/internal/bench-stand"

def private has_shell() : bool {
    return fexist("/bin/sh")
}

def private with_temp_dir(prefix : string; blk : block<(dir : string) : void>) {
    var err = ""
    let dir = create_temp_directory(prefix, err)
    verify(!empty(dir), "temp dir created")
    invoke(blk, dir)
    rmdir_rec(dir)
}

//! A `meta.json` of the shape `nightly.sh` writes, with the build's own verdict.
def private write_meta(dir : string; build_status : string) : string {
    let path = path_join(dir, "meta.json")
    fwrite(path, "\{\"run_id\":\"20260908T030000Z-abcdef12\",\"started\":\"2026-09-08T03:00:00Z\","
        + "\"commit\":\{\"sha\":\"abcdef1234567890\",\"date\":\"2026-09-08T01:00:00Z\",\"subject\":\"a commit\",\"author\":\"dev\"\},"
        + "\"machine\":\{\"host\":\"box\",\"cores\":4\},"
        + "\"build\":\{\"status\":\"{build_status}\",\"seconds\":12,\"log_tail\":\"ninja: build stopped\"\}\}")
    return path
}

//! A stand-in daslang: prints one benchmark sample, writes the report the runner asked for, and
//! answers the JIT probe, so `main.das run` completes without a real build.
def private write_fake_daslang(dir : string) : string {
    let path = path_join(dir, "fake_daslang.sh")
    fwrite(path, "#!/bin/sh\n"
        + "REPORT=\"\"\n"
        + "for a in \"$@\"; do case \"$PREV\" in --json-file) REPORT=$a;; esac; PREV=$a; done\n"
        + "case \"$*\" in *jit_probe*|*-jit*.das) echo jit-probe-ok; exit 0;; esac\n"
        + "echo '\{\"name\":\"b\",\"sub_name\":\"arm\",\"n\":10,\"time_ns\":1000,\"allocs\":0,\"heap_bytes\":0,\"string_allocs\":0,\"string_heap_bytes\":0,\"func_type\":\"INTERP\"\}'\n"
        + "[ -n \"$REPORT\" ] && echo '\{\"file\":\"x\",\"total\":0,\"passed\":0,\"failed\":0,\"errors\":0,\"skipped\":0,\"success\":true,\"time_usec\":1,\"tests\":[\{\"name\":\"b\",\"passed\":true,\"skipped\":false,\"time\":1,\"messages\":[],\"location\":\"\"\}]\}' > \"$REPORT\"\n"
        + "exit 0\n")
    var out = ""
    run_and_capture(["/bin/chmod", "755", path], out)
    return path
}

def private write_suite(dir : string; lanes : string) : string {
    let path = path_join(dir, "suite.json")
    fwrite(path, "\{ \"root\": \"benchmarks\", \"exclude\": [\"**/_*.das\"], \"lanes\": [{lanes}], \"repeat\": 1, \"baseline_runs\": 3 \}")
    return path
}

def private seed_benchmarks(dir : string) {
    mkdir_rec(path_join(dir, "benchmarks/core"))
    fwrite(path_join(dir, "benchmarks/core/one.das"), "options gen2\n")
    fwrite(path_join(dir, "benchmarks/core/two.das"), "options gen2\n")
}

//! Runs the tool under the host binary and returns its exit code; `output` gets stdout+stderr.
def private run_tool(args : array<string>; var output : string&) : int {
    var argv <- ["bin/daslang", path_join(TOOL_DIR, "main.das"), "--"]
    argv |> push_from(args)
    return run_and_capture(argv, output, 240.0)
}

[test]
def test_verb_run(t : T?) {
    if (!has_shell()) {
        t |> skip("needs /bin/sh")
        return
    }
    with_temp_dir("bench_cli_") $(dir) {
        seed_benchmarks(dir)
        let fake = write_fake_daslang(dir)
        let suite = write_suite(dir, "\"interp\"")
        t |> run("a clean run records one file per lane and exits 0") @(tt : T?) {
            let meta = write_meta(dir, "ok")
            let record = path_join(dir, "run.json")
            var out = ""
            let code = run_tool(["run", "--root", dir, "--bin", fake, "--suite", suite,
                "--meta", meta, "--out", record], out)
            tt |> equal(code, 0, "exit 0 when every file ran: {out}")
            var rec = RunRecord()
            tt |> success(sscan_json(fread(record), rec), "the record parses")
            tt |> equal(rec.run_id, "20260908T030000Z-abcdef12")
            tt |> equal(rec.status, RunStatus.ok)
            tt |> equal(length(rec.files), 2, "one file per lane, two benchmark files")
            tt |> equal(rec.lanes?["interp"] ?? "", "ok")
            tt |> success(rec.seconds >= 0.0lf, "the suite's duration is recorded")
            tt |> success(!empty(rec.finished), "the finish stamp is written")
        }
        t |> run("a meta with no build section is a night that ran, not a night whose build failed") @(tt : T?) {
            let bare = path_join(dir, "bare_meta.json")
            fwrite(bare, "\{\"run_id\":\"local-1\",\"started\":\"2026-09-09T10:00:00Z\"\}")
            let record = path_join(dir, "bare.json")
            var out = ""
            let code = run_tool(["run", "--root", dir, "--bin", fake, "--suite", suite,
                "--meta", bare, "--out", record], out)
            tt |> equal(code, 0, "a hand-driven local run exits 0: {out}")
            var rec = RunRecord()
            tt |> success(sscan_json(fread(record), rec), "the record parses")
            tt |> equal(rec.status, RunStatus.ok)
            tt |> equal(rec.build.status, "ok", "an absent build section reads as a build that was fine")
        }
        t |> run("--filter narrows the run to the files whose path contains it") @(tt : T?) {
            let meta = write_meta(dir, "ok")
            let record = path_join(dir, "filtered.json")
            var out = ""
            run_tool(["run", "--root", dir, "--bin", fake, "--suite", suite, "--meta", meta,
                "--out", record, "--filter", "one"], out)
            var rec = RunRecord()
            tt |> success(sscan_json(fread(record), rec), "the record parses")
            tt |> equal(length(rec.files), 1)
            tt |> equal(rec.files[0].path, "core/one.das")
        }
        t |> run("--lanes overrides the suite, and an unavailable lane runs no file") @(tt : T?) {
            let meta = write_meta(dir, "ok")
            let record = path_join(dir, "lanes.json")
            var out = ""
            run_tool(["run", "--root", dir, "--bin", "/nonexistent/daslang", "--suite", suite,
                "--meta", meta, "--out", record, "--lanes", "jit"], out)
            var rec = RunRecord()
            tt |> success(sscan_json(fread(record), rec), "the record parses")
            tt |> success(!key_exists(rec.lanes, "interp"), "the suite's lane list was overridden")
            tt |> success(find(rec.lanes?["jit"] ?? "", "no jit-compiled run") >= 0,
                "the jit lane names why it is unavailable: {rec.lanes?["jit"] ?? ""}")
            tt |> success(find(rec.lanes?["jit"] ?? "", "/nonexistent/daslang") >= 0,
                "and names the binary it could not run, since that binary printed nothing")
            tt |> success(empty(rec.files), "an unavailable lane contributes no file")
        }
        t |> run("--failed writes the record of a night that never benchmarked and exits 1") @(tt : T?) {
            let record = path_join(dir, "failed.json")
            var out = ""
            let code = run_tool(["run", "--root", dir, "--bin", fake, "--suite", suite,
                "--meta", write_meta(dir, "failed"), "--out", record, "--failed", "the build failed"], out)
            tt |> equal(code, 1, "a night that could not run is a failure")
            var rec = RunRecord()
            tt |> success(sscan_json(fread(record), rec), "the record parses")
            tt |> equal(rec.status, RunStatus.build_failed)
            tt |> success(empty(rec.files), "a failed build carries no file")
        }
    }
}

[test]
def test_verb_run_refusals(t : T?) {
    if (!has_shell()) {
        t |> skip("needs /bin/sh")
        return
    }
    with_temp_dir("bench_cli_") $(dir) {
        seed_benchmarks(dir)
        let fake = write_fake_daslang(dir)
        let suite = write_suite(dir, "\"interp\"")
        t |> run("a missing --meta, an unparseable one, and missing arguments each exit 1") @(tt : T?) {
            var out = ""
            tt |> equal(run_tool(["run", "--root", dir, "--out", path_join(dir, "x.json")], out), 1, "no --meta")
            tt |> success(find(out, "--meta and --out are required") >= 0, "the message names what is missing")
            let bad = path_join(dir, "bad_meta.json")
            fwrite(bad, "not json")
            tt |> equal(run_tool(["run", "--root", dir, "--bin", fake, "--suite", suite,
                "--meta", bad, "--out", path_join(dir, "y.json")], out), 1, "unparseable meta")
            tt |> success(find(out, "does not parse") >= 0, "and says so: {out}")
            tt |> equal(run_tool(["nonsense"], out), 1, "an unknown verb prints help and exits 1")
        }
        t |> run("a suite config that fails to load stops the run before it spawns anything") @(tt : T?) {
            let broken = path_join(dir, "broken_suite.json")
            fwrite(broken, "\{ \"lanes\": [] \}")
            var out = ""
            tt |> equal(run_tool(["run", "--root", dir, "--bin", fake, "--suite", broken,
                "--meta", write_meta(dir, "ok"), "--out", path_join(dir, "z.json")], out), 1)
            tt |> success(find(out, "no lanes") >= 0, "the config's own error reaches the operator")
            tt |> success(!fexist(path_join(dir, "z.json")), "and no record is written")
        }
    }
}

[test]
def test_verb_report(t : T?) {
    if (!has_shell()) {
        t |> skip("needs /bin/sh")
        return
    }
    with_temp_dir("bench_cli_") $(dir) {
        let suite = write_suite(dir, "\"interp\"")
        let runs_dir = path_join(dir, "runs")
        mkdir_rec(runs_dir)
        t |> run("report writes both artifacts and exits on the latest night's verdict") @(tt : T?) {
            for (n in range(4)) {
                var rec <- RunRecord(run_id = "r{n}", started = "2026-09-0{n + 1}T03:00:00Z",
                    commit = CommitInfo(sha = "sha{n}", date = "2026-09-0{n + 1}T01:00:00Z", subject = "c{n}"),
                    machine = MachineInfo(host = "box"), build = BuildInfo(status = "ok", seconds = 1.0lf),
                    status = RunStatus.ok)
                rec.lanes["interp"] = "ok"
                var f <- FileResult(path = "core/one.das", id = "core/one", group = "core",
                    lane = "interp", status = FileStatus.ok)
                f.samples |> emplace(Sample(id = "b/arm", ns = 100.0lf, ns_median = 100.0lf, runs = 1))
                rec.files |> emplace(f)
                fwrite(path_join(runs_dir, "r{n}.json"), sprint_json(rec, false))
            }
            let data = path_join(dir, "data.json")
            let summary = path_join(dir, "summary.md")
            var out = ""
            let code = run_tool(["report", "--suite", suite, "--runs", runs_dir,
                "--out-data", data, "--out-summary", summary, "--generated", "2026-09-09T00:00:00Z",
                "--repo-url", "https://example.invalid/repo", "--site-url", "https://example.invalid/bench/"], out)
            tt |> equal(code, 0, "a clean latest night exits 0: {out}")
            var ds = Dataset()
            tt |> success(sscan_json(fread(data), ds), "data.json parses")
            tt |> equal(length(ds.runs), 4)
            tt |> equal(ds.latest, 3)
            tt |> equal(ds.generated, "2026-09-09T00:00:00Z")
            tt |> equal(ds.repo_url, "https://example.invalid/repo")
            let md = fread(summary)
            tt |> success(find(md, "# Benchmark stand - ok") >= 0, "summary.md leads with the verdict")
            tt |> success(find(md, "https://example.invalid/bench/") >= 0, "and links the site")
        }
        t |> run("a corrupt record is warned about and left out, not fatal") @(tt : T?) {
            fwrite(path_join(runs_dir, "zz_corrupt.json"), "\{ not json")
            var out = ""
            let code = run_tool(["report", "--suite", suite, "--runs", runs_dir,
                "--out-data", path_join(dir, "d2.json"), "--out-summary", path_join(dir, "s2.md")], out)
            tt |> equal(code, 0, "the other nights still publish")
            tt |> success(find(out, "zz_corrupt.json") >= 0, "the corrupt file is named: {out}")
            remove(path_join(runs_dir, "zz_corrupt.json"))
        }
        t |> run("an empty runs directory exits 1 and says there is nothing yet") @(tt : T?) {
            let empty_dir = path_join(dir, "no_runs")
            mkdir_rec(empty_dir)
            var out = ""
            let code = run_tool(["report", "--suite", suite, "--runs", empty_dir,
                "--out-data", path_join(dir, "d3.json"), "--out-summary", path_join(dir, "s3.md")], out)
            tt |> equal(code, 1)
            tt |> success(find(fread(path_join(dir, "s3.md")), "No run records") >= 0, "the summary says so")
        }
        t |> run("missing arguments exit 1 before anything is written") @(tt : T?) {
            var out = ""
            tt |> equal(run_tool(["report", "--suite", suite, "--runs", runs_dir], out), 1)
            tt |> success(find(out, "are required") >= 0, "the message names what is missing")
        }
    }
}

//! Drives `nightly.sh gate` the way an inbound ssh connection does: the verb and its arguments
//! arrive only through SSH_ORIGINAL_COMMAND, and nothing else on that connection exists.
def private run_gate(home, command : string; var output : string&) : int {
    var code = -1
    var out = ""
    let env <- ["BENCH_STAND_HOME={home}", "SSH_ORIGINAL_COMMAND={command}"]
    with_process(["/bin/bash", path_join(TOOL_DIR, "nightly.sh"), "gate"], ".", env) $(var p) {
        unsafe {
            while (true) {
                process_drain(p) $(line) {
                    out += "{line}\n"
                }
                code = process_poll(p)
                break if (code != process_running)
                sleep(20u)
            }
            process_drain(p) $(line) {
                out += "{line}\n"
            }
        }
    }
    output = out
    return code
}

[test]
def test_ssh_gate(t : T?) {
    if (!fexist("/bin/bash")) {
        t |> skip("needs bash")
        return
    }
    with_temp_dir("bench_gate_") $(home) {
        mkdir_rec(path_join(home, "site"))
        fwrite(path_join(home, "site/status.json"), "\{\"state\":\"finished\",\"exit\":0\}")
        fwrite(path_join(home, "site/summary.md"), "# a night\n")
        t |> run("the two read-only verbs answer with what the box published") @(tt : T?) {
            var out = ""
            tt |> equal(run_gate(home, "status", out), 0)
            tt |> success(find(out, "\"state\":\"finished\"") >= 0, "status prints status.json: {out}")
            tt |> equal(run_gate(home, "summary", out), 0)
            tt |> success(find(out, "# a night") >= 0, "summary prints summary.md")
        }
        t |> run("an argument carrying anything but path characters is refused") @(tt : T?) {
            for (hostile in ["start master;rm -rf /", "start $(whoami)", "start `id`",
                "start master&touch /tmp/pwned", "follow ../../etc/passwd|cat"]) {
                var out = ""
                let code = run_gate(home, hostile, out)
                tt |> equal(code, 2, "refused: {hostile}")
                tt |> success(find(out, "refused") >= 0, "and says so: {out}")
            }
        }
        t |> run("more arguments than any verb takes is refused") @(tt : T?) {
            var out = ""
            tt |> equal(run_gate(home, "start master fail-on-regression extra", out), 2)
            tt |> success(find(out, "too many arguments") >= 0, "the message names the reason: {out}")
        }
        t |> run("a verb the gate does not carry is refused, naming it") @(tt : T?) {
            var out = ""
            tt |> equal(run_gate(home, "run", out), 2, "`run` is a local verb, never an ssh one")
            tt |> success(find(out, "unknown verb 'run'") >= 0, "the message names the verb: {out}")
            tt |> equal(run_gate(home, "bash", out), 2)
        }
        t |> run("a missing status file still answers, rather than failing the caller") @(tt : T?) {
            remove(path_join(home, "site/status.json"))
            var out = ""
            tt |> equal(run_gate(home, "status", out), 0)
            tt |> success(find(out, "never-run") >= 0, "it says the stand has not run: {out}")
        }
    }
}
