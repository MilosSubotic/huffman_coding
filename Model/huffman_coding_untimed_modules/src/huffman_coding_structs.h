/*
 * @license MIT
 * @brief Huffman coding structs.
 */

#ifndef HUFFMAN_CODING_STRUCTS_H
#define HUFFMAN_CODING_STRUCTS_H

///////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "huffman_coding_priv_types.h"
#include "huffman_coding_print.h"

///////////////////////////////////////////////////////////////////////////////

#define DECLARE_BINARY_OSTREAM_OPERATOR(T) \
	binary_ostream& operator<<(binary_ostream& bos, const T& x)

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	// node_width ones.
	const node_t null_node = (1 << (node_width))-1;

	class sym_and_freq {
	public:
		sym_t sym;
		freq_t freq;
	};
	DECLARE_BINARY_OSTREAM_OPERATOR(sym_and_freq);

	class node_and_freq {
	public:
		node_t node;
		freq_t freq;

		// Null object.
		node_and_freq()
			: node(null_node), freq(null_node) {}

		node_and_freq(node_t node, freq_t freq)
			: node(node), freq(freq) {}

		node_and_freq(const sym_and_freq& s)
			: node(s.sym), freq(s.freq) {}

		bool is_null() const {
			return freq == null_node;
		}
	};
	DECLARE_BINARY_OSTREAM_OPERATOR(node_and_freq);

	class node_and_dep {
	public:
		node_t node;
		dep_t dep;
	};
	DECLARE_BINARY_OSTREAM_OPERATOR(node_and_dep);

	class sym_and_len{
	public:
		sym_t sym;
		len_t len;
	};
	DECLARE_BINARY_OSTREAM_OPERATOR(sym_and_len);


}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_CODING_STRUCTS_H

