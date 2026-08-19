#!/usr/bin/env bash
# Verify that every utility in a daslang release bundle at least launches.
#
# Designed to catch install-rule regressions like the v0.6.2-RC3
# `utils/mcp/cpp_search_config.das` miss, where the README-documented
# launch (`daslang.exe utils/mcp/main.das`) failed at parse time because
# a top-level peer of `main.das` was omitted from CMake's install(FILES …)
# list.
#
# Usage:  bash ci/smoke_test_bundle.sh <bundle-root>
#
# <bundle-root> is the directory produced by `cmake --install … --prefix <dir>`
# (contains bin/, daslib/, utils/, modules/, …).
#
# Exit 0 if every check passes; non-zero with a per-check failure log otherwise.

set -u

if [[ $# -lt 1 ]]; then
    echo "usage: $0 <bundle-root>" >&2
    exit 2
fi

# Canonicalize to absolute up front. The script `cd "$BUNDLE"` later, so any
# relative path baked into $DASLANG / exe paths would resolve from the wrong
# cwd and every check would fail with command-not-found.
BUNDLE="$(cd "$1" 2>/dev/null && pwd -P)" \
    || { echo "ERROR: bundle dir not found or not enterable: $1" >&2; exit 2; }

# Resolved BEFORE the `cd "$BUNDLE"` below: the shipped-skill checker lives beside
# this script in the repo, which is not part of the bundle.
CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

# Two exe naming conventions exist:
#   * CPP_SUFFIX — for binaries built via cmake `add_executable` (daslang,
#     daslang-live, gen1_to_gen2): platform-natural suffix (.exe on Windows,
#     none on Linux/macOS).
#   * DASEXE_SUFFIX — for binaries built via `daslang -exe`: ALWAYS `.exe` on
#     every platform (utils/CMakeLists.txt: "daslang -exe appends `.exe` to the
#     output path").
# Use `-f` (file-exists) rather than `-x` (executable bit) — Windows Git-Bash
# doesn't see the +x bit on Linux ELF binaries even when this script runs
# locally on a Linux-bundle for cross-platform repro.
if [[ -f "$BUNDLE/bin/daslang.exe" ]]; then
    DASLANG="$BUNDLE/bin/daslang.exe"
    CPP_SUFFIX=".exe"
elif [[ -f "$BUNDLE/bin/daslang" ]]; then
    DASLANG="$BUNDLE/bin/daslang"
    CPP_SUFFIX=""
else
    echo "ERROR: daslang binary not found in $BUNDLE/bin/" >&2
    exit 2
fi
DASEXE_SUFFIX=".exe"

cd "$BUNDLE"

# Source-based tests — `daslang -compile-only` parses + infers + links requires
# without executing. This is what catches install-rule misses: a missing peer
# `.das` file surfaces as `error[20605]: missing prerequisite …`.
#
# Add a row when a new util ships a .das entry point — every shipped tool keeps
# its source form in the bundle, exe or not. The list is intentionally explicit
# (no glob) so removing a util is a deliberate one-line change.
# EVERY installed .das entry point belongs here. The list was previously missing
# five of them, which is how `utils/mcp/setup.das` shipped absent from the bundle
# through 0.6.4 while both the installed README and skills/mcp_tools.md documented
# running it: nothing ever compiled it, so nothing noticed.
COMPILE_TESTS=(
    "aot|utils/aot/main.das"
    "benchctl|utils/benchctl/main.das"
    "das-fmt|utils/das-fmt/dasfmt.das"
    "dascov|utils/dascov/main.das"
    "dasllama-convert|utils/dasllama-convert/main.das"
    "dasllama-server|utils/dasllama-server/main.das"
    "daspkg|utils/daspkg/main.das"
    "detect-dupe|utils/detect-dupe/main.das"
    "fix-lint-errors|utils/fix-lint-errors/main.das"
    "jobque-timeline|utils/jobque-timeline/main.das"
    "lint|utils/lint/main.das"
    "mcp|utils/mcp/main.das"
    "mcp-cpp|utils/mcp/cpp_main.das"
    "mcp-setup|utils/mcp/setup.das"
    # Not an entry point, but the library an adopting repo's REVIEW.das requires
    # (REVIEW_COMMON.md contract) — a bundle where it does not compile breaks
    # every external gate.
    "review-gate|dastest/review_gate.das"
    # In-tree module das layers — these catch the missing-payload class (the
    # dasImgui merge shipped binaries + descriptor but zero .das for a while:
    # the descriptor resolved to files the bundle did not carry).
    "imgui-example|modules/dasImgui/examples/features/button_repeat.das"
    "vulkan-example|modules/dasVulkan/examples/smoke.das"
    "vulkan-tutorial|modules/dasVulkan/tutorials/01_triangle/triangle_tut.das"
)

# Tools intentionally NOT in COMPILE_TESTS:
#   find-dupe — require chain needs the `anthropic/anthropic` daspkg package
#               fetched at runtime + ANTHROPIC_API_KEY.

# Prebuilt exes `cmake --install` drops into bin/. `cpp` rows are add_executable
# targets (platform-natural suffix); `dasexe` rows are the DAS_UTILS_SHIPPED_EXES
# set from utils/CMakeLists.txt (always `.exe`), installed on single-config
# generators — which is every generator the release bundles are cut on.
# `cpp` rows are presence-checked; `dasexe` rows are also launched (`--help`, exit 0):
# a `daslang -exe` binary resolves the runtime .so/.dylib through its embedded rpath,
# and a bundle whose rpath points back at the build tree is present-but-dead on every
# user's box.
SHIPPED_EXE_TESTS=(
    "gen1_to_gen2|cpp"
    "daslang-live|cpp"
    "benchctl|dasexe"
    "dascov|dasexe"
    "das-fmt|dasexe"
    "daspkg|dasexe"
    "dastest|dasexe"
    "detect-dupe|dasexe"
    "lint|dasexe"
)

# Stdio launch test for the mcp JSON-RPC server (run from source via daslang —
# the exe is no longer bundled): the only safe "did it actually start" probe is
# to feed empty stdin and check for a clean exit. It prints "Starting daslang
# MCP server" then "stdin closed, shutting down" within ~1s.

PASS=0
FAIL=0
LOG="$(mktemp)"

# The build tree's lib/ (CMAKE_LIBRARY_OUTPUT_DIRECTORY, beside ci/) is also on every
# dasexe's rpath, so on the runner that built the bundle a build-tree rpath resolves
# and the launch check would pass without the bundle-relative entry. Hide it for the
# run; Windows has no rpath and locks open DLL dirs, so only POSIX.
BUILD_LIB="$(cd "$CI_DIR/.." && pwd -P)/lib"
HIDDEN_LIB=""
if [[ -z "$CPP_SUFFIX" && -d "$BUILD_LIB" ]]; then
    HIDDEN_LIB="$BUILD_LIB.smokehidden"
    mv "$BUILD_LIB" "$HIDDEN_LIB"
fi
restore_and_clean() {
    rm -f "$LOG" "$LOG.suite" "$LOG.lint"
    if [[ -n "$HIDDEN_LIB" && -d "$HIDDEN_LIB" ]]; then mv "$HIDDEN_LIB" "$BUILD_LIB"; fi
}
trap restore_and_clean EXIT

run_check() {
    local label="$1"; shift
    printf '  %-30s ' "$label"
    if "$@" > "$LOG" 2>&1; then
        echo "OK"
        PASS=$((PASS + 1))
    else
        local rc=$?
        echo "FAIL (exit $rc)"
        sed 's/^/      /' "$LOG"
        FAIL=$((FAIL + 1))
    fi
}

echo "==================================================================="
echo "daslang bundle smoke test"
echo "  bundle : $BUNDLE"
echo "  daslang: $DASLANG"
echo "==================================================================="

echo
echo "Source compile (-compile-only):"
for entry in "${COMPILE_TESTS[@]}"; do
    name="${entry%%|*}"
    path="${entry#*|}"
    run_check "$name" "$DASLANG" -compile-only "$path"
done

echo
echo "Prebuilt exes (bin/) - presence, dasexe rows also launched:"
for entry in "${SHIPPED_EXE_TESTS[@]}"; do
    name="${entry%%|*}"
    kind="${entry#*|}"
    case "$kind" in
        dasexe) suffix="$DASEXE_SUFFIX" ;;
        cpp)    suffix="$CPP_SUFFIX" ;;
        *)      echo "ERROR: unknown kind '$kind' for $name" >&2; FAIL=$((FAIL + 1)); continue ;;
    esac
    exe="$BUNDLE/bin/${name}${suffix}"
    if [[ ! -f "$exe" ]]; then
        printf '  %-30s MISSING (%s)\n' "$name" "$exe"
        FAIL=$((FAIL + 1))
    elif [[ "$kind" == dasexe ]]; then
        run_check "$name (launch)" "$exe" --help
    else
        printf '  %-30s OK\n' "$name"
        PASS=$((PASS + 1))
    fi
