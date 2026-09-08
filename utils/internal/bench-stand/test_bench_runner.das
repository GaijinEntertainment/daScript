options gen2

require dastest/testing_boost public
require bench_runner
require bench_suite
require daslib/fio
require strings
require daslib/strings_boost
require math

def private stats_line(name, sub : string; n : int; time_ns : int64; allocs : int = 0; tier : string = "INTERP";
                       string_allocs : int = 0) : string {
    return "\{\"name\":\"{name}\",\"sub_name\":\"{sub}\",\"n\":{n},\"time_ns\":{time_ns},\"allocs\":{allocs},\"heap_bytes\":{allocs * 16},\"string_allocs\":{string_allocs},\"string_heap_bytes\":{string_allocs * 32},\"func_type\":\"{tier}\"\}"
}

def private report(success : bool; names : array<string>; failed : array<string>) : string {
    let tests <- [for (n in names); "\{\"name\":\"{n}\",\"passed\":{find_index(failed, n) < 0},\"skipped\":false,\"time\":1,\"messages\":[],\"location\":\"\"\}"]
    return "\{\"file\":\"x.das\",\"total\":0,\"passed\":0,\"failed\":{length(failed)},\"errors\":0,\"skipped\":0,\"success\":{success},\"time_usec\":1,\"tests\":[{join(tests, ",")}]\}"
}

def private fresh_result(status : FileStatus = FileStatus.ok) : FileResult {
    return <- FileResult(path = "core/x.das", id = "core/x", group = "core", lane = "interp", status = status)
}

[test]
def test_parse_bench_output(t : T?) {
    t |> run("samples aggregate over repeats: min, median, spread, per-op allocation medians") @(tt : T?) {
        var res <- fresh_result()
        let lines <- [
            "2026-09-08T14:11:43.241Z run 1/3: benchmarks/core/x.das",
            stats_line("bench_a", "insert/100", 100, 10000l, 300, "INTERP", 100),
            stats_line("bench_a", "read/100", 1000, 50000l),
            stats_line("bench_a", "insert/100", 100, 12000l, 500, "INTERP", 100),
            stats_line("bench_a", "read/100", 1000, 40000l),
            stats_line("bench_a", "insert/100", 100, 11000l, 400, "INTERP", 100),
            stats_line("bench_a", "read/100", 1000, 45000l),
            "",
            "0 tests, 0 passed, 0 failed, 0 errors, 0 skipped",
            "SUCCESS! (3.9s)"]
        parse_bench_output(lines, report(true, ["bench_a"], []), 0, res)
        tt |> equal(res.status, FileStatus.ok)
        tt |> equal(res.exit_code, 0)
        tt |> equal(res.tier, "INTERP")
        tt |> equal(length(res.samples), 2)
        tt |> equal(res.samples[0].id, "bench_a/insert/100")
        tt |> equal(res.samples[0].runs, 3)
        tt |> success(abs(res.samples[0].ns - 100.0lf) < 0.001lf, "min is 100 ns/op")
        tt |> success(abs(res.samples[0].ns_median - 110.0lf) < 0.001lf, "median is 110 ns/op")
        tt |> success(abs(res.samples[0].spread - 0.2lf) < 0.001lf, "spread is (120-100)/100")
        tt |> equal(res.samples[0].allocs, 4l, "allocs is the per-op median of 3, 4, 5")
        tt |> equal(res.samples[0].bytes, 64l, "heap bytes is the per-op median of 48, 64, 80")
        tt |> equal(res.samples[0].string_allocs, 1l)
        tt |> equal(res.samples[0].string_bytes, 32l)
        tt |> equal(res.samples[1].allocs, 0l, "an arm that allocates nothing reports zero")
        tt |> equal(res.samples[1].id, "bench_a/read/100")
        tt |> success(abs(res.samples[1].ns - 40.0lf) < 0.001lf, "read min is 40 ns/op")
        tt |> equal(res.message, "")
    }
    t |> run("the log tail keeps the non-sample lines, newest last") @(tt : T?) {
        var res <- fresh_result()
        var lines <- [for (i in range(100)); "line {i}"]
        lines |> push(stats_line("b", "s", 10, 100l))
        parse_bench_output(lines, report(true, ["b"], []), 0, res)
        tt |> success(find(res.log_tail, "line 0\n") < 0, "oldest lines dropped")
        tt |> success(ends_with(res.log_tail, "line 99"), "newest line kept")
        tt |> success(find(res.log_tail, "\"sub_name\"") < 0, "sample lines are not log")
    }
    t |> run("a compile error names the first error line") @(tt : T?) {
        var res <- fresh_result()
        let lines <- [
            "[E] Failed to compile benchmarks/core/x.das",
            "error[30344]: local variable x initialization type mismatch; int const = string const",
            "benchmarks/core/x.das:5:8",
            "FAIL benchmarks/core/x.das (0.04s)"]
        parse_bench_output(lines, "", 1, res)
        tt |> equal(res.status, FileStatus.compile_error)
        tt |> success(find(res.message, "error[30344]") >= 0, "message carries the error code")
        tt |> equal(res.exit_code, 1)
    }
    t |> run("a failed benchmark function drops its arms and names itself; the others survive") @(tt : T?) {
        var res <- fresh_result()
        let lines <- [
            "benchmarks/core/x.das:5: values differ",
            stats_line("failing", "fail_op", 100, 100l),
            "[E] --- FAIL 'failing' in 'fail_op' (1.7s)",
            stats_line("ok_after", "ok_op", 100, 100l),
            "FAIL benchmarks/core/x.das (3.0s)"]
        parse_bench_output(lines, report(false, ["failing", "ok_after"], ["failing"]), 1, res)
        tt |> equal(res.status, FileStatus.failed)
        tt |> success(find(res.message, "failing") >= 0, "message names the function")
        tt |> equal(length(res.samples), 1)
        tt |> equal(res.samples[0].id, "ok_after/ok_op")
    }
}

