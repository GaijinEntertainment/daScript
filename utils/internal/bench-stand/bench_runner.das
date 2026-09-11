options gen2
options indenting = 4

module bench_runner public

require bench_suite
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require daslib/strings_convert
require strings

//! How one benchmark file's process ended.
enum FileStatus {
    ok
    skipped
    spawn_failed
    compile_error
    failed
    timeout
    memory
    exit_nonzero
}

//! One `run("name")` arm of one `[benchmark]` function, aggregated over the repeats:
//! `ns` is the minimum ns/op (the noise-robust statistic), `ns_median` the median,
//! `spread` `(max - min) / min`. The four allocation fields are per-op medians.
struct Sample {
    id : string
    ns : double
    ns_median : double
    spread : double
    runs : int
    bytes : int64
    allocs : int64
    string_bytes : int64
    string_allocs : int64
}

//! One benchmark file in one lane. `samples` carries only the arms whose `[benchmark]` function
//! passed - a failed function's arms are dropped, so a broken assertion cannot publish a number.
struct FileResult {
    path : string
    id : string
    group : string
    lane : string
    status : FileStatus
    exit_code : int
    seconds : double
    tier : string
    message : string
    log_tail : string
    samples : array<Sample>
}

struct RunLimits {
    timeout_seconds : int = 900
    memory_limit_mb : int = 4096
    repeat : int = 3
}

//! `dastest --bench-format json` emits one of these per arm per repeat; the report file
//! (`--json-file`) carries the per-function verdicts.
struct BenchStatsLine {
    name : string
    sub_name : string
    n : int
    time_ns : int64
    allocs : uint64
    heap_bytes : uint64
    string_allocs : uint64
    string_heap_bytes : uint64
    func_type : string
}

struct ReportTest {
    name : string
    passed : bool
}

struct DastestReport {
    success : bool
    tests : array<ReportTest>
}

let TAIL_LINES = 60
let TAIL_LINE_CHARS = 400

def is_failure(st : FileStatus) : bool {
    return st != FileStatus.ok && st != FileStatus.skipped
}

def private median(var vals : array<double>) : double {
    return 0.0lf if (empty(vals))
    sort(vals)
    let n = length(vals)
    return (n % 2 == 1) ? vals[n / 2] : (vals[n / 2 - 1] + vals[n / 2]) / 2.0lf
}

def private median_i64(var vals : array<int64>) : int64 {
    return 0l if (empty(vals))
    sort(vals)
    return vals[length(vals) / 2]
}

def private keep_tail(var tail : array<string>; line : string) {
    if (length(tail) >= TAIL_LINES) {
        tail |> erase(0)
    }
    tail |> push(length(line) > TAIL_LINE_CHARS ? slice(line, 0, TAIL_LINE_CHARS) : line)
}

def private first_line(text : string) : string {
    let nl = find(text, "\n")
    return nl < 0 ? text : slice(text, 0, nl)
}

def private error_or_last_line(text : string) : string {
    var last = ""
    for (raw in split(text, "\n")) {
        let line = strip(raw)
        continue if (empty(line))
        return line if (starts_with(line, "FATAL") || starts_with(line, "error") || find(line, "[E]") >= 0)
        last = line
    }
    return last
}

struct private OutputScan {
    by_arm : table<string; array<BenchStatsLine>>
    arm_order : array<string>
    tail : array<string>
    compile_error : string
    failed_to_compile : string
    fatal : string
    failed_fns : table<string>
    tier : string
}

def private note_diagnostic(var scan : OutputScan; line : string) {
    if (empty(scan.compile_error) && find(line, "error[") >= 0 && find(line, "]: ") >= 0) {
        scan.compile_error = strip(line)
    }
    if (empty(scan.failed_to_compile) && find(line, "Failed to compile") >= 0) {
        scan.failed_to_compile = strip(line)
    }
    if (empty(scan.fatal) && starts_with(line, "FATAL")) {
        scan.fatal = strip(line)
    }
    let fail_at = find(line, "--- FAIL '")
    return if (fail_at < 0)
    let name_start = fail_at + length("--- FAIL '")
    let name_end = find(line, "'", name_start)
    if (name_end > name_start) {
        scan.failed_fns |> insert(slice(line, name_start, name_end))
    }
}

