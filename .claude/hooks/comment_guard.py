#!/usr/bin/env python3
"""PostToolUse hook: flags comments added by Edit/Write that the house rules reject.

.das: the formatter strips everything outside the kept set, so a freshly written
comment is already dead — this says so at the moment it happens. C/C++: no
stripper runs, but the same rule applies to NEW comments.

Pure python, no filesystem or config access: stdin JSON in, exit 0 (silent) or
exit 2 with the advisory on stderr. Anything unexpected in the input = exit 0.

The scanner is a char-level state machine over the finite comment/string grammar:
strings and bounded char literals mask comment markers; das strings span lines
and interpolate (`"{expr}"` may hold nested plain strings); das block comments
NEST; das reader macros (`%name~ ... %%`) are opaque bodies; C raw strings
R"delim(...)delim" and backslash-continued strings span lines; `scheme://` is a
URL, not a comment.
"""

import json
import re
import sys
from collections import Counter

DAS_EXTS = (".das",)
C_EXTS = (".cpp", ".cc", ".cxx", ".c", ".h", ".hpp", ".hxx", ".inc")
MAX_QUOTED = 5

_DAS_PREAMBLE = re.compile(r"^\s*(?:options|module|require)\b")
_C_PREAMBLE = re.compile(r"^\s*#\s*[a-z_]+\b")
_RAW_OPEN = re.compile(r'R"([^()\\ \t]{0,16})\(')
_READER_OPEN = re.compile(r"%[A-Za-z_][A-Za-z0-9_]*[~!]")
_SCHEME = re.compile(r"[A-Za-z][A-Za-z0-9+.\-]+$")
_WS = " \t\f\v\r\n"

CODE, BLOCK, STR, RAW, READER = range(5)


def _lines(text):
    """Split on real line breaks only — python's splitlines() also breaks on
    \\f \\v \\u2028 etc., which neither lexer treats as a newline."""
    return text.replace("\r\n", "\n").replace("\r", "\n").split("\n")


def _string_end(line, start):
    """Index just past the closing double quote, or -1 while the literal stays
    open at end of line. `start` is the index AFTER the opening quote."""
    i = start
    n = len(line)
    while i < n:
        c = line[i]
        if c == "\\":
            i += 2
            continue
        if c == '"':
            return i + 1
        i += 1
    return -1


def _das_string_scan(line, start, depth, nested):
    """Advance through a das interpolated string. `depth` is `{}` interpolation
    depth, `nested` marks a plain string open inside an interpolation. Returns
    (index past the close, depth, nested); index -1 while open at end of line."""
    i = start
    n = len(line)
    while i < n:
        c = line[i]
        if c == "\\":
            i += 2
            continue
        if nested:
            if c == '"':
                nested = False
            i += 1
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            if depth > 0:
                depth -= 1
        elif c == '"':
            if depth == 0:
                return i + 1, 0, False
            nested = True
        i += 1
    return -1, depth, nested


def _char_end(line, start):
    """Index just past a closing single quote within a short span, or -1 for a
    lone apostrophe (it's, 1'000'000) — char literals are bounded, prose is not."""
    i = start
    limit = min(len(line), start + 6)
    while i < limit:
        c = line[i]
        if c == "\\":
            i += 2
            continue
        if c == "'":
            return i + 1
        i += 1
    return -1


def _url_at(line, i):
    """True when the `//` at i is the tail of `scheme://x` — an alpha-first
    scheme of 2+ chars right before the colon, and something after the slashes."""
    if i < 2 or line[i - 1] != ":":
        return False
    if i + 2 >= len(line) or line[i + 2].isspace():
        return False
    m = _SCHEME.search(line, 0, i - 1)
    return m is not None


