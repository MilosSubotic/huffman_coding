/**
 * @license MIT
 * @brief Private type definitions for Huffman coding.
 */

#ifndef HUFFMAN_CODING_PRIV_TYPES_H
#define HUFFMAN_CODING_PRIV_TYPES_H

///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include <systemc.h>

#include "huffman_coding_public_types.h"
#include "huffman_coding_priv_config.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	typedef sc_uint<freq_width> freq_t;
	typedef sc_uint<freq_width> node_t;
	typedef sc_uint<dep_width> dep_t;
	typedef sc_uint<len_width> len_t;
	typedef sc_uint<len_freq_width> len_freq_t;
	typedef sc_uint<code_width> code_t;

	typedef sc_uint<enc_chunk_size_width> enc_chunk_size_t;

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_PRIV_TYPES_H

