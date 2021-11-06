options indenting = 4

require fio
require rtti
require strings
require uriparser
require dastest/fs
require dastest/suite
require dastest/log
require daslib/json_boost


let jsonResultPrefix = "##dastest##\n"


def private collect_input_paths(args: array<string>; var paths: array<string>)
    for i in range(length(args) - 1)
        if args[i] == "--test"
            paths |> push <| args[i + 1]


def private collect_tests_names(args: array<string>; var names: array<string>)
    for i in range(length(args) - 1)
        if args[i] == "--test-names"
            names |> push <| args[i + 1]


def private collect_files(input_paths: array<string>; var files: array<string>)
    var cache: table<string; void?>
    for path in input_paths
        if path |> ends_with(".das")
            if !cache |> key_exists(path)
                let fStat = stat(path)
                if !fStat.is_valid || !fStat.is_reg
                    log::error("Invalid file path {path}", get_line_info())
                    continue
                cache[path] = null
                files |> push <| path
        else
            fs::scan_dir(path, cache, files, ".das")
    delete cache


def create_suite_ctx(args: array<string>; uri_paths: bool)
    var ctx: SuiteCtx
    ctx.dastestRoot = dir_name(args[1]) |> join_path <| "dastest"
    ctx.uriPaths = uri_paths
    collect_tests_names(args, ctx.testNames)
    return <- ctx


[export]
def main()
    var args <- get_command_line_arguments()
    let uriPaths = args |> has_value("--uri-paths")
    let runIdx = args |> find_index("--run")
    if runIdx != -1
        let res = suite::test_file(args[runIdx + 1], create_suite_ctx(args, uriPaths))
        log::info_raw("{jsonResultPrefix}{write_json(JV(res))}", get_line_info())
        return

    let totalTime = ref_time_ticks()
    var inputPaths: array<string>
    collect_input_paths(args, inputPaths)
    var files: array<string>
    collect_files(inputPaths, files)

    var res: SuiteResult
    let isolatedMode = args |> has_value("--isolated-mode")
    if !isolatedMode
        let ctx <- create_suite_ctx(args, uriPaths)
        for file in files
            let uri = uriPaths ? file_name_to_uri(file) : file
            let fileTime = ref_time_ticks()
            let status = suite::test_file(file, ctx)
            let fileDt = get_time_usec(fileTime)
            if status.errors + status.failed == 0
                if status.total > 0
                    log::info("PASS {uri} ({double(fileDt)/1000000.0lf}s)", get_line_info())
            else
                log::info("FAIL {uri} ({double(fileDt)/1000000.0lf}s)", get_line_info())
            res += status
    else
        for file in files
            let uri = uriPaths ? file_name_to_uri(file) : file
            let singleTest = "{args[0]} {args[1]} -- --run {file}"
            var exitCode = 0
            var parsedResult = false
            var status: SuiteResult
            let fileTime = ref_time_ticks()
            unsafe
                exitCode = popen(singleTest) <| $(f)
                    if f == null
                        log::error("Internal error: Failed to execute cmd > {singleTest}", get_line_info())
                        return
                    var readJson = false
                    var jsonResult = ""
                    while !feof(f)
                        let st = fgets(f)
                        if st == jsonResultPrefix
                            readJson = true
                            continue
                        if readJson
                            jsonResult += st
                        else
                            log::info_raw(st, get_line_info())
                    var jsError: string
                    var js = jsonResult |> read_json(jsError)
                    if !empty(jsError)
                        log::error("Internal error: failed to parse json result {jsError}", get_line_info())
                    elif js != null
                        parsedResult = true
                        status = SuiteResult(js)
            let fileDt = get_time_usec(fileTime)
            res += status
            if exitCode == 0 && parsedResult && status.failed + status.errors == 0
                log::info("PASS {uri} ({double(fileDt)/1000000.0lf}s)", get_line_info())
            else
                log::info("FAIL {uri} ({double(fileDt)/1000000.0lf}s)", get_line_info())
                if exitCode != 0
                    log::info("exit status {exitCode}", get_line_info())
                if !parsedResult
                    res.total += 1
                    res.errors += 1
                    log::error("`{uri}` execution was interrupted, crashed for example", get_line_info())
                    log::info("to get more information, run the test manually\n> {singleTest}", get_line_info())

    let resCode = res.failed + res.errors
    let totalDt = get_time_usec(totalTime)
    log::info("\n{res.total} tests, {res.passed} passed, {res.failed} failed, {res.errors} errors, {res.skipped} skipped", get_line_info())
    log::info("{resCode == 0 ? "SUCCESS!" : "FAILED!"} ({double(totalDt)/1000000.0lf}s)", get_line_info())
    unsafe
        fio::exit(resCode)


// options debugger
// require daslib/debug
