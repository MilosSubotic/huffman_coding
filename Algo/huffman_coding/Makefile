#
###############################################################################
# @license MIT
# @brief Build huffman_coding
###############################################################################

SOURCES=src/huffman_coding.cpp huffman_coding_test.cpp

CPPFLAGS+=-Iinclude

###############################################################################

BUILD_OS=$(shell uname)
ifneq (,$(findstring MINGW,$(BUILD_OS)))
	BUILD_OS=WINNT
endif
ifneq (,$(findstring MSYS,$(BUILD_OS)))
	BUILD_OS=WINNT
endif

ifeq (${BUILD_OS},WINNT)
	EXE=.exe
else
	EXE=.elf
endif

CXXFLAGS=-O3 -std=c++0x
ifeq (${BUILD_OS},WINNT)
	CC=gcc
	CPPFLAGS+=-mno-ms-bitfields
endif

OBJECTS=$(SOURCES:%.cpp=%.o)

###############################################################################

.PHONY: default all run clean
default: run

###############################################################################

run: all
	./huffman_coding_test${EXE} 2>&1 | tee huffman_coding_test.log

###############################################################################

all: huffman_coding_test${EXE}
huffman_coding_test${EXE}: ${OBJECTS}
	${CXX} -o $@ $^ ${LDFLAGS} ${LIBS}

*.o: Makefile include/*.h
src/*.o: Makefile include/*.h src/*.h

###############################################################################

clean:
	rm -f *.exe *.elf *.o *.log

###############################################################################
