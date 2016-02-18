/*
 * @license MIT
 * @brief Test Huffman codec.
 */
 
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

using namespace std;

#include "huffman_codec.h"

//////////////////////////////////////////////////////////////////////////////

int main() {
	ifstream f("The King's Breakfast.txt");
	if(!f){
		cerr << "Cannot open file!" << endl;
		return 1;
	}
	
	vector<uint8_t> d_in;

	f.seekg(0, ios::end);
	d_in.reserve(f.tellg());
	f.seekg(0, ios::beg);

	d_in.assign(
		istreambuf_iterator<char>(f),
		istreambuf_iterator<char>()
	);
	
	f.close();

	
	size_t len = d_in.size();
	vector<uint8_t> compressed(len*10);
	vector<uint8_t> d_out(len);

	int rc = huffman_encode(d_in.data(), len, compressed.data(), len*10);
	if(rc){
		cerr << "Error while encoding! rc: " << rc << endl;
		return 1;
	}
	
	rc = huffman_decode(compressed.data(), len*10, d_out.data(), len);
	if(rc){
		cerr << "Error while decoding! rc: " << rc << endl;
		return 1;
	}

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
