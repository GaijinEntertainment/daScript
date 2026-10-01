options gen2

require strings
require math
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of tests/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang tests/REVIEW.das - exit 0 clean, 1 with findings.

let private FOLDER = "tests"

//! a write call the gate recognizes: the function name, and which arguments are paths it writes
struct private WriteCall {
    name : string
    path_args : array<int>
    is_method : bool
}

def private write_calls : array<WriteCall> {
    let first_arg_writers = ["remove", "remove_result", "rmdir", "rmdir_result", "rmdir_rec", "rmdir_rec_result",
                             "mkdir", "mkdir_result", "mkdir_rec", "mkdir_rec_result", "fwrite", "fwrite_if_changed",
                             "stbi_write_png", "stbi_write_bmp", "stbi_write_tga", "stbi_write_jpg", "stbi_write_hdr"]
    var calls <- [for (name in first_arg_writers); WriteCall(name = name, path_args <- [0])]
    calls |> emplace(WriteCall(name = "rename", path_args <- [0, 1]))
    calls |> emplace(WriteCall(name = "rename_result", path_args <- [0, 1]))
    calls |> emplace(WriteCall(name = "copy_file", path_args <- [1]))
    calls |> emplace(WriteCall(name = "copy_file_result", path_args <- [1]))
    calls |> emplace(WriteCall(name = "save", path_args <- [0], is_method = true))
    return <- calls
}

//! the arguments of the call whose `(` sits at `open`, split on top-level commas; string
//! literals, interpolations and nested brackets are kept whole
def private call_args(text : string; open : int) : array<string> {
    var args : array<string>
    peek_data(text) $(d) {
        var depth = 0
        var in_string = false
        var start = open + 1
        var i = open + 1
        while (i < length(d)) {
            let c = int(d[i])
            if (in_string) {
                if (c == '\\') {
                    i += 2
                    continue
                }
                if (c == '"') {
                    in_string = false
                }
            } elif (c == '"') {
                in_string = true
            } elif (c == '(' || c == '[' || c == '{') {
                depth++
            } elif (c == ')' || c == ']' || c == '}') {
                if (depth == 0) {
                    args |> push(strip(slice(d, start, i)))
                    return
                }
                depth--
            } elif (c == ',' && depth == 0) {
                args |> push(strip(slice(d, start, i)))
                start = i + 1
            }
            i++
        }
    }
    return <- args
}

//! the identifier an expression opens with when it opens with a call - `f(...)` gives "f"
def private leading_callee(expr : string) : string {
    var callee = ""
    peek_data(expr) $(d) {
        var n = 0
        while (n < length(d) && is_ident_byte(int(d[n]))) {
            n++
        }
        if (n > 0 && n < length(d) && d[n] == uint8('(')) {
            callee = slice(d, 0, n)
        }
    }
    return callee
}

let private PATH_BUILDERS <- { "get_das_root", "path_join", "join_path", "dir_name", "string" }

//! a call to anything but a path builder or a repo-root helper hands back its own value, whatever
//! its arguments name - a helper that copies a fixture into a temp directory returns a temp path
def private is_root_path(expr : string; root_funcs : table<string>; root_vars : table<string>) : bool {
    let callee = leading_callee(strip(expr))
    return false if (!empty(callee) && !key_exists(PATH_BUILDERS, callee) && !key_exists(root_funcs, callee))
    return true if (find(expr, "get_das_root()") >= 0)
    for (f in keys(root_funcs)) {
        return true if (contains_word(expr, f) && find(expr, "{f}(") >= 0)
    }
    for (v in keys(root_vars)) {
        return true if (contains_word(expr, v))
    }
    return false
}

//! a string literal with no interpolation up front and no absolute root resolves against the
//! working directory, which is the repo root when the suites run
def private is_cwd_relative_literal(expr : string) : bool {
    let interpolated_or_absolute = starts_with(expr, "\"\{") || starts_with(expr, "\"/") || starts_with(expr, "\"\\")
    return false if (!starts_with(expr, "\"") || starts_with(expr, "\"\"") || interpolated_or_absolute)
    return !(length(expr) > 2 && slice(expr, 2, 3) == ":")
}

//! a file split into its top-level functions; the text before the first `def` is chunk 0
def private top_level_chunks(text : string) : array<string> {
    var chunks : array<string>
    peek_data(text) $(d) {
        var start = 0
        while (true) {
            let at = find(d, "\ndef ", start)
            if (at < 0) {
                chunks |> push(slice(d, start))
                break
            }
            chunks |> push(slice(d, start, at + 1))
            start = at + 1
        }
    }
    return <- chunks
}

def private def_name(chunk : string) : string {
    return "" if (!starts_with(chunk, "def "))
    let skip = starts_with(chunk, "def private ") ? 12 : 4
    var name = ""
    peek_data(chunk) $(d) {
        var i = skip
        while (i < length(d) && is_ident_byte(int(d[i]))) {
            i++
        }
        name = slice(d, skip, i)
    }
    return name
}

//! helpers whose `return` hands back a path built from the repo root, closed over each other
def private root_helpers(chunks : array<string>) : table<string> {
    var helpers : table<string>
    var changed = true
    while (changed) {
        changed = false
        for (chunk in chunks) {
            let name = def_name(chunk)
            continue if (empty(name) || key_exists(helpers, name))
            let header_end = find(chunk, "\{")
            continue if (header_end < 0 || find(slice(chunk, 0, header_end), ": string") < 0)
            var inscope locals <- root_vars_of(chunk, helpers, default<table<string>>)
            for (line in split(chunk, "\n")) {
                let body = strip(line)
                if (starts_with(body, "return ") && is_root_path(slice(body, 7), helpers, locals)) {
                    helpers |> insert(name)
                    changed = true
                    break
                }
            }
        }
    }
    return <- helpers
}

