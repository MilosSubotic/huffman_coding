#
###############################################################################
# @license MIT
# @brief Build huffman_coding
###############################################################################

SOURCES=huffman_coding.cpp

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
	./huffman_coding${EXE}

###############################################################################

all: huffman_coding${EXE}
huffman_coding${EXE}: ${OBJECTS}
	${CXX} -o $@ $^ ${LDFLAGS} ${LIBS}

###############################################################################

clean:
	make clean -C other/examples/
	make clean -C other/huffman_codec/
	rm -f *.exe *.elf *.o

###############################################################################
