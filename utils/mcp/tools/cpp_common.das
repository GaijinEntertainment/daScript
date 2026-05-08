options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public
require grep_usage public
require outline public
require ../cpp_search_config.das public
require daslib/json public
require daslib/json_boost public
require daslib/strings_boost public
require strings public
require math public
require daslib/fio public

let CPP_RULES_FILE = "tree-sitter-daslang/cpp_outline_rules.yml"
let CPP_MAX_FIND_RESULTS = 50

let private CPP_SEARCH_CONFIG_PATH = "utils/mcp/cpp_search_config.das"

struct CppMatch {
    file : string       // relative to das_root, generic-path
    line : int          // 1-based
    kind : string       // function/class/struct/enum/union/typedef/namespace/macro
    name : string       // extracted identifier (best-effort, unqualified)
    qualified : string  // fully-qualified name where available (e.g. "Module::addFunction")
    text : string       // first line of declaration
}

def cpp_default_search_dirs() : array<string> {
    let root = get_das_root()
    var dirs : array<string>
    dirs |> reserve(length(CPP_SEARCH_DIRS))
    for (d in CPP_SEARCH_DIRS) {
        dirs |> push(path_join(root, d))
    }
    return <- dirs
}

def cpp_rule_id_to_kind(rule_id : string) : string {
    if (find(rule_id, "functions") >= 0) {
        return "function"
    } elif (find(rule_id, "classes") >= 0) {
        return "class"
    } elif (find(rule_id, "structs") >= 0) {
        return "struct"
    } elif (find(rule_id, "enums") >= 0) {
        return "enum"
    } elif (find(rule_id, "unions") >= 0) {
        return "union"
    } elif (find(rule_id, "typedefs") >= 0) {
        return "typedef"
    } elif (find(rule_id, "namespaces") >= 0) {
        return "namespace"
    } elif (find(rule_id, "macros") >= 0) {
        return "macro"
    }
    return "unknown"
}

def is_cpp_ident_char(ch : int) : bool {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_' || ch == ':' || ch == '~'
}

def is_cpp_op_char(ch : int) : bool {
    // chars that may appear as part of operator overloads (operator+, operator==, operator<<, etc.)
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '=' || ch == '<' || ch == '>' || ch == '!' || ch == '&' || ch == '|' || ch == '^' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == ']'
}

def cpp_first_line(text : string) : string {
    let nl = find(text, "\n")
    return strip(nl >= 0 ? slice(text, 0, nl) : text)
}

// Find the position of the first '(' that is NOT inside <...> brackets (skips template arg lists).
def find_open_paren(line : string) : int {
    var depth = 0
    for (i in range(length(line))) {
        let ch = character_at(line, i) // nolint:PERF002,PERF003
        if (ch == '<') {
            depth++
        } elif (ch == '>') {
            if (depth > 0) {
                depth--
            }
        } elif (ch == '(' && depth == 0) {
            return i
        }
    }
    return -1
}

