[![Travis CI Build Status](https://travis-ci.org/uriparser/uriparser.svg?branch=master)](https://travis-ci.org/uriparser/uriparser)


# uriparser

uriparser is a
strictly [RFC 3986](http://tools.ietf.org/html/rfc3986) compliant
URI parsing and handling library
written in C89 ("ANSI C").
uriparser is cross-platform,
fast,
supports Unicode, and
is licensed under the [New BSD license](https://github.com/uriparser/uriparser/blob/master/COPYING).

To learn more about uriparser,
please check out [https://uriparser.github.io/](https://uriparser.github.io/).


# Compilation

## Compilation (standalone, GNU make, Linux)
```console
# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..  # see CMakeLists.txt for options
# make
# make test
# make install
```

## Available CMake options
```console
# cmake -LAH | grep -B1 'URIPARSER_\|BUILD_SHARED_LIBS'
// Build shared libraries (rather than static ones)
BUILD_SHARED_LIBS:BOOL=ON
--
// Build code supporting data type 'char'
URIPARSER_BUILD_CHAR:BOOL=ON
--
// Build API documentation (requires Doxygen, Graphviz, and (optional) Qt's qhelpgenerator)
URIPARSER_BUILD_DOCS:BOOL=ON
--
// Build test suite (requires GTest >=1.8.1)
URIPARSER_BUILD_TESTS:BOOL=ON
--
// Build tools (e.g. CLI "uriparse")
URIPARSER_BUILD_TOOLS:BOOL=ON
--
// Build code supporting data type 'wchar_t'
URIPARSER_BUILD_WCHAR_T:BOOL=ON
```
