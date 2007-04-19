#!/bin/bash

INSTALL_DIR="${HOME}/install"
PREFIX="--prefix=${INSTALL_DIR}"
## PREFIX=""

MAKE_PARAMS="-j10"
## MAKE_PARAMS=""

## MY_ACLOCAL="/usr/bin/aclocal"
MY_ACLOCAL="aclocal"
## MY_AUTOCONF="/usr/bin/autoconf"
MY_AUTOCONF="autoconf"
## MY_AUTOMAKE="/usr/bin/automake"
MY_AUTOMAKE="automake"



echo ========== cleanup ==========
rm uriparser-*.tar.* uriparser-*.zip &> /dev/null
make clean &> /dev/null
make distclean &> /dev/null

echo
echo ========== prepare ==========
${MY_ACLOCAL} \
&& ${MY_AUTOMAKE} \
&& ${MY_AUTOCONF} \
&& echo \
&& echo ========== configure ========== \
&& ./configure ${PREFIX} \
&& echo \
&& echo ========== make uninstall ========== \
&& make uninstall \
&& echo \
&& echo ========== make ========== \
&& make ${MAKE_PARAMS} \
\
\
\
&& echo \
&& echo ========== make check ========== \
&& make check \
\
\
\
&& echo \
&& echo ========== make install ========== \
&& make install \
\
\
\
&& echo \
&& echo ========== make dist ========== \
&& make dist \
\
\
\
&& echo \
&& echo Fine.
