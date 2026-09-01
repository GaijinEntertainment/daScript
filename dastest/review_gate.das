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

// `script_quotes` widens what opens a string literal from `"` alone to all three script
// quote styles - the one axis the two comment strippers differ on. A quoted string ends at
// the line it started on; only a template literal spans lines, so only a backtick carries.
// Every newline survives, so a line count taken on the result matches the source.
def private strip_to_line_comment(text : string; script_quotes : bool) : string {
    return build_string() $(var b : StringBuilderWriter) {
        peek_data(text) $(d) {
            let n = length(d)
            var quote = 0
            var in_comment = false
            var i = 0
            while (i < n) {
                let c = int(d[i])
                if (c == '\n') {
                    in_comment = false
                    if (quote != '`') {
                        quote = 0
                    }
                    b |> write("\n")
                } elif (in_comment) {
                    // dropped
                } elif (quote != 0) {
                    if (c == '\\' && i + 1 < n) {
                        b |> write(slice(d, i, i + 2))
                        i++
                    } else {
                        if (c == quote) {
                            quote = 0
                        }
                        b |> write(slice(d, i, i + 1))
                    }
                } elif (c == '"' || (script_quotes && (c == '\'' || c == '`'))) {
                    quote = c
                    b |> write(slice(d, i, i + 1))
                } elif (c == '/' && i + 1 < n && int(d[i + 1]) == '/') {
                    in_comment = true
                } else {
                    b |> write(slice(d, i, i + 1))
                }
                i++
            }
        }
    }
}

