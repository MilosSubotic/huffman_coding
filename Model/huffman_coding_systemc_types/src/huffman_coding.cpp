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
#include "huffman_coding_structs.h"

///////////////////////////////////////////////////////////////////////////////

#define DEBUG(var) \
	do{ \
		bcout << #var << " = " << var << endl; \
	}while(0)

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	binary_ostream bcout(cout);

	template<typename T>
	static void shift(deque<T>& d) {
		d.pop_front();
		d.push_back(T());
	}

	void huffman_encode(
		const std::vector<sym_t>& in_data,
		std::vector<uint32_t>& out_enc_data
	) {
		bcout << "Encoding..." << endl << endl;

		uint64_t acc = 0;
		int acc_len = 0;
		auto pack = [&](uint32_t bits, unsigned len) {
			assert(len <= 32);
			acc |= uint64_t(bits) << acc_len;
			acc_len += len;
			if(acc_len >= 32){
				out_enc_data.push_back(acc & 0xffffffff);
				acc >>= 32;
				acc_len -= 32;
			}
		};

		const int block_num = ceil(float(in_data.size())/block_len);
		bcout << "block_num: " << block_num << endl << endl;
		for(int block = 0; block < block_num; block++){
			bcout << "block: " << block << endl << endl;

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

			bcout << "in_data_block:" << endl;
			for(int d = 0; d < block_len; d++){
				bcout << in_data_block[d] << endl;
			}
			bcout << endl << endl;

			bcout << "Creating histogram..." << endl;

			vector<freq_t> histogram(sym_num, 0);

			for(int d = 0; d < block_len; d++){
				histogram[in_data_block[d]]++;
			}

			bcout << "histogram:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": " << histogram[sym] << endl;
			}
			bcout << endl << endl;



			bcout << "Preparing for sorting symbols by count..." << endl;

			vector<sym_and_freq> sort_freq(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				sort_freq[sym].sym = sym;
				if(histogram[sym] == 0){
					sort_freq[sym].freq = null_node;
				}else{
					sort_freq[sym].freq = histogram[sym];
				}
			}

			bcout << "sort_freq:" << endl;
			for(int i = 0; i < sym_num; i++){
				bcout << sort_freq[i] << endl;
			}
			bcout << endl << endl;



			bcout << "Sorting symbols by count..." << endl;

			sort(
				sort_freq.begin(),
				sort_freq.end(),
				[](const sym_and_freq& x, const sym_and_freq& y){
					return x.freq < y.freq;
				}
			);

			bcout << "sort_freq:" << endl;
			for(int i = 0; i < sym_num; i++){
				bcout  << sort_freq[i] << endl;
			}
			bcout << endl << endl;




			bcout << "Creating leaves..." << endl;


			deque<node_and_freq> leaves(sym_num);
			// Copy sorted symbols to leaf nodes.
			// Symbols now becomes IDs leaves.
			for(int i = 0; i < sym_num; i++){
				leaves[i] = sort_freq[i];
			}

			bcout << "leaves:" << endl;
			for(int i = 0; i < sym_num; i++){
				bcout << leaves[i] << endl;
			}
			bcout << endl << endl;




			bcout << "Tracking depth of leaves on quasi-tree..." << endl;

			deque<node_and_freq> parents(sym_num);
			int parents_end = 0;

			// TODO Solve problem when only one leaf exists.

			// Parent have nodes IDs larger than leaves IDs.
			node_t new_parent_node = sym_num;

			// Used for making canonical Huffman tree.
			vector<node_and_dep> depth_tracker(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				depth_tracker[sym].node = sym;
				depth_tracker[sym].dep = 0;
			}

			// Could be done in sym_num-1 iterations,
			// because there is max sym_num-1 parents.
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

				bcout << "iter " << iter << ":" << endl;
				bcout << "leaves:" << endl;
				for(int i = 0; i < sym_num; i++){
					bcout << leaves[i] << endl;
				}
				bcout << "parents:" << endl;
				for(int i = 0; i < sym_num; i++){
					bcout << parents[i] << endl;
				}
				bcout << "parents_end: " << parents_end << endl;
				bcout << "depth_tracker:" << endl;
				for(int i = 0; i < sym_num; i++){
					bcout << depth_tracker[i] << endl;
				}
				bcout << endl;

			}

			// No leafs and only one root parent node.
			assert(leaves[0].is_null());
			assert(!parents[0].is_null());
			assert(parents[1].is_null());
			bcout << endl << endl;




			bcout << "Extracting bit-lengths of codes for symbols..." << endl;

			vector<len_t> codes_len(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				codes_len[sym] = depth_tracker[sym].dep;
				assert(codes_len[sym] <= max_code_len);
			}

			bcout << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": "
					<< codes_len[sym] << endl;
			}
			bcout << endl << endl;



			bcout << "Counting same bit-lengths..." << endl;

			vector<len_freq_t> lens_freq(len_freq_num, 0); // Init to zeros.

			for(int sym = 0; sym < sym_num; sym++){
				lens_freq[codes_len[sym]]++;
			}

			bcout << "lens_freq:" << endl;
			for(int len = 0; len < len_freq_num; len++){
				bcout << len << ": "
					<< lens_freq[len] << endl;
			}
			bcout << endl << endl;




			bcout << "Preparing for sorting symbols by bit-lengths..." << endl;

			const int null_len = (1 << len_width) - 1;

			vector<sym_and_len> sort_len(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				sort_len[sym].sym = sym;
				if(histogram[sym] == 0){
					sort_len[sym].len = null_len;
				}else{
					sort_len[sym].len = codes_len[sym];
				}
			}

			bcout << "sort_len:" << endl;
			for(int i = 0; i < sym_num; i++){
				bcout << sort_len[i] << endl;
			}
			bcout << endl << endl;



			bcout << "Sorting symbols by lengths..." << endl;

			sort(
				sort_len.begin(),
				sort_len.end(),
				[](const sym_and_len& x, const sym_and_len& y){
					return x.len < y.len;
				}
			);

			bcout << "sort_len:" << endl;
			for(int i = 0; i < sym_num; i++){
				bcout << sort_len[i] << endl;
			}
			bcout << endl << endl;




			bcout << "Creating canonical code table..." << endl;

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

			bcout << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": " << codes_len[sym] << endl;
			}
			bcout << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": " << code_table[sym] << endl;
			}
			bcout << endl << endl;



			bcout << "Mirroring codes for little endian decoding..." << endl;

			for(int sym = 0; sym < sym_num; sym++){
				code_t src = code_table[sym];
				code_t dst;
				for(int i = 0; i < code_width; i++){
					dst[code_width-1-i] = src[i];
				}
				code_table[sym] = dst;
			}

			bcout << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": "
					<< codes_len[sym] << endl;
			}
			bcout << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": " << code_table[sym] << endl;
			}
			bcout << endl << endl;



			bcout << "Packing canonical table..." << endl;

			uint32_t store_len = 0;

			for(int len = 1; len < len_freq_num; len++){
				pack(lens_freq[len], len_freq_width);

				store_len += len_freq_width;
			}

			// Store symbols.
			for(int i = 0; i < sym_num; i++){
				len_t len = sort_len[i].len;
				// Don't save symbols with invalid length
				// ie. symbols with count 0.
				if(len != null_len){
					pack(sort_len[i].sym, sym_width);

					store_len += sym_width;
				}
			}

			bcout << "stored_len: " << store_len << endl;
			bcout << endl << endl;




			bcout << "Encoding and packing data..." << endl;

			bcout << "Encoding:" << endl;
			for(int d = 0; d < block_len; d++){
				sym_t sym = in_data_block[d];
				code_t code = code_table[sym];
				len_t code_len = codes_len[sym];

				// Strap 1s above code length.
				// TODO Isn't this redudant a little.
				code &= (1 << code_len) - 1;

				pack(code, code_len);

				store_len += code_len;

				bcout << "iter " << d << ":" << endl;
				bcout << "sym: " << sym << endl;
				bcout << "code_len: " << code_len << endl;
				bcout << "code: " << code << endl;
				bcout << endl;

			}
			bcout << "stored_len: " << store_len << endl;
			bcout << endl << endl;

		}

		bcout << "last byte acc_len: " << acc_len << endl;
		// Push unfinished byte, if one exists.
		if(acc_len != 0){
			out_enc_data.push_back(acc);
		}

	}

	void huffman_decode(
		const std::vector<uint32_t>& in_enc_data,
		std::vector<sym_t>& out_data
	) {

		bcout << "Decoding..." << endl << endl;

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

			bcout << "Unpacking bit-lengths count..." << endl;

			vector<len_freq_t> lens_freq(len_freq_num);

			for(int len = 1; len < len_freq_num; len++){
				lens_freq[len] = unpack(len_freq_width);
			}

			bcout << "lens_freq:" << endl;
			for(int len = 0; len < len_freq_num; len++){
				bcout << len << ": "
					<< lens_freq[len] << endl;
			}
			bcout << endl << endl;




			bcout << "Unpacking symbols and creating canonical code table..."
				<< endl;

			vector<len_t> codes_len(sym_num, 0);
			vector<code_t> code_table(sym_num);

			code_t code = 0;
			for(len_t len = 1; len < len_freq_num; len++){
				for(int freq = lens_freq[len]; freq > 0; freq--){
					sym_t sym = unpack(sym_width);
					code_table[sym] = code;
					codes_len[sym] = len;
					// Increment to next code.
					code += 1 << (code_width - len);
				}
			}


			bcout << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": "
					<< codes_len[sym] << endl;
			}
			bcout << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": " << code_table[sym] << endl;
			}
			bcout << endl << endl;



			bcout << "Mirroring codes for little endian decoding..." << endl;

			for(int sym = 0; sym < sym_num; sym++){
				code_t src = code_table[sym];
				code_t dst;
				for(int i = 0; i < code_width; i++){
					dst[code_width-1-i] = src[i];
				}
				code_table[sym] = dst;
			}

			bcout << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": "
					<< codes_len[sym] << endl;
			}
			bcout << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				bcout << sym << ": " << code_table[sym] << endl;
			}
			bcout << endl << endl;




			bcout << "Decoding data..." << endl;

			for(int d = 0; d < block_len; d++){
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

				bcout << "iter " << d << ":" << endl;
				bcout << "best_len: " << best_len << endl;
				bcout << "best_code: " << best_code << endl;
				bcout << "best_sym: " << best_sym << endl;
				bcout << endl;
			}

		}

		bcout << "out_data:" << endl;
		for(int d = 0; d < out_data.size(); d++){
			bcout << d << ": " << out_data[d] << endl;
		}
		bcout << endl << endl;

	}

}

///////////////////////////////////////////////////////////////////////////////
