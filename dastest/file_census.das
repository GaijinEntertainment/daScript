options gen2
module file_census shared

require strings
require daslib/strings_boost
require daslib/fio

//! The per-file census of a test tree, read from the text without
//! compiling: which files dastest runs (a `.das` whose name and every
//! directory on its path avoid the `_` prefix), how many `[test]`
//! functions and `run("...")` arms each holds, each test's
//! `description`, the file's `//!` header line, and its fence
//! signature - the `options` that keep two files from sharing one
//! program. The `tests/` gate (`REVIEW.das`) and the test-audit tool
//! both read this one census.

enum TestFileClass {
    suite       // one or more [test] functions
    // an `expect` directive, or a failed_/cant_/invalid_ name: a
    // compile-fail fixture
    fixture
    script      // no [test] and no expect: a bare script that passes when it runs
}

struct TestEntry {
    funcName : string
    description : string
    // why this test's file cannot share a program with another; "" when
    // it can
    standalone : string
    timeoutSec : float
    line : int
}

struct TestFileCensus {
    path : string           // generic path relative to the census root
    folder : string         // first path component under the root, "." at the root
    stem : string
    cls : TestFileClass
    tests : array<TestEntry>
    arms : int              // `run("...")` sub-arms
    lines : int
    fenceOptions : array<string>    // sorted, deduplicated; `fence_signature` joins them
    noAot : bool
    header : string         // the file's first `//!` line, comment marker stripped; "" when none
    unparsedAnnotationLines : array<int>    // a `[test(` that does not close on its line
}

//! a fence option changes what program a file compiles into;
//! formatting and lint knobs do not. A `_`-prefixed option is a
//! module's own (`_flatten_expand_lerp`) and fences, except the lint
//! knobs named here
var private NON_FENCE_OPTIONS <- {
    "gen2", "indenting", "no_unused_function_arguments", "no_unused_block_arguments",
    "_nolint", "_lint", "_no_linq_perf_warn", "_cyclomatic_complexity", "_function_length",
    "_unused_private", "_ascii_strings", "_comment_hygiene"
}

def is_fence_option(name : string) : bool => !(NON_FENCE_OPTIONS |> key_exists(name))

def fence_signature(c : TestFileCensus) : string => join(c.fenceOptions, ",")

def is_fixture_name(stem : string) : bool =>
    (stem |> starts_with("failed_") || stem |> starts_with("cant_") ||
    stem |> starts_with("invalid_"))

def private strip_line_comment(line : string) : string {
    let c = find(line, "//")
    return c < 0 ? line : slice(line, 0, c)
}

def private is_test_annotation(line : string) : bool =>
    (line == "[test]" || line |> starts_with("[test,") || line |> starts_with("[test(") ||
    line |> starts_with("[test ") || contains(line, " test]") || contains(line, ",test]") ||
    contains(line, " test,") || contains(line, ", test("))

//! the string argument `name` of a one-line annotation:
//! `[test(description = "...", timeout = 5)]`
def private annotation_string_arg(line : string; name : string) : string {
    var result = ""
    peek_data(line) $(d) {
        let key = find(d, name)
        return if (key < 0)
        let eq = find(d, "=", key + length(name))
        return if (eq < 0)
        let open = find(d, "\"", eq)
        return if (open < 0)
        var close = open + 1
        while (close < length(d)) {
            let ch = int(d[close])
            if (ch == '\\') {
                close += 2
                continue
            }
            break if (ch == '"')
            close++
        }
        result = slice(d, open + 1, close)
    }
    return result
}

def private annotation_number_arg(line : string; name : string) : float {
    var result = 0.0
    peek_data(line) $(d) {
        let key = find(d, name)
        return if (key < 0)
        let eq = find(d, "=", key + length(name))
        return if (eq < 0)
        let at = skip_white_space(d, eq + 1)
        var end = at
        while (end < length(d)) {
            let ch = int(d[end])
            break if (!(is_number(ch) || ch == '.'))
            end++
        }
        if (end > at) {
            result = to_float(slice(d, at, end))
        }
    }
    return result
}