[test]
def test_parse_bench_output_verdicts(t : T?) {
    t |> run("the FAIL log line alone marks the function failed when no report was written") @(tt : T?) {
        var res <- fresh_result()
        let lines <- [stats_line("failing", "fail_op", 100, 100l), "[E] --- FAIL 'failing' in 'fail_op' (1.7s)"]
        parse_bench_output(lines, "", 1, res)
        tt |> equal(res.status, FileStatus.failed)
        tt |> equal(length(res.samples), 0)
    }
    t |> run("a measured file that dies at shutdown keeps its numbers and says so") @(tt : T?) {
        var res <- fresh_result()
        let lines <- [
            stats_line("b", "s", 10, 100l),
            "SUCCESS! (48.9s)",
            "FATAL: exiting with code 1 -- 1 smart pointer(s) still alive at shutdown"]
        parse_bench_output(lines, report(true, ["b"], []), 1, res)
        tt |> equal(res.status, FileStatus.ok, "the measurement stands: {res.message}")
        tt |> equal(length(res.samples), 1)
        tt |> success(find(res.message, "at shutdown") >= 0, "the message names when it happened")
        tt |> success(find(res.message, "smart pointer") >= 0, "and carries the FATAL line")
    }
    t |> run("a nonzero exit with no finished report is a failure, samples or not") @(tt : T?) {
        var res <- fresh_result()
        let lines <- [stats_line("b", "s", 10, 100l)]
        parse_bench_output(lines, "", 1, res)
        tt |> equal(res.status, FileStatus.exit_nonzero)
        tt |> success(find(res.message, "did not run to its end") >= 0, "the message says the suite was cut short: {res.message}")
    }
    t |> run("a limit verdict survives whatever the output says, keeping the finished arms") @(tt : T?) {
        var res <- fresh_result(FileStatus.timeout)
        res.message = "killed after 1 s (timeout_seconds)"
        let lines <- [stats_line("b", "done", 10, 100l)]
        parse_bench_output(lines, "", 9, res)
        tt |> equal(res.status, FileStatus.timeout)
        tt |> equal(res.message, "killed after 1 s (timeout_seconds)")
        tt |> equal(length(res.samples), 1)
        tt |> equal(res.exit_code, 9)
    }
    t |> run("a clean exit with no samples and no report is not ok") @(tt : T?) {
        var res <- fresh_result()
        let no_lines : array<string>
        parse_bench_output(no_lines, "", 0, res)
        tt |> equal(res.status, FileStatus.exit_nonzero)
        var res2 <- fresh_result()
        parse_bench_output(no_lines, report(true, no_lines, no_lines), 0, res2)
        tt |> equal(res2.status, FileStatus.failed)
        tt |> success(find(res2.message, "no benchmark samples") >= 0, "message says what is missing")
    }
    t |> run("is_failure separates ok and skipped from the rest") @(tt : T?) {
        tt |> success(!is_failure(FileStatus.ok), "ok")
        tt |> success(!is_failure(FileStatus.skipped), "skipped")
        tt |> success(is_failure(FileStatus.timeout), "timeout")
        tt |> success(is_failure(FileStatus.exit_nonzero), "exit_nonzero")
    }
}

