/**
 * @license MIT
 * @brief Public type definitions for Huffman coding.
 */

#ifndef HUFFMAN_CODING_PUBLIC_TYPES_H
#define HUFFMAN_CODING_PUBLIC_TYPES_H

///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include <systemc.h>

#include "huffman_coding_config.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	typedef sc_uint<sym_width> sym_t;
	typedef sc_uint<enc_chunk_width> enc_chunk_t;

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_PUBLIC_TYPES_H

