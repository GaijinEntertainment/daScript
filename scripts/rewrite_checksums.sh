#! /usr/bin/env bash
(
cd $(dirname $(which "$0"))/.. || exit 1
echo "Writing checksums..."

rm -f MD5SUMS SHA1SUMS
for i in uriparser-*.{tar.{bz2,gz,lzma},zip} ; do
	[ -f "${i}" ] || continue
	echo "${i}"
	md5sum "${i}" >> MD5SUMS
	sha1sum "${i}" >> SHA1SUMS
done

cd doc
rm -f MD5SUMS SHA1SUMS
for i in uriparser-doc-*.{chm,qch,zip} ; do
	[ -f "${i}" ] || continue
	echo "${i}"
	md5sum "${i}" >> MD5SUMS
	sha1sum "${i}" >> SHA1SUMS
done

)
exit $?