def private scan_line(var scan : OutputScan; line : string) {
    if (starts_with(line, "\{")) {
        var st : BenchStatsLine
        if (sscan_json(line, st) && st.n > 0) {
            let key = "{st.name}/{st.sub_name}"
            if (!key_exists(scan.by_arm, key)) {
                scan.arm_order |> push(key)
            }
            scan.by_arm[key] |> push(st)
            if (empty(scan.tier)) {
                scan.tier = st.func_type
            }
            return
        }
    }
    keep_tail(scan.tail, line)
    note_diagnostic(scan, line)
}

//! True when the report parsed; its failed functions join the scan's.
def private note_report(var scan : OutputScan; report_text : string) : bool {
    return false if (empty(report_text))
    var report : DastestReport
    return false if (!sscan_json(report_text, report))
    for (test in report.tests) {
        if (!test.passed) {
            scan.failed_fns |> insert(test.name)
        }
    }
    return true
}

def private aggregate_arm(key : string; stats : array<BenchStatsLine>) : Sample {
    var ns <- [for (st in stats); double(st.time_ns) / double(st.n)]
    var bytes <- [for (st in stats); int64(st.heap_bytes) / int64(st.n)]
    var allocs <- [for (st in stats); int64(st.allocs) / int64(st.n)]
    var sbytes <- [for (st in stats); int64(st.string_heap_bytes) / int64(st.n)]
    var sallocs <- [for (st in stats); int64(st.string_allocs) / int64(st.n)]
    sort(ns)
    let lo = ns[0]
    let hi = ns[length(ns) - 1]
    return Sample(id = key, ns = lo, ns_median = median(ns),
        spread = lo > 0.0lf ? (hi - lo) / lo : 0.0lf, runs = length(ns),
        bytes = median_i64(bytes), allocs = median_i64(allocs),
        string_bytes = median_i64(sbytes), string_allocs = median_i64(sallocs))
}

def private decide_status(scan : OutputScan; report_ok : bool; exit_code : int; var res : FileResult&) {
    if (!empty(scan.failed_to_compile)) {
        res.status = FileStatus.compile_error
        res.message = !empty(scan.compile_error) ? scan.compile_error : scan.failed_to_compile
    } elif (!empty(scan.failed_fns)) {
        res.status = FileStatus.failed
        var names <- [for (n in keys(scan.failed_fns)); n]
        sort(names)
        res.message = "benchmark function(s) failed: {join(names, ", ")}"
    } elif (exit_code != 0 && report_ok && !empty(res.samples)) {
        // dastest wrote a finished report and the arms are here, so the measurement stands and
        // only the shutdown went wrong - a leak dump, a smart-pointer assert, a signal after the
        // last benchmark. Worth saying, never worth discarding a night for: a RelWithDebInfo
        // host's allocation tracker takes every process to 1 and would fail the whole suite.
        res.status = FileStatus.ok
        if (empty(scan.fatal)) {
            res.message = "measured, then exited with code {exit_code} at shutdown"
        } else {
            res.message = "measured, then exited with code {exit_code} at shutdown: {scan.fatal}"
        }
    } elif (exit_code != 0) {
        res.status = FileStatus.exit_nonzero
        if (!empty(scan.fatal)) {
            res.message = scan.fatal
        } elif (empty(res.samples)) {
            res.message = "process exited with code {exit_code} before measuring anything, printing no diagnostic (a signal number when it was killed; a missing or unrunnable daslang binary lands here)"
        } else {
            res.message = "process exited with code {exit_code} without a finished dastest report - the suite did not run to its end"
        }
    } elif (!report_ok) {
        res.status = FileStatus.exit_nonzero
        res.message = "dastest wrote no report file - the process ended before the suite finished"
    } elif (empty(res.samples)) {
        res.status = FileStatus.failed
        res.message = "the file ran but produced no benchmark samples (no [benchmark] function, or every arm reported n = 0)"
    } else {
        res.status = FileStatus.ok
    }
}

