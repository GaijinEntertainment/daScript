#!/usr/bin/env bash
# Assemble a standalone cpp-mcp bundle from a built daslang tree.
#
# The AOT cpp-mcp binary still compiles cpp_main.das (+ daslib) at startup
# (AOT only swaps interpreter SimNodes for the linked native code), so the
# bundle ships the .das sources alongside the binary. getDasRoot() derives the
# root from the exe path, so the binary MUST live at <bundle>/bin/ — the rest of
# the layout mirrors the source tree (utils/mcp, utils/common, daslib).
#
# Usage:  bash ci/make_cpp_mcp_bundle.sh <out-bundle-dir>
# Run from anywhere; paths resolve relative to the repo root. The cpp-mcp target
# must already be built (bin/cpp-mcp[.exe] present).

set -eu

OUT="${1:?usage: $0 <out-bundle-dir>}"
ROOT="$(cd "$(dirname "$0")/.." && pwd -P)"
cd "$ROOT"

if [[ -f bin/cpp-mcp.exe ]]; then
    BIN=bin/cpp-mcp.exe
elif [[ -f bin/cpp-mcp ]]; then
    BIN=bin/cpp-mcp
else
    echo "ERROR: bin/cpp-mcp[.exe] not found — build the cpp-mcp target first" >&2
    exit 2
fi

rm -rf "$OUT"
mkdir -p "$OUT/bin" "$OUT/utils/mcp/tools" "$OUT/utils/common"

cp "$BIN"                              "$OUT/bin/"
cp utils/mcp/*.das                     "$OUT/utils/mcp/"
cp utils/mcp/tools/*.das               "$OUT/utils/mcp/tools/"
cp utils/common/git_signature.das      "$OUT/utils/common/"
cp -r daslib                           "$OUT/daslib"
# Windows vcvars launcher (no-op on Linux/macOS — clang/gcc find system headers)
cp utils/mcp/daslang-mcp-msvc.cmd      "$OUT/utils/mcp/" 2>/dev/null || true
# Setup guide -> bundle README
cp utils/mcp/cpp-mcp-setup.md          "$OUT/README.md" 2>/dev/null || true

echo "cpp-mcp bundle assembled at: $OUT"
du -sh "$OUT" 2>/dev/null || true
