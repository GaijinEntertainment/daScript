options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/regex
require daslib/regex_boost

//! Support library for `REVIEW.das` gates — the executable half of a folder's `REVIEW.md`
//! checklist (contract: `REVIEW_COMMON.md` at the repo root).

var private g_findings : array<string>

var private KEBAB <- %regex~^[a-z0-9]+(-[a-z0-9]+)*$%%

//! Record one finding against a file.
def gate_finding(path : string; msg : string) {
    g_findings |> push("{path}: {msg}")
}

//! Record one finding against a specific line (1-based; 0 = no line known).
def gate_finding(path : string; line : int; msg : string) {
    if (line > 0) {
        g_findings |> push("{path}:{line}: {msg}")
    } else {
        g_findings |> push("{path}: {msg}")
    }
}

//! All findings recorded so far, in report order.
def gate_findings : array<string> {
    return clone_to_move(g_findings)
}

//! Drop accumulated findings (for tests).
def gate_reset {
    g_findings |> clear()
}

//! Print the report and return the gate's exit code: 0 clean, 1 with findings.
def gate_verdict(name : string) : int {
    for (f in g_findings) {
        print("{f}\n")
    }
    if (empty(g_findings)) {
        print("REVIEW.das {name}: OK\n")
        return 0
    }
    print("REVIEW.das {name}: {length(g_findings)} finding(s)\n")
    return 1
}

//! True for `like-this` names: lowercase [a-z0-9] runs joined by single hyphens.
def is_kebab_case(name : string) : bool {
    return regex_match(KEBAB, name) >= 0
}

//! 1-based line number of the first occurrence of `needle` in `text`; 0 when absent.
def find_line(text : string; needle : string) : int {
    var line = 0
    peek_data(text) $(d) {
        let idx = find(d, needle)
        return if (idx < 0)
        line = 1
        for (i in range(idx)) {
            if (int(d[i]) == '\n') {
                line++
            }
        }
    }
    return line
}

//! Module paths from a file's line-level `require` statements (`require a/b public` → "a/b").
//! Line-based on purpose: a require inside a block comment is still reported.
def das_requires(path : string) : array<string> {
    var out : array<string>
    let text = fread(path)
    for (raw in split(text, "\n")) {
        var line = strip(raw)
        continue if (!starts_with(line, "require "))
        line = strip(trim_prefix(line, "require "))
        let cut = find(line, "//")
        if (cut >= 0) {
            line = strip(slice(line, 0, cut))
        }
        continue if (empty(line))
        let sp = find(line, ' ')
        out |> push(sp >= 0 ? slice(line, 0, sp) : line)
    }
    return <- out
}

def private is_ident_byte(b : int) : bool {
    return is_alnum(b) || b == '_'
}

def private to_lower_byte(b : int) : int {
    return b >= 'A' && b <= 'Z' ? b + 32 : b
}

def private lowered_bytes(s : string) : array<int> {
    var out : array<int>
    peek_data(s) $(d) {
        out |> reserve(length(d))
        for (i in range(length(d))) {
            out |> push(to_lower_byte(int(d[i])))
        }
    }
    return <- out
}

// CMake command names are case-insensitive, so `Add_Test(` opens a block like `add_test(`.
def private matches_lowered(d; at : int; lowered : array<int>) : bool {
    let m = length(lowered)
    return false if (at + m > length(d))
    for (i in range(m)) {
        return false if (to_lower_byte(int(d[at + i])) != lowered[i])
    }
    return true
}

//! `text` with every // comment cut to end-of-line (string-literal-aware, so a "//" inside
//! a quoted URL survives). Use before scanning sources or descriptors: a name mentioned
//! only in a comment must not count.
def strip_line_comments(text : string) : string {
    return build_string() $(var b : StringBuilderWriter) {
        for (line in split(text, "\n")) {
            var cut = -1
            peek_data(line) $(d) {
                var in_str = false
                var i = 0
                while (i < length(d)) {
                    let c = int(d[i])
                    if (in_str) {
                        if (c == '\\') {
                            i++
                        } elif (c == '"') {
                            in_str = false
                        }
                    } elif (c == '"') {
                        in_str = true
                    } elif (c == '/' && i + 1 < length(d) && int(d[i + 1]) == '/') {
                        cut = i
                        break
                    }
                    i++
                }
            }
            b |> write(cut >= 0 ? slice(line, 0, cut) : line)
            b |> write("\n")
        }
    }
}

def private quoted_ident_tokens(text : string) : array<string> {
    var out : array<string>
    var inside = false
    for (seg in split(text, "\"")) {
        if (inside && !empty(seg)) {
            var all_ident = true
            peek_data(seg) $(d) {
                for (i in range(length(d))) {
                    if (!is_ident_byte(int(d[i]))) {
                        all_ident = false
                        break
                    }
                }
            }
            if (all_ident) {
                out |> push(seg)
            }
        }
        inside = !inside
    }
    return <- out
}

