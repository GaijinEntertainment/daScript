#!/usr/bin/env bash
# Build an .rpm from an installed daslang bundle directory.
# Usage: rpm_build.sh <bundle_dir> <tag_or_version> <out_dir>
#        rpm_build.sh --spec-only <bundle_dir> <tag_or_version>   (spec to stdout, no rpmbuild)
# The tag is normalized to an rpm version: leading v stripped, -RCn -> ~rcn
# (tilde sorts BEFORE the release, so 0.6.5~rc1 upgrades cleanly to 0.6.5).
# The package mirrors the .deb: everything under /opt/daslang, the user-facing
# binaries symlinked into /usr/bin, no declared dependencies.
set -euo pipefail

SPEC_ONLY=0
if [ "${1:-}" = "--spec-only" ]; then
    SPEC_ONLY=1
    shift
fi
BUNDLE="$(cd "$1" && pwd -P)"
RAW_VERSION="${2:-0.0.0-dev}"
OUT="${3:-}"

VERSION="${RAW_VERSION#v}"
VERSION="$(echo "$VERSION" | sed -E 's/-[Rr][Cc]([0-9]+)/~rc\1/')"
VERSION="${VERSION//-/_}"

# the user-facing binaries; everything else is reached relative to /opt/daslang
LINKS=()
for exe in daslang daslang-live; do
    if [ -x "$BUNDLE/bin/$exe" ]; then
        LINKS+=("$exe")
    fi
done

emit_spec() {
    cat << SPEC
# The bundle is prebuilt and already stripped: no debuginfo, no post-install
# rewriting of its files, no dependency scan (the .deb declares none either).
%global debug_package %{nil}
%global __os_install_post %{nil}
%global __brp_mangle_shebangs %{nil}
%global _build_id_links none
%undefine __brp_check_rpaths
%undefine _missing_build_ids_terminate_build

Name: daslang
Version: $VERSION
Release: 1
Summary: daslang programming language SDK
License: BSD-3-Clause
URL: https://daslang.io
AutoReqProv: no

%description
High-performance statically-typed scripting language for games and
real-time applications: compiler, JIT, standard library, test framework,
tools, and modules. Installed under /opt/daslang.

%install
mkdir -p %{buildroot}/opt/daslang %{buildroot}/usr/bin
cp -a "$BUNDLE"/. %{buildroot}/opt/daslang/
SPEC
    for exe in ${LINKS[@]+"${LINKS[@]}"}; do
        echo "ln -s /opt/daslang/bin/$exe %{buildroot}/usr/bin/$exe"
    done
    cat << SPEC

%files
/opt/daslang
SPEC
    for exe in ${LINKS[@]+"${LINKS[@]}"}; do
        echo "/usr/bin/$exe"
    done
}

if [ "$SPEC_ONLY" = 1 ]; then
    emit_spec
    exit 0
fi

[ -n "$OUT" ] || { echo "usage: $0 <bundle_dir> <tag_or_version> <out_dir>" >&2; exit 2; }
mkdir -p "$OUT"
OUT="$(cd "$OUT" && pwd -P)"
ARCH="$(rpm --eval '%{_arch}')"

TOP="$(mktemp -d)"
trap 'rm -rf "$TOP"' EXIT
mkdir -p "$TOP/SPECS" "$TOP/RPMS" "$TOP/BUILD" "$TOP/BUILDROOT"
emit_spec > "$TOP/SPECS/daslang.spec"

rpmbuild -bb --quiet --define "_topdir $TOP" --target "$ARCH" "$TOP/SPECS/daslang.spec"
RPM="$(find "$TOP/RPMS" -name '*.rpm' | head -1)"
cp "$RPM" "$OUT/"
echo "built: $OUT/$(basename "$RPM")"
