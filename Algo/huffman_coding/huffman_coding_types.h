/*
 * @license MIT
 * @brief Type definitions for Huffman coding.
 */

#ifndef HUFFMAN_CODING_TYPES_H
#define HUFFMAN_CODING_TYPES_H

///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include "huffman_coding_config.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	typedef uint32_t sym_t; // sym_width
	typedef uint32_t freq_t; // freq_width
	typedef freq_t node_t; // freq_width
	typedef uint32_t dep_t; // dep_width
	typedef dep_t len_t; // len_width
	typedef uint32_t len_freq_t; // len_freq_width
	typedef uint32_t code_t; // code_width

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_TYPES_H

