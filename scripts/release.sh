#! /usr/bin/env bash
PWD_BACKUP=${PWD}
SCRIPT_DIR=`dirname "${PWD}/$0"`
cd "${SCRIPT_DIR}/.." || exit 1
function fail() { cd "${PWD_BACKUP}" ; exit 1; }
####################################################################

INSTALL_DIR="${HOME}/install"
PREFIX="--prefix=${INSTALL_DIR}"
MAKE_PARAMS=-j10



echo ========== cleanup ==========
rm uriparser-*.tar.* uriparser-*.zip &> /dev/null
make clean &> /dev/null
make distclean &> /dev/null

echo
echo ========== doc ==========
cd doc
./bootstrap.sh || fail
cd -

echo
echo ========== bootstrap ==========
./bootstrap.sh || fail

echo
echo ========== configure ==========
./configure ${PREFIX} || fail

echo
echo ========== make uninstall ==========
make uninstall || fail

echo
echo ========== make ==========
make ${MAKE_PARAMS} || fail



echo
echo ========== make check ==========
make check || fail



echo
echo ========== make install ==========
make install || fail



echo
echo ========== make distcheck ==========
make distcheck || fail



echo
echo Fine.

echo
cat <<'CHECKLIST'
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
CHECKLIST

####################################################################
cd "${PWD_BACKUP}" || fail
exit 0

