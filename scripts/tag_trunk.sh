#! /usr/bin/env bash
[ x$1 = x ] && { echo "USAGE: $0 <version>" ; exit 1 ; }

version=$1
base="https://uriparser.svn.sourceforge.net/svnroot/uriparser/uriparser"
src="${base}/trunk/"
dst="${base}/tags/uriparser-${version}"
msg="uriparser ${version}"

echo "# svn cp -m \"${msg}\" \\"
echo "#   \"${src}\" \\"
echo "#   \"${dst}\" \\"
echo

printf 'Shall I run this command? [y/N] '
read -s line
[ "${line}" = y ] || exit 0
echo

echo "Running..."
svn cp -m "$msg" "$src" "$dst"
exit $?
