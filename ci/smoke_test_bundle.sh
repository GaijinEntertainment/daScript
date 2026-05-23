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

if [[ -x "$BUNDLE/bin/daslang.exe" ]]; then
    DASLANG="$BUNDLE/bin/daslang.exe"
    EXE_SUFFIX=".exe"
elif [[ -x "$BUNDLE/bin/daslang" ]]; then
    DASLANG="$BUNDLE/bin/daslang"
    EXE_SUFFIX=""
else
    echo "ERROR: daslang binary not found in $BUNDLE/bin/" >&2
    exit 2
fi

cd "$BUNDLE"

# Source-based tests — `daslang -compile-only` parses + infers + links requires
# without executing. This is what catches install-rule misses: a missing peer
# `.das` file surfaces as `error[20605]: missing prerequisite …`.
#
# Add a row when a new util ships a .das entry point. The list is intentionally
# explicit (no glob) so removing a util is a deliberate one-line change.
COMPILE_TESTS=(
    "aot|utils/aot/main.das"
    "benchctl|utils/benchctl/main.das"
    "das-fmt|utils/das-fmt/dasfmt.das"
    "dascov|utils/dascov/main.das"
    "daspkg|utils/daspkg/main.das"
    "detect-dupe|utils/detect-dupe/main.das"
    "hygiene|utils/hygiene/main.das"
    "lint|utils/lint/main.das"
    "mcp|utils/mcp/main.das"
)

# Tools intentionally NOT in COMPILE_TESTS:
#   find-dupe — require chain needs the `anthropic/anthropic` daspkg package
#               fetched at runtime + ANTHROPIC_API_KEY.

# Prebuilt exes that `cmake --install` should drop into bin/ when build flags
# allow it (DAS_LLVM, DAS_SQLITE, DAS_HV gates). Presence-checked only — we
# don't run them with `--help` because daslang itself intercepts `--help` and
# prints its own usage even after the `--` separator, swallowing the script's
# CLI surface.
EXE_PRESENCE_TESTS=(
    "aot"
    "benchctl"
    "das-fmt"
    "dascov"
    "daslang-live"
    "daspkg"
    "dastest"
    "detect-dupe"
    "hygiene"
    "mcp"
)

# Stdio launch test for mcp.exe — it's a JSON-RPC server, so the only safe
# "did it actually start" probe is to feed empty stdin and check for a clean
# exit. The bundled mcp.exe prints "Starting daslang MCP server" then
# "stdin closed, shutting down" within ~1s.

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
for name in "${EXE_PRESENCE_TESTS[@]}"; do
    exe="$BUNDLE/bin/${name}${EXE_SUFFIX}"
    printf '  %-30s ' "$name"
    if [[ -x "$exe" ]]; then
        echo "OK"
        PASS=$((PASS + 1))
    else
        echo "MISSING ($exe)"
        FAIL=$((FAIL + 1))
    fi
done

echo
echo "Runtime launch:"
run_check "mcp.exe (empty stdin)" bash -c \
    "'$BUNDLE/bin/mcp${EXE_SUFFIX}' < /dev/null"
run_check "mcp.das (empty stdin)" bash -c \
    "'$DASLANG' utils/mcp/main.das < /dev/null"

echo
echo "==================================================================="
if [[ $FAIL -eq 0 ]]; then
    echo "ALL OK ($PASS checks passed)"
    exit 0
else
    echo "FAILED: $FAIL failure(s), $PASS pass(es)"
    exit 1
fi
