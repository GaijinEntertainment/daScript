options gen2
options indenting = 4

//! the lint CLI as a child over a planted tree: a cell plants files
//! under `test_temp_dir()`, runs the lint with its cache under the
//! tree, and reads a file's line off the output
module _lint_child shared public

require dastest/testing_boost public
require strings
require daslib/strings_boost
require daslib/fio
require daslib/command_line   // get_das_exe

def plant(dir, name, content : string) {
    fopen(path_join(dir, name), "wb") $(f) {
        if (f != null) {
            fwrite(f, content)
        }
    }
}

//! the lint over `root` with its cache under `root/cache`; `host`:
//! daslang's flags before the script, `extra`: after `--`
def run_lint(root : string; extra, host : array<string>) : string {
    var argv <- [get_das_exe(), "-dasroot", get_das_root()]
    argv |> push_from(host)
    argv |> push("{get_das_root()}/utils/lint/main.das")
    argv |> push("--")
    argv |> push(root)
    argv |> push("--cache-dir")
    argv |> push(path_join(root, "cache"))
    argv |> push_from(extra)
    var output : string
    unsafe(popen_argv(argv, 0.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
    return output
}

//! the result-cache entries under `root/cache`, the stamp aside
def entry_count(root : string) : int {
    var n = 0
    dir(path_join(root, "cache")) $(name) {
        n++ if (name != "." && name != ".." && name != "lint_hash.txt")
    }
    return n
}

//! the first line of `out` about `file` under `root` that opens with
//! one of `prefixes`: its index, -1 when none, the line in `line`
def line_of(out, root, file : string; prefixes : array<string>; var line : string&) : int {
    let path = to_generic_path(path_join(root, file))
    for (candidate, i in split(out, "\n"), count()) {
        for (prefix in prefixes) {
            if (candidate |> starts_with(prefix) && find(to_generic_path(candidate), path) >= 0) {
                line = candidate
                return i
            }
        }
    }
    return -1
}

//! the status word on `file`'s line - PASS, CACHED, WARN or FAIL -
//! or "?" when the run printed none
def status_of(out, root, file : string) : string {
    var line = ""
    return "?" if (line_of(out, root, file, ["PASS ", "CACHED ", "WARN ", "FAIL "], line) < 0)
    return slice(line, 0, find(line, " "))
}