done

echo
echo "Runtime launch:"
run_check "mcp.das (empty stdin)" bash -c \
    "'$DASLANG' utils/mcp/main.das < /dev/null"

# Two prebuilt tools past --help. dastest.exe must compile and run a shipped suite
# (isolated mode also spawns its own workers); lint.exe over daslib must resolve every
# module native path - its summary line grows ", N skipped" when it cannot, so the
# anchored grep is what rejects a skip.
run_check "dastest.exe runs a shipped suite" bash -c \
    "set -o pipefail; '$BUNDLE/bin/dastest${DASEXE_SUFFIX}' --test utils/common/tests --isolated-mode | tee '$LOG.suite' \
     && grep -Eq '^[1-9][0-9]* tests, [1-9][0-9]* passed, 0 failed, 0 errors' '$LOG.suite'"
run_check "lint.exe lints daslib, no skips" bash -c \
    "set -o pipefail; '$BUNDLE/bin/lint${DASEXE_SUFFIX}' daslib | tee '$LOG.lint' \
     && grep -Eq '^[0-9]+ files, 0 issue\(s\), 0 error\(s\)\$' '$LOG.lint'"

# Shipped skills must not send the reader to a path the bundle does not contain.
# This is the same class of bug as the utils/mcp/setup.das miss: the docs promised
# something the install rules never delivered. Here the right answer is the reverse
# of shipping it -- src/ and tests/ are never going in the bundle -- so the check
# demands the line be marked `repo-only` instead.
echo
echo "Shipped skills:"
# nothing lists the shipped skills any more, so the layout itself is the assertion
printf '  %-30s ' "bundle layout"
if [[ -f "$BUNDLE/skills/daslang/SKILL.md" && -d "$BUNDLE/skills/daslang/references" \
      && ! -d "$BUNDLE/skills/internal" && -f "$BUNDLE/.claude/skills/daslang/SKILL.md" \
      && -f "$BUNDLE/REVIEW_COMMON.md" && -f "$BUNDLE/.claude/agents/dragon.md" ]]; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL (need skills/daslang/{SKILL.md,references/}, .claude/skills/daslang, .claude/agents/dragon.md, REVIEW_COMMON.md, and no skills/internal/)"
    FAIL=$((FAIL + 1))
