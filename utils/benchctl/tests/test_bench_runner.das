options gen2

require dastest/testing_boost public
require ../bench_runner.das
require ../bench_suite.das
require _test_common
require daslib/clargs
require strings

def private fresh_result(status : FileStatus = FileStatus.ok) : FileResult {
    return <- FileResult(path = "core/x.das", id = "core/x", group = "core", lane = "interp", status = status)
}

def private parsed(lines : array<string>; report : string; exit_code : int; status : FileStatus = FileStatus.ok) : FileResult {
    var res <- fresh_result(status)
    classify_run_output(lines, report, exit_code, res)
    return <- res
}

[test]
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

def private bench_file() : BenchFile {
    return BenchFile(path = "x/y.das", id = "x/y", group = "x", timeout_seconds = 2)
}

[test]
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
        t |> run("a failed benchmark function is reported and its arms are dropped") @(tt : T?) {
            fake_repo(root, "fail")
            let res <- run_bench_file(get_host_binary(), root, bench_file(), "interp", RunLimits(timeout_seconds = 120))
            tt |> equal(res.status, FileStatus.failed)
            tt |> success(find(res.message, "b") >= 0, "the message names the function: {res.message}")
            tt |> equal(length(res.samples), 0)
        }
        t |> run("a child that outlives timeout_seconds is killed; the arms it printed survive") @(tt : T?) {
            fake_repo(root, "hang")
            let res <- run_bench_file(get_host_binary(), root, bench_file(), "interp", RunLimits(timeout_seconds = 20))
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

[test]
def test_probe_jit(t : T?) {
    t |> run("a daslang that runs the probe reports the lane available; one that cannot run says so") @(tt : T?) {
        tt |> equal(probe_jit(get_host_binary()), "", "the running host prints the marker")
        let why = probe_jit("/nonexistent/daslang")
        tt |> success(find(why, "/nonexistent/daslang") >= 0, "the reason names the binary it could not run: {why}")
    }
}