//! Descriptor census for a module whose CMake derives its resolver rows from `.das_module`
//! (ADD_MODULE_DAS_FROM_DESCRIPTOR), both directions: every `.das` in `census_dirs` is
//! named by a quoted token in the descriptor, and every identifier-shaped quoted token
//! outside `non_module_tokens` (e.g. the require category, a DLL registration class)
//! names a file in some census dir. Neither direction has a build-time signal - a gate
//! built on this is the only enforcer.
def gate_descriptor_census(descriptor_path : string; census_dirs : array<string>; non_module_tokens : table<string>) {
    if (!fexist(descriptor_path)) {
        gate_finding(descriptor_path, "missing - the descriptor is the module's one registration surface")
        return
    }
    let descriptor = strip_line_comments(fread(descriptor_path))
    for (folder in census_dirs) {
        dir(folder) $(name) {
            if (name |> ends_with(".das")) {
                let base = slice(name, 0, length(name) - 4)
                if (find(descriptor, "\"{base}\"") < 0) {
                    gate_finding("{folder}/{name}",
                        "absent from {descriptor_path} - the descriptor is the one registration surface (the resolver rows derive from it), so an unlisted file resolves in neither the static binary nor the DLL/daspkg flow")
                }
            }
        }
    }
    let dirs_text = join(census_dirs, ", ")
    for (tok in quoted_ident_tokens(descriptor)) {
        continue if (key_exists(non_module_tokens, tok))
        if (!any_dir_holds(census_dirs, "{tok}.das")) {
            gate_finding(descriptor_path,
                "names {tok}, but no census dir ({dirs_text}) holds {tok}.das - a stale registration is a silently dropped resolver row")
        }
    }
}

def private any_dir_holds(census_dirs : array<string>; fname : string) : bool {
    for (folder in census_dirs) {
        if (fexist("{folder}/{fname}")) {
            return true
        }
    }
    return false
}

//! Census with nothing ignored - for descriptors whose every quoted token is a module name.
def gate_descriptor_census(descriptor_path : string; census_dirs : array<string>) {
    let no_ignored_tokens : table<string>
    gate_descriptor_census(descriptor_path, census_dirs, no_ignored_tokens)
}

//! Every `command( ... )` invocation in CMake text, paren-balanced, with `#` comments and
//! double-quoted strings respected. Each result is the full block text, command name included.
def cmake_command_blocks(text : string; command : string) : array<string> {
    var out : array<string>
    var inscope needle <- lowered_bytes(command)
    peek_data(text) $(d) {
        let n = length(d)
        let m = length(needle)
        var i = 0
        while (i < n) {
            if (int(d[i]) == '#') {   // a commented-out invocation must not open a block
                while (i < n && int(d[i]) != '\n') {
                    i++
                }
                continue
            }
            if (!matches_lowered(d, i, needle)) {
                i++
                continue
            }
            if (i > 0 && is_ident_byte(int(d[i - 1]))) {
                i++
                continue
            }
            var j = i + m
            while (j < n && is_tab_or_space(int(d[j]))) {
                j++
            }
            if (j >= n || int(d[j]) != '(') {
                i++
                continue
            }
            var depth = 0
            var k = j
            var in_string = false
            var in_comment = false
            while (k < n) {
                let b = int(d[k])
                if (in_comment) {
                    in_comment = b != '\n'
                } elif (in_string) {
                    if (b == '\\') {
                        k++
                    } elif (b == '"') {
                        in_string = false
                    }
                } elif (b == '"') {
                    in_string = true
                } elif (b == '#') {
                    in_comment = true
                } elif (b == '(') {
                    depth++
                } elif (b == ')') {
                    depth--
                    break if (depth == 0)
                }
                k++
            }
            if (k < n) {
                out |> push(slice(d, i, k + 1))
                i = k + 1
            } else {
                i++
            }
        }
    }
    return <- out
}

//! Words of CMake text, `#` comments dropped, whitespace treated as a separator. A
//! double-quoted run is one word: quotes removed, backslashes kept as written. `(` and `)`
//! are words of their own, so a caller can see where one command's arguments end.
def cmake_words(text : string) : array<string> {
    var out : array<string>
    peek_data(text) $(d) {
        let n = length(d)
        var i = 0
        while (i < n) {
            let c = int(d[i])
            if (c == '#') {
                while (i < n && int(d[i]) != '\n') {
                    i++
                }
            } elif (c == '"') {
                var j = i + 1
                while (j < n && int(d[j]) != '"') {
                    if (int(d[j]) == '\\' && j + 1 < n) {
                        j++
                    }
                    j++
                }
                out |> push(slice(d, i + 1, j))
                i = j + 1
            } elif (c == '(' || c == ')') {
                out |> push(slice(d, i, i + 1))
                i++
            } elif (is_white_space(c)) {
                i++
            } else {
                var j = i
                while (j < n && !is_word_break_byte(int(d[j]))) {
                    j++
                }
                out |> push(slice(d, i, j))
                i = j
            }
        }
    }
    return <- out
}

