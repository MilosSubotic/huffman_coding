/*
 * @license MIT
 * @brief Print stuff for types and structs.
 */

#ifndef HUFFMAN_CODING_PRINT_H
#define HUFFMAN_CODING_PRINT_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

#include "huffman_coding_structs.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	class binary_ostream : std::ostream {
	public:
		explicit binary_ostream(std::ostream& os)
			: _os(os) {}

		template<typename T>
		binary_ostream& operator<<(T x) {
			_os << x;
			return *this;
		}

		binary_ostream& operator<<(ostream& (*pf)(ostream&)) {
			pf(_os);
			return *this;
		}

		template<int W>
		binary_ostream& operator<<(const sc_uint<W>& x) {
			this->print(x);
			return *this;
		}

	private:
		std::ostream& _os;

		void print(const sc_uint_base&);

	};

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_STRUCTS_H

