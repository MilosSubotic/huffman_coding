/*
 * @license MIT
 * @brief Huffman codec, encoder and decoder.
 */
 
//////////////////////////////////////////////////////////////////////////////
// Config.

#define BITS

#define LOG_TO_FILE "huffman_codec.log"

//////////////////////////////////////////////////////////////////////////////

#include "huffman_codec.h"

#include <vector>

#if LOG_TO_FILE
#include <fstream>
#endif

//////////////////////////////////////////////////////////////////////////////

int huffman_encode(
	const uint8_t* data,
	size_t data_len,
	uint8_t* compressed,
	size_t compressed_max_len
) {
	if(!data || !compressed){
		return 1;
	}

	vector<uint32_t> histogram(256);
	
	for(int i = 0; i < data_len; i++){
		histogram[data[i]]++;
	}
	
#if LOG_TO_FILE
	ofstream f(LOG_TO_FILE, ios::write);
	if(!f){
		return 2;
	}
	
	f << "Histogram:" << endl;
	for(int i = 0; i
	
#endif

	return 0;
}

int huffman_decode(
	const uint8_t* compressed,
	size_t compressed_len,
	uint8_t* data,
	size_t data_max_len
) {
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
