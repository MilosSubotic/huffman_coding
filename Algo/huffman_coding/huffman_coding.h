/*
 * @license MIT
 * @brief Test Huffman coding.
 */

#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

//////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "huffman_coding_types.h"

//////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	void huffman_encode(
		const std::vector<sym_t>& in_data,
		std::vector<uint8_t>& out_enc_data
	);

	void huffman_encode(
		const std::vector<uint8_t>& in_enc_data,
		std::vector<sym_t>& out_data
	);
}

//////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_H_

