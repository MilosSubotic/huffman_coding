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

###############################################################################

.PHONY: default run run2 clean
default: run

###############################################################################

run: huffman_coding${EXE}
	./huffman_coding${EXE} 2>&1 | tee huffman_coding.log

run2: huffman_coding_2${EXE}
	./huffman_coding_2${EXE}

###############################################################################

huffman_coding${EXE}: huffman_coding.o
	${CXX} -o $@ $^ ${LDFLAGS} ${LIBS}

huffman_coding_2${EXE}: huffman_coding_2.o
	${CXX} -o $@ $^ ${LDFLAGS} ${LIBS}

###############################################################################

clean:
	make clean -C other/examples/
	make clean -C other/huffman_codec/
	rm -f *.exe *.elf *.o

###############################################################################
