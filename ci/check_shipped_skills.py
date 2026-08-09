#!/usr/bin/env python3
"""Gate the skills shipped into an SDK bundle.

install/skills.list copies skills/*.md into the bundle verbatim, so anything a skill
says has to be true *from inside the bundle*. A skill that points at src/, links a file
that is not there, or tells the reader to run bin/Release/daslang.exe is a dead end for
an SDK user -- and for Claude running in an SDK install, which cannot tell a stale path
from a real one.

Five checks. Each is deliberately narrow: a gate that cries wolf gets muted, and then it
is worse than no gate. Two escape hatches, both spelled `repo-only`:
  * in a HEADING -> exempts that section, resetting at the next heading of any level.
    (Not "everything after the marker": cpp_integration.md has a repo-internals section
    in the MIDDLE, and a trailing rule would exempt the rest of the file.)
  * on a LINE    -> exempts that line, for one-off mentions in prose.

Usage:
    python3 ci/check_shipped_skills.py <bundle-root> [<skills.list>]
"""
import os
import re
import sys

# --- what can never be inside a bundle -------------------------------------------------
REPO_PATH = re.compile(
    r"(?<![\w./-])("
    r"src/[A-Za-z0-9_./+-]*[A-Za-z0-9_+-]"
    r"|tests/[A-Za-z0-9_./+-]*[A-Za-z0-9_+-]"
    r"|tests-cpp/[A-Za-z0-9_./+-]*[A-Za-z0-9_+-]"
    r"|doc/source/[A-Za-z0-9_./+*-]*[A-Za-z0-9_+*-]"
    r"|benchmarks/[A-Za-z0-9_./+-]*[A-Za-z0-9_+-]"
    r"|modules/[A-Za-z0-9_-]+/(?:src|tests|tutorial|harness)[A-Za-z0-9_./+-]*"
    r")")

# Windows MSVC build layout. A bundle has bin/daslang -- no config subdir, and no .exe
# outside Windows. These are checked INSIDE code fences too, because that is exactly
# where command lines live.
PLATFORM_BIN = re.compile(r"bin/(?:Release|Debug|RelWithDebInfo|MinSizeRel)/")
# Only an INVOCATION, not a prose mention. cpp_integration.md says "shared across
# `daslang.exe` and every module DLL", which is fine; `daslang.exe utils/aot/main.das`
# is not. The discriminator is the next token: a flag, or a real file with an extension.
EXE_CMD = re.compile(
    r"(?<![\w/])(daslang|daslang-live|das-fmt|gen1_to_gen2|dastest)\.exe"
    r"(?=\s+(?:-{1,2}[A-Za-z]|[A-Za-z0-9_./-]+\.(?:das|cpp|py|json|md)\b))")

# SOMEONE'S MACHINE, not a generic example. Deliberately not "any absolute path": the
# junction workflow in external_module_debugging.md uses illustrative C:/DummyRoot paths
# 17 times on purpose, and flagging those would force 17 noise markers onto correct
# content. A bare `\\`-UNC branch is also out -- it matched `\\d` / `\\s` in regex prose.
# AppData needs a leading separator: bare `APPDATA` is the ENV VAR NAME, which
# environment_variables.md documents alongside XDG_CONFIG_HOME. Only a real path is a leak.
MACHINE_PATH = re.compile(
    r"(?:[A-Za-z]:[\\/](?:Users|Work|DASPKG)\b|/home/[A-Za-z0-9_.]+|[\\/]AppData\b)", re.I)

SKILL_REF = re.compile(r"skills/([a-z_0-9]+\.md)")
MD_LINK = re.compile(r"\[[^\]]*\]\(([^)\s]+)\)")
FENCE = re.compile(r"^\s*```")
HEADING = re.compile(r"^#+\s")


def load_ship_list(path):
    out = set()
    if not os.path.exists(path):
        return out, False
    with open(path, encoding="utf-8") as fh:
        for ln in fh:
            ln = ln.strip()
            if ln and not ln.startswith("#"):
                out.add(ln)
    return out, True


