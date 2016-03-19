/**
 * @license MIT
 * @brief Huffman coding interfaces between modules.
 */

///////////////////////////////////////////////////////////////////////////////

#include "huffman_coding_interfaces.h"

///////////////////////////////////////////////////////////////////////////////

#define START_OPERATOR(T) \
	binary_ostream& operator<<(binary_ostream& bos, const T& x) { \

#define PRINT_FIELD(f) \
	bos << x.f << ' ';

#define PRINT_LAST_FIELD(f) \
	bos << x.f;

#define END_OPERATOR() \
		return bos; \
	}

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	START_OPERATOR(codes_len_and_code_table_t);
	PRINT_FIELD(codes_len);
	PRINT_LAST_FIELD(code_table);
	END_OPERATOR();
}

///////////////////////////////////////////////////////////////////////////////
