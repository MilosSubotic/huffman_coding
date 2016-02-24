/*
 * @license MIT
 * @brief Type definitions for Huffman coding.
 */

#ifndef HUFFMAN_CODING_TYPES_H
#define HUFFMAN_CODING_TYPES_H

//////////////////////////////////////////////////////////////////////////////

#include "ttmath/ttmath.h"

#include "huffman_coding_config.h"

//////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

#ifdef TTMATH_PLATFORM32
	constexpr int bits_to_nums(int bits) {
		return ceil(float(bits)/32);
	}
#else
	constexpr int bits_to_nums(int bits) {
		return ceil(float(bits)/64);
	}
#endif

	typedef ttmath::UInt<bits_to_nums(sym_width)> sym_t;
	typedef ttmath::UInt<bits_to_nums(freq_width)> freq_t;
	typedef freq_t node_t;
	typedef ttmath::UInt<bits_to_nums(dep_width)> dep_t; // 3-bit.
	typedef dep_t len_t; // 3-bit.
/*	typedef bitset<> len_freq_t; // 5-bit.
	typedef int16_t code_t; // 5-bits.
*/
}

//////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_TYPES_H

