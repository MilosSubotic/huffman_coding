/**
 * @license MIT
 * @brief Print stuff for types and structs.
 */

///////////////////////////////////////////////////////////////////////////////

#include "huffman_coding_print.h"
#include <fstream>

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	void binary_ostream::print(const sc_uint_base& x) {
		for(int i = x.length()-1; i >= 0; i--) {
			_os << (x[i] ? '1' : '0');
		}
	}

	ofstream algo_log_f("logs/alog.log");
	// TODO Maybe algo log should be binary.
	binary_ostream algo_log(algo_log_f);
	binary_ostream debug_log(cout);
}

///////////////////////////////////////////////////////////////////////////////
