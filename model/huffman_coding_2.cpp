/*
 * @license MIT
 * @brief Test Huffman codec.
 */
 
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <cassert>
#include <bitset>

using namespace std;

//////////////////////////////////////////////////////////////////////////////

#define DEBUG_LOGS
#define TEXT "babadeda"

//////////////////////////////////////////////////////////////////////////////

#define DEBUG(var) \
	do{ \
		cout << #var << " = " << var << endl; \
	}while(0)

//////////////////////////////////////////////////////////////////////////////

template<typename T>
void shift(deque<T>& d) {
	d.pop_front();
	d.push_back(T());
}

string bits_to_string(uint64_t bits, unsigned start, unsigned end = 0) {
	ostringstream oss;
	bitset<64> bf(bits);
	for(int i = start-1; i >= int(end); i--){
		oss << bf[i];
	}
	return oss.str();
}

typedef uint16_t sym_t; // 4-bits.
// Maximum depth of leaf in Huffman tree is 5.
// Because we decode max 16 symbols in block,
// then count could be max 16.
typedef uint16_t cnt_t; // 5-bits.
typedef uint16_t dep_t; // 3-bit.
// Couldn't have more than 16 depth of same count.
// Practically even less. 
// TODO Calculate how much.
typedef uint16_t dep_cnt_t; // 4-bit.
// Because max depths is 5, max bits in symbols is also 5.
typedef int16_t code_t; // 5-bits.

