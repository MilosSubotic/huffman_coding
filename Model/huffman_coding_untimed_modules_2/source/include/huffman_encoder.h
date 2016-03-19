/**
 * @license MIT
 * @brief
 */


#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

///////////////////////////////////////////////////////////////////////////////

#include "axis_channel.h"

#include "huffman_coding_public_types.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	SC_MODULE(huffman_encoder) {
	public:
		axis_in<sym_t> in_data;
		axis_out<enc_chunk_t> out_enc_data;

	    SC_CTOR(huffman_encoder) {
	        SC_THREAD(encode);
	    }

	private:
	    void encode();

	};

}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_ENCODER_H

