/**
 * @license MIT
 * @brief
 */

///////////////////////////////////////////////////////////////////////////////

#include "huffman_encoder.h"
#include "huffman_coding_interfaces.h"

using namespace std;

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

	void huffman_encoder::encode() {
		assert(enc_chunk_width == 32);

		algo_log << "Encoding..." << endl << endl;

		uint64_t acc = 0;
		int acc_len = 0;
		vector<enc_chunk_t> enc_data;
		auto pack = [&](uint32_t bits, unsigned len) {
			assert(len <= 32);
			acc |= uint64_t(bits) << acc_len;
			acc_len += len;
			if(acc_len >= 32){
				enc_data.push_back(acc & 0xffffffff);
				acc >>= 32;
				acc_len -= 32;
			}
		};

		for(int block = 0; ; block++){
			bool last = false;

			algo_log << "block: " << block << endl << endl;

			vector<sym_t> in_data_block(block_len);

			int d;
			for(d = 0; d < block_len && !last; d++){
				sym_t data;
				in_data->read(data, last);
				in_data_block[d] = data;
			}
			for(; d < block_len; d++){
				// Pading with zeros.
				in_data_block[d] = 0;
			}
			algo_log << "last: " << last << endl;

			algo_log << "in_data_block:" << endl;
			algo_log << in_data_block << endl;
			algo_log << endl << endl;


			algo_log << "Creating histogram..." << endl;

			vector<freq_t> histogram(sym_num, 0);

			for(int d = 0; d < block_len; d++){
				histogram[in_data_block[d]]++;
			}

			algo_log << "histogram:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				algo_log << sym << ": " << histogram[sym] << endl;
			}
			algo_log << endl << endl;



			algo_log << "Preparing for sorting symbols by count..." << endl;

			vector<sym_and_freq> sort_freq(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				sort_freq[sym].sym = sym;
				if(histogram[sym] == 0){
					sort_freq[sym].freq = null_node;
				}else{
					sort_freq[sym].freq = histogram[sym];
				}
			}

			algo_log << "sort_freq:" << endl;
			algo_log << sort_freq << endl;
			algo_log << endl << endl;



			algo_log << "Sorting symbols by count..." << endl;

			stable_sort(
				sort_freq.begin(),
				sort_freq.end(),
				[](const sym_and_freq& x, const sym_and_freq& y){
					return x.freq < y.freq;
				}
			);

			algo_log << "sort_freq:" << endl;
			algo_log  << sort_freq << endl;
			algo_log << endl << endl;




			algo_log << "Creating leaves..." << endl;


			deque<node_and_freq> leaves(sym_num);
			// Copy sorted symbols to leaf nodes.
			// Symbols now becomes IDs leaves.
			for(int i = 0; i < sym_num; i++){
				leaves[i] = sort_freq[i];
			}

			algo_log << "leaves:" << endl;
			algo_log << leaves << endl;
			algo_log << endl << endl;




			algo_log << "Tracking depth of leaves on quasi-tree..." << endl;

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

				algo_log << "iter " << iter << ":" << endl;
				algo_log << "leaves:" << endl;
				algo_log << leaves << endl;
				algo_log << "parents:" << endl;
				algo_log << parents << endl;
				algo_log << "parents_end: " << parents_end << endl;
				algo_log << "depth_tracker:" << endl;
				algo_log << depth_tracker << endl;
				algo_log << endl;

			}

			// No leafs and only one root parent node.
			assert(leaves[0].is_null());
			assert(!parents[0].is_null());
			assert(parents[1].is_null());
			algo_log << endl << endl;




			algo_log << "Extracting bit-lengths of codes for symbols..." << endl;

			vector<len_t> codes_len(sym_num);
			for(int sym = 0; sym < sym_num; sym++){
				codes_len[sym] = depth_tracker[sym].dep;
				assert(codes_len[sym] <= max_code_len);
			}

			algo_log << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				algo_log << sym << ": "
					<< codes_len[sym] << endl;
			}
			algo_log << endl << endl;



			algo_log << "Counting same bit-lengths..." << endl;

			vector<len_freq_t> lens_freq(len_freq_num, 0); // Init to zeros.

			for(int sym = 0; sym < sym_num; sym++){
				lens_freq[codes_len[sym]]++;
			}

			algo_log << "lens_freq:" << endl;
			for(int len = 0; len < len_freq_num; len++){
				algo_log << len << ": "
					<< lens_freq[len] << endl;
			}
			algo_log << endl << endl;




			algo_log << "Preparing for sorting symbols by bit-lengths..." << endl;

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

			algo_log << "sort_len:" << endl;
			algo_log << sort_len << endl;
			algo_log << endl << endl;



			algo_log << "Sorting symbols by lengths..." << endl;

			stable_sort(
				sort_len.begin(),
				sort_len.end(),
				[](const sym_and_len& x, const sym_and_len& y){
					return x.len < y.len;
				}
			);

			algo_log << "sort_len:" << endl;
			algo_log << sort_len << endl;
			algo_log << endl << endl;




			algo_log << "Creating canonical code table..." << endl;

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

			algo_log << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				algo_log << sym << ": " << codes_len[sym] << endl;
			}
			algo_log << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				algo_log << sym << ": " << code_table[sym] << endl;
			}
			algo_log << endl << endl;



			algo_log << "Mirroring codes for little endian decoding..." << endl;

			for(int sym = 0; sym < sym_num; sym++){
				code_t src = code_table[sym];
				code_t dst;
				for(int i = 0; i < code_width; i++){
					dst[code_width-1-i] = src[i];
				}
				code_table[sym] = dst;
			}

			algo_log << "codes_len:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				algo_log << sym << ": "
					<< codes_len[sym] << endl;
			}
			algo_log << "code_table:" << endl;
			for(int sym = 0; sym < sym_num; sym++){
				algo_log << sym << ": " << code_table[sym] << endl;
			}
			algo_log << endl << endl;



			algo_log << "Packing canonical table..." << endl;

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

			algo_log << "stored_len: " << store_len << endl;
			algo_log << endl << endl;




			algo_log << "Encoding and packing data..." << endl;

			algo_log << "Encoding:" << endl;
			for(int d = 0; d < block_len; d++){
				sym_t sym = in_data_block[d];
				code_t code = code_table[sym];
				len_t code_len = codes_len[sym];

				// Strap 1s above code length.
				// TODO Isn't this redudant a little.
				code &= (1 << code_len) - 1;

				pack(code, code_len);

				store_len += code_len;

				algo_log << "iter " << d << ":" << endl;
				algo_log << "sym: " << sym << endl;
				algo_log << "code_len: " << code_len << endl;
				algo_log << "code: " << code << endl;
				algo_log << endl;

			}
			algo_log << "stored_len: " << store_len << endl;
			algo_log << endl << endl;


			auto iter = enc_data.begin();
			auto end = enc_data.end()-1;
			for(; iter != end; iter++){
				out_enc_data->write(*iter);
			}

			if(!last){
				out_enc_data->write(*iter, false);
			}else{
				algo_log << "last byte acc_len: " << acc_len << endl;
				if(acc_len == 0){
					out_enc_data->write(*iter, true);
				}else{
					out_enc_data->write(*iter, false);
					// Push unfinished byte, if one exists.
					out_enc_data->write(acc, true);
				}
			}

			enc_data.clear();
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
