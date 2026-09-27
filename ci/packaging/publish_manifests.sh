#!/usr/bin/env bash
# Render a package's Homebrew and scoop manifests for a release and push them to the tap and
# bucket repos (under github.com/$REPO_OWNER), or show the diff without pushing.
# Usage: publish_manifests.sh <package> <tag> [--push]
# Needs: gh (reads the release's .sha256 assets), git, python3. --push needs PACKAGING_TOKEN,
# a token that can push to both repos. Without --push nothing leaves the runner.
set -euo pipefail

PACKAGE="$1"
TAG="$2"
PUSH="${3:-}"
REPO_OWNER="borisbat"
SOURCE_REPO="${GITHUB_REPOSITORY:-GaijinEntertainment/daScript}"
HERE="$(cd "$(dirname "$0")" && pwd -P)"

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

mkdir -p "$WORK/sha"
gh release download "$TAG" --repo "$SOURCE_REPO" --pattern '*.sha256' --dir "$WORK/sha"
python3 "$HERE/render_manifests.py" "$PACKAGE" "$TAG" "$WORK/sha" "$WORK/out"

for repo_dir in "$WORK/out"/*; do
    repo="$(basename "$repo_dir")"
    if [ "$PUSH" = "--push" ]; then
        [ -n "${PACKAGING_TOKEN:-}" ] || { echo "publish_manifests: PACKAGING_TOKEN is not set" >&2; exit 2; }
        url="https://x-access-token:${PACKAGING_TOKEN}@github.com/$REPO_OWNER/$repo.git"
    else
        url="https://github.com/$REPO_OWNER/$repo.git"
    fi
    git clone --quiet --depth 1 "$url" "$WORK/clone-$repo"
    cp -R "$repo_dir"/. "$WORK/clone-$repo/"
    cd "$WORK/clone-$repo"
    git add -A
    if git diff --cached --quiet; then
        echo "$repo: already at $PACKAGE @ $TAG"
        cd - > /dev/null
        continue
    fi
    git diff --cached --stat
    if [ "$PUSH" != "--push" ]; then
        git diff --cached
        cd - > /dev/null
        continue
    fi
    git -c user.name="daslang release" -c user.email="team@daslang.io" commit --quiet -m "$PACKAGE manifests @ $TAG"
    # the daslang and dasllama release workflows push to the same repos minutes apart
    for attempt in 1 2 3; do
        if git push --quiet origin HEAD; then break; fi
        [ "$attempt" = 3 ] && { echo "$repo: push failed three times" >&2; exit 1; }
        git pull --quiet --rebase origin HEAD
    done
    echo "$repo: pushed $PACKAGE @ $TAG"
    cd - > /dev/null
done