def private is_word_break_byte(b : int) : bool {
    return is_white_space(b) || b == '(' || b == ')' || b == '#' || b == '"'
}

//! The argument words of one `command(...)` block - the command name and the enclosing
//! parens dropped.
def cmake_args(command_block : string) : array<string> {
    var inscope words <- cmake_words(command_block)
    var out : array<string>
    var depth = 0
    out |> reserve(length(words))
    for (w in words) {
        if (w == "(") {
            depth++
            continue if (depth == 1)
        } elif (w == ")") {
            break if (depth == 1)
            depth--
        }
        out |> push(w) if (depth >= 1)
    }
    return <- out
}

//! True for a word CMake reads as a keyword rather than a value: caps, digits and
//! underscores only, with at least one letter (`PROPERTIES`, `PASS_REGULAR_EXPRESSION`).
def is_cmake_keyword(word : string) : bool {
    var letters = 0
    var ok = !empty(word)
    peek_data(word) $(d) {
        for (i in range(length(d))) {
            let b = int(d[i])
            if (b >= 'A' && b <= 'Z') {
                letters++
            } elif (!is_number(b) && b != '_') {
                ok = false
                break
            }
        }
    }
    return ok && letters > 0
}

//! Every ctest label the file's `LABELS` properties name, in order, without duplicates.
//! Both spellings count: `LABELS "big;style_lint"` and the `LABELS big style_lint` an
//! unquoted list expands to. Command-agnostic on purpose: `add_test`,
//! `set_tests_properties` and a discovery macro's `PROPERTIES LABELS` all count.
def cmake_test_labels(text : string) : array<string> {
    var out : array<string>
    var seen : table<string>
    var inscope words <- cmake_words(text)
    let n = length(words)
    var i = 0
    while (i < n) {
        if (words[i] != "LABELS") {
            i++
            continue
        }
        // the value run ends at the next property keyword or at the end of the command
        var j = i + 1
        while (j < n && words[j] != ")" && words[j] != "(" && !is_cmake_keyword(words[j])) {
            for (lab in split(words[j], ";")) {
                continue if (empty(lab) || key_exists(seen, lab))
                seen |> insert(lab)
                out |> push(lab)
            }
            j++
        }
        i = j
    }
    return <- out
}

//! The word after `COMMAND` in every `add_test` block, verbatim - the thing that runs the
//! test, which may be a target name, a `$<TARGET_FILE:x>` expression, or an interpreter.
def cmake_test_commands(text : string) : array<string> {
    var out : array<string>
    for (blk in cmake_command_blocks(text, "add_test")) {
        var inscope args <- cmake_args(blk)
        for (i in range(length(args) - 1)) {
            if (args[i] == "COMMAND") {
                out |> push(args[i + 1])
                break
            }
        }
    }
    return <- out
}

//! Name of the first argument of every `command(...)` block, in order - the target a
//! `add_dependencies` / `add_custom_target` / `add_executable` block names.
def cmake_command_targets(text : string; command : string) : array<string> {
    var out : array<string>
    for (blk in cmake_command_blocks(text, command)) {
        var inscope args <- cmake_args(blk)
        if (!empty(args)) {
            out |> push(args[0])
        }
    }
    return <- out
}

//! Entries of a CMake list across its `set(NAME ...)` and `list(APPEND NAME ...)` blocks,
//! in order, `#` comments dropped. `${...}` references come back verbatim.
def cmake_list_entries(text : string; listname : string) : array<string> {
    var out : array<string>
    for (cmd in ["set", "list"]) {
        for (blk in cmake_command_blocks(text, cmd)) {
            let open = find(blk, "(")
            let body = slice(blk, open + 1, length(blk) - 1)
            var toks : array<string>
            for (raw in split(body, "\n")) {
                var line = raw
                let cut = find(line, "#")
                if (cut >= 0) {
                    line = slice(line, 0, cut)
                }
                for (t in split_by_chars(line, " \t\r()")) {
                    if (!empty(t)) {
                        toks |> push(t)
                    }
                }
            }
            continue if (length(toks) < 2)
            var start = -1
            if (toks[0] == listname) {
                start = 1
            } elif (toks[0] == "APPEND" && toks[1] == listname) {
                start = 2
            }
            continue if (start < 0)
            out |> reserve(length(out) + length(toks) - start)
            for (idx in range(start, length(toks))) {
                out |> push(toks[idx])
            }
        }
    }
    return <- out
}
