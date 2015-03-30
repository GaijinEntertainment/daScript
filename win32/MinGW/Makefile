# Makefile to compile uriparser with Mingw (4.2.1 for example).
# To use with mingw32-make.exe while running in cmd.exe
#
# Copyright (C) 2008, Michael Anthony Puls II <shadow2531@gmail.com>
# Copyright (C) 2008, Sebastian Pipping <webmaster@hartwork.org>
# All rights reserved.
#
# Redistribution  and use in source and binary forms, with or without
# modification,  are permitted provided that the following conditions
# are met:
#
#     * Redistributions   of  source  code  must  retain  the   above
#       copyright  notice, this list of conditions and the  following
#       disclaimer.
#
#     * Redistributions  in  binary  form must  reproduce  the  above
#       copyright  notice, this list of conditions and the  following
#       disclaimer   in  the  documentation  and/or  other  materials
#       provided with the distribution.
#
#     * Neither  the name of the <ORGANIZATION> nor the names of  its
#       contributors  may  be  used to endorse  or  promote  products
#       derived  from  this software without specific  prior  written
#       permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT  NOT
# LIMITED  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS
# FOR  A  PARTICULAR  PURPOSE ARE DISCLAIMED. IN NO EVENT  SHALL  THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL,    SPECIAL,   EXEMPLARY,   OR   CONSEQUENTIAL   DAMAGES
# (INCLUDING,  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES;  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT  LIABILITY,  OR  TORT (INCLUDING  NEGLIGENCE  OR  OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.
#

CFLAGS = -Wall -Wextra -O3 -s

TEST_DIR = ../../test
CPP_TEST_DIR = ../CppTest
LIB_DIR = ../../src
INCLUDE_DIR = ../../include
WIN32_DIR = ..
CC = gcc
CXX = g++
dos = $(subst /,\,$(1))

URIPARSER_OBJECTS := $(patsubst %.c,%.o,$(wildcard $(LIB_DIR)/*.c))
TEST_OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(CPP_TEST_DIR)/*.cpp)) \
		$(patsubst %.cpp,%.o,$(wildcard $(TEST_DIR)/*.cpp))
LIBRARY = $(WIN32_DIR)/liburiparser.a
TEST_SUITE = $(WIN32_DIR)/test.exe

all: $(LIBRARY)

$(LIBRARY): $(URIPARSER_OBJECTS)
	ar rcs $(LIBRARY) $(URIPARSER_OBJECTS)

$(URIPARSER_OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o "$@" -c "$<"

$(TEST_SUITE): $(LIBRARY) $(TEST_OBJECTS)
	$(CXX) $(CFLAGS) -I$(INCLUDE_DIR) $(TEST_OBJECTS) \
		-L$(WIN32_DIR) -luriparser -o $(TEST_SUITE)

$(TEST_OBJECTS): %.o : %.cpp
	$(CXX) $(CFLAGS) -I$(INCLUDE_DIR) -c "$<" -o "$@"

clean:
	del $(call dos,$(URIPARSER_OBJECTS)) $(call dos,$(LIBRARY)) \
		$(call dos,$(TEST_OBJECTS)) $(call dos,$(TEST_SUITE))

clean-unix:
	$(RM) $(URIPARSER_OBJECTS) $(LIBRARY) \
		$(TEST_OBJECTS) $(TEST_SUITE)

check: $(TEST_SUITE)
	$(TEST_SUITE)

.PHONY: all check clean clean-unix
