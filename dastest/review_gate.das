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

//! Every `command( ... )` invocation in CMake text, paren-balanced, with `#` comments and
//! double-quoted strings respected. Each result is the full block text, command name included.
def cmake_command_blocks(text : string; command : string) : array<string> {
    var out : array<string>
    peek_data(text) $(d) {
        let n = length(d)
        let m = length(command)
        var i = 0
        while (i < n) {
            if (int(d[i]) == '#') {   // a commented-out invocation must not open a block
                while (i < n && int(d[i]) != '\n') {
                    i++
                }
                continue
            }
            if (!starts_with(d, i, command)) {
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

//! Entries of a CMake list across its `set(NAME ...)` and `list(APPEND NAME ...)` blocks,
//! in order, `#` comments dropped. `${...}` references come back verbatim.
def cmake_list_entries(text : string; listname : string) : array<string> {
    var out : array<string>
    for (cmd in ["set", "SET", "list", "LIST"]) {
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