void huffman_encode(
	const string& in_text,
	vector<uint8_t>& enc_data
) {

	uint8_t acc = 0;
	int acc_len = 0;
	bool last_block;

	for(int block = 0; block < in_text.size()/8; block++){

		// Setting text for decoding.
		cout << "Setting text for decoding." << endl;
	
		cout << "block: " << block << endl;
		cout << "blteock: " << block << endl;

		// 8 chars, 8 bytes, 16 nibbles, 16 symbols.
	
		// Doing max 16 symbols per block.
		vector<sym_t> in_data(16);
		for(int d = 0; d < 16; d++){
			int index = block*8 + d/2;
			uint8_t c;
			// If no more text encode '\0'.
			if(index >= in_text.size()){
				c = 0;
				last_block = true;
			}else{
				c = in_text[index];
			}

			if(d%2){
				in_data[d] = c & 0x0f;
			}else{
				in_data[d] = c >> 4;
			}
		}
	
		cout << "in_data:" << endl;
		for(int d = 0; d < 16; d++){
			cout << setw(2) << in_data[d] << endl;
		}
		cout << endl << endl;
	
	
		// Histogram.
		cout << "Histogram." << endl;
	
		vector<cnt_t> histogram(16);
	
		for(int d = 0; d < 16; d++){
			histogram[in_data[d]]++;
		}
	
		cout << "histogram:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " << setw(2) << histogram[sym] << endl;
		}
		cout << endl << endl;
	

	
		// Prepare for sorting.
		cout << "Prepare for sorting." << endl;

		class sym_and_cnt {
		public:
			sym_t sym;
			cnt_t cnt;
		};
	
		vector<sym_and_cnt> sort_cnt(16);
		for(int sym = 0; sym < 16; sym++){
			sort_cnt[sym].sym = sym;
			// If count is 0 put it to invalid value 31,
			// which is also greater than 16,
			// to be sorted at the end of array.
			if(histogram[sym] == 0){
				sort_cnt[sym].cnt = 31;
			}else{
				sort_cnt[sym].cnt = histogram[sym];
			}
		}

		cout << "sort_cnt:" << endl;
		for(int i = 0; i < 16; i++){
			cout << setw(2) << sort_cnt[i].sym << ": " 
				<< setw(2) << sort_cnt[i].cnt << endl;
		}
		cout << endl << endl;


	
		// Sorting.
		cout << "Sorting." << endl;


		sort(
			sort_cnt.begin(),
			sort_cnt.end(),
			[](const sym_and_cnt& x, const sym_and_cnt& y){ 
				return x.cnt < y.cnt;
			}
		);
	
		cout << "sort_cnt:" << endl;
		for(int i = 0; i < 16; i++){
			cout << setw(2) << sort_cnt[i].sym << ": " 
				<< setw(2) << sort_cnt[i].cnt << endl;
		}
		cout << endl << endl;
	

		// Create leaves.
		cout << "Create leaves." << endl;
	
		// System have max 16 leaves and max 15 parent nodes,
		// so node 31 will never exists and it is null-object.
		typedef uint16_t node_t; // 5-bit.
	
		class node_and_cnt {
		public:
			node_t node;
			cnt_t cnt;
		
			// Null object.
			node_and_cnt()
				: node(31), cnt(31) {}
			
			node_and_cnt(node_t node, cnt_t cnt)
				: node(node), cnt(cnt) {}
			
			node_and_cnt(const sym_and_cnt& s)
				: node(s.sym), cnt(s.cnt) {}		
		
			bool is_null() const {
				return cnt == 31;
			}
		};
	
		deque<node_and_cnt> leaves(16);
		// Copy sorted symbols to leaf nodes.
		// Symbols now becomes IDs leaves.
		for(int i = 0; i < 16; i++){
			leaves[i] = sort_cnt[i];
		}

		cout << "leaves:" << endl;
		for(int i = 0; i < 16; i++){
			cout << setw(2) << leaves[i].node << ": " 
				<< setw(2) << leaves[i].cnt << endl;
		}
		cout << endl << endl;




		// Quasi-tree.
		cout << "Quasi-tree." << endl;

		deque<node_and_cnt> parents(16);
		int parents_end = 0;

		// TODO Solve problem when only one leaf exists.
	
		// Parent have nodes IDs larger than leaves IDs.
		node_t new_parents_node = 16;
	
		// Used for making canonical Huffman tree.
		class node_and_dep {
		public:
			node_t node;
			dep_t dep;		
		};
		vector<node_and_dep> depth_tracker(16);
		for(int sym = 0; sym < 16; sym++){
			depth_tracker[sym].node = sym;
			depth_tracker[sym].dep = 0;
		}
	
		// Could be done in 15 iterations, because there is max 15 parents.
		for(int iter = 0; iter < 15; iter++){
			// When only one node and no more leaves then tree is built.
			if(leaves[0].is_null() && parents[1].is_null()){
				assert(!parents[0].is_null());
				break;
			}
		
			assert(new_parents_node < 31);
		
			// Cannot both be empty at the same time.
			assert(!(leaves[0].is_null() && parents[0].is_null()));

			node_and_cnt child0;
			node_and_cnt child1;
		
			if(leaves[0].cnt < parents[0].cnt){
				// First leaf.
				child0 = leaves[0];
				
				if(leaves[1].cnt < parents[0].cnt){
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
			
				if(parents[1].cnt < leaves[0].cnt){
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
		
			// There is always two childs.
			assert(!child0.is_null() && !child1.is_null());
		
			// Create parent.
			node_and_cnt new_parent(new_parents_node, child0.cnt + child1.cnt);
			// Ready for new parent.
			new_parents_node++;
		
			// Insert parent.
			parents[parents_end++] = new_parent;
		
			// Increment symbols depth.
			for(int sym = 0; sym < 16; sym++){
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
			for(int i = 0; i < 16; i++){
				cout << setw(2) << leaves[i].node << ": " 
					<< setw(2) << leaves[i].cnt << endl;
			}
			cout << "parents:" << endl;
			for(int i = 0; i < 16; i++){
				cout << setw(2) << parents[i].node << ": " 
					<< setw(2) << parents[i].cnt << endl;
			}
			cout << "parents_end: " << parents_end << endl;
			cout << "depth_tracker:" << endl;
			for(int i = 0; i < 16; i++){
				cout << setw(2) << depth_tracker[i].node << ": " 
					<< setw(2) << depth_tracker[i].dep << endl;
			}
			cout << endl;
		
		}
	
		// No leafs and only one root parent node.
		assert(leaves[0].is_null());
		assert(!parents[0].is_null());
		assert(parents[1].is_null());
		cout << endl << endl;
	
	
	
		// Extract depth of symbols.
		cout << "Extract depth of symbols." << endl;
	
		vector<dep_t> symbols_depth(16);
		for(int sym = 0; sym < 16; sym++){
			symbols_depth[sym] = depth_tracker[sym].dep;
			assert(symbols_depth[sym] <= 5);
		}
	
		cout << "symbols_depth:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " 
				<< setw(2) << symbols_depth[sym] << endl;
		}
		cout << endl << endl;


	
		// Count same depths.
		cout << "Count same depths" << endl;

		vector<dep_cnt_t> depths_count(5, 0); // Init to zeros.

		for(int sym = 0; sym < 16; sym++){
			depths_count[symbols_depth[sym]]++;
		}
	
		cout << "depths_count:" << endl;
		for(int dep = 0; dep < 5; dep++){
			cout << setw(2) << dep << ": " 
				<< setw(2) << depths_count[dep] << endl;
		}
		cout << endl << endl;




		// Sorting symbols by depths.
		cout << "Sorting symbols by depths." << endl;
	
		class sym_and_dep{
		public:
			sym_t sym;
			dep_t dep;
		};

		vector<sym_and_dep> sort_dep(16);
		for(int sym = 0; sym < 16; sym++){
			sort_dep[sym].sym = sym;
			// If depth is 0 put it to invalid value 7,
			// which is also greater than 5,
			// to be sorted at the end of array.
			if(histogram[sym] == 0){
				sort_dep[sym].dep = 7;
			}else{
				sort_dep[sym].dep = symbols_depth[sym];
			}
		}

		sort(
			sort_dep.begin(),
			sort_dep.end(),
			[](const sym_and_dep& x, const sym_and_dep& y){
				return x.dep < y.dep;
			}
		);
	
		cout << "sort_dep:" << endl;
		for(int i = 0; i < 16; i++){
			cout << setw(2) << sort_dep[i].sym << ": " 
				<< setw(2) << sort_dep[i].dep << endl;
		}
		cout << endl << endl;


		// Creating canonical code table.
		cout << "Creating canonical code table." << endl;
	
		vector<dep_t> code_lens = symbols_depth;
		vector<code_t> code_table(16);
		code_t code = 0;
		for(int i = 0; i < 16; i++){
			sym_t sym = sort_dep[i].sym;
			dep_t dep = sort_dep[i].dep;
			// Assign current code for symbol's depth.
			code_table[sym] = code;
			// Increment to next code.
			code += 1 << (5 - dep);
		}
	
		cout << "code_lens:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " << setw(1) << code_lens[sym] << endl;
		}
		cout << "code_table:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " 
				<< bits_to_string(code_table[sym], 5, 5-code_lens[sym]) 
				<< endl;
		}
		cout << endl << endl;




		// Mirror codes for little endian encoding.
		cout << "Mirror codes for little endian encoding." << endl;

		for(int sym = 0; sym < 16; sym++){
			bitset<5> src(code_table[sym]);
			bitset<5> dst(src);
			dst[4] = src[0];
			dst[3] = src[1];
			dst[1] = src[3];
			dst[0] = src[4];
			code_table[sym] = dst.to_ulong();
		}

		cout << "code_table:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " 
				<< setw(5) << bits_to_string(code_table[sym], code_lens[sym], 0) 
				<< endl;
		}
		cout << endl << endl;




		// Encode data.
		cout << "Encode data." << endl;
	
		// In worst case, if all data symbols are different, 
		// all symbols will be coded with 4-bit codes,
		// times 16 symbols, that is max 64-bit output.
		typedef uint64_t enc_block_t;
		// Max length is 64.
		typedef uint16_t enc_len_t;// 7-bits.
		enc_block_t enc_block = 0;
		enc_len_t enc_len = 0;
	
		cout << "Encoding:" << endl;
		for(int d = 0; d < 16; d++){
			sym_t sym = in_data[d];
			code_t code = code_table[sym];
			dep_t code_len = code_lens[sym];

			// Strap 1s above code length.
			code &= (1 << code_len) - 1;

			// Add encoded symbol.
			enc_block |= enc_block_t(code) << enc_len;
			enc_len += code_len;


			cout << "iter " << d << ":" << endl;
			cout << "sym: " << setw(2) << sym << endl;
			cout << "code: " 
				<< setw(5) << bits_to_string(code, code_len, 0) << endl;
			cout << "enc_block: " 
				<< setw(64) << bits_to_string(enc_block, enc_len, 0) << endl;
			cout << "enc_len: " << enc_len << endl;
			cout << endl;

		}
		cout << "enc_block: " << setw(64) 
			<< bits_to_string(enc_block, enc_len) << endl;
		cout << "enc_len: " << enc_len << endl;
		cout << endl << endl;




		// Store canonical table.
		cout << "Store canonical table." << endl;

		uint32_t store_len = 0;

		// Store 4 depths count.
		for(int dep = 1; dep < 5; dep++){
			uint8_t b = depths_count[dep];
			acc |= b << acc_len;
			acc_len += 4; // dep_cnt_t is 4-bit.
			if(acc_len >= 8){
				enc_data.push_back(acc);
				acc_len -= 8;
				acc = b >> 4 - acc_len;
			}
			store_len += 4;
		}
		cout << "acc_len: " << acc_len << endl;

		// Store symbols.
		for(int i = 0; i < 16; i++){
			dep_t dep = sort_dep[i].dep;
			// Don't save symbols with invalid depth ie. symbols with count 0.
			if(dep != 7){
				uint8_t b = sort_dep[i].sym;
				acc |= b << acc_len;
				acc_len += 4; // sym_t is 4-bit.
				if(acc_len >= 8){
					enc_data.push_back(acc);
					acc_len -= 8;
					acc = b >> 4 - acc_len;
				}
				store_len += 4;
			}
		}

		cout << "stored_len: " << store_len << endl;
		cout << endl << endl;





		// Store encoded data.
		cout << "Store encoded data." << endl;

		while(enc_len > 0){
			uint8_t b = enc_block;
			acc |= b << acc_len;
			int len = 8 - acc_len;
			if(enc_len >= len){
				enc_data.push_back(acc);
				acc_len = 0;
				acc = 0;

				enc_block >>= len;
				enc_len -= len;

				store_len += len;
			}else{
				if(last_block){
					enc_data.push_back(acc);
				}
				acc_len += enc_len;
				assert(acc_len < 8);

				enc_block >>= enc_len; // Redudant.
				enc_len = 0; // Redudant.

				store_len += enc_len;
			}
		}

		cout << "stored_len: " << store_len << endl;
		cout << endl << endl;

	}

}