def private has_shell() : bool {
    return fexist("/bin/sh")
}

def private bench_file() : BenchFile {
    return BenchFile(path = "x/y.das", id = "x/y", group = "x", timeout_seconds = 2, memory_limit_mb = 64)
}

//! A stand-in for the daslang binary. `body` runs with `$REPORT` set to the path the runner
//! passed after `--json-file` and `$ALL` to the whole argv, so a fixture reads the arguments by
//! name rather than encoding their order.
def private fake_daslang(body : string) : string {
    var err = ""
    let path = create_temp_file("bench_stand_fake_", ".sh", err)
    fwrite(path, "#!/bin/sh\nALL=\"$*\"\nREPORT=\"\"\nwhile [ $# -gt 0 ]; do\n  [ \"$1\" = --json-file ] && REPORT=$2\n  shift\ndone\n{body}\n")
    var out = ""
    run_and_capture(["/bin/chmod", "755", path], out)
    return path
}

let private report_text = "\{\"file\":\"x.das\",\"total\":0,\"passed\":0,\"failed\":0,\"errors\":0,\"skipped\":0,\"success\":true,\"time_usec\":1,\"tests\":[\{\"name\":\"b\",\"passed\":true,\"skipped\":false,\"time\":1,\"messages\":[],\"location\":\"\"\}]\}"

[test]
def test_run_bench_file_limits(t : T?) {
    if (!fexist("/bin/sh")) {
        t |> skip("needs /bin/sh")
        return
    }
    t |> run("a skipped file never spawns and carries its reason") @(tt : T?) {
        var f = bench_file()
        f.skip = "too slow"
        let res <- run_bench_file("/bin/sh", ".", f, "interp", RunLimits())
        tt |> equal(res.status, FileStatus.skipped)
        tt |> equal(res.message, "too slow")
    }
    t |> run("a child that outlives timeout_seconds is killed; the arms it printed survive") @(tt : T?) {
        let fake = fake_daslang("echo '{stats_line("b", "done", 10, 100l)}'; exec sleep 30")
        let res <- run_bench_file(fake, ".", bench_file(), "interp", RunLimits(timeout_seconds = 1))
        remove(fake)
        tt |> equal(res.status, FileStatus.timeout)
        tt |> success(find(res.message, "timeout_seconds") >= 0, "message names the knob")
        tt |> success(res.seconds >= 1.0lf && res.seconds < 15.0lf, "killed near the limit, not at the child's own end")
        tt |> equal(length(res.samples), 1)
    }
    t |> run("a child whose resident set passes memory_limit_mb is killed and reported") @(tt : T?) {
        if (!fexist("/usr/bin/perl")) {
            tt |> skip("needs perl to grow a child's resident set")
            return
        }
        let fake = fake_daslang("exec /usr/bin/perl -e '$x = \"a\" x 200000000; sleep 30'")
        let res <- run_bench_file(fake, ".", bench_file(), "interp", RunLimits(timeout_seconds = 20, memory_limit_mb = 64))
        remove(fake)
        tt |> equal(res.status, FileStatus.memory)
        tt |> success(find(res.message, "memory_limit_mb") >= 0, "message names the knob")
        tt |> success(res.seconds < 15.0lf, "killed well before the timeout")
    }
    t |> run("a child that exits cleanly with samples is ok, from the repo root cwd") @(tt : T?) {
        let fake = fake_daslang("echo '{stats_line("b", "done", 10, 100l)}'; echo '{report_text}' > \"$REPORT\"; exit 0")
        let res <- run_bench_file(fake, ".", bench_file(), "interp", RunLimits(timeout_seconds = 5))
        remove(fake)
        tt |> equal(res.status, FileStatus.ok)
        tt |> equal(length(res.samples), 1)
        tt |> equal(res.exit_code, 0)
    }
    t |> run("the jit lane puts -jit ahead of dastest, and the interp lane does not") @(tt : T?) {
        let fake = fake_daslang("echo \"ARGV: $ALL\"; exit 1")
        let jit <- run_bench_file(fake, ".", bench_file(), "jit", RunLimits(timeout_seconds = 10))
        let interp <- run_bench_file(fake, ".", bench_file(), "interp", RunLimits(timeout_seconds = 10, repeat = 1))
        remove(fake)
        tt |> success(find(jit.log_tail, "ARGV: -jit ") >= 0, "the jit lane's first argument is -jit: {jit.log_tail}")
        tt |> success(find(jit.log_tail, "dastest/dastest.das") >= 0, "and dastest follows it")
        tt |> success(find(interp.log_tail, "-jit") < 0, "the interp lane names no -jit: {interp.log_tail}")
        tt |> success(find(interp.log_tail, "--count 1") >= 0, "the repeat count reaches the child: {interp.log_tail}")
    }
}

