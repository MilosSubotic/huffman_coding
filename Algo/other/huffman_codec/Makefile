#
###############################################################################
# @license MIT
# @brief Build huffman_codec
###############################################################################

SOURCES=huffman_codec.cpp test_huffman_codec.cpp

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
	./test_huffman_codec${EXE}

###############################################################################

all: test_huffman_codec${EXE}
test_huffman_codec${EXE}: ${OBJECTS}
	${CXX} -o $@ $^ ${LDFLAGS} ${LIBS}

${OBJECTS}: *.h

###############################################################################

clean:
	rm -f *.exe *.elf *.o

###############################################################################
