#! /bin/sh
echo "[  0%] aclocal"
aclocal || exit 1

echo "[ 17%] automake"
automake --add-missing --copy || exit 1

echo "[ 34%] autoconf"
autoconf || exit 1

echo "[ 50%] ./configure"
./configure || exit 1

echo "[ 67%] make"
make || exit 1

echo "[ 85%] make dist"
make dist || exit 1

echo "[100%] ."
