#!/usr/bin/env python3
"""The job matrices of build.yml and extended_checks.yml, one place, as JSON.

    ci_matrix.py build <event_name>       -> {"include": [cell, ...]}
    ci_matrix.py extended <event_name>    -> {"include": [cell, ...]}

A per-PR job fits a 35-minute wall or its cells run on the nightly cron (and on a manual
dispatch) instead. The workflow's pre_job step evaluates this script and the fan-out job reads
`fromJSON(needs.pre_job.outputs.<matrix>)`, so the cells are data here rather than an include
list the runner merges by its own rules. `ci/test_ci_matrix.py` pins both sets.
"""
import json
import sys

NIGHTLY_EVENTS = ("schedule", "workflow_dispatch")


def is_nightly(event_name):
    return event_name in NIGHTLY_EVENTS


def _cells(target, architecture, presets, **props):
    return [dict(target=target, architecture=architecture, cmake_preset=preset, sanitizers="none", **props)
            for preset in presets]


def build_cells(event_name):
    """build.yml: Release + Debug on every platform per PR; the sanitizer cells and windows
    64 Debug are nightly-only (each is a 40-55 minute job whose signal a day's cadence serves)."""
    cmake = dict(build_system="cmake", cmake_generator="Ninja")
    cells = []
    cells += _cells("linux", 64, ["Debug", "Release"], release_target="linux", release_arch="x86_64",
                    runner="ubuntu-latest", archive_ext="tar.gz", **cmake)
    cells += _cells("linux_arm", 64, ["Debug", "Release"], release_target="linux", release_arch="arm64",
                    runner="ubuntu-24.04-arm", archive_ext="tar.gz", **cmake)
    cells += _cells("darwin15", "arm64", ["Debug", "Release"], release_target="darwin15", release_arch="arm64",
                    runner="macos-15", architecture_string="arm64", archive_ext="tar.gz", **cmake)
    cells += _cells("darwin26", "arm64", ["Debug", "Release"], release_target="darwin26", release_arch="arm64",
                    runner="macos-26", architecture_string="arm64", archive_ext="tar.gz", **cmake)
    # win32 Release is the 32-bit compile+test gate; win32 Debug outgrew its test budgets and is gone.
    cells += _cells("windows", 32, ["Release"], release_target="windows", release_arch="x86",
                    runner="windows-latest", architecture_string="Win32", archive_ext="zip", **cmake)
    # The Win64 Release lane is the fast MSVC compile/interpreter gate: LLVM off, JIT off (the
    # nightly build_windows_release_llvm_nightly job keeps the JIT sweep). Win64 Debug skips JIT too.
    cells += _cells("windows", 64, ["Release"], release_target="windows", release_arch="x86_64",
                    runner="windows-latest", architecture_string="x64", archive_ext="zip",
                    llvm_disabled="ON", jit_disabled="ON", **cmake)
    if is_nightly(event_name):
        cells += _cells("windows", 64, ["Debug"], release_target="windows", release_arch="x86_64",
                        runner="windows-latest", architecture_string="x64", archive_ext="zip",
                        jit_disabled="ON", nightly_only="ON", **cmake)
        for san in ("asan", "tsan", "ubsan"):
            cells.append(dict(target="linux", architecture=64, cmake_preset="Release", sanitizers=san,
                              runner="ubuntu-latest", build_name="linux_" + san, nightly_only="ON", **cmake))
    return cells


def extended_cells(event_name):
    """extended_checks.yml: per PR two darwin15 jobs split by role (core: the tree's own gates and
    utils; modules: the module and service suites); the nightly runs every step on linux, darwin15
    and windows in one job each (role all)."""
    cmake = dict(build_system="cmake", cmake_generator="Ninja")
    darwin = dict(target="darwin15", architecture="arm64", runner="macos-15", architecture_string="arm64", **cmake)
    if is_nightly(event_name):
        return [
            dict(target="linux", architecture=64, role="all", runner="ubuntu-latest", **cmake),
            dict(role="all", **darwin),
            dict(target="windows", architecture=64, role="all", runner="windows-latest", architecture_string="x64", **cmake),
        ]
    return [dict(role="core", **darwin), dict(role="modules", **darwin)]


def main(argv):
    if len(argv) != 3 or argv[1] not in ("build", "extended"):
        sys.stderr.write(__doc__)
        return 2
    cells = build_cells(argv[2]) if argv[1] == "build" else extended_cells(argv[2])
    print(json.dumps({"include": cells}, separators=(",", ":")))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
