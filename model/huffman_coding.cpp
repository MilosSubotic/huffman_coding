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

#define TEXT "babadeda"
//#define TEXT "deadbeef"
//#define TEXT "babadedadeadbeef"

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
// Because we decode max 16 symbols in block,
// then count could be max 16.
typedef uint16_t cnt_t; // 5-bits.
// Max depth of leaf in Huffman tree is 5.
// This is also max bit-length of code.
typedef uint16_t dep_t; // 3-bit.
typedef uint16_t len_t; // 3-bit.
// Couldn't have more than 16 bit-lengths of same count.
// Practically even less. 
// TODO Calculate how much.
typedef uint16_t len_cnt_t; // 4-bit. FIXME Maybe 5-bit.
typedef int16_t code_t; // 5-bits.


void huffman_encode(
	const string& in_text,
	vector<uint8_t>& enc_data
) {

	cout << "Encoding..." << endl << endl;

	uint16_t acc = 0;
	int acc_len = 0;
	auto pack = [&](uint16_t bits, unsigned len) {
		assert(len <= 8);
		acc |= bits << acc_len;
		acc_len += len;
		if(acc_len >= 8){
			enc_data.push_back(acc & 0xff);
			acc >>= 8;
			acc_len -= 8;
		}
	};

	for(int block = 0; block < in_text.size()/8; block++){


		cout << "Setting up data for decoding..." << endl;
	
		cout << "block: " << block << endl;

		// 8 chars, 8 bytes, 16 nibbles, 16 symbols.
	
		// Doing max 16 symbols per block.
		vector<sym_t> in_data(16);
		for(int d = 0; d < 16; d++){
			int index = block*8 + d/2;
			uint8_t c;
			// If no more text then encode '\0' up to block size.
			if(index >= in_text.size()){
				c = 0;
			}else{
				c = in_text[index];
			}

			if(d%2 == 0){
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




		cout << "Creating histogram..." << endl;
	
		vector<cnt_t> histogram(16);
	
		for(int d = 0; d < 16; d++){
			histogram[in_data[d]]++;
		}
	
		cout << "histogram:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " << setw(2) << histogram[sym] << endl;
		}
		cout << endl << endl;
	

	
		cout << "Preparing for sorting symbols by count..." << endl;

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


	
		cout << "Sorting symbols by count..." << endl;

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
	



		cout << "Creating leaves..." << endl;
	
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




		cout << "Tracking depth of leaves on quasi-tree..." << endl;

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
	
	
	

		cout << "Extracting bit-lengths of codes for symbols..." << endl;
	
		vector<len_t> codes_len(16);
		for(int sym = 0; sym < 16; sym++){
			codes_len[sym] = depth_tracker[sym].dep;
			assert(codes_len[sym] <= 5);
		}
	
		cout << "codes_len:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " 
				<< setw(2) << codes_len[sym] << endl;
		}
		cout << endl << endl;


	
		cout << "Counting same bit-lengths..." << endl;

		vector<len_cnt_t> lens_cnt(5, 0); // Init to zeros.

		for(int sym = 0; sym < 16; sym++){
			lens_cnt[codes_len[sym]]++;
		}
	
		cout << "lens_cnt:" << endl;
		for(int len = 0; len < 5; len++){
			cout << setw(2) << len << ": " 
				<< setw(2) << lens_cnt[len] << endl;
		}
		cout << endl << endl;




		cout << "Preparing for sorting symbols by bit-lengths..." << endl;
	
		class sym_and_len{
		public:
			sym_t sym;
			len_t len;
		};

		vector<sym_and_len> sort_len(16);
		for(int sym = 0; sym < 16; sym++){
			sort_len[sym].sym = sym;
			// If length is 0 put it to invalid value 7,
			// which is also greater than 5,
			// to be sorted at the end of array.
			if(histogram[sym] == 0){
				sort_len[sym].len = 7;
			}else{
				sort_len[sym].len = codes_len[sym];
			}
		}

		cout << "sort_len:" << endl;
		for(int i = 0; i < 16; i++){
			cout << setw(2) << sort_len[i].sym << ": " 
				<< setw(2) << sort_len[i].len << endl;
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
		for(int i = 0; i < 16; i++){
			cout << setw(2) << sort_len[i].sym << ": " 
				<< setw(2) << sort_len[i].len << endl;
		}
		cout << endl << endl;




		cout << "Creating canonical code table..." << endl;
	
		vector<code_t> code_table(16);
		code_t code = 0;
		for(int i = 0; i < 16; i++){
			sym_t sym = sort_len[i].sym;
			len_t len = sort_len[i].len;
			// Assign current code for symbol's length.
			code_table[sym] = code;
			// Increment to next code.
			code += 1 << (5 - len);
		}
	
		cout << "codes_len:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " 
				<< setw(1) << codes_len[sym] << endl;
		}
		cout << "code_table:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " 
				<< bits_to_string(code_table[sym], 5, 5-codes_len[sym]) 
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
			cout << setw(2) << sym << ": " << setw(5) 
				<< bits_to_string(code_table[sym], codes_len[sym], 0) 
				<< endl;
		}
		cout << endl << endl;



		cout << "Packing canonical table..." << endl;

		uint32_t store_len = 0;

		// Store 4 bit-lengths count.
		for(int len = 1; len < 5; len++){
			pack(lens_cnt[len], 4); // len_cnt_t is 4-bit.

			store_len += 4;
		}

		// Store symbols.
		for(int i = 0; i < 16; i++){
			len_t len = sort_len[i].len;
			// Don't save symbols with invalid length ie. symbols with count 0.
			if(len != 7){
				pack(sort_len[i].sym, 4); // sym_t is 4-bit.

				store_len += 4;
			}
		}

		cout << "stored_len: " << store_len << endl;
		cout << endl << endl;




		cout << "Encoding and packing data..." << endl;
	
		cout << "Encoding:" << endl;
		for(int d = 0; d < 16; d++){
			sym_t sym = in_data[d];
			code_t code = code_table[sym];
			len_t code_len = codes_len[sym];

			// Strap 1s above code length.
			code &= (1 << code_len) - 1; // TODO Isn't this redudant a little.

			pack(code, code_len);

			store_len += code_len;

			cout << "iter " << d << ":" << endl;
			cout << "sym: " << setw(2) << sym << endl;
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
		enc_data.push_back(acc);
	}

}

