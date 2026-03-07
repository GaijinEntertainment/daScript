#!/bin/bash
# Batch test .das files with tree-sitter parse, in parallel, with timing.
# Usage: ./test_batch.sh [directory...]
# Defaults to ../tutorials ../tests ../examples ../benchmarks

set -euo pipefail

DIRS=("${@:-../tutorials ../tests ../examples ../benchmarks}")
if [ $# -eq 0 ]; then
  DIRS=(../tutorials ../tests ../examples ../benchmarks)
fi

TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

# Collect all .das files
find "${DIRS[@]}" -name '*.das' > "$TMPDIR/files.txt"
TOTAL=$(wc -l < "$TMPDIR/files.txt")
echo "Testing $TOTAL files across: ${DIRS[*]}"
echo ""

# Parse each file, record timing and status
parse_one() {
  local f="$1"
  local outfile="$2"
  local start=$(date +%s%N 2>/dev/null || python3 -c "import time; print(int(time.time()*1e9))")
  local result
  result=$(npx tree-sitter parse "$f" 2>&1)
  local end=$(date +%s%N 2>/dev/null || python3 -c "import time; print(int(time.time()*1e9))")
  local ms=$(( (end - start) / 1000000 ))
  local status="OK"
  if echo "$result" | grep -q "ERROR\|MISSING"; then
    status="FAIL"
  fi
  echo "$status $ms $f" >> "$outfile"
}

export -f parse_one

# Run in parallel (8 jobs)
cat "$TMPDIR/files.txt" | xargs -P 8 -I{} bash -c 'parse_one "$@"' _ {} "$TMPDIR/results.txt"

# Sort and display results
echo "=== FAILURES ==="
grep "^FAIL" "$TMPDIR/results.txt" | sort -t' ' -k3 || echo "(none)"
echo ""

echo "=== SLOWEST FILES (top 20) ==="
sort -t' ' -k2 -rn "$TMPDIR/results.txt" | head -20
echo ""

# Summary
FAIL_COUNT=$(grep -c "^FAIL" "$TMPDIR/results.txt" || true)
OK_COUNT=$(grep -c "^OK" "$TMPDIR/results.txt" || true)
echo "=== SUMMARY ==="
echo "Total: $TOTAL  OK: $OK_COUNT  FAIL: $FAIL_COUNT  Success: $(( OK_COUNT * 100 / TOTAL ))%"
