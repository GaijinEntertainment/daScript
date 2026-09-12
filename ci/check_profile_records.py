#!/usr/bin/env python3
"""Gate the dasProfile records the site deploys: every das lane on every board, or the deploy fails.

A record is profile_results_<platform>.json as borisbat/dasProfile publishes it. The site's
benchmark boards render whatever lanes the record carries and drop a missing one silently, so a
capture that lost a das lane (an AOT module that never loaded, a runner started without -jit)
would ship a board with no das column and nobody would notice. This script fails the deploy
instead.

Usage: check_profile_records.py <record.json>...   (a missing platform is the fetch step's
business - only files that exist are checked)
"""
import json
import sys

INTERPRETED = "Interpreted"
AOT_OR_JIT = "AOT or JIT"
STARTUP = "Startup"
STARTUP_ROW = "hello world"

DAS_LANES = {
    INTERPRETED: ["DAS INTERPRETER"],
    AOT_OR_JIT: ["DAS AOT", "DAS JIT"],
}
DAS_STARTUP_LANES = ["DAS INTERPRETER", "DAS JIT", "DAS EXE"]


def lanes_of(rows):
    return {r.get("language") for r in rows if isinstance(r, dict)}


def check_record(path):
    problems = []
    try:
        with open(path, encoding="utf-8") as f:
            record = json.load(f)
    except (OSError, ValueError) as e:
        return [f"{path}: unreadable record: {e}"]
    if not isinstance(record, dict):
        return [f"{path}: record root is {type(record).__name__}, not an object"]
    for board in (INTERPRETED, AOT_OR_JIT, STARTUP):
        if not isinstance(record.get(board), dict) or not record[board]:
            problems.append(f"{path}: board '{board}' is missing or empty")
    if problems:
        return problems
    tests = set(record[INTERPRETED]) | set(record[AOT_OR_JIT])
    if set(record[INTERPRETED]) != set(record[AOT_OR_JIT]):
        problems.append(
            f"{path}: the two boards list different tests: "
            f"{sorted(set(record[INTERPRETED]) ^ set(record[AOT_OR_JIT]))}")
    for board, lanes in DAS_LANES.items():
        for test in sorted(tests):
            present = lanes_of(record[board].get(test, []))
            for lane in lanes:
                if lane not in present:
                    problems.append(f"{path}: '{board}' / '{test}' has no '{lane}' cell")
    startup = record[STARTUP].get(STARTUP_ROW)
    if not isinstance(startup, list):
        problems.append(f"{path}: '{STARTUP}' has no '{STARTUP_ROW}' row")
    else:
        present = lanes_of(startup)
        for lane in DAS_STARTUP_LANES:
            if lane not in present:
                problems.append(f"{path}: '{STARTUP}' / '{STARTUP_ROW}' has no '{lane}' cell")
    return problems


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    problems = []
    for path in argv[1:]:
        problems += check_record(path)
    for p in problems:
        print(f"::error::{p}")
    if problems:
        print(f"{len(problems)} problem(s): a das lane is missing from a benchmark record - "
              "the capture is incomplete, not the site")
        return 1
    print(f"profile records ok: {len(argv) - 1} file(s), every das lane present on every board")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
