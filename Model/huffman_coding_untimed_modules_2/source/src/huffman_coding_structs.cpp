/**
 * @license MIT
 * @brief Huffman coding structs.
 */

///////////////////////////////////////////////////////////////////////////////

#include "huffman_coding_structs.h"
#include "huffman_coding_print.h"

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

	START_OPERATOR(sym_and_freq);
	PRINT_FIELD(sym);
	PRINT_LAST_FIELD(freq);
	END_OPERATOR();

	START_OPERATOR(node_and_freq);
	PRINT_FIELD(node);
	PRINT_LAST_FIELD(freq);
	END_OPERATOR();

	START_OPERATOR(node_and_dep);
	PRINT_FIELD(node);
	PRINT_LAST_FIELD(dep);
	END_OPERATOR();

	START_OPERATOR(sym_and_len);
	PRINT_FIELD(sym);
	PRINT_LAST_FIELD(len);
	END_OPERATOR();

}

///////////////////////////////////////////////////////////////////////////////
