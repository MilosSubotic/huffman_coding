/**
 * @license MIT
 * @brief Huffman coding interfaces between modules.
 */

#ifndef HUFFMAN_CODING_INTERFACES_H
#define HUFFMAN_CODING_INTERFACES_H

///////////////////////////////////////////////////////////////////////////////

#include "huffman_coding_structs.h"

#include <array>

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	typedef std::array<len_freq_t, len_freq_num> lens_freq_t;

	typedef std::array<len_t, sym_num> codes_len_t;
	typedef std::array<code_t, sym_num> code_table_t;
	struct codes_len_and_code_table_t {
		codes_len_t codes_len;
		code_table_t code_table;
	};
	DECLARE_BINARY_OSTREAM_OPERATOR(codes_len_and_code_table_t);
}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_INTERFACES_H

