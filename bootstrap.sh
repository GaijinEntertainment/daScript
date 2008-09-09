#! /bin/bash
CUR=0
COUNT=5

step() {
	PERC=$((100 * CUR / COUNT))
	printf "[%3i%%] %s" ${PERC} "$1"
	echo
	CUR=$((CUR + 1))
}

## Aclocal
ACLOCAL_VERSION=`aclocal --version | head -1 | grep -o '[^ ]*$'`
step "aclocal     ${ACLOCAL_VERSION}"
LIBTOOL_M4=$(dirname $(dirname $(which libtool)))/share/libtool/libtool.m4
if [ -e ${LIBTOOL_M4} ]; then
	cp "${LIBTOOL_M4}" acinclude.m4 || exit 1
fi
aclocal || exit 1

## Libtoolize
LIBTOOLIZE=libtoolize
WHITESPACE='  '
if glibtoolize --version &>/dev/null ; then
	LIBTOOLIZE=glibtoolize
	WHITESPACE=' '
fi
LIBTOOLIZE_VERSION=`${LIBTOOLIZE} --version | head -1 | grep -o '[^ ]*$'`
step "${LIBTOOLIZE}${WHITESPACE}${LIBTOOLIZE_VERSION}"
mkdir -p build-aux
${LIBTOOLIZE} --copy --force >/dev/null || exit 1

## Autoconf
AUTOCONF_VERSION=`autoconf --version | head -1 | grep -o '[^ ]*$'`
step "autoconf    ${AUTOCONF_VERSION}"
autoconf || exit 1

## Autoheader
AUTOHEADER_VERSION=`autoheader --version | head -1 | grep -o '[^ ]*$'`
step "autoheader  ${AUTOHEADER_VERSION}"
autoheader || exit 1

## Automake
AUTOMAKE_VERSION=`automake --version | head -1 | grep -o '[^ ]*$'`
step "automake    ${AUTOMAKE_VERSION}"
automake --add-missing --copy || exit 1

step "."
exit 0
