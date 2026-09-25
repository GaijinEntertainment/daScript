options gen2
options indenting = 4
options no_aot = true

require dastest/testing_boost public
require daslib/defer
require daslib/fio
require strings

def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

def private emitter(t : T?) : string {
    var error : string
    let tmp = create_temp_directory("das_log_level", error)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {error}")
        return ""
    }
    let path = path_join(tmp, "emit.das")
    fwrite(path, "options gen2\n[export]\ndef main \{\n    print(\"P\\n\")\n    to_log(LOG_DEBUG, \"D\\n\")\n    to_log(LOG_INFO, \"I\\n\")\n    to_log(LOG_WARNING, \"W\\n\")\n    to_log(LOG_ERROR, \"E\\n\")\n\}\n")
    return path
}

def private run_at(level, script : string) : string {
    var out : string
    var prefix = ""
    if (!empty(level)) {
        prefix = get_platform_name() == "windows" ? "set \"DAS_LOG_LEVEL={level}\"&& " : "DAS_LOG_LEVEL=\"{level}\" "
    } else {   //! the default floor is the child's own: a caller's floor (preflight runs its gates at info) must not reach it
        prefix = get_platform_name() == "windows" ? "set \"DAS_LOG_LEVEL=\"&& " : "env -u DAS_LOG_LEVEL "
    }
    unsafe(popen_timeout("{prefix}{das_exe()} {script}", 120.0) $(f) {
        if (f != null) {
            out = fread(f)
        }
    })
    return out
}

[test]
def test_debug_is_dropped_by_default(t : T?) {
    let script = emitter(t)
    if (empty(script)) return
    defer() {
        var e : string
        remove(script, e)
    }
    let out = run_at("", script)
    t |> success(find(out, "P") >= 0, "print always survives - a script's output is not a log record:\n{out}")
    t |> success(find(out, "D") < 0, "LOG_DEBUG is below the default floor")
    t |> success(find(out, "I") < 0, "LOG_INFO is below the default floor too")
    t |> success(find(out, "E") >= 0, "LOG_ERROR is above it")
    t |> success(find(out, "W") >= 0, "and LOG_WARNING is the floor itself")
}

[test]
def test_the_floor_moves_both_ways(t : T?) {
    let script = emitter(t)
    if (empty(script)) return
    defer() {
        var e : string
        remove(script, e)
    }
    let low = run_at("debug", script)
    t |> success(find(low, "D") >= 0, "DAS_LOG_LEVEL=debug lets the debug record through:\n{low}")
    let high = run_at("error", script)
    t |> success(find(high, "W") < 0, "DAS_LOG_LEVEL=error drops warning")
    t |> success(find(high, "E") >= 0, "and keeps error")
    t |> success(find(high, "P") >= 0, "print is never filtered")
}

[test]
def test_the_level_name_is_case_insensitive(t : T?) {
    let script = emitter(t)
    if (empty(script)) return
    defer() {
        var e : string
        remove(script, e)
    }
    let high = run_at("ERROR", script)
    t |> success(find(high, "W") < 0, "DAS_LOG_LEVEL=ERROR reads as error, not as a level of 0:\n{high}")
    t |> success(find(high, "E") >= 0, "and keeps error")
    let mixed = run_at("Info", script)
    t |> success(find(mixed, "I") >= 0, "DAS_LOG_LEVEL=Info lets info through")
    t |> success(find(mixed, "D") < 0, "and still drops debug")
}
