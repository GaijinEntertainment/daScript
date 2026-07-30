#!/usr/bin/env bash
# Stage the docs-assets release MP4s into doc/source/_static/tutorials/.
#
# Tutorial recordings are NOT in git (51 MB of MP4s would tax every clone);
# they live as assets on the rolling `docs-assets` GitHub release. Docs builds
# (doc.yml, pages.yml) and local sphinx runs call this before sphinx — the
# `.. video::` directive fails a -W build on any recording that is not staged.
#
# Re-record workflow: modules/dasImgui/tests/rerecord_all.ps1 regenerates APNGs, ffmpeg
# converts to MP4, then `gh release upload docs-assets *.mp4 --clobber`.
#
# Requires: gh (authenticated in CI via GH_TOKEN; the repo is public, so any
# token works). --repo is pinned so forks and worktrees fetch the canonical set.

set -eu

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/../.." && pwd)"
dest="$repo_root/doc/source/_static/tutorials"

mkdir -p "$dest"
gh release download docs-assets \
    --repo GaijinEntertainment/daScript \
    --pattern '*.mp4' \
    --dir "$dest" \
    --clobber
echo "[docs_assets] staged $(ls "$dest"/*.mp4 2>/dev/null | wc -l) MP4s into $dest"
