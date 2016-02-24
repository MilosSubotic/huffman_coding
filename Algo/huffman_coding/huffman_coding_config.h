/*
 * @license MIT
 * @brief Configuration file for Huffman coding.
 */

#ifndef HUFFMAN_CODING_CONFIG_H
#define HUFFMAN_CODING_CONFIG_H

//////////////////////////////////////////////////////////////////////////////

#include <cmath>

//////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {
	const int sym_width = 4;
	const int sym_block_len = 16;

	// Calculated.
	const int max_freq = sym_block_len;
	const int freq_width = ceil(log2(max_freq + 1));

	// Calculated by huffman_tree_depth.jl script.
	const int max_dep = 5; // Max depth.
	const int dep_width = ceil(log2(max_dep + 1));
}

//////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_CONFIG_H

