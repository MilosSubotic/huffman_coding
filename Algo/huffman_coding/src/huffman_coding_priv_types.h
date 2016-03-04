/**
 * @license MIT
 * @brief Private type definitions for Huffman coding.
 */

#ifndef HUFFMAN_CODING_PRIV_TYPES_H
#define HUFFMAN_CODING_PRIV_TYPES_H

///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include "huffman_coding_public_types.h"
#include "huffman_coding_priv_config.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	typedef uint32_t freq_t; // freq_width
	typedef uint32_t node_t; // node_width
	typedef uint32_t dep_t; // dep_width
	typedef uint32_t len_t; // len_width
	typedef uint32_t len_freq_t; // len_freq_width
	typedef uint32_t code_t; // code_width

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_PRIV_TYPES_H

