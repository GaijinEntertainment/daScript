#! /usr/bin/env bash
PWD_BACKUP=${PWD}
SCRIPT_DIR=`dirname "${PWD}/$0"`
cd "${SCRIPT_DIR}/.." || exit 1
function fail() { cd "${PWD_BACKUP}" ; exit 1; }
####################################################################

kwrite \
	doc/configure.ac \
	include/uriparser/UriBase.h \
	configure.ac \
	ChangeLog \
	&

####################################################################
cd "${PWD_BACKUP}" || fail
exit 0

