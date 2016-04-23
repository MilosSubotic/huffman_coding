/**
 * @license MIT
 * @brief Print stuff for types and structs.
 */

#ifndef HUFFMAN_CODING_PRINT_H
#define HUFFMAN_CODING_PRINT_H

///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>

#include <systemc.h>

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	class binary_ostream {
	public:
		explicit binary_ostream(std::ostream& os)
			: _os(os) {}

		template<typename T>
		binary_ostream& operator<<(const T& x) {
			_os << x;
			return *this;
		}

		binary_ostream& operator<<(std::ostream& (*pf)(std::ostream&)) {
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


	template<typename T>
	binary_ostream& operator<<(binary_ostream& bos, std::vector<T> v) {
		auto iter = v.begin();
		auto last = v.end() - 1;
		for(; iter != last; iter++){
			bos << *iter << std::endl;
		}
		bos << *last;

		return bos;
	}

	template<typename T>
	binary_ostream& operator<<(binary_ostream& bos, std::deque<T> v) {
		auto iter = v.begin();
		auto last = v.end() - 1;
		for(; iter != last; iter++){
			bos << *iter << std::endl;
		}
		bos << *last;

		return bos;
	}


	extern binary_ostream bcout;

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_STRUCTS_H