fi
# No installed file may name a utils/internal/ path -- that class is a shipped
# tutorial/scaffold invoking a tool the bundle does not carry (found live: the
# AOT integration scaffolds). skills/ is excluded here: its own gate above owns
# skills content, with repo-only marker semantics this raw grep cannot honor.
# mcp_supervisor.py is excluded: it PROBES for the in-repo das-herd behind an
# exists-check, so the literal is functional and inert in a bundle.
# CHANGELIST.md is excluded: release history legitimately NAMES the utils/internal
# split; prose there is documentation, not a reference that can dangle.
printf '  %-30s ' "no utils/internal references"
INTERNAL_REFS="$(grep -rIl 'utils/internal' "$BUNDLE" --exclude-dir=skills --exclude=mcp_supervisor.py --exclude=CHANGELIST.md 2>/dev/null || true)"
if [[ -z "$INTERNAL_REFS" ]]; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL"
    printf '%s\n' "$INTERNAL_REFS" | sed 's/^/    /'
    FAIL=$((FAIL + 1))
fi

printf '  %-30s ' "references resolve in bundle"
# Skipped rather than failed when no python3 is present, so this script stays
# runnable on a bare box.
# Probe by EXECUTING, not by `command -v`: Windows ships a WindowsApps python3 shim that
# is on PATH but exits "Permission denied" until the user installs from the Store.
PY=""
for cand in python3 python py; do
    if "$cand" -c "pass" >/dev/null 2>&1; then PY="$cand"; break; fi
done
if [[ -z "$PY" ]]; then
    echo "SKIP (no python3)"
elif SKILL_REFS="$("$PY" "$CI_DIR/check_shipped_skills.py" "$BUNDLE" 2>&1)"; then
    echo "OK"
    PASS=$((PASS + 1))
else
    echo "FAIL"
    printf '%s\n' "$SKILL_REFS" | sed 's/^/    /'
    FAIL=$((FAIL + 1))
fi

echo
echo "==================================================================="
if [[ $FAIL -eq 0 ]]; then
    echo "ALL OK ($PASS checks passed)"
    exit 0
else
    echo "FAILED: $FAIL failure(s), $PASS pass(es)"
    exit 1
fi