def scan(text, is_das):
    """Return (comments, has_code): comments as (start_line, text, is_full_line)
    with multi-line blocks joined whole; has_code = the set of line numbers that
    hold anything besides whitespace and comments."""
    comments = []
    has_code = set()
    state = CODE
    depth = 0
    str_depth = 0
    str_nested = False
    raw_term = ""
    b_line = 0
    b_text = []
    b_full = False
    for ln, line in enumerate(_lines(text), 1):
        i = 0
        n = len(line)
        code_seen = False
        if state == BLOCK:
            while i < n:
                if is_das and line.startswith("/*", i):
                    depth += 1
                    i += 2
                    continue
                if line.startswith("*/", i):
                    depth -= 1
                    i += 2
                    if depth == 0:
                        b_text.append(line[:i])
                        comments.append((b_line, "\n".join(b_text), b_full))
                        state = CODE
                        break
                    continue
                i += 1
            if state == BLOCK:
                b_text.append(line)
                continue
        elif state == STR:
            has_code.add(ln)
            if is_das:
                j, str_depth, str_nested = _das_string_scan(line, 0, str_depth, str_nested)
                if j < 0:
                    continue
            else:
                j = _string_end(line, 0)
                if j < 0:
                    if not line.endswith("\\"):
                        state = CODE
                    continue
            state = CODE
            i = j
            code_seen = True
        elif state == RAW:
            end = line.find(raw_term)
            has_code.add(ln)
            if end < 0:
                continue
            state = CODE
            i = end + len(raw_term)
            code_seen = True
        elif state == READER:
            end = line.find("%%")
            has_code.add(ln)
            if end < 0:
                continue
            state = CODE
            i = end + 2
            code_seen = True
        while i < n:
            c = line[i]
            if c in " \t":
                i += 1
                continue
            if c == "/" and line.startswith("//", i):
                if _url_at(line, i):
                    i += 2
                    code_seen = True
                    continue
                comments.append((ln, line[i:].rstrip(), line[:i].strip() == ""))
                break
            if c == "/" and line.startswith("/*", i):
                full = line[:i].strip() == ""
                depth = 1
                j = i + 2
                while j < n:
                    if is_das and line.startswith("/*", j):
                        depth += 1
                        j += 2
                        continue
                    if line.startswith("*/", j):
                        depth -= 1
                        j += 2
                        if depth == 0:
                            break
                        continue
                    j += 1
                if depth == 0:
                    comments.append((ln, line[i:j], full))
                    i = j
                    continue
                state = BLOCK
                b_line = ln
                b_text = [line[i:].rstrip()]
                b_full = full
                break
            if c == '"':
                code_seen = True
                if is_das:
                    j, str_depth, str_nested = _das_string_scan(line, i + 1, 0, False)
                    if j < 0:
                        state = STR
                        break
                else:
                    j = _string_end(line, i + 1)
                    if j < 0:
                        if line.endswith("\\"):
                            state = STR
                        break
                i = j
                continue
            if c == "'":
                j = _char_end(line, i + 1)
                code_seen = True
                i = j if j > 0 else i + 1
                continue
            if c == "%" and is_das:
                m = _READER_OPEN.match(line, i)
                if m:
                    code_seen = True
                    end = line.find("%%", m.end())
                    if end < 0:
                        state = READER
                        break
                    i = end + 2
                    continue
            if c == "R" and not is_das and line.startswith('R"', i):
                m = _RAW_OPEN.match(line, i)
                if m:
                    code_seen = True
                    raw_term = ")" + m.group(1) + '"'
                    end = line.find(raw_term, m.end())
                    if end < 0:
                        state = RAW
                        break
                    i = end + len(raw_term)
                    continue
            code_seen = True
            i += 1
        if code_seen:
            has_code.add(ln)
    if state == BLOCK:
        comments.append((b_line, "\n".join(b_text), b_full))
    return comments, has_code


def first_code_line(text, is_das, has_code):
    """First line holding real code — preamble statements (das options/module/
    require; C preprocessor directives) don't count. Mirrors the formatter's
    strip_comment_tokens: a comment is header while no code token was seen,
    even when code follows it on the same line. inf when the file has none."""
    pre = _DAS_PREAMBLE if is_das else _C_PREAMBLE
    for ln, line in enumerate(_lines(text), 1):
        if ln in has_code and not pre.match(line):
            return ln
    return float("inf")


def _kept_line_comment(ctext, is_das):
    if ctext.startswith("//!"):
        return True
    rest = ctext[2:].lstrip(_WS)
    if is_das:
        # exact mirror of is_kept_comment in daslib/das_source_formatter.das:
        # //fmt: with no space, nolint WITH the colon, @nolint — all case-sensitive
        return ctext.startswith("//fmt:") or rest.startswith("nolint:") or rest.startswith("@nolint")
    low = rest.lower()
    return low.startswith("nolint") or low.startswith("clang-format")


def _kept_block_comment(ctext, is_das):
    if is_das:
        return False
    if ctext.startswith("/*!"):
        return True
    return ctext.startswith("/**") and (len(ctext) == 3 or ctext[3] in _WS)


