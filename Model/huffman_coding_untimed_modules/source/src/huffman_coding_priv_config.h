/**
 * @license MIT
 * @brief Private calculated configurations for Huffman coding.
 */

#ifndef HUFFMAN_CODING_PRIV_CONFIG_H
#define HUFFMAN_CODING_PRIV_CONFIG_H

///////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "huffman_coding_config.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	// Calculated by huffman_tree_depth.jl script.
	const int max_dep = 11; // Max depth.

	// Calculated.
	const int sym_num = 1 << sym_width; // Max number of frequencies.
	const int freq_width = block_len_log2 + 1;
	const int node_width = freq_width;
	const int num_freq = block_len;
	const int dep_width = ceil(log2(max_dep + 1));
	const int len_width = dep_width;
	const int len_freq_width = freq_width;
	const int len_freq_num = max_dep + 1;
	const int code_width = max_dep;
	const int max_code_len = code_width;
}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_PRIV_CONFIG_H