//! the name and the right-hand side of `let|var [inscope] NAME [: T] = RHS` or `NAME = RHS`;
//! an empty name when the line binds nothing
def private binding(line : string) : tuple<name : string; rhs : string> {
    var body = strip(line)
    if (starts_with(body, "let ") || starts_with(body, "var ")) {
        body = slice(body, 4)
    }
    if (starts_with(body, "inscope ")) {
        body = slice(body, 8)
    }
    var result : tuple<name : string; rhs : string>
    peek_data(body) $(d) {
        let eq = find(d, "=", 0)
        return if (eq <= 0 || eq + 1 >= length(d) || int(d[eq + 1]) == '=')
        var name_end = 0
        while (name_end < eq && (is_ident_byte(int(d[name_end])) || d[name_end] == uint8('.'))) {
            name_end++
        }
        return if (name_end == 0)
        let between = strip(slice(d, name_end, eq))
        return if (!empty(between) && !starts_with(between, ":"))
        result = (name = slice(d, 0, name_end), rhs = slice(d, eq + 1))
    }
    return result
}

//! names bound to a repo-root path, closed over each other
def private root_vars_of(chunk : string; root_funcs : table<string>; seed : table<string>) : table<string> {
    var vars := seed
    var changed = true
    while (changed) {
        changed = false
        for (line in split(chunk, "\n")) {
            let (name, rhs) = binding(line)
            continue if (empty(name) || key_exists(vars, name))
            if (is_root_path(rhs, root_funcs, vars)) {
                vars |> insert(name)
                changed = true
            }
        }
    }
    return <- vars
}

def private is_write_mode(mode : string) : bool {
    return starts_with(mode, "\"w") || starts_with(mode, "\"a") || find(mode, "+") >= 0
}

def private line_of(text : string; at : int) : int {
    var line = 1
    peek_data(text) $(d) {
        for (i in range(min(at, length(d)))) {
            if (d[i] == uint8('\n')) {
                line++
            }
        }
    }
    return line
}

def private report(path : string; text : string; at : int; call : string; arg : string) {
    gate_finding(path, line_of(text, at), "{call}(...) writes `{arg}` under the repo tree - write it under test_temp_dir() (dastest/testing_boost) or a directory from create_temp_directory (daslib/fio)")
}

//! every `needle(` in `chunk` that is a call of that name, not the tail of a longer name
def private call_sites(chunk : string; needle : string; is_method : bool) : array<int> {
    var sites : array<int>
    let pattern = is_method ? ".{needle}(" : "{needle}("
    peek_data(chunk) $(d) {
        var from = 0
        while (true) {
            let at = find(d, pattern, from)
            break if (at < 0)
            from = at + length(pattern)
            continue if (!is_method && at > 0 && (is_ident_byte(int(d[at - 1])) || d[at - 1] == uint8('.')))
            sites |> push(at + length(pattern) - 1)
        }
    }
    return <- sites
}

def private check_file(path : string; calls : array<WriteCall>) {
    let text = strip_line_comments(fread(path))
    var inscope chunks <- top_level_chunks(text)
    var inscope root_funcs <- root_helpers(chunks)
    let top_level = join([for (line in split(text, "\n")); line; where starts_with(line, "let ") || starts_with(line, "var ")], "\n")
    var inscope globals <- root_vars_of(top_level, root_funcs, default<table<string>>)
    var offset = 0
    for (chunk in chunks) {
        var inscope vars <- root_vars_of(chunk, root_funcs, globals)
        for (wc in calls) {
            for (open in call_sites(chunk, wc.name, wc.is_method)) {
                var inscope args <- call_args(chunk, open)
                for (k in wc.path_args) {
                    if (k < length(args) && is_root_path(args[k], root_funcs, vars)) {
                        report(path, text, offset + open, wc.name, args[k])
                    }
                }
            }
        }
        for (open in call_sites(chunk, "fopen", false)) {
            var inscope args <- call_args(chunk, open)
            if (length(args) >= 2 && is_write_mode(args[1]) && (is_root_path(args[0], root_funcs, vars) || is_cwd_relative_literal(args[0]))) {
                report(path, text, offset + open, "fopen", args[0])
            }
        }
        offset += length(chunk)
    }
}

//! every file dastest runs as a test: a `.das` with no `_`-prefixed name or directory on its path
def private check_folder(folder : string; calls : array<WriteCall>) {
    dir_rec(folder) $(rel : string; is_dir : bool) {
        let underscored = starts_with(rel, "_") || find(rel, "/_") >= 0 || find(rel, "\\_") >= 0
        return if (is_dir || underscored || !ends_with(rel, ".das") || base_name(rel) == "REVIEW.das")
        check_file("{folder}/{rel}", calls)
    }
}

//! `--root <dir>` scans another folder in place of `tests/` - the gate's own fixture test uses it
[export]
def main() : int {
    var folder = FOLDER
    let args <- get_command_line_arguments()
    for (i in range(length(args) - 1)) {
        if (args[i] == "--root") {
            folder = args[i + 1]
        }
    }
    var inscope calls <- write_calls()
    check_folder(folder, calls)
    return gate_verdict("tests")
}
