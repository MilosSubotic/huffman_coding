/*
 * @license MIT
 * @brief Huffman coding function interfaces.
 */

#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

///////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "huffman_coding_types.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	void huffman_encode(
		const std::vector<sym_t>& in_data,
		std::vector<uint32_t>& out_enc_data
	);

	void huffman_decode(
		const std::vector<uint32_t>& in_enc_data,
		std::vector<sym_t>& out_data
	);
}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_H

