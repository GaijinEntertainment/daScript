#!/usr/bin/env python3
"""The job matrices of build.yml, nightly.yml and extended_checks.yml, one place, as JSON.

    ci_matrix.py build|build_nightly|extended|extended_nightly    -> {"include": [cell, ...]}

A per-PR job fits a 35-minute wall or its cells run on the nightly cron (and on a manual
dispatch) instead. The workflow's pre_job step evaluates this script and the fan-out job reads
`fromJSON(needs.pre_job.outputs.<matrix>)`, so the cells are data here rather than an include
list the runner merges by its own rules. `ci/test_ci_matrix.py` pins both sets.
"""
import json
import sys

CLANG = "-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++"
LINUX_ARM_EXTRA = CLANG + " -DDAS_GLFW_DISABLED=ON -DDAS_HV_DISABLED=OFF -DDAS_SQLITE_DISABLED=OFF"
WINDOWS_EXTRA = ("-DCMAKE_C_COMPILER_LAUNCHER=sccache -DCMAKE_CXX_COMPILER_LAUNCHER=sccache"
                 " -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake")


SANITIZERS = ("asan", "tsan", "ubsan")
# one nightly sanitizer job per step: the per-PR sweeps, the full AOT suite, the backend sweeps -
# in one job they ran 2.5 to 4 hours back to back, and the tsan backend sweep never fit its cap
SANITIZER_PHASES = ("tests", "slow", "backend")


def _cells(target, architecture, presets, **props):
    props.setdefault("fast_math", "OFF")
    props.setdefault("phase", "all")
    return [dict(target=target, architecture=architecture, cmake_preset=preset, sanitizers="none", **props)
            for preset in presets]


def build_cells(nightly):
    """build.yml: Release + Debug on every platform per PR; the sanitizer cells and windows
    64 Debug are nightly-only. A sanitizer cell is three jobs, one per test step (`phase`);
    every other cell runs every step (`phase` all)."""
    cmake = dict(build_system="cmake", cmake_generator="Ninja")
    cells = []
    cells += _cells("linux", 64, ["Debug", "Release"], release_target="linux", release_arch="x86_64",
                    runner="ubuntu-latest", archive_ext="tar.gz", cmake_extra=CLANG, **cmake)
    cells += _cells("linux_arm", 64, ["Debug", "Release"], release_target="linux", release_arch="arm64",
                    runner="ubuntu-24.04-arm", archive_ext="tar.gz", cmake_extra=LINUX_ARM_EXTRA, **cmake)
    cells += _cells("darwin15", "arm64", ["Debug", "Release"], release_target="darwin15", release_arch="arm64",
                    runner="macos-15", architecture_string="arm64", archive_ext="tar.gz",
                    cmake_extra=CLANG + " -DCMAKE_OSX_ARCHITECTURES=arm64", **cmake)
    cells += _cells("darwin26", "arm64", ["Debug", "Release"], release_target="darwin26", release_arch="arm64",
                    runner="macos-26", architecture_string="arm64", archive_ext="tar.gz",
                    cmake_extra=CLANG + " -DCMAKE_OSX_ARCHITECTURES=arm64", **cmake)
    # win32 Release is the 32-bit compile+test gate; win32 Debug outgrew its test budgets and is gone.
    cells += _cells("windows", 32, ["Release"], release_target="windows", release_arch="x86",
                    runner="windows-latest", architecture_string="Win32", archive_ext="zip",
                    cmake_extra=WINDOWS_EXTRA, **cmake)
    # The Win64 Release lane is the fast MSVC compile/interpreter gate: LLVM off, JIT off (the
    # nightly build_windows_release_llvm_nightly job keeps the JIT sweep). Win64 Debug skips JIT too.
    cells += _cells("windows", 64, ["Release"], release_target="windows", release_arch="x86_64",
                    runner="windows-latest", architecture_string="x64", archive_ext="zip",
                    llvm_disabled="ON", jit_disabled="ON", cmake_extra=WINDOWS_EXTRA, **cmake)
    if nightly:
        cells += _cells("windows", 64, ["Debug"], release_target="windows", release_arch="x86_64",
                        runner="windows-latest", architecture_string="x64", archive_ext="zip",
                        jit_disabled="ON", nightly_only="ON", cmake_extra=WINDOWS_EXTRA, **cmake)
        for san in SANITIZERS:
            for phase in SANITIZER_PHASES:
                cells.append(dict(target="linux", architecture=64, cmake_preset="Release", sanitizers=san,
                                  runner="ubuntu-latest", build_name="linux_" + san, nightly_only="ON",
                                  fast_math="OFF", phase=phase, cmake_extra=CLANG, **cmake))
        # the tree built -ffast-math, the way an embedder that passes it does (dagor is one)
        cells.append(dict(target="linux", architecture=64, cmake_preset="Release", sanitizers="none",
                          fast_math="ON", runner="ubuntu-latest", build_name="linux_fastmath",
                          nightly_only="ON", phase="all",
                          cmake_extra="-DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++", **cmake))
        cells = [c for c in cells if c["cmake_preset"] == "Release" or c.get("nightly_only")]
    return cells


def extended_cells(nightly):
    """extended_checks.yml: per PR two darwin15 jobs split by role (core: the tree's own gates and
    utils; modules: the module and service suites); the nightly runs every step on linux, darwin15
    and windows in one job each (role all)."""
    cmake = dict(build_system="cmake", cmake_generator="Ninja")
    darwin = dict(target="darwin15", architecture="arm64", runner="macos-15", architecture_string="arm64", **cmake)
    if nightly:
        return [
            dict(target="linux", architecture=64, role="all", runner="ubuntu-latest", **cmake),
            dict(role="all", **darwin),
            dict(target="windows", architecture=64, role="all", runner="windows-latest", architecture_string="x64", **cmake),
        ]
    return [dict(role="core", **darwin), dict(role="modules", **darwin)]


def main(argv):
    if len(argv) != 2 or argv[1] not in ("build", "build_nightly", "extended", "extended_nightly"):
        sys.stderr.write(__doc__)
        return 2
    nightly = argv[1].endswith("_nightly")
    cells = build_cells(nightly) if argv[1].startswith("build") else extended_cells(nightly)
    print(json.dumps({"include": cells}, separators=(",", ":")))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
