/**
 * @license MIT
 * @brief Configuration file for Huffman coding.
 */

#ifndef HUFFMAN_CODING_CONFIG_H
#define HUFFMAN_CODING_CONFIG_H

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {
	// User defined.
	const int sym_width = 8;
	const int block_len_log2 = 8;
	const int enc_chunk_width = 32;

	// Calculated.
	const int block_len = 1 << block_len_log2;
}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_CONFIG_H

