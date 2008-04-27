#! /bin/bash
CUR=0
COUNT=5

step() {
	PERC=$((100 * CUR / COUNT))
	printf "[%3i%%] %s" ${PERC} "$1"
	echo
	CUR=$((CUR + 1))
}

## Parent bootstrap
step "../bootstrap.sh"
PWD_BACKUP=$PWD
cd .. || exit 1
./bootstrap.sh || exit 1
cd "${PWD_BACKUP}" || exit 1

## Bootstrap
step "./bootstrap.sh"
./bootstrap.sh || exit 1

## Configure
step "./configure"
./configure || exit 1

## Make
step "make"
make || exit 1

## Make dist
step "make dist"
make dist || exit 1

step "."
exit 0
