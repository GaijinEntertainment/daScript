#!/usr/bin/env python3
"""PostToolUse hook: flags comments added by Edit/Write that the house rules reject.

.das: the formatter strips everything outside the kept set, so a freshly written
comment is already dead — this says so at the moment it happens. C/C++: no
stripper runs, but the same rule applies to NEW comments.

Pure python, no filesystem or config access: stdin JSON in, exit 0 (silent) or
exit 2 with the advisory on stderr. Anything unexpected in the input = exit 0.
"""

import json
import re
import sys

DAS_EXTS = (".das",)
C_EXTS = (".cpp", ".cc", ".cxx", ".c", ".h", ".hpp", ".hxx")

_STRING = re.compile(r'"(?:[^"\\]|\\.)*"' + r"|'(?:[^'\\]|\\.)*'")
_DAS_KEPT = re.compile(r"^//(?:!|\s*fmt:|\s*nolint\b)", re.I)
_C_KEPT = re.compile(r"^//(?:!|\s*nolint\b)", re.I)
_DAS_PREAMBLE = re.compile(r"^\s*(?:options|module|require)\b")

MAX_QUOTED = 5


def _mask_strings(line):
    """Blank out string/char literals so comment markers inside them are ignored,
    keeping every character position intact."""
    return _STRING.sub(lambda m: " " * (m.end() - m.start()), line)


def scan_comments(text):
    """Yield (line_no, comment_text, is_full_line) for each `//` and `/* */`
    comment; a multi-line block is reported once, at its opening line."""
    comments = []
    in_block = False
    for ln, raw in enumerate(text.splitlines(), 1):
        i = 0
        if in_block:
            end = raw.find("*/")
            if end < 0:
                continue
            in_block = False
            i = end + 2
        masked = _mask_strings(raw)
        while i < len(raw):
            s_line = masked.find("//", i)
            s_block = masked.find("/*", i)
            if s_line < 0 and s_block < 0:
                break
            if s_line >= 0 and (s_block < 0 or s_line < s_block):
                comments.append((ln, raw[s_line:].rstrip(), raw[:s_line].strip() == ""))
                break
            full = raw[:s_block].strip() == ""
            end = masked.find("*/", s_block + 2)
            if end < 0:
                comments.append((ln, raw[s_block:].rstrip(), full))
                in_block = True
                break
            comments.append((ln, raw[s_block:end + 2], full))
            i = end + 2
    return comments


def header_extent(text, is_das):
    """Line count of the leading header region: blank lines, full-line comments,
    and (das only) the options/module/require preamble the header may sit below."""
    in_block = False
    last = 0
    for ln, raw in enumerate(text.splitlines(), 1):
        s = raw.strip()
        if in_block:
            last = ln
            if "*/" in s:
                in_block = False
            continue
        if s == "" or s.startswith("//"):
            last = ln
            continue
        if s.startswith("/*"):
            last = ln
            if "*/" not in s[2:]:
                in_block = True
            continue
        if is_das and _DAS_PREAMBLE.match(raw):
            last = ln
            continue
        break
    return last


def violations(text, is_das, whole_file):
    """Comments in `text` outside the kept set. `whole_file` grants the leading
    header block; edit fragments get no positional context, so it stays False there."""
    kept = _DAS_KEPT if is_das else _C_KEPT
    header_end = header_extent(text, is_das) if whole_file else 0
    out = []
    for ln, ctext, full in scan_comments(text):
        if full and ln <= header_end:
            continue
        if ctext.startswith("//") and kept.match(ctext):
            continue
        out.append((ln, ctext))
    return out


def edit_violations(old, new, is_das):
    """Only comments the edit ADDS: anything already present verbatim in the
    replaced text (a move, a reindent) passes."""
    old_texts = {t.strip() for _, t in violations(old, is_das, False)}
    return [(ln, t) for ln, t in violations(new, is_das, False) if t.strip() not in old_texts]


def build_message(path, viol, is_das):
    quoted = "\n".join("  L{}: {}".format(ln, t) for ln, t in viol[:MAX_QUOTED])
    extra = "" if len(viol) <= MAX_QUOTED else "\n  ... and {} more".format(len(viol) - MAX_QUOTED)
    if is_das:
        return (
            "comment guard: {} comment(s) added to {} will NOT survive the formatter:\n"
            "{}{}\n"
            ".das keeps only `//!` public-API docs, `// nolint` suppressions, `//fmt:` directives, "
            "and the file's leading header block. Say it in the code (a name, a shape), pin it with "
            "a test, or move real documentation to the module's REVIEW.md / ARCHITECTURE.md. "
            "Teaching code (tutorials/examples) keeps its prose — ignore this warning there."
        ).format(len(viol), path, quoted, extra)
    return (
        "comment guard: {} new comment(s) in {} — house rule: no new C/C++ comments.\n"
        "{}{}\n"
        "Comments go stale, code does not. Prefer a clearer name, a shape, or a test; real "
        "documentation goes in the module's REVIEW.md / ARCHITECTURE.md. Kept: `//!` API docs, "
        "`// NOLINT` suppressions, and the file's leading header block."
    ).format(len(viol), path, quoted, extra)


def main():
    try:
        data = json.load(sys.stdin)
    except Exception:
        return 0
    if data.get("tool_name") not in ("Edit", "Write"):
        return 0
    tool_input = data.get("tool_input")
    if not isinstance(tool_input, dict):
        return 0
    path = str(tool_input.get("file_path") or "")
    low = path.lower()
    if low.endswith(DAS_EXTS):
        is_das = True
    elif low.endswith(C_EXTS):
        is_das = False
    else:
        return 0
    if data.get("tool_name") == "Write":
        viol = violations(str(tool_input.get("content") or ""), is_das, True)
    else:
        viol = edit_violations(str(tool_input.get("old_string") or ""),
                               str(tool_input.get("new_string") or ""), is_das)
    if not viol:
        return 0
    print(build_message(path, viol, is_das), file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
