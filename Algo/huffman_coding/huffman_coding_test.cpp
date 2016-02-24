/*
 * @license MIT
 * @brief Test Huffman coding.
 */

//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

#include "huffman_coding.h"

//////////////////////////////////////////////////////////////////////////////

#define DEBUG(var) \
	do{ \
		cout << #var << " = " << var << endl; \
	}while(0)

#define TEXT "babadedadeadbeef"

//////////////////////////////////////////////////////////////////////////////

int main() {

	using namespace huffman_coding;

/*
	vector<sym_t> in_data(strlen(TEXT));
	for(int i = 0; i < strlen(TEXT); i++){
		in_data[i] = TEXT[i];
	}
*/

	string in_text(TEXT);
	vector<sym_t> in_data(in_text.size()*2);
	for(int d = 0; d < in_text.size()*2; d++){
		int index = d/2;
		uint8_t c = in_text[index];
		if(d%2 == 0){
			in_data[d] = c & 0x0f;
		}else{
			in_data[d] = c >> 4;
		}
	}

	vector<uint8_t> enc_data;
	huffman_encode(in_data, enc_data);

	cout << "enc_data:" << endl;
	cout << hex << setfill('0');
	for(auto iter = enc_data.begin(); iter != enc_data.end(); iter++){
		cout << "0x" << setw(2) << uint16_t(*iter) << endl;
	}
	cout << dec << setfill(' ') << endl << endl;

	vector<sym_t> out_data;
	huffman_decode(enc_data, out_data);

	string out_text;
	for(int t = 0; t < out_data.size()/2; t++){
		uint8_t c = (out_data[t*2+1] << 4) | out_data[t*2];
		out_text += c;
	}

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