int main() {

	string in_text(TEXT);

	vector<uint8_t> enc_data;
	huffman_encode(
		in_text,
		enc_data
	);


/*
	


	
	


	// TODO Decode canonical table.


	// Decode data.
	cout << "Decode data." << endl;
	
	vector<sym_t> out_data(16);
	
	for(int d = 0; d < 16; d++){
		DEBUG(bitset<64>(enc_block));
		dep_t best_len = 7;
		code_t best_code;
		sym_t best_sym;
		for(int sym = 0; sym < 7; sym++){
			dep_t code_len = code_lens[sym];
			if(code_len != 0 && code_len < best_len){
				code_t mask = (1 << code_len) - 1;
				code_t enc_code = enc_block & mask;
				if(enc_code == code_table[sym]){
					best_len = code_len;
					best_code = code_table[sym];
					best_sym = sym;
				}
			}
		}
		assert(0 < best_len && best_len <= 5);
		
		// Remove decoded symbol.
		enc_block >>= best_len;
		enc_len -= best_len;
		
		out_data[d] = best_sym;
		
		cout << "iter " << d << ":" << endl;
		cout << "best_len: " << setw(2) << best_len << endl;
		cout << "best_code: "
			<< setw(5) << bits_to_string(code, best_len, 0) << endl;
		cout << "best_sym: " << setw(2) << best_sym << endl;
		cout << "enc_block: " 
			<< setw(64) << bits_to_string(enc_block, enc_len, 0) << endl;
		cout << "enc_len: " << enc_len << endl;
		cout << endl;
	}
	assert(enc_len == 0);
	
	cout << "out_data:" << endl;
	for(int d = 0; d < 16; d++){
		cout << setw(2) << out_data[d] << endl;
	}
	cout << endl << endl;




	// Comparing input and output.
	cout << "Comparing input and output." << endl;
	
	for(int i = 0; i < 16; i++){
		if(in_data[i] != out_data[i]){
			cerr << "Mismatch symbol " << i << endl;
			return 1;
		}
	}
	cout << endl << endl;
*/


	cout << "End." << endl;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
