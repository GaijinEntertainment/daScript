#!/usr/bin/env bash
# Smoke-test a standalone cpp-mcp bundle: it starts, advertises the cpp tool
# subset, and cpp_status returns a report — all with a clean exit. Run from a
# neutral cwd so getDasRoot() MUST resolve from the exe path (proving the bundle
# is self-contained, not leaning on a sibling daslang tree).
#
# Usage:  bash ci/smoke_test_cpp_mcp.sh <bundle-dir>
# Exit 0 on pass, non-zero with a diagnostic otherwise.

set -u

B="$(cd "${1:?usage: $0 <bundle-dir>}" 2>/dev/null && pwd -P)" \
    || { echo "ERROR: bundle dir not found: ${1:-}" >&2; exit 2; }

if [[ -f "$B/bin/cpp-mcp.exe" ]]; then
    BIN="$B/bin/cpp-mcp.exe"
elif [[ -f "$B/bin/cpp-mcp" ]]; then
    BIN="$B/bin/cpp-mcp"
else
    echo "ERROR: cpp-mcp binary not found in $B/bin/" >&2
    exit 2
fi

REQ="$(mktemp)"; OUT="$(mktemp)"; ERR="$(mktemp)"
trap 'rm -f "$REQ" "$OUT" "$ERR"' EXIT

cat > "$REQ" <<'EOF'
{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2024-11-05","capabilities":{},"clientInfo":{"name":"smoke","version":"1.0"}}}
{"jsonrpc":"2.0","id":2,"method":"tools/list","params":{}}
{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"cpp_status","arguments":{}}}
EOF

echo "==================================================================="
echo "cpp-mcp bundle smoke test"
echo "  bundle : $B"
echo "  binary : $BIN"
echo "==================================================================="

# Neutral cwd: the binary must find its .das sources via getDasRoot() (the
# bundle), not via the current directory.
( cd / && "$BIN" < "$REQ" ) > "$OUT" 2> "$ERR"
rc=$?

tools=$(grep -oE '"name":"(cpp_[a-z_]+|grep_usage|outline|shutdown)"' "$OUT" | sort -u | wc -l | tr -d ' ')
echo "exit code        : $rc"
echo "tools advertised : $tools"

fail=0
[[ $rc -eq 0 ]]                                  || { echo "FAIL: non-zero exit ($rc)"; fail=1; }
grep -q '"name":"cpp_status"' "$OUT"             || { echo "FAIL: cpp_status not advertised"; fail=1; }
grep -q '"name":"cpp_compile_check"' "$OUT"      || { echo "FAIL: cpp_compile_check not advertised"; fail=1; }
grep -q 'cpp-mcp server status' "$OUT"           || { echo "FAIL: cpp_status call returned no report"; fail=1; }
# The compile-DB / status tools are always on; ast-grep tools are env-gated.
[[ ${tools:-0} -ge 4 ]]                          || { echo "FAIL: too few tools ($tools, expected >= 4)"; fail=1; }

if [[ $fail -ne 0 ]]; then
    echo "--- server stderr ---"; tail -20 "$ERR"
    echo "--- first response bytes ---"; head -c 400 "$OUT"; echo
    echo "RESULT: FAIL"
    exit 1
fi

echo "RESULT: PASS"
