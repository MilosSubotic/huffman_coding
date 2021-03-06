/**
 * @license MIT
 * @brief Public type definitions for Huffman coding.
 */

#ifndef HUFFMAN_CODING_PUBLIC_TYPES_H
#define HUFFMAN_CODING_PUBLIC_TYPES_H

///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include "huffman_coding_config.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	typedef uint32_t sym_t; // sym_width
	typedef uint32_t enc_chunk_t; // enc_chunk_width

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_PUBLIC_TYPES_H

