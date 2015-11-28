/*
 * @license MIT
 * @brief Test Huffman codec.
 */
 
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <vector>
#include <deque>
#include <algorithm>
#include <cassert>
#include <bitset>

using namespace std;

//////////////////////////////////////////////////////////////////////////////

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

string bits_to_string(uint64_t bits, unsigned len) {
	ostringstream oss;
	bitset<64> bf(bits);
	for(int i = len-1; i >= 0; i--){
		oss << bf[i];
	}
	return oss.str();
}

int main() {

	// Setting text for decoding.
	
	cout << "TEXT: " << TEXT << endl;

	// 8 chars, 8 bytes, 16 nibbles, 16 symbols.
	assert(strlen(TEXT) == 8);
	
	typedef uint16_t sym_t; // 4-bits.
	// Doing max 16 symbols per block.
	vector<sym_t> in_data(16);
	for(int d = 0; d < 16; d++){
		if(d%2){
			in_data[d] = TEXT[d/2] & 0x0f;
		}else{
			in_data[d] = TEXT[d/2] >> 4;
		}
	}
	
	cout << "in_data:" << endl;
	for(int d = 0; d < 16; d++){
		cout << setw(2) << in_data[d] << endl;
	}
	cout << endl << endl;
	
	
	// Histogram.
	
	// Because we decode max 16 symbols in block,
	// then count could be max 16.
	typedef uint16_t cnt_t; // 5-bits.
	
	vector<cnt_t> histogram(16);
	
	// TODO Don't use i over the place, be smarter.
	
	for(int d = 0; d < 16; d++){
		histogram[in_data[d]]++;
	}
	
	cout << "Histogram:" << endl;
	for(int sym = 0; sym < 16; sym++){
		cout << setw(2) << sym << ": " << setw(2) << histogram[sym] << endl;
	}
	cout << endl << endl;
	
	
	
	// Sorting.
	class sym_and_cnt {
	public:
		sym_t sym;
		cnt_t cnt;
	};
	
	vector<sym_and_cnt> sort_vec(16);
	for(int sym = 0; sym < 16; sym++){
		sort_vec[sym].sym = sym;
		sort_vec[sym].cnt = histogram[sym];
	}
#if 1
	sort(
		sort_vec.begin(),
		sort_vec.end(),
		[](sym_and_cnt x, sym_and_cnt y){ return x.cnt < y.cnt; }
	);
#else
	// Bubble sort.
	// TODO
	for(int i = 0; i < 16; i++){
		if(i%2){ // Odd pairs.
		}else{ // Even pairs.
		}
	}
#endif
	
	cout << "Sorted:" << endl;
	for(int i = 0; i < 16; i++){
		cout << setw(2) << sort_vec[i].sym << ": " 
			<< setw(2) << sort_vec[i].cnt << endl;
	}
	cout << endl << endl;
	

	// Remove empty leaves.
	
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
		leaves[i] = sort_vec[i];
	}
	
	for(int i = 0; i < 16; i++){
		if(leaves[0].cnt == 0){
			shift(leaves);
		}
	}

	cout << "Leafs:" << endl;
	for(int i = 0; i < 16; i++){
		cout << setw(2) << leaves[i].node << ": " 
			<< setw(2) << leaves[i].cnt << endl;
	}
	cout << endl << endl;
	

	// Quasi-tree.
	cout << "Quasi-tree:" << endl;

	deque<node_and_cnt> parents(16);
	int parents_end = 0;

	// TODO Solve problem when only one leaf exists.
	
	// Parent have nodes IDs larger than leaves IDs.
	node_t new_parents_node = 16;
	
	// Used for making canonical Huffman tree.
	// Maximum depth of leaf in Huffman tree is 5.
	typedef uint16_t dep_t; // 3-bit.
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
	
	// Could have more than 16 depth of same count.
	// Practically even less. 
	// TODO Calculate how much.
	typedef uint16_t dep_cnt_t; // 4-bit.
	vector<dep_cnt_t> depths_count(5, 0); // Init to zeros.

	for(int sym = 0; sym < 16; sym++){
		depths_count[symbols_depth[sym]]++;
	}
	
	cout << "depths_count:" << endl;
	for(int dep = 0; dep < 5; dep++){
		cout << setw(2) << dep << ": " << setw(2) << depths_count[dep] << endl;
	}
	cout << endl << endl;
	
	// Calculate start codes for depths.
	
	// Because max depths is 5, max bits in symbols is also 5.
	typedef int16_t code_t; // 5-bits.
	vector<code_t> start_codes(5);
	
	code_t code = -1; // All 1s.
	// Last non-zero depth when code was decremented.
	dep_t last_nz_dep = 0;
	for(int dep = 1; dep < 5; dep++){
		if(depths_count[dep] != 0){
			start_codes[dep] = code;
			code -= depths_count[dep] << last_nz_dep;
			last_nz_dep = dep;
		}
	}
	
	cout << "start_codes:" << endl;
	for(int dep = 0; dep < 5; dep++){
		cout << setw(2) << dep << ": " 
			<< setw(5) << bits_to_string(start_codes[dep], dep) << endl;
	}
	cout << endl << endl;
	
