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

# The GitHub API answers a 5xx now and then; one bad response must not red a three-hour nightly.
retry() {
    local attempt=1 delay=5
    until "$@"; do
        [ "$attempt" -lt 5 ] || return 1
        echo "[docs_assets] attempt $attempt failed, retrying in ${delay}s" >&2
        sleep "$delay"
        attempt=$((attempt + 1))
        delay=$((delay * 2))
    done
}

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/../../.." && pwd)"
dest="$repo_root/doc/source/_static/tutorials"

mkdir -p "$dest"

# A staged file's mtime is when it was downloaded, so the release held its then-current bytes at
# that moment: an asset uploaded since reads newer, and nothing else does. One metadata call
# answers it for the whole set, and a staged set that is already current costs no download.
remote="$(retry gh release view docs-assets \
    --repo GaijinEntertainment/daScript \
    --json assets \
    -q '.assets[] | select(.name | endswith(".mp4")) | "\(.name) \(.updatedAt)"')"

epoch_of() {  # GNU coreutils on CI, BSD on a developer's mac
    date -u -d "$1" +%s 2>/dev/null || date -u -j -f '%Y-%m-%dT%H:%M:%SZ' "$1" +%s
}
mtime_of() {
    stat -c %Y "$1" 2>/dev/null || stat -f %m "$1"
}

stale=0
while read -r name updated; do
    [ -n "$name" ] || continue
    file="$dest/$name"
    if [ ! -f "$file" ] || [ "$(epoch_of "$updated")" -gt "$(mtime_of "$file")" ]; then
        stale=1
        break
    fi
done <<< "$remote"

if [ "$stale" -eq 0 ]; then
    echo "[docs_assets] $(ls "$dest"/*.mp4 2>/dev/null | wc -l) MP4s already current in $dest"
    exit 0
fi

retry gh release download docs-assets \
    --repo GaijinEntertainment/daScript \
    --pattern '*.mp4' \
    --dir "$dest" \
    --clobber
echo "[docs_assets] staged $(ls "$dest"/*.mp4 2>/dev/null | wc -l) MP4s into $dest"
