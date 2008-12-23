#! /usr/bin/env bash
PWD_BACKUP=${PWD}
SCRIPT_DIR=`dirname "${PWD}/$0"`
cd "${SCRIPT_DIR}/.." || exit 1
function fail() { cd "${PWD_BACKUP}" ; exit 1; }
####################################################################


echo ========== cleanup ==========
rm -vf uriparser-*.tar.* uriparser-*.zip 2> /dev/null
rm -vRf uriparser-* 2> /dev/null

echo
echo ========== bootstrap ==========
./autogen.sh -v --download || fail

if [[ ! -f Makefile ]]; then
echo
echo ========== configure ==========
./configure || fail
fi

echo
echo ========== make distcheck ==========
make -j10 distcheck || fail

cat <<'CHECKLIST'

Fine.


Have you
* run ./edit_version.sh
* updated the soname
* updated file lists
  - Makefile.am
  - Code::Blocks
  - Visual Studio 2005
* searched for TODO inside code using
  grep -R 'TODO' include/* lib/* test/*
?

If so ..
* upload release with ReleaseForge
* announce through ..
  - Blog
  - Mailing lists
  - Freshmeat
  - SourceForge news
* upload doc
* update doc on website

CHECKLIST


####################################################################
cd "${PWD_BACKUP}" || fail
exit 0
