/**
 * @license MIT
 * @brief
 */


#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

///////////////////////////////////////////////////////////////////////////////

#include "axis_channel.h"

#include "huffman_coding_public_types.h"

///////////////////////////////////////////////////////////////////////////////

namespace huffman_coding {

	class huffman_decoder : sc_module {
	public:
		axis_in<enc_chunk_t> in_enc_data;
		axis_out<sym_t> out_data;

	    SC_CTOR(huffman_decoder) {
	        SC_THREAD(decode);
	    }

	private:
	    void decode();

	};
}

///////////////////////////////////////////////////////////////////////////////

#endif // HUFFMAN_DECODER_H

