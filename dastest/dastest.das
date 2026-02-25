options gen2
options indenting = 4

require fio
require strings
require uriparser
require debugapi
require daslib/json_boost
require daslib/jobque_boost

require fs
require suite
require log
require math

options multiple_contexts

let private jsonResultPrefix = "##dastest##\n"

struct FileTiming {
    uri : string
    dt : int     // microseconds
}

var private fileTimings : array<FileTiming>
var private timingOutliers : int = 0


def private collect_input_paths(args : array<string>; var paths : array<string>) {
    for (i in range(length(args) - 1)) {
        if (args[i] == "--test") {
            paths |> push <| args[i + 1]
        }
    }
}


def private collect_files(input_paths : array<string>; var files : array<string>) : bool {
    var inscope cache : table<string; void?>
    for (path in input_paths) {
        if (path |> ends_with(".das")) {
            if (!cache |> key_exists(path)) {
                cache |> insert(path, null)
                files |> push <| path
            }
        } else {
            if (!fs::scan_dir(path, cache, files, ".das")) {
                log::error("Unable to scan given path '{path}'")
                return false
            }
        }
    }
    return true
}


def private get_float_arg(args : array<string>; name : string; def_val : float) : float {
    let idx = find_index(args, name)
    return idx >= 0 && idx + 1 < length(args) ? float(args[idx + 1]) : def_val
}

def private get_int_arg(args : array<string>; name : string; def_val : int) : int {
    let idx = find_index(args, name)
    return idx >= 0 && idx + 1 < length(args) ? int(args[idx + 1]) : def_val
}

struct IsoInput {
    uri : string
    cmd : string
}

struct IsolatedResult {
    uri : string
    cmd : string
    exitCode : int
    parsedResult : bool
    status : SuiteResult
    fileDt : int
    err : array<string>
    log : array<string>
}

[export]
def main() {
    var args <- get_command_line_arguments()
    log::init_log(args)
    let runIdx = args |> find_index("--run")
    if (runIdx != -1) {
        let res = suite::test_file(args[runIdx + 1], SuiteCtx(args))
        log::info_raw("\n{jsonResultPrefix}{write_json(JV(res))}\n{jsonResultPrefix}")
        return
    }

    var res : SuiteResult
    let startTime = ref_time_ticks()

    let timeout = args |> get_float_arg("--timeout", 600.) // 10min
    if (timeout > 0.) {
        unsafe {
            var mainCtx & = this_context()
            new_thread <| @capture(& res, & mainCtx) {
                fio::sleep(uint(timeout * 1000.))
                unsafe {
                    mainCtx |> invoke_in_context("timeout_tests", res, startTime, timeout)
                }
            }
        }
    }

    var inputPaths : array<string>
    collect_input_paths(args, inputPaths)
    var files : array<string>
    if (!collect_files(inputPaths, files)) {
        unsafe {
            fio::exit(1)
        }
        return
    }

    timingOutliers = args |> get_int_arg("--timing-outliers", 0)

    let isolatedMode = args |> has_value("--isolated-mode")
    var ctx <- SuiteCtx(args)
    if (!isolatedMode) {
        for (file in files) {
            let uri = ctx.uriPaths ? file_name_to_uri(file) : file
            let fileTime = ref_time_ticks()
            let status = suite::test_file(file, ctx)
            let fileDt = get_time_usec(fileTime)
            if (status.errors + status.failed == 0) {
                if (status.total > 0) {
                    log::green("PASS {uri} {time_dt_hr(fileDt)}")
                }
            } else {
                log::red("FAIL {uri} {time_dt_hr(fileDt)}")
            }
            if (timingOutliers > 0) {
                fileTimings |> push(FileTiming(uri = clone_string(uri), dt = fileDt))
            }
            res += status
        }
    } else {
        let totalFiles = length(files)
        let totalThreads = max(1, args |> get_int_arg("--isolated-mode-threads", get_total_hw_threads() * 2)) // magic x2, os should be able to schedule sub processes in parallel
        log::info("Running {totalFiles} tests in isolated mode with {totalThreads} threads\n")
        with_channel(totalFiles) $(inputChannel) {
            with_channel(totalFiles) $(outputChannel) {
                with_job_status(totalThreads) $(completion) {

                    for (t in range(totalThreads)) {
                        new_thread <| @ {
                            for_each_clone(inputChannel) $(input : IsoInput#) {
                                var isoRes : IsolatedResult
                                isoRes.uri = clone_string(input.uri)
                                isoRes.cmd = clone_string(input.cmd)
                                let fileTime = ref_time_ticks()
                                unsafe {
                                    isoRes.exitCode = popen(input.cmd) $(f) {
                                        if (f == null) {
                                            // log::error("Internal error: Failed to execute cmd > {input.cmd}")
                                            isoRes.err |> push("Internal error: Failed to execute cmd > {input.cmd}")
                                            return
                                        }
                                        var readJson = false
                                        var jsonResult = ""
                                        while (!feof(f)) {
                                            let st = fgets(f)
                                            if (st == jsonResultPrefix) {
                                                readJson = !readJson
                                                continue
                                            }
                                            if (readJson) {
                                                jsonResult += st
                                            } else {
                                                // log::info_raw(st)
                                                isoRes.log |> push(st)
                                            }
                                        }
                                        var jsError : string
                                        var js = jsonResult |> read_json(jsError)
                                        if (!empty(jsError)) {
                                            // log::error("Internal error: failed to parse json result {jsError}")
                                            isoRes.err |> push("Internal error: failed to parse json result {jsError}")
                                        } elif (js != null) {
                                            isoRes.parsedResult = true
                                            isoRes.status = SuiteResult(js)
                                        }
                                    }
                                }
                                isoRes.fileDt = get_time_usec(fileTime)

                                outputChannel |> push_clone(isoRes)
                                outputChannel |> notify()
                            }

                            inputChannel |> release()
                            outputChannel |> release()
                            completion |> notify_and_release()
                        }
                    }

                    for (file in files) {
                        let uri = ctx.uriPaths ? file_name_to_uri(file) : file
                        let jitstr = jit_enabled() ? "-jit" : ""
                        let singleTest = "{args[0]} {args[1]} {jitstr} -- --run {file} {log::useTtyColors ? " --color" : ""}"
                        inputChannel |> push_clone(IsoInput(uri = clone_string(uri), cmd = clone_string(singleTest)))
                        inputChannel |> notify()
                    }

                    for (isoRes in each_clone(outputChannel, type<IsolatedResult>)) {
                        res += isoRes.status
                        for (err in isoRes.err) {
                            log::error(err)
                        }
                        for (l in isoRes.log) {
                            log::info_raw(l)
                        }
                        if (timingOutliers > 0) {
                            fileTimings |> push(FileTiming(uri = clone_string(isoRes.uri), dt = isoRes.fileDt))
                        }
                        if (isoRes.exitCode == 0 && isoRes.parsedResult && isoRes.status.failed + isoRes.status.errors == 0) {
                            log::green("PASS {isoRes.uri} {time_dt_hr(isoRes.fileDt)}")
                        } else {
                            log::red("FAIL {isoRes.uri} {time_dt_hr(isoRes.fileDt)}")
                            if (isoRes.exitCode != 0 || !isoRes.parsedResult) {
                                res.total += 1
                                res.errors += 1
                            }
                            if (!isoRes.parsedResult) {
                                log::red("`{isoRes.uri}` execution was interrupted, crashed for example")
                                log::info("to get more information, run the test manually\n> {isoRes.cmd}")
                            }
                            if (isoRes.exitCode != 0) {
                                log::red("exit status {isoRes.exitCode}")
                            }
                        }
                    }

                    completion |> join()
                }
            }
        }
    }
    finish_tests(res, startTime)
}