// Extract the qualified declared name from a C++ first-line.
// Best-effort. Misses some edge cases (function-pointer typedefs returning fn-ptrs,
// trailing-return-type lambdas, complex template specializations).
// Returns ("name", "qualified") — `qualified` includes ::-scope; `name` is the last segment.
def cpp_extract_name_pair(text : string; kind : string) : tuple<string; string> {
    let line = cpp_first_line(text)
    var qualified = ""
    if (kind == "function") {
        let op = find_open_paren(line)
        if (op > 0) {
            // Walk backwards from `op` over ident/op chars to find the function name span.
            var i = op - 1
            while (i >= 0 && (character_at(line, i) == ' ' || character_at(line, i) == '\t')) { // nolint:PERF002,PERF003
                i--
            }
            let span_end = i + 1
            // First, check if this looks like an `operator<X>` form by walking back over op chars.
            while (i >= 0 && is_cpp_op_char(character_at(line, i))) { // nolint:PERF002,PERF003
                i--
            }
            // Now walk back over ident chars (covers regular names + qualifiers + ~dtor + "operator")
            while (i >= 0 && is_cpp_ident_char(character_at(line, i))) { // nolint:PERF002,PERF003
                i--
            }
            let span_start = i + 1
            if (span_end > span_start) {
                qualified = slice(line, span_start, span_end)
            }
        }
    } elif (kind == "class" || kind == "struct" || kind == "union") {
        let kw = kind == "class" ? "class " : (kind == "struct" ? "struct " : "union ")
        let kw_pos = find(line, kw)
        if (kw_pos >= 0) {
            var rest = strip(slice(line, kw_pos + length(kw)))
            // skip optional API macros (any UPPER_SNAKE token before the name)
            while (length(rest) > 0 && (first_character(rest) >= 'A' && first_character(rest) <= 'Z')) {
                // Keep looking — only skip if the first token is followed by a space and the *next*
                // token starts with another uppercase letter or is a known macro.
                let first_space = find(rest, " ")
                if (first_space < 0) {
                    break
                }
                let head = slice(rest, 0, first_space)
                // Heuristic: if the head is all uppercase/underscore and ≥ 3 chars, treat it as macro and skip.
                var all_upper = true
                for (i in range(length(head))) {
                    let c = character_at(head, i) // nolint:PERF002,PERF003
                    if (!((c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9'))) {
                        all_upper = false
                        break
                    }
                }
                if (all_upper && length(head) >= 3) {
                    rest = strip(slice(rest, first_space + 1))
                } else {
                    break
                }
            }
            // name ends at '{', ':', '<', whitespace, or ';'
            var nend = length(rest)
            for (i in range(length(rest))) {
                let c = character_at(rest, i) // nolint:PERF002,PERF003
                if (c == '{' || c == ':' || c == '<' || c == ' ' || c == '\t' || c == ';') {
                    nend = i
                    break
                }
            }
            qualified = slice(rest, 0, nend)
        }
    } elif (kind == "enum") {
        let kw_pos = find(line, "enum ")
        if (kw_pos >= 0) {
            var rest = strip(slice(line, kw_pos + 5))
            if (starts_with(rest, "class ")) {
                rest = strip(slice(rest, 6))
            } elif (starts_with(rest, "struct ")) {
                rest = strip(slice(rest, 7))
            }
            if (length(rest) > 0 && first_character(rest) != '{') {
                var nend = length(rest)
                for (i in range(length(rest))) {
                    let c = character_at(rest, i) // nolint:PERF002,PERF003
                    if (c == '{' || c == ':' || c == ' ' || c == '\t' || c == ';') {
                        nend = i
                        break
                    }
                }
                qualified = slice(rest, 0, nend)
            }
        }
    } elif (kind == "namespace") {
        let kw_pos = find(line, "namespace ")
        if (kw_pos >= 0) {
            let rest = strip(slice(line, kw_pos + 10))
            var nend = length(rest)
            for (i in range(length(rest))) {
                let c = character_at(rest, i) // nolint:PERF002,PERF003
                if (c == '{' || c == ' ' || c == '\t' || c == ';') {
                    nend = i
                    break
                }
            }
            qualified = slice(rest, 0, nend)
        }
    } elif (kind == "macro") {
        let pos = find(line, "#define")
        if (pos >= 0) {
            let rest = strip(slice(line, pos + 7))
            var nend = length(rest)
            for (i in range(length(rest))) {
                let c = character_at(rest, i) // nolint:PERF002,PERF003
                if (c == '(' || c == ' ' || c == '\t') {
                    nend = i
                    break
                }
            }
            qualified = slice(rest, 0, nend)
        }
    } elif (kind == "typedef") {
        // C++11 alias form: "using NAME = ...;"  (matches cpp-outline-typedefs-using rule).
        // Take the identifier between "using " and the first "=".
        if (starts_with(line, "using ")) {
            let after_using = strip(slice(line, 6))
            let eq_pos = find(after_using, "=")
            if (eq_pos > 0) {
                qualified = strip(slice(after_using, 0, eq_pos))
            }
        }
        if (empty(qualified)) {
            // function-pointer form: "typedef <ret> (*NAME)(args);"
            let star_paren = find(line, "(*")
            if (star_paren >= 0) {
                let rest = strip(slice(line, star_paren + 2))
                var nend = length(rest)
                for (i in range(length(rest))) {
                    let c = character_at(rest, i) // nolint:PERF002,PERF003
                    if (c == ')' || c == ' ' || c == ',' || c == '\t') {
                        nend = i
                        break
                    }
                }
                qualified = slice(rest, 0, nend)
            } else {
                // simple form: take last identifier before ;
                var clean = line
                let semi = find(clean, ";")
                if (semi >= 0) {
                    clean = slice(clean, 0, semi)
                }
                clean = strip(clean)
                var ce = length(clean)
                while (ce > 0 && !is_cpp_ident_char(character_at(clean, ce - 1))) { // nolint:PERF002,PERF003
                    ce--
                }
                var cs = ce
                while (cs > 0 && is_cpp_ident_char(character_at(clean, cs - 1))) { // nolint:PERF002,PERF003
                    cs--
                }
                qualified = slice(clean, cs, ce)
            }
        }
    }
    // unqualified: split off the last ::-segment
    var unq = qualified
    let last_sep = rfind(qualified, "::")
    if (last_sep >= 0) {
        unq = slice(qualified, last_sep + 2)
    }
    return unq => qualified
}

// Convenience accessor — returns just the unqualified name.
def cpp_extract_name(text : string; kind : string) : string {
    let pair = cpp_extract_name_pair(text, kind)
    return pair._0
}

// Walk `search_dirs` looking for sub-folders that contain a `.git` file or
// directory — submodules, daspkg-installed packages, FetchContent destinations,
// ad-hoc clones. Returns `--globs` exclusion patterns for each, except those
// listed in `overrides`. Compares overrides as repo-relative generic paths.
def cpp_collect_git_excludes(search_dirs : array<string>; overrides : array<string>) : array<string> {
    var excl : array<string>
    var seen : table<string; bool>
    for (sd in search_dirs) {
        let sd_base = base_name(sd)  // e.g. "modules" from "/abs/.../modules"
        dir_rec(sd) $(filename, is_dir) {
            if (base_name(filename) != ".git") {
                return
            }
            let inner_repo = dir_name(filename)  // ".daspkg_cache/index"
            let rel = empty(inner_repo) ? sd_base : "{sd_base}/{to_generic_path(inner_repo)}"
            if (key_exists(seen, rel)) {
                return
            }
            seen[rel] = true
            for (ov in overrides) {
                if (ov == rel) {
                    return
                }
            }
            excl |> push("!**/{rel}/**")
        }
    }
    return <- excl
}

// Append the standard `--globs` arguments (always-excludes + auto-`.git`-folder
// excludes for `search_dirs` + `*.cpp/*.h/*.hpp` extension lock + caller's
// optional `extra_glob_filter`) to `args`. Used by both `cpp_run_scan` (via
// rules-yaml) and `cpp_grep_usage` (via `sg run -p`).
def cpp_push_glob_args(var args : array<string>; search_dirs : array<string>; extra_glob_filter : string) {
    let git_excludes <- cpp_collect_git_excludes(search_dirs, CPP_SEARCH_INCLUDE_OVERRIDES)
    args |> reserve(length(args)
        + 2 * length(CPP_SEARCH_ALWAYS_EXCLUDE)
        + 2 * length(git_excludes)
        + 2 * length(CPP_SEARCH_INCLUDE_GLOBS)
        + 2)
    for (g in CPP_SEARCH_ALWAYS_EXCLUDE) {
        args |> push("--globs")
        args |> push(g)
    }
    for (g in git_excludes) {
        args |> push("--globs")
        args |> push(g)
    }
    for (g in CPP_SEARCH_INCLUDE_GLOBS) {
        args |> push("--globs")
        args |> push(g)
    }
    if (!empty(extra_glob_filter)) {
        args |> push("--globs")
        args |> push(extra_glob_filter)
    }
}

// Run `sg scan` with cpp rules. Returns array of CppMatch.
// On error, fills err_message and returns empty array.
def cpp_run_scan(search_dirs : array<string>; glob_filter : string; var err_message : string&) : array<CppMatch> {
    var entries : array<CppMatch>
    let sg = get_ast_grep_cmd()
    if (empty(sg)) {
        err_message := "ast-grep not available"
        return <- entries
    }
    let das_root = get_das_root()
    let rules_path = path_join(das_root, CPP_RULES_FILE)
    var dirs_to_scan : array<string>
    if (empty(search_dirs)) {
        dirs_to_scan <- cpp_default_search_dirs()
    } else {
        dirs_to_scan |> reserve(length(search_dirs))
        for (d in search_dirs) {
            dirs_to_scan |> push(d)
        }
    }
    var args <- [sg, "scan", "-r", rules_path, "--json"]
    cpp_push_glob_args(args, dirs_to_scan, glob_filter)
    args |> reserve(length(args) + length(dirs_to_scan))
    for (d in dirs_to_scan) {
        args |> push(d)
    }
    var raw : string
    let rc = run_and_capture(args, raw, 60.0)
    if (rc != 0 && empty(raw)) {
        err_message := "sg scan failed (exit {rc})"
        return <- entries
    }
    if (empty(raw) || raw == "[]") {
        return <- entries
    }
    var parse_err : string
    var parsed = read_json(raw, parse_err)
    if (parsed == null) {
        let truncated = length(raw) > 500 ? slice(raw, 0, 500) : raw
        err_message := "Failed to parse sg scan output: {parse_err}\nRaw: {truncated}"
        return <- entries
    }
    if (!(parsed.value is _array)) {
        unsafe { delete parsed; }
        err_message := "Unexpected sg scan output format"
        return <- entries
    }
    unsafe {
        let matches & = parsed.value as _array
        entries |> reserve(length(matches))
        for (m in matches) {
            let rule_id = get_json_string(m, "ruleId")
            let kind = cpp_rule_id_to_kind(rule_id)
            let file_path = get_json_string(m, "file")
            let text = get_json_string(m, "text")
            let rng = (m.value as _object)?["range"] ?? null
            let start = rng != null ? ((rng.value as _object)?["start"] ?? null) : null
            let line_num = start != null ? get_json_int(start, "line") + 1 : 0
            let pair = cpp_extract_name_pair(text, kind)
            entries |> emplace(CppMatch(
                file = make_relative_path(file_path, das_root),
                line = line_num,
                kind = kind,
                name = pair._0,
                qualified = pair._1,
                text = cpp_first_line(text)
            ))
        }
    }
    unsafe { delete parsed; }
    return <- entries
}

// Resolve search directories from a single `directory` arg.
// Empty -> default cpp dirs. Otherwise a single absolute or repo-relative path.
def cpp_resolve_search_dirs(directory : string) : array<string> {
    var dirs : array<string>
    if (empty(directory)) {
        return <- cpp_default_search_dirs()
    }
    dirs |> push(resolve_path(directory))
    return <- dirs
}

// ── Lazy index for redirect + cpp_goto_definition ─────────────────────
//
// Built on first call (~2s scan over `CPP_SEARCH_DIRS`). Subsequent calls
// recompute a cheap signature (~70-200ms: git rev-parse HEAD + git status
// filtered to .cpp/.h/.hpp under search scope + per-file mtimes + config
// file mtime). On signature mismatch, the index is rebuilt; otherwise the
// cached state is reused.
//
// Maps both unqualified (`addFunction`) and qualified (`Module::addFunction`)
// names to CppMatch indices.  Powers find_symbol's `with_cpp_source`
// (first match) and cpp_goto_definition (all matches, ranked by caller).

var cpp_index_signature : string  // empty == not built yet
var cpp_index : table<string; array<int>>
var cpp_index_entries : array<CppMatch>

// True if `path` (extracted from a `git status --porcelain` line) refers to a
// .cpp/.h/.hpp file inside one of CPP_SEARCH_DIRS. Anything else (.das edits,
// .cmake edits, tutorials/foo.cpp, etc.) is irrelevant to the index.
def private is_indexed_path(path : string) : bool {
    if (!ends_with_any(path, CPP_SEARCH_INCLUDE_GLOBS)) {
        return false
    }
    for (dir in CPP_SEARCH_DIRS) {
        if (path == dir || starts_with(path, "{dir}/")) {
            return true
        }
    }
    return false
}

// `*.cpp` / `*.h` / `*.hpp` style patterns are simple suffixes (no embedded
// glob meta-chars) — a substring compare is sufficient and avoids pulling
// in a full glob engine just for extension checks.
def private ends_with_any(path : string; suffix_globs : array<string>) : bool {
    for (g in suffix_globs) {
        // strip leading "*" if present (e.g. "*.cpp" -> ".cpp")
        let suffix = starts_with(g, "*") ? slice(g, 1) : g
        if (ends_with(path, suffix)) {
            return true
        }
    }
    return false
}

// Strip `git status --porcelain` 3-char status prefix. Handles the "rename"
// form `R  old -> new` by returning the new name.
def private extract_status_path(line : string) : string {
    if (length(line) <= 3) {
        return ""
    }
    var path = slice(line, 3)
    let arrow = find(path, " -> ")
    if (arrow >= 0) {
        path = slice(path, arrow + 4)
    }
    return path
}

// stat() wrapper that returns 0 on missing/error so signature compute doesn't
// crash on a deleted file.
def private stat_mtime_or_zero(path : string) : int64 {
    let st = stat(path)
    return st.is_valid ? int64(st.mtime) : 0l
}

// Compute the staleness signature. Empty result on git failure → caller
// treats as "no-git" or as transient and retries.
def cpp_compute_index_signature() : string {
    var head_buf : string
    if (run_and_capture(["git", "rev-parse", "HEAD"], head_buf, 5.0) != 0
        || empty(head_buf)) {
        return "no-git"
    }
    var status_buf : string
    if (run_and_capture(["git", "status", "--porcelain", "--untracked-files=normal"],
                       status_buf, 5.0) != 0) {
        return "no-git"
    }
    let lines <- split(status_buf, "\n")
    let combined = build_string() $(var w) {
        w |> write(head_buf)
        for (line in lines) {
            let path = extract_status_path(line)
            if (!empty(path) && is_indexed_path(path)) {
                w |> write("{line}:{stat_mtime_or_zero(path)}\n")
            }
        }
        // include config file mtime so config edits trigger rebuild
        let cfg_path = path_join(get_das_root(), CPP_SEARCH_CONFIG_PATH)
        w |> write("cfg:{stat_mtime_or_zero(cfg_path)}\n")
    }
    return "{hash(combined)}"
}

def ensure_cpp_index() {
    let sig = cpp_compute_index_signature()
    if (!empty(sig) && sig == cpp_index_signature) {
        return  // cache hit
    }
    // rebuild from scratch
    cpp_index |> clear()
    cpp_index_entries |> clear()
    var err : string
    var entries <- cpp_run_scan(cpp_default_search_dirs(), "", err)
    if (!empty(err)) {
        cpp_index_signature := ""  // reset; next call retries the scan
        return
    }
    for (i in range(length(entries))) {
        let e & = unsafe(entries[i])
        if (empty(e.name)) {
            continue
        }
        cpp_index_entries |> push(e)
        let new_idx = length(cpp_index_entries) - 1
        cpp_index[e.name] |> push(new_idx) // nolint:PERF006
        if (!empty(e.qualified) && e.qualified != e.name) {
            cpp_index[e.qualified] |> push(new_idx) // nolint:PERF006
        }
    }
    cpp_index_signature := sig  // commit on success
}

// Look up a C++ name (qualified or unqualified). Returns true if found and
// fills `match` with the FIRST stored match. Tries the name as given first,
// then strips ::-qualifiers and tries the unqualified suffix.
def cpp_lookup_by_name(cppName : string; var match : CppMatch&) : bool {
    if (empty(cppName)) {
        return false
    }
    ensure_cpp_index()
    if (key_exists(cpp_index, cppName)) {
        let idx = cpp_index[cppName][0]
        match = cpp_index_entries[idx]
        return true
    }
    let last_sep = rfind(cppName, "::")
    if (last_sep >= 0) {
        let unq = slice(cppName, last_sep + 2)
        if (key_exists(cpp_index, unq)) {
            let idx = cpp_index[unq][0]
            match = cpp_index_entries[idx]
            return true
        }
    }
    return false
}

// Return ALL CppMatch entries indexed under `name` (or its unqualified form).
// Empty array if absent.  Used by cpp_goto_definition.  The returned array
// holds copies (not references) so callers can sort freely.
def cpp_lookup_all_by_name(name : string) : array<CppMatch> {
    var out : array<CppMatch>
    if (empty(name)) {
        return <- out
    }
    ensure_cpp_index()
    var indices : array<int>
    if (key_exists(cpp_index, name)) {
        indices := cpp_index[name]
    } else {
        let last_sep = rfind(name, "::")
        if (last_sep >= 0) {
            let unq = slice(name, last_sep + 2)
            if (key_exists(cpp_index, unq)) {
                indices := cpp_index[unq]
            }
        }
    }
    out |> reserve(length(indices))
    for (idx in indices) {
        out |> push(cpp_index_entries[idx])
    }
    return <- out
}
