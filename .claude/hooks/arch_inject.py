#!/usr/bin/env python3
"""PostToolUse hook (Edit|Write|MultiEdit): when the edit lands inside a function that carries
[arch(at="<doc>.md#<anchor>")], hand the cited section to the model - once per anchor per
session (a memo keyed by anchor and section hash), so twelve citers of one section cost one
injection. `--reset` (PreCompact / SessionEnd) drops the memo: after a compaction the earlier
copy left the context, so the next edit re-injects. Reads only stdin plus the files it names."""

import hashlib
import json
import os
import re
import sys
import tempfile

CITE = re.compile(r'arch\s*\(\s*at\s*=\s*"([^"#]+)#([^"]+)"')
DEF = re.compile(r'^\s*def\b')
HEADING = re.compile(r'^(#{1,6})\s+.*\{#([^}\s]+)\}\s*$')
MAX_LINES = 60


def memo_path(session_id):
    return os.path.join(tempfile.gettempdir(), "claude-arch-memo-%s.json" % (session_id or "nosession"))


def load_memo(path):
    try:
        with open(path, "r", encoding="utf-8") as f:
            return json.load(f)
    except Exception:
        return {}


def save_memo(path, memo):
    try:
        with open(path, "w", encoding="utf-8") as f:
            json.dump(memo, f)
    except Exception:
        pass


def read_text(path):
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as f:
            return f.read()
    except Exception:
        return ""


def citations_above(lines, def_index):
    """The [arch] citations in the annotation block directly above a def line."""
    found = []
    i = def_index - 1
    while i >= 0:
        t = lines[i].strip()
        if t == "" or t.startswith("//"):
            i -= 1
            continue
        if t.startswith("[") or t.endswith("]") or t.endswith(","):
            found.extend(CITE.findall(t))
            i -= 1
            continue
        break
    return found


def enclosing_citations(text, offset):
    """Citations of the function whose body holds `offset` (or the annotation block the edit sits in)."""
    lines = text.split("\n")
    line_no = text.count("\n", 0, max(offset, 0))
    for i in range(min(line_no, len(lines) - 1), -1, -1):
        if DEF.match(lines[i]):
            return citations_above(lines, i)
        # an edit on the annotation block itself
        if CITE.search(lines[i]) and i >= line_no:
            return CITE.findall(lines[i])
    return []


def all_citations(text):
    return CITE.findall(text)


def section_of(doc_text, anchor):
    lines = doc_text.split("\n")
    start = -1
    level = 0
    for i, line in enumerate(lines):
        m = HEADING.match(line.rstrip("\r"))
        if m and m.group(2) == anchor:
            start = i
            level = len(m.group(1))
            break
    if start < 0:
        return ""
    stop = len(lines)
    for i in range(start + 1, len(lines)):
        m = re.match(r'^(#{1,6})\s', lines[i])
        if m and len(m.group(1)) <= level:
            stop = i
            break
    body = [l.rstrip("\r") for l in lines[start:stop]]
    while len(body) > 1 and body[-1].strip() == "":
        body.pop()
    if len(body) > MAX_LINES:
        body = body[:MAX_LINES] + ["... (section continues - `arch_of` returns it whole)"]
    return "\n".join(body)


def main():
    raw = sys.stdin.read()
    try:
        payload = json.loads(raw) if raw.strip() else {}
    except Exception:
        return 0
    session = payload.get("session_id", "")
    path = memo_path(session)
    if "--reset" in sys.argv:
        try:
            os.remove(path)
        except Exception:
            pass
        return 0
    tool = payload.get("tool_name", "")
    tin = payload.get("tool_input", {}) or {}
    file_path = tin.get("file_path", "") or ""
    if tool not in ("Edit", "Write", "MultiEdit") or not file_path.endswith(".das"):
        return 0
    text = read_text(file_path)
    if not text or "arch" not in text:
        return 0
    cites = []
    if tool == "Write":
        cites = all_citations(text)
    else:
        edits = tin.get("edits", []) if tool == "MultiEdit" else [tin]
        for e in edits:
            needle = e.get("new_string", "") or e.get("old_string", "") or ""
            if not needle.strip():
                continue
            # every occurrence: a repeated snippet cannot say which function took the edit,
            # so each enclosing function's sections are owed (the memo folds the repeats)
            at = text.find(needle)
            hits = 0
            while at >= 0 and hits < 16:
                cites.extend(enclosing_citations(text, at))
                hits += 1
                at = text.find(needle, at + 1)
    if not cites:
        return 0
    folder = os.path.dirname(os.path.abspath(file_path))
    memo = load_memo(path)
    out = []
    seen_here = set()
    for doc, anchor in cites:
        doc_path = os.path.normpath(os.path.join(folder, doc)).replace("\\", "/")
        key = "%s#%s" % (doc_path, anchor)
        if key in seen_here:
            continue
        seen_here.add(key)
        section = section_of(read_text(doc_path), anchor)
        if not section:
            continue
        digest = hashlib.sha1(section.encode("utf-8")).hexdigest()
        if memo.get(key) == digest:
            continue
        memo[key] = digest
        rel = os.path.relpath(doc_path, payload.get("cwd", "") or os.getcwd()).replace("\\", "/")
        out.append("[arch] %s#%s - the section this function answers for:\n%s" % (rel, anchor, section))
    if not out:
        return 0
    save_memo(path, memo)
    context = ("The function you just edited cites an architecture section. Keep the code true to it, "
               "or change the section in the same diff (REVIEW_COMMON.md's [arch] audit duty).\n\n"
               + "\n\n".join(out))
    print(json.dumps({"hookSpecificOutput": {"hookEventName": "PostToolUse",
                                              "additionalContext": context}}))
    return 0


if __name__ == "__main__":
    sys.exit(main())
