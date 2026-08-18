#!/usr/bin/env bash
# Build a .deb from an installed daslang bundle directory.
# Usage: deb_build.sh <bundle_dir> <tag_or_version> <out_dir>
# The tag is normalized to a dpkg version: leading v stripped, -RCn -> ~rcn
# (tilde sorts BEFORE the release, so 0.6.4~rc1 upgrades cleanly to 0.6.4).
set -euo pipefail

BUNDLE="$1"
RAW_VERSION="${2:-0.0.0-dev}"
OUT="$3"

VERSION="${RAW_VERSION#v}"
VERSION="$(echo "$VERSION" | sed -E 's/-[Rr][Cc]([0-9]+)/~rc\1/')"
ARCH="$(dpkg --print-architecture)"

ROOT="$(mktemp -d)"
trap 'rm -rf "$ROOT"' EXIT
PKG="$ROOT/daslang_${VERSION}_${ARCH}"

mkdir -p "$PKG/opt/daslang" "$PKG/usr/bin" "$PKG/DEBIAN"
cp -a "$BUNDLE"/. "$PKG/opt/daslang/"

# the user-facing binaries; everything else is reached relative to /opt/daslang
for exe in daslang daslang-live gen1_to_gen2; do
    if [ -x "$PKG/opt/daslang/bin/$exe" ]; then
        ln -s "/opt/daslang/bin/$exe" "$PKG/usr/bin/$exe"
    fi
done

INSTALLED_SIZE=$(du -sk "$PKG/opt" | cut -f1)
cat > "$PKG/DEBIAN/control" << EOF
Package: daslang
Version: $VERSION
Section: devel
Priority: optional
Architecture: $ARCH
Installed-Size: $INSTALLED_SIZE
Maintainer: daslang maintainers <team@daslang.io>
Homepage: https://daslang.io
Description: daslang programming language SDK
 High-performance statically-typed scripting language for games and
 real-time applications: compiler, JIT, standard library, test framework,
 tools, and modules. Installed under /opt/daslang.
EOF

dpkg-deb --build --root-owner-group "$PKG" "$OUT/daslang_${VERSION}_${ARCH}.deb"
echo "built: $OUT/daslang_${VERSION}_${ARCH}.deb"
