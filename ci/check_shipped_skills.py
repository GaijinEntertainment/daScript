#!/usr/bin/env python3
"""Gate the skills shipped into an SDK bundle.

The FOLDER is the shipping decision: everything under skills/ ships except
skills/internal/, and skills/daslang/ is the standalone language skill. So
anything a shipped skill says has to be true *from inside the bundle*. A skill
that points at src/, links a file that is not there, or tells the reader to run
bin/Release/daslang.exe is a dead end for an SDK user -- and for Claude running
in an SDK install, which cannot tell a stale path from a real one.

Checks, each deliberately narrow (a gate that cries wolf gets muted, and then it
is worse than no gate). Two escape hatches, both spelled `repo-only`:
  * in a HEADING -> exempts that section, resetting at the next heading of any level.
    (Not "everything after the marker": cpp_integration.md has a repo-internals section
    in the MIDDLE, and a trailing rule would exempt the rest of the file.)
  * on a LINE    -> exempts that line, for one-off mentions in prose.
Files under skills/daslang/ get NO escape hatch: the bundle is standalone by
contract (no repo paths, no MCP, no CI, no lint rule IDs -- see its README in
the repo), so a violation there is fixed, never marked.

Also walks the REPO's skills/internal/ (when run from a checkout) for dead
relative links -- internal skills rot too, they just rot privately.

Usage:
    python3 ci/check_shipped_skills.py <bundle-root>
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
    r"|history/[A-Za-z0-9_./+-]*[A-Za-z0-9_+-]"
    r"|utils/daScript/[A-Za-z0-9_./+-]*"
    r"|modules/[A-Za-z0-9_-]+/(?:src|tests|tutorial|harness)[A-Za-z0-9_./+-]*"
    r")")
# NOT banned: utils/lint/tests/ -- CMake installs it (the SDK self-checks its linter).

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
# on purpose, and flagging those would force noise markers onto correct content.
# A bare `\\`-UNC branch is also out -- it matched `\\d` / `\\s` in regex prose.
# AppData needs a leading separator: bare `APPDATA` is the ENV VAR NAME. Only a real
# path is a leak.
MACHINE_PATH = re.compile(
    r"(?:[A-Za-z]:[\\/](?:Users|Work|DASPKG)\b|/home/[A-Za-z0-9_.]+|[\\/]AppData\b)", re.I)

# Nested paths too: the old single-segment regex was BLIND to skills/internal/x.md,
# which is exactly the reference that must not leak into a shipped file unmarked.
SKILL_REF = re.compile(r"skills/((?:[A-Za-z0-9_-]+/)*[A-Za-z0-9_-]+\.md)")
MD_LINK = re.compile(r"\[[^\]]*\]\(([^)\s]+)\)")
FENCE = re.compile(r"^\s*```")
HEADING = re.compile(r"^#+\s")

# skills/daslang/ standalone-contract bans (its README states the rules; the gate
# holds them). Narrow by design: \bMCP\b not "mcp" (filenames), rule IDs by shape.
# not \bCI\b: a das typedef named CI is legitimate code (generics.md has one)
BUNDLE_MCP = re.compile(r"\bMCP\b")
BUNDLE_CI = re.compile(r"\bCI (?:lane|run|job|pipeline)|\.github/|workflows/|GitHub Actions")
BUNDLE_LINT_ID = re.compile(r"\b(?:PERF|STYLE|LINT|IMGUI)\d{3}\b")


def scan_file(path, relname, problems, *, bundle_pure, link_bases, shipped_exists,
              links_only=False):
    """Run the content checks over one markdown file.

    bundle_pure: True under skills/daslang/ -- no repo-only escape, extra bans.
    link_bases: directories to resolve relative markdown links against, in order.
    shipped_exists: callable(skill_rel) -> bool for skills/<rel> references;
                    None disables the check (repo-side internal walk).
    links_only: dead-link check only -- for skills/internal/, where repo paths,
                bin/Release and machine examples are the whole point.
    """
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

        exempt = (not bundle_pure
                  and (exempt_section or ("repo-only" in low)
                       or para_marked.get(para_of[idx], False)))

        def flag(kind, detail):
            problems.append((relname, lineno, kind, detail))

        # these are about COMMANDS, so they apply inside fences as well
        if not exempt and not links_only:
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

        if not links_only:
            for m in REPO_PATH.finditer(raw):
                flag("not in bundle", m.group(1))

        if bundle_pure:
            if BUNDLE_MCP.search(raw):
                flag("bundle purity", "MCP mention -- the language skill is tool-agnostic")
            if BUNDLE_CI.search(raw):
                flag("bundle purity", "CI mention -- the language skill is repo-agnostic")
            m = BUNDLE_LINT_ID.search(raw)
            if m:
                flag("bundle purity",
                     "%s -- lint rule IDs stay out of the language skill" % m.group(0))

        if shipped_exists is not None:
            for m in SKILL_REF.finditer(raw):
                if not shipped_exists(m.group(1)):
                    flag("skill not shipped",
                         "skills/%s does not exist in the bundle" % m.group(1))

        for m in MD_LINK.finditer(raw):
            tgt = m.group(1)
            if re.match(r"^(https?:|mailto:|#|<)", tgt):
                continue
            bare = tgt.split("#")[0]
            # a letterless target is das subscript-call syntax (`arr[0](7)`),
            # not a markdown link
            if not bare or not re.search(r"[A-Za-z]", bare):
                continue
            # Links are authored relative to the skill's OWN directory first
            # (sibling `other_skill.md`, `references/x.md`), then the skills/
            # root, then the tree root -- all three spellings are in use.
            if not any(os.path.exists(os.path.join(base, bare))
                       for base in link_bases):
                flag("dead link", tgt)


def check(bundle):
    skills_dir = os.path.join(bundle, "skills")
    if not os.path.isdir(skills_dir):
        print("  no skills/ in bundle: %s" % skills_dir)
        return 2
    problems = []

    def shipped_exists(rel):
        return os.path.exists(os.path.join(skills_dir, rel))

    root_skills = []
    for dirpath, dirnames, filenames in os.walk(skills_dir):
        dirnames.sort()
        rel_dir = os.path.relpath(dirpath, skills_dir)
        for name in sorted(filenames):
            if not name.endswith(".md"):
                continue
            path = os.path.join(dirpath, name)
            rel = name if rel_dir == "." else rel_dir.replace(os.sep, "/") + "/" + name
            if rel_dir == ".":
                root_skills.append(name)
            scan_file(path, rel, problems,
                      bundle_pure=rel.startswith("daslang/"),
                      link_bases=(dirpath, skills_dir, bundle),
                      shipped_exists=shipped_exists)

    # Every shipped root skill needs a trigger row in the bundle's CLAUDE.md --
    # a skill with no row is a skill nobody opens.
    claude_md = os.path.join(bundle, "CLAUDE.md")
    if os.path.exists(claude_md):
        with open(claude_md, encoding="utf-8", errors="replace") as fh:
            claude_text = fh.read()
        for name in root_skills:
            if ("skills/" + name) not in claude_text:
                problems.append(("(bundle CLAUDE.md)", 0, "no trigger row",
                                 "skills/%s ships but has no row" % name))
    else:
        problems.append(("(bundle)", 0, "no trigger row", "CLAUDE.md missing from bundle"))

    # Repo-side: internal skills rot too, they just rot privately. Dead relative
    # links only -- internal files may reference anything that exists.
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    internal_dir = os.path.join(repo_root, "skills", "internal")
    if os.path.isdir(internal_dir):
        for name in sorted(os.listdir(internal_dir)):
            if not name.endswith(".md"):
                continue
            scan_file(os.path.join(internal_dir, name), "internal/" + name, problems,
                      bundle_pure=False,
                      link_bases=(internal_dir, os.path.join(repo_root, "skills"),
                                  repo_root),
                      shipped_exists=None, links_only=True)

    if not problems:
        return 0

    by_kind = {}
    for name, lineno, kind, detail in problems:
        by_kind.setdefault(kind, []).append((name, lineno, detail))
    for kind in sorted(by_kind):
        rows = by_kind[kind]
        print("  %s (%d):" % (kind, len(rows)))
        for name, lineno, detail in rows:
            where = name if name.startswith("(") else "skills/%s:%d" % (name, lineno)
            print("    %s  %s" % (where, detail))
    print("")
    print("  Fix the path, or mark it: put 'repo-only' on the line, or on the heading")
    print("  of a section that is entirely repo internals. Files under skills/daslang/")
    print("  have no marker escape -- fix the content.")
    return 1


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(__doc__)
        sys.exit(2)
    sys.exit(check(sys.argv[1]))
