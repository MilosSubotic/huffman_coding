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
	// User defined.
	const int sym_width = 4;
	const int max_block_len = 16;

	// Calculated by huffman_tree_depth.jl script.
	const int max_dep = 5; // Max depth.

	// Calculated.
	const int freq_num = 1 << sym_width;
	const int max_freq = max_block_len;
	const int freq_width = ceil(log2(max_freq + 1));
	const int dep_width = ceil(log2(max_dep + 1));
	const int len_freq_width = freq_num;
	const int max_code_width = max_dep;
}

//////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_CONFIG_H

