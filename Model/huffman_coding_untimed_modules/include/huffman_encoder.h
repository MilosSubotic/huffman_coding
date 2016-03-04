/**
 * @license MIT
 * @brief
 */


#ifndef HUFFMAN_ENCODER_H_
#define HUFFMAN_ENCODER_H_

///////////////////////////////////////////////////////////////////////////////

#include "axis_channel.h"

#include "huffman_coding_public_types.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	class huffman_encoder : sc_module {
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

#endif // HUFFMAN_ENCODER_H_
