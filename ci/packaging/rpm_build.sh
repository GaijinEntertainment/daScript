#!/usr/bin/env bash
# Build an .rpm from a release bundle directory.
# Usage: rpm_build.sh [--package NAME] <bundle_dir> <tag_or_version> <out_dir>
#        rpm_build.sh --spec-only [--package NAME] <bundle_dir> <tag_or_version>   (spec to stdout, no rpmbuild)
# NAME is a profile in packages.py (default daslang): the install prefix, the commands
# linked into /usr/bin, the declared dependencies.
# The tag is normalized to an rpm version: leading v stripped, -RCn -> ~rcn
# (tilde sorts BEFORE the release, so 0.6.5~rc1 upgrades cleanly to 0.6.5).
# The package mirrors the .deb: the bundle under the prefix, the user-facing
# binaries symlinked into /usr/bin.
set -euo pipefail

SPEC_ONLY=0
PACKAGE=daslang
while [ $# -gt 0 ]; do
    case "$1" in
        --spec-only) SPEC_ONLY=1; shift ;;
        --package) PACKAGE="$2"; shift 2 ;;
        *) break ;;
    esac
done
BUNDLE="$(cd "$1" && pwd -P)"
RAW_VERSION="${2:-0.0.0-dev}"
OUT="${3:-}"

PROFILE="$(python3 "$(dirname "$0")/packages.py" shell "$PACKAGE" "$BUNDLE" "$RAW_VERSION")"
eval "$PROFILE"
VERSION="$PKG_RPM_VERSION"

emit_spec() {
    cat << SPEC
# The bundle is prebuilt and already stripped: no debuginfo, no post-install
# rewriting of its files, no dependency scan (the requirements are declared).
%global debug_package %{nil}
%global __os_install_post %{nil}
%global __brp_mangle_shebangs %{nil}
%global _build_id_links none
%undefine __brp_check_rpaths
%undefine _missing_build_ids_terminate_build

Name: $PKG_NAME
Version: $VERSION
Release: 1
Summary: $PKG_SUMMARY
License: BSD-3-Clause
URL: https://daslang.io
AutoReqProv: no
SPEC
    for req in ${PKG_RPM_REQUIRES[@]+"${PKG_RPM_REQUIRES[@]}"}; do
        echo "Requires: $req"
    done
    cat << SPEC

%description
$PKG_DESCRIPTION

%install
mkdir -p %{buildroot}$PKG_PREFIX %{buildroot}/usr/bin
cp -a "$BUNDLE"/. %{buildroot}$PKG_PREFIX/
SPEC
    for pair in ${PKG_LINKS[@]+"${PKG_LINKS[@]}"}; do
        echo "ln -s $PKG_PREFIX/${pair#*=} %{buildroot}/usr/bin/${pair%%=*}"
    done
    cat << SPEC

%files
$PKG_PREFIX
SPEC
    for pair in ${PKG_LINKS[@]+"${PKG_LINKS[@]}"}; do
        echo "/usr/bin/${pair%%=*}"
    done
}

if [ "$SPEC_ONLY" = 1 ]; then
    emit_spec
    exit 0
fi

[ -n "$OUT" ] || { echo "usage: $0 [--package NAME] <bundle_dir> <tag_or_version> <out_dir>" >&2; exit 2; }
mkdir -p "$OUT"
OUT="$(cd "$OUT" && pwd -P)"
ARCH="$(rpm --eval '%{_arch}')"

TOP="$(mktemp -d)"
trap 'rm -rf "$TOP"' EXIT
mkdir -p "$TOP/SPECS" "$TOP/RPMS" "$TOP/BUILD" "$TOP/BUILDROOT"
emit_spec > "$TOP/SPECS/$PKG_NAME.spec"

rpmbuild -bb --quiet --define "_topdir $TOP" --target "$ARCH" "$TOP/SPECS/$PKG_NAME.spec"
RPM="$(find "$TOP/RPMS" -name '*.rpm' | head -1)"
cp "$RPM" "$OUT/"
echo "built: $OUT/$(basename "$RPM")"
