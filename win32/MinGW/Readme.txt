-- Building
To build <win32\liburiparser.a> run:
  mingw32-make.exe
in this directory.

To build and run the test suite (file <win32\test.exe>) run:
  mingw32-make.exe check
in this directory. 

Alternatively, install MSYS <http://www.mingw.org/msys.shtml>, set
it to use your MinGW distro during post-install and use ./configure in
the root of this package to produce the Makefile. Then run make.


-- Installation
To install uriparser in your MinGW distro:
1. Copy liburiparser.a to the MinGW lib directory.
2. Copy the uriparser directory (in this package's include
   directory) to the MinGW include directory.

-- Hello world à la uriparser
To build a C program that uses liburiparser:
gcc -Wall -Wextra example.c -o example -O3 -s -luriparser


--------------------------------------------------------------
Readme contributed by Michael Anthony Puls II.
