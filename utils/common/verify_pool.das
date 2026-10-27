options gen2

module verify_pool

require daslib/fio public
require daslib/jobque_boost public
require daslib/strings_boost
require math
require strings

//! The `-dry-run --ast-verify-batch` worker pool and its verdict,
//! shared by `utils/ast-verify --changed` and preflight's
//! ast-verify gate.

struct public VerifyResult {
    index : int
    //! clean, unverifiable, AST verify, timed out, crashed or killed
    verdict : string
    //! the `AST verify` lines, or the crash/timeout/kill line;
    //! "" when clean
    findings : string
    //! error 20510: the file is in the verifier's own require
    //! closure - never compiled under it
    unverifiable : bool
    ms : int
}

struct private VerifyItem {
    index : int
    path : string
    daslang : string
    cache : string
    timeout : float
}

struct private VerifyEvent {
    started : bool
    result : VerifyResult
}

//! A compile error is not a failure (that gate belongs to whoever
//! owns the file); an `AST verify` line, a crash, a timeout or a
//! kill is - batch mode has no pre-infer walk, so a tree a macro
//! breaks mid-inference surfaces as a crash, not a report.
def public verify_verdict(out : string; rc : int; timeout : float) : VerifyResult {
    var res : VerifyResult
    let lines <- [for (line in split(out, "\n")); strip(line)]
    var hits <- [for (line in lines); line; where find(line, "AST verify") >= 0]
    res.unverifiable = find(out, "error[20510]") >= 0
    res.verdict = res.unverifiable ? "unverifiable" : (empty(hits) ? "clean" : "AST verify")
    if (empty(hits) && !res.unverifiable) {
        //! popen_argv: POSIX signal death = the raw signal number
        //! (2..31), Windows = a negative NTSTATUS; daslang itself only
        //! exits 0/1/255. No CRASH: banner = nothing ran the handler
        //! (an OOM or external kill), which is still not a pass
        let crash_lines <- [for (line in lines); line; where find(line, "CRASH:") >= 0]
        if (rc == popen_timed_out) {
            res.verdict = "timed out"
            hits |> push("timed out after {int(timeout)}s")
        } elif (!empty(crash_lines)) {
            res.verdict = "crashed"
            hits |> push("crashed (exit {rc})")
            hits |> push_from(crash_lines)
        } elif (rc < 0 || rc >= 128 || (rc >= 2 && rc <= 31)) {
            res.verdict = "killed"
            let causes = "the runner, an external kill, or a stack overflow off the main thread"
            hits |> push("killed (exit {rc}), no crash report - {causes}")
        }
    }
    res.findings = join(hits, "\n")
    return <- res
}

//! `-no-module-cache` without a run cache: a record is written even
//! by a compile the verifier reported on, and a module served from a
//! cache is never walked, so a warm default cache passes a broken
//! file on every later run. A run cache is a fresh file the caller
//! owns and deletes - each record in it was walked by the child that
//! wrote it, in this run.
def public verify_argv(daslang, path, cache : string) : array<string> {
    if (empty(cache)) {
        return <- [daslang, "-no-module-cache", "-dry-run", "--ast-verify-batch", path]
    }
    return <- [daslang, "-module-cache", cache, "-dry-run", "--ast-verify-batch", path]
}

def private log_in_flight(in_flight : table<int; int64>; files : array<string>) {
    var names <- [for (k, t0 in keys(in_flight), values(in_flight));
                  "{files[k]} ({get_time_usec(t0) / 1000000}s)"]
    sort(names)
    to_log(LOG_INFO, "ast-verify: still running: {join(names, ", ")}\n")
}

def private finished_line(i, n : int; file : string; r : VerifyResult) : string {
    let ok = r.unverifiable || empty(r.findings)
    let status = ok ? "ok" : "FAILED"
    return "ast-verify: [{i + 1}/{n}] {file} {status} in {r.ms}ms ({r.verdict})\n"
}

//! One child per file, `workers` in flight, `timeout` seconds each,
//! results in input order. Each file logs `[i/n] file` when its child
//! starts and `[i/n] file ok|FAILED in Nms` when it ends, and every
//! `heartbeat_s` seconds the files still in flight are named.
def public run_verify_pool(daslang : string; files : array<string>; workers : int; timeout : float;
                           cache : string; heartbeat_s : int = 60) : array<VerifyResult> {
    let n = length(files)
    var results : array<VerifyResult>
    results |> resize(n)
    for (r in results) {
        r.verdict = "lost"
        r.findings = "the worker pool returned no result"
    }
    return <- results if (n == 0)
    let nw = clamp(workers, 1, n)
    with_channel(n) $(inCh) {
        with_channel(2 * n) $(outCh) {
            with_job_status(nw) $(done) {
                for (_t in range(nw)) {
                    new_thread <| @ {
                        for_each_clone(inCh) $(item : VerifyItem#) {
                            let begun = VerifyResult(index = item.index)
                            outCh |> push_clone(VerifyEvent(started = true, result = begun))
                            outCh |> notify()
                            let t0 = ref_time_ticks()
                            var out : string
                            let argv <- verify_argv(string(item.daslang), string(item.path),
                                                    string(item.cache))
                            let rc = unsafe(popen_argv(argv, item.timeout, $(f) {
                                if (f != null) {
                                    out := fread(f)
                                }
                            }))
                            var res <- verify_verdict(out, rc, item.timeout)
                            res.index = item.index
                            res.ms = get_time_usec(t0) / 1000
                            outCh |> push_clone(VerifyEvent(started = false, result <- res))
                            outCh |> notify()
                        }
                        inCh |> release()
                        outCh |> release()
                        done |> notify_and_release()
                    }
                }
                for (i in range(n)) {
                    inCh |> push_clone(VerifyItem(index = i, path = files[i], daslang = daslang,
                                                  cache = cache, timeout = timeout))
                    inCh |> notify()
                }
                var in_flight : table<int; int64>
                var finished = 0
                var last_beat = ref_time_ticks()
                let wait_ms = heartbeat_s * 1000
                while (finished < n) {
                    let got = pop_with_timeout_clone(outCh, wait_ms) $(ev : VerifyEvent#) {
                        let i = ev.result.index
                        return if (i < 0 || i >= n)
                        if (ev.started) {
                            in_flight |> insert(i, ref_time_ticks())
                            to_log(LOG_INFO, "ast-verify: [{i + 1}/{n}] {files[i]}\n")
                            return
                        }
                        in_flight |> erase(i)
                        finished++
                        results[i] := ev.result
                        to_log(LOG_INFO, finished_line(i, n, files[i], results[i]))
                    }
                    if (!got && empty(in_flight)) {
                        let lost = n - finished
                        let quiet = "the worker pool went quiet"
                        to_log(LOG_ERROR, "ast-verify: {quiet} with {lost} file(s) unreported\n")
                        break
                    }
                    if (!empty(in_flight) && get_time_usec(last_beat) >= heartbeat_s * 1000000) {
                        log_in_flight(in_flight, files)
                        last_beat = ref_time_ticks()
                    }
                }
                done |> join()
            }
        }
    }
    return <- results
}
