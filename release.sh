#! /bin/sh

INSTALL_DIR="${HOME}/install"
PREFIX="--prefix=${INSTALL_DIR}"
MAKE_PARAMS=-j10



echo ========== cleanup ==========
rm uriparser-*.tar.* uriparser-*.zip &> /dev/null
make clean &> /dev/null
make distclean &> /dev/null

echo
echo ========== prepare ==========
aclocal || exit 1
automake || exit 1
autoconf || exit 1

echo
echo ========== configure ==========
./configure ${PREFIX} || exit 1

echo
echo ========== make uninstall ==========
make uninstall || exit 1

echo
echo ========== make ==========
make ${MAKE_PARAMS} || exit 1



echo
echo ========== make check ==========
make check || exit 1



echo
echo ========== make install ==========
make install || exit 1



echo
echo ========== make distcheck ==========
make distcheck || exit 1



echo
echo Fine.

echo
echo <<'CHECKLIST'
* run ./edit_version.sh
* updated the soname
* updated file lists
  - Makefile.am
  - Code::Blocks
  - Visual Studio 2005
* searched for TODO inside code using
  grep -R "TODO" include/* src/* test/*
?
CHECKLIST
