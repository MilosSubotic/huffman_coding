/*
 * @license MIT
 * @brief Type definitions for Huffman coding.
 */

#ifndef HUFFMAN_CODING_TYPES_H
#define HUFFMAN_CODING_TYPES_H

///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include <systemc.h>

#include "huffman_coding_config.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	typedef sc_uint<sym_width> sym_t;
	typedef sc_uint<freq_width> freq_t;
	typedef sc_uint<freq_width> node_t;
	typedef sc_uint<dep_width> dep_t;
	typedef sc_uint<len_width> len_t;
	typedef sc_uint<len_freq_width> len_freq_t;
	typedef sc_uint<code_width> code_t;

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_TYPES_H

