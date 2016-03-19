/**
 * @license MIT
 * @brief
 */


#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

///////////////////////////////////////////////////////////////////////////////

#include <vector>

#include "axis_channel.h"

#include "huffman_coding_public_types.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	SC_MODULE(huffman_decoder) {
	public:
		axis_in<enc_chunk_t> in_enc_data;
		axis_out<sym_t> out_data;

	    SC_CTOR(huffman_decoder) {
	        SC_THREAD(decode);
	        init();
	    }

	private:

	    void init();
	    void decode();

	};
}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_DECODER_H