def check(bundle, ship_list_path):
    skills_dir = os.path.join(bundle, "skills")
    if not os.path.isdir(skills_dir):
        print("  no skills/ in bundle: %s" % skills_dir)
        return 2
    ships, have_list = load_ship_list(ship_list_path)
    problems = []

    for name in sorted(os.listdir(skills_dir)):
        if not name.endswith(".md"):
            continue
        path = os.path.join(skills_dir, name)
        with open(path, encoding="utf-8", errors="replace") as fh:
            lines = fh.readlines()

        # Pass 1: fence state, and which PARAGRAPH each line belongs to. The marker scopes
        # to the paragraph, not the line: prose wraps, so a marker on the closing line of a
        # sentence has to cover the path mentioned two lines earlier. Scoping per-line means
        # authors must reflow sentences to satisfy the gate, which is how a gate gets muted.
        in_fence = []
        para_of = []
        para_marked = {}
        fence = False
        para = 0
        prev_blank = True
        for raw in lines:
            if FENCE.match(raw):
                fence = not fence
                in_fence.append(True)
                para_of.append(None)
                prev_blank = True
                continue
            in_fence.append(fence)
            blank = not raw.strip()
            if fence:
                para_of.append(None)
            elif blank or HEADING.match(raw):
                para_of.append(None)
                prev_blank = True
            else:
                if prev_blank:
                    para += 1
                prev_blank = False
                para_of.append(para)
                if "repo-only" in raw.lower():
                    para_marked[para] = True

        # Pass 2: the checks.
        exempt_section = False
        for idx, raw in enumerate(lines):
            lineno = idx + 1
            fence = in_fence[idx]
            if FENCE.match(raw):
                continue
            low = raw.lower()

            if not fence and HEADING.match(raw):
                exempt_section = "repo-only" in low
                continue

            exempt = (exempt_section or ("repo-only" in low)
                      or para_marked.get(para_of[idx], False))

            def flag(kind, detail):
                problems.append((name, lineno, kind, detail))

            # these two are about COMMANDS, so they apply inside fences as well
            if not exempt:
                m = PLATFORM_BIN.search(raw)
                if m:
                    flag("windows build layout",
                         "%s -- a bundle has bin/daslang" % m.group(0))
                m = EXE_CMD.search(raw)
                if m:
                    flag("windows-only exe",
                         "%s.exe -- drop .exe, it is Windows-only" % m.group(1))
                m = MACHINE_PATH.search(raw)
                if m:
                    flag("machine-local path", m.group(0))

            if fence or exempt:
                continue

            for m in REPO_PATH.finditer(raw):
                flag("not in bundle", m.group(1))

            if have_list:
                for m in SKILL_REF.finditer(raw):
                    if m.group(1) not in ships:
                        flag("skill not shipped",
                             "skills/%s is absent from install/skills.list" % m.group(1))

            for m in MD_LINK.finditer(raw):
                tgt = m.group(1)
                if re.match(r"^(https?:|mailto:|#|<)", tgt):
                    continue
                bare = tgt.split("#")[0]
                if not bare:
                    continue
                # Links are authored relative to the skill's own directory
                # (`../daslib/x.das`, sibling `other_skill.md`), so try that FIRST;
                # fall back to bundle-root-relative, which some skills also use.
                if not any(os.path.exists(os.path.join(base, bare))
                           for base in (skills_dir, bundle)):
                    flag("dead link", tgt)

    if not problems:
        return 0

    by_kind = {}
    for name, lineno, kind, detail in problems:
        by_kind.setdefault(kind, []).append((name, lineno, detail))
    for kind in sorted(by_kind):
        rows = by_kind[kind]
        print("  %s (%d):" % (kind, len(rows)))
        for name, lineno, detail in rows:
            print("    skills/%s:%d  %s" % (name, lineno, detail))
    print("")
    print("  Fix the path, or mark it: put 'repo-only' on the line, or on the heading")
    print("  of a section that is entirely repo internals.")
    return 1


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(2)
    bundle = sys.argv[1]
    ship_list = sys.argv[2] if len(sys.argv) > 2 else os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "install", "skills.list")
    sys.exit(check(bundle, ship_list))