def private record_options(var c : TestFileCensus; body : string) {
    for (part in split(body, ",")) {
        var nm = strip(part)
        var value = ""
        let eq = find(nm, "=")
        if (eq >= 0) {
            value = strip(slice(nm, eq + 1))
            nm = strip(slice(nm, 0, eq))
        }
        let sp = find(nm, " ")
        if (sp >= 0) {
            nm = slice(nm, 0, sp)
        }
        continue if (empty(nm))
        if (nm == "no_aot") {
            c.noAot = true
        }
        // the value is part of the fence: `auto_inline_functions = false` and
        // `= true` are two programs, and `gen2 = false` (gen1 syntax) fences
        // although `gen2` itself does not
        let fence = empty(value) || value == "true" ? nm : "{nm}={value}"
        let fences = nm == "gen2" ? value == "false" : is_fence_option(nm)
        if (fences && find_index(c.fenceOptions, fence) < 0) {
            c.fenceOptions |> push(fence)
        }
    }
}

//! the function name on a `def` line: `def test_x(t : T?) {` ->
//! `test_x`
def private def_name(line : string) : string {
    var result = ""
    peek_data(line) $(d) {
        let at = skip_white_space(d, length("def "))
        var end = at
        while (end < length(d)) {
            let ch = int(d[end])
            break if (!(is_alnum(ch) || ch == '_'))
            end++
        }
        result = slice(d, at, end)
    }
    return result
}

//! the header block ends where the first declaration starts
def private is_declaration_start(line : string) : bool =>
    (line |> starts_with("[") || line |> starts_with("def ") || line |> starts_with("struct ") ||
    line |> starts_with("class ") || line |> starts_with("enum ") || line |> starts_with("let ") ||
    line |> starts_with("var "))

def census_text(path : string; text : string) : TestFileCensus {
    var c : TestFileCensus
    c.path = to_generic_path(path)
    let slash = find(c.path, "/")
    c.folder = slash < 0 ? "." : slice(c.path, 0, slash)
    c.stem = stem(path)
    var sawExpect = false
    var pending : TestEntry
    var havePending = false
    var lineNo = 0
    var inHeader = true
    var headerOpen = false
    for (raw in split(text, "\n")) {
        lineNo++
        c.lines++
        let trimmedRaw = strip(raw)
        if (inHeader && trimmedRaw |> starts_with("//!") && (empty(c.header) || headerOpen)) {
            // consecutive `//!` lines of the header block are one claim
            let piece = strip(slice(trimmedRaw, length("//!")))
            c.header = empty(c.header) ? piece : "{c.header} {piece}"
            headerOpen = true
        } elif (!(trimmedRaw |> starts_with("//!"))) {
            headerOpen = false
        }
        let line = strip(strip_line_comment(raw))
        continue if (empty(line))
        if (inHeader && is_declaration_start(line)) {
            inHeader = false
        }
        if (havePending && line |> starts_with("def ")) {
            pending.funcName = def_name(line)
            c.tests |> push(pending)
            havePending = false
        }
        if (line |> starts_with("options ")) {
            record_options(c, strip(slice(line, length("options "))))
        } elif (line |> starts_with("expect ")) {
            sawExpect = true
        } elif (line |> starts_with("[") && is_test_annotation(line)) {
            if (!(line |> ends_with("]"))) {
                c.unparsedAnnotationLines |> push(lineNo)
            }
            pending = TestEntry(description = annotation_string_arg(line, "description"),
                                standalone = annotation_string_arg(line, "standalone"),
                timeoutSec = annotation_number_arg(line, "timeout"), line = lineNo)
            havePending = true
        } elif (contains(line, "|> run(") || line |> starts_with("run(")) {
            c.arms++
        }
    }
    sort(c.fenceOptions)
    if (sawExpect || is_fixture_name(c.stem)) {
        c.cls = TestFileClass.fixture
    } elif (!empty(c.tests)) {
        c.cls = TestFileClass.suite
    } else {
        c.cls = TestFileClass.script
    }
    return <- c
}

def census_file(root : string; rel : string) : TestFileCensus =>
    <- census_text(rel, fread(path_join(root, rel)))

def is_run_file(rel : string) : bool {
    //! true when dastest runs the file as a test: a `.das` with no `_`-
    //! or `.`-prefixed name or directory on its path, and not a folder's
    //! `REVIEW.das` gate
    return false if (extension(rel) != ".das" || base_name(rel) == "REVIEW.das")
    for (part in split(to_generic_path(rel), "/")) {
        return false if (part |> starts_with("_") || part |> starts_with("."))
    }
    return true
}

//! every run file under `root`, in walk order
def census_tree(root : string; var out : array<TestFileCensus>) {
    dir_rec(root) $(name, is_dir) {
        return if (is_dir || !is_run_file(name))
        out |> emplace(census_file(root, name))
    }
}
