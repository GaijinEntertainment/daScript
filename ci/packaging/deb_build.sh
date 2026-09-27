#!/usr/bin/env bash
# Build a .deb from a release bundle directory.
# Usage: deb_build.sh [--package NAME] <bundle_dir> <tag_or_version> <out_dir>
#        deb_build.sh --control-only [--package NAME] <bundle_dir> <tag_or_version>   (control + links to stdout, no dpkg-deb)
# NAME is a profile in packages.py (default daslang): the install prefix, the commands
# linked into /usr/bin, the declared dependencies.
# The tag is normalized to a dpkg version: leading v stripped, -RCn -> ~rcn
# (tilde sorts BEFORE the release, so 0.6.4~rc1 upgrades cleanly to 0.6.4).
set -euo pipefail

CONTROL_ONLY=0
PACKAGE=daslang
while [ $# -gt 0 ]; do
    case "$1" in
        --control-only) CONTROL_ONLY=1; shift ;;
        --package) PACKAGE="$2"; shift 2 ;;
        *) break ;;
    esac
done
BUNDLE="$(cd "$1" && pwd -P)"
RAW_VERSION="${2:-0.0.0-dev}"
OUT="${3:-}"

PROFILE="$(python3 "$(dirname "$0")/packages.py" shell "$PACKAGE" "$BUNDLE" "$RAW_VERSION")"
eval "$PROFILE"
VERSION="$PKG_DEB_VERSION"

emit_control() {
    echo "Package: $PKG_NAME"
    echo "Version: $VERSION"
    echo "Section: $PKG_DEB_SECTION"
    echo "Priority: optional"
    echo "Architecture: $ARCH"
    echo "Installed-Size: $1"
    [ -z "$PKG_DEB_DEPENDS" ] || echo "Depends: $PKG_DEB_DEPENDS"
    echo "Maintainer: daslang maintainers <team@daslang.io>"
    echo "Homepage: https://daslang.io"
    echo "Description: $PKG_SUMMARY"
    # continuation lines of a control field start with a space
    echo "$PKG_DESCRIPTION" | sed 's/^/ /'
}

if [ "$CONTROL_ONLY" = 1 ]; then
    ARCH=all
    emit_control 0
    for pair in ${PKG_LINKS[@]+"${PKG_LINKS[@]}"}; do
        echo "link: /usr/bin/${pair%%=*} -> $PKG_PREFIX/${pair#*=}"
    done
    exit 0
fi

[ -n "$OUT" ] || { echo "usage: $0 [--package NAME] <bundle_dir> <tag_or_version> <out_dir>" >&2; exit 2; }
mkdir -p "$OUT"
ARCH="$(dpkg --print-architecture)"

ROOT="$(mktemp -d)"
trap 'rm -rf "$ROOT"' EXIT
PKG="$ROOT/${PKG_NAME}_${VERSION}_${ARCH}"

mkdir -p "$PKG$PKG_PREFIX" "$PKG/usr/bin" "$PKG/DEBIAN"
cp -a "$BUNDLE"/. "$PKG$PKG_PREFIX/"

# the user-facing binaries; everything else is reached relative to the prefix
for pair in ${PKG_LINKS[@]+"${PKG_LINKS[@]}"}; do
    ln -s "$PKG_PREFIX/${pair#*=}" "$PKG/usr/bin/${pair%%=*}"
done

emit_control "$(du -sk "$PKG$PKG_PREFIX" | cut -f1)" > "$PKG/DEBIAN/control"

dpkg-deb --build --root-owner-group "$PKG" "$OUT/${PKG_NAME}_${VERSION}_${ARCH}.deb"
echo "built: $OUT/${PKG_NAME}_${VERSION}_${ARCH}.deb"