//! Turn a finished process's output into the result's samples, status and message. `lines` is
//! the merged stdout+stderr, `report_text` the `--json-file` contents (empty when the process
//! never wrote it). `res.status` on entry is the limit verdict (`timeout` / `memory`) or `ok`,
//! and a limit verdict is kept whatever the output says.
def parse_bench_output(lines : array<string>; report_text : string; exit_code : int; var res : FileResult&) {
    var scan <- OutputScan()
    for (line in lines) {
        scan_line(scan, line)
    }
    res.log_tail = join(scan.tail, "\n")
    res.exit_code = exit_code
    res.tier = scan.tier
    let report_ok = note_report(scan, report_text)
    res.samples |> reserve(length(scan.arm_order))
    for (key in scan.arm_order) {
        let fn_name = slice(key, 0, find(key, "/"))
        continue if (key_exists(scan.failed_fns, fn_name))
        res.samples |> emplace(aggregate_arm(key, scan.by_arm[key]))
    }
    return if (res.status == FileStatus.timeout || res.status == FileStatus.memory)
    decide_status(scan, report_ok, exit_code, res)
}

def private rss_mb_from_proc(pid : int) : int {
    let text = fread("/proc/{pid}/status")
    return -1 if (empty(text))
    let at = find(text, "VmRSS:")
    return -1 if (at < 0)
    let line = first_line(slice(text, at + length("VmRSS:")))
    let kb = try_to_int(strip(replace(line, "kB", ""))) ?? -1
    return -1 if (kb < 0)
    return (kb + 1023) / 1024
}

def private rss_mb_from_ps(pid : int) : int {
    var out = ""
    let code = run_and_capture(["/bin/ps", "-o", "rss=", "-p", "{pid}"], out, 5.0)
    return -1 if (code != 0)
    let kb = try_to_int(strip(out)) ?? -1
    return -1 if (kb < 0)
    return (kb + 1023) / 1024
}

//! Resident set size of a live process in MB, rounded up so a process holding memory never
//! reads as 0 and a cap fires a hair early rather than late: `/proc` where it exists, `ps`
//! otherwise (macOS), -1 when neither answers - a caller reading -1 as "under the cap" is what
//! makes a missing source disable the memory limit rather than kill every child.
def process_rss_mb(pid : int) : int {
    let from_proc = rss_mb_from_proc(pid)
    return from_proc if (from_proc >= 0)
    return rss_mb_from_ps(pid)
}

//! The three lanes differ only in argv. `aot` needs BOTH `-use-aot` on the host and `--use-aot`
//! on dastest: without the second one every function silently interprets and the lane measures
//! the interpreter under an AOT label. With it, a function missing its stub is `error[50101]`,
//! so a run that finishes is proof the stubs were live.
def private bench_argv(bin, repo_root : string; f : BenchFile; lane : string; report_path : string; repeat : int) : array<string> {
    var argv <- [bin]
    if (lane == "jit") {
        argv |> push("-jit")
    } elif (lane == "aot") {
        argv |> push("-use-aot")
    }
    argv |> push(to_generic_path(path_join(repo_root, "dastest/dastest.das")))
    argv |> push("--")
    if (lane == "aot") {
        argv |> push("--use-aot")
    }
    argv |> push_from(["--bench", "--bench-format", "json", "--json-file", report_path,
        "--count", "{repeat}", "--test", to_generic_path(path_join(path_join(repo_root, "benchmarks"), f.path))])
    return <- argv
}

def private new_result(f : BenchFile; lane : string) : FileResult {
    return <- FileResult(path = f.path, id = f.id, group = f.group, lane = lane)
}