//! `text` with every // comment cut to end-of-line (string-literal-aware, so a "//" inside
//! a quoted URL survives). Use before scanning sources or descriptors: a name mentioned
//! only in a comment must not count. Only `"` opens a literal - for script text, where a
//! single quote does too, `script_urls` is the scanner that reads it correctly.
def strip_line_comments(text : string) : string {
    return strip_to_line_comment(text, false)
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

def private is_shell_operator(tok : string) : bool {
    return (tok == "||" || tok == "&&" || tok == ";" || tok == "|" || tok |> starts_with(">") ||
        tok |> starts_with("2>") || tok |> starts_with("&"))
}

def private unquoted(tok : string) : string {
    var out = tok
    if (out |> starts_with("\"") || out |> starts_with("'")) {
        out = slice(out, 1)
    }
    if (out |> ends_with("\"") || out |> ends_with("'")) {
        out = slice(out, 0, length(out) - 1)
    }
    return out
}

//! Sources of every `cp` in shell text whose destination starts with `dest_prefix` - one
//! entry per source token, quotes removed, `-r`-style flags dropped, the argument list cut
//! at the first shell operator (`||`, `2>`, ...). Line-based: a `cp` inside a YAML `run:`
//! block is one line of shell.
def shell_cp_sources(text : string; dest_prefix : string) : array<string> {
    var out : array<string>
    for (line in split(text, "\n")) {
        var inscope toks <- [for (w in split_by_chars(line, " \t\r")); w; where !empty(w)]
        let at = find_index(toks, "cp")
        continue if (at < 0)
        var args : array<string>
        args |> reserve(length(toks))
        for (i in range(at + 1, length(toks))) {
            break if (is_shell_operator(toks[i]))
            continue if (toks[i] |> starts_with("-"))
            args |> push(unquoted(toks[i]))
        }
        continue if (length(args) < 2 || !(args[length(args) - 1] |> starts_with(dest_prefix)))
        out |> reserve(length(out) + length(args) - 1)
        for (i in range(length(args) - 1)) {
            out |> push(args[i])
        }
    }
    return <- out
}

//! Sources of every `-E copy` / `-E copy_directory` in CMake text whose destination
//! contains `dest_marker` - one entry per source, `${...}` references verbatim. The
//! argument list of one copy ends at the next CMake keyword or the closing paren.
def cmake_copy_sources(text : string; dest_marker : string) : array<string> {
    var out : array<string>
    var inscope words <- cmake_words(text)
    let n = length(words)
    var i = 0
    while (i + 1 < n) {
        if (words[i] != "-E" || (words[i + 1] != "copy" && words[i + 1] != "copy_directory")) {
            i++
            continue
        }
        var j = i + 2
        var args : array<string>
        args |> reserve(8)
        while (j < n && words[j] != ")" && words[j] != "(" && !is_cmake_keyword(words[j])) {
            args |> push(words[j])
            j++
        }
        if (length(args) >= 2 && find(args[length(args) - 1], dest_marker) >= 0) {
            out |> reserve(length(out) + length(args) - 1)
            for (k in range(length(args) - 1)) {
                out |> push(args[k])
            }
        }
        i = j
    }
    return <- out
}

//! Home-directory gate for a published record: every line of the file at `path` that
//! `tilde_home` would change - one naming `/Users/<u>/`, `/home/<u>/` or a drive-letter
//! `\\Users\\<u>\\` - is a finding. A receipt that reaches a reader names no user.
def gate_home_paths(path : string) {
    let text = fread(path)
    if (empty(text) && stat(path).size > 0ul) {
        gate_finding(path, "unreadable - the gate cannot pass a file it could not read")
        return
    }
    var line = 0
    for (raw in split(text, "\n")) {
        line++
        // a JSON record doubles its backslashes, so a drive-letter home reads `C:\\\\Users\\\\` there
        let unescaped = replace(raw, "\\\\", "\\")
        if (tilde_home(unescaped) != unescaped) {
            gate_finding(path, line, "names a home directory - a published record spells it ~ (tilde_home in daslib/fio)")
        }
    }
}

//! Host of an absolute or protocol-relative URL, lowercased - "" when the URL is relative
//! or when what follows the scheme is not host-shaped. Scheme and host are matched
//! case-insensitively, as a browser reads them; a bracketed IPv6 literal comes back with
//! its brackets. Userinfo and port are dropped: an allow-list names hosts.
def url_host(url : string) : string {
    var rest = ""
    peek_data(url) $(u) {
        if (byte_run_at_ci(u, 0, "https://")) {
            rest = slice(u, 8, length(u))
        } elif (byte_run_at_ci(u, 0, "http://")) {
            rest = slice(u, 7, length(u))
        } elif (byte_run_at(u, 0, "//")) {
            rest = slice(u, 2, length(u))
        }
    }
    return "" if (empty(rest))
    var authority = rest
    peek_data(rest) $(d) {
        for (i in range(length(d))) {
            let c = int(d[i])
            if (c == '/' || c == '?' || c == '#') {
                authority = slice(d, 0, i)
                break
            }
        }
    }
    let at = authority |> find("@")
    var host = at >= 0 ? slice(authority, at + 1) : authority
    if (host |> starts_with("[")) {
        // a bracketed IPv6 literal carries colons of its own - the port is the one after `]`
        let close = host |> find("]")
        return "" if (close < 0)
        host = slice(host, 0, close + 1)
    } else {
        let colon = host |> find(":")
        if (colon >= 0) {
            host = slice(host, 0, colon)
        }
    }
    return is_host_shaped(host) ? to_lower(host) : ""
}

def private is_host_shaped(host : string) : bool {
    var ok = !empty(host)
    let bracketed = host |> starts_with("[")
    peek_data(host) $(d) {
        for (i in range(length(d))) {
            let c = int(d[i])
            continue if (bracketed && (c == '[' || c == ']' || c == ':'))
            if (!is_alnum(c) && c != '-' && c != '.' && c != '_') {
                ok = false
                break
            }
        }
    }
    return ok
}

// a URL scheme and an HTML tag name are both case-insensitive to a browser
def private byte_run_at_ci(d; at : int; lit : string) : bool {
    var ok = true
    peek_data(lit) $(l) {
        let m = length(l)
        if (at + m > length(d)) {
            ok = false
        } else {
            for (i in range(m)) {
                if (to_lower_byte(int(d[at + i])) != to_lower_byte(int(l[i]))) {
                    ok = false
                    break
                }
            }
        }
    }
    return ok
}

def private byte_run_at(d; at : int; lit : string) : bool {
    var ok = true
    peek_data(lit) $(l) {
        let m = length(l)
        if (at + m > length(d)) {
            ok = false
        } else {
            for (i in range(m)) {
                if (int(d[at + i]) != int(l[i])) {
                    ok = false
                    break
                }
            }
        }
    }
    return ok
}

def private is_resource_tag(name : string) : bool {
    return (name == "script" || name == "link" || name == "iframe" || name == "img" ||
        name == "source" || name == "video" || name == "audio" || name == "embed" ||
        name == "object")
}

def private is_url_break_byte(b : int) : bool {
    return (is_white_space(b) || b == '"' || b == '\'' || b == '`' || b == '<' || b == '>' ||
        b == ')' || b == ',' || b == ';')
}

//! Absolute and protocol-relative URLs written as string literals in script text, `//`
//! comments cut first so a commented-out address does not count. A protocol-relative
//! literal is recognized by the quote that opens it.
def script_urls(text : string) : array<string> {
    var out : array<string>
    let src = strip_to_line_comment(text, true)
    peek_data(src) $(d) {
        let n = length(d)
        var i = 0
        while (i < n) {
            let c = int(d[i])
            var start = -1
            if ((c == '"' || c == '\'' || c == '`') && byte_run_at(d, i + 1, "//")) {
                start = i + 1
            } elif (byte_run_at_ci(d, i, "http://") || byte_run_at_ci(d, i, "https://")) {
                start = i
            }
            if (start < 0) {
                i++
                continue
            }
            var e = start
            while (e < n && !is_url_break_byte(int(d[e]))) {
                e++
            }
            let url = slice(d, start, e)
            if (!empty(url_host(url))) {
                out |> push(url)
            }
            i = e + 1
        }
    }
    return <- out
}

def private strip_block_comments(text : string) : string {
    return build_string() $(var b : StringBuilderWriter) {
        peek_data(text) $(d) {
            let n = length(d)
            var i = 0
            while (i < n) {
                let open = d |> find("/*", i)
                if (open < 0) {
                    b |> write(slice(d, i, n))
                    break
                }
                b |> write(slice(d, i, open))
                let close = d |> find("*/", open + 2)
                break if (close < 0)
                i = close + 2
            }
        }
    }
}

//! Absolute and protocol-relative URLs a stylesheet fetches - the address of an `@import`
//! and of every `url(...)`, quoted or bare, with `/* */` comments cut first so a reference
//! written in prose does not count. CSS has no `//` comment, so a `//` opened by `(` or a
//! quote is a protocol-relative address.
def css_urls(text : string) : array<string> {
    var out : array<string>
    peek_data(strip_block_comments(text)) $(d) {
        let n = length(d)
        var i = 0
        while (i < n) {
            let c = int(d[i])
            var start = -1
            if ((c == '(' || c == '"' || c == '\'') && byte_run_at(d, i + 1, "//")) {
                start = i + 1
            } elif (byte_run_at_ci(d, i, "http://") || byte_run_at_ci(d, i, "https://")) {
                start = i
            }
            if (start < 0) {
                i++
                continue
            }
            var e = start
            while (e < n && !is_url_break_byte(int(d[e]))) {
                e++
            }
            let url = slice(d, start, e)
            if (!empty(url_host(url))) {
                out |> push(url)
            }
            i = e + 1
        }
    }
    return <- out
}

// HTML closes script data at `</script` followed by whitespace, `/` or `>` - `</scriptx`
// is ordinary text, so it must not end the scanned body
def private is_script_end_tag_at(d; at : int) : bool {
    return false if (!byte_run_at_ci(d, at, "</script"))
    let after = at + 8
    return true if (after >= length(d))
    let c = int(d[after])
    return is_white_space(c) || c == '/' || c == '>'
}

// every attribute value between `from` and `to` - HTML permits an unquoted one, so the
// value runs to whitespace when no quote opens it
def private push_tag_urls(d; from : int; to : int; var out : array<string>) {
    var p = from
    while (p < to) {
        if (int(d[p]) != '=') {
            p++
            continue
        }
        var v = p + 1
        while (v < to && is_white_space(int(d[v]))) {
            v++
        }
        break if (v >= to)
        let opener = int(d[v])
        var vs = v
        var ve = v
        if (opener == '"' || opener == '\'') {
            vs = v + 1
            ve = vs
            while (ve < to && int(d[ve]) != opener) {
                ve++
            }
            p = ve + 1
        } else {
            while (ve < to && !is_white_space(int(d[ve]))) {
                ve++
            }
            p = ve
        }
        push_attribute_urls(slice(d, vs, ve), out)
    }
}

// one attribute can carry several addresses - `srcset` writes them comma-separated, each
// followed by a descriptor (`1x`, `640w`) that is not a URL and drops out on its own
def private push_attribute_urls(value : string; var out : array<string>) {
    for (token in split_by_chars(value, " \t\r\n,")) {
        continue if (empty(token))
        if (!empty(url_host(token))) {
            out |> push(token)
        }
    }
}

//! Absolute and protocol-relative URLs a page's markup makes a browser fetch without a
//! click: every attribute value of a resource tag (script, link, iframe, img, source,
//! video, audio, embed, object) - quoted or bare, and split so a multi-address `srcset`
//! yields all of them - plus the URL literals of every inline script body. An
//! `<a>` or `<meta>` address is left out - a link is a click, not a load. Markup-level on
//! purpose: a resource tag inside an HTML comment is still reported.
def html_resource_urls(text : string) : array<string> {
    var out : array<string>
    peek_data(text) $(d) {
        let n = length(d)
        var i = 0
        while (i < n) {
            if (int(d[i]) != '<') {
                i++
                continue
            }
            var j = i + 1
            while (j < n && is_alpha(int(d[j]))) {
                j++
            }
            let tag = to_lower(slice(d, i + 1, j))
            var k = j
            var quote = 0
            while (k < n) {
                let c = int(d[k])
                if (quote != 0) {
                    if (c == quote) {
                        quote = 0
                    }
                } elif (c == '"' || c == '\'') {
                    quote = c
                } elif (c == '>') {
                    break
                }
                k++
            }
            if (is_resource_tag(tag)) {
                push_tag_urls(d, j, k, out)
            }
            if (tag == "script" && k < n) {
                // no end tag: a browser runs the script to end of file, so the scan does too
                var close = k + 1
                while (close < n && !is_script_end_tag_at(d, close)) {
                    close++
                }
                for (url in script_urls(slice(d, k + 1, close))) {
                    out |> push(url)
                }
            }
            i = k + 1
        }
    }
    return <- out
}

//! True for a repository rule or architecture document - a `.md` the document system owns,
//! which no site serves (the SDK bundle gate bans every one of them from an install).
def private is_rule_document(name : string) : bool {
    return (name |> starts_with("REVIEW") || name |> starts_with("ARCHITECTURE") ||
        name == "LAWS.md" || name == "README.md")
}

def private is_markup_file(name : string) : bool {
    return (name |> ends_with(".html") || name |> ends_with(".rst") || name |> ends_with(".md"))
}

// an asset manifest lists what the playground fetches, so every entry in it is a load;
// no other JSON carries that guarantee - a `link` field is a click - so no other JSON is read
def private is_asset_manifest(name : string) : bool {
    return name |> ends_with(".assets.json")
}

// the walker takes basenames, so the rule-document test reads a name, never a path
def private is_scanned_file(name : string) : bool {
    return false if (is_rule_document(name))
    return (is_markup_file(name) || name |> ends_with(".js") || name |> ends_with(".css") ||
        is_asset_manifest(name))
}

def private walk_web_files(folder : string; skip_dirs : table<string>; var out : array<string>) {
    dir(folder) $(name) {
        return if (name == "." || name == "..")
        let full = "{folder}/{name}"
        if (stat(full).is_dir) {
            if (!key_exists(skip_dirs, name)) {
                walk_web_files(full, skip_dirs, out)
            }
        } elif (is_scanned_file(name)) {
            out |> push(full)
        }
    }
}

def private resource_urls_of(path : string; text : string) : array<string> {
    if (is_markup_file(path)) {
        return <- html_resource_urls(text)
    }
    return <- path |> ends_with(".css") ? css_urls(text) : script_urls(text)
}

//! Cookie-and-host gate for a served web tree: every markup (`.html`, `.rst`, `.md`),
//! `.js`, `.css` and `.assets.json` file under `roots`, never descending into a directory
//! named in `skip_dirs`, is read for `document.cookie` and for a resource host
//! `allowed_hosts` does not name. Markup contributes its resource tags and inline script
//! bodies, script text and an asset manifest their URL literals, a stylesheet its
//! `@import` and `url(...)` addresses.
def gate_web_third_party(roots : array<string>; skip_dirs : table<string>; allowed_hosts : table<string>) {
    var files : array<string>
    for (root in roots) {
        walk_web_files(root, skip_dirs, files)
    }
    for (path in files) {
        let text = fread(path)
        if (empty(text) && stat(path).size > 0ul) {
            // fread returns "" for both an empty file and a failed read - a gate that cannot
            // read a file must say so, never pass it
            gate_finding(path, "unreadable - the gate cannot pass a file it could not read")
            continue
        }
        let cookie_line = find_line(text, "document.cookie")
        if (cookie_line > 0) {
            gate_finding(path, cookie_line,
                "uses document.cookie - this site sets none; keep per-visitor state in localStorage instead")
        }
        var inscope urls <- resource_urls_of(path, text)
        for (url in urls) {
            let host = url_host(url)
            continue if (key_exists(allowed_hosts, host))
            gate_finding(path, find_line(text, url),
                "reaches {host}, which the gate's allowed-host list does not name - add the host only once its data handling is reviewed")
        }
    }
}