[test]
def test_run_bench_file_lanes(t : T?) {
    if (!has_shell()) {
        t |> skip("needs /bin/sh")
        return
    }
    t |> run("the aot lane carries both use-aot flags, or it would measure the interpreter") @(tt : T?) {
        let fake = fake_daslang("echo \"ARGV: $ALL\"; exit 1")
        let aot <- run_bench_file(fake, ".", bench_file(), "aot", RunLimits(timeout_seconds = 10))
        remove(fake)
        // the host flag arms the stub lookup, the dastest flag arms fail_on_no_aot; with only the
        // first, every function silently interprets and the lane reports interpreter numbers
        tt |> success(find(aot.log_tail, "ARGV: -use-aot ") >= 0, "the host flag comes first: {aot.log_tail}")
        tt |> success(find(aot.log_tail, "-- --use-aot ") >= 0, "and dastest's own flag follows the separator")
        tt |> success(find(aot.log_tail, "-jit") < 0, "the aot lane is not the jit lane")
    }
    t |> run("a file skipped in one lane still measures in the others") @(tt : T?) {
        var f = bench_file()
        f.skip_lanes["aot"] = "the emitter cannot handle this one"
        tt |> equal(skip_reason(f, "aot"), "the emitter cannot handle this one")
        tt |> equal(skip_reason(f, "interp"), "", "the other lanes are untouched")
        let fake = fake_daslang("echo '{stats_line("b", "done", 10, 100l)}'; echo '{report_text}' > \"$REPORT\"; exit 0")
        let aot <- run_bench_file(fake, ".", f, "aot", RunLimits(timeout_seconds = 5))
        let interp <- run_bench_file(fake, ".", f, "interp", RunLimits(timeout_seconds = 5))
        remove(fake)
        tt |> equal(aot.status, FileStatus.skipped)
        tt |> equal(aot.message, "the emitter cannot handle this one")
        tt |> equal(interp.status, FileStatus.ok, "and the interpreter still measured it")
        f.skip = "broken everywhere"
        tt |> equal(skip_reason(f, "interp"), "broken everywhere", "a whole-file skip answers for every lane")
    }
    t |> run("an unrunnable binary reports that nothing was measured, naming the cause") @(tt : T?) {
        let res <- run_bench_file("/nonexistent/daslang", ".", bench_file(), "interp", RunLimits(timeout_seconds = 5))
        tt |> success(res.status == FileStatus.spawn_failed || res.status == FileStatus.exit_nonzero, "spawn failure or an immediate nonzero exit: {res.status}")
        tt |> success(empty(res.samples), "nothing measured")
        tt |> success(find(res.message, "before measuring anything") >= 0 || find(res.message, "cannot spawn") >= 0,
            "the message says nothing ran rather than claiming the benchmarks passed: {res.message}")
    }
}

