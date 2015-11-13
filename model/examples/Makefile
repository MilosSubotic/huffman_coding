#
###############################################################################
# @license MIT
# @brief Build huffman_codec
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

CXXFLAGS=-O3
ifeq (${BUILD_OS},WINNT)
	CC=gcc
	CPPFLAGS+=-mno-ms-bitfields
endif

###############################################################################

.PHONY: default all run clean
default: run3

###############################################################################

run1: huffman_coding_rosettacode_c${EXE}
	./$< | tee $<.log
	
run2: huffman_coding_rosettacode_c_2${EXE}
	./$< | tee $<.log
	
run3: huffman_coding_rosettacode_cpp${EXE}
	./$< | tee $<.log
	
###############################################################################

huffman_coding_rosettacode_c${EXE}: huffman_coding_rosettacode_c.o
	${CC} -o $@ $^ ${LDFLAGS} ${LIBS}

huffman_coding_rosettacode_c_2${EXE}: huffman_coding_rosettacode_c_2.o
	${CC} -o $@ $^ ${LDFLAGS} ${LIBS}
	
huffman_coding_rosettacode_cpp${EXE}: huffman_coding_rosettacode_cpp.o
	${CXX} -o $@ $^ ${LDFLAGS} ${LIBS}
	
###############################################################################

clean:
	rm -f *.exe *.elf *.o

###############################################################################