[export, pinvoke]
def timeout_tests(var res : SuiteResult; start_time : int64; timeout_sec : float) {
    res.errors += 1
    res.total += 1
    log::error("Test timed out after {timeout_sec}s")
    finish_tests(res, start_time)
}


def private print_timing_outliers() {
    let n = length(fileTimings)
    // compute mean
    var sum = 0.0lf
    for (ft in fileTimings) {
        sum += double(ft.dt)
    }
    let mean = sum / double(n)
    // compute stddev
    var variance = 0.0lf
    for (ft in fileTimings) {
        let diff = double(ft.dt) - mean
        variance += diff * diff
    }
    let stddev = sqrt(variance / double(n))
    // sort descending by time
    sort(fileTimings) <| $(a, b) {
        return a.dt > b.dt
    }
    let count = min(timingOutliers, n)
    let threshold = mean + 2.0lf * stddev
    // only print if there are actual outliers (top entry above threshold) or if few files
    var meanStr = ""
    meanStr = fmt(":.3f", mean / 1000000.0lf)
    var stddevStr = ""
    stddevStr = fmt(":.3f", stddev / 1000000.0lf)
    var thresholdStr = ""
    thresholdStr = fmt(":.3f", threshold / 1000000.0lf)
    log::info("Timing: mean {meanStr}s, stddev {stddevStr}s, threshold {thresholdStr}s")
    log::info("Top {count} slowest files:")
    for (i in range(count)) {
        let dtSec = double(fileTimings[i].dt) / 1000000.0lf
        let marker = double(fileTimings[i].dt) > threshold ? " [outlier]" : ""
        var dtStr = ""
        dtStr = fmt(":.3f", dtSec)
        var line = "  {dtStr}s  {fileTimings[i].uri}{marker}"
        log::info(line)
    }
}


def finish_tests(res : SuiteResult; start_time : int64) {
    let resCode = res.failed + res.errors
    let totalDt = get_time_usec(start_time)
    log::info("\n{to_string(res)}")
    if (resCode == 0 && timingOutliers > 0 && length(fileTimings) > 1) {
        print_timing_outliers()
    }
    if (resCode == 0) {
        log::green("SUCCESS! {time_dt_hr(totalDt)}")
    } else {
        log::red("FAILED! {time_dt_hr(totalDt)}")
    }
    unsafe {
        fio::exit(resCode)
    }
}


// options debugger
// require daslib/debug