[test]
def test_process_rss_mb(t : T?) {
    t |> run("a live child's resident set reads back as a positive number of megabytes") @(tt : T?) {
        if (!fexist("/bin/sh")) {
            tt |> skip("needs /bin/sh")
            return
        }
        var mb = -1
        with_process(["/bin/sh", "-c", "sleep 5"]) $(var p) {
            unsafe {
                mb = process_rss_mb(process_pid(p))
                process_kill(p)
                process_wait(p, 5.0)
            }
        }
        tt |> success(mb > 0 && mb < 4096, "a shell's RSS is a small positive figure, got {mb} MB")
        tt |> success(mb >= 1, "rounded up, so a process under a megabyte still reads as one")
    }
    t |> run("a pid nothing owns reads back as -1, which leaves the limit disarmed") @(tt : T?) {
        tt |> equal(process_rss_mb(0), -1)
    }
}

[test]
def test_probe_aot(t : T?) {
    if (!fexist("/bin/sh")) {
        t |> skip("needs /bin/sh")
        return
    }
    t |> run("a missing AOT binary names the target that builds it") @(tt : T?) {
        let why = probe_aot("/nonexistent/test_aot")
        tt |> success(find(why, "no AOT binary") >= 0, "it says what is absent: {why}")
        tt |> success(find(why, "test_aot") >= 0, "and names the target that builds it")
    }
    t |> run("an AOT binary that runs the probe reports the lane available") @(tt : T?) {
        let fake = fake_daslang("echo aot-probe-ok; exit 0")
        let why = probe_aot(fake)
        remove(fake)
        tt |> equal(why, "", "an available lane has no reason")
    }
    t |> run("an AOT binary that prints its help instead of running is not available") @(tt : T?) {
        // what the real binary does when handed a flag it does not take: usage text, exit 255
        let fake = fake_daslang("echo '    -log-compile-time  log per-module breakdown'; exit 255")
        let why = probe_aot(fake)
        remove(fake)
        tt |> success(find(why, "ran no program") >= 0, "the reason says no program ran: {why}")
        tt |> success(find(why, "exit 255") >= 0, "and names the exit code")
    }
    t |> run("an AOT binary whose marker is missing is not available even on exit 0") @(tt : T?) {
        let fake = fake_daslang("exit 0")
        let why = probe_aot(fake)
        remove(fake)
        tt |> success(!empty(why), "silence is not availability: {why}")
    }
}

[test]
def test_probe_jit(t : T?) {
    if (!fexist("/bin/sh")) {
        t |> skip("needs /bin/sh")
        return
    }
    t |> run("a binary that runs the probe reports the lane available") @(tt : T?) {
        let fake = fake_daslang("echo jit-probe-ok; exit 0")
        let why = probe_jit(fake)
        remove(fake)
        tt |> equal(why, "", "an available lane has no reason")
    }
    t |> run("a host whose only failure is its exit code still has a working lane") @(tt : T?) {
        // what a RelWithDebInfo daslang does: the program runs, then the allocation tracker's
        // exit-time report takes the process code to 1
        let fake = fake_daslang("echo jit-probe-ok; echo '=== daslang C++ heap leak report ==='; exit 1")
        let why = probe_jit(fake)
        remove(fake)
        tt |> equal(why, "", "the marker printed, so the lane runs: {why}")
    }
    t |> run("a binary that fails the probe reports the reason it printed") @(tt : T?) {
        let fake = fake_daslang("echo 'error[30812]: undefined function argument type NameLookup? const'; exit 1")
        let why = probe_jit(fake)
        remove(fake)
        tt |> success(find(why, "exit 1") >= 0, "the reason names the exit code: {why}")
        tt |> success(find(why, "error[30812]") >= 0, "and the compile error it printed, not some later line")
    }
    t |> run("a binary that exits 0 without the probe marker is not taken as available") @(tt : T?) {
        let fake = fake_daslang("echo nothing to see; exit 0")
        let why = probe_jit(fake)
        remove(fake)
        tt |> success(!empty(why), "silence is not availability: {why}")
        tt |> success(find(why, "no jit-compiled run") >= 0, "and the reason says what was missing")
    }
}