def violations(text, is_das, whole_file):
    """Comments in `text` outside the kept set. `whole_file` grants the leading
    header block; edit fragments carry no positional context, so it stays False."""
    text = text.lstrip("\ufeff")
    comments, has_code = scan(text, is_das)
    header_line = first_code_line(text, is_das, has_code) if whole_file else 0
    out = []
    for ln, ctext, full in comments:
        if full and ln <= header_line:
            continue
        if ctext.startswith("//"):
            if _kept_line_comment(ctext, is_das):
                continue
        elif _kept_block_comment(ctext, is_das):
            continue
        out.append((ln, ctext))
    return out


def _text_key(t):
    return " ".join(t.split())


def edit_violations(old, new, is_das):
    """Fires only on a NET comment increase; quotes the comments not present
    (whitespace-normalized) in the replaced text. Moves, reindents, rewords,
    and deletions all stay silent — the rule is 'no new comments', not 'no
    touching old ones'."""
    old_list = violations(old, is_das, False)
    new_list = violations(new, is_das, False)
    if len(new_list) <= len(old_list):
        return []
    counts = Counter(_text_key(t) for _, t in old_list)
    out = []
    for ln, t in new_list:
        key = _text_key(t)
        if counts[key] > 0:
            counts[key] -= 1
            continue
        out.append((ln, t))
    return out


def build_message(path, viol, is_das, is_write):
    quoted = "\n".join("  L{}: {}".format(ln, t.splitlines()[0]) for ln, t in viol[:MAX_QUOTED])
    extra = "" if len(viol) <= MAX_QUOTED else "\n  ... and {} more".format(len(viol) - MAX_QUOTED)
    if is_write:
        what = "comment(s) in"
        where_note = " A full-file Write also lists the file's PRE-EXISTING comments — ignore those."
    else:
        what = "new comment(s) in"
        where_note = " Line numbers refer to the replacement text, not the file."
    if is_das:
        return (
            "comment guard: {} {} {} will NOT survive the formatter:\n"
            "{}{}\n"
            ".das keeps only `//!` public-API docs, `// nolint:CODE` / `// @nolint` suppressions, "
            "`//fmt:` directives, and the file's leading header block. Say it in the code (a name, "
            "a shape), pin it with a test, or move real documentation to the module's REVIEW.md / "
            "ARCHITECTURE.md. Teaching code (tutorials/examples) keeps its prose — ignore this "
            "warning there.{}"
        ).format(len(viol), what, path, quoted, extra, where_note)
    return (
        "comment guard: {} {} {} — house rule: no new C/C++ comments.\n"
        "{}{}\n"
        "Comments go stale, code does not. Prefer a clearer name, a shape, or a test; real "
        "documentation goes in the module's REVIEW.md / ARCHITECTURE.md. Kept: `//!` and "
        "`/** */` API docs, NOLINT suppressions, `// clang-format` directives, and the file's "
        "leading header block.{}"
    ).format(len(viol), what, path, quoted, extra, where_note)


def main():
    try:
        data = json.loads(sys.stdin.buffer.read().decode("utf-8", "replace"))
    except Exception:
        return 0
    if not isinstance(data, dict):
        return 0
    if data.get("hook_event_name") not in (None, "PostToolUse"):
        return 0
    if data.get("tool_name") not in ("Edit", "Write"):
        return 0
    tool_input = data.get("tool_input")
    if not isinstance(tool_input, dict):
        return 0
    resp = data.get("tool_response")
    if isinstance(resp, dict) and resp.get("success") is False:
        return 0
    path = str(tool_input.get("file_path") or "")
    low = path.lower()
    if low.endswith(DAS_EXTS):
        is_das = True
    elif low.endswith(C_EXTS):
        is_das = False
    else:
        return 0
    is_write = data.get("tool_name") == "Write"
    try:
        if is_write:
            viol = violations(str(tool_input.get("content") or ""), is_das, True)
        else:
            viol = edit_violations(str(tool_input.get("old_string") or ""),
                                   str(tool_input.get("new_string") or ""), is_das)
    except Exception:
        return 0
    if not viol:
        return 0
    try:
        sys.stderr.reconfigure(encoding="utf-8", errors="backslashreplace")
    except Exception:
        pass
    print(build_message(path, viol, is_das, is_write), file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
