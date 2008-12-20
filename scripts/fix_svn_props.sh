#! /usr/bin/env bash
PWD_BACKUP=${PWD}
SCRIPT_DIR=`dirname "${PWD}/$0"`
cd "${SCRIPT_DIR}/.." || exit 1
function fail() { cd "${PWD_BACKUP}" ; exit 1; }
####################################################################

## Apply native EOL style
find . -name '.svn' -prune -o \( \
        -name '*.cpp' \
        -o -name '*.c' \
        -o -name '*.h' \
        -o -name 'COPYING' \
        -o -name 'Doxyfile' \
        -o -name 'Makefile.*' \
        -o -name '*.sh' \
        \) \
    -exec svn propset svn:eol-style native {} \;
EXTRA_NATIVE="\
    doc/Mainpage \
    \
    AUTHORS \
    BUGS \
    ChangeLog \
    COPYING \
    INSTALL \
    NEWS \
    README \
    THANKS \
    TODO.txt \
"
for i in ${EXTRA_NATIVE} ; do
        svn propset svn:eol-style native ${i}
done


## Fix inconsistent EOLs on Windows
if [ "${TERM}" = "cygwin" ]; then
    find . -name '.svn' -prune -o \( \
            -name '*.cpp' \
            -o -name '*.c' \
            -o -name '*.h' \
            \) \
        -exec unix2dos {} \;
fi


## Make scripts executable
find . -name '.svn' -prune -o \( -name '*.sh' \) -exec svn propset svn:executable '*' {} \;

####################################################################
cd "${PWD_BACKUP}" || fail
exit 0
