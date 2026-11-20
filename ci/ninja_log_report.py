#!/usr/bin/env python3
"""Prints where a ninja build spent its time, from the build directory's .ninja_log.

    ninja_log_report.py <build dir>/.ninja_log [top N, default 25]

Ninja's own output gives only the moment an edge finished. The log holds each edge's start and
end, so this is the one place a job's time by target can be read. One table per ninja
invocation the log holds: a workflow job runs ninja once per step that builds.
"""
import collections
import os
import re
import sys

COMPILED = (".o", ".obj")
LINKED = (".a", ".lib", ".so", ".dylib", ".dll", ".exe", ".shared_module")
SLOW_SINGLES_SHOWN = 12
SLOW_SINGLE_MIN_MS = 30000


def read_invocations(path):
    """The log's edges as (start ms, end ms, output), one list per ninja invocation.

    The log marks no boundary between invocations, so one begins wherever an end time goes
    back; a short invocation followed by a longer one reads as one. The log has a line per
    output, and the lines of one edge share its times and its command hash: a link that writes
    a DLL and its import library, or a stamp cmake names by two paths, is one edge here.
    """
    invocations = []
    seen = set()
    last_end = None
    with open(path, encoding="utf-8", errors="replace") as log:
        for line in log:
            fields = line.rstrip("\n").split("\t")
            if len(fields) < 4 or not (fields[0].isdigit() and fields[1].isdigit()):
                continue
            start, end, output = int(fields[0]), int(fields[1]), fields[3].replace("\\", "/")
            if last_end is None or end < last_end:
                invocations.append([])
                seen = set()
            last_end = end
            command = fields[4] if len(fields) > 4 else os.path.basename(output)
            dedupe_key = (start, end, command)
            if dedupe_key in seen:
                continue
            seen.add(dedupe_key)
            invocations[-1].append((start, end, output))
    return invocations


def row_key(output):
    """'(compile)' or '(link)' for those edges, else the output's basename less .stamp, its digits folded to N."""
    if output.endswith(COMPILED):
        return "(compile)"
    if output.endswith(LINKED):
        return "(link)"
    name = os.path.basename(output)
    if name.endswith(".stamp"):
        name = name[:-len(".stamp")]
    return re.sub(r"\d+", "N", name)


def report(invocation, top):
    wall = max(end for _, end, _ in invocation) / 60000.0
    edge_minutes = collections.Counter()
    edges = collections.Counter()
    longest = collections.Counter()
    for start, end, output in invocation:
        row = row_key(output)
        minutes = (end - start) / 60000.0
        edge_minutes[row] += minutes
        edges[row] += 1
        longest[row] = max(longest[row], minutes)
    lines = [f"wall {wall:.1f} min, {len(invocation)} edges, {sum(edge_minutes.values()):.1f} edge-minutes"]
    lines.append(f"{'edge-min':>9} {'edges':>6} {'longest':>8}  what")
    for row, minutes in edge_minutes.most_common(top):
        lines.append(f"{minutes:9.1f} {edges[row]:6d} {longest[row]:8.1f}  {row}")
    compiles_and_links = [(end - start, output) for start, end, output in invocation
                          if output.endswith(COMPILED + LINKED)]
    slow = [(ms, output) for ms, output in sorted(compiles_and_links, reverse=True)[:SLOW_SINGLES_SHOWN]
            if ms >= SLOW_SINGLE_MIN_MS]
    if slow:
        lines.append("slowest single compiles and links:")
        for ms, output in slow:
            lines.append(f"{ms / 60000.0:9.1f}  {output}")
    return lines


def main(argv):
    if len(argv) < 2:
        sys.stderr.write(__doc__)
        return 2
    path = argv[1]
    top = int(argv[2]) if len(argv) > 2 else 25
    if not os.path.isfile(path):
        print(f"ninja log: no {path} - this build did not run under ninja")
        return 0
    for n, invocation in enumerate(read_invocations(path), 1):
        print(f"ninja log: invocation {n} (one begins where an end time goes back)")
        for line in report(invocation, top):
            print("  " + line)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
