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
#     daslang-live, das-fmt): platform-natural suffix (.exe on Windows,
#     none on Linux/macOS). These are the ONLY binaries the bundle contains,
#     because only `add_executable` targets can carry an install(TARGETS) rule.
#   * DASEXE_SUFFIX — for binaries built via `daslang -exe` (aot, benchctl,
#     dascov, daspkg, dastest, detect-dupe, hygiene, jit, lint, mcp): ALWAYS
#     `.exe` on every platform (utils/CMakeLists.txt: "daslang -exe appends
#     `.exe` to the output path"). utils/CMakeLists.txt builds these with
#     add_custom_command into <repo>/bin, so install(TARGETS) cannot reach them
#     and NONE of them ship — the bundle runs each from source instead
#     (`daslang utils/<name>/main.das`), which is what COMPILE_TESTS covers.
#     The suffix is kept here for the `dasexe` row kind below.
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
# Add a row when a new util ships a .das entry point. The list is intentionally
# explicit (no glob) so removing a util is a deliberate one-line change.
# EVERY installed .das entry point belongs here. The list was previously missing
# five of them, which is how `utils/mcp/setup.das` shipped absent from the bundle
# through 0.6.4 while both the installed README and skills/mcp_tools.md documented
# running it: nothing ever compiled it, so nothing noticed.
COMPILE_TESTS=(
    "aot|utils/aot/main.das"
    "benchctl|utils/benchctl/main.das"
    "das-fmt|utils/das-fmt/dasfmt.das"
    "dascov|utils/dascov/main.das"
    "daspkg|utils/daspkg/main.das"
    "detect-dupe|utils/detect-dupe/main.das"
    "fix-lint-errors|utils/fix-lint-errors/main.das"
    "hygiene|utils/hygiene/main.das"
    "jit|utils/jit/main.das"
    "lint|utils/lint/main.das"
    "mcp|utils/mcp/main.das"
    "mcp-cpp|utils/mcp/cpp_main.das"
    "mcp-setup|utils/mcp/setup.das"
    "requirefix|utils/requirefix/main.das"
)

# Tools intentionally NOT in COMPILE_TESTS:
#   find-dupe — require chain needs the `anthropic/anthropic` daspkg package
#               fetched at runtime + ANTHROPIC_API_KEY.

# Prebuilt exes `cmake --install` drops into bin/. Only cmake add_executable
# targets qualify (see the DASEXE_SUFFIX note above — the `daslang -exe` utils are
# custom-command outputs and never install), so every row here is kind `cpp`;
# `dasexe` stays supported for the day one of them gains an install(PROGRAMS) rule.
# Presence-checked only — we don't run them with `--help` because daslang itself
# intercepts `--help` and prints its own usage even after the `--` separator,
# swallowing the script's CLI surface.
EXE_PRESENCE_TESTS=(
    "das-fmt|cpp"
    "daslang-live|cpp"
)

# Stdio launch test for the mcp JSON-RPC server (run from source via daslang —
# the exe is no longer bundled): the only safe "did it actually start" probe is
# to feed empty stdin and check for a clean exit. It prints "Starting daslang
# MCP server" then "stdin closed, shutting down" within ~1s.

PASS=0
FAIL=0
LOG="$(mktemp)"
trap 'rm -f "$LOG"' EXIT

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
echo "Prebuilt exe presence (bin/):"
for entry in "${EXE_PRESENCE_TESTS[@]}"; do
    name="${entry%%|*}"
    kind="${entry#*|}"
    case "$kind" in
        dasexe) suffix="$DASEXE_SUFFIX" ;;
        cpp)    suffix="$CPP_SUFFIX" ;;
        *)      echo "ERROR: unknown kind '$kind' for $name" >&2; FAIL=$((FAIL + 1)); continue ;;
    esac
    exe="$BUNDLE/bin/${name}${suffix}"
    printf '  %-30s ' "$name"
    if [[ -f "$exe" ]]; then
        echo "OK"
        PASS=$((PASS + 1))
    else
        echo "MISSING ($exe)"
        FAIL=$((FAIL + 1))
    fi
done

echo
echo "Runtime launch:"
run_check "mcp.das (empty stdin)" bash -c \
    "'$DASLANG' utils/mcp/main.das < /dev/null"

# Shipped skills must not send the reader to a path the bundle does not contain.
# This is the same class of bug as the utils/mcp/setup.das miss: the docs promised
# something the install rules never delivered. Here the right answer is the reverse
# of shipping it -- src/ and tests/ are never going in the bundle -- so the check
# demands the line be marked `repo-only` instead.
echo
echo "Shipped skills:"
printf '  %-30s ' "references resolve in bundle"
# The ship list is read from the REPO manifest, not the bundle, so a skill sitting on disk
# but missing from install/skills.list is still caught. Skipped rather than failed when no
# python3 is present, so this script stays runnable on a bare box.
# Probe by EXECUTING, not by `command -v`: Windows ships a WindowsApps python3 shim that
# is on PATH but exits "Permission denied" until the user installs from the Store.
PY=""
for cand in python3 python py; do
    if "$cand" -c "pass" >/dev/null 2>&1; then PY="$cand"; break; fi
done
if [[ -z "$PY" ]]; then
    echo "SKIP (no python3)"
elif SKILL_REFS="$("$PY" "$CI_DIR/check_shipped_skills.py" "$BUNDLE" \
                        "$CI_DIR/../install/skills.list" 2>&1)"; then
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