void huffman_decode(
	const vector<uint8_t>& enc_data,
	string& out_text
) {

	cout << "Decoding..." << endl << endl;

	uint16_t acc = 0;
	unsigned acc_len = 0;
	auto ed = enc_data.begin();
	auto unpack = [&](unsigned len) -> uint8_t {
		assert(len <= 8);
		uint8_t ret;
		if(acc_len < len){
			acc |= (*ed++) << acc_len;
			acc_len += 8;
		}
		ret = acc & ((1 << len) - 1);
		acc >>= len;
		acc_len -= len;
		return ret;
	};
	

	while(ed < enc_data.end()){

		cout << "Unpacking bit-lengths count..." << endl;

		vector<len_cnt_t> lens_cnt(5);

		for(int len = 1; len < 5; len++){
			lens_cnt[len] = unpack(4); // len_cnt_t is 4-bit.
		}

		cout << "lens_cnt:" << endl;
		for(int len = 0; len < 5; len++){
			cout << setw(2) << len << ": " 
				<< setw(2) << lens_cnt[len] << endl;
		}
		cout << endl << endl;




		cout << "Unpacking symbols and creating canonical code table..."
			<< endl;
		
		vector<len_t> codes_len(16, 0);
		vector<code_t> code_table(16);

		code_t code = 0;
		for(len_t len = 1; len < 5; len++){
			for(int cnt = lens_cnt[len]; cnt > 0; cnt--){
				sym_t sym = unpack(4); // sym_t is 4-bit.
				code_table[sym] = code;
				codes_len[sym] = len;
				// Increment to next code.
				code += 1 << (5 - len);
			}
		}
	
		cout << "codes_len:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " 
				<< setw(1) << codes_len[sym] << endl;
		}
		cout << "code_table:" << endl;
		for(int sym = 0; sym < 16; sym++){
			cout << setw(2) << sym << ": " 
				<< bits_to_string(code_table[sym], 5, 5-codes_len[sym]) 
				<< endl;
		}
		cout << endl << endl;




		cout << "Mirroring codes for little endian decoding..." << endl;

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
			cout << setw(2) << sym << ": " << setw(5) 
				<< bits_to_string(code_table[sym], codes_len[sym], 0) 
				<< endl;
		}
		cout << endl << endl;




		cout << "Decoding data..." << endl;

		vector<sym_t> out_data(16);

		for(int d = 0; d < 16; d++){
			if(acc_len < 5){
				acc |= (*ed++) << acc_len;
				acc_len += 8;
			}

			len_t best_len = 7;
			code_t best_code;
			sym_t best_sym;
			for(int sym = 0; sym < 16; sym++){
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
			assert(0 < best_len && best_len <= 5);
		
			// Remove decoded symbol.
			acc >>=  best_len;
			acc_len -=  best_len;
		
			out_data[d] = best_sym;
		
			cout << "iter " << d << ":" << endl;
			cout << "best_len: " << setw(2) << best_len << endl;
			cout << "best_code: "
				<< setw(5) << bits_to_string(code, best_len, 0) << endl;
			cout << "best_sym: " << setw(2) << best_sym << endl;
			cout << endl;
		}
	
		cout << "out_data:" << endl;
		for(int d = 0; d < 16; d++){
			cout << setw(2) << out_data[d] << endl;
		}
		cout << endl << endl;




		cout << "Converting data to text..." << endl;
		for(int t = 0; t < 8; t++){
			uint8_t c = (out_data[t*2+1] << 4) | out_data[t*2];
			out_text += c;
		}
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



	
	cout << "compression ratio: " 
		<< enc_data.size()/float(in_text.size()) << endl << endl;



	string out_text;

	huffman_decode(
		enc_data,
		out_text
	);





	cout << "Comparing input and output..." << endl;
	if(in_text != out_text){
		cout << "Mismatch!" << endl;
		for(int i = 0; i < in_text.size(); i++){
			if(in_text[i] != out_text[i]){
				cout << "Mismatch at pos " << i << endl;
				break;
			}
		}
	}else{
		cout << "Match!" << endl;
	}
	cout << endl << endl;




	cout << "End." << endl;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