return 0;

	// Creating canonical code table.
	
	vector<dep_t> code_lens = symbols_depth;
	vector<code_t> dep_codes = start_codes;
	vector<code_t> code_table(16);
	for(int sym = 0; sym < 6; sym++){
		dep_t dep = symbols_depth[sym];
		// Assign current code for symbol's depth.
		code_table[sym] = dep_codes[dep]; 
		// Decrement to next code.
		dep_codes[dep] -= 1;
	}
	
	cout << "code_lens:" << endl;
	for(int sym = 0; sym < 16; sym++){
		cout << setw(2) << sym << ": " << setw(1) << code_lens[sym] << endl;
	}
	cout << "code_table:" << endl;
	for(int sym = 0; sym < 16; sym++){
		cout << setw(2) << sym << ": " 
			<< setw(5) << bits_to_string(code_table[sym], code_lens[sym]) 
			<< endl;
	}
	cout << endl << endl;
	
return 0;	

	
	// Encode data.
	
	// In worst case, if all data symbols are different, 
	// all symbols will be coded with 4-bit codes,
	// times 16 symbols, that is max 64-bit output.
	typedef uint64_t enc_block_t;
	// Max length is 64.
	typedef uint16_t enc_len_t;// 7-bits.
	enc_block_t enc_data = 0;
	enc_len_t enc_len = 0;
	
	cout << "Encoding:" << endl;
	for(int d = 0; d < 16; d++){
		sym_t sym = in_data[d];
		code_t code = code_table[sym];
		dep_t code_len = code_lens[sym];

		// Strap 1s above code length.
		code &= (1 << code_len) - 1;

		// Add encoded symbol.
		enc_data |= enc_block_t(code) << enc_len;
		enc_len += code_len;


		cout << "iter " << d << ":" << endl;
		cout << "sym: " << setw(2) << sym << endl;
		cout << "code: " << setw(5) << bits_to_string(code, code_len) << endl;
		cout << "enc_data: " << setw(64) 
			<< bits_to_string(enc_data, enc_len) << endl;
		cout << "enc_len: " << enc_len << endl;
		cout << endl;

	}
	cout << "enc_data: " << setw(64) 
		<< bits_to_string(enc_data, enc_len) << endl;
	cout << "enc_len: " << enc_len << endl;
	cout << endl << endl;
	



	// TODO Encode canonical table.
	



	// Decode data.
	
	// TODO Decode canonical table.
	
	vector<sym_t> out_data(16);
	
	for(int d = 0; d < 12; d++){
		DEBUG(bitset<64>(enc_data));
		dep_t best_len = 7;
		code_t best_code;
		sym_t best_sym;
		for(int sym = 0; sym < 7; sym++){
			dep_t code_len = code_lens[sym];
			if(code_len != 0 && code_len < best_len){
				code_t mask = (1 << code_len) - 1;
				code_t enc_code = enc_data & mask;
				if(enc_code == (code_table[sym] & mask)){
					best_len = code_len;
					best_code = code_table[sym];
					best_sym = sym;
				}
			}
		}
		assert(0 < best_len && best_len <= 5);
		
		// Remove decoded symbol.
		enc_data >>= best_len;
		enc_len -= best_len;
		
		out_data[d] = best_sym;
		
		cout << "iter " << d << ":" << endl;
		cout << "best_len: " << setw(2) << best_len << endl;
		cout << "best_code: " << setw(5) 
			<< bits_to_string(best_code, best_len) << endl;
		cout << "best_sym: " << setw(2) << best_sym << endl;
		cout << "enc_data: " << setw(64) 
			<< bits_to_string(enc_data, enc_len) << endl;
		cout << "enc_len: " << enc_len << endl;
		cout << endl;
	}
	//assert(enc_len != 0);
	
	cout << "out_data:" << endl;
	for(int d = 0; d < 16; d++){
		cout << setw(5) << out_data[d] << endl;
	}
	cout << endl << endl;
	
#if 0
	for(int i = 0; i < len; i++){
		if(d_in[i] != d_out[i]){
			cerr << "Mismatch after encoding and decoding at byte index " << i << endl;
			return 1;
		}
	}
#endif

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
