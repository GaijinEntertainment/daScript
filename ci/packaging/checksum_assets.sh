#!/usr/bin/env bash
# Write `<asset>.sha256` beside every asset in a directory (existing .sha256 files skipped), in
# the `<hex>  <name>` form render_manifests.py reads back.
# Usage: checksum_assets.sh <dir> [<asset>...]   (no assets named: every file in <dir>)
set -euo pipefail

DIR="$1"
shift
cd "$DIR"
if [ $# -eq 0 ]; then set -- *; fi
for f in "$@"; do
    case "$f" in *.sha256) continue ;; esac
    (sha256sum "$f" 2>/dev/null || shasum -a 256 "$f") > "$f.sha256"
done
cat -- *.sha256
