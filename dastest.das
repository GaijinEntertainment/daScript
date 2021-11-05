options indenting = 4

require fio
require rtti
require strings
require dastest/fs
require dastest/suite
require dastest/log
require daslib/json_boost


let jsonResultPrefix = "##dastest##\n"


def private collect_input_paths(args: array<string>; var paths: array<string>)
    for i in range(length(args) - 1)
        if args[i] == "--test"
            paths |> push <| args[i + 1]


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


[export]
def main()
    var args <- get_command_line_arguments()
    let runInOneProcess = args |> has_value("--one-process")
    let runIdx = args |> find_index("--run")
    if runIdx != -1
        let res = suite::test_file(args[runIdx + 1])
        log::info("\n{jsonResultPrefix}{write_json(JV(res))}", get_line_info())
        return

    var inputPaths: array<string>
    collect_input_paths(args, inputPaths)
    var files: array<string>
    collect_files(inputPaths, files)

    var res: SuiteResult
    for file in files
        if runInOneProcess
            res += suite::test_file(file)
        else
            let singleTest = "{args[0]} {args[1]} -- --run {file}"
            var exitCode = 0
            var parsedResult = false
            unsafe
                exitCode = popen(singleTest) <| $(f)
                    if f != null
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
                        var js = read_json(jsonResult, jsError)
                        if !empty(jsError)
                            log::error(jsError, get_line_info())
                        elif js != null
                            parsedResult = true
                            res += SuiteResult(js)
            if exitCode != 0 || !parsedResult
                res.total += 1
                res.errors += 1
                if exitCode != 0
                    log::error("exit status {exitCode}", get_line_info())
                else
                    log::error("`{file}` execution was interrupted, crashed for example\ntry to run test manually: {singleTest}", get_line_info())
                log::info("to get more information, run the test manually\n> {singleTest}", get_line_info())

    log::info("\n{res.total} tests, {res.failed} failed, {res.errors} errors, {res.passed} passed", get_line_info())
    unsafe
        fio::exit(res.failed + res.errors)


// options debugger
// require daslib/debug