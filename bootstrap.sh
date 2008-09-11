#! /bin/bash
CUR=0
COUNT=5

extractGnuVersion() {
	command=$1
	echo `"${command}" --version | head -1 | grep -o '[^ ]*$'`
}

step() {
	PERC=$((100 * CUR / COUNT))
	printf "[%3i%%] %-12s %s" ${PERC} "$1" "$2"
	echo
	CUR=$((CUR + 1))
}

## Aclocal
step aclocal `extractGnuVersion aclocal`
LIBTOOL_M4=$(dirname $(dirname $(which libtool)))/share/libtool/libtool.m4
if [ -e ${LIBTOOL_M4} ]; then
	cp "${LIBTOOL_M4}" acinclude.m4 || exit 1
	cp "${LIBTOOL_M4}" doc/acinclude.m4 || exit 1
else
	touch acinclude.m4 || exit 1
	touch doc/acinclude.m4 || exit 1
fi
aclocal || exit 1
cd doc && aclocal && cd .. || exit 1

## Libtoolize
LIBTOOLIZE=libtoolize
if glibtoolize --version &>/dev/null ; then
	LIBTOOLIZE=glibtoolize
fi
step ${LIBTOOLIZE} `extractGnuVersion ${LIBTOOLIZE}`
mkdir -p build-aux
${LIBTOOLIZE} --copy --force >/dev/null || exit 1

## Autoconf
step autoconf `extractGnuVersion autoconf`
autoconf || exit 1
cd doc && autoconf && cd .. || exit 1

## Autoheader
step autoheader `extractGnuVersion autoheader`
autoheader || exit 1

## Automake
step automake `extractGnuVersion automake`
automake --add-missing --copy || exit 1
cd doc && automake --add-missing --copy && cd .. || exit 1

step "." ""
exit 0