//! Run one benchmark file in one lane under the limits, from the repo root. The child is
//! killed when it outlives `timeout_seconds` or its RSS passes `memory_limit_mb`; the result
//! then says so and keeps the arms that completed before the kill.
def run_bench_file(bin, repo_root : string; f : BenchFile; lane : string; limits : RunLimits) : FileResult {
    var res <- new_result(f, lane)
    let skipped = skip_reason(f, lane)
    if (!empty(skipped)) {
        res.status = FileStatus.skipped
        res.message = skipped
        return <- res
    }
    var err = ""
    let report_path = create_temp_file("bench_stand_", ".json", err)
    if (empty(report_path)) {
        res.status = FileStatus.spawn_failed
        res.message = "cannot create the dastest report file: {err}"
        return <- res
    }
    remove(report_path)
    let argv <- bench_argv(bin, repo_root, f, lane, report_path, limits.repeat)
    var lines : array<string>
    var exit_code = process_running
    let t0 = ref_time_ticks()
    let noenv : array<string>
    with_process(argv, repo_root, noenv) $(var p) {
        unsafe {
            if (process_pid(p) <= 0) {
                res.status = FileStatus.spawn_failed
                res.message = "cannot spawn {argv[0]}"
                return
            }
            var checks = 0
            while (true) {
                process_drain(p) $(line) {
                    lines |> push(line)
                }
                exit_code = process_poll(p)
                break if (exit_code != process_running)
                let elapsed = double(get_time_usec(t0)) / 1000000.0lf
                if (elapsed > double(limits.timeout_seconds)) {
                    res.status = FileStatus.timeout
                    res.message = "killed after {limits.timeout_seconds} s (timeout_seconds)"
                    process_kill(p)
                    exit_code = process_wait(p, 10.0)
                    break
                }
                checks++
                if (checks % 10 == 0) {
                    let rss = process_rss_mb(process_pid(p))
                    if (rss > limits.memory_limit_mb) {
                        res.status = FileStatus.memory
                        res.message = "killed at {rss} MB resident (memory_limit_mb = {limits.memory_limit_mb})"
                        process_kill(p)
                        exit_code = process_wait(p, 10.0)
                        break
                    }
                }
                sleep(100u)
            }
            process_drain(p) $(line) {
                lines |> push(line)
            }
        }
    }
    res.seconds = double(get_time_usec(t0)) / 1000000.0lf
    return <- res if (res.status == FileStatus.spawn_failed)
    let report_text = fread(report_path)
    remove(report_path)
    parse_bench_output(lines, report_text, exit_code, res)
    return <- res
}

//! Empty when the AOT binary exists and reports the AOT tier; otherwise the reason. An absent
//! binary is the ordinary case on a tree nobody built the AOT target in.
def probe_aot(aot_bin : string) : string {
    return "no AOT binary at {aot_bin} - build the test_aot target first" if (!fexist(aot_bin))
    var err = ""
    let probe = create_temp_file("bench_stand_aot_", ".das", err)
    return "cannot create the probe file: {err}" if (empty(probe))
    fwrite(probe, "options gen2\n[export]\ndef main() \{\n    print(\"aot-probe-ok\\n\")\n\}\n")
    var out = ""
    let code = run_and_capture([aot_bin, "-use-aot", probe], out, 120.0)
    remove(probe)
    // The marker, as with the JIT probe: it prints only if the binary got as far as running a
    // program. A trivial program has no stubs of its own, so this proves the binary runs, never
    // that a benchmark's stubs are present - `--use-aot` and error[50101] prove that, per file.
    return "" if (find(out, "aot-probe-ok") >= 0)
    return "{aot_bin} -use-aot ran no program (exit {code}): {error_or_last_line(out)}"
}

//! Empty when `daslang -jit` runs a trivial program on this box; otherwise the reason the
//! JIT lane is unavailable (dasLLVM not built, a missing LLVM library).
def probe_jit(daslang_bin : string) : string {
    var err = ""
    let probe = create_temp_file("bench_stand_jit_", ".das", err)
    return "cannot create the probe file: {err}" if (empty(probe))
    fwrite(probe, "options gen2\n[export]\ndef main() \{\n    print(\"jit-probe-ok\\n\")\n\}\n")
    var out = ""
    let code = run_and_capture([daslang_bin, "-jit", probe], out, 120.0)
    remove(probe)
    // The marker, not the exit code, is the evidence: it prints only after the JIT compiled and
    // ran the function. A RelWithDebInfo host exits 1 from its allocation tracker's report on
    // every process, and keying on the code there would delete half the stand's measurements
    // on every developer build.
    return "" if (find(out, "jit-probe-ok") >= 0)
    let said = error_or_last_line(out)
    return "daslang -jit produced no jit-compiled run (exit {code}) and printed nothing - is {daslang_bin} runnable?" if (empty(said))
    return "daslang -jit produced no jit-compiled run (exit {code}): {said}"
}
