options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of src/REVIEW.md (contract: REVIEW_COMMON.md at
// the repo root). Run from the repo root: bin/daslang src/REVIEW.das
// - exit 0 clean, 1 with findings.

struct private NarrowPathFile {
    file : string   //!< a C++ file whose narrow path calls compile only off Windows
    why : string
}

var private NARROW_PATH_FILES <- [
    NarrowPathFile(file = "src/builtin/module_builtin_fio.cpp",
        why = "the UTF-8 file API itself; its narrow calls are its POSIX arm, absent on Windows"),
    NarrowPathFile(file = "src/misc/job_que_cpu_budget.h",
        why = "cgroup files under /sys and /proc, Linux only"),
    NarrowPathFile(file = "src/misc/job_que.cpp",
        why = "/sys/devices/system/cpu, Linux only")
]

let private NARROW_PATH_CALLS <- ["fopen", "freopen", "stat", "_stat", "_stat64", "_findfirst",
    "opendir", "rename", "remove", "unlink", "_access", "access"]

let private FINDING = ("reads its path in the ANSI codepage on Windows - use the UTF-8 file API in "
    + "include/daScript/misc/sysos.h; a file whose narrow calls compile only off Windows gets a "
    + "NARROW_PATH_FILES row naming those platforms")

//! a member call (`.remove(`, `->rename(`) or a member initializer
//! (`: rename(r)`, `, rename(r)`) is not the C call; `::stat(` is
def private is_member_or_initializer(line : string; call_at : int) : bool {
    var res = false
    peek_data(line) $(d) {
        var i = call_at - 1
        while (i >= 0 && int(d[i]) == ' ') {
            i--
        }
        if (i >= 0) {
            let c = int(d[i])
            let scoped = c == ':' && i >= 1 && int(d[i - 1]) == ':'
            res = c == '.' || c == '>' || c == ',' || (c == ':' && !scoped)
        }
    }
    return res
}

//! every top-level argument on the line is a string literal
def private arguments_are_literals(line : string; call_at : int) : bool {
    var res = false
    peek_data(line) $(d) {
        let n = length(d)
        var i = call_at
        while (i < n && int(d[i]) != '(') {
            i++
        }
        i++
        var all_literal = true
        var any_arg = false
        var depth = 0
        var in_string = false
        var at_arg_start = true
        while (i < n && all_literal) {
            let c = int(d[i])
            if (in_string) {
                if (c == '\\') {
                    i++
                } elif (c == '"') {
                    in_string = false
                }
            } elif (c == ')' && depth == 0) {
                break
            } elif (c == ' ') {
                pass
            } elif (at_arg_start) {
                all_literal = c == '"'
                any_arg = true
                in_string = all_literal
                at_arg_start = false
            } elif (c == ',' && depth == 0) {
                at_arg_start = true
            } elif (c == '(') {
                depth++
            } elif (c == ')') {
                depth--
            }
            i++
        }
        res = any_arg && all_literal
    }
    return res
}

def private is_tolerated_narrow_file(path : string) : bool {
    for (row in NARROW_PATH_FILES) {
        return true if (row.file == path)
    }
    return false
}

def private is_cpp_source(path : string) : bool => ends_with(path, ".cpp") || ends_with(path, ".h")

def private is_call_at(line : string; call : string; at : int) : bool =>
    find(line, "{call}(", at) == at || find(line, "{call} (", at) == at

//! REVIEW.md "A diff whose C++ under this folder ... opens, stats,
//! lists, renames or removes a file ..."
def private check_narrow_path_calls {
    for (f in tracked_files(["src"], "narrow path call")) {
        continue if (!is_cpp_source(f) || is_tolerated_narrow_file(f))
        let text = strip_line_comments(fread(f))
        var line_no = 0
        for (line in split(text, "\n")) {
            line_no++
            for (call in NARROW_PATH_CALLS) {
                var at = word_index(line, call)
                while (at >= 0) {
                    if (is_call_at(line, call, at) && !is_member_or_initializer(line, at)
                            && !arguments_are_literals(line, at)) {
                        gate_finding(f, line_no, "`{call}` {FINDING}")
                    }
                    at = word_index(line, call, at + 1)
                }
            }
        }
    }
}

[export]
def main {
    if (!fexist("REVIEW_COMMON.md") || !fexist("CMakeLists.txt")) {
        to_log(LOG_ERROR, "src/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_narrow_path_calls()
    return gate_verdict("src")
}
