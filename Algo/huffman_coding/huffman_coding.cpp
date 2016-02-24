/*
 * @license MIT
 * @brief Test Huffman coding.
 */

///////////////////////////////////////////////////////////////////////////////

#include <deque>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <bitset>
#include <sstream>

using namespace std;


#include "huffman_coding.h"

using namespace huffman_coding;


///////////////////////////////////////////////////////////////////////////////

#define DEBUG(var) \
	do{ \
		cout << #var << " = " << var << endl; \
	}while(0)

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {


	template<typename T>
	static void shift(deque<T>& d) {
		d.pop_front();
		d.push_back(T());
	}

	static string bits_to_string(
			uint64_t bits, unsigned start, unsigned end = 0) {
		ostringstream oss;
		bitset<64> bf(bits);
		for(int i = start-1; i >= int(end); i--){
			oss << bf[i];
		}
		return oss.str();
	}

	void huffman_encode(
		const std::vector<sym_t>& in_data,
		std::vector<uint32_t>& out_enc_data
	) {
		cout << "Encoding..." << endl << endl;

		uint64_t acc = 0;
		int acc_len = 0;
		auto pack = [&](uint32_t bits, unsigned len) {
			assert(len <= 32);
			acc |= bits << acc_len;
			acc_len += len;
			if(acc_len >= 32){
				out_enc_data.push_back(acc & 0xffffffff);
				acc >>= 32;
				acc_len -= 32;
			}
		};

		for(int block = 0; block < in_data.size()/block_len; block++){
			cout << "block: " << block << endl << endl;

			vector<sym_t> in_data_block(block_len);

			for(int d = 0; d < block_len; d++){
				int index = block*block_len + d;
				if(index < in_data.size()){
					in_data_block[d] = in_data[index];
				}else{
					// Pading with zeros.
					in_data_block[d] = 0;
				}
			}

			cout << "in_data_block:" << endl;
			for(int d = 0; d < block_len; d++){
				cout << setw(3)
					<< in_data_block[d] << endl;
			}
			cout << endl << endl;


			cout << "Creating histogram..." << endl;

			vector<freq_t> histogram(sym_num, 0);

			for(int d = 0; d < sym_num; d++){
				histogram[in_data_block[d]]++;
			}

			cout << "histogram:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				cout << setw(3) << sym << ": " << setw(3)
						<< histogram[sym] << endl;
			}
			cout << endl << endl;



			cout << "Preparing for sorting symbols by count..." << endl;

			class sym_and_freq {
			public:
				sym_t sym;
				freq_t freq;
			};

			// freq_width ones.
			const node_t null_node = (1 << (freq_width))-1;

			vector<sym_and_freq> sort_freq(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				sort_freq[sym].sym = sym;
				if(histogram[sym] == 0){
					sort_freq[sym].freq = null_node;
				}else{
					sort_freq[sym].freq = histogram[sym];
				}
			}

			cout << "sort_freq:" << endl;
			for(int i = 0; i < sym_num; i++){
				cout << setw(3) << sort_freq[i].sym << ": "
					<< setw(3) << sort_freq[i].freq << endl;
			}
			cout << endl << endl;



			cout << "Sorting symbols by count..." << endl;

			sort(
				sort_freq.begin(),
				sort_freq.end(),
				[](const sym_and_freq& x, const sym_and_freq& y){
					return x.freq < y.freq;
				}
			);

			cout << "sort_freq:" << endl;
			for(int i = 0; i < sym_num; i++){
				cout << setw(3) << sort_freq[i].sym << ": "
					<< setw(3) << sort_freq[i].freq << endl;
			}
			cout << endl << endl;




			cout << "Creating leaves..." << endl;


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

			deque<node_and_freq> leaves(sym_num);
			// Copy sorted symbols to leaf nodes.
			// Symbols now becomes IDs leaves.
			for(int i = 0; i < sym_num; i++){
				leaves[i] = sort_freq[i];
			}

			cout << "leaves:" << endl;
			for(int i = 0; i < sym_num; i++){
				cout << setw(3) << leaves[i].node << ": "
					<< setw(3) << leaves[i].freq << endl;
			}
			cout << endl << endl;




			cout << "Tracking depth of leaves on quasi-tree..." << endl;

			deque<node_and_freq> parents(sym_num);
			int parents_end = 0;

			// TODO Solve problem when only one leaf exists.

			// Parent have nodes IDs larger than leaves IDs.
			node_t new_parent_node = sym_num;

			// Used for making canonical Huffman tree.
			class node_and_dep {
			public:
				node_t node;
				dep_t dep;
			};
			vector<node_and_dep> depth_tracker(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				depth_tracker[sym].node = sym;
				depth_tracker[sym].dep = 0;
			}

			// Could be done in 15 iterations, because there is max 15 parents.
			for(int iter = 0; iter < sym_num-1; iter++){
				// When only one node and no more leaves then tree is built.
				if(leaves[0].is_null() && parents[1].is_null()){
					assert(!parents[0].is_null());
					break;
				}

				assert(new_parent_node < null_node);

				// Cannot both be empty at the same time.
				assert(!(leaves[0].is_null() && parents[0].is_null()));

				node_and_freq child0;
				node_and_freq child1;

				if(leaves[0].freq < parents[0].freq){
					// First leaf.
					child0 = leaves[0];

					if(leaves[1].freq < parents[0].freq){
						// First leaf and second leaf.
						child1 = leaves[1];
						shift(leaves);
					}else{
						// First leaf and first parent.
						child1 = parents[0];
						shift(parents);
						parents_end--;
					}
					shift(leaves);
				}else{
					// First parent.
					child0 = parents[0];

					if(parents[1].freq < leaves[0].freq){
						// First parent and second parent.
						child1 = parents[1];
						shift(parents);
						parents_end--;
					}else{
						// First parent and first leaf.
						child1 = leaves[0];
						shift(leaves);
					}
					shift(parents);
					parents_end--;
				}
				if(parents_end < 0){
					parents_end = 0;
				}


				// Create parent.
				node_and_freq new_parent(
					new_parent_node,
					child0.freq + child1.freq
				);
				// Ready for new parent.
				new_parent_node++;

				// Insert parent.
				parents[parents_end++] = new_parent;

				// Increment symbols depth.
				for(int sym = 0; sym < sym_num; sym++){
					if(depth_tracker[sym].node == child0.node){
						depth_tracker[sym].dep++;
						depth_tracker[sym].node = new_parent.node;
					}else if(depth_tracker[sym].node == child1.node){
						depth_tracker[sym].dep++;
						depth_tracker[sym].node = new_parent.node;
					}
				}

				cout << "iter " << iter << ":" << endl;
				cout << "leaves:" << endl;
				for(int i = 0; i < sym_num; i++){
					cout << setw(3) << leaves[i].node << ": "
						<< setw(3) << leaves[i].freq << endl;
				}
				cout << "parents:" << endl;
				for(int i = 0; i < sym_num; i++){
					cout << setw(3) << parents[i].node << ": "
						<< setw(3) << parents[i].freq << endl;
				}
				cout << "parents_end: " << parents_end << endl;
				cout << "depth_tracker:" << endl;
				for(int i = 0; i < sym_num; i++){
					cout << setw(3) << depth_tracker[i].node << ": "
						<< setw(3) << depth_tracker[i].dep << endl;
				}
				cout << endl;

			}

			// No leafs and only one root parent node.
			assert(leaves[0].is_null());
			assert(!parents[0].is_null());
			assert(parents[1].is_null());
			cout << endl << endl;




			cout << "Extracting bit-lengths of codes for symbols..." << endl;

			vector<len_t> codes_len(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				codes_len[sym] = depth_tracker[sym].dep;
				assert(codes_len[sym] <= max_code_len);
			}

			cout << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				cout << setw(3) << sym << ": "
					<< setw(3) << codes_len[sym] << endl;
			}
			cout << endl << endl;



			cout << "Counting same bit-lengths..." << endl;

			vector<len_freq_t> lens_freq(len_freq_num, 0); // Init to zeros.

			for(int sym = 0; sym < sym_num; sym++){
				lens_freq[codes_len[sym]]++;
			}

			cout << "lens_freq:" << endl;
			for(int len = 0; len < len_freq_num; len++){
				cout << setw(3) << len << ": "
					<< setw(3) << lens_freq[len] << endl;
			}
			cout << endl << endl;




			cout << "Preparing for sorting symbols by bit-lengths..." << endl;

			class sym_and_len{
			public:
				sym_t sym;
				len_t len;
			};

			const int null_len = (1 << len_width) - 1;

			vector<sym_and_len> sort_len(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				sort_len[sym].sym = sym;
				// If length is 0 put it to invalid value 7,
				// which is also greater than 5,
				// to be sorted at the end of array.
				if(histogram[sym] == 0){
					sort_len[sym].len = null_len;
				}else{
					sort_len[sym].len = codes_len[sym];
				}
			}

			cout << "sort_len:" << endl;
			for(int i = 0; i < sym_num; i++){
				cout << setw(3) << sort_len[i].sym << ": "
					<< setw(3) << sort_len[i].len << endl;
			}
			cout << endl << endl;



			cout << "Sorting symbols by lengths..." << endl;

			sort(
				sort_len.begin(),
				sort_len.end(),
				[](const sym_and_len& x, const sym_and_len& y){
					return x.len < y.len;
				}
			);

			cout << "sort_len:" << endl;
			for(int i = 0; i < sym_num; i++){
				cout << setw(3) << sort_len[i].sym << ": "
					<< setw(3) << sort_len[i].len << endl;
			}
			cout << endl << endl;




			cout << "Creating canonical code table..." << endl;

			vector<code_t> code_table(sym_num);
			code_t code = 0;
			for(int i = 0; i < sym_num; i++){
				sym_t sym = sort_len[i].sym;
				len_t len = sort_len[i].len;
				// Assign current code for symbol's length.
				code_table[sym] = code;
				// Increment to next code.
				code += 1 << (code_width - len);
			}

			cout << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				cout << setw(3) << sym << ": "
					<< setw(1) << codes_len[sym] << endl;
			}
			cout << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				cout << setw(3) << sym << ": "
					<< bits_to_string(
							code_table[sym],
							max_dep,
							max_dep-codes_len[sym]
						)
					<< endl;
			}
			cout << endl << endl;



			// Mirror codes for little endian encoding.
			cout << "Mirror codes for little endian encoding." << endl;

			for(int sym = 0; sym < sym_num; sym++){
				bitset<64> src(code_table[sym]);
				bitset<64> dst(src);
				for(int i = 0; i < code_width; i++){
					dst[code_width-1-i] = src[i];
				}
				code_table[sym] = dst.to_ulong();
			}

			cout << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				cout << setw(3) << sym << ": " << setw(code_width)
					<< bits_to_string(code_table[sym], codes_len[sym], 0)
					<< endl;
			}
			cout << endl << endl;



			cout << "Packing canonical table..." << endl;

			uint32_t store_len = 0;

			// Store 4 bit-lengths count.
			for(int len = 1; len < len_freq_num; len++){
				pack(lens_freq[len], len_freq_width); // len_freq_t is 5-bit.

				store_len += len_freq_width;
			}

			// Store symbols.
			for(int i = 0; i < sym_num; i++){
				len_t len = sort_len[i].len;
				// Don't save symbols with invalid length ie. symbols with count 0.
				if(len != null_len){
					pack(sort_len[i].sym, sym_width); // sym_t is 4-bit.

					store_len += sym_width;
				}
			}

			cout << "stored_len: " << store_len << endl;
			cout << endl << endl;




			cout << "Encoding and packing data..." << endl;

			cout << "Encoding:" << endl;
			for(int d = 0; d < sym_num; d++){
				sym_t sym = in_data_block[d];
				code_t code = code_table[sym];
				len_t code_len = codes_len[sym];

				// Strap 1s above code length.
				code &= (1 << code_len) - 1; // TODO Isn't this redudant a little.

				pack(code, code_len);

				store_len += code_len;

				cout << "iter " << d << ":" << endl;
				cout << "sym: " << setw(3) << sym << endl;
				cout << "code: "
					<< setw(5) << bits_to_string(code, code_len, 0) << endl;
				cout << endl;

			}
			cout << "stored_len: " << store_len << endl;
			cout << endl << endl;

		}

		cout << "last byte acc_len: " << acc_len << endl;
		// Push unfinished byte, if one exists.
		if(acc_len != 0){
			out_enc_data.push_back(acc);
		}

	}

	void huffman_decode(
		const std::vector<uint32_t>& in_enc_data,
		std::vector<sym_t>& out_data
	) {

		cout << "Decoding..." << endl << endl;

		uint64_t acc = 0;
		unsigned acc_len = 0;
		auto ed = in_enc_data.begin();
		auto unpack = [&](unsigned len) -> uint32_t {
			assert(len <= 32);
			uint32_t ret;
			if(acc_len < len){
				acc |= uint64_t(*ed++) << acc_len;
				acc_len += 32;
			}
			ret = acc & ((uint64_t(1) << len) - 1);
			acc >>= len;
			acc_len -= len;
			return ret;
		};


		while(ed < in_enc_data.end()) {

			cout << "Unpacking bit-lengths count..." << endl;

			vector<len_freq_t> lens_freq(len_freq_num);

			for(int len = 1; len < len_freq_num; len++){
				lens_freq[len] = unpack(len_freq_width); // len_freq_t is 5-bit.
			}

			cout << "lens_freq:" << endl;
			for(int len = 0; len < len_freq_num; len++){
				cout << setw(3) << len << ": "
					<< setw(3) << lens_freq[len] << endl;
			}
			cout << endl << endl;




			cout << "Unpacking symbols and creating canonical code table..."
				<< endl;

			vector<len_t> codes_len(sym_num, 0);
			vector<code_t> code_table(sym_num);

			code_t code = 0;
			for(len_t len = 1; len < len_freq_num; len++){
				for(int freq = lens_freq[len]; freq > 0; freq--){
					sym_t sym = unpack(sym_width); // sym_t is 4-bit.
					code_table[sym] = code;
					codes_len[sym] = len;
					// Increment to next code.
					code += 1 << (code_width - len);
				}
			}

			cout << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				cout << setw(3) << sym << ": "
					<< setw(3) << codes_len[sym] << endl;
			}
			cout << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				cout << setw(3) << sym << ": "
					<< bits_to_string(
							code_table[sym],
							code_width,
							code_width-codes_len[sym]
						)
					<< endl;
			}
			cout << endl << endl;




			cout << "Mirroring codes for little endian decoding..." << endl;

			for(int sym = 0; sym < sym_num; sym++){
				bitset<64> src(code_table[sym]);
				bitset<64> dst(src);
				for(int i = 0; i < code_width; i++){
					dst[code_width-1-i] = src[i];
				}
				code_table[sym] = dst.to_ulong();
			}

			cout << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				cout << setw(3) << sym << ": " << setw(max_code_len)
					<< bits_to_string(code_table[sym], codes_len[sym], 0)
					<< endl;
			}
			cout << endl << endl;




			cout << "Decoding data..." << endl;

			for(int d = 0; d < sym_num; d++){
				if(acc_len < max_code_len){
					acc |= uint64_t(*ed++) << acc_len;
					acc_len += 32;
				}

				const int null_code_len = (1 << len_width) - 1;

				len_t best_len = null_code_len;
				code_t best_code;
				sym_t best_sym;
				for(int sym = 0; sym < sym_num; sym++){
					len_t code_len = codes_len[sym];
					if(code_len != 0 && code_len < best_len){
						code_t mask = (1 << code_len) - 1;
						code_t enc_code = acc & mask;
						if(enc_code == code_table[sym]){
							best_len = code_len;
							best_code = code_table[sym];
							best_sym = sym;
						}
					}
				}
				assert(0 < best_len && best_len <= max_code_len);

				// Remove decoded symbol.
				acc >>=  best_len;
				acc_len -=  best_len;

				out_data.push_back(best_sym);

				cout << "iter " << d << ":" << endl;
				cout << "best_len: " << setw(3) << best_len << endl;
				cout << "best_code: " << setw(max_code_len)
						<< bits_to_string(code, best_len, 0) << endl;
				cout << "best_sym: " << setw(3) << best_sym << endl;
				cout << endl;
			}

		}

		cout << "out_data:" << endl;
		for(int d = 0; d < out_data.size(); d++){
			cout << setw(3) << d << ": " << setw(3) << out_data[d] << endl;
		}
		cout << endl << endl;

	}

}

///////////////////////////////////////////////////////////////////////////////
