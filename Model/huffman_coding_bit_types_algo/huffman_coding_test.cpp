/**
 * @license MIT
 * @brief Test Huffman coding.
 */

///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;

#include <systemc.h>

#include "huffman_coding.h"

///////////////////////////////////////////////////////////////////////////////

#define DEBUG(var) \
	do{ \
		cout << #var << " = " << var << endl; \
	}while(0)

#define DATA_LEN block_len*2

///////////////////////////////////////////////////////////////////////////////

int sc_main(int, char**){

	using namespace huffman_coding;

	unsigned seed = time(NULL);
	cout << "seed: " << seed << endl;
	srand(seed);

	vector<sym_t> in_data(DATA_LEN);
	for(int i = 0; i < DATA_LEN; i++){
		in_data[i] = rand() % (1 << sym_width);
	}

	vector<enc_chunk_t> enc_data;
	huffman_encode(in_data, enc_data);

	cout << "enc_data size: " << enc_data.size() << endl;
	cout << "enc_data:" << endl;
	cout << hex << setfill('0');
	for(auto iter = enc_data.begin(); iter != enc_data.end(); iter++){
		cout << "0x" << setw(8) << *iter << endl;
	}
	cout << dec << setfill(' ') << endl << endl;

	vector<sym_t> out_data;
	huffman_decode(enc_data, out_data);

	cout << "Comparing input and output..." << endl;
	cout << "in_data size: " << in_data.size() << endl;
	cout << "out_data size: " << out_data.size() << endl;
	if(in_data != out_data){
		cout << endl << "Mismatch!!!" << endl;
		cerr << endl << "Mismatch!!!" << endl;
		for(int i = 0; i < in_data.size(); i++){
			if(in_data[i] != out_data[i]){
				cout << "Mismatch at pos " << i << endl;
				break;
			}
		}
		return 1;
	}else{
		cout << "Match!" << endl;
	}
	cout << endl << endl;

	cout << "End." << endl;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
